#pragma once

#include "stored_value_sensor.h"

struct MockSensor final : public StoredValueSensor
{
	MockSensor(SensorType type) : StoredValueSensor(type)
	{
	}

	void Set(float value)
	{
		SetValidValue(value);
	}

	void Invalidate()
	{
		StoredValueSensor::Invalidate();
	}
};
