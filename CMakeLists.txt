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

add_executable(test_golden_scenario tests/test_golden_scenario.cpp)
target_link_libraries(test_golden_scenario simcore)
add_test(NAME golden_scenario COMMAND test_golden_scenario)


add_executable(test_snapshot_input_exactness tests/test_snapshot_input_exactness.cpp)
target_link_libraries(test_snapshot_input_exactness simcore)
add_test(NAME snapshot_input_exactness COMMAND test_snapshot_input_exactness)


add_executable(test_rollback_convergence_inputs tests/test_rollback_convergence_inputs.cpp)
target_link_libraries(test_rollback_convergence_inputs simcore)
add_test(NAME rollback_convergence_inputs COMMAND test_rollback_convergence_inputs)


add_executable(test_fuzz_determinism tests/test_fuzz_determinism.cpp)
target_link_libraries(test_fuzz_determinism simcore)
add_test(NAME fuzz_determinism COMMAND test_fuzz_determinism)


add_executable(test_state_diff_reporter tests/test_state_diff_reporter.cpp)
target_link_libraries(test_state_diff_reporter simcore)
add_test(NAME state_diff_reporter COMMAND test_state_diff_reporter)


add_executable(test_expected_failure_hash tests/test_expected_failure_hash.cpp)
target_link_libraries(test_expected_failure_hash simcore)
add_test(NAME expected_failure_hash COMMAND test_expected_failure_hash)
set_tests_properties(expected_failure_hash PROPERTIES WILL_FAIL TRUE)


set_tests_properties(expected_failure_hash PROPERTIES WILL_FAIL TRUE)


add_executable(test_first_divergence tests/test_first_divergence.cpp)
target_link_libraries(test_first_divergence simcore)
add_test(NAME first_divergence COMMAND test_first_divergence)


add_executable(test_chaos_rollback_stress tests/test_chaos_rollback_stress.cpp)
target_link_libraries(test_chaos_rollback_stress simcore)
add_test(NAME chaos_rollback_stress COMMAND test_chaos_rollback_stress)


add_executable(test_checkpointed_rollback tests/test_checkpointed_rollback.cpp)
target_link_libraries(test_checkpointed_rollback simcore)
add_test(NAME checkpointed_rollback COMMAND test_checkpointed_rollback)

