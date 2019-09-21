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
 * float currentSensorValue = mySensorReader.getOrDefault();
 * alternatively, because we have implicit conversion:
 * float currentSensorValue = mySensorReader;
 *
 * Simply calling the get() method returns the full SensorResult, should
 * more complex logic be required in case of an invalid sensor reading.
 */
template <SensorType TSensorType>
class SensorReader final {
public:
	using sensorValueType = float;

	explicit SensorReader(sensorValueType defaultValue)
		: m_defaultValue(defaultValue) {}

	SensorResult get() const {
		return Sensor::get(TSensorType);
	}

	// Get the sensor's reading, or a default value.
	// Inteded for applications where a default may be used silently,
	// while elsewhere in the world the failed sensor is otherwise handled.
	sensorValueType getOrDefault() const {
		auto result = get();

		if (result.Valid) {
			return (sensorValueType)result.Value;
		} else {
			return m_defaultValue;
		}
	}

	// Implicit conversion operator
	operator sensorValueType() const {
		return getOrDefault();
	}

private:
	const sensorValueType m_defaultValue;
};
