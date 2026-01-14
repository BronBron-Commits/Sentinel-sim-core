#include <iostream>
#include <cassert>

#include "sim_state.hpp"
#include "sim_hash.hpp"
#include "sim_snapshot.hpp"
#include "sim_snapshot_ops.hpp"
#include "sim_input_log.hpp"
#include "sim_run_with_input.hpp"

int main() {
    constexpr double dt = 0.016;
    constexpr uint64_t total_ticks = 500;

    SimInputLog log;

    // Deterministic inputs
    log.events.push_back({10, InputType::MoveX, 1.0});
    log.events.push_back({20, InputType::MoveY, -2.0});
    log.events.push_back({30, InputType::MoveZ, 0.5});
    log.events.push_back({100, InputType::MoveX, -1.0});

    SimState a{};
    SimState b{};

    uint64_t tick = 0;

    tick = sim_run_with_input(a, tick, total_ticks, dt, log);
    uint64_t hash_a = sim_hash(a);

    // Replay
    tick = 0;
    tick = sim_run_with_input(b, tick, total_ticks, dt, log);
    uint64_t hash_b = sim_hash(b);

    std::cout
        << "A=0x" << std::hex << hash_a
        << " B=0x" << hash_b << std::dec << "\n";

    assert(hash_a == hash_b && "Input replay hash mismatch");

    return 0;
}
