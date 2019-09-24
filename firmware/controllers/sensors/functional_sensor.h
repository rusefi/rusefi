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

struct FunctionalSensorBase : public StoredValueSensor {
	FunctionalSensorBase(SensorType type)
		: StoredValueSensor(type) {}

	virtual void postRawValue(float value) = 0;
};

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
template <typename TFunc>
class FunctionalSensor final : public FunctionalSensorBase {
	static_assert(std::is_base_of_v<SensorConverter, TFunc>, "TFunc must inherit from SensorConverter");
	static_assert(std::is_default_constructible_v<TFunc>, "TFunc must be default constructible");

public:
	explicit FunctionalSensor(SensorType type)
		: FunctionalSensorBase(type) {}

	void postRawValue(float inputValue) {
		// Report the raw value
		float *rawReportLocation = m_rawReportingLocation;
		if (rawReportLocation) {
			*rawReportLocation = inputValue;
		}

		auto r = m_function.convert(inputValue);

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

	// Allow access to the underlying function
	TFunc &f() {
		return m_function;
	}

private:
	float *m_rawReportingLocation = nullptr;

	// Conversion function for this sensor
	TFunc m_function;
};
