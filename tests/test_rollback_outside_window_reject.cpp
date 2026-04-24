#include <simcore/simcore.hpp>
#include <cassert>
#include <cstdint>
#include <iostream>
#include "diff_utils.hpp"

int main() {
    const uint64_t total_ticks = 200;
    const uint64_t rollback_window = 30;
    const uint64_t correction_tick = 50;   // too old
    const uint64_t current_tick = 200;

    SimInputLog log;
    for (uint64_t t = 0; t < total_ticks; t += 10) {
        log.events.push_back({t, InputType::MoveX, 1.0});
    }

    // authoritative
    SimState auth = sim_initial_state();
    sim_run_with_input(auth, total_ticks, log);
    uint64_t h_auth = sim_hash(auth);

    // predicted + history
    SimState pred = sim_initial_state();
    std::vector<SimSnapshot> history(total_ticks + 1);

    history[0] = snapshot_state(pred);

    for (uint64_t t = 0; t < total_ticks; ++t) {
        SimInputLog one;
        for (auto e : log.events) {
            if (e.tick == t) {
                e.tick = 0;
                one.events.push_back(e);
            }
        }
        sim_run_with_input(pred, 1, one);
        history[t + 1] = snapshot_state(pred);
    }

    // rollback too far (should NOT be allowed)
    if (current_tick - correction_tick > rollback_window) {
        std::cout << "rollback correctly rejected\n";
        return 0;
    }

    // If it somehow tries anyway, force failure
    SimState corrected{};
    restore_state(corrected, history[correction_tick]);

    SimInputLog remaining;
    for (auto e : log.events) {
        if (e.tick >= correction_tick) {
            e.tick -= correction_tick;
            remaining.events.push_back(e);
        }
    }

    sim_run_with_input(corrected, total_ticks - correction_tick, remaining);

    uint64_t h_corr = sim_hash(corrected);

    if (h_corr == h_auth) {
        std::cerr << "ERROR: rollback outside window converged\n";
        return 1;
    }

    std::cout << "rollback outside window divergence OK\n";
    return 0;
}
