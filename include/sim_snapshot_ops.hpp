#pragma once
#include "sim_snapshot.hpp"
#include "sim_state.hpp"

inline SimSnapshot snapshot_state(const SimState& s) {
    SimSnapshot snap{};
    snap.state = s;
    return snap;
}

inline void restore_state(SimState& s, const SimSnapshot& snap) {
    s = snap.state;
}
