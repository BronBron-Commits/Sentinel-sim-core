#pragma once
#include <cstdint>

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
