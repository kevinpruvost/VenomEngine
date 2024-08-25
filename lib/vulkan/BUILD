cc_library(
    name = "venom_vulkan_static",
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h"]),
    copts = [
        "-DGLFW_DLL",
    ] + select({
        "@platforms//os:windows": [
            "-D_GLFW_WIN32",
        ],
        "@platforms//os:linux": [
            "-D_GLFW_X11",
            #"-D_GLFW_WAYLAND",
        ],
        "@platforms//os:macos": [
            "-D_GLFW_COCOA",
        ],
    }),
    visibility = ["//visibility:public"],
    deps = [
        "//lib:glm",
        "//lib/common:venom_common_static",
        "@rules_vulkan//vulkan:vulkan_cc_library",
    ],
)

cc_shared_library(
    name = "VenomVulkan",
    dynamic_deps = [
        "//lib/common:VenomCommon",
        "//lib:glfw",
    ],
    exports_filter = [
        "//lib/common:VenomCommon",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":venom_vulkan_static",
    ],
)
