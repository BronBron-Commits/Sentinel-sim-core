#include <simcore/simcore.hpp>
#include <cassert>
#include <iostream>

int main() {
    SimState state = sim_initial_state();

    SimInputLog log;
    log.events.push_back({10, InputType::MoveX,  1.0});
    log.events.push_back({20, InputType::MoveY, -2.0});
    log.events.push_back({30, InputType::MoveZ,  0.5});

    sim_run_with_input(state, 120, log);

    uint64_t hash = sim_hash(state);

    std::cout << "GOLDEN HASH=0x"
              << std::hex << hash
              << std::dec << "\n";

    const uint64_t expected = 0xb2fef4c4b10f6be7ULL;

    assert(hash == expected && "Golden scenario hash mismatch");
    return 0;
}
