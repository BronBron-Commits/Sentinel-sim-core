#include <simcore/simcore.hpp>
#include <cassert>
#include <iostream>
#include "diff_utils.hpp"

int main() {
    SimInputLog log;
    log.events.push_back({10, InputType::MoveX,  1.0});
    log.events.push_back({20, InputType::MoveY, -2.0});
    log.events.push_back({30, InputType::MoveZ,  0.5});

    SimState a = sim_initial_state();
    SimState b = sim_initial_state();

    const uint64_t max_ticks = 200;

    for (uint64_t t = 0; t < max_ticks; ++t) {
        // introduce controlled divergence
        if (t == 50) {
            b.x = b.x + Fixed::from_int(1);
        }

        sim_update(a);
        sim_update(b);

        uint64_t ha = sim_hash(a);
        uint64_t hb = sim_hash(b);

        if (ha != hb) {
            std::cout << "FIRST DIVERGENCE AT TICK " << t << "\n";
            std::cout << "A=0x" << std::hex << ha
                      << " B=0x" << hb << std::dec << "\n";
            std::cout << "STATE DIFF:\n" << diff_states(a, b);
            return 0;
        }
    }

    assert(false && "Expected divergence not found");
    return 1;
}
