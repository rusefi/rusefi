#include "pch.h"

#include "init.h"
#include "frequency_sensor.h"
#include "output_shaft_speed_converter.h"

FrequencySensor outputShaftSpeedSensor(SensorType::OutputShaftSpeed, MS2NT(500));
static OutputShaftSpeedConverter outputSpeedConverter;

void initOutputShaftSpeedSensor() {
	int reciprocal = engineConfiguration->ossFilterReciprocal;
	if (reciprocal < 3 || reciprocal > 200) {
		reciprocal = 3;
	}

	// Unassigned pins leave the registry slot available for a Lua/CAN provider.
	outputShaftSpeedSensor.invalidate();
	outputShaftSpeedSensor.initIfValid(engineConfiguration->outputShaftSpeedSensorPin,
		outputSpeedConverter, 1.0f / reciprocal);
}

void deinitOutputShaftSpeedSensor() {
	outputShaftSpeedSensor.deInit();
	outputShaftSpeedSensor.unregister();
	outputShaftSpeedSensor.invalidate();
}
