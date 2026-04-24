#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

struct Entity {
    int id;
    int64_t x;
    int64_t vx;
    bool alive;
};

struct World {
    uint64_t tick = 0;
    std::vector<Entity> entities;
};

struct Snapshot {
    World world;
};

static uint64_t hash_world(const World& w) {
    uint64_t h = 1469598103934665603ULL;

    auto mix = [&](uint64_t v) {
        h ^= v;
        h *= 1099511628211ULL;
    };

    mix(w.tick);

    for (const auto& e : w.entities) {
        if (!e.alive) continue;
        mix((uint64_t)e.id);
        mix((uint64_t)e.x);
        mix((uint64_t)e.vx);
    }

    return h;
}

static void step(World& w) {
    for (auto& e : w.entities) {
        if (e.alive) {
            e.x += e.vx;
        }
    }

    // kill entity id 2 at tick 10
    if (w.tick == 10) {
        for (auto& e : w.entities) {
            if (e.id == 2) {
                e.alive = false;
            }
        }
    }

    // reuse id 2 at tick 20
    if (w.tick == 20) {
        w.entities.push_back({2, 100, -1, true});
    }

    w.tick++;
}

int main() {
    const int total_ticks = 40;
    const int rollback_tick = 15;

    World auth;
    auth.entities.push_back({1, 0, 1, true});
    auth.entities.push_back({2, 5, -1, true});

    // authoritative run
    World a = auth;
    for (int i = 0; i < total_ticks; ++i) {
        step(a);
    }
    uint64_t ha = hash_world(a);

    // predicted with rollback
    World p = auth;
    std::vector<Snapshot> history(total_ticks + 1);

    history[0] = {p};

    for (int t = 0; t < total_ticks; ++t) {
        step(p);
        history[t + 1] = {p};
    }

    // rollback before reuse happens
    World corrected = history[rollback_tick].world;

    for (int t = rollback_tick; t < total_ticks; ++t) {
        step(corrected);
    }

    uint64_t hc = hash_world(corrected);

    std::cout << "id_reuse auth=0x"
              << std::hex << ha
              << " corrected=0x" << hc
              << std::dec << "\n";

    assert(ha == hc && "ID reuse broke rollback determinism");

    return 0;
}
