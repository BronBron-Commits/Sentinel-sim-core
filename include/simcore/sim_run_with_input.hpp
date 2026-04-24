#include <simcore/platform_guard.hpp>
#pragma once
#include <simcore/sim_state.hpp>
#include <simcore/sim_input_log.hpp>
#include <simcore/sim_apply_input.hpp>
#include <simcore/sim_update.hpp>
#include <algorithm>

inline void sim_run_with_input(
    SimState& state,
    uint64_t ticks,
    const SimInputLog& log
) {
    SimInputLog sorted_log = log;

    std::sort(sorted_log.events.begin(), sorted_log.events.end(),
        [](const SimInputEvent& a, const SimInputEvent& b) {
            return a.tick < b.tick;
        });

    size_t input_index = 0;

    for (uint64_t tick = 0; tick < ticks; ++tick) {
        while (input_index < sorted_log.events.size() &&
               sorted_log.events[input_index].tick == tick) {
            sim_apply_input(state, sorted_log.events[input_index]);
            ++input_index;
        }

        sim_update(state);
    }
}
