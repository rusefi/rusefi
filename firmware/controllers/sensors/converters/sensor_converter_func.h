#pragma once

#include "sensor.h"

class Logging;

struct SensorConverter {
	virtual SensorResult convert(float raw) const = 0;
	virtual void showInfo(Logging* logger, float testRawValue) const {}
};
