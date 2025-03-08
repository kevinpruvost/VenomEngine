cmake_minimum_required(VERSION 3.10)

# Do not name same name as folder, it can generate problems with linking (e.g. VenomEngine/ folder in build/
# but VenomEngine target is also named VenomEngine)
project(VenomEngineLauncher)

# Source files
file(GLOB_RECURSE VenomEngine_Srcs
    "./*.cc"
)

set(VenomEngine_Srcs
    ${VenomEngine_Srcs}
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
add_dependencies(${PROJECT_NAME} link_resources VenomVulkan VenomContextGLFW)

if (APPLE)
add_dependencies(${PROJECT_NAME} VenomMetal)
add_dependencies(${PROJECT_NAME} VenomContextApple)
elseif (WIN32)
#add_dependencies(${PROJECT_NAME} VenomDX12)
endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

if (APPLE)
    set(PROJECT_PACKAGE_NAME ${PROJECT_NAME}_Package)

    add_executable(${PROJECT_PACKAGE_NAME}
        ${VenomEngine_Srcs}
    )
    target_include_directories(${PROJECT_PACKAGE_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
    target_link_libraries(${PROJECT_PACKAGE_NAME} PUBLIC
        VenomCommon_Package
    )
    add_dependencies(${PROJECT_PACKAGE_NAME} link_resources VenomVulkan)
    add_dependencies(${PROJECT_PACKAGE_NAME} link_resources VenomMetal)

    target_sources(${PROJECT_PACKAGE_NAME} PRIVATE
        ${RESOURCE_FILES}
    )

    set(BUNDLE_APP_LOCATION "${CMAKE_INSTALL_PREFIX}/${PROJECT_PACKAGE_NAME}.app" PARENT_SCOPE)

    set_target_properties(${PROJECT_PACKAGE_NAME} PROPERTIES
        MACOSX_BUNDLE YES
        MACOSX_BUNDLE_BUNDLE_NAME "VenomEngine"
        MACOSX_BUNDLE_BUNDLE_VERSION "1.0.0"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.venomengine"
        MACOSX_BUNDLE_ICON_FILE "VenomEngine.icns"
        MACOSX_BUNDLE_INFO_STRING "VenomEngine"
        MACOSX_BUNDLE_LONG_VERSION_STRING "1.0.0"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0.0"

        INSTALL_RPATH "@executable_path/../Frameworks"

        XCODE_LINK_BUILD_PHASE_MODE BUILT_ONLY
        RESOURCE "icons/VenomEngine.icns"
    )

    # Copy resources to bundle
    set(RESOURCES_BUNDLE_LOCATION "${BUNDLE_APP_LOCATION}/Contents/Resources" PARENT_SCOPE)
    foreach (FILE ${RESOURCE_FILES})
        # Ignore log folder
        if ("${FILE}" MATCHES "log")
            continue()
        endif()
        file(RELATIVE_PATH NEW_FILE "${CMAKE_SOURCE_DIR}/resources" ${FILE})
        get_filename_component(NEW_FILE_PATH ${NEW_FILE} DIRECTORY)
        set_source_files_properties(${FILE}
                PROPERTIES
                MACOSX_PACKAGE_LOCATION "Resources/${NEW_FILE_PATH}"
                HEADER_FILE_ONLY TRUE
        )
        set(RESOURCES_BUNDLE_FILES ${RESOURCES_BUNDLE_FILES} ${RESOURCES_BUNDLE_LOCATION}/${NEW_FILE} PARENT_SCOPE)
    endforeach()

    # Installation
    install(TARGETS ${PROJECT_PACKAGE_NAME}
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            RUNTIME DESTINATION bin
            BUNDLE DESTINATION .
    )

    # Names of all plugins

    set(CMAKE_XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "-o linker-signed")
    
    install(CODE "set(CODESIGN_COMMAND \"${CODESIGN_COMMAND}\")")
    install(CODE "set(PROJECT_PACKAGE_NAME \"${PROJECT_PACKAGE_NAME}\")")
    install(CODE [[
        include(BundleUtilities)
        # Note Mac specific extension .app
        set(APPS ${CMAKE_INSTALL_PREFIX}/${PROJECT_PACKAGE_NAME}.app)

        # Directories to look for dependencies
        set(DIRS ${CMAKE_INSTALL_PREFIX})

        message(STATUS "APPS=${APPS}")
        message(STATUS "DIRS=${DIRS}")

        file(GLOB PLUGIN_FILES ${APPS}/Contents/Frameworks/*.dylib)
        message(STATUS "PLUGIN_FILES=${PLUGIN_FILES}")

        # NOTE: No need `${DIRS}` when already setting rpath in exectuable in install by `set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")`
        fixup_bundle(${APPS}
        "${PLUGIN_FILES}"
        ${DIRS}
        )

        # Sign every file in the bundle
        file(GLOB_RECURSE DYLIB_FILES_TO_SIGN "${APPS}/Contents/Frameworks/*")
        file(GLOB_RECURSE RESOURCE_FILES_TO_SIGN "${APPS}/Contents/Resources/*")
        file(GLOB_RECURSE BINARY_FILES_TO_SIGN "${APPS}/Contents/MacOS/*")
        file(GLOB_RECURSE IOS_BINARY_FILES_TO_SIGN "${APPS}/Contents/iOS/*")
        set(FILES_TO_SIGN
        ${APPS}
        ${DYLIB_FILES_TO_SIGN}
        #${RESOURCE_FILES_TO_SIGN}
        ${BINARY_FILES_TO_SIGN}
        #${IOS_BINARY_FILES_TO_SIGN}
        )

        foreach(FILE ${FILES_TO_SIGN})
            message(STATUS "Signing file: ${CODESIGN_COMMAND} ${FILE}")
            execute_process(COMMAND ${CODESIGN_COMMAND} "${FILE}"
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                RESULT_VARIABLE CODESIGN_RESULT
                OUTPUT_VARIABLE CODESIGN_OUTPUT
                ERROR_VARIABLE CODESIGN_ERROR
            )
            message(STATUS "CODESIGN_OUTPUT=${CODESIGN_OUTPUT}")
            message(STATUS "CODESIGN_ERROR=${CODESIGN_ERROR}")
            message(STATUS "CODESIGN_RESULT=${CODESIGN_RESULT}")
        endforeach()
    ]])

    set(CPACK_GENERATOR "DRAGNDROP")
    set(CPACK_PACKAGE_NAME "VenomEngine")
    set(CPACK_PACKAGE_VERSION "1.0.0")
    set(CPACK_PACKAGE_VENDOR "VenomEngine")
    set(CPACK_BUNDLE_APPLE_CODESIGN_PARAMETER "--deep -f --options runtime")
    include(CPack)
endif()

