cmake_minimum_required(VERSION 3.16)
project(sentinel-sim-core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------------------------------
# Core simulation library
# -------------------------------------------------
add_library(simcore STATIC
    src/sim_update.cpp
    src/sim_runner.cpp
    src/sim_hash.cpp
    src/sim_input_io.cpp
)

target_include_directories(simcore PUBLIC
    include
)

# -------------------------------------------------
# Core headless executable
# -------------------------------------------------
add_executable(sentinel-sim-core
    src/main.cpp
)

target_link_libraries(sentinel-sim-core simcore)

# -------------------------------------------------
# Tests
# -------------------------------------------------
add_executable(test_determinism
    tests/test_determinism.cpp
)
target_link_libraries(test_determinism simcore)

add_executable(test_rewind
    tests/test_rewind.cpp
)
target_link_libraries(test_rewind simcore)

add_executable(test_input_replay
    tests/test_input_replay.cpp
)
target_link_libraries(test_input_replay simcore)

add_executable(test_input_file_replay
    tests/test_input_file_replay.cpp
)
target_link_libraries(test_input_file_replay simcore)

add_executable(test_rollback_network
    tests/test_rollback_network.cpp
)
target_link_libraries(test_rollback_network simcore)

# -------------------------------------------------
# SDL Visualizations
# -------------------------------------------------
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

add_executable(sentinel-sim-visual
    visual/main.cpp
)
target_include_directories(sentinel-sim-visual PRIVATE
    ${SDL2_INCLUDE_DIRS}
)
target_link_libraries(sentinel-sim-visual
    simcore
    ${SDL2_LIBRARIES}
)

add_executable(sentinel-sim-side-by-side
    visual/side_by_side.cpp
)
target_include_directories(sentinel-sim-side-by-side PRIVATE
    ${SDL2_INCLUDE_DIRS}
)
target_link_libraries(sentinel-sim-side-by-side
    simcore
    ${SDL2_LIBRARIES}
)

