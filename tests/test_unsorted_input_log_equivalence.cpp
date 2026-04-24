#include <simcore/simcore.hpp>
#include <algorithm>
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

    SimInputLog unsorted;
    unsorted.events.push_back({80, InputType::MoveX,  2.0});
    unsorted.events.push_back({10, InputType::MoveY, -3.0});
    unsorted.events.push_back({40, InputType::MoveZ,  1.0});
    unsorted.events.push_back({20, InputType::MoveX, -5.0});

    SimInputLog sorted = unsorted;
    sort_log(sorted);

    SimState a = sim_initial_state();
    SimState b = sim_initial_state();

    sim_run_with_input(a, ticks, sorted);
    sim_run_with_input(b, ticks, unsorted);

    uint64_t ha = sim_hash(a);
    uint64_t hb = sim_hash(b);

    std::cout << "sorted=0x" << std::hex << ha
              << " unsorted=0x" << hb
              << std::dec << "\n";

    if (ha != hb) {
        std::cerr << "UNSORTED INPUT LOG BUG FOUND\n";
        std::cerr << "Same events produce different result based only on order.\n";
        std::cerr << "STATE DIFF:\n" << diff_states(a, b);
        return 1;
    }

    return 0;
}
