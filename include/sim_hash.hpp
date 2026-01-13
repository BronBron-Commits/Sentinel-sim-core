#pragma once
#include <cstdint>
#include "sim_state.hpp"

// Deterministic hash of simulation state
uint64_t hash_state(const SimState& state);
