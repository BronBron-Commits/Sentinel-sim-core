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
        w.entities.push_back({i, i * 10, (i % 5) - 2});
    }
    return w;
}

static void step_single(World& w) {
    sort_entities(w);

    for (auto& e : w.entities) {
        e.x += e.vx;
    }

    w.tick++;
}

static void step_chunked(World& w, const std::vector<int>& chunk_order) {
    sort_entities(w);

    const int chunk_size = 8;
    const int chunk_count = (int)w.entities.size() / chunk_size;

    for (int chunk_id : chunk_order) {
        assert(chunk_id >= 0 && chunk_id < chunk_count);

        int begin = chunk_id * chunk_size;
        int end = begin + chunk_size;

        for (int i = begin; i < end; ++i) {
            w.entities[i].x += w.entities[i].vx;
        }
    }

    w.tick++;
}

int main() {
    World single = make_world();
    World chunks_a = make_world();
    World chunks_b = make_world();

    std::vector<int> order_a = {0,1,2,3,4,5,6,7};
    std::vector<int> order_b = {7,6,5,4,3,2,1,0};

    for (int t = 0; t < 1000; ++t) {
        step_single(single);
        step_chunked(chunks_a, order_a);
        step_chunked(chunks_b, order_b);
    }

    uint64_t hs = hash_world(single);
    uint64_t ha = hash_world(chunks_a);
    uint64_t hb = hash_world(chunks_b);

    std::cout << "parallel_chunk single=0x" << std::hex << hs
              << " chunkA=0x" << ha
              << " chunkB=0x" << hb
              << std::dec << "\n";

    assert(hs == ha && "Chunked order A diverged from single-thread");
    assert(hs == hb && "Chunked order B diverged from single-thread");

    return 0;
}
