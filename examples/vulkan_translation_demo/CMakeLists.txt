cmake_minimum_required(VERSION 3.20)
project(vulkan_translation_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Find Vulkan SDK ────────────────────────────────────────────────────────────
find_package(Vulkan QUIET)

add_executable(vulkan_translation_demo main.cpp)
target_link_libraries(vulkan_translation_demo PRIVATE NatureRealityEngine)
target_compile_features(vulkan_translation_demo PRIVATE cxx_std_20)

if(Vulkan_FOUND)
    target_compile_definitions(vulkan_translation_demo PRIVATE VULKAN_SDK_AVAILABLE)
    target_include_directories(vulkan_translation_demo PRIVATE ${Vulkan_INCLUDE_DIRS})
    target_link_libraries(vulkan_translation_demo PRIVATE ${Vulkan_LIBRARIES})
    message(STATUS "vulkan_translation_demo: Vulkan SDK found — full renderer enabled")
else()
    message(STATUS "vulkan_translation_demo: Vulkan SDK NOT found — building in stub mode")
endif()

# Platform definitions (mirror the top-level CMakeLists logic)
if(WIN32)
    target_compile_definitions(vulkan_translation_demo PRIVATE PLATFORM_WINDOWS)
elseif(APPLE)
    target_compile_definitions(vulkan_translation_demo PRIVATE PLATFORM_MACOS)
elseif(UNIX)
    target_compile_definitions(vulkan_translation_demo PRIVATE PLATFORM_LINUX)
endif()

if(ENABLE_RAY_TRACING)
    target_compile_definitions(vulkan_translation_demo PRIVATE ENABLE_RAY_TRACING)
endif()

# Install
install(TARGETS vulkan_translation_demo RUNTIME DESTINATION bin/examples)
