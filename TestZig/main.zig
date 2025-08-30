const std = @import("std");

const sea = @cImport({
    @cInclude("Allocator.h");
    @cInclude("Arena.h");
    @cInclude("Error.h");
    @cInclude("Iterator.h");
    @cInclude("JSONArray.h");
    @cInclude("JSONObject.h");
    @cInclude("JSONParser.h");
    @cInclude("JSONType.h");
    @cInclude("JSONValue.h");
    @cInclude("ListDyn.h");
    @cInclude("ListChunked.h");
    @cInclude("ListSegmented.h");
    @cInclude("Random.h");
    @cInclude("Time.h");
    @cInclude("UUID.h");
});

pub fn uuidToHex(uuid: []const u8, allocator: *std.mem.Allocator) ![]u8 {
    const buf = try allocator.alloc(u8, 16*2);
    var index: usize = 0;
    for (uuid) |b| {
        const hex = std.fmt.hex(b);
        buf[index] = hex[0];
        buf[index + 1] = hex[1];
        index += 2;
    }
    return buf;
}

pub fn Iterator(comptime ElementType: type) type {
    return struct {
        pub const Element = ElementType;
        raw: ?*sea.SEA_Iterator,

        pub fn init(raw: *sea.SEA_Iterator) @This() {
            return .{
                .raw = raw,
            };
        }

        pub fn next(self: @This()) ?*ElementType {
            const raw = self.raw orelse return null;
            const next_fn = raw.next orelse return null;
            const ptr = next_fn(raw);
            if (ptr == null) return null;
            return @alignCast(@ptrCast(ptr));
        }

        pub fn hasNext(self: * const @This()) bool {
            const raw = self.raw orelse return false;
            const has_next_fn = raw.hasNext orelse return false;
            return has_next_fn(raw);
        }

        pub fn destroy(self: *@This()) void {
            const raw = self.raw orelse return;
            const destroy_fn = raw.destroy orelse return;
            destroy_fn(raw);
            self.raw = null;
        }

        pub fn forEach(self: *@This(), body: fn (*ElementType) void) void {
            defer self.destroy();
            while (self.hasNext()) {
                if (self.next()) |value| {
                    body(value);
                } else {
                    break;
                }
            }
        }

        pub fn toSlice(self: *@This()) []ElementType {
            var values = std.ArrayList(ElementType).init(std.heap.page_allocator);
            defer values.deinit();
            while (self.hasNext()) {
                if (self.next()) |value| {
                    _ = values.append(value.*);
                } else {
                    break;
                }
            }
            return values.toOwnedSlice();
        }
    };
}

pub fn ListDyn(comptime ElementType: type) type {
    return struct {
        // =====================
        // Comptime
        pub const Element = ElementType;

        // =====================
        // Members
        raw: sea.SEA_ListDyn,

        // =====================
        // Public
        pub fn init(allocator: *sea.SEA_Allocator) @This() {
            return .{
                .raw = sea.SEA_ListDyn{
                    .allocator = allocator,
                    .elementSize = @sizeOf(ElementType),
                    .count = 0,
                    .capacity = 16,
                    .data = sea.SEA_Allocator_alloc(
                        allocator,
                        @sizeOf(ElementType) * 16,
                    ),
                },
            };
        }

        pub fn deinit(self: *@This()) void {
            if (self.raw.data == null) return;
            sea.SEA_ListDyn_free(&self.raw);
            self.raw.data = null;
            self.raw.count = 0;
            self.raw.capacity = 0;
            self.raw.allocator = null;
            self.raw.elementSize = 0;
        }

        pub fn add(self: *@This(), value: ElementType) void {
            if (self.raw.data == null) return;
            _ = sea.SEA_ListDyn_add(&self.raw, @ptrCast(&value));
        }

        pub fn addAll(self: *@This(), values: []const ElementType) void {
            for (values) |v| {
                self.add(v);
            }
        }

        pub fn iterator(self: *@This()) Iterator(ElementType) {
            return Iterator(ElementType).init(
                sea.SEA_ListDyn_iterator(&self.raw, null)
            );
        }

        pub fn get(self: *@This(), index: usize) ?*ElementType {
            return @alignCast(@ptrCast(sea.SEA_ListDyn_get(&self.raw, index)));
        }

        pub fn asArray(self: *@This()) [*]ElementType {
            return @alignCast(@ptrCast(self.raw.data));
        }

        pub fn asSlice(self: *@This()) []ElementType {
            return self.asArray()[0..self.raw.count];
        }
    };
}

pub inline fn inlineComment(_: []const u8) void {
}

pub fn main() !void {

    // ===================================================
    // MARK: Allocator
    // ===================================================

    inlineComment("=========== Allocator ===========");

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    var allocator = gpa.allocator();

    // ===================================================
    // MARK: JSON
    // ===================================================

    inlineComment("=========== JSON ===========");

    const str = "{\"test\":52, \"abc\":\"hello world\"}";
    const jsonObjectResult = sea.SEA_JSONValue_FromString(
        str,
        str.len,
        sea.SEA_Allocator_Heap
    );
    defer sea.SEA_JSONValue_free(jsonObjectResult, sea.SEA_Allocator_Heap);

    const stringified = sea.SEA_JSONValue_toString(jsonObjectResult, sea.SEA_Allocator_Heap);
    defer sea.SEA_Allocator_free(sea.SEA_Allocator_Heap, stringified);

    std.debug.print("SEA_JSONObject\n\t{s}\n", .{stringified});

    // ===================================================
    // MARK: ListDyn
    // ===================================================

    inlineComment("=========== ListDyn ===========");

    var newList = ListDyn(u32).init(sea.SEA_Allocator_Heap);
    defer newList.deinit();

    newList.addAll(&[_]u32{ 10, 15, 20, 25, 30 });

    inlineComment("=========== Slice ===========");

    for (newList.asSlice()) |v| {
        std.debug.print("Slice Value: {d}\n", .{v});
    }

    inlineComment("=========== Iterator ===========");

    var it = newList.iterator();
    it.forEach(struct {
        fn f(val: *@TypeOf(it).Element) void {
            std.debug.print("Iter Value: {d}\n", .{val.*});
        }
    }.f);

    // ===================================================
    // MARK: UUID
    // ===================================================

    inlineComment("=========== UUID ===========");

    var uuidTest = sea.SEA_UUID{};
    sea.SEA_UUID_generateV7(&uuidTest);

    const hexStr = try uuidToHex(&uuidTest.bytes, &allocator);
    defer allocator.free(hexStr);

    std.debug.print("UUID HEX: {s}\n", .{hexStr});
}