
add_subdirectory("external")
add_subdirectory("common")
if(NOT CMAKE_SYSTEM_NAME MATCHES "iOS")
    add_subdirectory("vulkan")
endif ()
if (APPLE)
    add_subdirectory("metal")
endif()