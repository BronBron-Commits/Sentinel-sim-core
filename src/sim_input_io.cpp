#include <simcore/sim_input_io.hpp>
#include <simcore/sim_input_log.hpp>
#include <simcore/sim_input.hpp>

#include <fstream>
#include <algorithm>

bool write_input_log(const SimInputLog& log, const std::string& path) {
    std::ofstream out(path, std::ios::binary);
    if (!out) return false;

    uint64_t count = log.events.size();
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const auto& e : log.events) {
        uint64_t tick = e.tick;
        uint8_t  type = static_cast<uint8_t>(e.type);
        double   val  = e.value;

        out.write(reinterpret_cast<const char*>(&tick), sizeof(tick));
        out.write(reinterpret_cast<const char*>(&type), sizeof(type));
        out.write(reinterpret_cast<const char*>(&val),  sizeof(val));
    }

    return true;
}

bool read_input_log(SimInputLog& log, const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return false;

    uint64_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));

    log.events.clear();
    log.events.reserve(count);

    for (uint64_t i = 0; i < count; ++i) {
        uint64_t tick;
        uint8_t  type;
        double   val;

        in.read(reinterpret_cast<char*>(&tick), sizeof(tick));
        in.read(reinterpret_cast<char*>(&type), sizeof(type));
        in.read(reinterpret_cast<char*>(&val),  sizeof(val));

        log.events.push_back({
            tick,
            static_cast<InputType>(type),
            val
        });
    }

    std::sort(
        log.events.begin(),
        log.events.end(),
        [](const SimInputEvent& a, const SimInputEvent& b) {
            return a.tick < b.tick;
        }
    );

    return true;
}
