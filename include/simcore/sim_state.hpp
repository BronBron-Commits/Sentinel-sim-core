#include <simcore/platform_guard.hpp>
#pragma once
#include <cstdint>

// =======================================================
// Fixed-point math (Q32.32)
// =======================================================

struct Fixed {
    int64_t raw;

    static constexpr int FRACTION_BITS = 32;

    static Fixed from_int(int64_t v) {
        return Fixed{ v << FRACTION_BITS };
    }

    static Fixed from_double(double v) {
        return Fixed{ static_cast<int64_t>(v * (1LL << FRACTION_BITS)) };
    }

    double to_double() const {
        return static_cast<double>(raw) / static_cast<double>(1LL << FRACTION_BITS);
    }

    Fixed operator+(Fixed o) const { return { raw + o.raw }; }
    Fixed operator-(Fixed o) const { return { raw - o.raw }; }

    Fixed& operator+=(Fixed o) {
        raw += o.raw;
        return *this;
    }
};

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
