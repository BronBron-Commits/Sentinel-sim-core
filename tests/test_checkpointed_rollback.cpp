#include <simcore/simcore.hpp>
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
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

static void sort_log(SimInputLog& log) {
    std::sort(log.events.begin(), log.events.end(),
        [](const SimInputEvent& a, const SimInputEvent& b) {
            return a.tick < b.tick;
        });
}

static SimInputLog make_log(uint32_t seed, uint64_t ticks, int count) {
    uint32_t s = seed;
    SimInputLog log;

    for (int i = 0; i < count; ++i) {
        uint64_t tick = rng_next(s) % ticks;
        log.events.push_back({tick, rand_type(s), rand_value(s)});
    }

    sort_log(log);
    return log;
}

static SimInputLog filter_log_from_tick(const SimInputLog& src, uint64_t start_tick) {
    SimInputLog out;

    for (const auto& e : src.events) {
        if (e.tick >= start_tick) {
            auto rebased = e;
            rebased.tick = e.tick - start_tick;
            out.events.push_back(rebased);
        }
    }

    sort_log(out);
    return out;
}

int main() {
    const uint64_t ticks = 360;
    const int cases = 100;
    const int inputs = 160;
    const uint64_t checkpoint_stride = 30;

    for (int c = 0; c < cases; ++c) {
        uint32_t seed = 12000u + (uint32_t)c;
        SimInputLog log = make_log(seed, ticks, inputs);

        SimState authoritative = sim_initial_state();
        sim_run_with_input(authoritative, ticks, log);
        uint64_t h_auth = sim_hash(authoritative);

        std::vector<SimSnapshot> checkpoints;
        std::vector<uint64_t> checkpoint_ticks;

        for (uint64_t checkpoint_tick = 0; checkpoint_tick <= ticks; checkpoint_tick += checkpoint_stride) {
            SimState checkpoint_state = sim_initial_state();
            sim_run_with_input(checkpoint_state, checkpoint_tick, log);

            checkpoints.push_back(snapshot_state(checkpoint_state));
            checkpoint_ticks.push_back(checkpoint_tick);
        }

        uint32_t pick = seed;
        size_t idx = rng_next(pick) % checkpoints.size();

        SimState corrected{};
        restore_state(corrected, checkpoints[idx]);

        uint64_t start_tick = checkpoint_ticks[idx];
        SimInputLog remaining_log = filter_log_from_tick(log, start_tick);

        sim_run_with_input(corrected, ticks - start_tick, remaining_log);

        uint64_t h_corr = sim_hash(corrected);

        if (h_auth != h_corr) {
            std::cerr << "CHECKPOINT ROLLBACK FAIL seed=" << seed
                      << " checkpoint_tick=" << start_tick << "\n";
            std::cerr << "auth=0x" << std::hex << h_auth
                      << " corr=0x" << h_corr << std::dec << "\n";
            std::cerr << "STATE DIFF:\n" << diff_states(authoritative, corrected);
            return 1;
        }
    }

    std::cout << "checkpointed_rollback passed cases=" << cases << "\n";
    return 0;
}
