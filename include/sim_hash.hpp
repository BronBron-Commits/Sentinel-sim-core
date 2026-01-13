#pragma once
#include <cstdint>
#include "sim_state.hpp"

// Deterministic hash of simulation state
uint64_t sim_hash(const SimState& state);
