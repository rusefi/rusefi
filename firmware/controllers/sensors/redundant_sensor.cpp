#include "pch.h"

#include "redundant_sensor.h"

RedundantSensor::RedundantSensor(SensorType outputType, SensorType first, SensorType second)
	: Sensor(outputType)
	, m_first(first)
	, m_second(second)
{
}

void RedundantSensor::configure(float maxDifference, bool ignoreSecondSensor, bool averageSensors, float secondMaximum) {
	m_maxDifference = maxDifference;
	m_ignoreSecond = ignoreSecondSensor;
	m_averageSensors = averageSensors;
	m_secondMaximum = secondMaximum;
}

SensorResult RedundantSensor::get() const {
	// Sensor 1 is always full range, i.e. 0% -> 100%
	auto sensor1 = Sensor::get(m_first);

	// If sensor 1 result is invalid, return inconsistency error
	if (!sensor1.Valid) {
		return UnexpectedCode::Inconsistent;
	}

	// If we're set to disable redundancy, just pass thru the first sensor
	if (m_ignoreSecond) {
		return sensor1;
	}

	// Sensor 2 may be partial, i.e. 0% -> m_secondMaximum%
	auto sensor2 = Sensor::get(m_second);

	// If sensor 2 result is invalid, return inconsistency error
	if (!sensor2.Valid) {
		return UnexpectedCode::Inconsistent;
	}

	float sensor2Value = sensor2.Value;

	// Partial redundancy; useful for some sensors: e.g. Ford and Toyota ETCS-i
	if (m_secondMaximum != 100) {
		// scale the second sensor value accordingly, proportioning to the first sensor
		sensor2Value *= m_secondMaximum / 100;

		// The partial redundancy threshold, slightly less than 100% to avoid issues near full-range
		float threshold = m_secondMaximum * 0.95;

		// Check second sensor is below partial redundancy switch-over threshold
		if (sensor2Value > threshold) {
			// Check first sensor is at or above partial redundancy switch-over threshold
			if (sensor1.Value > threshold - m_maxDifference) {
				return sensor1.Value;
			}

			// There's a discrepancy: first sensor is out of compliance, return inconsistency error
			return UnexpectedCode::Inconsistent;
		}
	}

	float delta = absF(sensor1.Value - sensor2Value);
	if (delta <= m_maxDifference) {
		// All is well: sensors are valid and values check out
		return m_averageSensors ? (sensor1.Value + sensor2Value) / 2 : sensor1.Value;
	}

	// Any other condition indicates an unexpected discrepancy, return inconsistency error
	return UnexpectedCode::Inconsistent;
}
