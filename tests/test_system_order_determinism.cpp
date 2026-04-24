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

static void movement_system(World& w) {
    sort_entities(w);
    for (auto& e : w.entities) {
        if (e.alive) e.x += e.vx;
    }
}

static void hazard_damage_system(World& w) {
    sort_entities(w);
    for (auto& e : w.entities) {
        if (e.alive && e.x == 1) {
            e.hp -= 1;
        }
    }
}

static void cleanup_system(World& w) {
    w.entities.erase(
        std::remove_if(w.entities.begin(), w.entities.end(),
            [](const Entity& e) { return !e.alive || e.hp <= 0; }),
        w.entities.end()
    );
    sort_entities(w);
}

static void step_order_a(World& w) {
    movement_system(w);
    hazard_damage_system(w);
    cleanup_system(w);
    w.tick++;
}

static void step_order_b(World& w) {
    hazard_damage_system(w);
    movement_system(w);
    cleanup_system(w);
    w.tick++;
}

static World initial_world() {
    World w;
    w.entities.push_back({1, 0, 1, 3, true});
    w.entities.push_back({2, 1, 0, 3, true});
    return w;
}

int main() {
    World a = initial_world();
    World b = initial_world();

    for (int i = 0; i < 3; ++i) {
        step_order_a(a);
        step_order_b(b);
    }

    uint64_t ha = hash_world(a);
    uint64_t hb = hash_world(b);

    std::cout << "system_order A=0x" << std::hex << ha
              << " B=0x" << hb << std::dec << "\n";

    if (ha != hb) {
        std::cerr << "SYSTEM ORDER DEPENDENCY FOUND\n";
        std::cerr << "movement->hazard != hazard->movement\n";
        return 1;
    }

    return 0;
}
