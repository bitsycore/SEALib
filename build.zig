const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "ZigBinding",
        .root_module = b.createModule(.{
            .root_source_file = b.path("TestZig//main.zig"),
            .target = target,
            .optimize = optimize,
            .link_libc = true,
        }),
    });

    // Link with prebuilt CMake library
    exe.addIncludePath(b.path("SEA/"));
    exe.addObjectFile(b.path("cmake-build-debug-clang-msvc/SEA.lib"));
    exe.linkSystemLibrary("bcrypt");

    b.installArtifact(exe);

    const run_exe = b.addRunArtifact(exe);

    const run_step = b.step("run", "Run the application");
    run_step.dependOn(&run_exe.step);
}
