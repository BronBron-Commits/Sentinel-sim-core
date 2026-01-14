#include <cassert>
#include <iostream>

#include "sim_state.hpp"
#include "sim_update.hpp"
#include "sim_hash.hpp"

int main() {
    SimState state1{};
    SimState state2{};

    const uint64_t steps = 2000;

    for (uint64_t i = 0; i < steps; ++i) {
        sim_update(state1);
        sim_update(state2);
    }

    uint64_t h1 = sim_hash(state1);
    uint64_t h2 = sim_hash(state2);

    std::cout
        << "ticks=" << steps
        << " hash1=0x" << std::hex << h1
        << " hash2=0x" << h2 << std::dec
        << "\n";

    assert(h1 == h2);
    return 0;
}
