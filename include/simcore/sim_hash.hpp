#include <simcore/platform_guard.hpp>
#pragma once
#include <cstdint>
#include <simcore/sim_state.hpp>

// Deterministic hash of simulation state
uint64_t sim_hash(const SimState& state);
