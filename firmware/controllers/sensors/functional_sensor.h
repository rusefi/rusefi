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
class FunctionalSensor final : public StoredValueSensor {
public:
	explicit FunctionalSensor(SensorType type)
		: StoredValueSensor(type) { }

	void postRawValue(float inputValue) {
		// Report the raw value
		float *rawReportLocation = m_rawReportingLocation;
		if (rawReportLocation) {
			*rawReportLocation = inputValue;
		}

		// If no function is set, this sensor isn't valid.
		if (!m_function) {
			invalidate();
			return;
		}

		auto r = m_function->convert(inputValue);

		// This has to happen so that we set the valid bit after
		// the value is stored, to prevent the data race of reading
		// an old invalid value
		if (r.Valid) {
			setValidValue(r.Value);
		} else {
			invalidate();
		}
	}

	void setRawReportingLocation(float *rawReportingLocation) {
		m_rawReportingLocation = rawReportingLocation;
	}

	void setFunction(SensorConverter& func) {
		m_function = &func;
	}

private:
	float *m_rawReportingLocation = nullptr;

	// Conversion function for this sensor
	SensorConverter* m_function = nullptr;
};
