cmake_minimum_required(VERSION 3.25)

# Project name and version
project(VenomCommon VERSION 1.0)

# Define the static library target
file(GLOB_RECURSE venom_common_srcs
    "src/*.cc")
file(GLOB_RECURSE venom_common_hdrs 
    "include/venom/common/*.h"
    "include/venom/common/**/*.h"
)

add_library(${PROJECT_NAME} SHARED ${venom_common_srcs} ${venom_common_hdrs}
        include/venom/common/plugin/graphics/MaterialComponent.h)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

target_link_libraries(${PROJECT_NAME}
    assimp
    glfw
    glm
    stb_image
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    VENOM_COMMON_EXPORTS
)

target_include_directories(${PROJECT_NAME} PUBLIC
    "include/"
)