#pragma once

#include "sensor_converter_func.h"

struct IdentityFunction : public SensorConverter {
	SensorResult convert(float raw) const {
		return raw;
	}

	void showInfo(float testRawValue) const;
};

extern IdentityFunction identityFunction;
