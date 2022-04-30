/*
 * @file init_aux_speed_sensor.cpp
 *
 */

#include "pch.h"
#include "init.h"
#include "frequency_sensor.h"

static FrequencySensor auxSpeed1(SensorType::AuxSpeed1, MS2NT(500));
static FrequencySensor auxSpeed2(SensorType::AuxSpeed2, MS2NT(500));

static class : public SensorConverter  {
public:
	SensorResult convert(float frequency) const override {
		auto hz = frequency;

		auto rpm = hz * 60;

		return rpm;
	}
} converter;


void initAuxSpeedSensors() {
	auxSpeed1.initIfValid(engineConfiguration->auxSpeedSensorInputPin[0], converter, 0.05f);
	auxSpeed2.initIfValid(engineConfiguration->auxSpeedSensorInputPin[1], converter, 0.05f);
}

void deinitAuxSpeedSensors() {
	auxSpeed1.deInit();
	auxSpeed2.deInit();
}
