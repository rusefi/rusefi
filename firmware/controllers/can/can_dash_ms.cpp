/**
 * @file	can_dash_ms.cpp
 *
 * This file implements MegaSquirt CAN realtime data broadcasting format
 *
 * @date May 8, 2023
 * @author AndreyGusakov, (c) 2023
 */

#include "pch.h"

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST
#include "can.h"
#include "can_msg_tx.h"

#include "rusefi_types.h"
#include "rtc_helper.h"
#include "fuel_math.h"

/* TODO:
 * use engine->outputChannels instead of Sensor::getOrZero as it cheaper */

struct ms1512 {
	/* kPa */
	scaled_channel<int16_t, 10> Map;
	/* RPM */
	scaled_channel<uint16_t, 1> Rpm;
	/* Fahrenheit! */
	scaled_channel<int16_t, 10> Clt;
	/* % */
	scaled_channel<int16_t, 10> Tps;
};

void populateFrame(ms1512& msg)
{
	msg.Map = Sensor::getOrZero(SensorType::Map);
	msg.Rpm = Sensor::getOrZero(SensorType::Rpm);
	/* Celsius to Fahrenheit */
	msg.Clt = Sensor::getOrZero(SensorType::Clt) * 9 / 5 + 32;
	msg.Tps = Sensor::getOrZero(SensorType::Tps1);
}

struct ms1513 {
	/* mS */
	scaled_channel<uint16_t, 1000> pw1;
	scaled_channel<uint16_t, 1000> pw2;
	scaled_channel<int16_t, 10> mat;
	scaled_channel<int16_t, 10> adv_deg;
};

void populateFrame(ms1513& msg)
{
	/* TODO: per-bank */
	msg.pw1 = msg.pw2 = engine->engineState.injectionDuration;
	/* Celsius to Fahrenheit */
	msg.mat = Sensor::getOrZero(SensorType::Iat) * 9 / 5 + 32;
	float timing = engine->engineState.timingAdvance[0];
	msg.adv_deg = timing > 360 ? timing - 720 : timing;
}

struct ms1514 {
	scaled_channel<uint8_t, 10> afrtgt1;
	scaled_channel<uint8_t, 10> AFR1;
	scaled_channel<int16_t, 10> EGOcor1;
	scaled_channel<int16_t, 10> egt1;
	scaled_channel<int16_t, 10> pwseq1;
};

void populateFrame(ms1514& msg)
{
#if EFI_ENGINE_CONTROL
	msg.afrtgt1 = (float)engine->fuelComputer.targetLambda * STOICH_RATIO;
	msg.AFR1 = Sensor::getOrZero(SensorType::Lambda1) * STOICH_RATIO;
	/* TODO: banks? */
	msg.EGOcor1 = engine->engineState.stftCorrection[0];
	/* TODO */
	msg.egt1 = 0;
	msg.pwseq1 = engine->engineState.injectionDuration;
#endif // EFI_ENGINE_CONTROL
}

struct ms1515 {
	scaled_channel<uint16_t, 10> Vbat;
	scaled_channel<uint16_t, 10> sensor1;
	scaled_channel<uint16_t, 10> sensor2;
	scaled_channel<uint8_t, 10> knk_rtd;
	uint8_t unused;	/* do we need this? */
};

void populateFrame(ms1515& msg)
{
	msg.Vbat = Sensor::getOrZero(SensorType::BatteryVoltage);
	/* TODO */
	msg.sensor1 = 0;
	msg.sensor2 = 0;
	msg.knk_rtd = engine->module<KnockController>()->m_knockLevel;
	msg.unused = 0;
}

struct ms1516 {
	scaled_channel<uint16_t, 10> VSS1;
	scaled_channel<uint16_t, 10> tc_retard;
	scaled_channel<uint16_t, 10> launch_timing;
	uint16_t unsused;
};

void populateFrame(ms1516& msg)
{
	/* ms-1 ??? */
	msg.VSS1 = Sensor::getOrZero(SensorType::VehicleSpeed);
	/* TODO */
	msg.tc_retard = 0;
	msg.launch_timing = 0;
	msg.unsused = 0;
}

void canDashboardTS(CanCycle cycle) {
	/* TODO: get from settings */
	uint32_t baseId = 1512; /* 0x5e8 */

	if (!cycle.isInterval(CI::_10ms)) {
		return;
	}

	bool busIndex = 0;
	transmitStruct<ms1512>(CanCategory::NBC, baseId + 0, false, busIndex);
	transmitStruct<ms1513>(CanCategory::NBC, baseId + 1, false, busIndex);
	transmitStruct<ms1514>(CanCategory::NBC, baseId + 2, false, busIndex);
	transmitStruct<ms1515>(CanCategory::NBC, baseId + 3, false, busIndex);
	transmitStruct<ms1516>(CanCategory::NBC, baseId + 4, false, busIndex);
}

#endif /* EFI_CAN_SUPPORT */
