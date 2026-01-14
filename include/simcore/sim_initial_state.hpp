#include <simcore/platform_guard.hpp>
#pragma once
#include <simcore/sim_state.hpp>

// Canonical deterministic starting state
inline SimState sim_initial_state() {
    SimState s{};
    return s;
}
