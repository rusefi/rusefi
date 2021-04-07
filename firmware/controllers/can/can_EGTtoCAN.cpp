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
#include "can_EGTtoCAN.h"
#include "tunerstudio_outputs.h"

// EGTTOCAN message format - MsgID, Length, EGT#, EGT#, EGT#, EGT#
// EGT 1-4 comes in on SID 610h, EGT 5-8 comes in on SID 611h
// EGTTOCAN module needs to be set to 500k baud rate for rusEFI
// Temperatures come in as simple hex values with no scaling. (eg. 0x17 - 23 degC)

// Need to add a config field for EM EGTtoCAN to allow .ini to have { max31855_cs1 != 0 || enableEcumasterEgt }


EGTtoCAN::EGTtoCAN(uint8_t EGTIndex)
	: CanListener(0x610 + EGTIndex)
	, m_egtIndex(EGTIndex)
{
}

void EGTtoCAN::decodeFrame(const CANRxFrame& frame, efitick_t nowNt) {
	if (frame.DLC != 8) {
		return;
	}

	auto& egts = tsOutputChannels.egtValues.values;

	if (m_egtIndex == 0) {
		egts[0] = SWAP_UINT16(frame.data16[0]);
		egts[1] = SWAP_UINT16(frame.data16[2]);
		egts[2] = SWAP_UINT16(frame.data16[4]);
		egts[3] = SWAP_UINT16(frame.data16[6]);
	} else {
		egts[4] = SWAP_UINT16(frame.data16[0]);
		egts[5] = SWAP_UINT16(frame.data16[2]);
		egts[6] = SWAP_UINT16(frame.data16[4]);
		egts[7] = SWAP_UINT16(frame.data16[6]);
	}
}
