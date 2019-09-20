#pragma once

#include "stored_value_sensor.h"

struct MockSensor final : public StoredValueSensor
{
	MockSensor(SensorType type) : StoredValueSensor(type)
	{
	}

	void set(float value)
	{
		setValidValue(value);
	}

	void invalidate()
	{
		StoredValueSensor::invalidate();
	}
};
