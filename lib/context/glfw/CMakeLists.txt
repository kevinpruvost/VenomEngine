cmake_minimum_required(VERSION 3.25)

# Project name and version
project(VenomContextGLFW VERSION 1.0)

# Define the static library target
file(GLOB_RECURSE venom_context_glfw_srcs
    "src/*.cc")
if (APPLE)
    # Add Objective-C++ source files
    file(GLOB_RECURSE venom_context_glfw_srcs_apple
        "src/*.mm"
        "src/*.m"
    )
    set(venom_context_glfw_srcs ${venom_context_glfw_srcs} ${venom_context_glfw_srcs_apple})
endif()
file(GLOB_RECURSE venom_context_glfw_hdrs 
    "include/venom/common/context/glfw/*.h"
    "include/venom/common/context/glfw/**/*.h"
)

set(venom_context_glfw_include_dirs
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)