#include <simcore/simcore.hpp>
#include <cassert>
#include <iostream>
#include "diff_utils.hpp"

int main() {
    const uint64_t ticks = 120;

    SimInputLog a_log;
    a_log.events.push_back({10, InputType::MoveX,  100.0});
    a_log.events.push_back({10, InputType::MoveX, -100.0});
    a_log.events.push_back({10, InputType::MoveY,   50.0});
    a_log.events.push_back({10, InputType::MoveY,  -50.0});

    SimInputLog b_log;
    b_log.events.push_back({10, InputType::MoveY,  -50.0});
    b_log.events.push_back({10, InputType::MoveX, -100.0});
    b_log.events.push_back({10, InputType::MoveY,   50.0});
    b_log.events.push_back({10, InputType::MoveX,  100.0});

    SimState a = sim_initial_state();
    SimState b = sim_initial_state();

    sim_run_with_input(a, ticks, a_log);
    sim_run_with_input(b, ticks, b_log);

    uint64_t ha = sim_hash(a);
    uint64_t hb = sim_hash(b);

    std::cout << "same_tick_order A=0x" << std::hex << ha
              << " B=0x" << hb << std::dec << "\n";

    if (ha != hb) {
        std::cerr << "SAME-TICK ORDER SENSITIVITY FOUND\n";
        std::cerr << "STATE DIFF:\n" << diff_states(a, b);
        return 1;
    }

    return 0;
}
