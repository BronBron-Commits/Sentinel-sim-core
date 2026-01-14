#pragma once
#include "sim_state.hpp"
#include "sim_input.hpp"

inline void sim_apply_input(SimState& state, const SimInputEvent& e) {
    switch (e.type) {
        case InputType::MoveX:
            state.x += static_cast<int64_t>(e.value);
            break;
        case InputType::MoveY:
            state.y += static_cast<int64_t>(e.value);
            break;
        case InputType::MoveZ:
            state.z += static_cast<int64_t>(e.value);
            break;
        default:
            break;
    }
}
