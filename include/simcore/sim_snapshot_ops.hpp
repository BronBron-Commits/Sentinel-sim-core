#include <simcore/platform_guard.hpp>
#pragma once
#include <simcore/sim_snapshot.hpp>
#include <simcore/sim_state.hpp>

inline SimSnapshot snapshot_state(const SimState& s) {
    SimSnapshot snap{};
    snap.state = s;
    return snap;
}

inline void restore_state(SimState& s, const SimSnapshot& snap) {
    s = snap.state;
}
