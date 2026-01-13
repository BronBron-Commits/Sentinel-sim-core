#pragma once
#include "sim_state.hpp"

// Exact copy of simulation state at a given tick
struct SimSnapshot {
    SimState state;
    unsigned long long tick;
};
