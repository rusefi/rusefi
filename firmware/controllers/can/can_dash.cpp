/**
 * @file	can_dash.cpp
 *
 * This file handles transmission of ECU data to various OE dashboards.
 *
 * @date Mar 19, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#include "pch.h"

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST
#include "can_dash.h"
#include "can_dash_ms.h"
#include "can_dash_nissan.h"
#include "can_dash_haltech.h"
#include "board_overrides.h"
#include "can_bmw.h"
#include "can_vag.h"
#include "can_dash_honda.h"

#include "rusefi_types.h"
#include "rtc_helper.h"
#include "fuel_math.h"

// CAN Bus ID for broadcast
#define CAN_FIAT_MOTOR_INFO           0x561
#define CAN_MAZDA_RX_RPM_SPEED        0x201
#define CAN_MAZDA_RX_STEERING_WARNING 0x300
#define CAN_MAZDA_RX_STATUS_1         0x212
#define CAN_MAZDA_RX_STATUS_2         0x420

//w202 DASH
#define W202_STAT_1	     0x308 /* _20ms cycle */
#define W202_STAT_2      0x608 /* _100ms cycle */
#define W202_ALIVE	     0x210 /* _200ms cycle */
#define W202_STAT_3      0x310 /* _200ms cycle */

//BMW E90 DASH
#define E90_ABS_COUNTER      0x0C0
#define E90_SEATBELT_COUNTER 0x0D7
#define E90_T15	             0x130
#define E90_RPM              0x175
#define E90_BRAKE_COUNTER    0x19E
#define E90_SPEED            0x1A6
// https://github.com/HeinrichG-V12/E65_ReverseEngineering/blob/main/docs/0x1D0.md
#define E90_TEMP             0x1D0
// MECH Anzeige Getriebedaten
#define E90_GEAR             0x1D2
#define E90_FUEL             0x349
#define E90_EBRAKE           0x34F
#define E90_TIME             0x39E

#if !EFI_UNIT_TEST
static time_msecs_t mph_timer;
static time_msecs_t mph_ctr;
#endif

/**
 * https://docs.google.com/spreadsheets/d/1IkP05ODpjNt-k4YQLYl58_TNlN9U4IBu5z7i0BPVEM4
 */
#define GENESIS_COUPLE_RPM_316 0x316
#define GENESIS_COUPLE_COOLANT_329 0x329
#define GENESIS_COUPLE_SENSORS_382 0x382
// when A/C compressor is allowed to be on, these values need to be sent so the A/C panel activates the compressor
#define GENESIS_COUPLE_AC_ENABLE_18F 0x18F


static uint8_t rpmcounter;
static uint8_t seatbeltcnt;
static uint8_t abscounter = 0xF0;
static uint8_t brakecnt_1 = 0xF0, brakecnt_2 = 0xF0;
static uint8_t mph_a, mph_2a, mph_last, tmp_cnt, gear_cnt;
static uint16_t mph_counter = 0xF000;
static bool cluster_time_set;

constexpr uint8_t e90_temp_offset = 49;

// todo: those forward declarations are out of overall code style
void canDashboardFiat(CanCycle cycle);
void canMazdaRX8(CanCycle cycle);
void canDashboardW202(CanCycle cycle);
void canDashboardVagMqb(CanCycle cycle);
void canDashboardGenesisCoupe(CanCycle cycle);
void canDashboardAim(CanCycle cycle);

//BMW Dashboard
//todo: we use 50ms fixed cycle, trace is needed to check for correct period
static void canDashboardBmwE46(CanCycle cycle) {

	if (cycle.isInterval(CI::_10ms)) {
		{
			CanTxMessage msg(CanCategory::NBC, CAN_BMW_E46_RPM);
      msg[0] = 0x05; // ASC message
      msg[1] = 0x0C; // Indexed Engine Torque in % of C_TQ_STND TBD
			msg.setShortValue((int) (Sensor::getOrZero(SensorType::Rpm) * 6.4), 2);
      msg[4] = 0x0C;
      msg[5] = 0x15;
      msg[6] = 0x00;
      msg[7] = 0x35;
		}

		{
			CanTxMessage msg(CanCategory::NBC, CAN_BMW_E46_DME2);
      msg[0] = 0x11;
      msg[1] = (Sensor::getOrZero(SensorType::Clt) + 48.373) / 0.75;
      msg[2] = 0x00; // baro sensor
      msg[3] = 0x08;
      msg[4] = 0x00; // TPS_VIRT_CRU_CAN, not used.
      msg[5] = 0x00; // TPS out, but we set to 0 just in case.
      msg[6] = 0x00; // brake system status Ok.
      msg[7] = 0x00; // not used
		}
  }
}

//todo: we use 50ms fixed cycle, trace is needed to check for correct period
void canMazdaRX8(CanCycle cycle) {
	if (cycle.isInterval(CI::_50ms)) {
		{
			CanTxMessage msg(CanCategory::NBC, CAN_MAZDA_RX_STEERING_WARNING);
			// todo: something needs to be set here? see http://rusefi.com/wiki/index.php?title=Vehicle:Mazda_Rx8_2004
		}

		{
			CanTxMessage msg(CanCategory::NBC, CAN_MAZDA_RX_RPM_SPEED);

			float kph = Sensor::getOrZero(SensorType::VehicleSpeed);

      // todo: LSB+SWAP? lol, that's MSB?
			msg.setShortValue(SWAP_UINT16(Sensor::getOrZero(SensorType::Rpm) * 4), 0);
			msg.setShortValue(0xFFFF, 2);
      // todo: LSB+SWAP? lol, that's MSB?
			msg.setShortValue(SWAP_UINT16((int )(100 * kph + 10000)), 4);
			msg.setShortValue(0, 6);
		}

		{
			CanTxMessage msg(CanCategory::NBC, CAN_MAZDA_RX_STATUS_1);
			msg[0] = 0xFE; //Unknown
			msg[1] = 0xFE; //Unknown
			msg[2] = 0xFE; //Unknown
			msg[3] = 0x34; //DSC OFF in combo with byte 5 Live data only seen 0x34
			msg[4] = 0x00; // B01000000; // Brake warning B00001000;  //ABS warning
			msg[5] = 0x40; // TCS in combo with byte 3
			msg[6] = 0x00; // Unknown
			msg[7] = 0x00; // Unused
		}

		{
			CanTxMessage msg(CanCategory::NBC, CAN_MAZDA_RX_STATUS_2);
			auto clt = Sensor::get(SensorType::Clt);
			msg[0] = (uint8_t)(clt.value_or(0) + 69); //temp gauge //~170 is red, ~165 last bar, 152 centre, 90 first bar, 92 second bar
			// TODO: fixme!
			//msg[1] = ((int16_t)(engine->engineState.vssEventCounter*(engineConfiguration->vehicleSpeedCoef*0.277*2.58))) & 0xff;
			msg[2] = 0x00; // unknown
			msg[3] = 0x00; //unknown
			msg[4] = 0x01; //Oil Pressure (not really a gauge)
			msg[5] = 0x00; //check engine light
			msg[6] = 0x00; //Coolant, oil and battery
			if ((Sensor::getOrZero(SensorType::Rpm)>0) && (Sensor::get(SensorType::BatteryVoltage).value_or(VBAT_FALLBACK_VALUE)<13)) {
				msg.setBit(6, 6); // battery light
			}
			if (!clt.Valid || clt.Value > 105) {
				// coolant light, 101 - red zone, light means its get too hot
				// Also turn on the light in case of sensor failure
				msg.setBit(6, 1);
			}
			//oil pressure warning lamp bit is 7
			msg[7] = 0x00; //unused
		}
	}

}

void canDashboardFiat(CanCycle cycle) {
	if (cycle.isInterval(CI::_50ms)) {
		{
			//Fiat Dashboard
			CanTxMessage msg(CanCategory::NBC, CAN_FIAT_MOTOR_INFO);
			msg.setShortValue((int) (Sensor::getOrZero(SensorType::Clt) - 40), 3); //Coolant Temp
			msg.setShortValue(Sensor::getOrZero(SensorType::Rpm) / 32, 6); //RPM
		}
	}
}

void canDashboardVAG(CanCycle cycle) {
	if (cycle.isInterval(CI::_10ms)) {
		{
			// https://github.com/commaai/opendbc/blob/57c8340a180dd8c75139b18050eb17c72c9cb6e4/vw_golf_mk4.dbc#L394
			//VAG Dashboard
			CanTxMessage msg(CanCategory::NBC, CAN_VAG_Motor_1);
			msg.setShortValue(Sensor::getOrZero(SensorType::Rpm) * 4, 2); //RPM
		}

		float clt = Sensor::getOrZero(SensorType::Clt);

		{
			CanTxMessage msg(CanCategory::NBC, CAN_VAG_Motor_2);
			msg.setShortValue((int) ((clt + 48.373) / 0.75), 1); //Coolant Temp
		}

		{
			CanTxMessage msg(CanCategory::NBC, CAN_VAG_CLT_V2);
			msg.setShortValue((int) ((clt + 48.373) / 0.75), 4); //Coolant Temp
		}

		{
			CanTxMessage msg(CanCategory::NBC, CAN_VAG_IMMO);
			msg.setShortValue(0x80, 1);
		}
	}
}

void canDashboardW202(CanCycle cycle) {
	if (cycle.isInterval(CI::_20ms)) {
		{
			CanTxMessage msg(CanCategory::NBC, W202_STAT_1);
			msg[0] = 0x08; // Unknown
			msg.setShortValueMsb(Sensor::getOrZero(SensorType::Rpm), 1);
			msg[3] = 0x00; // 0x01 - tank blink, 0x02 - EPC
			msg[4] = 0x00; // Unknown
			msg[5] = 0x00; // Unknown
			msg[6] = 0x00; // Unknown - oil info
			msg[7] = 0x00; // Unknown - oil info
		}
	}

	if (cycle.isInterval(CI::_100ms)) {
		{
			CanTxMessage msg(CanCategory::NBC, W202_STAT_2); //dlc 7
			msg[0] = (int)(Sensor::getOrZero(SensorType::Clt) + 40); // CLT -40 offset
			msg[1] = 0x3D; // TBD
			msg[2] = 0x63; // Const
			msg[3] = 0x41; // Const
			msg[4] = 0x00; // Unknown
			msg[5] = 0x05; // Const
			msg[6] = 0x50; // TBD
			msg[7] = 0x00; // Unknown
		}
	}

	if (cycle.isInterval(CI::_200ms)) {
		{
			CanTxMessage msg(CanCategory::NBC, W202_ALIVE);
			msg[0] = 0x0A; // Const
			msg[1] = 0x18; // Const
			msg[2] = 0x00; // Const
			msg[3] = 0x00; // Const
			msg[4] = 0xC0; // Const
			msg[5] = 0x00; // Const
			msg[6] = 0x00; // Const
			msg[7] = 0x00; // Const
		}

		{
			CanTxMessage msg(CanCategory::NBC, W202_STAT_3);
			msg[0] = 0x00; // Const
			msg[1] = 0x00; // Const
			msg[2] = 0x6D; // TBD
			msg[3] = 0x7B; // Const
			msg[4] = 0x21; // TBD
			msg[5] = 0x07; // Const
			msg[6] = 0x33; // Const
			msg[7] = 0x05; // Const
		}
	}
}

void canDashboardGenesisCoupe(CanCycle cycle) {
	if (cycle.isInterval(CI::_50ms)) {
    {
			CanTxMessage msg(CanCategory::NBC, GENESIS_COUPLE_RPM_316, 8);
			msg.setShortValueMsb(Sensor::getOrZero(SensorType::Rpm) * 4, /*offset*/ 3);
	  }
		{
			CanTxMessage msg(CanCategory::NBC, GENESIS_COUPLE_COOLANT_329, 8);
			int clt = Sensor::getOrZero(SensorType::Clt) * 2;
			msg[1] = clt;
		}
	}
}

/**
 * https://docs.google.com/spreadsheets/d/1XMfeGlhgl0lBL54lNtPdmmFd8gLr2T_YTriokb30kJg
 */
void canDashboardVagMqb(CanCycle cycle) {
	if (cycle.isInterval(CI::_50ms)) {

		{ // 'turn-on'
			CanTxMessage msg(CanCategory::NBC, 0x3C0, 4);
			// ignition ON
			msg[2] = 3;
		}

		{ //RPM
			CanTxMessage msg(CanCategory::NBC, 0x107, 8);
			msg.setShortValue(Sensor::getOrZero(SensorType::Rpm) / 3.5, /*offset*/ 3);
		}
	}
}

static void canDashboardBmwE90(CanCycle cycle) {

	if (cycle.isInterval(CI::_50ms)) {

		{ //T15 'turn-on'
			CanTxMessage msg(CanCategory::NBC, E90_T15, 5);
			msg[0] = 0x45;
			msg[1] = 0x41;
			msg[2] = 0x61;
			msg[3] = 0x8F;
			msg[4] = 0xFC;
		}

		{ //Ebrake light
			CanTxMessage msg(CanCategory::OBD, E90_EBRAKE, 2);
			msg[0] = 0xFD;
			msg[1] = 0xFF;
		}

		{ //RPM
			rpmcounter++;
			if (rpmcounter > 0xFE)
				rpmcounter = 0xF0;
			CanTxMessage msg(CanCategory::OBD, E90_RPM, 3);
			msg[0] = rpmcounter;
			msg.setShortValue(Sensor::getOrZero(SensorType::Rpm) * 4, 1);
		}

		{ //oil & coolant temp (all in C, despite gauge being F)
			tmp_cnt++;
			if (tmp_cnt >= 0x0F)
				tmp_cnt = 0x00;
			CanTxMessage msg(CanCategory::OBD, E90_TEMP, 8);
			msg[0] = (int)(Sensor::getOrZero(SensorType::Clt) + e90_temp_offset); //coolant
			msg[1] = (int)(Sensor::getOrZero(SensorType::AuxTemp1) + e90_temp_offset); //oil (AuxTemp1)
			msg[2] = tmp_cnt;
			msg[3] = 0xC8;
			msg[4] = 0xA7;
			msg[5] = 0xD3;
			msg[6] = 0x0D;
			msg[7] = 0xA8;
		}
	}

	if (cycle.isInterval(CI::_100ms)) {
		{
			//Seatbelt counter
			seatbeltcnt++;
			if (seatbeltcnt > 0xFE)
				seatbeltcnt = 0x00;
			CanTxMessage msg(CanCategory::NBC, E90_SEATBELT_COUNTER, 2);
			msg[0] = seatbeltcnt;
			msg[1] = 0xFF;
		}

		{
			//Brake counter 100ms
			brakecnt_1 += 16;
			brakecnt_2 += 16;
			if (brakecnt_1 > 0xEF)
				brakecnt_1 = 0x0F;
			if (brakecnt_2 > 0xF0)
				brakecnt_2 = 0xA0;
			CanTxMessage msg(CanCategory::NBC, E90_BRAKE_COUNTER, 8);
			msg[0] = 0x00;
			msg[1] = 0xE0;
			msg[2] = brakecnt_1;
			msg[3] = 0xFC;
			msg[4] = 0xFE;
			msg[5] = 0x41;
			msg[6] = 0x00;
			msg[7] = brakecnt_2;
		}

		{ //ABS counter
			abscounter++;
			if (abscounter > 0xFE)
				abscounter = 0xF0;
			CanTxMessage msg(CanCategory::NBC, E90_ABS_COUNTER, 2);
			msg[0] = abscounter;
			msg[1] = 0xFF;
		}

		{ //Fuel gauge
			CanTxMessage msg(CanCategory::NBC, E90_FUEL, 5); //fuel gauge
			msg[0] = 0x76;
			msg[1] = 0x0F;
			msg[2] = 0xBE;
			msg[3] = 0x1A;
			msg[4] = 0x00;
		}

		{ //Gear indicator/counter
			gear_cnt++;
			if (gear_cnt >= 0x0F)
				gear_cnt = 0x00;
			CanTxMessage msg(CanCategory::NBC, E90_GEAR, 6);
			msg[0] = 0x78;
			msg[1] = 0x0F;
			msg[2] = 0xFF;
			msg[3] = (gear_cnt << 4) | 0xC;
			msg[4] = 0xF1;
			msg[5] = 0xFF;
		}

#if !EFI_UNIT_TEST
		{ //E90_SPEED
			auto vehicleSpeed = Sensor::getOrZero(SensorType::VehicleSpeed);
			float mph = vehicleSpeed * 0.6213712;
			mph_ctr = ((TIME_I2MS(chVTGetSystemTime()) - mph_timer) / 50);
			mph_a = (mph_ctr * mph / 2);
			mph_2a = mph_a + mph_last;
			mph_last = mph_2a;
			mph_counter += mph_ctr * 100;
			if(mph_counter >= 0xFFF0)
				mph_counter = 0xF000;
			mph_timer = TIME_I2MS(chVTGetSystemTime());
			CanTxMessage msg(CanCategory::NBC, E90_SPEED, 8);
			msg.setShortValue(mph_2a, 0);
			msg.setShortValue(mph_2a, 2);
			msg.setShortValue(mph_2a, 4);
			msg[6] = mph_counter & 0xFF;
			// todo: what are we packing into what exactly? note the '| 0xF0'
			msg[7] = (mph_counter >> 8) | 0xF0;
		}
#endif
	}

	{
		if (!cluster_time_set) {
#if EFI_RTC
			efidatetime_t dateTime = getRtcDateTime();
#else // EFI_RTC
			efidatetime_t dateTime = {
				.year = 0, .month = 0, .day = 0,
				.hour = 0, .minute = 0, .second = 0,
			};
#endif // EFI_RTC
			CanTxMessage msg(CanCategory::NBC, E90_TIME, 8);
			msg[0] = dateTime.hour;
			msg[1] = dateTime.minute;
			msg[2] = dateTime.second;
			msg[3] = dateTime.day;
			msg[4] = (dateTime.month << 4) | 0x0F;
			msg[5] = dateTime.year & 0xFF;
			msg[6] = (dateTime.year >> 8) | 0xF0; // collides CAN dash at 4096!
			msg[7] = 0xF2;
			cluster_time_set = 1;
		}
	}
}

//Based on AIM can protocol
//https://www.aimtechnologies.com/support/racingecu/AiM_CAN_101_eng.pdf

struct Aim5f0 {
	scaled_channel<uint16_t, 1> Rpm;
	scaled_channel<uint16_t, 650> Tps;
	scaled_channel<uint16_t, 650> Pps;
	scaled_channel<uint16_t, 100> Vss;
};

void populateFrame(Aim5f0& msg) {
	msg.Rpm = Sensor::getOrZero(SensorType::Rpm);
	msg.Tps = Sensor::getOrZero(SensorType::Tps1);
	msg.Pps = Sensor::getOrZero(SensorType::AcceleratorPedal);
	msg.Vss = Sensor::getOrZero(SensorType::VehicleSpeed);
}

struct Aim5f1 {
	scaled_channel<uint16_t, 10> WheelSpeedFR;
	scaled_channel<uint16_t, 10> WheelSpeedFL;
	scaled_channel<uint16_t, 10> WheelSpeedRR;
	scaled_channel<uint16_t, 10> WheelSpeedRL;
};

void populateFrame(Aim5f1& msg) {
	// We don't handle wheel speed, just set to 0?
	msg.WheelSpeedFR = 0;
	msg.WheelSpeedFL = 0;
	msg.WheelSpeedRR = 0;
	msg.WheelSpeedRL = 0;
}

struct Aim5f2 {
	scaled_channel<uint16_t, 190> Iat;
	scaled_channel<uint16_t, 190> Ect;
	scaled_channel<uint16_t, 190> FuelT;
	scaled_channel<uint16_t, 190> OilT;
};

void populateFrame(Aim5f2& msg) {
	msg.Iat = Sensor::getOrZero(SensorType::Iat) + 45;
	msg.Ect = Sensor::getOrZero(SensorType::Clt) + 45;
	msg.FuelT = Sensor::getOrZero(SensorType::FuelTemperature) + 45;
	msg.OilT = Sensor::getOrZero(SensorType::OilTemperature) + 45;
}

struct Aim5f3 {
	scaled_channel<uint16_t, 10> Map;
	scaled_channel<uint16_t, 10> Baro;
	scaled_channel<uint16_t, 1000> OilP;
	scaled_channel<uint16_t, 20> FuelP;
};

void populateFrame(Aim5f3& msg) {
	// MAP/Baro are sent in millibar -> 10 millibar per kpa
	msg.Map = 10 * Sensor::getOrZero(SensorType::Map);
	msg.Baro = 10 * Sensor::getOrZero(SensorType::BarometricPressure);

	// Oil/Fuel P use bar -> 100 kpa per bar
	msg.OilP = Sensor::getOrZero(SensorType::OilPressure) / 100;
	msg.FuelP = Sensor::getOrZero(SensorType::FuelPressureInjector) / 100;
}

struct Aim5f4 {
	scaled_channel<uint16_t, 10000> Boost;
	scaled_channel<uint16_t, 3200> Vbat;
	scaled_channel<uint16_t, 10> FuelConsumptionLH;
	scaled_channel<int16_t, 1> Gear;
};

void populateFrame(Aim5f4& msg) {
	float deltaKpa = Sensor::getOrZero(SensorType::Map)
		- Sensor::get(SensorType::BarometricPressure).value_or(STD_ATMOSPHERE);
	float boostBar = deltaKpa / 100;

#ifdef MODULE_ODOMETER
	float gPerSecond = engine->module<TripOdometer>()->getConsumptionGramPerSecond();
#else
	float gPerSecond = 0;
#endif // MODULE_ODOMETER

	float gPerHour = gPerSecond * 3600.0f;
	float literPerHour = gPerHour * 0.00139f;

	msg.Boost = boostBar;
	msg.Vbat = Sensor::getOrZero(SensorType::BatteryVoltage);
	msg.FuelConsumptionLH = 10 * literPerHour;
	msg.Gear = Sensor::getOrZero(SensorType::DetectedGear);
}

struct Aim5f5 {
	scaled_channel<uint16_t, 1> ShiftFlag;
	scaled_channel<uint16_t, 1> GearTime;
	scaled_channel<uint16_t, 1> TpsV;
	scaled_channel<uint16_t, 100> FuelLevel;
};

void populateFrame(Aim5f5& msg) {
	msg.FuelLevel = Sensor::getOrZero(SensorType::FuelLevel);

	// Dunno what to do with these
	msg.ShiftFlag = 0;
	msg.GearTime = 0;
	msg.TpsV = 0;
}

struct Aim5f6 {
	scaled_channel<uint16_t, 2000> Lambda1;
	scaled_channel<uint16_t, 2000> Lambda2;
	scaled_channel<uint16_t, 10> LambdaTemp1;
	scaled_channel<uint16_t, 10> LambdaTemp2;
};

void populateFrame(Aim5f6& msg) {
	msg.Lambda1 = Sensor::getOrZero(SensorType::Lambda1);
	msg.Lambda2 = Sensor::getOrZero(SensorType::Lambda2);
	msg.LambdaTemp1 = 0;
	msg.LambdaTemp2 = 0;
}

struct Aim5f7 {
	scaled_channel<uint16_t, 10> LambdaErr1;
	scaled_channel<uint16_t, 10> LambdaErr2;
	scaled_channel<uint16_t, 2000> LambdaTarget1;
	scaled_channel<uint16_t, 2000> LambdaTarget2;
};

void populateFrame(Aim5f7& msg) {
#if EFI_ENGINE_CONTROL
	// We don't handle wheel speed, just set to 0?
	msg.LambdaErr1 = 0;
	msg.LambdaErr2 = 0;
	// both targets are the same for now
	msg.LambdaTarget1 = (float)engine->fuelComputer.targetLambda;
	msg.LambdaTarget2 = (float)engine->fuelComputer.targetLambda;
#endif // EFI_ENGINE_CONTROL
}

void canDashboardAim(CanCycle cycle) {
	if (!cycle.isInterval(CI::_10ms)) {
		return;
	}

	auto canChannel = engineConfiguration->canBroadcastUseChannelTwo;

	transmitStruct<Aim5f0>(CanCategory::NBC, 0x5f0, false, canChannel);
	transmitStruct<Aim5f1>(CanCategory::NBC, 0x5f1, false, canChannel);
	transmitStruct<Aim5f2>(CanCategory::NBC, 0x5f2, false, canChannel);
	transmitStruct<Aim5f3>(CanCategory::NBC, 0x5f3, false, canChannel);
	transmitStruct<Aim5f4>(CanCategory::NBC, 0x5f4, false, canChannel);
	transmitStruct<Aim5f5>(CanCategory::NBC, 0x5f5, false, canChannel);
	transmitStruct<Aim5f6>(CanCategory::NBC, 0x5f6, false, canChannel);
	transmitStruct<Aim5f7>(CanCategory::NBC, 0x5f7, false, canChannel);

	// there are more, but less important for us
	// transmitStruct<Aim5f8>(0x5f8, false);
	// transmitStruct<Aim5f9>(0x5f9, false);
	// transmitStruct<Aim5fa>(0x5fa, false);
	// transmitStruct<Aim5fb>(0x5fb, false);
	// transmitStruct<Aim5fc>(0x5fc, false);
	// transmitStruct<Aim5fd>(0x5fd, false);
}

std::optional<board_can_update_dash_type> custom_board_update_dash;

PUBLIC_API_WEAK void boardUpdateDash(CanCycle cycle) { UNUSED(cycle); }

void updateDash(CanCycle cycle) {
	// TODO: use call_board_override
	if (custom_board_update_dash.has_value()) {
		custom_board_update_dash.value()(cycle);
	}

	boardUpdateDash(cycle);

	// Transmit dash data, if enabled
	switch (engineConfiguration->canNbcType) {
	case CAN_BUS_NBC_NONE:
		break;
	case CAN_BUS_BMW_E46:
		canDashboardBmwE46(cycle);
		break;
	case CAN_BUS_Haltech:
		canDashboardHaltech(cycle);
		break;
	case CAN_BUS_NBC_FIAT:
		canDashboardFiat(cycle);
		break;
	case CAN_BUS_NBC_VAG:
		canDashboardVAG(cycle);
		break;
	case CAN_BUS_MAZDA_RX8:
		canMazdaRX8(cycle);
		break;
	case CAN_BUS_W202_C180:
		canDashboardW202(cycle);
		break;
	case CAN_BUS_BMW_E90:
		canDashboardBmwE90(cycle);
		break;
	case CAN_BUS_MQB:
		canDashboardVagMqb(cycle);
		break;
	case CAN_BUS_NISSAN_VQ:
		canDashboardNissanVQ(cycle);
		break;
	case CAN_BUS_GENESIS_COUPE:
		canDashboardGenesisCoupe(cycle);
		break;
    case CAN_BUS_HONDA_K:
		canDashboardHondaK(cycle);
		break;
	case CAN_AIM_DASH:
		canDashboardAim(cycle);
		break;
	case CAN_BUS_MS_SIMPLE_BROADCAST:
		canDashboardTS(cycle);
		break;
	default:
		criticalError("Nothing for canNbcType %d/%s", engineConfiguration->canNbcType, getCan_nbc_e(engineConfiguration->canNbcType));
		break;
	}
}

#endif // EFI_CAN_SUPPORT
