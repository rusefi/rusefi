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
#include "engine.h"

EXTERN_ENGINE
;

typedef float fsio_table_8x8_f32t_linear[FSIO_TABLE_8 * FSIO_TABLE_8];

bool acceptCanRx(int sid DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (!CONFIG(useFSIOTableForCanSniffingFiltering)) {
		// accept anything if filtering is not enabled
		return true;
	}
/*
	// the whole table reuse and 2D table cast to 1D array is a major hack, but it's OK for prototyping
	fsio_table_8x8_f32t_linear *array =
			(fsio_table_8x8_f32t_linear*) (void*) &config->fsioTable1;

	int arraySize = efi::size(*array);

	int showOnlyCount = (int) array[arraySize - 1];
	if (showOnlyCount > 0 && showOnlyCount < arraySize) {
		for (int i = 0; i < showOnlyCount; i++) {
			if (sid == (int) array[arraySize - 2 - i]) {
				return true;
			}
		}
		// if white list is not empty and element not on the white list we do not check ignore list
		return false;
	}

	int ignoreListCount = (int) array[0];
	if (ignoreListCount > 0 && ignoreListCount < arraySize) {
		for (int i = 0; i < ignoreListCount; i++) {
			if (sid == (int) array[1 + i]) {
				// element is in ignore list
				return false;
			}
		}
	}
*/
	return true;
}

#if EFI_CAN_SUPPORT

#include "can.h"
#include "obd2.h"
#include "engine.h"
#include "can_sensor.h"
#include "can_vss.h"
#include "rusefi_wideband.h"

/**
 * this build-in CAN sniffer is very basic but that's our CAN sniffer
 */
static void printPacket(const CANRxFrame &rx) {
	bool accept = acceptCanRx(CAN_SID(rx));
	if (!accept) {
		return;
	}

	// only print info if we're in can debug mode

	// internet people use both hex and decimal to discuss packed IDs, for usability it's better to print both right here
	efiPrintf("CAN_rx %x %d %x, %x %x %x %x %x %x %x %x", CAN_SID(rx),
			CAN_SID(rx), rx.DLC, rx.data8[0], rx.data8[1], rx.data8[2], rx.data8[3],
			rx.data8[4], rx.data8[5], rx.data8[6], rx.data8[7]);

}

volatile float canMap = 0;

CanListener *canListeners_head = nullptr;

void serviceCanSubscribers(const CANRxFrame &frame, efitick_t nowNt) {
	CanListener *current = canListeners_head;

	while (current) {
		current = current->processFrame(frame, nowNt);
	}
}

void registerCanListener(CanListener& listener) {
	listener.setNext(canListeners_head);
	canListeners_head = &listener;
}

void registerCanSensor(CanSensorBase& sensor) {
	registerCanListener(sensor);
	sensor.Register();
}

void processCanRxMessage(const CANRxFrame &frame, efitick_t nowNt) {
	if (CONFIG(debugMode) == DBG_CAN) {
		printPacket(frame);
	}

	serviceCanSubscribers(frame, nowNt);

	//Vss is configurable, should we handle it here:
	processCanRxVss(frame, nowNt);

#if EFI_CANBUS_SLAVE
	if (CAN_EID(frame) == CONFIG(verboseCanBaseAddress) + CAN_SENSOR_1_OFFSET) {
		int16_t mapScaled = *reinterpret_cast<const int16_t*>(&frame.data8[0]);
		canMap = mapScaled / (1.0 * PACK_MULT_PRESSURE);
	} else
#endif
	{
		obdOnCanPacketRx(frame);
	}

#if EFI_WIDEBAND_FIRMWARE_UPDATE
	// Bootloader acks with address 0x727573 aka ascii "rus"
	if (CAN_EID(frame) == 0x727573) {
		handleWidebandBootloaderAck();
	}
#endif
}

#endif // EFI_CAN_SUPPORT
