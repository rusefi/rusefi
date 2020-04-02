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
#include "can.h"
#include "can_hw.h"
#include "can_dash.h"


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

extern CANTxFrame txmsg;

static void setShortValue(CANTxFrame *txmsg, int value, int offset) {
	txmsg->data8[offset] = value;
	txmsg->data8[offset + 1] = value >> 8;
}

void canDashboardBMW(void) {
	//BMW Dashboard
	commonTxInit(CAN_BMW_E46_SPEED);
	setShortValue(&txmsg, 10 * 8, 1);
	sendCanMessage();

	commonTxInit(CAN_BMW_E46_RPM);
	setShortValue(&txmsg, (int) (GET_RPM() * 6.4), 2);
	sendCanMessage();

	commonTxInit(CAN_BMW_E46_DME2);
	setShortValue(&txmsg, (int) ((getCoolantTemperature() + 48.373) / 0.75), 1);
	sendCanMessage();
}

void canMazdaRX8(void) {
	commonTxInit(CAN_MAZDA_RX_STEERING_WARNING);
	// todo: something needs to be set here? see http://rusefi.com/wiki/index.php?title=Vehicle:Mazda_Rx8_2004
	sendCanMessage();

	commonTxInit(CAN_MAZDA_RX_RPM_SPEED);

	float kph = getVehicleSpeed();

	setShortValue(&txmsg, SWAP_UINT16(GET_RPM() * 4), 0);
	setShortValue(&txmsg, 0xFFFF, 2);
	setShortValue(&txmsg, SWAP_UINT16((int )(100 * kph + 10000)), 4);
	setShortValue(&txmsg, 0, 6);
	sendCanMessage();

	commonTxInit(CAN_MAZDA_RX_STATUS_1);
	txmsg.data8[0] = 0xFE; //Unknown
	txmsg.data8[1] = 0xFE; //Unknown
	txmsg.data8[2] = 0xFE; //Unknown
	txmsg.data8[3] = 0x34; //DSC OFF in combo with byte 5 Live data only seen 0x34
	txmsg.data8[4] = 0x00; // B01000000; // Brake warning B00001000;  //ABS warning
	txmsg.data8[5] = 0x40; // TCS in combo with byte 3
	txmsg.data8[6] = 0x00; // Unknown
	txmsg.data8[7] = 0x00; // Unused
	sendCanMessage();

	commonTxInit(CAN_MAZDA_RX_STATUS_2);
	txmsg.data8[0] = (uint8_t)(getCoolantTemperature() + 69); //temp gauge //~170 is red, ~165 last bar, 152 centre, 90 first bar, 92 second bar
	txmsg.data8[1] = ((int16_t)(engine->engineState.vssEventCounter*(engineConfiguration->vehicleSpeedCoef*0.277*2.58))) & 0xff;
	txmsg.data8[2] = 0x00; // unknown
	txmsg.data8[3] = 0x00; //unknown
	txmsg.data8[4] = 0x01; //Oil Pressure (not really a gauge)
	txmsg.data8[5] = 0x00; //check engine light
	txmsg.data8[6] = 0x00; //Coolant, oil and battery
	if ((GET_RPM()>0) && (engine->sensors.vBatt<13)) {
		setTxBit(6, 6); // battery light
	}
	if (getCoolantTemperature() > 105) {
		setTxBit(6, 1); // coolant light, 101 - red zone, light means its get too hot
	}
	//oil pressure warning lamp bit is 7
	txmsg.data8[7] = 0x00; //unused
	sendCanMessage();
}

void canDashboardFiat(void) {
	//Fiat Dashboard
	commonTxInit(CAN_FIAT_MOTOR_INFO);
	setShortValue(&txmsg, (int) (getCoolantTemperature() - 40), 3); //Coolant Temp
	setShortValue(&txmsg, GET_RPM() / 32, 6); //RPM
	sendCanMessage();
}

void canDashboardVAG(void) {
	//VAG Dashboard
	commonTxInit(CAN_VAG_RPM);
	setShortValue(&txmsg, GET_RPM() * 4, 2); //RPM
	sendCanMessage();

	commonTxInit(CAN_VAG_CLT);
	setShortValue(&txmsg, (int) ((getCoolantTemperature() + 48.373) / 0.75), 1); //Coolant Temp
	sendCanMessage();

	commonTxInit(CAN_VAG_CLT_V2);
		setShortValue(&txmsg, (int) ((getCoolantTemperature() + 48.373) / 0.75), 4); //Coolant Temp
		sendCanMessage();

	commonTxInit(CAN_VAG_IMMO);
		setShortValue(&txmsg, 0x80, 1);
		sendCanMessage();
}

#endif // EFI_CAN_SUPPORT
