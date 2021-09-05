#include "pch.h"
#include "sensor_converter_func.h"
#include "can_vss.h"

class VehicleSpeedConverter : public SensorConverter {
public:
	DECLARE_ENGINE_PTR;

	SensorResult convert(float frequency) const override {
		auto speed;
		if (CONFIG(enableCanVss)) {
			speed = getVehicleCanSpeed();
		} else {
			speed = frequency * engineConfiguration->vehicleSpeedCoef;
		}
	
		return speed;
	}
};
