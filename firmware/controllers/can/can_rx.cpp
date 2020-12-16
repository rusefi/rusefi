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

volatile float canMap = 0;

CanSensorBase* cansensors_head = nullptr;

void serviceCanSubscribers(const CANRxFrame& frame, efitick_t nowNt) {
	CanSensorBase* current = cansensors_head;

	while (current) {
		current = current->processFrame(frame, nowNt);
	}
}

void registerCanSensor(CanSensorBase& sensor) {
	sensor.setNext(cansensors_head);
	cansensors_head = &sensor;
	sensor.Register();
}

void processCanRxMessage(const CANRxFrame& frame, Logging* logger, efitick_t nowNt) {
	if (CONFIG(debugMode) == DBG_CAN) {
		printPacket(frame, logger);
	}

	serviceCanSubscribers(frame, nowNt);

	//Vss is configurable, should we handle it here:
	processCanRxVss(frame, nowNt);

#if EFI_CANBUS_SLAVE
	if (frame.EID == CONFIG(verboseCanBaseAddress) + CAN_SENSOR_1_OFFSET) {
		int16_t mapScaled = *reinterpret_cast<const int16_t*>(&frame.data8[0]);
		canMap = mapScaled / (1.0 * PACK_MULT_PRESSURE);
	} else
#endif
	{
		obdOnCanPacketRx(frame);
	}

#if EFI_WIDEBAND_FIRMWARE_UPDATE
	// Bootloader acks with address 0x727573 aka ascii "rus"
	if (frame.EID == 0x727573) {
		handleWidebandBootloaderAck();
	}
#endif
}

#endif // EFI_CAN_SUPPORT
