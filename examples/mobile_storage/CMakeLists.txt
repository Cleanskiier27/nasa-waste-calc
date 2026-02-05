cmake_minimum_required(VERSION 3.20)

project(MobileStorageExample)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(mobile_storage_example
    main.cpp
)

# Include directories
target_include_directories(mobile_storage_example PRIVATE
    ${CMAKE_SOURCE_DIR}
)

# Link libraries (when implemented)
# target_link_libraries(mobile_storage_example PRIVATE
#     NRE_Core
#     NRE_Runtime
# )

# Install target
install(TARGETS mobile_storage_example
    RUNTIME DESTINATION examples
)
