/**
 * @file    function_pointer_sensor.h
 * @brief A sensor to provide a bridge from old getX()-style functions to the new sensor registry.
 *
 * @date September 12, 2019
 * @author Matthew Kennedy, (c) 2019
 */

#pragma once

#include "sensor.h"

/* This class is intended as a bridge to bridge from old getMySensor() functions
 * to the new system.  This way, producers and consumers can be independently
 * updated to the new system, with sensors being usable either way for some time.
 */
class FunctionPointerSensor final : public Sensor {
public:
	FunctionPointerSensor(SensorType type, float (*func)())
		: Sensor(type)
		, m_func(func) {}

	SensorResult get() const final {
		float result = m_func();

		// check for NaN
		bool valid = !(result != result);

		if (!valid) {
			return unexpected;
		}

		return result;
	}

	void showInfo(const char* /*sensorName*/) const override {}

private:
	float (*m_func)();
};
