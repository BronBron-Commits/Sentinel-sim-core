#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>

struct Entity {
    int id;
    int64_t x;
    int64_t vx;
    int64_t hp;
};

struct Input {
    uint64_t tick;
    int entity_id;
    int64_t dvx;
};

struct World {
    uint64_t tick = 0;
    std::vector<Entity> entities;
};

struct Snapshot {
    World world;
};

static void sort_entities(World& w) {
    std::sort(w.entities.begin(), w.entities.end(),
        [](const Entity& a, const Entity& b) {
            return a.id < b.id;
        });
}

static void sort_inputs(std::vector<Input>& inputs) {
    std::sort(inputs.begin(), inputs.end(),
        [](const Input& a, const Input& b) {
            if (a.tick != b.tick) return a.tick < b.tick;
            return a.entity_id < b.entity_id;
        });
}

static uint64_t hash_world(const World& w) {
    uint64_t h = 1469598103934665603ULL;
    auto mix = [&](uint64_t v) {
        h ^= v;
        h *= 1099511628211ULL;
    };

    mix(w.tick);
    for (const auto& e : w.entities) {
        mix((uint64_t)e.id);
        mix((uint64_t)e.x);
        mix((uint64_t)e.vx);
        mix((uint64_t)e.hp);
    }

    return h;
}

static void apply_input(World& w, const Input& in) {
    for (auto& e : w.entities) {
        if (e.id == in.entity_id) {
            e.vx += in.dvx;
            return;
        }
    }
}

static void step_world(World& w) {
    sort_entities(w);

    for (auto& e : w.entities) {
        e.x += e.vx;
    }

    for (size_t i = 0; i < w.entities.size(); ++i) {
        for (size_t j = i + 1; j < w.entities.size(); ++j) {
            auto& a = w.entities[i];
            auto& b = w.entities[j];

            if (a.x == b.x) {
                a.hp -= 1;
                b.hp -= 1;

                if (a.id < b.id) {
                    a.x -= 1;
                    b.x += 1;
                } else {
                    a.x += 1;
                    b.x -= 1;
                }

                a.vx = -a.vx;
                b.vx = -b.vx;
            }
        }
    }

    w.tick++;
}

static void run_world(World& w, uint64_t ticks, std::vector<Input> inputs) {
    sort_inputs(inputs);

    size_t input_index = 0;

    for (uint64_t local_tick = 0; local_tick < ticks; ++local_tick) {
        while (input_index < inputs.size() && inputs[input_index].tick == local_tick) {
            apply_input(w, inputs[input_index]);
            input_index++;
        }

        step_world(w);
    }
}

static std::vector<Input> rebase_from_tick(const std::vector<Input>& src, uint64_t start_tick) {
    std::vector<Input> out;

    for (auto in : src) {
        if (in.tick >= start_tick) {
            in.tick -= start_tick;
            out.push_back(in);
        }
    }

    sort_inputs(out);
    return out;
}

static World initial_world() {
    World w;
    w.entities.push_back({1, 0, 1, 10});
    w.entities.push_back({2, 8, -1, 10});
    w.entities.push_back({3, 4, 0, 10});
    return w;
}

int main() {
    const uint64_t total_ticks = 80;
    const uint64_t rollback_window = 30;
    const uint64_t correction_tick = 50;

    std::vector<Input> authoritative_inputs = {
        {10, 1,  1},
        {15, 2, -1},
        {30, 3,  2},
        {45, 1, -2},
        {50, 2,  3}
    };

    std::vector<Input> predicted_inputs = {
        {10, 1,  1},
        {15, 2, -1},
        {30, 3,  2},
        {45, 1, -2}
        // missing authoritative correction at tick 50
    };

    World authoritative = initial_world();
    run_world(authoritative, total_ticks, authoritative_inputs);
    uint64_t h_auth = hash_world(authoritative);

    World predicted = initial_world();

    std::vector<Snapshot> history(total_ticks + 1);
    history[0] = {predicted};

    for (uint64_t t = 0; t < total_ticks; ++t) {
        std::vector<Input> one_tick;
        for (auto in : predicted_inputs) {
            if (in.tick == t) {
                in.tick = 0;
                one_tick.push_back(in);
            }
        }

        run_world(predicted, 1, one_tick);
        history[t + 1] = {predicted};
    }

    uint64_t current_tick = total_ticks;

    assert(current_tick - correction_tick <= rollback_window &&
           "Correction outside rollback window");

    World corrected = history[correction_tick].world;

    auto remaining = rebase_from_tick(authoritative_inputs, correction_tick);
    run_world(corrected, total_ticks - correction_tick, remaining);

    uint64_t h_corr = hash_world(corrected);

    std::cout << "authority_prediction_partial_rollback auth=0x"
              << std::hex << h_auth
              << " corrected=0x" << h_corr
              << std::dec << "\n";

    assert(h_auth == h_corr && "Partial rollback did not converge to authority");

    return 0;
}
