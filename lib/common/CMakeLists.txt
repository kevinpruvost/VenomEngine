cmake_minimum_required(VERSION 3.25)

# Project name and version
project(VenomCommon VERSION 1.0)

# Define the static library target
file(GLOB_RECURSE venom_common_srcs
    "src/*.cc")
if (APPLE)
    # Add Objective-C++ source files
    file(GLOB_RECURSE venom_common_srcs_apple
        "src/*.mm"
        "src/*.m"
    )
    list(APPEND venom_common_srcs ${venom_common_srcs_apple})
endif()
file(GLOB_RECURSE venom_common_hdrs 
    "include/venom/common/*.h"
    "include/venom/common/**/*.h"
)

add_library(${PROJECT_NAME} SHARED
        ${venom_common_srcs} ${venom_common_hdrs}
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

if (APPLE)
    find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)
    find_library(CORESERVICES_FRAMEWORK CoreServices)
    find_library(FOUNDATION_FRAMEWORK Foundation)

    target_link_libraries(${PROJECT_NAME} "-framework CoreFoundation")
    target_link_libraries(${PROJECT_NAME} "-framework CoreServices")
    target_link_libraries(${PROJECT_NAME} "-framework Foundation")

    install(TARGETS ${PROJECT_NAME}
            LIBRARY DESTINATION lib # No need actually
            ARCHIVE DESTINATION lib # No need actually
            RUNTIME DESTINATION bin # No need actually
            FILE_SET HEADERS DESTINATION include # Key need actually
    )
endif()