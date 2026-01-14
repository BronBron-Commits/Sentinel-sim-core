#include <simcore/platform_guard.hpp>
#pragma once
#include <simcore/sim_state.hpp>

// Pure snapshot of simulation state (no time, no tick)
struct SimSnapshot {
    SimState state;
};
