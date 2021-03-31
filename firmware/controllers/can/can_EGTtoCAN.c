
/**
 * @file	can_EGTtoCAN.cpp
 *
 * This file hadles the receipt of EGT data via can from the ECUMaster EGTTOCAN module
 *
 * @date Mar 20, 2021
 * @author Simon Orchard, (c) 2021
 */

#include "global.h"
#include "globalaccess.h"

// EGTTOCAN message format - MsgID, Length, EGT#, EGT#, EGT#, EGT#
// EGT 1-4 comes in on SID 610h, EGT 5-8 comes in on SID 611h
// EGTTOCAN module needs to be set to 500k baud rate for rusEFI
// Temperatures come in as simple hex values with no scaling. (eg. 0x17 - 23 degC)

//Need to add a config field for EM EGTtoCAN to allow .ini to have { max31855_cs1 != 0 || enableEcumasterEgt }


EGTtoCAN::EGTtoCAN(uint8_t sensorIndex, SensorType type)
	: CanSensorBase(
		0x610 + EGTIndex,	// EGT 1-4 
	)
{}

void EGTtoCAN::decodeFrame(const CANRxFrame& frame, efitick_t nowNt) {
	if (frame.DLC != 8) {
		invalidate();
		return;
	}
	if EGTIndex = 0 
	uint16_t egt1 = SWAP_UINT16(frame.data16[0]);
	uint16_t egt2 = SWAP_UINT16(frame.data16[2]);
	uint16_t egt3 = SWAP_UINT16(frame.data16[4]);
	uint16_t egt4 = SWAP_UINT16(frame.data16[6]);
	else
	uint16_t egt5 = SWAP_UINT16(frame.data16[0]);
	uint16_t egt6 = SWAP_UINT16(frame.data16[2]);
	uint16_t egt7 = SWAP_UINT16(frame.data16[4]);
	uint16_t egt8 = SWAP_UINT16(frame.data16[6]);
}
