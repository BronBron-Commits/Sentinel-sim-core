#pragma once

#include <simcore/platform_guard.hpp>

#include <simcore/sim_state.hpp>
#include <simcore/sim_input.hpp>
#include <simcore/sim_snapshot.hpp>

namespace simcore {

class SimRunner {
public:
    explicit SimRunner(SimState initial);

    void push_input(const SimInputEvent& input);
    void step();

    const SimState& state() const;

private:
    SimState state_;
    SimSnapshot snapshot_;
};

} // namespace simcore
