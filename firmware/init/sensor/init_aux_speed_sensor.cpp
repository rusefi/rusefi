/*
 * @file init_aux_speed_sensor.cpp
 *
 */

#include "pch.h"
#include "init.h"
#include "frequency_sensor.h"

static FrequencySensor auxSpeed1(SensorType::AuxSpeed1, MS2NT(500));
static FrequencySensor auxSpeed2(SensorType::AuxSpeed2, MS2NT(500));

static FrequencySensor WheelSpeedFL(SensorType::WheelSpeedFL, MS2NT(500)); // fr
static FrequencySensor WheelSpeedFR(SensorType::WheelSpeedFR, MS2NT(500)); // fl
static FrequencySensor WheelSpeedRL(SensorType::WheelSpeedRL, MS2NT(500)); // rr
static FrequencySensor WheelSpeedRR(SensorType::WheelSpeedRR, MS2NT(500)); // rl


static class : public SensorConverter  {
	public:
	SensorResult convert(float frequency) const override {
		auto vssRevPerKm = engineConfiguration->driveWheelRevPerKm1 * engineConfiguration->vssGearRatio1;

		auto pulsePerKm = (vssRevPerKm * engineConfiguration->vssToothCount1);

		if (pulsePerKm == 0) {
			// avoid div by 0
			return 0;
		}

		auto kmPerPulse = 1 / pulsePerKm;

		//     1 pulse       3600 sec      1 km       km
		//    ---------  *  ---------- * --------- = ----
		//       sec           1 hr       1 pulse     hr
		return frequency *     3600    * kmPerPulse;
	}

} converter1;


static class : public SensorConverter  {
	public:
	SensorResult convert(float frequency) const override {
		auto vssRevPerKm = engineConfiguration->driveWheelRevPerKm2 * engineConfiguration->vssGearRatio2;

		auto pulsePerKm = (vssRevPerKm * engineConfiguration->vssToothCount2);

		if (pulsePerKm == 0) {
			// avoid div by 0
			return 0;
		}

		auto kmPerPulse = 1 / pulsePerKm;

		//     1 pulse       3600 sec      1 km       km
		//    ---------  *  ---------- * --------- = ----
		//       sec           1 hr       1 pulse     hr
		return frequency *     3600    * kmPerPulse;
	}

} converter2;


static class : public SensorConverter  {
	public:
	SensorResult convert(float frequency) const override {
		auto vssRevPerKm = engineConfiguration->driveWheelRevPerKm3 * engineConfiguration->vssGearRatio3;

		auto pulsePerKm = (vssRevPerKm * engineConfiguration->vssToothCount3);

		if (pulsePerKm == 0) {
			// avoid div by 0
			return 0;
		}

		auto kmPerPulse = 1 / pulsePerKm;

		//     1 pulse       3600 sec      1 km       km
		//    ---------  *  ---------- * --------- = ----
		//       sec           1 hr       1 pulse     hr
		return frequency *     3600    * kmPerPulse;
	}

} converter3;



static class : public SensorConverter  {
	public:
	SensorResult convert(float frequency) const override {
		auto vssRevPerKm = engineConfiguration->driveWheelRevPerKm4 * engineConfiguration->vssGearRatio4;

		auto pulsePerKm = (vssRevPerKm * engineConfiguration->vssToothCount4);

		if (pulsePerKm == 0) {
			// avoid div by 0
			return 0;
		}

		auto kmPerPulse = 1 / pulsePerKm;

		//     1 pulse       3600 sec      1 km       km
		//    ---------  *  ---------- * --------- = ----
		//       sec           1 hr       1 pulse     hr
		return frequency *     3600    * kmPerPulse;
	}

} converter4;






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


	int parameter1 = engineConfiguration->vssFilterReciprocal1;
	int parameter2 = engineConfiguration->vssFilterReciprocal2;
	int parameter3 = engineConfiguration->vssFilterReciprocal3;
	int parameter4 = engineConfiguration->vssFilterReciprocal4;

	if (parameter1 < VSS_FILTER_MIN || parameter1 > VSS_FILTER_MAX) {
		parameter1 = VSS_FILTER_MIN;
	}
	if (parameter2 < VSS_FILTER_MIN || parameter2 > VSS_FILTER_MAX) {
		parameter2 = VSS_FILTER_MIN;
	}
	if (parameter3 < VSS_FILTER_MIN || parameter3 > VSS_FILTER_MAX) {
		parameter3 = VSS_FILTER_MIN;
	}
	if (parameter4 < VSS_FILTER_MIN || parameter4 > VSS_FILTER_MAX) {
		parameter4 = VSS_FILTER_MIN;
	}

	float filterParameter1 = 1.0f / parameter1;
	float filterParameter2 = 1.0f / parameter2;
	float filterParameter3 = 1.0f / parameter3;
	float filterParameter4 = 1.0f / parameter4;

	WheelSpeedFL.initIfValid(engineConfiguration->wheelSpeedSensorInputPin[0], converter1, filterParameter1);
	WheelSpeedFR.initIfValid(engineConfiguration->wheelSpeedSensorInputPin[1], converter2, filterParameter2);
	WheelSpeedRL.initIfValid(engineConfiguration->wheelSpeedSensorInputPin[2], converter3, filterParameter3);
	WheelSpeedRR.initIfValid(engineConfiguration->wheelSpeedSensorInputPin[3], converter4, filterParameter4);


  if (engineConfiguration->useAuxSpeedForSlipRatio) {
	  wheelSlipSensor.Register();
	}
}

void deinitAuxSpeedSensors() {
	auxSpeed1.deInit();
	auxSpeed2.deInit();
	WheelSpeedFL.deInit();
	WheelSpeedFR.deInit();
	WheelSpeedRL.deInit();
	WheelSpeedRR.deInit();

	wheelSlipSensor.unregister();
}
