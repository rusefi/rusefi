/**
 * @file	can_rx.cpp
 *
 * CAN reception handling.  This file handles multiplexing incoming CAN frames as appropriate
 * to the subsystems that consume them.
 *
 * @date Mar 19, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#include "pch.h"

#include "rusefi_lua.h"

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
	efiPrintf("CAN_rx %x %d %x %x %x %x %x %x %x %x %x",
			CAN_SID(rx),
			CAN_SID(rx), rx.DLC,
			rx.data8[0], rx.data8[1], rx.data8[2], rx.data8[3],
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

#define VAG_YAW_RATE_G_LAT 0x130
#define VAG_YAW_ACCEL_G_LONG 0x131

#define MM5_10_YAW_Y 0x174
#define MM5_10_ROLL_X 0x178
#define MM5_10_Z 0x17C

static uint16_t getLSB_intel(const CANRxFrame& frame, int offset) {
	return (frame.data8[offset + 1] << 8) + frame.data8[offset];
}

static int16_t getShiftedLSB_intel(const CANRxFrame& frame, int offset) {
	return getLSB_intel(frame, offset) - 0x8000;
}

static void processCanRxImu(const CANRxFrame& frame, efitick_t nowNt) {
/*
	if (CAN_SID(frame) == 0x130) {
		float a = getShiftedLSB_intel(frame, 0);
		float b = getShiftedLSB_intel(frame, 4);
		efiPrintf("CAN_rx 130 %f %f", a, b);
	}

	if (CONFIG(imuType) == IMU_VAG) {
		if (CAN_SID(frame) == VAG_YAW_RATE_G_LAT) {
			efiPrintf("CAN_rx VAG_YAW_RATE_G_LAT");
		} else if (CAN_SID(frame) == VAG_YAW_ACCEL_G_LONG) {
			efiPrintf("CAN_rx VAG_YAW_ACCEL_G_LONG");
		}
	}
	*/



	if (CONFIG(imuType) == IMU_MM5_10) {
#define MM5_10_RATE_QUANT 0.005
#define MM5_10_ACC_QUANT 0.0001274

		if (CAN_SID(frame) == MM5_10_YAW_Y) {
			float yaw = getShiftedLSB_intel(frame, 0);
			float accY = getShiftedLSB_intel(frame, 4);

			efiPrintf("CAN_rx MM5_10_YAW_Y %f %f", yaw, accY);
			engine->sensors.accelerometer.yaw = yaw * MM5_10_RATE_QUANT;
			engine->sensors.accelerometer.y = accY * MM5_10_ACC_QUANT;
		} else if (CAN_SID(frame) == MM5_10_ROLL_X) {
			float roll = getShiftedLSB_intel(frame, 0);
			float accX = getShiftedLSB_intel(frame, 4);
			efiPrintf("CAN_rx MM5_10_ROLL_X %f %f", roll, accX);

			engine->sensors.accelerometer.roll = roll * MM5_10_RATE_QUANT;
			engine->sensors.accelerometer.x = accX * MM5_10_ACC_QUANT;
		} else if (CAN_SID(frame) == MM5_10_Z) {
			float accZ = getShiftedLSB_intel(frame, 4);
			efiPrintf("CAN_rx MM5_10_Z %f", accZ);
			engine->sensors.accelerometer.z = accZ * MM5_10_ACC_QUANT;
		}
	}
}

void processCanRxMessage(const CANRxFrame &frame, efitick_t nowNt) {
	if (CONFIG(debugMode) == DBG_CAN) {
		printPacket(frame);
	}

	serviceCanSubscribers(frame, nowNt);

	// todo: convert to CanListener or not?
	//Vss is configurable, should we handle it here:
	processCanRxVss(frame, nowNt);

	// todo: convert to CanListener or not?
	processCanRxImu(frame, nowNt);

	processLuaCan(frame);

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
