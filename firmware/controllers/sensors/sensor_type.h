#pragma once

#include <cstdint>

enum class SensorType : uint8_t
{
    Invalid = 0,
    Clt,
    Iat,

    // This is the "resolved" position, potentially composited out of the following two
    Tps1,
    // This is the first sensor
    Tps1Primary,
    // This is the second sensor
    Tps1Secondary,

    // Leave me at the end!
    PlaceholderLast
};
