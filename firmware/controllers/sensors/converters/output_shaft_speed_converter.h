#pragma once

#include "sensor_converter_func.h"

class OutputShaftSpeedConverter : public SensorConverter {
public:
	SensorResult convert(float frequency) const override {
		auto teeth = engineConfiguration->outputShaftSpeedSensorTeeth;
		if (teeth == 0 || !std::isfinite(frequency) || frequency < 0) {
			return UnexpectedCode::Configuration;
		}
		return frequency * 60 / teeth;
	}
};
