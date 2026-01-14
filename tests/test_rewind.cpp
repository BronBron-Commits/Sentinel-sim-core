#include <simcore/simcore.hpp>
#include <cassert>
#include <iostream>


int main() {
    SimState state{};

    const uint64_t pre_steps  = 500;
    const uint64_t post_steps = 500;

    // Run forward
    for (uint64_t i = 0; i < pre_steps; ++i) {
        sim_update(state);
    }

    // Snapshot
    SimSnapshot snap = snapshot_state(state);

    // Run further
    for (uint64_t i = 0; i < post_steps; ++i) {
        sim_update(state);
    }

    uint64_t full_hash = sim_hash(state);

    // Rewind
    restore_state(state, snap);

    // Replay
    for (uint64_t i = 0; i < post_steps; ++i) {
        sim_update(state);
    }

    uint64_t replay_hash = sim_hash(state);

    std::cout
        << "full=0x" << std::hex << full_hash
        << " replay=0x" << replay_hash << std::dec
        << "\n";

    assert(full_hash == replay_hash);
    return 0;
}
