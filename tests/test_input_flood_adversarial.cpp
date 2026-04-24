#include <simcore/simcore.hpp>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>
#include "diff_utils.hpp"

int main() {
    const uint64_t total_ticks = 200;

    SimInputLog a;
    SimInputLog b;

    for (uint64_t t = 0; t < total_ticks; ++t) {
        for (int i = 0; i < 50; ++i) {
            a.events.push_back({t, InputType::MoveX, (double)(i % 3)});
            a.events.push_back({t, InputType::MoveY, (double)(-i % 5)});
        }
    }

    // Same exact events, hostile insertion order.
    b.events = a.events;
    std::reverse(b.events.begin(), b.events.end());

    SimState sa = sim_initial_state();
    sim_run_with_input(sa, total_ticks, a);
    uint64_t ha = sim_hash(sa);

    SimState sb = sim_initial_state();
    sim_run_with_input(sb, total_ticks, b);
    uint64_t hb = sim_hash(sb);

    std::cout << "input_flood A=0x"
              << std::hex << ha
              << " B=0x" << hb
              << std::dec << "\n";

    if (ha != hb) {
        std::cerr << "INPUT FLOOD / ORDERING BUG\n";
        std::cerr << "STATE DIFF:\n" << diff_states(sa, sb);
        return 1;
    }

    return 0;
}
