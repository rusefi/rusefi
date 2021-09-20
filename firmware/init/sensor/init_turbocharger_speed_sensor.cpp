#include "pch.h"

#include "init.h"
#include "frequency_sensor.h"
#include "turbocharger_speed_converter.h"

static FrequencySensor turbochargerSpeedSensor(SensorType::TurbochargerSpeed, MS2NT(500));
static TurbochargerSpeedConverter turbochargerSpeedConverter;


void initTurbochargerSpeedSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	INJECT_ENGINE_REFERENCE(&turbochargerSpeedConverter);

	auto pin = CONFIG(turboSpeedSensorInputPin);

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