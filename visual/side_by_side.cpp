#include <SDL.h>
#include <cstdint>
#include <iostream>

#include "sim_state.hpp"
#include "sim_update.hpp"
#include "sim_hash.hpp"

static constexpr int WIDTH  = 640;
static constexpr int HEIGHT = 480;
static constexpr int SCALE  = 1000;

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Sentinel Sim – Side by Side",
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

    left.vx  = Fixed::from_int(1);
    right.vx = Fixed::from_int(1);

    bool injected = false;
    int frames = 0;

    while (frames++ < 300) { // ~5 seconds
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                goto done;
        }

        sim_update(left);
        sim_update(right);

        if (!injected && left.x.raw > 2000) {
            std::cout << "[INJECT] divergence injected\n";
            right.x += Fixed::from_int(1);
            injected = true;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        int lx = 160 + (left.x.raw / SCALE);
        int rx = 480 + (right.x.raw / SCALE);

        SDL_SetRenderDrawColor(renderer, 0, 200, 255, 255);
        SDL_Rect ldot{ lx - 4, 240 - 4, 8, 8 };
        SDL_RenderFillRect(renderer, &ldot);

        SDL_SetRenderDrawColor(renderer, 255, 80, 80, 255);
        SDL_Rect rdot{ rx - 4, 240 - 4, 8, 8 };
        SDL_RenderFillRect(renderer, &rdot);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

done:
    uint64_t hL = sim_hash(left);
    uint64_t hR = sim_hash(right);

    std::cout << "left hash  = 0x" << std::hex << hL << "\n";
    std::cout << "right hash = 0x" << std::hex << hR << "\n";

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
