#include <simcore/platform_guard.hpp>
#pragma once
#include <cstdint>

enum class InputType : uint8_t {
    None = 0,
    MoveX,
    MoveY,
    MoveZ
};

struct SimInputEvent {
    uint64_t tick;      // simulation tick this input applies to
    InputType type;
    double value;       // deterministic scalar payload
};
