#pragma once
#include "sim_state.hpp"
#include "sim_input_log.hpp"
#include "sim_apply_input.hpp"
#include "sim_update.hpp"

inline void sim_run_with_input(
    SimState& state,
    uint64_t ticks,
    const SimInputLog& log
) {
    size_t input_index = 0;

    for (uint64_t tick = 0; tick < ticks; ++tick) {

        while (input_index < log.events.size() &&
               log.events[input_index].tick == tick) {
            sim_apply_input(state, log.events[input_index]);
            ++input_index;
        }

        // 🔑 ALWAYS advance simulation
        sim_update(state);
    }
}
