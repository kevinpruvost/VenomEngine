cmake_minimum_required(VERSION 3.25)

# Project name and version
project(VenomCommon VERSION 1.0)

# Define the static library target
file(GLOB_RECURSE venom_common_srcs
    "src/*.cc"
    "src/**/*.cc"
)
if (APPLE)
    # Add Objective-C++ source files
    file(GLOB_RECURSE venom_common_srcs_apple
        "src/*.mm"
        "src/*.m"
        "src/**/*.mm"
        "src/**/*.m"
    )
endif()
file(GLOB_RECURSE venom_common_hdrs 
    "include/venom/common/*.h"
    "include/venom/common/**/*.h"
)

add_library(${PROJECT_NAME} SHARED
        ${venom_common_srcs}
        ${venom_common_srcs_apple}
        ${venom_common_hdrs}
)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

set(venom_common_libraries
    assimp
    glm
    stb_image
    flecs::flecs
    OpenEXR::OpenEXR
    openvdb_static
    MikkTSpace
    IconFontCppHeaders
    flatbuffers
)

# Skip GLFW & NFD on Iphone
if ((NOT APPLE) OR (NOT CMAKE_SYSTEM_NAME MATCHES "iOS"))
    set(venom_common_libraries ${venom_common_libraries} glfw nfd)
endif ()

if (APPLE)
    if (CMAKE_SYSTEM_NAME MATCHES "iOS")
        set(venom_common_libraries ${venom_common_libraries} "-framework UIKit")
    else ()
        set(venom_common_libraries ${venom_common_libraries} "-framework Cocoa")
    endif ()
    find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)
    find_library(METAL_FRAMEWORK Metal)
    find_library(METALKIT_FRAMEWORK MetalKit)
    set(venom_common_libraries ${venom_common_libraries} "-framework CoreFoundation")
    set(venom_common_libraries ${venom_common_libraries} "-framework Metal")
    set(venom_common_libraries ${venom_common_libraries} "-framework MetalKit")
endif ()

target_link_libraries(${PROJECT_NAME}
    ${venom_common_libraries}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    VENOM_COMMON_EXPORTS
    CMAKE_PROJECT_DIR="${CMAKE_SOURCE_DIR}"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    "include/"
)

set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_STYLE "AUTOMATIC")

if (APPLE)
    find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)
    find_library(CORESERVICES_FRAMEWORK CoreServices)
    find_library(FOUNDATION_FRAMEWORK Foundation)

    set(PROJECT_NAME_PACKAGE ${PROJECT_NAME})

#    add_library(${PROJECT_NAME_PACKAGE} SHARED
#        ${venom_common_srcs}
#        ${venom_common_srcs_apple}
#        ${venom_common_hdrs}
#    )

    set_target_properties(${PROJECT_NAME_PACKAGE} PROPERTIES
        PREFIX ""
    )

#    target_link_libraries(${PROJECT_NAME_PACKAGE}
#        ${venom_common_libraries}
#    )
#
#    target_compile_definitions(${PROJECT_NAME_PACKAGE} PRIVATE
#        VENOM_COMMON_EXPORTS
#        VENOM_PACKAGE
#    )

#    target_include_directories(${PROJECT_NAME_PACKAGE} PUBLIC
#        "include/"
#    )

    set_target_properties(${PROJECT_NAME_PACKAGE} PROPERTIES
        BUNDLE True
        MACOSX_RPATH ON
        INSTALL_RPATH "@executable_path/Frameworks"
        MACOSX_PACKAGE_LOCATION "Frameworks"
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