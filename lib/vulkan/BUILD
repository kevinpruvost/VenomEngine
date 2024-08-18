cc_library(
    name = "venom_vulkan_static",
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h"]),
    copts = [
        "-DGLFW_DLL",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//lib/common:venom_common_static",
        "//lib/glm",
        "@rules_vulkan//vulkan:vulkan_cc_library",
    ],
)

cc_shared_library(
    name = "VenomVulkan",
    dynamic_deps = [
        "//lib/common:VenomCommon",
        "//lib/GLFW:glfw",
    ],
    exports_filter = [
        "//lib/common:VenomCommon",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":venom_vulkan_static",
    ],
)
