/**
 *	Frequency to RPM converter for turbocharger pin
 */

#include "pch.h"
#include "sensor_converter_func.h"

class TurbochargerSpeedConverter : public SensorConverter  {
public:
	SensorResult convert(float frequency) const override {
		auto hz = frequency * engineConfiguration->turboSpeedSensorMultiplier;

		auto rpm = hz * 60;

		return rpm;
	}
};
