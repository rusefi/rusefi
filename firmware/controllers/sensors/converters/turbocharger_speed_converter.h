/**
 *	Frequency to RPM converter for turbocharger pin
 */

#include "pch.h"
#include "sensor_converter_func.h"

class TurbochargerSpeedConverter : public SensorConverter {
public:
	DECLARE_ENGINE_PTR;

	SensorResult convert(float frequency) const override {
		auto speed = frequency * engineConfiguration->turboSpeedSensorMultiplier;
		return speed;
	}
};

