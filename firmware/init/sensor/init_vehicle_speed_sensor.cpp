#include "pch.h"

#include "init.h"
#include "frequency_sensor.h"
#include "vehicle_speed_converter.h"

static FrequencySensor vehicleSpeedSensor(SensorType::VehicleSpeed, MS2NT(500));
static VehicleSpeedConverter vehicleSpeedConverter;

void initVehicleSpeedSensor() {
	vehicleSpeedConverter.inject();

	auto pin = CONFIG(vehicleSpeedSensorInputPin);

	// Nothing to do if no sensor configured
	if (!isBrainPinValid(pin)) {
		return;
	}

	vehicleSpeedSensor.setFunction(vehicleSpeedConverter);
	vehicleSpeedSensor.init(pin);
	vehicleSpeedSensor.Register();
}

void deInitVehicleSpeedSensor() {
	vehicleSpeedSensor.deInit();
}