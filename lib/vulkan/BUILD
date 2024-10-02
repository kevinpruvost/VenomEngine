cc_library(
    name = "venom_vulkan_static",
    srcs = glob(["src/*.cc"]),
    hdrs = glob(["include/venom/vulkan/**/**/**/*.h"]),
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
    include_prefix = "include",
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        "//lib/common:venom_common_static",
        #"//lib/external:SPIRVReflect",
        "//lib/external:glm",
        "@rules_vulkan//vulkan:vulkan_cc_library",
    ],
)

cc_shared_library(
    name = "VenomVulkan",
    dynamic_deps = [
        "//lib/common:VenomCommon",
        "//lib/external:glfw",
    ],
    exports_filter = [
        "//lib/common:VenomCommon",
        "//lib/external:glm_library",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":venom_vulkan_static",
    ],
)
