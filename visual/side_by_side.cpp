#include <SDL2/SDL.h>
#include <iostream>
#include <cstdint>

#include "sim_state.hpp"
#include "sim_update.hpp"
#include "sim_hash.hpp"
#include "sim_snapshot_ops.hpp"

static int fx(const Fixed& v) {
    return static_cast<int>(v.to_double() * 10.0);
}

static void draw_dot(SDL_Renderer* r, int x, int y, SDL_Color c) {
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
    SDL_Rect dot{ x - 4, y - 4, 8, 8 };
    SDL_RenderFillRect(r, &dot);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Sentinel Rollback Visualization",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640, 480,
        0
    );

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SimState left{};
    SimState right{};

    left.vx  = Fixed::from_int(1);
    right.vx = Fixed::from_int(1);

    SimSnapshot snap{};
    bool diverged  = false;
    bool converged = false;

    for (int frame = 0; frame < 240; ++frame) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) goto quit;
        }

        sim_update(left);
        sim_update(right);

        if (frame == 50 && !diverged) {
            snap = snapshot_state(left);
            std::cout << "[INJECT] divergence injected\n";
            right.x += Fixed::from_int(20);
            diverged = true;
        }

        if (frame == 120 && !converged) {
            std::cout << "[ROLLBACK] restoring snapshot\n";
            restore_state(right, snap);
            converged = true;
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        SDL_Color color =
            converged ? SDL_Color{0, 255, 0, 255}
                      : diverged ? SDL_Color{255, 0, 0, 255}
                                 : SDL_Color{0, 200, 255, 255};

        draw_dot(renderer, 160 + fx(left.x),  240 + fx(left.y),  color);
        draw_dot(renderer, 480 + fx(right.x), 240 + fx(right.y), color);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

quit:
    std::cout << "left  hash  = 0x" << std::hex << sim_hash(left)  << "\n";
    std::cout << "right hash  = 0x" << std::hex << sim_hash(right) << "\n";

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
