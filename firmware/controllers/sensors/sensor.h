#pragma once

#include "sensor_type.h"
#include <cstddef>

struct SensorResult {
    const bool Valid;
    const float Value;
};

// Fwd declare
struct SensorRegistryEntry;

class Sensor {
public:
    // Register this sensor in the sensor registry.
    // Returns true if registration succeeded, or false if
    // another sensor of the same type is already registered.
    [[nodiscard]] bool Register();

    // Remove all sensors from the sensor registry - tread carefully if you use this outside of a unit test
    static void ResetRegistry();

    /*
     * Static helper for sensor lookup
     */
    static const Sensor* GetSensorOfType(SensorType type);

    /*
     * Get a reading from the specified sensor.
     */
    static SensorResult Get(SensorType type);

    /*
     * Mock a value for a particular sensor.
     */
    static void SetMockValue(SensorType type, float value);

    /*
     * Reset mock for a particular sensor.
     */
    static void ResetMockValue(SensorType type);

    /*
     * Reset mocking for all sensors.
     */
    static void ResetAllMocks();

protected:
    // Protected constructor - only subclasses call this
    Sensor(SensorType type) : m_type(type) { }

private:
    // Retrieve the current reading from the sensor.
    //
    // Override this in a particular sensor's implementation.  As reading sensors is in many hot paths,
    // it is unwise to synchronously read the sensor or do anything otherwise costly here.  At the most,
    // this should be field lookup and simple math.
    virtual SensorResult Get() const = 0;

    SensorType m_type;

    // Get this sensor's index in the list
    constexpr size_t GetIndex() {
        return GetIndex(m_type);
    }

    // Get the index in the list for a sensor of particular type
    static constexpr size_t GetIndex(SensorType type) {
        return static_cast<size_t>(type);
    }

    /*
     * Static helper for sensor lookup
     */
    static SensorRegistryEntry* GetEntryForType(SensorType type);
};
