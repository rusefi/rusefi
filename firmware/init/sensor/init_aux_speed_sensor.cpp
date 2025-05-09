/*
 * @file init_aux_speed_sensor.cpp
 *
 */

#include "pch.h"
#include "init.h"
#include "frequency_sensor.h"

static FrequencySensor WheelSpeedFL(SensorType::WheelSpeedFL, MS2NT(500)); // fr
static FrequencySensor WheelSpeedFR(SensorType::WheelSpeedFR, MS2NT(500)); // fl
static FrequencySensor WheelSpeedRL(SensorType::WheelSpeedRL, MS2NT(500)); // rr
static FrequencySensor WheelSpeedRR(SensorType::WheelSpeedRR, MS2NT(500)); // rl

static FrequencySensor auxSpeed1(SensorType::AuxSpeed1, MS2NT(500)); // aux1
static FrequencySensor auxSpeed2(SensorType::AuxSpeed2, MS2NT(500)); // aux2
static FrequencySensor auxSpeed3(SensorType::AuxSpeed3, MS2NT(500)); // aux1
static FrequencySensor auxSpeed4(SensorType::AuxSpeed4, MS2NT(500)); // aux2


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
		return frequency * engineConfiguration->auxSpeedMult1;
	}
} converter5;

static class : public SensorConverter  {
	public:
	SensorResult convert(float frequency) const override {
		return frequency * engineConfiguration->auxSpeedMult2;
	}
} converter6;

static class : public SensorConverter  {
	public:
	SensorResult convert(float frequency) const override {
		return frequency * engineConfiguration->auxSpeedMult3;
	}
} converter7;

static class : public SensorConverter  {
	public:
	SensorResult convert(float frequency) const override {
		return frequency * engineConfiguration->auxSpeedMult4;
	}
} converter8;



class WheelSlip : public Sensor {
public:
  WheelSlip() : Sensor(SensorType::WheelSlipRatio)  {  }

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

static WheelSlip wheelSlipSensor;





// this one needs averaged of front and rear
class WheelSlipFrontRear : public Sensor {
public:
	WheelSlipFrontRear() : Sensor(SensorType::WheelSlipRatioFrontRear)  {  }


//	SensorResult clt = Sensor::get(SensorType::Clt);
//	engine->outputChannels.coolant = clt.value_or(0);
//	engine->outputChannels.isCltError = !clt.Valid;

	SensorResult get() const final override {
		auto FRONTSPEEDAVG = Sensor::get(SensorType::WheelSpeedFrontAvg);
		auto REARSPEEDAVG  = Sensor::get(SensorType::WheelSpeedRearAvg);

		if (!FRONTSPEEDAVG.Valid && !REARSPEEDAVG.Valid) { // no wheels
			return UnexpectedCode::Unknown;
		}

        if ((!FRONTSPEEDAVG.Valid && REARSPEEDAVG.Valid) || (!REARSPEEDAVG.Valid && FRONTSPEEDAVG.Valid)) { // can't calculate front rear slip with only front or rear
          	return 1;
        }


        // treating awd as rwd, not enough knowledge
		if (engineConfiguration->drive_train_layout == DRIVETRAIN_RWD || engineConfiguration->drive_train_layout == DRIVETRAIN_AWD) { // slip 1 = front wheel rotating same speed as rear, slip > 1 rear wheel rotating faster, slip < 1 rear wheel rotating slower = LOCKED
	        if (FRONTSPEEDAVG.Valid && REARSPEEDAVG.Valid && FRONTSPEEDAVG.value_or(0) > 0) {	// front and rear present
            	return REARSPEEDAVG.value_or(0) /FRONTSPEEDAVG.value_or(0);
            }
        }

        // fwd , rear wheels are dragging, so slip goes same way, > 1 = overpowering driving wheels
		if (engineConfiguration->drive_train_layout == DRIVETRAIN_FWD) { // slip 1 = front wheel rotating same speed as rear, slip > 1 front wheel rotating faster, slip < 1 front wheel rotating slower = LOCKED
			if (FRONTSPEEDAVG.Valid && REARSPEEDAVG.Valid && REARSPEEDAVG.value_or(0) > 0) {	// front and rear present
				return FRONTSPEEDAVG.value_or(0) / REARSPEEDAVG.value_or(0);
			}
		}

		return UnexpectedCode::Unknown;
	}

	void showInfo(const char*) const override { }
};

static WheelSlipFrontRear wheelSlipSensorFrontRear;








class WheelSlipLeftRightFRONT : public Sensor {
public:
	WheelSlipLeftRightFRONT() : Sensor(SensorType::WheelSlipRatioLeftRightFRONT)  {  }

	SensorResult get() const final override {
		auto FL = WheelSpeedFL.get();
		auto FR = WheelSpeedFR.get();

		if (!FL.Valid && !FR.Valid) { // no wheels
			return UnexpectedCode::Unknown;
		}

		if ((!FL.Valid && FR.Valid) || (!FR.Valid && FL.Valid)) { // only one wheel defined, can't slip with itself, so slip is 1
			return 1;
		}

		if (FR.Valid && FL.Valid && FR.value_or(0) > 0) {
			return FL.value_or(0) / FR.value_or(0);
		}

        return 1;
	}
	void showInfo(const char*) const override { }
};

static WheelSlipLeftRightFRONT wheelSlipSensorLeftRightFRONT;



// calculate the slip ratio between rear LEFT and rear RIGHT wheels
class WheelSlipLeftRightREAR : public Sensor {
public:
	WheelSlipLeftRightREAR() : Sensor(SensorType::WheelSlipRatioLeftRightREAR)  {  }

	SensorResult get() const final override {
		auto RL = WheelSpeedRL.get();
		auto RR = WheelSpeedRR.get();

		if (!RL.Valid && !RR.Valid) { // no wheels
			return UnexpectedCode::Unknown;
		}

        if ((!RL.Valid && RR.Valid) || (!RR.Valid && RL.Valid)) { // only one wheel defined, can't slip with itself, so slip is 1
          return 1;
        }

        if (RR.Valid && RL.Valid && RR.value_or(0) > 0) {
          return RL.value_or(0) / RR.value_or(0);
        }
		return 1;
	}
	void showInfo(const char*) const override { }
};

static WheelSlipLeftRightREAR wheelSlipSensorLeftRightREAR;

class WheelSpeedFrontRearAvg : public Sensor {
public:
	WheelSpeedFrontRearAvg() : Sensor(SensorType::WheelSpeedFrontRearAvg)  {  }

	SensorResult get() const final override {
		auto FRONTSPEEDAVG = Sensor::get(SensorType::WheelSpeedFrontAvg);
		auto REARSPEEDAVG  = Sensor::get(SensorType::WheelSpeedRearAvg);

		if (!FRONTSPEEDAVG.Valid && !REARSPEEDAVG.Valid) { // no wheels
			return UnexpectedCode::Unknown;
		}

		if ((!FRONTSPEEDAVG.Valid && REARSPEEDAVG.Valid)) { // only rear valid
			return REARSPEEDAVG.value_or(0);
		}

		if ((!REARSPEEDAVG.Valid && FRONTSPEEDAVG.Valid)) { // only front  valid
			return FRONTSPEEDAVG.value_or(0);
		}

		if (FRONTSPEEDAVG.Valid && REARSPEEDAVG.Valid) { // both valid, return average
			return (FRONTSPEEDAVG.value_or(0) + REARSPEEDAVG.value_or(0)) / 2;
		}
		return 1;
	}
	void showInfo(const char*) const override { }
};

static WheelSpeedFrontRearAvg WheelSpeedFrontRearAvg;


class WheelSpeedFrontAvg : public Sensor {
public:
	WheelSpeedFrontAvg() : Sensor(SensorType::WheelSpeedFrontAvg)  {  }

	SensorResult get() const final override {
		auto FL = WheelSpeedFL.get();
		auto FR = WheelSpeedFR.get();

		if (!FL.Valid && !FR.Valid) { // no wheels
			return UnexpectedCode::Unknown;
		}

		if ((!FL.Valid && FR.Valid)) { // only one wheel defined, can't slip with itself, so slip is 1
			return FR.value_or(0);
		}

		if ((!FR.Valid && FL.Valid)) { // only one wheel defined, can't slip with itself, so slip is 1
			return FL.value_or(0);
		}

		if (FR.Valid && FL.Valid) { // both valid, return average
			return (FL.value_or(0) + FR.value_or(0)) / 2;
		}
		return 1;
	}
	void showInfo(const char*) const override { }
};

static WheelSpeedFrontAvg WheelSpeedFrontAvg;

class WheelSpeedRearAvg : public Sensor {
public:
	WheelSpeedRearAvg() : Sensor(SensorType::WheelSpeedRearAvg)  {  }

	SensorResult get() const final override {
		auto RL = WheelSpeedRL.get();
		auto RR = WheelSpeedRR.get();

		if (!RL.Valid && !RR.Valid) { // no wheels
			return UnexpectedCode::Unknown;
		}

		if ((!RL.Valid && RR.Valid)) { // only one wheel defined, can't slip with itself, so slip is 1
			return RR.value_or(0);
		}

		if ((!RR.Valid && RL.Valid)) { // only one wheel defined, can't slip with itself, so slip is 1
			return RL.value_or(0);
		}

		if (RR.Valid && RL.Valid) { // both valid, return average
			return (RL.value_or(0) + RR.value_or(0)) / 2;
		}
		return 1;
	}
	void showInfo(const char*) const override { }
};

static WheelSpeedRearAvg WheelSpeedRearAvg;






void initAuxSpeedSensors() {
	WheelSpeedFL.useBiQuad = engineConfiguration->useBiQuadOnWheelSpeedSensors1;
	WheelSpeedFR.useBiQuad = engineConfiguration->useBiQuadOnWheelSpeedSensors2;
	WheelSpeedRL.useBiQuad = engineConfiguration->useBiQuadOnWheelSpeedSensors3;
	WheelSpeedRR.useBiQuad = engineConfiguration->useBiQuadOnWheelSpeedSensors4;

	auxSpeed1.useBiQuad = engineConfiguration->useBiQuadOnAuxSpeedSensors1;
	auxSpeed2.useBiQuad = engineConfiguration->useBiQuadOnAuxSpeedSensors2;
	auxSpeed3.useBiQuad = engineConfiguration->useBiQuadOnAuxSpeedSensors3;
	auxSpeed4.useBiQuad = engineConfiguration->useBiQuadOnAuxSpeedSensors4;


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

	auxSpeed1.initIfValid(engineConfiguration->auxSpeedSensorInputPin[0], converter5, engineConfiguration->auxFrequencyFilter1);
	auxSpeed2.initIfValid(engineConfiguration->auxSpeedSensorInputPin[1], converter6, engineConfiguration->auxFrequencyFilter2);
	auxSpeed3.initIfValid(engineConfiguration->auxSpeedSensorInputPin[2], converter5, engineConfiguration->auxFrequencyFilter3);
	auxSpeed4.initIfValid(engineConfiguration->auxSpeedSensorInputPin[3], converter6, engineConfiguration->auxFrequencyFilter4);

  if (engineConfiguration->useAuxSpeedForSlipRatio) {
	wheelSlipSensor.Register();
  	wheelSlipSensorFrontRear.Register();
  	wheelSlipSensorLeftRightFRONT.Register();
  	wheelSlipSensorLeftRightREAR.Register();
  	WheelSpeedFrontAvg.Register();
  	WheelSpeedRearAvg.Register();
  	WheelSpeedFrontRearAvg.Register();
  }
}

void deinitAuxSpeedSensors() {
	auxSpeed1.deInit();
	auxSpeed2.deInit();
	auxSpeed3.deInit();
	auxSpeed4.deInit();
	WheelSpeedFL.deInit();
	WheelSpeedFR.deInit();
	WheelSpeedRL.deInit();
	WheelSpeedRR.deInit();

	wheelSlipSensor.unregister();
	wheelSlipSensorFrontRear.unregister();
	wheelSlipSensorLeftRightFRONT.unregister();
	wheelSlipSensorLeftRightREAR.unregister();
	WheelSpeedFrontAvg.unregister();
	WheelSpeedRearAvg.unregister();
	WheelSpeedFrontRearAvg.unregister();


}
