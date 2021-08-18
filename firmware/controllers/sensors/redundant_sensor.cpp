#include "pch.h"

#include "redundant_sensor.h"

RedundantSensor::RedundantSensor(SensorType outputType, SensorType first, SensorType second)
	: Sensor(outputType)
	, m_first(first)
	, m_second(second)
{
}

void RedundantSensor::configure(float maxDifference, bool ignoreSecondSensor) {
	m_maxDifference = maxDifference;
	m_ignoreSecond = ignoreSecondSensor;
}

SensorResult RedundantSensor::get() const {
	auto result1 = Sensor::get(m_first);

	// If we're set to disable redundancy, just pass thru the first sensor
	if (m_ignoreSecond) {
		return result1;
	}

	auto result2 = Sensor::get(m_second);

	// If either result is invalid, return invalid.
	if (!result1.Valid || !result2.Valid) {
		return unexpected;
	}

	// If both are valid, check that they're near one another
	float delta = absF(result1.Value - result2.Value);
	if (delta > m_maxDifference) {
		return unexpected;
	}

	// Both sensors are valid, and their readings are close. All is well.
	// Return the average
	return (result1.Value + result2.Value) / 2;
}
