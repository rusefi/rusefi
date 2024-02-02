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
		return frequency;
	}
} converter;

class WheelSlipBasedOnAuxSpeedSensor : public Sensor {
public:
  WheelSlipBasedOnAuxSpeedSensor() : Sensor(SensorType::WheelSlipRatio)  {  }

	SensorResult get() const final override {
	  auto value1 = auxSpeed1.get();
	  auto value2 = engineConfiguration->useVssAsSecondWheelSpeed ? Sensor::get(SensorType::VehicleSpeed) : auxSpeed2.get();
	  if (!value1.Valid || !value2.Valid) {
	    return UnexpectedCode::Unknown;
	  }
	  // todo: remove handling of zero in July of 2024
	  float correctedAuxSpeed1Multiplier = engineConfiguration->auxSpeed1Multiplier == 0 ? 1 : engineConfiguration->auxSpeed1Multiplier;
	  float result = value1.Value * correctedAuxSpeed1Multiplier / value2.Value;
	  return result;
	}

	void showInfo(const char*) const override { }
};

static WheelSlipBasedOnAuxSpeedSensor wheelSlipSensor;

void initAuxSpeedSensors() {
	auxSpeed1.useBiQuad = engineConfiguration->useBiQuadOnAuxSpeedSensors;
	auxSpeed1.initIfValid(engineConfiguration->auxSpeedSensorInputPin[0], converter, engineConfiguration->auxFrequencyFilter);
	auxSpeed2.initIfValid(engineConfiguration->auxSpeedSensorInputPin[1], converter, 0.05f);

  if (engineConfiguration->useAuxSpeedForSlipRatio) {
	  wheelSlipSensor.Register();
	}
}

void deinitAuxSpeedSensors() {
	auxSpeed1.deInit();
	auxSpeed2.deInit();
	wheelSlipSensor.unregister();
}
