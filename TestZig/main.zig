const std = @import("std");

const SEA = @cImport({
    @cInclude("UUID.h");
    @cInclude("JSONValue.h");
    @cInclude("Allocator.h");
});

pub fn main() void {
    const jsonObjectResult = SEA.SEA_JSONValue.FromString.?(
        "{\"test\":52}",
        0,
        SEA.SEA_Allocator.Heap
    );
    const stringified = SEA.SEA_JSONValue.toString.?(jsonObjectResult, SEA.SEA_Allocator.Heap);
    std.debug.print("JSON OBJECT\n{s}\n", .{stringified});
    var uuidTest = SEA.struct_SEA_UUID{};
    SEA.SEA_UUID.generateV7.?(&uuidTest);
    std.debug.print("UUID CONTENT\n{}\n", .{SEA.SEA_UUID});
    std.debug.print("UUID TEST\n{}\n", .{uuidTest});
}