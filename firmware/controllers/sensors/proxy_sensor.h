/**
 * @file    proxy_sensor.h
 * @brief A sensor to duplicate a sensor to an additional SensorType.
 * 
 * This was built for the use case of "driver throttle intent" where we care what the driver's
 * right foot is doing, but that might mean TPS (cable throttle) or pedal (electronic throttle).
 *
 * @date March 22, 2020
 * @author Matthew Kennedy, (c) 2019
 */

#pragma once

#include "sensor.h"

class ProxySensor final : public Sensor {
public:
	explicit ProxySensor(SensorType type) : Sensor(type) {}

	void setProxiedSensor(SensorType proxiedSensor) {
		m_proxiedSensor = proxiedSensor;
	}

	void showInfo(const char* sensorName) const override;

private:
	SensorResult get() const override {
		return Sensor::get(m_proxiedSensor);
	}

	bool hasSensor() const override {
		// query if the underlying sensor exists
		return Sensor::hasSensor(m_proxiedSensor);
	}

	SensorType m_proxiedSensor = SensorType::Invalid;
};
