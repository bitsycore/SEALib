const std = @import("std");

const SEA = @cImport({
    @cInclude("Allocator.h");
    @cInclude("JSONValue.h");
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

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    var allocator = gpa.allocator();

    const str = "{\"test\":52, \"abc\":\"hello world\"}";
    const jsonObjectResult = SEA.JSONValue_FromString(
        str,
        str.len,
        SEA.SEA_Allocator_Heap
    );

    const stringified = SEA.JSONValue_toString(jsonObjectResult, SEA.SEA_Allocator_Heap);
    std.debug.print("SEA_JSONObject\n\t{s}\n", .{stringified});
    var uuidTest = SEA.SEA_UUID{};
    SEA.UUID_generateV7(&uuidTest);

    const hexStr = try uuidToHex(&uuidTest.bytes, &allocator);
    defer allocator.free(hexStr);
    std.debug.print("UUID HEX: {s}\n", .{hexStr});
}