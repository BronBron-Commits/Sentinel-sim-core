#include <iostream>
#include <cassert>

#include "sim_state.hpp"
#include "sim_update.hpp"
#include "sim_snapshot.hpp"
#include "sim_snapshot_ops.hpp"
#include "sim_runner.hpp"
#include "sim_hash.hpp"

int main() {
    constexpr double dt = 0.016;
    constexpr unsigned long long total_ticks = 1000;
    constexpr unsigned long long rewind_tick = 400;

    SimState state{};
    state.time = 0.0;

    unsigned long long tick = 0;

    // Run to rewind point
    tick = sim_run_ticks(state, tick, rewind_tick, dt);

    // Take snapshot
    SimSnapshot snap = sim_take_snapshot(state, tick);

    // Run to end
    tick = sim_run_ticks(state, tick, total_ticks - rewind_tick, dt);
    auto hash_full = sim_hash(state);

    // Rewind
    sim_restore_snapshot(state, snap);
    tick = snap.tick;

    // Replay
    tick = sim_run_ticks(state, tick, total_ticks - rewind_tick, dt);
    auto hash_replay = sim_hash(state);

    std::cout << "full=0x" << std::hex << hash_full
              << " replay=0x" << hash_replay << std::dec << "\n";

    assert(hash_full == hash_replay && "Replay hash mismatch!");

    return 0;
}
