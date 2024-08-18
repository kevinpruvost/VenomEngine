cc_library(
    name = "venom_vulkan_static",
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h"]),
    deps = [
        "@rules_vulkan//vulkan:vulkan_cc_library",
        "//lib/common:venom_common_static",
    ],
    visibility = ["//visibility:public"],
)

cc_shared_library(
    name = "venom_vulkan_dll",
    deps = [
        ":venom_vulkan_static",
    ],
    dynamic_deps = [
        "//lib/common:venom_common_dll",
    ],
    visibility = ["//visibility:public"],
)