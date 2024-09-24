##
## GLFW
##

cc_shared_library(
    name = "glfw",
    exports_filter = [
        ":glfw_static",
        ":glfw_platform",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":glfw_platform",
        ":glfw_static",
    ],
)

cc_library(
    name = "glfw_static",
    srcs = [
        "glfw/src/context.c",
        "glfw/src/egl_context.c",
        "glfw/src/init.c",
        "glfw/src/input.c",
        "glfw/src/monitor.c",
        "glfw/src/null_init.c",
        "glfw/src/null_joystick.c",
        "glfw/src/null_monitor.c",
        "glfw/src/null_window.c",
        "glfw/src/osmesa_context.c",
        "glfw/src/platform.c",
        "glfw/src/vulkan.c",
        "glfw/src/window.c",
    ],
    hdrs = [
        "glfw/include/GLFW/glfw3.h",
        "glfw/include/GLFW/glfw3native.h",
        "glfw/src/internal.h",
        "glfw/src/mappings.h",
    ],
    # Macros: GLFW_BUILD_WIN32,GLFW_BUILD_COCOA,GLFW_BUILD_X11
    copts = select({
        "@platforms//os:windows": [
            "-D_GLFW_WIN32",
            "-D_GLFW_BUILD_DLL",
        ],
        "@platforms//os:linux": [
            "-D_GLFW_X11",
            #"-D_GLFW_WAYLAND",
            "-D_GLFW_BUILD_DLL",
        ],
        "@platforms//os:macos": [
            "-D_GLFW_COCOA",
            "-D_GLFW_BUILD_DLL",
        ],
    }),
    includes = ["glfw/include"],
    strip_include_prefix = "./glfw/",
    visibility = ["//visibility:public"],
    deps = [
        ":glfw_platform",
    ],
)

cc_library(
    name = "glfw_platform",
    srcs = select({
        "@platforms//os:windows": [
            "glfw/src/wgl_context.c",
            "glfw/src/win32_init.c",
            "glfw/src/win32_joystick.c",
            "glfw/src/win32_module.c",
            "glfw/src/win32_monitor.c",
            "glfw/src/win32_thread.c",
            "glfw/src/win32_time.c",
            "glfw/src/win32_window.c",
        ],
        "@platforms//os:linux": [
            "glfw/src/glx_context.c",
            "glfw/src/posix_thread.c",
            "glfw/src/posix_time.c",
            "glfw/src/x11_init.c",
            "glfw/src/x11_monitor.c",
            "glfw/src/x11_window.c",
            "glfw/src/xkb_unicode.c",
        ],
        "@platforms//os:macos": [
            "glfw/src/cocoa_init.m",
            "glfw/src/cocoa_joystick.m",
            "glfw/src/cocoa_monitor.m",
            "glfw/src/cocoa_time.c",
            "glfw/src/cocoa_window.m",
            "glfw/src/nsgl_context.m",
            "glfw/src/posix_thread.c",
        ],
    }),
    hdrs = [
        "glfw/include/GLFW/glfw3.h",
        "glfw/include/GLFW/glfw3native.h",
        "glfw/src/internal.h",
        "glfw/src/null_joystick.h",
        "glfw/src/null_platform.h",
        "glfw/src/platform.h",
    ] + select({
        "@platforms//os:windows": [
            "glfw/src/win32_joystick.h",
            "glfw/src/win32_platform.h",
            "glfw/src/win32_thread.h",
            "glfw/src/win32_time.h",
        ],
        "@platforms//os:linux": [
            "glfw/src/glx_context.h",
            "glfw/src/posix_thread.h",
            "glfw/src/posix_time.h",
            "glfw/src/x11_init.h",
            "glfw/src/x11_monitor.h",
            "glfw/src/x11_platform.h",
            "glfw/src/x11_window.h",
            "glfw/src/xkb_unicode.h",
        ],
        "@platforms//os:macos": [
            "glfw/src/cocoa_joystick.h",
            "glfw/src/cocoa_monitor.h",
            "glfw/src/cocoa_platform.h",
            "glfw/src/cocoa_time.h",
            "glfw/src/cocoa_window.h",
            "glfw/src/nsgl_context.h",
            "glfw/src/posix_thread.h",
        ],
    }),
    # Macros: GLFW_BUILD_WIN32,GLFW_BUILD_COCOA,GLFW_BUILD_X11
    copts = select({
        "@platforms//os:windows": [
            "-D_GLFW_WIN32",
            "-D_GLFW_BUILD_DLL",
        ],
        "@platforms//os:linux": [
            "-D_GLFW_X11",
            #"-D_GLFW_WAYLAND",
            "-D_GLFW_BUILD_DLL",
        ],
        "@platforms//os:macos": [
            "-D_GLFW_COCOA",
            "-D_GLFW_BUILD_DLL",
        ],
    }),
    linkopts = select({
        "@platforms//os:windows": [
            "-DEFAULTLIB:gdi32.lib",
            "-DEFAULTLIB:shell32.lib",
            "-DEFAULTLIB:kernel32.lib",
            "-DEFAULTLIB:user32.lib",
        ],
        "//conditions:default": [],
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

##
## GLM
##

# Include directories for the header-only library
cc_library(
    name = "glm_header_only",
    hdrs = glob([
        "glm/glm/**/*.hpp",
        "glm/glm/**/*.inl",
        "glm/glm/**/*.natvis",
    ]),
    includes = ["glm"],
    strip_include_prefix = "./glm",
    visibility = ["//visibility:public"],
)

# Conditional library definition depending on GLM_BUILD_LIBRARY flag
cc_library(
    name = "glm_library",
    srcs = glob([
        "glm/glm/*.cpp",
        "glm/glm/detail/**/*.cpp",
        "glm/glm/ext/**/*.cpp",
        "glm/glm/gtc/**/*.cpp",
        "glm/glm/gtx/**/*.cpp",
        "glm/glm/simd/**/*.cpp",
    ]),
    hdrs = glob([
        "glm/glm/*.hpp",
        "glm/glm/*.inl",
        "glm/glm/detail/**/*.hpp",
        "glm/glm/detail/**/*.inl",
        "glm/glm/ext/**/*.hpp",
        "glm/glm/ext/**/*.inl",
        "glm/glm/gtc/**/*.hpp",
        "glm/glm/gtc/**/*.inl",
        "glm/glm/gtx/**/*.hpp",
        "glm/glm/gtx/**/*.inl",
        "glm/glm/simd/**/*.hpp",
        "glm/glm/simd/**/*.h",
    ]),
    strip_include_prefix = "./glm",
    visibility = ["//visibility:public"],
    deps = [":glm_header_only"],
)

# Alias for the glm::glm target
alias(
    name = "glm",
    actual = ":glm_library",
    visibility = ["//visibility:public"],
)

# VulkanMemoryAllocator

cc_library(
    name = "VulkanMemoryAllocator",
    srcs = [],
    hdrs = glob(["VulkanMemoryAllocator/include/*.h"]),
    includes = ["VulkanMemoryAllocator/include"],
    visibility = ["//visibility:public"],
    deps = [],
)

# stb_image

cc_library(
    name = "stb_image",
    srcs = ["stb_image/stb_vorbis.c"],
    hdrs = glob(["stb_image/*.h"]),
    includes = ["stb_image/"],
    visibility = ["//visibility:public"],
    deps = [],
)
