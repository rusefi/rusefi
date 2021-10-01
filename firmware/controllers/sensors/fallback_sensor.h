#pragma once

#include "sensor.h"

class FallbackSensor final : public Sensor {
public:
	FallbackSensor(SensorType outputType, SensorType primarySensor, SensorType fallbackSensor)
		: Sensor(outputType)
		, m_primary(primarySensor)
		, m_fallback(fallbackSensor)
	{
	}

	SensorResult get() const override {
		auto primary = Sensor::get(m_primary);

		if (primary) {
			return primary;
		}

		return Sensor::get(m_fallback);
	}

	void showInfo(const char* sensorName) const override {};

private:
	const SensorType m_primary;
	const SensorType m_fallback;
};
