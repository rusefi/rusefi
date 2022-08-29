/**
 * @file	can_verbose.cpp
 *
 * TODO: change 'verbose' into 'broadcast'?
 *
 * @author Matthew Kennedy, (c) 2020
 */

#include "pch.h"
#if EFI_CAN_SUPPORT

#include "scaled_channel.h"
#include "can_msg_tx.h"
#include "can.h"
#include "fuel_math.h"
#include "spark_logic.h"

struct Status {
	uint16_t warningCounter;
	uint16_t lastErrorCode;

	uint8_t revLimit : 1;
	uint8_t mainRelay : 1;
	uint8_t fuelPump : 1;
	uint8_t checkEngine : 1;
	uint8_t o2Heater : 1;

	uint8_t pad6 : 1;
	uint8_t pad7 : 1;
	uint8_t pad8 : 1;

	uint8_t pad[3];
};

static void populateFrame(Status& msg) {
	msg.warningCounter = engine->engineState.warnings.warningCounter;
	msg.lastErrorCode = engine->engineState.warnings.lastErrorCode;

	msg.revLimit = Sensor::getOrZero(SensorType::Rpm) > engineConfiguration->rpmHardLimit;
	msg.mainRelay = enginePins.mainRelay.getLogicValue();
	msg.fuelPump = enginePins.fuelPumpRelay.getLogicValue();
	msg.checkEngine = enginePins.checkEnginePin.getLogicValue();
	msg.o2Heater = enginePins.o2heater.getLogicValue();
}

struct Speeds {
	uint16_t rpm;
	scaled_angle timing;
	scaled_channel<uint8_t, 2> injDuty;
	scaled_channel<uint8_t, 2> coilDuty;
	scaled_channel<uint8_t> vssKph;
	uint8_t EthanolPercent;
};

static void populateFrame(Speeds& msg) {
	auto rpm = Sensor::getOrZero(SensorType::Rpm);
	msg.rpm = rpm;

	auto timing = engine->engineState.timingAdvance[0];
	msg.timing = timing > 360 ? timing - 720 : timing;

	msg.injDuty = getInjectorDutyCycle(rpm);
	msg.coilDuty = getCoilDutyCycle(rpm);

	msg.vssKph = Sensor::getOrZero(SensorType::VehicleSpeed);

	msg.EthanolPercent = Sensor::getOrZero(SensorType::FuelEthanolPercent);
}

struct PedalAndTps {
	scaled_percent pedal;
	scaled_percent tps1;
	scaled_percent tps2;
	uint8_t pad[2];
};

static void populateFrame(PedalAndTps& msg)
{
	msg.pedal = Sensor::get(SensorType::AcceleratorPedal).value_or(-1);
	msg.tps1 = Sensor::get(SensorType::Tps1).value_or(-1);
	msg.tps2 = Sensor::get(SensorType::Tps2).value_or(-1);
}

struct Sensors1 {
	scaled_pressure map;
	scaled_channel<uint8_t> clt;
	scaled_channel<uint8_t> iat;
	scaled_channel<uint8_t> aux1;
	scaled_channel<uint8_t> aux2;
	scaled_channel<uint8_t> mcuTemp;
	scaled_channel<uint8_t, 2> fuelLevel;
};

static void populateFrame(Sensors1& msg) {
	msg.map = Sensor::getOrZero(SensorType::Map);

	msg.clt = Sensor::getOrZero(SensorType::Clt) + PACK_ADD_TEMPERATURE;
	msg.iat = Sensor::getOrZero(SensorType::Iat) + PACK_ADD_TEMPERATURE;

	msg.aux1 = Sensor::getOrZero(SensorType::AuxTemp1) + PACK_ADD_TEMPERATURE;
	msg.aux2 = Sensor::getOrZero(SensorType::AuxTemp2) + PACK_ADD_TEMPERATURE;

#if	HAL_USE_ADC
	msg.mcuTemp = getMCUInternalTemperature();
#endif

	msg.fuelLevel = Sensor::getOrZero(SensorType::FuelLevel);
}

struct Sensors2 {
	scaled_afr afr; // deprecated
	scaled_pressure oilPressure;
	scaled_angle vvtPos;	// deprecated
	scaled_voltage vbatt;
};

static void populateFrame(Sensors2& msg) {
	msg.afr = Sensor::getOrZero(SensorType::Lambda1) * STOICH_RATIO;
	msg.oilPressure = Sensor::get(SensorType::OilPressure).value_or(-1);
#if EFI_SHAFT_POSITION_INPUT
	msg.vvtPos = engine->triggerCentral.getVVTPosition(0, 0);
#endif // EFI_SHAFT_POSITION_INPUT
	msg.vbatt = Sensor::getOrZero(SensorType::BatteryVoltage);
}

struct Fueling {
	scaled_channel<uint16_t, 1000> cylAirmass;
	scaled_channel<uint16_t, 100> estAirflow;
	scaled_ms fuel_pulse;
	uint16_t knockCount;
};

static void populateFrame(Fueling& msg) {
	msg.cylAirmass = engine->engineState.sd.airMassInOneCylinder;
	msg.estAirflow = engine->engineState.airflowEstimate;
	msg.fuel_pulse = engine->actualLastInjection[0];
	msg.knockCount = engine->module<KnockController>()->getKnockCount();
}

struct Fueling2 {
	scaled_channel<uint16_t> fuelConsumedGram;
	scaled_channel<uint16_t, PACK_MULT_FUEL_FLOW> fuelFlowRate;
	scaled_percent fuelTrim[2];
};

static void populateFrame(Fueling2& msg) {
	msg.fuelConsumedGram = engine->engineState.fuelConsumption.getConsumedGrams();
	msg.fuelFlowRate = engine->engineState.fuelConsumption.getConsumptionGramPerSecond();

	for (size_t i = 0; i < 2; i++) {
		msg.fuelTrim[i] = 100.0f * (engine->stftCorrection[i] - 1.0f);
	}
}

struct Fueling3 {
	scaled_channel<uint16_t, 10000> Lambda;
	scaled_channel<uint16_t, 10000> Lambda2;
	scaled_channel<int16_t, 30> FuelPressureLow;
	scaled_channel<int16_t, 10> FuelPressureHigh;
};

static void populateFrame(Fueling3& msg) {
	msg.Lambda = Sensor::getOrZero(SensorType::Lambda1);
	msg.Lambda2 = Sensor::getOrZero(SensorType::Lambda2);
	msg.FuelPressureLow = Sensor::getOrZero(SensorType::FuelPressureLow);
	msg.FuelPressureHigh = KPA2BAR(Sensor::getOrZero(SensorType::FuelPressureHigh));
}

struct Cams {
	int8_t Bank1IntakeActual;
	int8_t Bank1IntakeTarget;
	int8_t Bank1ExhaustActual;
	int8_t Bank1ExhaustTarget;
	int8_t Bank2IntakeActual;
	int8_t Bank2IntakeTarget;
	int8_t Bank2ExhaustActual;
	int8_t Bank2ExhaustTarget;
};

static void populateFrame(Cams& msg) {
	msg.Bank1IntakeActual  = engine->triggerCentral.getVVTPosition(0, 0);
	msg.Bank1ExhaustActual = engine->triggerCentral.getVVTPosition(0, 1);
	msg.Bank2IntakeActual  = engine->triggerCentral.getVVTPosition(1, 0);
	msg.Bank2ExhaustActual = engine->triggerCentral.getVVTPosition(1, 1);
	
	// TODO: maybe don't rely on outputChannels here
	msg.Bank1IntakeTarget = engine->outputChannels.vvtTargets[0];
	msg.Bank1ExhaustTarget = engine->outputChannels.vvtTargets[1];
	msg.Bank2IntakeTarget = engine->outputChannels.vvtTargets[2];
	msg.Bank2ExhaustTarget = engine->outputChannels.vvtTargets[3];
}

void sendCanVerbose() {
	auto base = engineConfiguration->verboseCanBaseAddress;
	auto isExt = engineConfiguration->rusefiVerbose29b;

	transmitStruct<Status>	    (CanCategory::VERBOSE, base + 0, isExt);
	transmitStruct<Speeds>	    (CanCategory::VERBOSE, base + 1, isExt);
	transmitStruct<PedalAndTps> (CanCategory::VERBOSE, base + CAN_PEDAL_TPS_OFFSET, isExt);
	transmitStruct<Sensors1>	(CanCategory::VERBOSE, base + CAN_SENSOR_1_OFFSET, isExt);
	transmitStruct<Sensors2>	(CanCategory::VERBOSE, base + 4, isExt);
	transmitStruct<Fueling>	    (CanCategory::VERBOSE, base + 5, isExt);
	transmitStruct<Fueling2>	(CanCategory::VERBOSE, base + 6, isExt);
	transmitStruct<Fueling3>	(CanCategory::VERBOSE, base + 7, isExt);
	transmitStruct<Cams>		(CanCategory::VERBOSE, base + 8, isExt);
}

#endif // EFI_CAN_SUPPORT
