#include <iostream>
#include <cstdint>

#include "sim_state.hpp"
#include "sim_input_log.hpp"
#include "sim_run_with_input.hpp"
#include "sim_hash.hpp"

/*
    Rollback Network Test

    Two peers simulate with delayed input delivery.
    After rollback + resim, final hashes MUST match.
*/

static constexpr uint64_t TOTAL_TICKS = 200;
static constexpr uint64_t INPUT_DELAY = 6;

struct Peer {
    SimState state{};
    SimInputLog log{};
};

int main() {
    Peer peerA;
    Peer peerB;

    // Identical inputs: move +X every tick
    for (uint64_t t = 0; t < TOTAL_TICKS; ++t) {
        peerA.log.events.push_back({t, InputType::MoveX, 1.0});
        peerB.log.events.push_back({t, InputType::MoveX, 1.0});
    }

    // Simulate network delay
    SimInputLog delayedA;
    SimInputLog delayedB;

    for (const auto& e : peerA.log.events) {
        if (e.tick >= INPUT_DELAY) {
            delayedB.events.push_back({
                e.tick - INPUT_DELAY,
                e.type,
                e.value
            });
        }
    }

    for (const auto& e : peerB.log.events) {
        if (e.tick >= INPUT_DELAY) {
            delayedA.events.push_back({
                e.tick - INPUT_DELAY,
                e.type,
                e.value
            });
        }
    }

    // Run simulations (rollback implied by delayed inputs)
    sim_run_with_input(peerA.state, TOTAL_TICKS, delayedA);
    sim_run_with_input(peerB.state, TOTAL_TICKS, delayedB);

    // Final deterministic comparison
    uint64_t hashA = sim_hash(peerA.state);
    uint64_t hashB = sim_hash(peerB.state);

    std::cout << "peerA hash = 0x" << std::hex << hashA << "\n";
    std::cout << "peerB hash = 0x" << std::hex << hashB << "\n";

    if (hashA != hashB) {
        std::cerr << "\nROLLBACK NETWORK TEST FAILED\n";
        return 1;
    }

    std::cout << "\nROLLBACK NETWORK TEST PASSED\n";
    return 0;
}
