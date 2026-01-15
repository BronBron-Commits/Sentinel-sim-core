#include <simcore/sim_update.hpp>
#include <simcore/sim_state.hpp>

// --------------------------------------------
// Deterministic classic projectile motion
// --------------------------------------------

// Precomputed constants (fixed-point safe)
static const Fixed GRAVITY      = Fixed::from_double(-0.015);
static const Fixed LAUNCH_VX    = Fixed::from_double(0.848528); // 1.2 / sqrt(2)
static const Fixed LAUNCH_VY    = Fixed::from_double(0.848528);

void sim_update(SimState &state)
{
    // Launch once
    if (state.tick == 0) {
        state.vx = LAUNCH_VX;
        state.vy = LAUNCH_VY;
    }

    // Integrate position
    state.x += state.vx;
    state.y += state.vy;

    // Apply gravity
    state.vy += GRAVITY;

    // Ground impact clamp
    if (state.y.raw < 0) {
        state.y.raw = 0;
        state.vx.raw = 0;
        state.vy.raw = 0;
    }

    state.tick++;
}
