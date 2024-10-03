cmake_minimum_required(VERSION 3.10)

project(VenomEngine)

# Source files
set(VenomEngine_Srcs
    main.cc
)

add_executable(${PROJECT_NAME} ${VenomEngine_Srcs})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    VenomCommon
)