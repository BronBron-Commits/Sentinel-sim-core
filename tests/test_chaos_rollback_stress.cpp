#include <simcore/simcore.hpp>
#include <cassert>
#include <cstdint>
#include <iostream>
#include "diff_utils.hpp"

static uint32_t rng_next(uint32_t& s) {
    s ^= s << 13;
    s ^= s >> 17;
    s ^= s << 5;
    return s;
}

static InputType rand_type(uint32_t& s) {
    switch (rng_next(s) % 3) {
        case 0: return InputType::MoveX;
        case 1: return InputType::MoveY;
        default: return InputType::MoveZ;
    }
}

static double rand_value(uint32_t& s) {
    int v = (int)(rng_next(s) % 21) - 10;
    return (double)v / 10.0;
}

static SimInputLog make_authoritative_log(uint32_t seed, uint64_t ticks, int count) {
    uint32_t s = seed;
    SimInputLog log;

    for (int i = 0; i < count; ++i) {
        uint64_t tick = rng_next(s) % ticks;
        log.events.push_back({tick, rand_type(s), rand_value(s)});
    }

    return log;
}

static SimInputLog drop_some_inputs(const SimInputLog& src) {
    SimInputLog out;
    for (size_t i = 0; i < src.events.size(); ++i) {
        if ((i % 5) != 0) {
            out.events.push_back(src.events[i]);
        }
    }
    return out;
}

int main() {
    const uint64_t ticks = 360;
    const int cases = 100;
    const int inputs = 160;

    for (int i = 0; i < cases; ++i) {
        uint32_t seed = 9000u + (uint32_t)i;

        SimInputLog authoritative_log = make_authoritative_log(seed, ticks, inputs);
        SimInputLog lossy_log = drop_some_inputs(authoritative_log);

        SimState authoritative = sim_initial_state();
        sim_run_with_input(authoritative, ticks, authoritative_log);

        SimState predicted = sim_initial_state();
        sim_run_with_input(predicted, ticks / 2, lossy_log);

        SimSnapshot rollback = snapshot_state(sim_initial_state());

        SimState corrected{};
        restore_state(corrected, rollback);
        sim_run_with_input(corrected, ticks, authoritative_log);

        uint64_t ha = sim_hash(authoritative);
        uint64_t hc = sim_hash(corrected);

        if (ha != hc) {
            std::cerr << "CHAOS ROLLBACK FAILED seed=" << seed << "\n";
            std::cerr << "authoritative=0x" << std::hex << ha
                      << " corrected=0x" << hc << std::dec << "\n";
            std::cerr << "STATE DIFF:\n" << diff_states(authoritative, corrected);
            return 1;
        }
    }

    std::cout << "chaos_rollback_stress passed cases=" << cases
              << " ticks=" << ticks
              << " inputs=" << inputs << "\n";

    return 0;
}
