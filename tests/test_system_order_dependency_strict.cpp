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
        [](const Entity& a, const Entity& b) { return a.id < b.id; });
}

static uint64_t hash_world(const World& w) {
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

static void targeting_system(World& w) {
    sort_entities(w);
    for (auto& e : w.entities) {
        if (!e.alive) continue;

        // order-sensitive: checks where movement WOULD put the entity
        if (e.x + e.vx == 5) {
            e.vx = 0;
        }
    }
}

static void movement_system(World& w) {
    sort_entities(w);
    for (auto& e : w.entities) {
        if (e.alive) e.x += e.vx;
    }
}

static void step_A_then_B(World& w) {
    targeting_system(w);
    movement_system(w);
    w.tick++;
}

static void step_B_then_A(World& w) {
    movement_system(w);
    targeting_system(w);
    w.tick++;
}

static World initial_world() {
    World w;
    w.entities.push_back({1, 4, 1, 10, true});
    return w;
}

int main() {
    World a = initial_world();
    World b = initial_world();

    step_A_then_B(a);
    step_B_then_A(b);

    uint64_t ha = hash_world(a);
    uint64_t hb = hash_world(b);

    std::cout << "system_dependency A=0x"
              << std::hex << ha
              << " B=0x" << hb
              << std::dec << "\n";

    if (ha != hb) {
        std::cout << "SYSTEM ORDER DEPENDENCY FOUND AS EXPECTED\n";
        return 0;
    }

    std::cerr << "ERROR: expected system order dependency, but hashes matched\n";
    return 1;
}
