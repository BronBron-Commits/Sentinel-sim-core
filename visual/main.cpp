#include <SDL2/SDL.h>
#include <iostream>

#include "sim_state.hpp"
#include "sim_update.hpp"

static int to_pixels(const Fixed& v, int scale = 1000) {
    return static_cast<int>(v.to_double() * scale);
}

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Sentinel Sim Visual",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SimState state;
    state.x  = Fixed::from_int(0);
    state.y  = Fixed::from_int(0);
    state.vx = Fixed::from_int(1);
    state.vy = Fixed::from_int(1);

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
        }

        sim_update(state);

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        int cx = 320;
        int cy = 240;

        int x = cx + to_pixels(state.x);
        int y = cy + to_pixels(state.y);

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
