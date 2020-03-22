#pragma once

#include "sensor.h"

class ProxySensor final : public Sensor {
public:
	explicit ProxySensor(SensorType type) : Sensor(type) {}

	void setProxiedSensor(SensorType proxiedSensor) {
		m_proxiedSensor = proxiedSensor;
	}

private:
	SensorResult get() const {
		return Sensor::get(m_proxiedSensor);
	}

	SensorType m_proxiedSensor = SensorType::Invalid;
};