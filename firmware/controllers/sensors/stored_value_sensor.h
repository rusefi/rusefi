/**
 * @file    stored_value_sensor.h
 * @brief Base class for a sensor that has its value asynchronously
 * set, then later retrieved by a consumer.
 *
 * @date September 12, 2019
 * @author Matthew Kennedy, (c) 2019-2020
 */

#pragma once

#include "sensor.h"

#include "efitime.h"

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
	SensorResult get() const final override {
		bool valid = m_isValid;
		float value = m_value;

		if (!valid) {
			return unexpected;
		}

		// Timeouts are disabled, return last value
		if (Sensor::s_inhibitSensorTimeouts) {
			return value;
		}

		if (m_timeoutPeriod != 0) { // zero m_timeoutPeriod means value lasts forever
			if (getTimeNowNt() - m_timeoutPeriod > m_lastUpdate) {
				return unexpected;
			}
		}

		return value;
	}

	StoredValueSensor(SensorType type, efitick_t timeoutNt)
		: Sensor(type)
		, m_timeoutPeriod(timeoutNt)
	{
	}

	// Invalidate the stored value.
	void invalidate() {
		m_isValid = false;
	}

	// A new reading is available: set and validate a new value for the sensor.
	void setValidValue(float value, efitick_t timestamp) {
		// Set value before valid - so we don't briefly have the valid bit set on an invalid value
		m_value = value;
		m_isValid = true;
		m_lastUpdate = timestamp;
	}

	void showInfo(const char*) const override { }

private:
	bool m_isValid = false;
	float m_value = 0.0f;

	const efitick_t m_timeoutPeriod;
	efitick_t m_lastUpdate = 0;
};
