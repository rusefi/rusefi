#pragma once

#include "stored_value_sensor.h"

class ConvertedSensor : public StoredValueSensor {
public:
    void PostRawValue(float inputValue) {
        auto r = ConvertFromInputValue(inputValue);

        // This has to happen so that we set the valid bit after
        // the value is stored, to prevent the data race of reading
        // and old invalid value
        if (r.Valid) {
            SetValidValue(r.Value);
        } else {
            Invalidate();
        }
    }

protected:
    ConvertedSensor(SensorType type) : StoredValueSensor(type) { }

    // Convert from the "raw" input value to a sensor reading (or invalid).
    // For example, this function might convert from a voltage to the pressure
    // represented by that voltage.
    virtual SensorResult ConvertFromInputValue(float inputValue) = 0;
};
