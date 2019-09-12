#pragma once

#include <cstdint>

// HOW TO ADD A NEW SENSOR:
// 1. Add an entry to the enum below.  Ensure that it is added
//    above the PlaceholderLast element.
// 2. Add appropriate logic to the relevant init file(s) in the
//    init/sensor folder.

enum class SensorType : uint8_t
{
    Invalid = 0,
    Clt,
    Iat,

    OilPressure,

    // This is the "resolved" position, potentially composited out of the following two
    Tps1,
    // This is the first sensor
    Tps1Primary,
    // This is the second sensor
    Tps1Secondary,

    // Leave me at the end!
    PlaceholderLast
};
