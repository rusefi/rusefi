#include "pch.h"
#include "sensor_converter_func.h"

class InputShaftSpeedConverter : public SensorConverter {
public:
	SensorResult convert(float frequency) const override {
		return frequency * 60 / engineConfiguration->tcuInputSpeedSensorTeeth;
	}
};
