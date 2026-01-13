#include <iostream>
#include <cassert>

#include "sim_state.hpp"
#include "sim_update.hpp"
#include "sim_hash.hpp"

int main() {
    constexpr double dt = 0.016;
    constexpr int ticks = 1000;

    SimState state{};
    state.time = 0.0;

    for (int i = 0; i < ticks; ++i) {
        sim_update(state, dt);
    }

    uint64_t h = sim_hash(state);

    std::cout << "ticks=" << ticks
              << " sim_time=" << ticks * dt
              << " state.time=" << state.time
              << " hash=0x" << std::hex << h << std::dec << "\n";

    assert(state.time == ticks * dt);

    return 0;
}
