#pragma once

#include "sensor.h"

template <SensorType TSensorType>
class SensorReader final
{
public:
    using sensorValueType = float;

    SensorReader(sensorValueType defaultValue) : m_defaultValue(defaultValue) { }

    SensorResult Get() const
    {
        return Sensor::Get(TSensorType);
    }

    // Get the sensor's reading, or a default value.
    // Inteded for applications where a default may be used silently,
    // while elsewhere in the world the failed sensor is otherwise handled.
    sensorValueType GetOrDefault() const
    {
        auto result = Get();

        if (result.Valid)
        {
            return (sensorValueType)result.Value;
        }
        else
        {
            return m_defaultValue;
        }
    }
private:
    const sensorValueType m_defaultValue;
};
