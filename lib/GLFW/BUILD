cc_library(
    name = "glfw",
    srcs = [
        "src/context.c",
        "src/init.c",
        "src/input.c",
        "src/monitor.c",
        "src/vulkan.c",
        "src/window.c",
    ],
    hdrs = [
        "include/GLFW/glfw3.h",
        "include/GLFW/glfw3native.h",
        "src/mappings.h",
    ],
    # Macros: GLFW_BUILD_WIN32,GLFW_BUILD_COCOA,GLFW_BUILD_X11
    copts = select({
        "@platforms//os:windows": [
            "-D_GLFW_WIN32",
        ],
        "@platforms//os:linux": [
            "-D_GLFW_X11",
        ],
        "@platforms//os:macos": [
            "-D_GLFW_COCOA",
        ],
    }),
    includes = ["include"],
    strip_include_prefix = ".",
    visibility = ["//visibility:public"],
    deps = [
        ":glfw_platform",
    ],
)

cc_library(
    name = "glfw_platform",
    srcs = select({
        "@platforms//os:windows": [
            "src/wgl_context.c",
            "src/win32_init.c",
            "src/win32_joystick.c",
            "src/win32_monitor.c",
            "src/win32_thread.c",
            "src/win32_time.c",
            "src/win32_window.c",
        ],
        "@platforms//os:linux": [
            "src/glx_context.c",
            "src/posix_thread.c",
            "src/posix_time.c",
            "src/x11_init.c",
            "src/x11_monitor.c",
            "src/x11_window.c",
            "src/xkb_unicode.c",
        ],
        "@platforms//os:macos": [
            "src/cocoa_init.m",
            "src/cocoa_joystick.m",
            "src/cocoa_monitor.m",
            "src/cocoa_time.c",
            "src/cocoa_window.m",
            "src/nsgl_context.m",
            "src/posix_thread.c",
        ],
    }),
    hdrs = [
        "include/GLFW/glfw3.h",
        "include/GLFW/glfw3native.h",
        "src/internal.h",
        "src/null_joystick.h",
        "src/null_platform.h",
        "src/platform.h",
    ] + select({
        "@platforms//os:windows": [
            "src/win32_joystick.h",
            "src/win32_platform.h",
            "src/win32_thread.h",
            "src/win32_time.h",
        ],
        "@platforms//os:linux": [
            "src/glx_context.h",
            "src/posix_thread.h",
            "src/posix_time.h",
            "src/x11_init.h",
            "src/x11_monitor.h",
            "src/x11_platform.h",
            "src/x11_window.h",
            "src/xkb_unicode.h",
        ],
        "@platforms//os:macos": [
            "src/cocoa_joystick.h",
            "src/cocoa_monitor.h",
            "src/cocoa_platform.h",
            "src/cocoa_time.h",
            "src/cocoa_window.h",
            "src/nsgl_context.h",
            "src/posix_thread.h",
        ],
    }),
    # Macros: GLFW_BUILD_WIN32,GLFW_BUILD_COCOA,GLFW_BUILD_X11
    copts = select({
        "@platforms//os:windows": [
            "-D_GLFW_WIN32",
        ],
        "@platforms//os:linux": [
            "-D_GLFW_X11",
        ],
        "@platforms//os:macos": [
            "-D_GLFW_COCOA",
        ],
    }),
    target_compatible_with = select({
        "@platforms//os:windows": ["@platforms//os:windows"],
        "@platforms//os:linux": ["@platforms//os:linux"],
        "@platforms//os:macos": ["@platforms//os:macos"],
    }),
    visibility = ["//visibility:public"],
    deps = [
    ],
)
