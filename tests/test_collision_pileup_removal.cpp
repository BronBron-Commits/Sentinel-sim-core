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

struct World {
    uint64_t tick = 0;
    std::vector<Entity> entities;
};

static void sort_entities(World& w) {
    std::sort(w.entities.begin(), w.entities.end(),
        [](const Entity& a, const Entity& b) {
            return a.id < b.id;
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
        if (!e.alive) continue;
        mix((uint64_t)e.id);
        mix((uint64_t)e.x);
        mix((uint64_t)e.vx);
        mix((uint64_t)e.hp);
    }

    return h;
}

static void remove_dead(World& w) {
    w.entities.erase(
        std::remove_if(
            w.entities.begin(),
            w.entities.end(),
            [](const Entity& e) {
                return !e.alive || e.hp <= 0;
            }
        ),
        w.entities.end()
    );
}

static void step_world(World& w) {
    sort_entities(w);

    for (auto& e : w.entities) {
        if (e.alive) {
            e.x += e.vx;
        }
    }

    // Collision pileup: many entities can share same x.
    // We use stable id order and defer removal until after all collision pairs.
    for (size_t i = 0; i < w.entities.size(); ++i) {
        for (size_t j = i + 1; j < w.entities.size(); ++j) {
            auto& a = w.entities[i];
            auto& b = w.entities[j];

            if (!a.alive || !b.alive) continue;

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

                if (a.hp <= 0) a.alive = false;
                if (b.hp <= 0) b.alive = false;
            }
        }
    }

    remove_dead(w);
    sort_entities(w);
    w.tick++;
}

static World make_world_a() {
    World w;

    // All converge near x=0.
    w.entities.push_back({1, -5,  1, 3, true});
    w.entities.push_back({2, -4,  1, 3, true});
    w.entities.push_back({3, -3,  1, 3, true});
    w.entities.push_back({4, -2,  1, 3, true});
    w.entities.push_back({5, -1,  1, 3, true});
    w.entities.push_back({6,  1, -1, 3, true});
    w.entities.push_back({7,  2, -1, 3, true});
    w.entities.push_back({8,  3, -1, 3, true});
    w.entities.push_back({9,  4, -1, 3, true});
    w.entities.push_back({10, 5, -1, 3, true});

    return w;
}

static World make_world_b() {
    World w;

    // Same entities, hostile insertion order.
    w.entities.push_back({10, 5, -1, 3, true});
    w.entities.push_back({7,  2, -1, 3, true});
    w.entities.push_back({3, -3,  1, 3, true});
    w.entities.push_back({1, -5,  1, 3, true});
    w.entities.push_back({9,  4, -1, 3, true});
    w.entities.push_back({5, -1,  1, 3, true});
    w.entities.push_back({2, -4,  1, 3, true});
    w.entities.push_back({8,  3, -1, 3, true});
    w.entities.push_back({4, -2,  1, 3, true});
    w.entities.push_back({6,  1, -1, 3, true});

    return w;
}

int main() {
    World a = make_world_a();
    World b = make_world_b();

    for (int i = 0; i < 20; ++i) {
        step_world(a);
        step_world(b);
    }

    uint64_t ha = hash_world(a);
    uint64_t hb = hash_world(b);

    std::cout << "collision_pileup_removal A=0x"
              << std::hex << ha
              << " B=0x" << hb
              << std::dec << "\n";

    assert(ha == hb && "Collision pileup/removal was order-dependent");

    return 0;
}
