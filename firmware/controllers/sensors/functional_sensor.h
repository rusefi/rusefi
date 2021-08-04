/**
 * @file    converter_sensor.h
 *
 * @date September 12, 2019
 * @author Matthew Kennedy, (c) 2019
 */

#pragma once

#include "converters/sensor_converter_func.h"
#include "stored_value_sensor.h"

#include <type_traits>

/**
 * @brief Class for sensors that convert from some raw floating point
 * value (ex: voltage, frequency, pulse width) to a sensor reading.
 *
 * To use this class, implement the conversion operation for your sensor
 * as a class that inherits from SensorConverter, and implement convert
 * to convert a raw reading from the sensor to a usable value (and valid bit).
 *
 * Register an instance of the new class with an interface
 * that provides and posts raw values so the sensor can update.
 */
class FunctionalSensor : public StoredValueSensor {
public:
	FunctionalSensor(SensorType type, efitick_t timeoutPeriod)
		: StoredValueSensor(type, timeoutPeriod) { }

	void postRawValue(float inputValue, efitick_t timestamp);

	void setFunction(SensorConverter& func) {
		m_function = &func;
	}

	float getRaw() const override final {
		return m_rawValue;
	}

	void showInfo(const char* sensorName) const override;

private:
	// Conversion function for this sensor
	SensorConverter* m_function = nullptr;

	float m_rawValue = 0;
};
