#include <simcore/simcore.hpp>
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include "diff_utils.hpp"

static void sort_log(SimInputLog& log) {
    std::sort(log.events.begin(), log.events.end(),
        [](const SimInputEvent& a, const SimInputEvent& b) {
            return a.tick < b.tick;
        });
}

int main() {
    const uint64_t ticks = 120;

    SimInputLog messy;
    messy.events.push_back({119, InputType::MoveX,  999999.0});
    messy.events.push_back({0,   InputType::MoveY, -999999.0});
    messy.events.push_back({60,  InputType::MoveZ,  0.0});
    messy.events.push_back({60,  InputType::MoveX,  1.0});
    messy.events.push_back({60,  InputType::MoveX, -1.0});
    messy.events.push_back({1,   InputType::MoveY,  0.000001});
    messy.events.push_back({1,   InputType::MoveY, -0.000001});
    messy.events.push_back({119, InputType::MoveZ, -999999.0});

    SimInputLog sorted = messy;
    sort_log(sorted);

    SimState a = sim_initial_state();
    SimState b = sim_initial_state();

    sim_run_with_input(a, ticks, sorted);
    sim_run_with_input(b, ticks, sorted);

    uint64_t ha = sim_hash(a);
    uint64_t hb = sim_hash(b);

    if (ha != hb) {
        std::cerr << "EDGE CASE DETERMINISM FAILED\n";
        std::cerr << "A=0x" << std::hex << ha
                  << " B=0x" << hb << std::dec << "\n";
        std::cerr << "STATE DIFF:\n" << diff_states(a, b);
        return 1;
    }

    std::cout << "edge_case_input_stress hash=0x"
              << std::hex << ha << std::dec << "\n";

    return 0;
}
