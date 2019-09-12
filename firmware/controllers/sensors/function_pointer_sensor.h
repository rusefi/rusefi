#pragma once

#include "sensor.h"

#include "efilib.h"

/* This class is intended as a bridge to bridge from old getMySensor() functions
 * to the new system.  This way, producers and consumers can be independently
 * updated to the new system, with sensors being usable either way for some time.
 */
class FunctionPointerSensor final : public Sensor
{
public:
    FunctionPointerSensor(SensorType type, float (*func)())
        : Sensor(type)
        , m_func(func)
    {
    }

    SensorResult Get() const final
    {
        float result = m_func();
        bool valid = !cisnan(result);

        return { valid, result };
    }

private:
    float(*m_func)();
};
