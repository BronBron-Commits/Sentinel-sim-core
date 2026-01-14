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

# =========================
# Visualization executable
# =========================

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

add_executable(sentinel-sim-visual
    visual/main.cpp
)

target_include_directories(sentinel-sim-visual PRIVATE
    include
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(sentinel-sim-visual
    simcore
    ${SDL2_LIBRARIES}
)

add_executable(sentinel-sim-side-by-side
    visual/side_by_side.cpp
)

target_link_libraries(sentinel-sim-side-by-side
    simcore
    SDL2
)

# --- SDL2 (Termux / pkg-config) ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

target_include_directories(sentinel-sim-side-by-side PRIVATE
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(sentinel-sim-side-by-side
    ${SDL2_LIBRARIES}
)

target_compile_options(sentinel-sim-side-by-side PRIVATE
    ${SDL2_CFLAGS_OTHER}
)
