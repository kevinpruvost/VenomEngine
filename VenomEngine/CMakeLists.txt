cmake_minimum_required(VERSION 3.20)

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
add_dependencies(${PROJECT_NAME} link_resources)

if ((NOT APPLE) OR (NOT CMAKE_SYSTEM_NAME MATCHES "iOS"))
    add_dependencies(${PROJECT_NAME} VenomVulkan)
endif ()

if (APPLE)
add_dependencies(${PROJECT_NAME} VenomMetal)
elseif (WIN32)
#add_dependencies(${PROJECT_NAME} VenomDX12)
endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

if (APPLE)
    macro(qt_ios_set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE XCODE_RELVERSION)
        set(XCODE_RELVERSION_I "${XCODE_RELVERSION}")
        if(XCODE_RELVERSION_I STREQUAL "All")
            set_property(TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY} "${XCODE_VALUE}")
        else()
            set_property(TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY}[variant=${XCODE_RELVERSION_I}] "${XCODE_VALUE}")
        endif()
    endmacro() # qt_ios_set_xcode_property


    # if iOS change Info.pList to another file
    if (CMAKE_SYSTEM_NAME MATCHES "iOS")
        set(PLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/iOS/Info.plist")
    else ()
        set(PLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/macOS/Info.plist")
    endif ()


    set(PROJECT_PACKAGE_NAME ${PROJECT_NAME})

    # Define the path to the asset catalog
    set(ASSET_CATALOG_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Assets.xcassets")

    message(STATUS "ASSET_CATALOG_PATH=${ASSET_CATALOG_PATH}")

    set(RESOURCE_FILES_NO_ASSET ${RESOURCE_FILES})
    set(RESOURCE_FILES ${RESOURCE_FILES} ${ASSET_CATALOG_PATH})

    target_sources(${PROJECT_PACKAGE_NAME} PRIVATE
        ${RESOURCE_FILES}
    )

    qt_ios_set_xcode_property (${PROJECT_PACKAGE_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "VenomEngineIcon" "All")

    # Ensure the asset catalog is included in the bundle resources
    set_source_files_properties(${ASSET_CATALOG_PATH}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )

    # Asset catalog app icon set
    list(APPEND app_icon_set "${ASSET_CATALOG_PATH}/VenomEngineIcon.appiconset")
    list(APPEND app_icon_set "${ASSET_CATALOG_PATH}/Contents.json")
    set_source_files_properties(${app_icon_set} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources/Assets.xcassets"
    )

    # Asset catalog icon files
    file(GLOB app_icon_files CONFIGURE_DEPENDS "${ASSET_CATALOG_PATH}/VenomEngineIcon.appiconset/*.png")
    list(APPEND app_icon_files "${ASSET_CATALOG_PATH}/VenomEngineIcon.appiconset/Contents.json")
    set_source_files_properties(${app_icon_files} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources/Assets.xcassets/VenomEngineIcon.appiconset"
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE YES
        MACOSX_BUNDLE_INFO_PLIST "${PLIST_FILE}"
        RESOURCE "${RESOURCE_FILES}"
    )

    # Embed libraries into the bundle
    function(get_all_dependencies target out_var)
        get_target_property(deps ${target} LINK_LIBRARIES)
        if (deps)
            foreach(dep ${deps})
                if (TARGET ${dep})
                    # Check if the target produces a binary file
                    get_target_property(TYPE ${dep} TYPE)
                    if (TYPE STREQUAL "SHARED_LIBRARY" OR TYPE STREQUAL "MODULE_LIBRARY")
#                        list(APPEND result "$<TARGET_FILE:${dep}>")
                        list(APPEND result "${dep}")
                    endif()

                    # Recursively collect dependencies
                    get_all_dependencies(${dep} result)
                endif()
            endforeach()
            set(${out_var} ${result} PARENT_SCOPE)
        endif()
    endfunction()

    get_all_dependencies(${PROJECT_PACKAGE_NAME} EMBEDDED_LIBS)

    list(APPEND EMBEDDED_LIBS ${VENOM_RUNTIME_LOADED_DLLS})

    message(STATUS "EMBEDDED_LIBS=${EMBEDDED_LIBS}")

    set_target_properties(${PROJECT_PACKAGE_NAME} PROPERTIES
        XCODE_EMBED_FRAMEWORKS "${EMBEDDED_LIBS}"
        XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY ON
        XCODE_LINK_BUILD_PHASE_MODE BUILT_ONLY
    )

    set(BUNDLE_APP_LOCATION "${CMAKE_INSTALL_PREFIX}/${PROJECT_PACKAGE_NAME}.app" PARENT_SCOPE)
    set(FRAMEWORKS_BUNDLE_LOCATION "${BUNDLE_APP_LOCATION}/Contents/Frameworks" PARENT_SCOPE)

    set_target_properties(${PROJECT_PACKAGE_NAME} PROPERTIES
            MACOSX_BUNDLE YES
            MACOSX_BUNDLE_INFO_PLIST "${PLIST_FILE}"
            MACOSX_BUNDLE_BUNDLE_NAME "VenomEngine"
            MACOSX_BUNDLE_BUNDLE_VERSION "1.0.0"
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.pruvostkevin.venomengine"
            MACOSX_BUNDLE_ICON_FILE "VenomEngine.icns"
            MACOSX_BUNDLE_INFO_STRING "VenomEngine"
            MACOSX_BUNDLE_LONG_VERSION_STRING "1.0.0"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0.0"
            XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.pruvostkevin.venomengine"

            INSTALL_RPATH "@executable_path/../Frameworks"

            XCODE_LINK_BUILD_PHASE_MODE BUILT_ONLY
            RESOURCE "icons/VenomEngine.icns"
    )

    # Copy resources to bundle
    set(RESOURCES_BUNDLE_LOCATION "${BUNDLE_APP_LOCATION}/Contents/Resources" PARENT_SCOPE)
    foreach (FILE ${RESOURCE_FILES_NO_ASSET})
        # Ignore log folder
        if ("${FILE}" MATCHES "log")
            continue()
        endif()
        file(RELATIVE_PATH NEW_FILE "${CMAKE_SOURCE_DIR}/resources" ${FILE})
        get_filename_component(NEW_FILE_PATH ${NEW_FILE} DIRECTORY)
        message(STATUS "Copying ${FILE} to ${NEW_FILE}")
        set_source_files_properties(${FILE}
                PROPERTIES
                MACOSX_PACKAGE_LOCATION "Resources/${NEW_FILE_PATH}"
                XCODE_FILE_ATTRIBUTES "CodeSignOnCopy"
                HEADER_FILE_ONLY TRUE
        )
        set(RESOURCES_BUNDLE_FILES ${RESOURCES_BUNDLE_FILES} ${RESOURCES_BUNDLE_LOCATION}/${NEW_FILE} PARENT_SCOPE)
    endforeach()

    set_source_files_properties(${ASSET_CATALOG_PATH}
            PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources"
            XCODE_FILE_ATTRIBUTES "CodeSignOnCopy"
            HEADER_FILE_ONLY TRUE
    )

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