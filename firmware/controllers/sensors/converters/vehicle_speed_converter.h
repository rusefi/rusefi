#include "pch.h"
#include "sensor_converter_func.h"

class VehicleSpeedConverter : public SensorConverter {
public:
	DECLARE_ENGINE_PTR;

	SensorResult 
	convert(float frequency) const override {
		auto speed = frequency * engineConfiguration->vehicleSpeedCoef;
		return speed;
	}
};
