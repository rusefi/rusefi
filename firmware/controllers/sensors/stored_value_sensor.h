#pragma once

#include "sensor.h"

class StoredValueSensor : public Sensor
{
public:
    SensorResult Get() const final
    {
        bool valid = m_isValid;
        float value = m_value;

        return { valid, value };
    }

protected:
    StoredValueSensor(SensorType type) : Sensor(type) {}

    // Invalidate the stored value.
    void Invalidate()
    {
        m_isValid = false;
    }

    // A new reading is available: set and validate a new value for the sensor.
    void SetValidValue(float value)
    {
        // Set value before valid - so we don't briefly have the valid bit set on an invalid value
        m_value = value;
        m_isValid = true;
    }

private:
    bool m_isValid = false;
    float m_value = 0.0f;
};
