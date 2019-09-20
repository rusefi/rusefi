/**
 * @file    converter_sensor.h
 *
 * @date September 12, 2019
 * @author Matthew Kennedy, (c) 2019
 */

#pragma once

#include "stored_value_sensor.h"

/**
 * @brief Base class for sensors that convert from some raw floating point
 * value (ex: voltage, frequency, pulse width) to a sensor reading.
 * 
 * To use this base class, inherit it and implement ConvertFromInputValue(float input).
 * Perform any conversion work necessary to convert from the raw value to a sensor
 * reading, and return it.  Register an instance of the new class with an interface
 * that provides and posts raw values so the sensor can update.
 */
class ConvertedSensor : public StoredValueSensor {
public:
	void postRawValue(float inputValue) {
		// Report the raw value
		float* rawReportLocation = m_rawReportingLocation;
		if (rawReportLocation) {
			*rawReportLocation = inputValue;
		}
		
		auto r = convertFromInputValue(inputValue);

		// This has to happen so that we set the valid bit after
		// the value is stored, to prevent the data race of reading
		// an old invalid value
		if (r.Valid) {
			setValidValue(r.Value);
		} else {
			invalidate();
		}
	}

	void setRawReportingLocation(float* rawReportingLocation) {
		m_rawReportingLocation = rawReportingLocation;
	}

protected:
	ConvertedSensor(SensorType type) : StoredValueSensor(type) { }

	/**
	 * @brief Convert from the "raw" input value to a sensor reading (or invalid).
	 * 
	 * For example, this function might convert from a voltage to the pressure
	 * represented by that voltage.
	 */
	virtual SensorResult convertFromInputValue(float inputValue) = 0;

private:
	float* m_rawReportingLocation = nullptr;
};
