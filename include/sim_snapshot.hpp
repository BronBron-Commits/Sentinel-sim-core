#pragma once
#include "sim_state.hpp"

// Pure snapshot of simulation state (no time, no tick)
struct SimSnapshot {
    SimState state;
};
