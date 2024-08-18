cc_library(
    name = "venom_common_static",
    srcs = glob(["*.cc"]),
    hdrs = glob(["include/common/*.h"]),
    defines = [
        "VENOM_COMMON_EXPORTS",
    ],
    includes = [
        "include",
        "//lib/common",
    ],
    visibility = ["//visibility:public"],
)

cc_shared_library(
    name = "venom_common_dll",
    visibility = ["//visibility:public"],
    deps = [
        ":venom_common_static",
    ],
)
