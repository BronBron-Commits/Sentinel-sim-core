cmake_minimum_required(VERSION 3.10)
project(sentinel-sim-core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sentinel-sim-core
    src/main.cpp
    src/sim_update.cpp
)

target_include_directories(sentinel-sim-core PUBLIC include)
