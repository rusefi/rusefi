#include "pch.h"

#include "init.h"
#include "vehicle_speed.h"
#include "frequency_sensor.h"
#include "vehicle_speed_converter.h"

static FrequencySensor vehicleSpeedSensor(
	SensorType::VehicleSpeed, 
	MS2NT(500),
	CONFIG(vehicleSpeedSensorInputPin));
static VehicleSpeedConverter vehicleSpeedConverter;


void initVehicleSpeedSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	INJECT_ENGINE_REFERENCE(&vehicleSpeedConverter);

	// Nothing to do if no sensor configured
	if (!isBrainPinValid(vehicleSpeedSensor.getPin())) {
		return;
	}

	vehicleSpeedSensor.setFunction(vehicleSpeedConverter);
	vehicleSpeedSensor.init("vss");
	vehicleSpeedSensor.Register();
}

void deInitVehicleSpeedSensor() {
	vehicleSpeedSensor.deInit();
}