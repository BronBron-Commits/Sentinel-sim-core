#pragma once
#include "sim_state.hpp"
#include "sim_input.hpp"

inline void sim_apply_input(SimState& state, const SimInputEvent& ev) {
    switch (ev.type) {
        case InputType::MoveX:
            state.x += ev.value;
            break;
        case InputType::MoveY:
            state.y += ev.value;
            break;
        case InputType::MoveZ:
            state.z += ev.value;
            break;
        default:
            break;
    }
}
