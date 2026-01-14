#include <simcore/platform_guard.hpp>
#pragma once
#include <string>

struct SimInputLog;

// Reads/writes deterministic input logs.
// NOTE: Input ticks are normalized on load so replay starts at tick 0.
bool write_input_log(const SimInputLog& log, const std::string& path);
bool read_input_log(SimInputLog& log, const std::string& path);
