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
    bool alive;
};

struct Input {
    uint64_t tick;
    int entity_id;
    int64_t dvx;
    uint64_t packet_id;
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
        [](const Entity& a, const Entity& b) { return a.id < b.id; });
}

static void sort_inputs(std::vector<Input>& inputs) {
    std::sort(inputs.begin(), inputs.end(),
        [](const Input& a, const Input& b) {
            if (a.tick != b.tick) return a.tick < b.tick;
            if (a.entity_id != b.entity_id) return a.entity_id < b.entity_id;
            return a.packet_id < b.packet_id;
        });
}

static uint64_t hash_world(World w) {
    sort_entities(w);

    uint64_t h = 1469598103934665603ULL;
    auto mix = [&](uint64_t v) { h ^= v; h *= 1099511628211ULL; };

    mix(w.tick);

    for (const auto& e : w.entities) {
        if (!e.alive) continue;
        mix((uint64_t)e.id);
        mix((uint64_t)e.x);
        mix((uint64_t)e.vx);
        mix((uint64_t)e.hp);
    }

    return h;
}

static World initial_world() {
    World w;
    w.entities.push_back({1, 0, 1, 10, true});
    w.entities.push_back({2, 8, -1, 10, true});
    w.entities.push_back({3, 4, 0, 10, true});
    return w;
}

static void apply_input(World& w, const Input& in) {
    for (auto& e : w.entities) {
        if (e.alive && e.id == in.entity_id) {
            e.vx += in.dvx;
            return;
        }
    }
}

static void step_world(World& w) {
    sort_entities(w);

    for (auto& e : w.entities) {
        if (e.alive) e.x += e.vx;
    }

    // deterministic collision/damage pass
    for (size_t i = 0; i < w.entities.size(); ++i) {
        for (size_t j = i + 1; j < w.entities.size(); ++j) {
            auto& a = w.entities[i];
            auto& b = w.entities[j];

            if (!a.alive || !b.alive) continue;

            if (a.x == b.x) {
                a.hp -= 1;
                b.hp -= 1;
                a.vx = -a.vx;
                b.vx = -b.vx;
            }
        }
    }

    w.entities.erase(
        std::remove_if(w.entities.begin(), w.entities.end(),
            [](const Entity& e) { return !e.alive || e.hp <= 0; }),
        w.entities.end()
    );

    sort_entities(w);
    w.tick++;
}

static void run_authoritative(World& w, uint64_t total_ticks, std::vector<Input> inputs) {
    sort_inputs(inputs);
    size_t idx = 0;

    for (uint64_t t = 0; t < total_ticks; ++t) {
        while (idx < inputs.size() && inputs[idx].tick == t) {
            apply_input(w, inputs[idx]);
            idx++;
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

static std::vector<Input> make_authoritative_inputs() {
    std::vector<Input> inputs = {
        {5,  1,  1, 1001},
        {8,  2, -1, 1002},
        {12, 3,  2, 1003},
        {20, 1, -2, 1004},
        {22, 2,  3, 1005},
        {35, 3, -1, 1006},
        {40, 1,  1, 1007},
        {50, 2, -2, 1008},
        {65, 3,  1, 1009},
        {70, 1, -1, 1010}
    };

    sort_inputs(inputs);
    return inputs;
}

static std::vector<Input> make_client_visible_inputs(const std::vector<Input>& auth, int client_id) {
    std::vector<Input> out;

    for (auto in : auth) {
        // Client starts missing some remote inputs.
        if (client_id == 1 && in.packet_id == 1005) continue;
        if (client_id == 2 && in.packet_id == 1007) continue;

        out.push_back(in);

        // Add duplicate network packet. Dedup is simulated by packet_id filter below.
        if (in.packet_id == 1003 || in.packet_id == 1008) {
            out.push_back(in);
        }
    }

    // Hostile packet order.
    std::reverse(out.begin(), out.end());
    return out;
}

static std::vector<Input> dedupe_packets(std::vector<Input> inputs) {
    std::sort(inputs.begin(), inputs.end(),
        [](const Input& a, const Input& b) {
            return a.packet_id < b.packet_id;
        });

    inputs.erase(
        std::unique(inputs.begin(), inputs.end(),
            [](const Input& a, const Input& b) {
                return a.packet_id == b.packet_id;
            }),
        inputs.end()
    );

    sort_inputs(inputs);
    return inputs;
}

static World run_client_with_late_correction(
    uint64_t total_ticks,
    uint64_t correction_tick,
    uint64_t rollback_window,
    std::vector<Input> initially_visible,
    std::vector<Input> authoritative_inputs
) {
    World client = initial_world();

    initially_visible = dedupe_packets(initially_visible);
    authoritative_inputs = dedupe_packets(authoritative_inputs);

    std::vector<Snapshot> history(total_ticks + 1);
    history[0] = {client};

    // Predict forward with incomplete visible inputs.
    for (uint64_t t = 0; t < total_ticks; ++t) {
        std::vector<Input> one_tick;

        for (auto in : initially_visible) {
            if (in.tick == t) {
                in.tick = 0;
                one_tick.push_back(in);
            }
        }

        run_authoritative(client, 1, one_tick);
        history[t + 1] = {client};
    }

    // Late server correction arrives.
    assert(total_ticks - correction_tick <= rollback_window &&
           "Correction outside rollback window");

    World corrected = history[correction_tick].world;

    auto remaining = rebase_from_tick(authoritative_inputs, correction_tick);
    run_authoritative(corrected, total_ticks - correction_tick, remaining);

    return corrected;
}

int main() {
    const uint64_t total_ticks = 90;
    const uint64_t correction_tick = 60;
    const uint64_t rollback_window = 40;

    auto authoritative_inputs = make_authoritative_inputs();

    World server = initial_world();
    run_authoritative(server, total_ticks, authoritative_inputs);

    World client_a = run_client_with_late_correction(
        total_ticks,
        correction_tick,
        rollback_window,
        make_client_visible_inputs(authoritative_inputs, 1),
        authoritative_inputs
    );

    World client_b = run_client_with_late_correction(
        total_ticks,
        correction_tick,
        rollback_window,
        make_client_visible_inputs(authoritative_inputs, 2),
        authoritative_inputs
    );

    uint64_t hs = hash_world(server);
    uint64_t ha = hash_world(client_a);
    uint64_t hb = hash_world(client_b);

    std::cout << "full_lockstep server=0x" << std::hex << hs
              << " clientA=0x" << ha
              << " clientB=0x" << hb
              << std::dec << "\n";

    assert(hs == ha && "Client A failed to converge to server authority");
    assert(hs == hb && "Client B failed to converge to server authority");

    return 0;
}
