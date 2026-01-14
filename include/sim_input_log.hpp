#pragma once
#include <vector>
#include <algorithm>
#include <cstdint>
#include "sim_input.hpp"

struct SimInputLog {
    std::vector<SimInputEvent> events;

    void normalize() {
        std::stable_sort(
            events.begin(),
            events.end(),
            [](const SimInputEvent& a, const SimInputEvent& b) {
                if (a.tick != b.tick)
                    return a.tick < b.tick;
                return static_cast<uint8_t>(a.type) <
                       static_cast<uint8_t>(b.type);
            }
        );
    }
};
