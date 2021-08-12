#include "pch.h"

#include "init.h"
#include "vehicle_speed.h"
#include "function_pointer_sensor.h"

struct GetVehicleSpeedWrapper {
	DECLARE_ENGINE_PTR;

	float getVehicleSpeed() {
		return ::getVehicleSpeed(PASS_ENGINE_PARAMETER_SIGNATURE);
	}
};

static GetVehicleSpeedWrapper vehicleSpeedWrapper;

static FunctionPointerSensor vehicleSpeedSensor(SensorType::VehicleSpeed,
[]() {
	return vehicleSpeedWrapper.getVehicleSpeed();
});

void initVehicleSpeedSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	INJECT_ENGINE_REFERENCE(&vehicleSpeedWrapper);

	vehicleSpeedSensor.Register();
}