cmake_minimum_required(VERSION 3.10)

# Do not name same name as folder, it can generate problems with linking (e.g. VenomEngine/ folder in build/
# but VenomEngine target is also named VenomEngine)
project(VenomEngine)

# Source files
set(VenomEngine_Srcs
    main.cc
)

add_executable(${PROJECT_NAME}
    ${VenomEngine_Srcs}
)

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC
    VenomCommon
)

# Depend on link_resources to symlink/copy
add_dependencies(${PROJECT_NAME} link_resources VenomVulkan)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

if (APPLE)
    target_sources(${PROJECT_NAME} PRIVATE
        ${RESOURCE_FILES}
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE YES
        MACOSX_BUNDLE_BUNDLE_NAME "VenomEngine_Bundle"
        MACOSX_BUNDLE_BUNDLE_VERSION "1.0.0"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.venomengine"
        MACOSX_BUNDLE_ICON_FILE "VenomEngine.icns"
        MACOSX_BUNDLE_INFO_STRING "VenomEngine"
        MACOSX_BUNDLE_LONG_VERSION_STRING "1.0.0"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0.0"
        XCODE_LINK_BUILD_PHASE_MODE BUILT_ONLY
        RESOURCE "icons/VenomEngine.icns"
    )

    foreach (FILE ${RESOURCE_FILES})
        file(RELATIVE_PATH NEW_FILE "${CMAKE_SOURCE_DIR}/resources" ${FILE})
        get_filename_component(NEW_FILE_PATH ${NEW_FILE} DIRECTORY)
        set_source_files_properties(${FILE}
                PROPERTIES
                MACOSX_PACKAGE_LOCATION "Resources/${NEW_FILE_PATH}"
                HEADER_FILE_ONLY TRUE
        )
    endforeach()

    # Installation
    install(TARGETS ${PROJECT_NAME}
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            RUNTIME DESTINATION bin
            BUNDLE DESTINATION .
    )

    set(CMAKE_XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "-o linker-signed")

    install(CODE [[
        include(BundleUtilities)

        # Note Mac specific extension .app
        set(APPS ${CMAKE_INSTALL_PREFIX}/VenomEngine.app)

        # Directories to look for dependencies
        set(DIRS ${CMAKE_INSTALL_PREFIX})

        message(STATUS "APPS=${APPS}")
        message(STATUS "DIRS=${DIRS}")

        # NOTE: No need `${DIRS}` when already setting rpath in exectuable in install by `set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")`
        fixup_bundle(${APPS} "" ${DIRS})
        # fixup_bundle(${APPS} "" "" COMPONENT Runtime)
    ]])

    set(CPACK_GENERATOR "DRAGNDROP")
    set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
    set(CPACK_PACKAGE_VERSION "1.0.0")
    set(CPACK_PACKAGE_VENDOR "VenomEngine")
    include(CPack)
endif()

