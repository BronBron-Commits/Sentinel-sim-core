#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>

struct Entity {
    int id;
    int64_t x;
    int64_t vx;
};

struct World {
    uint64_t tick = 0;
    std::vector<Entity> entities;
    int64_t global_impulse = 0; // shared accumulator
};

static void sort_entities(World& w) {
    std::sort(w.entities.begin(), w.entities.end(),
        [](const Entity& a, const Entity& b) { return a.id < b.id; });
}

static uint64_t hash_world(World w) {
    sort_entities(w);
    uint64_t h = 1469598103934665603ULL;
    auto mix = [&](uint64_t v) { h ^= v; h *= 1099511628211ULL; };

    mix(w.tick);
    mix((uint64_t)w.global_impulse);

    for (const auto& e : w.entities) {
        mix((uint64_t)e.id);
        mix((uint64_t)e.x);
        mix((uint64_t)e.vx);
    }
    return h;
}

static World make_world() {
    World w;
    for (int i = 0; i < 64; ++i) {
        w.entities.push_back({i, i * 3, (i % 7) - 3});
    }
    return w;
}

// Deterministic single-thread: reduce then apply
static void step_single(World& w) {
    sort_entities(w);

    int64_t sum = 0;
    for (const auto& e : w.entities) {
        sum += (e.vx * ((e.id % 3) + 1)); // weighted sum
    }
    w.global_impulse += sum;

    for (auto& e : w.entities) {
        e.vx += (w.global_impulse % 5); // apply shared result
        e.x += e.vx;
    }

    w.tick++;
}

// Chunked but CORRECT: local reduce → deterministic combine → apply
static void step_chunked_correct(World& w, const std::vector<int>& order) {
    sort_entities(w);

    const int chunk_size = 8;
    const int chunk_count = (int)w.entities.size() / chunk_size;

    std::vector<int64_t> partials(chunk_count, 0);

    // local reductions (order doesn't matter)
    for (int cid : order) {
        int b = cid * chunk_size;
        int e = b + chunk_size;
        int64_t s = 0;
        for (int i = b; i < e; ++i) {
            s += (w.entities[i].vx * ((w.entities[i].id % 3) + 1));
        }
        partials[cid] = s;
    }

    // deterministic combine (fixed order)
    int64_t sum = 0;
    for (int cid = 0; cid < chunk_count; ++cid) {
        sum += partials[cid];
    }
    w.global_impulse += sum;

    // apply phase (can be chunked in any order)
    for (int cid : order) {
        int b = cid * chunk_size;
        int e = b + chunk_size;
        for (int i = b; i < e; ++i) {
            w.entities[i].vx += (w.global_impulse % 5);
            w.entities[i].x += w.entities[i].vx;
        }
    }

    w.tick++;
}

// Chunked but BUGGY: updates global accumulator during chunk processing (order-dependent)
static void step_chunked_buggy(World& w, const std::vector<int>& order) {
    sort_entities(w);

    const int chunk_size = 8;
    const int chunk_count = (int)w.entities.size() / chunk_size;

    // BAD: mutate shared state per-chunk → depends on order
    for (int cid : order) {
        int b = cid * chunk_size;
        int e = b + chunk_size;

        int64_t s = 0;
        for (int i = b; i < e; ++i) {
            s += (w.entities[i].vx * ((w.entities[i].id % 3) + 1));
        }
        w.global_impulse += s; // order-dependent

        for (int i = b; i < e; ++i) {
            w.entities[i].vx += (w.global_impulse % 5);
            w.entities[i].x += w.entities[i].vx;
        }
    }

    w.tick++;
}

int main() {
    World single = make_world();
    World correct_a = make_world();
    World correct_b = make_world();
    World buggy_a = make_world();
    World buggy_b = make_world();

    std::vector<int> order_a = {0,1,2,3,4,5,6,7};
    std::vector<int> order_b = {7,6,5,4,3,2,1,0};

    for (int t = 0; t < 500; ++t) {
        step_single(single);
        step_chunked_correct(correct_a, order_a);
        step_chunked_correct(correct_b, order_b);
        step_chunked_buggy(buggy_a, order_a);
        step_chunked_buggy(buggy_b, order_b);
    }

    uint64_t hs = hash_world(single);
    uint64_t hca = hash_world(correct_a);
    uint64_t hcb = hash_world(correct_b);
    uint64_t hba = hash_world(buggy_a);
    uint64_t hbb = hash_world(buggy_b);

    std::cout << "parallel_reduction single=0x" << std::hex << hs
              << " correctA=0x" << hca
              << " correctB=0x" << hcb
              << " buggyA=0x" << hba
              << " buggyB=0x" << hbb
              << std::dec << "\n";

    // Correct impl must match single-thread regardless of chunk order
    assert(hs == hca && hs == hcb && "Correct chunked reduction diverged");

    // Buggy impl should differ across orders (detect order dependence)
    if (hba != hbb) {
        std::cout << "ORDER-DEPENDENT REDUCTION DETECTED (expected for buggy path)\n";
        return 0;
    }

    std::cerr << "ERROR: buggy reduction did not show order dependence\n";
    return 1;
}
