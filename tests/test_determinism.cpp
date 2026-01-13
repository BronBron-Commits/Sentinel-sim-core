#include <iostream>
#include "sim_runner.hpp"
#include "sim_hash.hpp"

int main() {
    SimState state{};
    SimRunner runner{};

    constexpr double FIXED_DT = 0.016;
    constexpr int STEPS = 1000;

    for (int i = 0; i < STEPS; ++i) {
        sim_step(runner, state, FIXED_DT, FIXED_DT);
    }

    uint64_t h = hash_state(state);

    std::cout << "ticks=" << runner.tick
              << " sim_time=" << runner.sim_time
              << " state.time=" << state.time
              << " hash=0x" << std::hex << h
              << std::dec << "\n";

    return 0;
}
