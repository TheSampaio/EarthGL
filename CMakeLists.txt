# Setup and initialize CMake
cmake_minimum_required(VERSION 3.16)

# Append the configuration name (Debug, Release) dynamically
set(CMAKE_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/out")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_OUTPUT_DIR}/obj) # Static libraries (.lib, .a)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_OUTPUT_DIR}/bin) # Shared libraries (.dll, .so)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_OUTPUT_DIR}/bin) # Executables

# Project setup
project("EarthGL")

# Set C++ standard version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Vendor libraries
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb")

# Captures all .cpp files from the source folder
file(
    GLOB_RECURSE
    PROJECT_SOURCE
    "source/*.cpp"
)

# Create the executable
add_executable(
    ${PROJECT_NAME}
    ${PROJECT_SOURCE}
)

# Setup the pre compiled header file
target_precompile_headers(
    ${PROJECT_NAME} PUBLIC
    "include/PCH.hpp"
)

# Setup the project includes directory
target_include_directories(
    ${CMAKE_PROJECT_NAME}
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Setup the project link library
target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    "glad"
    "glfw"
    "glm"
    "spdlog"
    "stb"
)

if(MSVC)
    # Optimize MSVC
    add_compile_options(/arch:AVX2)

    # Link the C++ runtime libraries statically
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded<$<CONFIG:Debug>:Debug>")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded<$<CONFIG:Release>:Release>")
endif()

# Set this to ON to ship the game!
# Delete the "out" folder after changing it if Visual Studio doesn't recognize the change!
set(SHIPPING_BUILD OFF CACHE BOOL "Make this a shipping build." FORCE)

if(SHIPPING_BUILD)
    # Setup the resource path for project EXE and remove the option to debug asserts
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC PROJECT_RESOURCE_DIR="./resource/")
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC SHIPPING_BUILD=1)

    if(MSVC)
        set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:Windows /ENTRY:mainCRTStartup" )
        target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC _CRT_SECURE_NO_WARNINGS)

    endif()

else()
    # Setup the resource path for project while in IDE and add the option to debug asserts
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC PROJECT_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resource/")
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC SHIPPING_BUILD=0)

endif()

