#pragma once

#include <cstdint>

namespace Haltech {
    enum WboChannels : uint16_t {
        CHA = 0x2b1
        // TODO add IDs for Channels B,C,D as I don't have such on hand
    };

    enum class WboStatus : uint8_t {
        NoError = 0,
        BatteryLow = 1,
        BatteryHigh = 2,
        ShortCircuit = 3,
        OpenCircuit = 4,
        SensorCold = 5,
        FreeAir = 6,
        HeaterShortCircuit = 7,
        CondensationPhase = 8,
        ControllerInactive = 9,
        EngineStopped = 10,
        NotCalibrated = 11,
        Calibrating = 12,
        Unknown = 255
    };

#pragma pack(push, 1)
    struct DataFrame {
        uint8_t pad0 : 1;           // Bit 0 of Byte 0 (to align Lambda to bit 7)
        uint16_t lambda_raw : 16;   // Bit 7–22 (16-bit big-endian Lambda)

        uint8_t pad2[3];            // Skip to Byte 6

        union {
            struct {
                uint8_t reserved_low : 4;     // bits 0–3
                uint8_t lambda_status : 4;    // bits 4–7 of Byte 6
            };
            uint8_t status_byte;              // full byte 6 access if needed
        };

        uint8_t battery_voltage_raw;          // Byte 7
    };
#pragma pack(pop)

    float getLambda(const DataFrame& msg);

    float getBatteryVoltage(const DataFrame& msg);

    WboStatus decodeStatus(const DataFrame& msg);
}
