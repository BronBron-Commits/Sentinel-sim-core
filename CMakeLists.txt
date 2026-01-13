cmake_minimum_required(VERSION 3.10)
project(sentinel-sim-core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(simcore
    src/sim_update.cpp
    src/sim_runner.cpp
)

add_executable(sentinel-sim-core
    src/main.cpp
)

target_link_libraries(sentinel-sim-core simcore)

add_executable(test_determinism
    tests/test_determinism.cpp
)

target_link_libraries(test_determinism simcore)
