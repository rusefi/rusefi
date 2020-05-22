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

#include "engine.h"
#include "can_dash.h"
#include "can_msg_tx.h"

#include "sensor.h"
#include "allsensors.h"
#include "vehicle_speed.h"

EXTERN_ENGINE;

// CAN Bus ID for broadcast
/**
 * e46 data is from http://forums.bimmerforums.com/forum/showthread.php?1887229
 *
 * Same for Mini Cooper? http://vehicle-reverse-engineering.wikia.com/wiki/MINI
 *
 * All the below packets are using 500kb/s
 *
 */
#define CAN_BMW_E46_SPEED 0x153
#define CAN_BMW_E46_RPM 0x316
#define CAN_BMW_E46_DME2 0x329
#define CAN_BMW_E46_CLUSTER_STATUS 0x613
#define CAN_BMW_E46_CLUSTER_STATUS_2 0x615
#define CAN_FIAT_MOTOR_INFO 0x561
#define CAN_MAZDA_RX_RPM_SPEED 0x201
#define CAN_MAZDA_RX_STEERING_WARNING 0x300
#define CAN_MAZDA_RX_STATUS_1 0x212
#define CAN_MAZDA_RX_STATUS_2 0x420
// https://wiki.openstreetmap.org/wiki/VW-CAN
#define CAN_VAG_RPM 0x280
#define CAN_VAG_CLT 0x288
#define CAN_VAG_CLT_V2 0x420
#define CAN_VAG_IMMO 0x3D0
//w202 DASH
#define W202_STAT_1	0x308
#define W202_STAT_2 0x608
#define W202_ALIVE	0x210
#define W202_STAT_3 0x310

//BMW E90 DASH
#define E90_ABS_COUNTER 0x0C0
#define E90_SEATBELT_COUNTER 0x0D7
#define E90_T15	0x130
#define E90_RPM 0x175
#define E90_BRAKE_COUNTER 0x19E
#define E90_SPEED 0x1A6
#define E90_TEMP 0x1D0
#define E90_GEAR 0x1D2
#define E90_FUEL 0x349
#define E90_EBRAKE 0x34F
#define E90_LIGHTS 0x21A

static uint8_t rpmcounter;
static uint16_t msgcounter;
static uint8_t counter0d7;
static uint8_t abscounter = 0xF0;
static uint8_t airbagcounter;
static uint8_t brakecnt_1 = 0xF0, brakecnt_2 = 0xF0;

static uint8_t mph_a, mph_2a, mph_last, tmp_cnt, gear_cnt;
static uint16_t mph_counter = 0xF000;
static time_msecs_t mph_timer;
static time_msecs_t mph_ctr;

void canDashboardBMW(void) {
	//BMW Dashboard
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

void canMazdaRX8(void) {
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
		if ((GET_RPM()>0) && (engine->sensors.vBatt<13)) {
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

void canDashboardFiat(void) {
	{
		//Fiat Dashboard
		CanTxMessage msg(CAN_FIAT_MOTOR_INFO);
		msg.setShortValue((int) (Sensor::get(SensorType::Clt).value_or(0) - 40), 3); //Coolant Temp
		msg.setShortValue(GET_RPM() / 32, 6); //RPM
	}
}

void canDashboardVAG(void) {
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

void canDashboardW202(void) {
	
	{
		CanTxMessage msg(W202_STAT_1);
		msg[0] = 0x08; // Unknown
		msg.setShortValue(GET_RPM(), 1); //RPM
		msg[3] = 0x00; // 0x01 - tank blink, 0x02 - EPC
		msg[4] = 0x00; // Unknown
		msg[5] = 0x00; // Unknown
		msg[6] = 0x00; // Unknown - oil info
		msg[7] = 0x00; // Unknown - oil info
	}

	{
		CanTxMessage msg(W202_STAT_2); //dlc 7
		msg[0] = (int)(Sensor::get(SensorType::Clt).value_or(0) + 40); // CLT - 0x80 ~ 80C
		msg[1] = 0x3D; // TBD
		msg[2] = 0x63; // Const
		msg[3] = 0x41; // Const
		msg[4] = 0x00; // Unknown
		msg[5] = 0x05; // Const
		msg[6] = 0x50; // TBD
		msg[7] = 0x00; // Unknown
	}

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

void canDashboardBMWE90()
{
	// palTogglePad(GPIOG, 13);

	uint16_t temprpm;
	msgcounter++;

	{ //E90_T15
		CanTxMessage msg(E90_T15, 5);
		msg[0] = 0x45;
		msg[1] = 0x41;
		msg[2] = 0x61;
		msg[3] = 0x8F;
		msg[4] = 0xFC;			
	}
	{
		CanTxMessage msg(E90_EBRAKE, 2);
		msg[0] = 0xFD;
		msg[1] = 0xFF;
	}

	{ //E90_RPM
		rpmcounter++;
		if (rpmcounter > 0xFE)
			rpmcounter = 0xF0;

		temprpm = GET_RPM() * 4;

		CanTxMessage msg(E90_RPM, 3);
		msg[0] = rpmcounter;
		msg[1] = temprpm & 0xFF;
		msg[2] = temprpm >> 8;
	}

	
	{ //E90_TEMP
		// if (msgcounter % 2) {
			tmp_cnt++;
			if (tmp_cnt >= 0x0F)
				tmp_cnt = 0x00;
			CanTxMessage msg(E90_TEMP, 8);
			int temp = 90;
			msg[0] = 0x59; //coolant
			msg[1] = 0xAA; //olio
			msg[2] = tmp_cnt;
			msg[3] = 0xC8;
			msg[4] = 0xA7;
			msg[5] = 0xD3;
			msg[6] = 0x0D;
			msg[7] = 0xA8;
		// }
	}

	{ //E90_SEATBELT_COUNTER
		if (msgcounter % 2) {
			counter0d7++;
			if (counter0d7 > 0xFE)
				counter0d7 = 0x00;

			CanTxMessage msg(E90_SEATBELT_COUNTER, 2);
			msg[0] = counter0d7;
			msg[1] = 0xFF;
		}
	}

	{ //E90_BRAKE_COUNTER
		if (msgcounter % 2) {

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
	}

	{ //E90_ABS_COUNTER
		if (msgcounter % 2) {
			abscounter++;
			if (abscounter > 0xFE)
				abscounter = 0xF0;

			CanTxMessage msg(E90_ABS_COUNTER, 2);
			msg[0] = abscounter;
			msg[1] = 0xFF;
		}
	}

	{ //E90_FUEL
		if (msgcounter % 2) {
			CanTxMessage msg(E90_FUEL, 5); //fuel gauge
			msg[0] = 0x76;
			msg[1] = 0x0F;
			msg[2] = 0xBE;
			msg[3] = 0x1A;
			msg[4] = 0x00;
		}
	}

	{ //E90_GEAR
	if (msgcounter % 2) {

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
	}

	{ //E90_SPEED
		if (msgcounter % 2) {
		// float mph = getVehicleSpeed() * 0.6213712;
		float mph = 0;

		systime_t timer = TIME_I2MS(chVTGetSystemTime());

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

	// { // WARNING msg
	// 	CanTxMessage msg(0x592, 8);
	// 	msg[0] = 0x40;
	// 	msg[1] = 0x99; // error ID
	// 	msg[2] = 0x00;
	// 	msg[3] = 0x31; // first bit indicates on or off. 1 on, 0 off.
	// 	msg[4] = 0xFF;
	// 	msg[5] = 0xFF;
	// 	msg[6] = 0xFF;
	// 	msg[7] = 0xFF;

	// }
}

#endif // EFI_CAN_SUPPORT
