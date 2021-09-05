#include "pch.h"
#include "sensor_converter_func.h"
#include "can_vss.h"

class VehicleSpeedConverter : public SensorConverter {
public:
	DECLARE_ENGINE_PTR;

	SensorResult convert(float frequency) const override {
		
		if (CONFIG(enableCanVss)) {
			auto speed_can = getVehicleCanSpeed();
			return speed_can;
		} else {
			auto speed = frequency * engineConfiguration->vehicleSpeedCoef;
			return speed;
		}
	
		return 0;
	}
};
