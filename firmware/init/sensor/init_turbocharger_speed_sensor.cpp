#include "pch.h"

#include "init.h"
#include "frequency_sensor.h"
#include "turbocharger_speed_converter.h"

static FrequencySensor turbochargerSpeedSensor(SensorType::TurbochargerSpeed, MS2NT(500));
static TurbochargerSpeedConverter turbochargerSpeedConverter;


void initTurbochargerSpeedSensor() {
	// Filter parameter of 0.01 filters over roughly 100 teeth
	turbochargerSpeedSensor.initIfValid(engineConfiguration->turboSpeedSensorInputPin, turbochargerSpeedConverter, 0.01f);
}

void deinitTurbochargerSpeedSensor() {
	turbochargerSpeedSensor.deInit();
}
