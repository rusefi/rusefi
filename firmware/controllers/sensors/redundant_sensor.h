#pragma once

#include "sensor.h"

class RedundantSensor final : public Sensor
{
public:
	RedundantSensor(
		SensorType outputType,
		SensorType firstSensor,
		SensorType secondSensor
	);

	void configure(float maxDifference, bool ignoreSecondSensor);

	SensorResult get() const override;

	bool isRedundant() const override {
		// This sensor is redundant when not ignoring the second channel
		return !m_ignoreSecond;
	}

	void showInfo(const char* sensorName) const override;

private:
	// The two sensors we interpret to form one redundant sensor
	const SensorType m_first;
	const SensorType m_second;

	// How far apart do we allow the sensors to be before reporting an issue?
	float m_maxDifference = 0;

	// Should we ignore the second sensor? (disable redundancy)
	bool m_ignoreSecond = false;
};
