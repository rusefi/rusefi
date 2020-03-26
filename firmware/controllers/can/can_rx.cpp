/**
 * @file	can_rx.cpp
 *
 * CAN reception handling.  This file handles multiplexing incoming CAN frames as appropriate
 * to the subsystems that consume them.
 *
 * @date Mar 19, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#include "global.h"
#if EFI_CAN_SUPPORT

#include "can.h"
#include "obd2.h"
#include "engine.h"

EXTERN_ENGINE;

static void printPacket(const CANRxFrame& rx, Logging* logger) {
	// only print info if we're in can debug mode
	if (CONFIG(debugMode) == DBG_CAN) {
		scheduleMsg(logger, "Got CAN message: SID %x/%x %x %x %x %x %x %x %x %x", rx.SID, rx.DLC, rx.data8[0], rx.data8[1],
				rx.data8[2], rx.data8[3], rx.data8[4], rx.data8[5], rx.data8[6], rx.data8[7]);
	}
}

volatile float aemXSeriesLambda = 0;
volatile float canPedal = 0;

void processCanRxMessage(const CANRxFrame& frame, Logging* logger) {
	// TODO: if/when we support multiple lambda sensors, sensor N
	// has address 0x0180 + N where N = [0, 15]
	if (frame.SID == 0x0180) {
		// AEM x-series lambda sensor reports in 0.0001 lambda per bit
		uint16_t lambdaInt = SWAP_UINT16(frame.data16[0]);
		aemXSeriesLambda = 0.0001f * lambdaInt;
	} else if (frame.EID == 0x202) {
		int16_t pedalScaled = *reinterpret_cast<const uint16_t*>(&frame.data8[0]);
		canPedal = pedalScaled * 0.01f;
	} else {
		printPacket(frame, logger);
		obdOnCanPacketRx(frame);
	}
}

#endif // EFI_CAN_SUPPORT
