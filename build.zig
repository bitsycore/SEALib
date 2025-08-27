const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    //const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "ZigBinding",
        .root_module = b.createModule(.{
            .root_source_file = b.path("TestZig//main.zig"),
            .target = target,
            .optimize = .ReleaseSmall,
            .link_libc = false,
            .strip = false,
            .sanitize_c = .off,
            .error_tracing = false,
            .no_builtin = true,
            .stack_check = false,
            .stack_protector = false,
            .omit_frame_pointer = true,
        }),
    });

    // ===================================================
    // MARK: Link external lib
    // ===================================================

    exe.addIncludePath(b.path("SEA/"));
    exe.addObjectFile(b.path("cmake-build-debug-clang-msvc/SEA.lib"));

    exe.linkSystemLibrary("bcrypt");

    exe.addLibraryPath(.{ .cwd_relative = "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.22621.0\\ucrt\\x64" });
    exe.linkSystemLibrary2("ucrtd", .{ .preferred_link_mode = .dynamic });
    exe.linkSystemLibrary2("vcruntime", .{ .preferred_link_mode = .dynamic });

    exe.addLibraryPath(.{ .cwd_relative = "C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.22621.0\\um\\x64" });
    exe.linkSystemLibrary2("BufferOverflowU", .{ .preferred_link_mode = .dynamic });

    // ===================================================
    // MARK: Steps
    // ===================================================

    b.installArtifact(exe);

    const run_exe = b.addRunArtifact(exe);
    const run_step = b.step("run", "Run the application");
    run_step.dependOn(&run_exe.step);
}
