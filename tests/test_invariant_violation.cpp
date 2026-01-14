#include <iostream>
#include <cassert>

#include "sim_state.hpp"
#include "sim_update.hpp"
#include "sim_hash.hpp"

/*
    INVARIANT VIOLATION TEST

    This test intentionally breaks an invariant by:
    - Mutating state outside sim_update
    - Advancing time inconsistently

    The test PASSES if hashes DIVERGE.
*/

int main() {
    SimState a{};
    SimState b{};

    // Run both forward identically
    for (int i = 0; i < 100; ++i) {
        sim_update(a);
        sim_update(b);
    }

    // 🚨 INVARIANT VIOLATION:
    // External mutation outside sim_update
    a.x += Fixed::from_int(1);

    uint64_t ha = sim_hash(a);
    uint64_t hb = sim_hash(b);

    std::cout
        << "A hash = 0x" << std::hex << ha << "\n"
        << "B hash = 0x" << hb << std::dec << "\n";

    // Hashes MUST differ
    assert(ha != hb);

    std::cout << "INVARIANT VIOLATION DETECTED (EXPECTED)\n";
    return 0;
}
