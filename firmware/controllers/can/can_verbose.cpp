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

	msg.revLimit = GET_RPM() > engineConfiguration->rpmHardLimit;
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
	uint8_t pad[1];
};

static void populateFrame(Speeds& msg) {
	auto rpm = GET_RPM();
	msg.rpm = rpm;

	auto timing = engine->engineState.timingAdvance;
	msg.timing = timing > 360 ? timing - 720 : timing;

	msg.injDuty = getInjectorDutyCycle(rpm);
	msg.coilDuty = getCoilDutyCycle(rpm);

	msg.vssKph = Sensor::getOrZero(SensorType::VehicleSpeed);
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

	msg.mcuTemp = getMCUInternalTemperature();
	msg.fuelLevel = Sensor::getOrZero(SensorType::FuelLevel);
}

struct Sensors2 {
	scaled_afr afr;
	scaled_pressure oilPressure;
	scaled_angle vvtPos;
	scaled_voltage vbatt;
};

static void populateFrame(Sensors2& msg) {
	msg.afr = Sensor::getOrZero(SensorType::Lambda1) * STOICH_RATIO;
	msg.oilPressure = Sensor::get(SensorType::OilPressure).value_or(-1);
	msg.vvtPos = engine->triggerCentral.getVVTPosition(0, 0);
	msg.vbatt = Sensor::getOrZero(SensorType::BatteryVoltage);
}

struct Fueling {
	scaled_channel<uint16_t, 1000> cylAirmass;
	scaled_channel<uint16_t, 100> estAirflow;
	scaled_ms fuel_pulse;
	uint16_t pad;
};

static void populateFrame(Fueling& msg) {
	msg.cylAirmass = engine->engineState.sd.airMassInOneCylinder;
	msg.estAirflow = engine->engineState.airFlow;
	msg.fuel_pulse = engine->actualLastInjection[0];
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

void sendCanVerbose() {
	auto base = engineConfiguration->verboseCanBaseAddress;

	transmitStruct<Status>	  (base + 0);
	transmitStruct<Speeds>	  (base + 1);
	transmitStruct<PedalAndTps> (base + CAN_PEDAL_TPS_OFFSET);
	transmitStruct<Sensors1>	(base + CAN_SENSOR_1_OFFSET);
	transmitStruct<Sensors2>	(base + 4);
	transmitStruct<Fueling>	 (base + 5);
	transmitStruct<Fueling2>	(base + 6);
}

#endif // EFI_CAN_SUPPORT
