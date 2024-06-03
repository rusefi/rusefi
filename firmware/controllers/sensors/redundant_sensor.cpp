#include "pch.h"

#include "redundant_sensor.h"

RedundantSensor::RedundantSensor(SensorType outputType, SensorType first, SensorType second)
	: Sensor(outputType)
	, m_first(first)
	, m_second(second)
{
}

void RedundantSensor::configure(float /*split threshold*/maxDifference, bool ignoreSecondSensor) {
	m_maxDifference = maxDifference;
	m_ignoreSecond = ignoreSecondSensor;
}

SensorResult RedundantSensor::get() const {
	auto sensor1 = Sensor::get(m_first);

	// If we're set to disable redundancy, just pass thru the first sensor
	if (m_ignoreSecond) {
		return sensor1;
	}

	auto sensor2 = Sensor::get(m_second);

	// If either result is invalid, return invalid.
	if (!sensor1.Valid || !sensor2.Valid) {
		return UnexpectedCode::Inconsistent;
	}

	// If both are valid, check that they're near one another
	float delta = absF(sensor1.Value - sensor2.Value);
	if (delta > m_maxDifference) {
		return UnexpectedCode::Inconsistent;
	}

	// Both sensors are valid, and their readings are close. All is well.
	// Return the average
	return (sensor1.Value + sensor2.Value) / 2;
}
