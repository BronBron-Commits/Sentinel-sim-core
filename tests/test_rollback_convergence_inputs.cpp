#include <simcore/simcore.hpp>
#include <cassert>
#include <iostream>
#include "diff_utils.hpp"

int main() {
    SimInputLog authoritative_log;
    authoritative_log.events.push_back({10, InputType::MoveX,  1.0});
    authoritative_log.events.push_back({20, InputType::MoveY, -2.0});
    authoritative_log.events.push_back({30, InputType::MoveZ,  0.5});
    authoritative_log.events.push_back({75, InputType::MoveX, -1.0});

    SimState authoritative = sim_initial_state();
    sim_run_with_input(authoritative, 120, authoritative_log);
    uint64_t authoritative_hash = sim_hash(authoritative);

    SimInputLog delayed_log;
    delayed_log.events.push_back({10, InputType::MoveX,  1.0});
    delayed_log.events.push_back({20, InputType::MoveY, -2.0});
    delayed_log.events.push_back({75, InputType::MoveX, -1.0});

    SimState predicted = sim_initial_state();
    sim_run_with_input(predicted, 60, delayed_log);

    SimSnapshot rollback_point = snapshot_state(sim_initial_state());

    SimState corrected{};
    restore_state(corrected, rollback_point);

    sim_run_with_input(corrected, 120, authoritative_log);
    uint64_t corrected_hash = sim_hash(corrected);

    if (authoritative_hash != corrected_hash) {
        std::cerr << "HASH MISMATCH\n";
        std::cerr << "authoritative=0x" << std::hex << authoritative_hash << "\n";
        std::cerr << "corrected=0x" << corrected_hash << "\n";
        std::cerr << "STATE DIFF:\n" << diff_states(authoritative, corrected);
        assert(false && "Rollback did not converge");
    }

    std::cout << "rollback_convergence OK\n";
    return 0;
}
