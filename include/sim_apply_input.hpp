#pragma once
#include "sim_state.hpp"
#include "sim_input.hpp"

// Apply a single deterministic input event to state
inline void sim_apply_input(SimState& state, const SimInputEvent& e) {
    // Convert scalar payload to fixed-point once
    Fixed v = Fixed::from_double(e.value);

    switch (e.type) {
        case InputType::MoveX:
            state.x += v;
            break;

        case InputType::MoveY:
            state.y += v;
            break;

        case InputType::MoveZ:
            // Z no longer exists — intentionally ignored
            break;

        default:
            break;
    }
}
