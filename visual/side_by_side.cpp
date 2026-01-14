#include <SDL.h>
#include <iostream>

#include "sim_state.hpp"
#include "sim_update.hpp"
#include "sim_hash.hpp"
#include "sim_snapshot.hpp"
#include "sim_snapshot_ops.hpp"
#include "sim_input_log.hpp"
#include "sim_run_with_input.hpp"

static constexpr int WIDTH  = 640;
static constexpr int HEIGHT = 480;
static constexpr int SCALE  = 1000;
static constexpr uint64_t STEPS = 300;

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Sentinel Sim – Rollback Repair Demo",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        0
    );

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SimState left{};
    SimState right{};

    SimInputLog full_inputs;
    for (uint64_t t = 0; t < STEPS; ++t) {
        full_inputs.events.push_back({t, InputType::MoveX, 1.0});
    }

    SimSnapshot checkpoint{};
    bool injected = false;
    bool repaired = false;

    bool running = true;
    uint64_t tick = 0;

    while (running && tick < STEPS) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
        }

        // Snapshot before divergence
        if (tick == 80)
            checkpoint = snapshot_state(right);

        // Advance ONE tick using sliced input
        SimInputLog slice;
        for (const auto& ev : full_inputs.events) {
            if (ev.tick == tick)
                slice.events.push_back(ev);
        }

        sim_run_with_input(left,  1, slice);
        sim_run_with_input(right, 1, slice);

        // Inject divergence once
        if (tick == 120 && !injected) {
            std::cout << "[INJECT] divergence injected\n";
            right.x += Fixed::from_int(500);
            injected = true;
        }

        uint64_t hL = sim_hash(left);
        uint64_t hR = sim_hash(right);

        // Rollback + replay
        if (hL != hR && !repaired) {
            std::cout << "[ROLLBACK] restoring snapshot\n";
            restore_state(right, checkpoint);

            std::cout << "[REPLAY] reapplying inputs\n";
            for (uint64_t t = 80; t <= tick; ++t) {
                SimInputLog replay_slice;
                for (const auto& ev : full_inputs.events) {
                    if (ev.tick == t)
                        replay_slice.events.push_back(ev);
                }
                sim_run_with_input(right, 1, replay_slice);
            }

            repaired = true;
        }

        // Render
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        bool synced = sim_hash(left) == sim_hash(right);

        if (!synced)
            SDL_SetRenderDrawColor(renderer, 220, 50, 50, 255);
        else
            SDL_SetRenderDrawColor(renderer, 50, 220, 50, 255);

        int lx = WIDTH / 4  + static_cast<int>(left.x.raw  / SCALE);
        int ly = HEIGHT / 2 + static_cast<int>(left.y.raw / SCALE);

        int rx = 3 * WIDTH / 4 + static_cast<int>(right.x.raw / SCALE);
        int ry = HEIGHT / 2   + static_cast<int>(right.y.raw / SCALE);

        SDL_Rect L{lx - 4, ly - 4, 8, 8};
        SDL_Rect R{rx - 4, ry - 4, 8, 8};

        SDL_RenderFillRect(renderer, &L);
        SDL_RenderFillRect(renderer, &R);
        SDL_RenderPresent(renderer);

        SDL_Delay(16);
        ++tick;
    }

    std::cout << "left  hash  = 0x" << std::hex << sim_hash(left)  << "\n";
    std::cout << "right hash  = 0x" << std::hex << sim_hash(right) << "\n";

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
