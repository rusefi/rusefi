
#include "can.h"
#include "obd2.h"


static void printPacket(const CANRxFrame& rx, Logging* logger) {
	scheduleMsg(logger, "Got CAN message: SID %x/%x %x %x %x %x %x %x %x %x", rx.SID, rx.DLC, rx.data8[0], rx.data8[1],
			rx.data8[2], rx.data8[3], rx.data8[4], rx.data8[5], rx.data8[6], rx.data8[7]);
}

volatile float aemXSeriesLambda = 0;

void processCanRxMessage(const CANRxFrame& frame, Logging* logger) {
	// TODO: if/when we support multiple lambda sensors, sensor N
	// has address 0x0180 + N where N = [0, 15]
	if (frame.SID == 0x0180) {
		// AEM x-series lambda sensor reports in 0.0001 lambda per bit
		uint16_t lambdaInt = SWAP_UINT16(frame.data16[0]);
		aemXSeriesLambda = 0.0001f * lambdaInt;
	} else {
		printPacket(frame, logger);
		obdOnCanPacketRx(frame);
	}
}
