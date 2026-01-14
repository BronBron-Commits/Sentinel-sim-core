#include <simcore/sim_runner.hpp>
#include <simcore/sim_apply_input.hpp>
#include <simcore/sim_update.hpp>

namespace simcore {

SimRunner::SimRunner(SimState initial)
    : state_(initial) {}

void SimRunner::push_input(const SimInputEvent& input) {
    sim_apply_input(state_, input);
}

void SimRunner::step() {
    sim_update(state_);
}

const SimState& SimRunner::state() const {
    return state_;
}

} // namespace simcore
