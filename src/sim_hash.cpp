#include "sim_hash.hpp"

// Simple deterministic 64-bit mix (FNV-style)
static inline uint64_t mix(uint64_t h, uint64_t v) {
    h ^= v;
    h *= 0x100000001b3ULL;
    return h;
}

uint64_t sim_hash(const SimState& s) {
    uint64_t h = 0xcbf29ce484222325ULL;

    h = mix(h, s.tick);

    h = mix(h, static_cast<uint64_t>(s.x.raw));
    h = mix(h, static_cast<uint64_t>(s.y.raw));

    h = mix(h, static_cast<uint64_t>(s.vx.raw));
    h = mix(h, static_cast<uint64_t>(s.vy.raw));

    return h;
}
