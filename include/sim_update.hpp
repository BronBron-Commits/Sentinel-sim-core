#pragma once
#include "sim_state.hpp"

// =======================================================
// Canonical deterministic simulation step
// -------------------------------------------------------
// • Fixed timestep
// • No dt parameter
// • No overloads
// • Advances exactly ONE tick
// =======================================================

inline void sim_update(SimState& state) {
    // Deterministic state integration
    state.x += state.vx;
    state.y += state.vy;
}
