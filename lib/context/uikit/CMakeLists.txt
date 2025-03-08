cmake_minimum_required(VERSION 3.25)

# Project name and version
project(VenomUIKit VERSION 1.0)

# Define the static library target
file(GLOB_RECURSE venom_uikit_srcs
    "src/*.cc")
if (APPLE)
    # Add Objective-C++ source files
    file(GLOB_RECURSE venom_uikit_srcs_apple
        "src/*.mm"
        "src/*.m"
    )
endif()
file(GLOB_RECURSE venom_uikit_hdrs 
    "include/venom/context/glfw/*.h"
    "include/venom/context/glfw/**/*.h"
)

add_library(${PROJECT_NAME} SHARED
        ${venom_uikit_srcs} ${venom_uikit_hdrs}
)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

set(venom_uikit_libraries
    VenomCommon
    IconFontCppHeaders
)

target_link_libraries(${PROJECT_NAME}
    ${venom_uikit_libraries}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    VENOM_GLFW_EXPORTS
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
        ${venom_uikit_srcs}
        ${venom_uikit_srcs_apple}
        ${venom_uikit_hdrs}
    )

    set_target_properties(${PROJECT_NAME_PACKAGE} PROPERTIES
        PREFIX ""
    )

    target_link_libraries(${PROJECT_NAME_PACKAGE}
        ${venom_uikit_libraries}
        ${COREFOUNDATION_FRAMEWORK}
        ${CORESERVICES_FRAMEWORK}
        ${FOUNDATION_FRAMEWORK}
    )

    target_compile_definitions(${PROJECT_NAME_PACKAGE} PRIVATE
        VENOM_GLFW_EXPORTS
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