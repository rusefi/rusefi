#include "pch.h"

#include "redundant_sensor.h"

RedundantSensor::RedundantSensor(SensorType outputType, SensorType first, SensorType second)
	: Sensor(outputType)
	, m_first(first)
	, m_second(second)
{
}

void RedundantSensor::configure(float maxDifference, bool ignoreSecondSensor, float secondMaximum) {
	m_maxDifference = maxDifference;
	m_ignoreSecond = ignoreSecondSensor;
	m_secondMaximum = secondMaximum;
}

SensorResult RedundantSensor::get() const {
	// Sensor 1 is always full range, i.e. 0% -> 100%
	auto sensor1 = Sensor::get(m_first);

	// If we're set to disable redundancy, just pass thru the first sensor
	if (m_ignoreSecond) {
		return sensor1;
	}

	// If sensor 1 result is invalid, return inconsistency error
	if (!sensor1.Valid) {
		return UnexpectedCode::Inconsistent;
	}

	// Sensor 2 may be partial, i.e. 0% -> m_secondMaximum%
	auto sensor2 = Sensor::get(m_second);

	// If sensor 2 result is invalid, return inconsistency error
	if (!sensor2.Valid) {
		return UnexpectedCode::Inconsistent;
	}

	if (m_secondMaximum >= 100) {
		// Sensor is fully redundant

		float delta = absF(sensor1.Value - sensor2.Value);
		if (delta <= m_maxDifference) {
			// All is well: sensors are valid and values check out, return the average value
			return (sensor1.Value + sensor2.Value) / 2;
		}
	} else {
		// Sensor is partially redundant; useful for some sensors: e.g. Ford and Toyota ETCS-i

		// The threshold at which to switch to partial redundancy, just below maximum to avoid misbehavior near 100%
		float threshold = m_secondMaximum * 0.95f;

		// The scaled second sensor, proportioning it to the first sensor
		float scaledSecond = sensor2.Value * m_secondMaximum / 100;

		// Check second sensor is below partial redundancy switch-over threshold
		if (scaledSecond <= threshold) {
			float delta = absF(sensor1.Value - scaledSecond);
			if (delta <= m_maxDifference) {
				// All is well: sensors are valid and values check out, return the average value
				return (sensor1.Value + scaledSecond) / 2;
			}
		} else {
			// Check first sensor is at or above partial redundancy switch-over threshold
			if (sensor1.Value >= m_secondMaximum - m_maxDifference) {
				return sensor1.Value;
			}
		}
	}

	// Fall-through and any other condition indicates an unexpected discrepancy, return inconsistency error
	return UnexpectedCode::Inconsistent;
}
