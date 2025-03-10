cmake_minimum_required(VERSION 3.25)

# Project name and version
project(VenomContextApple VERSION 1.0)

# Define the static library target
file(GLOB_RECURSE venom_context_apple_srcs
    "src/*.cc"
    "src/*.mm"
    "src/*.m"
)

file(GLOB_RECURSE venom_context_apple_hdrs 
    "include/venom/common/context/apple/*.h"
    "include/venom/common/context/apple/**/*.h"
)

set(venom_context_apple_include_dirs
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)