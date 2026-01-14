#include <simcore/platform_guard.hpp>
#pragma once
#include <simcore/sim_state.hpp>

// =======================================================
// Canonical deterministic simulation step
// -------------------------------------------------------
// • Fixed timestep
// • No dt parameter
// • No overloads
// • Advances exactly ONE tick
// =======================================================

inline void sim_update(SimState& state) {
    // 🔑 REQUIRED: advance canonical time
    state.tick++;

    // Deterministic state integration
    state.x += state.vx;
    state.y += state.vy;
}
