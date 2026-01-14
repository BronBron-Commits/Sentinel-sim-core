#pragma once
#include "sim_state.hpp"

inline void sim_update(SimState& state) {
    state.step();
}
