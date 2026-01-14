#include <simcore/platform_guard.hpp>
#pragma once
#include <vector>
#include <simcore/sim_input.hpp>

struct DelayedInput {
    SimInputEvent event;
    uint64_t deliver_tick;
};

struct DelayedInputQueue {
    std::vector<DelayedInput> pending;

    void push(const SimInputEvent& e, uint64_t delay) {
        pending.push_back({e, e.tick + delay});
    }

    std::vector<SimInputEvent> pop_ready(uint64_t tick) {
        std::vector<SimInputEvent> out;
        auto it = pending.begin();
        while (it != pending.end()) {
            if (it->deliver_tick <= tick) {
                out.push_back(it->event);
                it = pending.erase(it);
            } else {
                ++it;
            }
        }
        return out;
    }
};
