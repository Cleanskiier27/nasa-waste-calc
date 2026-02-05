cmake_minimum_required(VERSION 3.20)

# Test framework would go here (e.g., Google Test, Catch2)
# For now, create a simple test structure

add_executable(test_renderer test_renderer.cpp)
target_link_libraries(test_renderer PRIVATE NatureRealityEngine)
target_compile_features(test_renderer PRIVATE cxx_std_20)

add_executable(test_physics test_physics.cpp)
target_link_libraries(test_physics PRIVATE NatureRealityEngine)
target_compile_features(test_physics PRIVATE cxx_std_20)

add_executable(test_ecosystem test_ecosystem.cpp)
target_link_libraries(test_ecosystem PRIVATE NatureRealityEngine)
target_compile_features(test_ecosystem PRIVATE cxx_std_20)

add_executable(test_storage test_storage.cpp)
target_link_libraries(test_storage PRIVATE NatureRealityEngine)
target_compile_features(test_storage PRIVATE cxx_std_20)

# Add tests to CTest
add_test(NAME RendererTest COMMAND test_renderer)
add_test(NAME PhysicsTest COMMAND test_physics)
add_test(NAME EcosystemTest COMMAND test_ecosystem)
add_test(NAME StorageTest COMMAND test_storage)

message(STATUS "Unit tests configured:")
message(STATUS "  - test_renderer")
message(STATUS "  - test_physics")
message(STATUS "  - test_ecosystem")
message(STATUS "  - test_storage")
