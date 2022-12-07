#include "pch.h"

#include "init.h"
#include "frequency_sensor.h"
#include "flex_sensor.h"

static FrequencySensor flexSensor(SensorType::FuelEthanolPercent, MS2NT(500));
static FlexConverter converter;

// https://rusefi.com/forum/viewtopic.php?p=37452&sid=829804c90d5b2e1fecd1b900cf1b1811#p37452

void initFlexSensor() {
	// 0.01 means filter bandwidth of ~1hz with ~100hz sensor
	flexSensor.initIfValid(engineConfiguration->flexSensorPin, converter, 0.01f);
}

void deInitFlexSensor() {
	flexSensor.deInit();
}
