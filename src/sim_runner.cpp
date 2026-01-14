#include "sim_runner.hpp"
#include "sim_update.hpp"

void run_sim(SimState& state, uint64_t ticks) {
    for (uint64_t i = 0; i < ticks; ++i) {
        sim_update(state);
    }
}
