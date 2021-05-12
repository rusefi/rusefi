#pragma once

#include "stored_value_sensor.h"
#include "global.h"

struct MockSensor final : public StoredValueSensor
{
	MockSensor(SensorType type) : StoredValueSensor(type, MS2NT(50))
	{
	}

	void set(float value) {
		setValidValue(value, getTimeNowNt());
	}

	void invalidate() {
		StoredValueSensor::invalidate();
	}

	bool hasSensor() const override {
		return m_hasSensor;
	}

	void setHasSensor(bool h) {
		m_hasSensor = h;
	}

	void showInfo(const char* name) const override {}

private:
	bool m_hasSensor = true;
};
