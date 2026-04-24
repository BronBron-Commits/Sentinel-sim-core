#include <simcore/simcore.hpp>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
#include "diff_utils.hpp"

int main() {
    const uint64_t total_ticks = 100000;
    const uint64_t snapshot_tick = 50000;

    SimInputLog log;

    for (uint64_t t = 0; t < total_ticks; t += 137) {
        log.events.push_back({t, InputType::MoveX,  1.0});
    }

    for (uint64_t t = 53; t < total_ticks; t += 211) {
        log.events.push_back({t, InputType::MoveY, -2.0});
    }

    for (uint64_t t = 91; t < total_ticks; t += 389) {
        log.events.push_back({t, InputType::MoveZ,  0.5});
    }

    SimState full = sim_initial_state();
    sim_run_with_input(full, total_ticks, log);
    uint64_t h_full = sim_hash(full);

    SimState partial = sim_initial_state();
    sim_run_with_input(partial, snapshot_tick, log);

    SimSnapshot snap = snapshot_state(partial);

    SimState restored{};
    restore_state(restored, snap);

    SimInputLog remaining;
    for (auto e : log.events) {
        if (e.tick >= snapshot_tick) {
            e.tick -= snapshot_tick;
            remaining.events.push_back(e);
        }
    }

    sim_run_with_input(restored, total_ticks - snapshot_tick, remaining);
    uint64_t h_restored = sim_hash(restored);

    std::cout << "long_run_snapshot full=0x"
              << std::hex << h_full
              << " restored=0x" << h_restored
              << std::dec << "\n";

    if (h_full != h_restored) {
        std::cerr << "LONG RUN SNAPSHOT CONSISTENCY FAILED\n";
        std::cerr << "STATE DIFF:\n" << diff_states(full, restored);
        return 1;
    }

    return 0;
}
