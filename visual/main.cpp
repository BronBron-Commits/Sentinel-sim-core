#include <SDL2/SDL.h>
#include <iostream>

#include "sim_state.hpp"
#include "sim_update.hpp"

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL init failed\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Sentinel Sim Visual",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_SOFTWARE
    );

    SimState state{};
    state.x = 0;
    state.y = 0;

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
        }

        sim_update(state);

        // ----- CLEAR -----
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        const int cx = 320;
        const int cy = 240;

        // ----- AXES -----
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, 0, cy, 640, cy);

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderDrawLine(renderer, cx, 0, cx, 480);

        // ----- SIM POINT -----
        int x = cx + static_cast<int>(state.x / 1000);
        int y = cy + static_cast<int>(state.y / 1000);

        SDL_SetRenderDrawColor(renderer, 0, 200, 255, 255);
        SDL_Rect dot{ x - 4, y - 4, 8, 8 };
        SDL_RenderFillRect(renderer, &dot);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
