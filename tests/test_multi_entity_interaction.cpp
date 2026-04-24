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

struct World {
    uint64_t tick = 0;
    std::vector<Entity> entities;
};

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

static void sort_entities(World& w) {
    std::sort(w.entities.begin(), w.entities.end(),
        [](const Entity& a, const Entity& b) {
            return a.id < b.id;
        });
}

static void step_world(World& w) {
    sort_entities(w);

    for (auto& e : w.entities) {
        e.x += e.vx;
    }

    // deterministic collision pass
    for (size_t i = 0; i < w.entities.size(); ++i) {
        for (size_t j = i + 1; j < w.entities.size(); ++j) {
            auto& a = w.entities[i];
            auto& b = w.entities[j];

            if (a.x == b.x) {
                // shared-state mutation
                a.hp -= 1;
                b.hp -= 1;

                // deterministic push apart by stable id ordering
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

static World make_world_order_a() {
    World w;
    w.entities.push_back({1, 0, 1, 10});
    w.entities.push_back({2, 4, -1, 10});
    w.entities.push_back({3, 2, 0, 10});
    return w;
}

static World make_world_order_b() {
    World w;
    w.entities.push_back({3, 2, 0, 10});
    w.entities.push_back({2, 4, -1, 10});
    w.entities.push_back({1, 0, 1, 10});
    return w;
}

int main() {
    World a = make_world_order_a();
    World b = make_world_order_b();

    for (int i = 0; i < 20; ++i) {
        step_world(a);
        step_world(b);
    }

    uint64_t ha = hash_world(a);
    uint64_t hb = hash_world(b);

    std::cout << "multi_entity_interaction A=0x"
              << std::hex << ha
              << " B=0x" << hb
              << std::dec << "\n";

    assert(ha == hb && "Entity ordering changed final world state");

    return 0;
}
