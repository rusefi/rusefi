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
		msg[0] = (int)(getCoolantTemperature()+40); // CLT - 0x80 ~ 80C
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
#endif // EFI_CAN_SUPPORT
