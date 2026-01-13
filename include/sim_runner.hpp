#pragma once
#include "sim_state.hpp"

struct SimRunner {
    double sim_time = 0.0;
    double accumulator = 0.0;
    unsigned long tick = 0;
};

void sim_step(
    SimRunner& runner,
    SimState& state,
    double real_dt,
    double fixed_dt
);
