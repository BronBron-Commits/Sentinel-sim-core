#include <simcore/simcore.hpp>
#include <cassert>
#include <iostream>

int main() {
    SimInputLog log;
    log.events.push_back({10, InputType::MoveX,  1.0});
    log.events.push_back({20, InputType::MoveY, -2.0});
    log.events.push_back({30, InputType::MoveZ,  0.5});

    SimState a = sim_initial_state();

    sim_run_with_input(a, 60, log);

    SimSnapshot snap = snapshot_state(a);

    sim_run_with_input(a, 120, log);
    uint64_t h1 = sim_hash(a);

    SimState b{};
    restore_state(b, snap);

    sim_run_with_input(b, 120, log);
    uint64_t h2 = sim_hash(b);

    std::cout << "snapshot_input_exactness A=0x"
              << std::hex << h1
              << " B=0x" << h2
              << std::dec << "\n";

    assert(h1 == h2 && "Snapshot restore with inputs did not converge");
    return 0;
}
