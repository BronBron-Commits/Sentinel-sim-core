#pragma once
#include "sim_snapshot.hpp"

// Capture current simulation state
inline SimSnapshot sim_take_snapshot(const SimState& state, unsigned long long tick) {
    return SimSnapshot{ state, tick };
}

// Restore simulation state
inline void sim_restore_snapshot(SimState& state, const SimSnapshot& snap) {
    state = snap.state;
}
