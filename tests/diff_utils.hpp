#pragma once
#include <simcore/simcore.hpp>
#include <sstream>
#include <string>

inline std::string diff_states(const SimState& a, const SimState& b) {
    std::ostringstream out;

    if (a.tick != b.tick) out << "tick: " << a.tick << " != " << b.tick << "\n";
    if (a.x.raw  != b.x.raw)  out << "x.raw: "  << a.x.raw  << " != " << b.x.raw  << "\n";
    if (a.y.raw  != b.y.raw)  out << "y.raw: "  << a.y.raw  << " != " << b.y.raw  << "\n";
    if (a.vx.raw != b.vx.raw) out << "vx.raw: " << a.vx.raw << " != " << b.vx.raw << "\n";
    if (a.vy.raw != b.vy.raw) out << "vy.raw: " << a.vy.raw << " != " << b.vy.raw << "\n";

    return out.str();
}
