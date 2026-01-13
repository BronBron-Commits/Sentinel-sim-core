#include "sim_hash.hpp"

uint64_t sim_hash(const SimState& state) {
    const uint64_t prime = 1099511628211ULL;
    uint64_t hash = 1469598103934665603ULL;

    const unsigned char* data =
        reinterpret_cast<const unsigned char*>(&state);

    for (size_t i = 0; i < sizeof(SimState); ++i) {
        hash ^= data[i];
        hash *= prime;
    }

    return hash;
}
