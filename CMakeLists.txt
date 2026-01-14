cmake_minimum_required(VERSION 3.16)
project(sentinel_sim_core LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

# ==============================
# simcore library (STRICT PUBLIC API)
# ==============================
add_library(simcore
    src/sim_update.cpp
    src/sim_runner.cpp
    src/sim_hash.cpp
    src/sim_input_io.cpp
)

# 🔒 Enforce platform isolation ONLY while building simcore
target_compile_definitions(simcore
    PRIVATE SIMCORE_NO_PLATFORM_HEADERS
)

target_include_directories(simcore
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(simcore PUBLIC cxx_std_17)

# ==============================
# Main executable
# ==============================
add_executable(sentinel-sim-core
    src/main.cpp
)
target_link_libraries(sentinel-sim-core simcore)

# ==============================
# Tests
# ==============================
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

add_executable(test_invariant_violation tests/test_invariant_violation.cpp)
target_link_libraries(test_invariant_violation simcore)
add_test(NAME invariant_violation COMMAND test_invariant_violation)
