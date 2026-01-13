#include "sim_hash.hpp"
#include <cstring>

// FNV-1a 64-bit
static uint64_t fnv1a(const void* data, size_t size) {
    const uint8_t* bytes = static_cast<const uint8_t*>(data);
    uint64_t hash = 1469598103934665603ull;
    for (size_t i = 0; i < size; ++i) {
        hash ^= bytes[i];
        hash *= 1099511628211ull;
    }
    return hash;
}

uint64_t hash_state(const SimState& state) {
    // Hash raw bytes of state (POD-only by design)
    return fnv1a(&state, sizeof(SimState));
}
