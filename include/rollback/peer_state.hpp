#pragma once
#include <unordered_map>
#include "sim_state.hpp"
#include "sim_input_log.hpp"
#include "sim_snapshot.hpp"

struct PeerState {
    SimState state;
    SimInputLog input_log;
    std::unordered_map<uint64_t, SimSnapshot> snapshots;
    uint64_t current_tick = 0;
};
