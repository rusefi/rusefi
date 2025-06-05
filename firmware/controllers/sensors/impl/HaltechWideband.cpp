#include "HaltechWideband.h"

namespace Haltech {
    float getLambda(const DataFrame& msg) {
        return static_cast<float>(msg.lambda_raw) * 0.001f;
    }

    float getBatteryVoltage(const DataFrame& msg) {
        return static_cast<float>(msg.battery_voltage_raw) * 0.0778947f;
    }

    WboStatus decodeStatus(const DataFrame& msg) {
        uint8_t status = msg.lambda_status;
        if (status <= 12) {
            return static_cast<WboStatus>(status);
        }
        return WboStatus::Unknown;
    }
}
