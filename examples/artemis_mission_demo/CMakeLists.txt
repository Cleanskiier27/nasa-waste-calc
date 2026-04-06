cmake_minimum_required(VERSION 3.20)

add_executable(artemis_mission_demo main.cpp)

target_link_libraries(artemis_mission_demo
    PRIVATE
    NatureRealityEngine
)

target_compile_features(artemis_mission_demo PRIVATE cxx_std_20)

install(TARGETS artemis_mission_demo
    RUNTIME DESTINATION bin/examples
)

message(STATUS "Artemis Mission Demo configured")
