#include "sim_update.hpp"

void sim_update(SimState &state, double dt) {
    state.time += dt;
}
