/**
 * @file	can_dash.cpp
 *
 * This file handles transmission of ECU data to various OE dashboards.
 *
 * @date Mar 19, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#include "globalaccess.h"
#if EFI_CAN_SUPPORT

#include "can_dash.h"
#include "can_msg_tx.h"

#include "allsensors.h"
#include "vehicle_speed.h"
#include "rtc_helper.h"
#include "fuel_math.h"
// CAN Bus ID for broadcast
/**
 * e46 data is from http://forums.bimmerforums.com/forum/showthread.php?1887229
 *
 * Same for Mini Cooper? http://vehicle-reverse-engineering.wikia.com/wiki/MINI
 *
 * All the below packets are using 500kb/s
 *
 * for verbose use "set debug_mode 26" command in console
 *
 */
#define CAN_BMW_E46_SPEED             0x153
#define CAN_BMW_E46_RPM               0x316
#define CAN_BMW_E46_DME2              0x329
#define CAN_BMW_E46_CLUSTER_STATUS    0x613
#define CAN_BMW_E46_CLUSTER_STATUS_2  0x615
#define CAN_FIAT_MOTOR_INFO           0x561
#define CAN_MAZDA_RX_RPM_SPEED        0x201
#define CAN_MAZDA_RX_STEERING_WARNING 0x300
#define CAN_MAZDA_RX_STATUS_1         0x212
#define CAN_MAZDA_RX_STATUS_2         0x420

// https://wiki.openstreetmap.org/wiki/VW-CAN
#define CAN_VAG_RPM      0x280 /* _10ms cycle */
#define CAN_VAG_CLT      0x288 /* _10ms cycle */
#define CAN_VAG_CLT_V2   0x420 /* _10ms cycle */
#define CAN_VAG_IMMO     0x3D0 /* _10ms cycle */

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
#define E90_TEMP             0x1D0
#define E90_GEAR             0x1D2
#define E90_FUEL             0x349
#define E90_EBRAKE           0x34F
#define E90_TIME             0x39E

static time_msecs_t mph_timer;
static time_msecs_t mph_ctr;

#define NISSAN_RPM_1F9 0x1F9
// Nissan z33 350Z and else
// 0x23d = 573
#define NISSAN_RPM_CLT       0x23D

#define NISSAN_VEHICLE_SPEED_280 0x280
// wheel speed see "102 CAN Communication decoded"
// 19500 value would be 100 kph
#define NISSAN_WHEEL_SPEED 0x285

#define NISSAN_CLT_551 0x551

static uint8_t rpmcounter;
static uint8_t seatbeltcnt;
static uint8_t abscounter = 0xF0;
static uint8_t brakecnt_1 = 0xF0, brakecnt_2 = 0xF0;
static uint8_t mph_a, mph_2a, mph_last, tmp_cnt, gear_cnt;
static uint16_t mph_counter = 0xF000;
static bool cluster_time_set;

constexpr uint8_t e90_temp_offset = 49;

void canDashboardBMW(CanCycle cycle);
void canDashboardFiat(CanCycle cycle);
void canDashboardVAG(CanCycle cycle);
void canMazdaRX8(CanCycle cycle);
void canDashboardW202(CanCycle cycle);
void canDashboardBMWE90(CanCycle cycle);
void canDashboardVagMqb(CanCycle cycle);
void canDashboardNissanVQ(CanCycle cycle);

void updateDash(CanCycle cycle) {

	// Transmit dash data, if enabled
	switch (CONFIG(canNbcType)) {
	case CAN_BUS_NBC_BMW:
		canDashboardBMW(cycle);
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
		canDashboardBMWE90(cycle);
		break;
	case CAN_BUS_MQB:
		canDashboardVagMqb(cycle);
		break;
	case CAN_BUS_NISSAN_VQ:
		canDashboardNissanVQ(cycle);
		break;
	default:
		break;
	}
}

//BMW Dashboard
//todo: we use 50ms fixed cycle, trace is needed to check for correct period
void canDashboardBMW(CanCycle cycle) {
	
	if (cycle.isInterval(CI::_50ms)) {
		{
			CanTxMessage msg(CAN_BMW_E46_SPEED);
			msg.setShortValue(10 * 8, 1);
		}

		{
			CanTxMessage msg(CAN_BMW_E46_RPM);
			msg.setShortValue((int) (GET_RPM() * 6.4), 2);
		}

		{
			CanTxMessage msg(CAN_BMW_E46_DME2);
			msg.setShortValue((int) ((Sensor::get(SensorType::Clt).value_or(0) + 48.373) / 0.75), 1);
		}
	}
}

//todo: we use 50ms fixed cycle, trace is needed to check for correct period
void canMazdaRX8(CanCycle cycle) {
	if (cycle.isInterval(CI::_50ms)) {
		{
			CanTxMessage msg(CAN_MAZDA_RX_STEERING_WARNING);
			// todo: something needs to be set here? see http://rusefi.com/wiki/index.php?title=Vehicle:Mazda_Rx8_2004
		}

		{
			CanTxMessage msg(CAN_MAZDA_RX_RPM_SPEED);

			float kph = getVehicleSpeed();

			msg.setShortValue(SWAP_UINT16(GET_RPM() * 4), 0);
			msg.setShortValue(0xFFFF, 2);
			msg.setShortValue(SWAP_UINT16((int )(100 * kph + 10000)), 4);
			msg.setShortValue(0, 6);
		}

		{
			CanTxMessage msg(CAN_MAZDA_RX_STATUS_1);
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
			CanTxMessage msg(CAN_MAZDA_RX_STATUS_2);
			auto clt = Sensor::get(SensorType::Clt);
			msg[0] = (uint8_t)(clt.value_or(0) + 69); //temp gauge //~170 is red, ~165 last bar, 152 centre, 90 first bar, 92 second bar
			msg[1] = ((int16_t)(engine->engineState.vssEventCounter*(engineConfiguration->vehicleSpeedCoef*0.277*2.58))) & 0xff;
			msg[2] = 0x00; // unknown
			msg[3] = 0x00; //unknown
			msg[4] = 0x01; //Oil Pressure (not really a gauge)
			msg[5] = 0x00; //check engine light
			msg[6] = 0x00; //Coolant, oil and battery
			if ((GET_RPM()>0) && (Sensor::get(SensorType::BatteryVoltage).value_or(VBAT_FALLBACK_VALUE)<13)) {
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
			CanTxMessage msg(CAN_FIAT_MOTOR_INFO);
			msg.setShortValue((int) (Sensor::get(SensorType::Clt).value_or(0) - 40), 3); //Coolant Temp
			msg.setShortValue(GET_RPM() / 32, 6); //RPM
		}
	}
}

void canDashboardVAG(CanCycle cycle) {
	if (cycle.isInterval(CI::_10ms)) {
		{
			//VAG Dashboard
			CanTxMessage msg(CAN_VAG_RPM);
			msg.setShortValue(GET_RPM() * 4, 2); //RPM
		}

		float clt = Sensor::get(SensorType::Clt).value_or(0);

		{
			CanTxMessage msg(CAN_VAG_CLT);
			msg.setShortValue((int) ((clt + 48.373) / 0.75), 1); //Coolant Temp
		}

		{
			CanTxMessage msg(CAN_VAG_CLT_V2);
			msg.setShortValue((int) ((clt + 48.373) / 0.75), 4); //Coolant Temp
		}

		{
			CanTxMessage msg(CAN_VAG_IMMO);
			msg.setShortValue(0x80, 1);
		}
	}
}

void canDashboardW202(CanCycle cycle) {
	if (cycle.isInterval(CI::_20ms)) {
		{
			CanTxMessage msg(W202_STAT_1);
			uint16_t tmp = GET_RPM();
			msg[0] = 0x08; // Unknown
			msg[1] = (tmp >> 8); //RPM
			msg[2] = (tmp & 0xff); //RPM
			msg[3] = 0x00; // 0x01 - tank blink, 0x02 - EPC
			msg[4] = 0x00; // Unknown
			msg[5] = 0x00; // Unknown
			msg[6] = 0x00; // Unknown - oil info
			msg[7] = 0x00; // Unknown - oil info
		}
	}

	if (cycle.isInterval(CI::_100ms)) {
		{
			CanTxMessage msg(W202_STAT_2); //dlc 7
			msg[0] = (int)(Sensor::get(SensorType::Clt).value_or(0) + 40); // CLT -40 offset
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
			CanTxMessage msg(W202_ALIVE);
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
			CanTxMessage msg(W202_STAT_3);
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

static int rollingId = 0;

void canDashboardNissanVQ(CanCycle cycle) {
	if (cycle.isInterval(CI::_50ms)) {
		{
			CanTxMessage msg(NISSAN_RPM_1F9, 8);
			msg[0] = 0x20;
			int rpm8 = (int)(GET_RPM() * 8);
			msg[2] = rpm8 >> 8;
			msg[3] = rpm8 & 0xFF;
		}

		{
			CanTxMessage msg(NISSAN_CLT_551, 8);

			int clt = 40; // todo read sensor
			msg[0] = clt + 45;
		}


		{
			CanTxMessage msg(NISSAN_RPM_CLT, 8);

			rollingId = (rollingId + 1) % 4;
			const uint8_t magicByte[4] = {0x03, 0x23, 0x42, 0x63};

			msg[0] = magicByte[rollingId];
			msg[1] = (int)(Sensor::get(SensorType::AcceleratorPedal).value_or(0) * 255 / 100);

			// thank you "102 CAN Communication decoded"
#define CAN_23D_RPM_MULT 3.15
			int rpm315 = (int)(GET_RPM() / CAN_23D_RPM_MULT);
			msg[3] = rpm315 & 0xFF;
			msg[4] = rpm315 >> 8;

			msg[7] = 0x70; // todo: CLT decoding?
		}
	}
}

/**
 * https://docs.google.com/spreadsheets/d/1XMfeGlhgl0lBL54lNtPdmmFd8gLr2T_YTriokb30kJg
 */
void canDashboardVagMqb(CanCycle cycle) {
	if (cycle.isInterval(CI::_50ms)) {

		{ // 'turn-on'
			CanTxMessage msg(0x3C0, 4);
			// ignition ON
			msg[2] = 3;
		}
	
		{ //RPM
			CanTxMessage msg(0x107, 8);
			msg[3] = ((int)(GET_RPM() / 3.5)) & 0xFF;
			msg[4] = ((int)(GET_RPM() / 3.5)) >> 8;
		}
	}
}

void canDashboardBMWE90(CanCycle cycle)
{

	if (cycle.isInterval(CI::_50ms)) {
		
		{ //T15 'turn-on'
			CanTxMessage msg(E90_T15, 5);
			msg[0] = 0x45;
			msg[1] = 0x41;
			msg[2] = 0x61;
			msg[3] = 0x8F;
			msg[4] = 0xFC;			
		}

		{ //Ebrake light
			CanTxMessage msg(E90_EBRAKE, 2);
			msg[0] = 0xFD;
			msg[1] = 0xFF;
		}

		{ //RPM
			rpmcounter++;
			if (rpmcounter > 0xFE)
				rpmcounter = 0xF0;
			CanTxMessage msg(E90_RPM, 3);
			msg[0] = rpmcounter;
			msg[1] = (GET_RPM() * 4) & 0xFF;
			msg[2] = (GET_RPM() * 4) >> 8;
		}

		{ //oil & coolant temp (all in C, despite gauge being F)
			tmp_cnt++;
			if (tmp_cnt >= 0x0F)
				tmp_cnt = 0x00;
			CanTxMessage msg(E90_TEMP, 8);
			msg[0] = (int)(Sensor::get(SensorType::Clt).value_or(0) + e90_temp_offset); //coolant
			msg[1] = (int)(Sensor::get(SensorType::AuxTemp1).value_or(0) + e90_temp_offset); //oil (AuxTemp1)
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
			CanTxMessage msg(E90_SEATBELT_COUNTER, 2);
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
			CanTxMessage msg(E90_BRAKE_COUNTER, 8);
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
			CanTxMessage msg(E90_ABS_COUNTER, 2);
			msg[0] = abscounter;
			msg[1] = 0xFF;
		}

		{ //Fuel gauge
			CanTxMessage msg(E90_FUEL, 5); //fuel gauge
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
			CanTxMessage msg(E90_GEAR, 6);
			msg[0] = 0x78;
			msg[1] = 0x0F;
			msg[2] = 0xFF;
			msg[3] = (gear_cnt << 4) | 0xC;
			msg[4] = 0xF1;
			msg[5] = 0xFF;
		}

		{ //E90_SPEED
			float mph = getVehicleSpeed() * 0.6213712;
			mph_ctr = ((TIME_I2MS(chVTGetSystemTime()) - mph_timer) / 50);
			mph_a = (mph_ctr * mph / 2);
			mph_2a = mph_a + mph_last;
			mph_last = mph_2a;
			mph_counter += mph_ctr * 100;
			if(mph_counter >= 0xFFF0)
				mph_counter = 0xF000;
			mph_timer = TIME_I2MS(chVTGetSystemTime());
			CanTxMessage msg(E90_SPEED, 8);
			msg[0] = mph_2a & 0xFF;
			msg[1] = mph_2a >> 8;
			msg[2] = mph_2a & 0xFF;
			msg[3] = mph_2a >> 8;
			msg[4] = mph_2a & 0xFF;
			msg[5] = mph_2a >> 8;
			msg[6] = mph_counter & 0xFF;
			msg[7] = (mph_counter >> 8) | 0xF0;
		}
	}

	{
		if (!cluster_time_set) {
			struct tm timp;
			date_get_tm(&timp);
			CanTxMessage msg(E90_TIME, 8);
			msg[0] = timp.tm_hour;
			msg[1] = timp.tm_min;
			msg[2] = timp.tm_sec;
			msg[3] = timp.tm_mday;
			msg[4] = (((timp.tm_mon + 1) << 4) | 0x0F);
			msg[5] = (timp.tm_year + 1900) & 0xFF;
			msg[6] = ((timp.tm_year + 1900) >> 8) | 0xF0;
			msg[7] = 0xF2;
			cluster_time_set = 1;
		}
	}
}

void canDashboardHaltech(CanCycle cycle) {
	
	uint16_t tmp;

	if (cycle.isInterval(CI::_20ms)) {
		/* 0x360 - 50Hz rate */
		{
			CanTxMessage msg(0x360, 8);
			tmp = GET_RPM();
			/* RPM */
			msg[0] = (tmp >> 8);
			msg[1] = (tmp & 0x00ff);
			/* MAP */
			tmp = (((uint16_t)(Sensor::get(SensorType::Map).value_or(0))) * 10); 
			msg[2] = (tmp >> 8);
			msg[3] = (tmp & 0x00ff);
			/* TPS  y = x/10 */
			tmp = (uint16_t)((float)(Sensor::get(SensorType::Tps1).value_or(0)) * 10);
			msg[4] = (tmp >> 8);
			msg[5] = (tmp & 0x00ff);
			/* Coolant pressure */
			msg[6] = 0;
			msg[7] = 0;
		}

		/* 0x361 - 50Hz rate */
		{ 
			CanTxMessage msg(0x361, 8);
			/* Fuel pressure */
			tmp =  (uint16_t)(Sensor::get(SensorType::FuelPressureLow).value_or(0));
			msg[0] = (tmp >> 8);
			msg[1] = (tmp&0x00ff);
			/* Oil pressure */
			tmp =  (uint16_t)(Sensor::get(SensorType::OilPressure).value_or(0));
			msg[2] = (tmp >> 8);
			msg[3] = (tmp & 0x00ff);
			/* Engine Demand */
			tmp =  (uint16_t)(Sensor::get(SensorType::Map).value_or(0));
			msg[4] = (tmp >> 8);
			msg[5] = (tmp & 0x00ff);
			/* Wastegate Pressure */
			msg[6] = 0;
			msg[7] = 0;			
		}

		/* 0x362 - 50Hz rate */
		{ 
			CanTxMessage msg(0x362, 6);
			/* Injection Stage 1 Duty Cycle - y = x/10 */
			uint16_t rpm = GET_RPM();
			tmp = (uint16_t)( getInjectorDutyCycle(rpm PASS_ENGINE_PARAMETER_SUFFIX) * 10) ;
			msg[0] = (tmp >> 8);
			msg[1] = (tmp & 0x00ff);
			/* Injcetion Stage 2 Duty Cycle */
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* Ignition Angle (Leading) - y = x/10 */
			float timing = engine->engineState.timingAdvance;
			int16_t ignAngle = ((timing > 360 ? timing - 720 : timing) * 10);
			msg[4] = (ignAngle >> 8);			
			msg[5] = (ignAngle & 0x00ff);
		}

		/* todo: 0x3E5 = 50Hz rate */
		{ 
			CanTxMessage msg(0x3E5, 8);
			msg[0] = 0x00; 
			msg[1] = 0x00;
			msg[2] = 0x00;
			msg[3] = 0x00;
			msg[4] = 0x00;
			msg[5] = 0x00;
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* todo: 0x3EA = 50Hz rate */
		{ 
			CanTxMessage msg(0x3EA, 8);
			msg[0] = 0x00; 
			msg[1] = 0x00;
			msg[2] = 0x00;
			msg[3] = 0x00;
			msg[4] = 0x00;
			msg[5] = 0x00;
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* todo: 0x3EB = 50Hz rate */
		{ 
			CanTxMessage msg(0x3EB, 8);
			msg[0] = 0x00; 
			msg[1] = 0x00;
			msg[2] = 0x00;
			msg[3] = 0x00;
			msg[4] = 0x00;
			msg[5] = 0x00;
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* todo: 0x3EC = 50Hz rate */
		{ 
			CanTxMessage msg(0x3EC, 8);
			msg[0] = 0x00; 
			msg[1] = 0x00;
			msg[2] = 0x00;
			msg[3] = 0x00;
			msg[4] = 0x00;
			msg[5] = 0x00;
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* todo: 0x3ED = 50Hz rate */
		{ 
			CanTxMessage msg(0x3ED, 2);
			msg[0] = 0x00; 
			msg[1] = 0x00;
		}

		/* todo: 0x471 = 50Hz rate */
		{ 
			CanTxMessage msg(0x471, 2);
			msg[0] = 0x00; 
			msg[1] = 0x00;
			msg[2] = 0x00;
			msg[3] = 0x00;
		}
	}

	if (cycle.isInterval(CI::_50ms)) {
	
		/* 0x363 - 20Hz rate */
		{ 
			CanTxMessage msg(0x363, 4);
			/* Wheel Slip */
			msg[0] = 0x00;
			msg[1] = 0x00;
			/* Wheel Diff */
			msg[2] = 0x00;
			msg[3] = 0x00 ;
		}

		/* 0x368 - 20Hz rate */
		{ 
			CanTxMessage msg(0x368, 8);
			/* Wideband Sensor 1 */
			tmp =  (uint16_t)(Sensor::get(SensorType::Lambda1).value_or(0)) * 1000;
			msg[0] = (tmp >> 8);
			msg[1] = (tmp & 0x00ff);
			/* Wideband Sensor 2 */
			tmp =  (uint16_t)(Sensor::get(SensorType::Lambda2).value_or(0) * 1000);
			msg[2] = (tmp >> 8);
			msg[3] = (tmp & 0x00ff);
			/* Wideband Sensor 3 */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* Wideband Sensor 4 */			
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* 0x369 - 20Hz rate */
		{ 
			CanTxMessage msg(0x369, 8);
			/* Trigger System Error Count */
			tmp = engine->triggerCentral.triggerState.totalTriggerErrorCounter;
			msg[0] = (tmp >> 8);
			msg[1] = (tmp & 0x00ff);
			/* Trigger Counter ?? */
			tmp =  engine->triggerCentral.getHwEventCounter((int)SHAFT_PRIMARY_FALLING);
			msg[2] = (tmp >> 8);
			msg[3] = (tmp & 0x00ff);
			/* unused */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* Trigger Sync Level ?? */
			msg[6] = 0x00;			
			msg[7] = 0x00;
		}

		/* 0x36A - 20Hz rate */
		/* todo: one day we should split this */
		{ 
			CanTxMessage msg(0x36A, 4);
			/* Knock Level 1 */
			tmp = (tsOutputChannels.knockLevel * 100);
			msg[0] = (tmp >> 8);
			msg[1] = (tmp & 0x00ff);
			/* Knock Level 2 */
			msg[2] = (tmp >> 8);
			msg[3] = (tmp * 0x00ff);
		}

		/* 0x36B - 20Hz rate */
		{ 
			CanTxMessage msg(0x36B, 8);
			/* Break Pressure */
			msg[0] = 0x00;
			msg[1] = 0x00;
			/* NOS pressure Sensor 1 */
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* Turbo Speed Sensor 1 */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* Lateral G */
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* 0x36C = 20Hz rate */
		{ 
			CanTxMessage msg(0x36C, 8);
			/* Wheel Speed Front Left */
			tmp = (getVehicleSpeed() * 10 );
			msg[0] = (tmp >> 8);
			msg[1] = (tmp & 0x00ff);
			/* Wheel Speed Front Right */
			msg[2] = (tmp >> 8);
			msg[3] = (tmp & 0x00ff);
			/* Wheel Speed Read Left */
			msg[4] = (tmp >> 8);
			msg[5] = (tmp & 0x00ff);
			/* Wheel Speed Read Right */
			msg[6] = (tmp >> 8);
			msg[7] = (tmp & 0x00ff);
		}

		/* 0x36D = 20Hz rate */
		{ 
			CanTxMessage msg(0x36D, 8);
			/* Unused */
			msg[0] = 0x00;
			msg[1] = 0x00;
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* Exhaust Cam Angle 1 */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* Exhaust Cam Angle 2 */
			msg[6] = 0x00;
			msg[7] = 0x00;
		}	

		/* 0x36E = 20Hz rate */
		{ 
			CanTxMessage msg(0x36E, 8);
			/* Engine Limiting Active 0 = off/1=on*/
			msg[0] = 0x00;
			msg[1] = 0x00;
			/* Launch Control Ignition Retard */
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* Launch Control Fuel Enrich */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* Longitudinal G */
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* 0x36F = 20Hz rate */
		{ 
			CanTxMessage msg(0x36F, 4);
			/* Generic Output 1 Duty Cycle */
			msg[0] = 0x00;
			msg[1] = 0x00;
			/* Boost Control Output */
			msg[2] = 0x00;
			msg[3] = 0x00;
		}

		/* 0x370 = 20Hz rate */
		{ 
			CanTxMessage msg(0x370, 8);
			/* Vehicle Speed */
			tmp = (getVehicleSpeed() * 10 );
			msg[0] = (tmp >> 8);
			msg[1] = (tmp & 0x00ff);
			/* unused */
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* Intake Cam Angle 1 */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* Intake Cam Angle 2 */
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* todo: 0x3E6 = 20Hz rate */
		{ 
			CanTxMessage msg(0x3E6, 8);
			msg[0] = 0x00; 
			msg[1] = 0x00;
			msg[2] = 0x00;
			msg[3] = 0x00;
			msg[4] = 0x00;
			msg[5] = 0x00;
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* todo: 0x3E7 = 20Hz rate */
		{ 
			CanTxMessage msg(0x3E7, 8);
			msg[0] = 0x00; 
			msg[1] = 0x00;
			msg[2] = 0x00;
			msg[3] = 0x00;
			msg[4] = 0x00;
			msg[5] = 0x00;
			msg[6] = 0x00;
			msg[7] = 0x00;
		}
	
		/* todo: 0x3E8 = 20Hz rate */
		{ 
			CanTxMessage msg(0x3E8, 8);
			msg[0] = 0x00; 
			msg[1] = 0x00;
			msg[2] = 0x00;
			msg[3] = 0x00;
			msg[4] = 0x00;
			msg[5] = 0x00;
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* todo: 0x3E9 = 20Hz rate */
		{ 
			CanTxMessage msg(0x3E9, 8);
			msg[0] = 0x00; 
			msg[1] = 0x00;
			msg[2] = 0x00;
			msg[3] = 0x00;
			msg[4] = 0x00;
			msg[5] = 0x00;
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* todo: 0x3EE = 20Hz rate */
		{ 
			CanTxMessage msg(0x3EE, 8);
			msg[0] = 0x00; 
			msg[1] = 0x00;
			msg[2] = 0x00;
			msg[3] = 0x00;
			msg[4] = 0x00;
			msg[5] = 0x00;
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* todo: 0x3EF = 20Hz rate */
		{ 
			CanTxMessage msg(0x3EF, 8);
			msg[0] = 0x00; 
			msg[1] = 0x00;
			msg[2] = 0x00;
			msg[3] = 0x00;
			msg[4] = 0x00;
			msg[5] = 0x00;
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* todo: 0x470 = 20Hz rate */
		{ 
			CanTxMessage msg(0x470, 8);
			msg[0] = 0x00; 
			msg[1] = 0x00;
			msg[2] = 0x00;
			msg[3] = 0x00;
			msg[4] = 0x00;
			msg[5] = 0x00;
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* todo: 0x472 = 20Hz rate */
		{ 
			CanTxMessage msg(0x472, 8);
			msg[0] = 0x00; 
			msg[1] = 0x00;
			msg[2] = 0x00;
			msg[3] = 0x00;
			msg[4] = 0x00;
			msg[5] = 0x00;
			msg[6] = 0x00;
			msg[7] = 0x00;
		}		
	}

	if (cycle.isInterval(CI::_100ms)) {
		
		/* 0x371 = 10Hz rate */
		{ 
			CanTxMessage msg(0x371, 4);
			/* Fuel Flow */
			msg[0] = 0x00;
			msg[1] = 0x00;
			/* Fuel Flow Return */
			msg[2] = 0x00;
			msg[3] = 0x00;
		}

		/* 0x372 = 10Hz rate */
		{ 
			CanTxMessage msg(0x372, 8);
			/* Battery Voltage */
			tmp =  (uint16_t)(Sensor::get(SensorType::BatteryVoltage).value_or(0) * 10);
			msg[0] = (tmp >> 8);
			msg[1] = (tmp & 0x00ff);
			/* unused */
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* Target Boost Level todo */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* Barometric pressure */
			tmp = (uint16_t)(getBaroPressure(PASS_ENGINE_PARAMETER_SIGNATURE)*10);
			msg[6] = (tmp >> 8);
			msg[7] = (tmp & 0x00ff);
		}
	
		/* 0x373 = 10Hz rate */
		{ 
			CanTxMessage msg(0x373, 8);
			/* EGT1 */
			msg[0] = 0x00;
			msg[1] = 0x00;
			/* EGT2 */
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* EGT3 */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* EGT4 */
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* 0x374 = 10Hz rate */
		{ 
			CanTxMessage msg(0x374, 8);
			/* EGT5 */
			msg[0] = 0x00;
			msg[1] = 0x00;
			/* EGT6 */
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* EGT7 */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* EGT8 */
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* 0x375 = 10Hz rate */
		{ 
			CanTxMessage msg(0x375, 8);
			/* EGT9 */
			msg[0] = 0x00;
			msg[1] = 0x00;
			/* EGT10 */
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* EGT11 */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* EGT12 */
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* 0x376 = 10Hz rate */
		{ 
			CanTxMessage msg(0x376, 8);
			/* Ambient Air Temperature */
			msg[0] = 0x00;
			msg[1] = 0x00;
			/* Relative Humidity */
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* Specific Humidity */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* Absolute Humidity */
			msg[6] = 0x00;
			msg[7] = 0x00;
		}
	}

	if (cycle.isInterval(CI::_200ms)) {
		/* 0x3E0 = 5Hz rate */
		{ 
			CanTxMessage msg(0x3E0, 8);
			/* Coolant temperature in K y = x/10 */
			tmp = ((Sensor::get(SensorType::Clt).value_or(0) + 273.15) * 10);
			msg[0] = (tmp >> 8);
			msg[1] = (tmp & 0x00ff);
			/* Air Temperature */
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* Fuel Temperature */
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* Oil Temperature */
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* 0x3E1 = 5Hz rate */
		{ 
			CanTxMessage msg(0x3E1, 6);
			/* Gearbox Oil Temperature */
			msg[0] = 0x00;
			msg[1] = 0x00;
			/* Diff oil Temperature */
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* Fuel Composition */
			msg[4] = 0x00;
			msg[5] = 0x00;
		}

		/* 0x3E2 = 5Hz rate */
		{ 
			CanTxMessage msg(0x3E2, 2);
			/* Fuel Level in Liters */
			msg[0] = 0x00;
			msg[1] = 0xff;
		}

		/* 0x3E3 = 5Hz rate */
		{ 
			CanTxMessage msg(0x3E3, 8);
			/* Fuel Trim Short Term Bank 1*/
			msg[0] = 0x00;
			msg[1] = 0x00;
			/* Fuel Trim Short Term Bank 2*/
			msg[2] = 0x00;
			msg[3] = 0x00;
			/* Fuel Trim Long Term Bank 1*/
			msg[4] = 0x00;
			msg[5] = 0x00;
			/* Fuel Trim Long Term Bank 2*/
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

		/* todo: 0x3E4 = 5Hz rate */
		{ 
			CanTxMessage msg(0x3E4, 8);
			msg[0] = 0x00; //unused
			/* Switch status */
			msg[1] = 0x00;
			/* Switch status */
			msg[2] = 0x00;
			msg[3] = 0x00;
			msg[4] = 0x00;
			msg[5] = 0x00;
			msg[6] = 0x00;
			msg[7] = 0x00;
		}

	}
}

#endif // EFI_CAN_SUPPORT
