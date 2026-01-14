#include <iostream>

#include "sim_state.hpp"
#include "sim_update.hpp"
#include "sim_hash.hpp"

int main() {
    SimState state{};          // pure state, no tick
    const uint64_t steps = 1000;

    for (uint64_t tick = 0; tick < steps; ++tick) {
        sim_update(state);
    }

    uint64_t h = sim_hash(state);

    std::cout
        << "ticks=" << steps
        << " hash=0x" << std::hex << h << std::dec
        << "\n";

    return 0;
}
