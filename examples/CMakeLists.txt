cmake_minimum_required(VERSION 3.20)

# Survival game example
add_executable(survival_game survival_game/main.cpp)
target_link_libraries(survival_game PRIVATE NatureRealityEngine)
target_compile_features(survival_game PRIVATE cxx_std_20)

# Minecraft enhanced example
add_executable(minecraft_enhanced minecraft_enhanced/main.cpp)
target_link_libraries(minecraft_enhanced PRIVATE NatureRealityEngine)
target_compile_features(minecraft_enhanced PRIVATE cxx_std_20)

# Install examples
install(TARGETS survival_game minecraft_enhanced
    RUNTIME DESTINATION bin/examples
)

message(STATUS "Example games configured:")
message(STATUS "  - survival_game")
message(STATUS "  - minecraft_enhanced")
