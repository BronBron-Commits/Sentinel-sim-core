#pragma once

#include <cstdint>
#include <simcore/fixed.hpp>

// =======================================================
// Canonical deterministic simulation state
// -------------------------------------------------------
// • No floats
// • No wall-clock time
// • All rollback-safe
// • Hashable byte-for-byte
// =======================================================

struct SimState {
    uint64_t tick = 0;

    Fixed x  = Fixed::from_int(0);
    Fixed y  = Fixed::from_int(0);

    Fixed vx = Fixed::from_int(0);
    Fixed vy = Fixed::from_int(0);
};
