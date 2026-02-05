cmake_minimum_required(VERSION 3.20)
project(NatureRealityEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Platform detection
if(WIN32)
    add_compile_definitions(PLATFORM_WINDOWS)
    message(STATUS "Building for Windows")
elseif(APPLE)
    add_compile_definitions(PLATFORM_MACOS)
    message(STATUS "Building for macOS")
elseif(UNIX)
    add_compile_definitions(PLATFORM_LINUX)
    message(STATUS "Building for Linux")
endif()

# Graphics API options
option(USE_VULKAN "Use Vulkan renderer" ON)
option(USE_METAL "Use Metal renderer (macOS/iOS)" OFF)
option(USE_DIRECTX12 "Use DirectX 12 (Windows)" OFF)
option(ENABLE_RAY_TRACING "Enable ray tracing support" ON)
option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_EXAMPLES "Build example games" ON)
option(BUILD_EDITOR "Build visual editor" ON)

# Configure graphics API
if(USE_VULKAN)
    add_compile_definitions(USE_VULKAN)
    message(STATUS "Vulkan renderer enabled")
endif()

if(USE_METAL AND APPLE)
    add_compile_definitions(USE_METAL)
    message(STATUS "Metal renderer enabled")
endif()

if(USE_DIRECTX12 AND WIN32)
    add_compile_definitions(USE_DIRECTX12)
    message(STATUS "DirectX 12 renderer enabled")
endif()

if(ENABLE_RAY_TRACING)
    add_compile_definitions(ENABLE_RAY_TRACING)
    message(STATUS "Ray tracing support enabled")
endif()

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/engine
    ${CMAKE_SOURCE_DIR}/runtime
    ${CMAKE_SOURCE_DIR}/editor
)

# Engine library
add_library(NatureRealityEngine INTERFACE)
target_include_directories(NatureRealityEngine INTERFACE
    ${CMAKE_SOURCE_DIR}/engine
)

# Subdirectories
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(BUILD_EDITOR)
    add_subdirectory(editor)
endif()

# Installation
install(DIRECTORY engine/ DESTINATION include/NatureRealityEngine
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")

# Package configuration (optional, for installation)
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.20")
    include(CMakePackageConfigHelper OPTIONAL)
    if(COMMAND write_basic_package_version_file)
        write_basic_package_version_file(
            "${CMAKE_CURRENT_BINARY_DIR}/NatureRealityEngineConfigVersion.cmake"
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY AnyNewerVersion
        )

        install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/NatureRealityEngineConfigVersion.cmake"
            DESTINATION lib/cmake/NatureRealityEngine
        )
    endif()
endif()

# Print configuration summary
message(STATUS "")
message(STATUS "=== Nature Reality Engine Configuration ===")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Vulkan: ${USE_VULKAN}")
message(STATUS "Metal: ${USE_METAL}")
message(STATUS "DirectX 12: ${USE_DIRECTX12}")
message(STATUS "Ray Tracing: ${ENABLE_RAY_TRACING}")
message(STATUS "Build Tests: ${BUILD_TESTS}")
message(STATUS "Build Examples: ${BUILD_EXAMPLES}")
message(STATUS "Build Editor: ${BUILD_EDITOR}")
message(STATUS "==========================================")
message(STATUS "")
