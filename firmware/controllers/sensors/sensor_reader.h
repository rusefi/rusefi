/**
 * @file    sensor_reader.h
 * @brief Declaration for SensorReader, the class used to acquire readings from a sensor.
 *
 * @date September 12, 2019
 * @author Matthew Kennedy, (c) 2019
 */

#pragma once

#include "sensor.h"

/**
 * @brief Provides an interface for reading sensors.
 * 
 * Example usage:
 * 
 * // Create a sensor reader for type MySensor, and fallback value 10.0
 * SensorReader<SensorType::MySensor> mySensorReader(10.0f);
 * 
 * <later>
 * 
 * // Returns the sensor value if valid, or 10.0 if invalid.
 * float currentSensorValue = mySensorReader.GetOrDefault();
 * 
 * Simply calling the Get() method returns the full SensorResult, should
 * more complex logic be required in case of an invalid sensor reading.
 */
template <SensorType TSensorType>
class SensorReader final {
public:
	using sensorValueType = float;

	SensorReader(sensorValueType defaultValue) : m_defaultValue(defaultValue) { }

	SensorResult Get() const {
		return Sensor::Get(TSensorType);
	}

	// Get the sensor's reading, or a default value.
	// Inteded for applications where a default may be used silently,
	// while elsewhere in the world the failed sensor is otherwise handled.
	sensorValueType GetOrDefault() const {
		auto result = Get();

		if (result.Valid) {
			return (sensorValueType)result.Value;
		} else {
			return m_defaultValue;
		}
	}
private:
	const sensorValueType m_defaultValue;
};
