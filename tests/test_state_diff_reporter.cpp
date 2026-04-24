#include <simcore/simcore.hpp>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

static std::string fixed_to_string(const Fixed& v) {
    std::ostringstream out;
    out << v.raw;
    return out.str();
}

static std::string diff_states(const SimState& a, const SimState& b) {
    std::ostringstream out;

    if (a.tick != b.tick) {
        out << "tick: " << a.tick << " != " << b.tick << "\n";
    }

    if (a.x.raw != b.x.raw) {
        out << "x.raw: " << a.x.raw << " != " << b.x.raw << "\n";
    }

    if (a.y.raw != b.y.raw) {
        out << "y.raw: " << a.y.raw << " != " << b.y.raw << "\n";
    }

    if (a.vx.raw != b.vx.raw) {
        out << "vx.raw: " << a.vx.raw << " != " << b.vx.raw << "\n";
    }

    if (a.vy.raw != b.vy.raw) {
        out << "vy.raw: " << a.vy.raw << " != " << b.vy.raw << "\n";
    }

    return out.str();
}

int main() {
    SimState a = sim_initial_state();
    SimState b = sim_initial_state();

    sim_update(a);
    sim_update(b);

    assert(diff_states(a, b).empty());

    b.x = b.x + Fixed::from_int(1);

    std::string diff = diff_states(a, b);

    std::cout << "STATE DIFF:\n" << diff;

    assert(diff.find("x.raw:") != std::string::npos);
    assert(diff.find("tick:") == std::string::npos);

    return 0;
}
