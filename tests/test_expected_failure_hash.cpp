#include <simcore/simcore.hpp>
#include <iostream>

int main() {
    SimState state = sim_initial_state();

    SimInputLog log;
    log.events.push_back({10, InputType::MoveX, 1.0});

    sim_run_with_input(state, 60, log);

    uint64_t actual = sim_hash(state);
    uint64_t intentionally_wrong = actual ^ 0x12345678ULL;

    std::cout << "actual=0x" << std::hex << actual
              << " intentionally_wrong=0x" << intentionally_wrong
              << std::dec << "\n";

    if (actual != intentionally_wrong) {
        return 1; // intentional expected failure
    }

    return 0;
}
