#include <simcore/platform_guard.hpp>
#pragma once
#include <cstdint>

struct InputFileHeader {
    uint32_t magic;        // 'SIMI'
    uint16_t version;      // format version
    uint16_t event_size;   // sizeof(SimInputEvent)
};
