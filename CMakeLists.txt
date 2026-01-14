cmake_minimum_required(VERSION 3.10)
project(sentinel-sim-core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(simcore
    src/sim_update.cpp
    src/sim_runner.cpp
    src/sim_hash.cpp
)

add_executable(sentinel-sim-core
    src/main.cpp
)

target_link_libraries(sentinel-sim-core simcore)

add_executable(test_determinism
    tests/test_determinism.cpp
)

target_link_libraries(test_determinism simcore)

add_executable(test_rewind tests/test_rewind.cpp)
target_link_libraries(test_rewind simcore)

add_executable(test_input_replay tests/test_input_replay.cpp)
target_link_libraries(test_input_replay simcore)

# Input log binary I/O
target_sources(simcore PRIVATE
    src/sim_input_io.cpp
)

# Binary replay test
add_executable(test_input_file_replay
    tests/test_input_file_replay.cpp
)
target_link_libraries(test_input_file_replay simcore)
