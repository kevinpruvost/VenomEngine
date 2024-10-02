##
## GLFW
##

load("@bazel_skylib//rules:copy_file.bzl", "copy_file")
load("@cmake_configure_file//:cmake_configure_file.bzl", "cmake_configure_file")

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

objc_library(
    name = "glfw_platform_macos",
    srcs = [
        "glfw/src/cocoa_init.m",
        "glfw/src/cocoa_joystick.h",
        "glfw/src/cocoa_joystick.m",
        "glfw/src/cocoa_monitor.m",
        "glfw/src/cocoa_platform.h",
        "glfw/src/cocoa_window.m",
        "glfw/src/nsgl_context.m",
    ],
    copts = [
        "-D_GLFW_COCOA",
        "-D_GLFW_BUILD_DLL",
    ],
    target_compatible_with = select({
        "@platforms//os:macos": ["@platforms//os:macos"],
    }),
    visibility = ["//visibility:public"],
    deps = [
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
            "glfw/src/posix_module.c",
            "glfw/src/posix_thread.c",
            "glfw/src/posix_time.c",
            "glfw/src/x11_init.c",
            "glfw/src/x11_monitor.c",
            "glfw/src/x11_window.c",
            "glfw/src/xkb_unicode.c",
        ],
        "@platforms//os:macos": [
            "glfw/src/cocoa_time.c",
            "glfw/src/posix_module.c",
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
            "glfw/src/cocoa_platform.h",
            "glfw/src/cocoa_time.h",
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
    deps = select({
        "@platforms//os:macos": [":glfw_platform_macos"],
        "//conditions:default": [],
    }),
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

# SPIRV-Reflect

cc_library(
    name = "SPIRVReflect",
    srcs = [
        "SPIRV-Reflect/spirv_reflect.cpp",
    ],
    hdrs = glob(["SPIRV-Reflect/*.h"]),
    includes = ["SPIRV-Reflect/"],
    visibility = ["//visibility:public"],
    deps = [],
)

# assimp

cmake_configure_file(
    name = "zlib_zconf",
    src = "zlib/zconf.h.in",
    out = "zlib/zconf.h",
    defines = [
    ],
)

cmake_configure_file(
    name = "assimp_zconf",
    src = "assimp/contrib/zlib/zconf.h.in",
    out = "assimp/contrib/zlib/zconf.h",
    defines = [
    ],
)

cmake_configure_file(
    name = "assimp_cmake_config",
    src = "assimp/include/assimp/config.h.in",
    out = "assimp/include/assimp/config.h",
    defines = [
        #        "ASSIMP_DOUBLE_PRECISION"
    ],
)

genrule(
    name = "generate_git_commit_hash",
    srcs = [],
    outs = ["assimp_bazel/git_commit_hash.txt"],
    cmd = "./$(location assimp_bazel/generate_git_commit.sh)",
    tools = [
        "assimp_bazel/generate_git_commit.sh",
    ],
)

# Change these variables when updating assimp

# Just check the project version in CMakeLists.txt of assimp in 'project(Assimp VERSION ...)'
ASSIMP_VERSION_MAJOR = 5

ASSIMP_VERSION_MINOR = 4

ASSIMP_VERSION_PATCH = 3

# git rev-parse --short=8 HEAD at assimp root
ASSIMP_GIT_COMMIT_HASH = "97257a3b"

cmake_configure_file(
    name = "assimp_cmake_revision",
    src = "assimp/include/assimp/revision.h.in",
    out = "assimp/include/assimp/revision.h",
    defines = [
        "GIT_COMMIT_HASH=" + ASSIMP_GIT_COMMIT_HASH,
        "GIT_BRANCH=main",
        "ASSIMP_VERSION_MAJOR=" + str(ASSIMP_VERSION_MAJOR),
        "ASSIMP_VERSION_MINOR=" + str(ASSIMP_VERSION_MINOR),
        "ASSIMP_VERSION_PATCH=" + str(ASSIMP_VERSION_PATCH),
        "ASSIMP_PACKAGE_VERSION=" + str(0),
        # Not needed
        "CMAKE_SHARED_LIBRARY_PREFIX=None",
        "CMAKE_DEBUG_POSTFIX=None",
        "LIBRARY_SUFFIX=None",
    ],
)

cc_library(
    name = "pugixml",
    srcs = [
        "assimp/contrib/pugixml/src/pugixml.cpp",
    ],
    hdrs = [
        "assimp/contrib/pugixml/src/pugiconfig.hpp",
        "assimp/contrib/pugixml/src/pugixml.hpp",
    ],
    copts = [
        #"-DPUGIXML_SOURCE",
    ],
    includes = [
        "assimp/contrib/pugixml/src",
    ],
    visibility = ["//visibility:public"],
    deps = [],
)

cc_library(
    name = "assimp",
    srcs = glob([
        "assimp/code/AssetLib/IFC/IFCBoolean.cpp",
        "assimp/code/AssetLib/IFC/IFCCurve.cpp",
        "assimp/code/AssetLib/IFC/IFCGeometry.cpp",
        "assimp/code/AssetLib/IFC/IFCLoader.cpp",
        "assimp/code/AssetLib/IFC/IFCLoader.h",
        "assimp/code/AssetLib/IFC/IFCMaterial.cpp",
        "assimp/code/AssetLib/IFC/IFCOpenings.cpp",
        "assimp/code/AssetLib/IFC/IFCProfile.cpp",
        "assimp/code/AssetLib/IFC/IFCReaderGen1_2x3.cpp",
        "assimp/code/AssetLib/IFC/IFCReaderGen2_2x3.cpp",
        "assimp/code/AssetLib/IFC/IFCReaderGen_2x3.h",
        "assimp/code/AssetLib/IFC/IFCUtil.cpp",
        "assimp/code/AssetLib/IFC/IFCUtil.h",
        "assimp/code/AssetLib/3DS/*.cpp",
        "assimp/code/AssetLib/3DS/*.h",
        "assimp/code/AssetLib/3MF/*.cpp",
        "assimp/code/AssetLib/3MF/*.h",
        "assimp/code/AssetLib/AC/*.cpp",
        "assimp/code/AssetLib/AC/*.h",
        "assimp/code/AssetLib/AMF/*.cpp",
        "assimp/code/AssetLib/AMF/*.hpp",
        "assimp/code/AssetLib/ASE/*.cpp",
        "assimp/code/AssetLib/ASE/*.h",
        "assimp/code/AssetLib/Assjson/*.cpp",
        "assimp/code/AssetLib/Assjson/*.h",
        "assimp/code/AssetLib/B3D/*.cpp",
        "assimp/code/AssetLib/B3D/*.h",
        "assimp/code/AssetLib/Blender/*.cpp",
        "assimp/code/AssetLib/Blender/*.h",
        "assimp/code/AssetLib/Blender/*.inl",
        "assimp/code/AssetLib/BVH/*.cpp",
        "assimp/code/AssetLib/BVH/*.h",
        "assimp/code/AssetLib/COB/*.cpp",
        "assimp/code/AssetLib/COB/*.h",
        "assimp/code/AssetLib/Collada/*.cpp",
        "assimp/code/AssetLib/Collada/*.h",
        "assimp/code/AssetLib/CSM/*.cpp",
        "assimp/code/AssetLib/CSM/*.h",
        "assimp/code/AssetLib/DXF/*.cpp",
        "assimp/code/AssetLib/DXF/*.h",
        "assimp/code/AssetLib/FBX/*.cpp",
        "assimp/code/AssetLib/FBX/*.h",
        "assimp/code/AssetLib/glTF/*.cpp",
        "assimp/code/AssetLib/glTF/*.inl",
        "assimp/code/AssetLib/glTF/*.h",
        "assimp/code/AssetLib/glTF2/*.cpp",
        "assimp/code/AssetLib/glTF2/*.h",
        "assimp/code/AssetLib/glTF2/*.inl",
        "assimp/code/AssetLib/HMP/*.cpp",
        "assimp/code/AssetLib/HMP/*.h",
        "assimp/code/AssetLib/IQM/*.cpp",
        "assimp/code/AssetLib/IQM/*.h",
        "assimp/code/AssetLib/Irr/*.cpp",
        "assimp/code/AssetLib/Irr/*.h",
        "assimp/code/AssetLib/IRRMESH/*.cpp",
        "assimp/code/AssetLib/IRRMESH/*.h",
        "assimp/code/AssetLib/LWO/*.cpp",
        "assimp/code/AssetLib/LWO/*.h",
        "assimp/code/AssetLib/LWS/*.cpp",
        "assimp/code/AssetLib/LWS/*.h",
        "assimp/code/AssetLib/M3D/*.cpp",
        "assimp/code/AssetLib/M3D/*.h",
        "assimp/code/AssetLib/MD2/*.cpp",
        "assimp/code/AssetLib/MD2/*.h",
        "assimp/code/AssetLib/MD3/*.cpp",
        "assimp/code/AssetLib/MD3/*.h",
        "assimp/code/AssetLib/MD5/*.cpp",
        "assimp/code/AssetLib/MD5/*.h",
        "assimp/code/AssetLib/MDC/*.cpp",
        "assimp/code/AssetLib/MDC/*.h",
        "assimp/code/AssetLib/MDL/*.cpp",
        "assimp/code/AssetLib/MDL/*.h",
        "assimp/code/AssetLib/MDL/HalfLife/*.cpp",
        "assimp/code/AssetLib/MDL/HalfLife/*.h",
        "assimp/code/AssetLib/MMD/*.cpp",
        "assimp/code/AssetLib/MMD/*.h",
        "assimp/code/AssetLib/MS3D/*.cpp",
        "assimp/code/AssetLib/MS3D/*.h",
        "assimp/code/AssetLib/NDO/*.cpp",
        "assimp/code/AssetLib/NDO/*.h",
        "assimp/code/AssetLib/NFF/*.cpp",
        "assimp/code/AssetLib/NFF/*.h",
        "assimp/code/AssetLib/Obj/*.cpp",
        "assimp/code/AssetLib/Obj/*.h",
        "assimp/code/AssetLib/OFF/*.cpp",
        "assimp/code/AssetLib/OFF/*.h",
        "assimp/code/AssetLib/Ogre/*.cpp",
        "assimp/code/AssetLib/Ogre/*.h",
        "assimp/code/AssetLib/OpenGEX/*.cpp",
        "assimp/code/AssetLib/OpenGEX/*.h",
        "assimp/code/AssetLib/Ply/*.cpp",
        "assimp/code/AssetLib/Ply/*.h",
        "assimp/code/AssetLib/Q3BSP/*.cpp",
        "assimp/code/AssetLib/Q3BSP/*.h",
        "assimp/code/AssetLib/Q3D/*.cpp",
        "assimp/code/AssetLib/Q3D/*.h",
        "assimp/code/AssetLib/Raw/*.cpp",
        "assimp/code/AssetLib/Raw/*.h",
        "assimp/code/AssetLib/SIB/*.cpp",
        "assimp/code/AssetLib/SIB/*.h",
        "assimp/code/AssetLib/SMD/*.cpp",
        "assimp/code/AssetLib/SMD/*.h",
        "assimp/code/AssetLib/STEP/*.cpp",
        "assimp/code/AssetLib/STEP/*.h",
        "assimp/code/AssetLib/STEPParser/*.cpp",
        "assimp/code/AssetLib/STEPParser/*.h",
        "assimp/code/AssetLib/STL/*.cpp",
        "assimp/code/AssetLib/STL/*.h",
        "assimp/code/AssetLib/Terragen/*.cpp",
        "assimp/code/AssetLib/Terragen/*.h",
        "assimp/code/AssetLib/X/*.cpp",
        "assimp/code/AssetLib/X/*.h",
        "assimp/code/AssetLib/X3D/*.cpp",
        "assimp/code/AssetLib/X3D/*.h",
        "assimp/code/AssetLib/X3D/*.hpp",
        "assimp/code/AssetLib/XGL/*.cpp",
        "assimp/code/AssetLib/XGL/*.h",
        "assimp/code/AssetLib/Unreal/*.cpp",
        "assimp/code/AssetLib/Unreal/*.h",
        "assimp/code/AssetLib/Blender/*.cpp",
        "assimp/code/AssetLib/Blender/*.h",
        "assimp/code/AssetLib/C4D/*.cpp",
        "assimp/code/AssetLib/C4D/*.h",
        "assimp/code/AssetLib/USD/*.cpp",
        "assimp/code/AssetLib/USD/*.h",
        "assimp/code/CApi/**/*.cpp",
        "assimp/code/CApi/**/*.h",
        "assimp/code/Common/**/*.cpp",
        "assimp/code/Common/**/*.h",
        "assimp/code/Geometry/**/*.cpp",
        "assimp/code/Geometry/**/*.h",
        "assimp/code/Material/**/*.cpp",
        "assimp/code/Material/**/*.h",
        "assimp/code/PostProcessing/**/*.cpp",
        "assimp/code/PostProcessing/**/*.h",
        "assimp/contrib/clipper/*.cpp",
        "assimp/contrib/Open3DGC/*.cpp",
        "assimp/contrib/poly2tri/**/*.cc",
        "assimp/contrib/unzip/*.c",
        "assimp/contrib/clipper/*.hpp",
        "assimp/contrib/Open3DGC/*.h",
        "assimp/contrib/Open3DGC/*.inl",
        "assimp/contrib/openddlparser/code/*.cpp",
        "assimp/contrib/openddlparser/include/openddlparser/*.h",
        "assimp/contrib/poly2tri/**/*.h",
        "assimp/contrib/rapidjson/include/rapidjson/**/*.h",
        #"assimp/contrib/pugixml/src/*.hpp",
        "assimp/contrib/utf8cpp/source/*.h",
        "assimp/contrib/utf8cpp/source/utf8/*.h",
        "assimp/contrib/stb/stb_image.h",
        "assimp/contrib/unzip/*.h",
        "assimp/code/CApi/**/*.h",
        "assimp/code/Common/**/*.h",
        "assimp/code/Common/**/*.inl",
        "assimp/code/Geometry/**/*.h",
        "assimp/code/Material/**/*.h",
        "assimp/code/PostProcessing/**/*.h",
    ]) + [
        ":assimp_cmake_config",
        ":assimp_cmake_revision",
        ":assimp_zconf",
    ],
    hdrs = glob([
        "assimp/include/assimp/Compiler/*.h",
        "assimp/include/assimp/*.hpp",
        "assimp/include/assimp/*.h",
        "assimp/include/assimp/*.inl",
    ]),
    copts = select({
        "@platforms//os:windows": [
            "-D_SCL_SECURE_NO_WARNINGS",
            "-D_CRT_SECURE_NO_WARNINGS",
        ],
        "@platforms//os:linux": [
        ],
        "@platforms//os:macos": [
            "-D_ENABLE_BITCODE",
        ],
    }) + [
        "-DASSIMP_BUILD_NO_EXPORT",
        "-DASSIMP_BUILD_DLL_EXPORT",
        "-DOPENDDLPARSER_BUILD",
        #"-DPUGIXML_SOURCE",
        #"-DPUGIXML_HEADER_ONLY",
        #"-DASSIMP_DOUBLE_PRECISION",

        # If you need IFC, USD or C4D or maybe more, you can remove these flags and add src files
        "-DASSIMP_BUILD_NO_IFC_IMPORTER",
        "-DASSIMP_BUILD_NO_USD_IMPORTER",
        "-DASSIMP_BUILD_NO_C4D_IMPORTER",
        "-DASSIMP_BUILD_NO_ASSBIN_EXPORTER",
        "-DASSIMP_BUILD_NO_ASSBIN_IMPORTER",
        "-DASSIMP_BUILD_NO_ASSXML_EXPORTER",
    ],
    includes = [
        "assimp/",
        "assimp/code/",
        "assimp/code/AssetLib/AMF/",
        "assimp/contrib/",
        "assimp/contrib/openddlparser/include/",
        #"assimp/contrib/pugixml/src/",
        "assimp/contrib/rapidjson/include/",
        "assimp/contrib/stb/",
        "assimp/contrib/unzip/",
        "assimp/contrib/utf8cpp/source/",
        #"assimp/contrib/zlib/",
        "assimp/include/",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@pugixml",
        "@zlib",
    ],
)

cc_shared_library(
    name = "assimp_shared",
    exports_filter = [
        ":assimp",
        ":pugixml",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":assimp",
    ],
)
