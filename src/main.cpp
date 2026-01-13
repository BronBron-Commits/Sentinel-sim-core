#include <iostream>
#include "sim_state.hpp"
#include "sim_update.hpp"

int main() {
    SimState state{};
    state.time = 0.0;

    const double dt = 0.016;      // 60 Hz fixed timestep
    double accumulator = 0.0;
    double real_time   = 0.0;

    for (int frame = 0; frame < 5; ++frame) {
        // Simulate variable frame time
        double frame_time = 0.010 + (frame * 0.003);
        real_time += frame_time;
        accumulator += frame_time;

        while (accumulator >= dt) {
            sim_update(state, dt);
            accumulator -= dt;
        }

        std::cout
            << "frame=" << frame
            << " real=" << real_time
            << " sim=" << state.time
            << " acc=" << accumulator
            << "\n";
    }

    return 0;
}
