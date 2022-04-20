#include "pch.h"

#include "init.h"
#include "frequency_sensor.h"
#include "input_shaft_speed_converter.h"

static FrequencySensor inputShaftSpeedSensor(SensorType::InputShaftSpeed, MS2NT(500));
static InputShaftSpeedConverter inputSpeedConverter;

void initInputShaftSpeedSensor() {
	int parameter = engineConfiguration->issFilterReciprocal;

	if (parameter <= 0 || parameter > 200) {
		parameter = 2;
	}

	float filterParameter = 1.0f / parameter;
	inputShaftSpeedSensor.initIfValid(engineConfiguration->tcuInputSpeedSensorPin, inputSpeedConverter, filterParameter);
}

void deinitInputShaftSpeedSensor() {
	inputShaftSpeedSensor.deInit();
}
