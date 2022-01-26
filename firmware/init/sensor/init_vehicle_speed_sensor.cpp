#include "pch.h"

#include "init.h"
#include "frequency_sensor.h"
#include "vehicle_speed_converter.h"

// 0.05 filter parameter means averaging over ~20 sensor teeth
static FrequencySensor vehicleSpeedSensor(SensorType::VehicleSpeed, MS2NT(500), 0.05f);
static VehicleSpeedConverter vehicleSpeedConverter;

void initVehicleSpeedSensor() {

	auto pin = engineConfiguration->vehicleSpeedSensorInputPin;

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