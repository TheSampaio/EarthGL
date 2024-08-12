cmake_minimum_required(VERSION 3.16)
project("EarthGL")

set(CXX_STANDARD, 20)
set(CXX_STANDARD_REQUIRED, 20)

file(
    GLOB_RECURSE
    PROJECT_FILE_SOURCE
    "source/*.cpp"
)

include_directories(
    "vendor/glew/include"
    "vendor/glfw/include"
)

add_executable(
    ${PROJECT_NAME}
    ${PROJECT_FILE_SOURCE}
)

target_precompile_headers(
    ${PROJECT_NAME} PUBLIC
    "source/PCH.hpp"
)

target_link_directories(
    ${PROJECT_NAME} PUBLIC
    "vendor/glew/lib/Release/x64"
    "vendor/glfw/lib-vc2022"
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    "opengl32"
    "glew32"
    "glfw3"
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_LIST_DIR}/vendor/glew/bin/Release/x64/glew32.dll"
        "${CMAKE_CURRENT_BINARY_DIR}/Debug"
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_LIST_DIR}/vendor/glew/bin/Release/x64/glew32.dll"
        "${CMAKE_CURRENT_BINARY_DIR}/Release"
)
