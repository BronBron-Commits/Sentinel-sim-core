#include <simcore/platform_guard.hpp>
#pragma once
#include <unordered_map>
#include <simcore/sim_state.hpp>
#include <simcore/sim_input_log.hpp>
#include <simcore/sim_snapshot.hpp>

struct PeerState {
    SimState state;
    SimInputLog input_log;
    std::unordered_map<uint64_t, SimSnapshot> snapshots;
    uint64_t current_tick = 0;
};
