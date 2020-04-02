#pragma once

#include "sensor.h"

class RedundantSensor final : public Sensor
{
public:
	RedundantSensor(
		SensorType outputType,
		SensorType firstSensor,
		SensorType secondSensor,
		float maximumDifference,
		bool ignoreSecondSensor);

	SensorResult get() const override;

private:
	// The two sensors we interpret to form one redundant sensor
	const SensorType m_first;
	const SensorType m_second;

	// How far apart do we allow the sensors to be before reporting an issue?
	const float m_maxDifference;

	// Should we ignore the second sensor? (disable redundancy)
    const bool m_ignoreSecond;
};
