#pragma once

#include "sensor.h"

struct SensorConverter {
	virtual SensorResult convert(float raw) const = 0;
};
