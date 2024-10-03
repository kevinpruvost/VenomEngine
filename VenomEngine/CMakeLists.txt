cmake_minimum_required(VERSION 3.10)

project(VenomEngine)

# Source files
set(VenomEngine_Srcs
    main.cc
)

add_executable(${PROJECT_NAME} ${VenomEngine_Srcs})

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

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}
)