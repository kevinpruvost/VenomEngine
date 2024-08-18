cc_library(
    name = "venom_vulkan_static",
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h"]),
    visibility = ["//visibility:public"],
    deps = [
        "//lib/GLFW:glfw",
        "//lib/common:venom_common_static",
        "//lib/glm",
        "@rules_vulkan//vulkan:vulkan_cc_library",
    ],
)

cc_shared_library(
    name = "venom_vulkan_dll",
    dynamic_deps = [
        "//lib/common:venom_common_dll",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":venom_vulkan_static",
    ],
)
