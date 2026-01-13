#include <iostream>
#include "sim_runner.hpp"

int main() {
    SimState state{};
    SimRunner runner{};

    constexpr double FIXED_DT = 0.016;
    constexpr int FRAMES = 1000;

    for (int i = 0; i < FRAMES; ++i) {
        sim_step(runner, state, FIXED_DT, FIXED_DT);
    }

    std::cout << "ticks=" << runner.tick
              << " sim_time=" << runner.sim_time
              << " state.time=" << state.time
              << "\n";

    return 0;
}
