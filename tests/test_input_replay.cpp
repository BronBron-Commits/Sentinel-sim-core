#include <cassert>
#include <iostream>

#include "sim_initial_state.hpp"
#include "sim_input_log.hpp"
#include "sim_run_with_input.hpp"
#include "sim_hash.hpp"

int main() {
    SimState a = sim_initial_state();
    SimState b = sim_initial_state();

    SimInputLog log;
    log.events.push_back({0,  InputType::MoveX,  1.0});
    log.events.push_back({5,  InputType::MoveY, -2.0});
    log.events.push_back({10, InputType::MoveZ,  0.5});

    sim_run_with_input(a, 100, log);
    sim_run_with_input(b, 100, log);

    auto ha = sim_hash(a);
    auto hb = sim_hash(b);

    std::cout << "A=0x" << std::hex << ha
              << " B=0x" << hb << std::dec << "\n";

    assert(ha == hb);
    return 0;
}
