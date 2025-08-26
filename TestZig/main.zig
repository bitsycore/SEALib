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
        buf[index] = std.fmt.hex(b)[0];
        buf[index + 1] = std.fmt.hex(b)[1];
        index += 2;
    }
    return buf;
}

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    var allocator = gpa.allocator();

    const jsonObjectResult = SEA.SEA_JSONValue.FromString.?(
        "{\"test\":52}",
        0,
        SEA.SEA_Allocator.Heap
    );
    const stringified = SEA.SEA_JSONValue.toString.?(jsonObjectResult, SEA.SEA_Allocator.Heap);
    std.debug.print("JSON OBJECT\n{s}\n", .{stringified});
    var uuidTest = SEA.struct_SEA_UUID{};
    SEA.SEA_UUID.generateV7.?(&uuidTest);

    const hexStr = try uuidToHex(&uuidTest.bytes, &allocator);
    defer allocator.free(hexStr);

    std.debug.print("UUID CONTENT\n{}\n", .{SEA.SEA_UUID});
    std.debug.print("UUID HEX: {s}\n", .{hexStr});
}