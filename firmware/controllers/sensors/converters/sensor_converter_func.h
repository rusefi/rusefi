#pragma once

#include "sensor.h"

class Logging;

struct SensorConverter {
	// Trying to copy a converter func by value is almost guaranteed to be a bug - disallow it
	SensorConverter(const SensorConverter&) = delete;
	// ...but doing so requires explicitly declaring the default constructor, so do that too.
	SensorConverter() = default;

	virtual SensorResult convert(float raw) const = 0;
	virtual void showInfo(Logging* logger, float testRawValue) const {
		// Unused base - nothing to print
		(void)logger;
		(void)testRawValue;
	}
};
