#pragma once
#include <cstdint>

struct Fixed {
    int64_t raw;

    static constexpr int64_t SCALE = 1000; // 1 unit = 1/1000

    Fixed() : raw(0) {}
    explicit Fixed(int64_t r) : raw(r) {}

    static Fixed from_double(double v) {
        return Fixed(static_cast<int64_t>(v * SCALE));
    }

    double to_double() const {
        return static_cast<double>(raw) / SCALE;
    }

    // Arithmetic
    Fixed operator+(Fixed other) const {
        return Fixed(raw + other.raw);
    }

    Fixed operator-(Fixed other) const {
        return Fixed(raw - other.raw);
    }

    Fixed& operator+=(Fixed other) {
        raw += other.raw;
        return *this;
    }

    Fixed& operator-=(Fixed other) {
        raw -= other.raw;
        return *this;
    }

    Fixed operator*(int64_t scalar) const {
        return Fixed(raw * scalar);
    }
};
