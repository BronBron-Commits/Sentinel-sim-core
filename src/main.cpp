#include <iostream>

#include <simcore/sim_state.hpp>
#include <simcore/sim_update.hpp>
#include <simcore/sim_hash.hpp>
#include <simcore/sim_initial_state.hpp>

int main()
{
    SimState state = sim_initial_state();

    for (int i = 0; i < 60; ++i)
    {
        sim_update(state);
        uint64_t h = sim_hash(state);

        std::cout
            << "tick=" << i
            << " hash=0x"
            << std::hex << h << std::dec
            << "\n";
    }

    return 0;
}
