#pragma once

#include "stored_value_sensor.h"
#include "global.h"

struct MockSensor final : public StoredValueSensor
{
	MockSensor(SensorType type) : StoredValueSensor(type, MS2NT(50))
	{
	}

	void set(float value)
	{
		setValidValue(value, 0);
	}

	void invalidate()
	{
		StoredValueSensor::invalidate();
	}
};
