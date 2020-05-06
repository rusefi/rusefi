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
#include "can_sensor.h"
#include "can_vss.h"

EXTERN_ENGINE;

static void printPacket(const CANRxFrame& rx, Logging* logger) {
	// only print info if we're in can debug mode
	if (CONFIG(debugMode) == DBG_CAN) {
		scheduleMsg(logger, "Got CAN message: SID %x/%x %x %x %x %x %x %x %x %x", rx.SID, rx.DLC, rx.data8[0], rx.data8[1],
				rx.data8[2], rx.data8[3], rx.data8[4], rx.data8[5], rx.data8[6], rx.data8[7]);
	}
}

volatile float aemXSeriesLambda = 0;
volatile float canMap = 0;

static CanSensorBase* s_head = nullptr;

void serviceCanSubscribers(const CANRxFrame& frame, efitick_t nowNt) {
	CanSensorBase* current = s_head;

	while (current) {
		current = current->processFrame(frame, nowNt);
	}
}

void registerCanSensor(CanSensorBase& sensor) {
	sensor.setNext(s_head);
	s_head = &sensor;
}

void processCanRxMessage(const CANRxFrame& frame, Logging* logger, efitick_t nowNt) {
	if (CONFIG(debugMode) == DBG_CAN) {
		printPacket(frame, logger);
	}

	serviceCanSubscribers(frame, nowNt);

	//Vss is configurable, should we handle it here:
	processCanRxVss(frame, nowNt);

	// TODO: if/when we support multiple lambda sensors, sensor N
	// has address 0x0180 + N where N = [0, 15]
	if (frame.SID == 0x0180) {
		// AEM x-series lambda sensor reports in 0.0001 lambda per bit
		uint16_t lambdaInt = SWAP_UINT16(frame.data16[0]);
		aemXSeriesLambda = 0.0001f * lambdaInt;
	} else if (frame.EID == CONFIG(verboseCanBaseAddress) + CAN_SENSOR_1_OFFSET) {
		int16_t mapScaled = *reinterpret_cast<const int16_t*>(&frame.data8[0]);
		canMap = mapScaled / (1.0 * PACK_MULT_PRESSURE);
		uint8_t cltShifted = *reinterpret_cast<const uint8_t*>(&frame.data8[2]);
#if EFI_CANBUS_SLAVE
		engine->sensors.clt = cltShifted - PACK_ADD_TEMPERATURE;
#endif /* EFI_CANBUS_SLAVE */
	} else {
		obdOnCanPacketRx(frame);
	}
}

#endif // EFI_CAN_SUPPORT
