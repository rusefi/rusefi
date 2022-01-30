#include "pch.h"

#include "init.h"
#include "frequency_sensor.h"
#include "turbocharger_speed_converter.h"

// Filter parameter of 0.01 filters over roughly 100 teeth
static FrequencySensor turbochargerSpeedSensor(SensorType::TurbochargerSpeed, MS2NT(500), 0.01f);
static TurbochargerSpeedConverter turbochargerSpeedConverter;


void initTurbochargerSpeedSensor() {

	auto pin = engineConfiguration->turboSpeedSensorInputPin;

	// Nothing to do if no sensor configured
	if (!isBrainPinValid(pin)) {
		return;
	}

	turbochargerSpeedSensor.setFunction(turbochargerSpeedConverter);
	turbochargerSpeedSensor.init(pin);
	turbochargerSpeedSensor.Register();
}

void deinitTurbochargerSpeedSensor() {
	turbochargerSpeedSensor.deInit();
}