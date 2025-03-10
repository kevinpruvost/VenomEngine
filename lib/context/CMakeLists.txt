
# Skip GLFW on Iphone
if ((NOT APPLE) OR (NOT CMAKE_SYSTEM_NAME MATCHES "iOS"))
    add_subdirectory("glfw")
endif ()
if (APPLE)
    add_subdirectory("apple")
endif()