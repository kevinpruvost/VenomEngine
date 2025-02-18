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
    flecs::flecs
    OpenEXR::OpenEXR
    openvdb_static
    MikkTSpace
    nfd
    IconFontCppHeaders
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    VENOM_COMMON_EXPORTS
    CMAKE_PROJECT_DIR="${CMAKE_SOURCE_DIR}"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    "include/"
)

if (APPLE)
    find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)
    find_library(CORESERVICES_FRAMEWORK CoreServices)
    find_library(FOUNDATION_FRAMEWORK Foundation)

    set(PROJECT_NAME_PACKAGE ${PROJECT_NAME}_Package)

    add_library(${PROJECT_NAME_PACKAGE} SHARED
        ${venom_common_srcs}
        ${venom_common_srcs_apple}
        ${venom_common_hdrs}
    )

    set_target_properties(${PROJECT_NAME_PACKAGE} PROPERTIES
        PREFIX ""
    )

    target_link_libraries(${PROJECT_NAME_PACKAGE}
        assimp
        glfw
        glm
        stb_image
        flecs::flecs
        OpenEXR::OpenEXR
        openvdb_static
        MikkTSpace
        nfd
    )

    target_compile_definitions(${PROJECT_NAME_PACKAGE} PRIVATE
        VENOM_COMMON_EXPORTS
    )

    target_include_directories(${PROJECT_NAME_PACKAGE} PUBLIC
        "include/"
    )

    target_link_libraries(${PROJECT_NAME_PACKAGE} "-framework CoreFoundation")
    target_link_libraries(${PROJECT_NAME_PACKAGE} "-framework CoreServices")
    target_link_libraries(${PROJECT_NAME_PACKAGE} "-framework Foundation")

    install(TARGETS ${PROJECT_NAME_PACKAGE}
            LIBRARY DESTINATION lib # No need actually
            ARCHIVE DESTINATION lib # No need actually
            RUNTIME DESTINATION bin # No need actually
            FILE_SET HEADERS DESTINATION include # Key need actually
    )
endif()