#include <simcore/simcore.hpp>
#include <cassert>
#include <cstdint>
#include <iostream>

static uint32_t rng_next(uint32_t& s) {
    s ^= s << 13;
    s ^= s >> 17;
    s ^= s << 5;
    return s;
}

static double rng_value(uint32_t& s) {
    int v = (int)(rng_next(s) % 2001) - 1000;
    return (double)v / 100.0;
}

static InputType rng_input_type(uint32_t& s) {
    switch (rng_next(s) % 3) {
        case 0: return InputType::MoveX;
        case 1: return InputType::MoveY;
        default: return InputType::MoveZ;
    }
}

static SimInputLog make_log(uint32_t seed, uint64_t ticks, int count) {
    uint32_t s = seed;
    SimInputLog log;

    for (int i = 0; i < count; ++i) {
        uint64_t tick = rng_next(s) % ticks;
        InputType type = rng_input_type(s);
        double value = rng_value(s);
        log.events.push_back({tick, type, value});
    }

    return log;
}

int main() {
    const uint64_t ticks = 240;
    const int cases = 250;
    const int inputs_per_case = 64;

    for (int i = 0; i < cases; ++i) {
        uint32_t seed = 1000u + (uint32_t)i;

        SimInputLog log_a = make_log(seed, ticks, inputs_per_case);
        SimInputLog log_b = make_log(seed, ticks, inputs_per_case);

        SimState a = sim_initial_state();
        SimState b = sim_initial_state();

        sim_run_with_input(a, ticks, log_a);
        sim_run_with_input(b, ticks, log_b);

        uint64_t ha = sim_hash(a);
        uint64_t hb = sim_hash(b);

        if (ha != hb) {
            std::cerr << "FUZZ DIVERGENCE seed=" << seed
                      << " A=0x" << std::hex << ha
                      << " B=0x" << hb
                      << std::dec << "\n";
        }

        assert(ha == hb && "Fuzz determinism mismatch");
    }

    std::cout << "fuzz_determinism passed cases=" << cases
              << " ticks=" << ticks
              << " inputs_per_case=" << inputs_per_case
              << "\n";

    return 0;
}
