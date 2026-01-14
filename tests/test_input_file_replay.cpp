#include <cassert>
#include <iostream>

#include "sim_initial_state.hpp"
#include "sim_input_log.hpp"
#include "sim_input_io.hpp"
#include "sim_run_with_input.hpp"
#include "sim_hash.hpp"

int main() {
    SimInputLog original;
    original.events.push_back({0,  InputType::MoveX,  1.0});
    original.events.push_back({5,  InputType::MoveY, -2.0});
    original.events.push_back({10, InputType::MoveZ, 0.5});

    write_input_log(original, "test_input.bin");

    SimInputLog loaded;
    read_input_log(loaded, "test_input.bin");

    SimState a = sim_initial_state();
    SimState b = sim_initial_state();

    sim_run_with_input(a, 100, original);
    sim_run_with_input(b, 100, loaded);

    auto ha = sim_hash(a);
    auto hb = sim_hash(b);

    std::cout << "A=0x" << std::hex << ha
              << " B=0x" << hb << std::dec << "\n";

    assert(ha == hb);
    return 0;
}
