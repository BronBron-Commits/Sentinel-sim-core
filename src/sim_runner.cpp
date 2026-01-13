#include "sim_runner.hpp"
#include "sim_update.hpp"

static constexpr int MAX_STEPS_PER_FRAME = 5;

void sim_step(
    SimRunner& runner,
    SimState& state,
    double real_dt,
    double fixed_dt
) {
    runner.accumulator += real_dt;

    int steps = 0;
    while (runner.accumulator >= fixed_dt && steps < MAX_STEPS_PER_FRAME) {
        sim_update(state, fixed_dt);
        runner.sim_time += fixed_dt;
        runner.accumulator -= fixed_dt;
        runner.tick++;
        steps++;
    }

    // hard clamp to prevent spiral of death
    if (steps == MAX_STEPS_PER_FRAME) {
        runner.accumulator = 0.0;
    }
}

unsigned long long sim_run_ticks(
    SimState& state,
    unsigned long long start_tick,
    unsigned long long ticks,
    double dt
) {
    for (unsigned long long i = 0; i < ticks; ++i) {
        sim_update(state, dt);
    }
    return start_tick + ticks;
}
