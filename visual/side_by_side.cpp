#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

#include "sim_state.hpp"
#include "sim_update.hpp"

static int to_pixels(const Fixed& v, int scale = 200) {
    return static_cast<int>(v.to_double() * scale);
}

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Sentinel Sim — Timeline Scrubber",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // ----------------------------
    // Deterministic simulation
    // ----------------------------
    SimState state{};
    state.vx = Fixed::from_double(0.01);
    state.vy = Fixed::from_double(0.0);

    std::vector<SimState> timeline;
    timeline.reserve(10'000);
    timeline.push_back(state);

    size_t cursor = 0;
    bool paused = false;

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;

            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_SPACE:
                        paused = !paused;
                        break;
                    case SDLK_RIGHT:
                        if (paused && cursor + 1 < timeline.size())
                            cursor++;
                        break;
                    case SDLK_LEFT:
                        if (paused && cursor > 0)
                            cursor--;
                        break;
                }
            }
        }

        if (!paused) {
            sim_update(state);
            timeline.push_back(state);
            cursor = timeline.size() - 1;
        }

        const SimState& view = timeline[cursor];

        // ----------------------------
        // Render
        // ----------------------------
        SDL_SetRenderDrawColor(renderer, 15, 15, 20, 255);
        SDL_RenderClear(renderer);

        int cx = 400;
        int cy = 300;

        int x = cx + to_pixels(view.x);
        int y = cy + to_pixels(view.y);

        if (paused)
            SDL_SetRenderDrawColor(renderer, 240, 220, 80, 255);   // yellow
        else
            SDL_SetRenderDrawColor(renderer, 80, 220, 120, 255);  // green

        SDL_Rect dot{ x - 6, y - 6, 12, 12 };
        SDL_RenderFillRect(renderer, &dot);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
