/**
 * @file    stored_value_sensor.h
 * @brief Base class for a sensor that has its value asynchronously
 * set, then later retrieved by a consumer.
 *
 * @date September 12, 2019
 * @author Matthew Kennedy, (c) 2019
 */

#pragma once

#include "sensor.h"

/**
 * @brief Base class for sensors that compute a value on one thread, and want
 * to make it available to consumers asynchronously.
 * 
 * Common examples include sensors that have to do heavy lifting to produce
 * a reading, and don't want to perform that conversion at the time of
 * consumption.
 * 
 * To use this class, create a class for your sensor that inherits StoredValueSensor,
 * and call Invalidate() and SetValidValue(float) as appropriate when readings are available
 * (or known to be invalid) for your sensor.
 * 
 * Consumers will retrieve the last set (or invalidated) value.
 */
class StoredValueSensor : public Sensor {
public:
	SensorResult get() const final {
		bool valid = m_isValid;
		float value = m_value;

		return { valid, value };
	}

protected:
	StoredValueSensor(SensorType type) : Sensor(type) {}

	// Invalidate the stored value.
	void invalidate() {
		m_isValid = false;
	}

	// A new reading is available: set and validate a new value for the sensor.
	void setValidValue(float value) {
		// Set value before valid - so we don't briefly have the valid bit set on an invalid value
		m_value = value;
		m_isValid = true;
	}

private:
	bool m_isValid = false;
	float m_value = 0.0f;
};
