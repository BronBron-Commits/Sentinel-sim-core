cmake_minimum_required(VERSION 3.16)
project(sentinel-sim-core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ----------------------------
# Core library
# ----------------------------
add_library(simcore
    src/sim_update.cpp
    src/sim_runner.cpp
    src/sim_hash.cpp
    src/sim_input_io.cpp
)

target_include_directories(simcore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ----------------------------
# CLI demo
# ----------------------------
add_executable(sentinel-sim-core
    src/main.cpp
)
target_link_libraries(sentinel-sim-core simcore)

# ----------------------------
# Tests
# ----------------------------
enable_testing()

add_executable(test_determinism tests/test_determinism.cpp)
target_link_libraries(test_determinism simcore)
add_test(NAME determinism COMMAND test_determinism)

add_executable(test_rewind tests/test_rewind.cpp)
target_link_libraries(test_rewind simcore)
add_test(NAME rewind COMMAND test_rewind)

add_executable(test_input_replay tests/test_input_replay.cpp)
target_link_libraries(test_input_replay simcore)
add_test(NAME input_replay COMMAND test_input_replay)

add_executable(test_input_file_replay tests/test_input_file_replay.cpp)
target_link_libraries(test_input_file_replay simcore)
add_test(NAME input_file_replay COMMAND test_input_file_replay)

add_executable(test_rollback_network tests/test_rollback_network.cpp)
target_link_libraries(test_rollback_network simcore)
add_test(NAME rollback_network COMMAND test_rollback_network)

# ----------------------------
# SDL Visualizations
# ----------------------------
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

add_executable(sentinel-sim-visual
    visual/main.cpp
)
target_include_directories(sentinel-sim-visual PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(sentinel-sim-visual simcore ${SDL2_LIBRARIES})

add_executable(sentinel-sim-side-by-side
    visual/side_by_side.cpp
)
target_include_directories(sentinel-sim-side-by-side PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(sentinel-sim-side-by-side simcore ${SDL2_LIBRARIES})

