#pragma once
#include "sim_state.hpp"
#include "sim_update.hpp"
#include "sim_input_log.hpp"
#include "sim_apply_input.hpp"

inline uint64_t sim_run_with_input(
    SimState& state,
    uint64_t start_tick,
    uint64_t ticks,
    double dt,
    const SimInputLog& log
) {
    uint64_t tick = start_tick;
    size_t input_index = 0;

    for (uint64_t i = 0; i < ticks; ++i, ++tick) {

        while (input_index < log.events.size() &&
               log.events[input_index].tick == tick) {
            sim_apply_input(state, log.events[input_index]);
            ++input_index;
        }

        sim_update(state, dt);
    }

    return tick;
}
