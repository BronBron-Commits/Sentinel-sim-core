#include "sim_hash.hpp"

/* Simple avalanche mixer */
static inline uint64_t mix(uint64_t x) {
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;
    return x;
}

uint64_t sim_hash(const SimState& s) {
    uint64_t h = 0xcbf29ce484222325ULL;

    h ^= mix(static_cast<uint64_t>(s.x));
    h ^= mix(static_cast<uint64_t>(s.y));
    h ^= mix(static_cast<uint64_t>(s.z));

    return h;
}
