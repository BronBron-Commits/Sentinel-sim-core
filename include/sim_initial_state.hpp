#pragma once
#include "sim_state.hpp"

// Canonical deterministic starting state
inline SimState sim_initial_state() {
    SimState s{};
    return s;
}
