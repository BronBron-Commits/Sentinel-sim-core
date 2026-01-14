#pragma once
#include <cstdint>

struct SimState {
    int64_t x = 0;
    int64_t y = 0;
    int64_t z = 0;

    inline void step() {
        // deterministic baseline step
        x += 0;
        y += 0;
        z += 0;
    }
};
