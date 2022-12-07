#include "pch.h"

/**
 * We were NOT able to get this code working reliable
 * it could be that biquad parameter or biquad overall is part of the problem
 */

#include "init.h"
#include "frequency_sensor.h"
#include "input_shaft_speed_converter.h"

FrequencySensor inputShaftSpeedSensor(SensorType::InputShaftSpeed, MS2NT(500));
static InputShaftSpeedConverter inputSpeedConverter;

void initInputShaftSpeedSensor() {
	int parameter = engineConfiguration->issFilterReciprocal;

	if (parameter < 3 || parameter > 200) {
		parameter = 3;
	}

	float filterParameter = 1.0f / parameter;
	inputShaftSpeedSensor.initIfValid(engineConfiguration->tcuInputSpeedSensorPin, inputSpeedConverter, filterParameter);
}

void deinitInputShaftSpeedSensor() {
	inputShaftSpeedSensor.deInit();
}
