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

typedef float SCRIPT_TABLE_8x8_f32t_linear[SCRIPT_TABLE_8 * SCRIPT_TABLE_8];

bool acceptCanRx(int /*sid*/) {
	if (!engineConfiguration->usescriptTableForCanSniffingFiltering) {
		// accept anything if filtering is not enabled
		return true;
	}
/*
	// the whole table reuse and 2D table cast to 1D array is a major hack, but it's OK for prototyping
	SCRIPT_TABLE_8x8_f32t_linear *array =
			(SCRIPT_TABLE_8x8_f32t_linear*) (void*) &config->scriptTable1;

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
static void printPacket(const size_t busIndex, const CANRxFrame &rx) {
//	bool accept = acceptCanRx(CAN_SID(rx));
//	if (!accept) {
//		return;
//	}

	// only print info if we're in can debug mode

	int id = CAN_ID(rx);

	// internet people use both hex and decimal to discuss packed IDs, for usability it's better to print both right here
	efiPrintf("CAN RX bus %d ID %x(%d) DLC %d: %02x %02x %02x %02x %02x %02x %02x %02x",
			busIndex,
			id,	id, // once in hex, once in dec
			rx.DLC,
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

/*
 * TODO:
 *  - convert to CanListener
 *  - move to hw_layer/sensors/yaw_rate_sensor.cpp | accelerometer.cpp ?
 */

#define VAG_YAW_RATE_G_LAT 0x130
#define VAG_YAW_ACCEL_G_LONG 0x131

/* Bosch Acceleration Sensor MM5.10 quantizations */
#define MM5_10_RATE_QUANT			0.005
#define MM5_10_ACC_QUANT			0.0001274

/* Bosch Acceleration Sensor MM5.10 CAN IDs */
#define MM5_10_YAW_Y				0x174
#define MM5_10_ROLL_X				0x178
#define MM5_10_Z					0x17C

/* Mercedes pn: A 006 542 26 18 CAN IDs */
#define MM5_10_MB_YAW_Y_CANID		0x150
#define MM5_10_MB_ROLL_X_CANID		0x151

static uint16_t getLSB_intel(const CANRxFrame& frame, int offset) {
	return (frame.data8[offset + 1] << 8) + frame.data8[offset];
}

static int16_t getShiftedLSB_intel(const CANRxFrame& frame, int offset) {
	return getLSB_intel(frame, offset) - 0x8000;
}

static void processCanRxImu_BoschM5_10_YawY(const CANRxFrame& frame) {
	float yaw = getShiftedLSB_intel(frame, 0);
	float accY = getShiftedLSB_intel(frame, 4);

	efiPrintf("CAN_rx MM5_10_YAW_Y %f %f", yaw, accY);
	engine->sensors.accelerometer.yaw = yaw * MM5_10_RATE_QUANT;
	engine->sensors.accelerometer.y = accY * MM5_10_ACC_QUANT;
}

static void processCanRxImu_BoschM5_10_RollX(const CANRxFrame& frame) {
	float roll = getShiftedLSB_intel(frame, 0);
	float accX = getShiftedLSB_intel(frame, 4);
	efiPrintf("CAN_rx MM5_10_ROLL_X %f %f", roll, accX);

	engine->sensors.accelerometer.roll = roll * MM5_10_RATE_QUANT;
	engine->sensors.accelerometer.x = accX * MM5_10_ACC_QUANT;
}

static void processCanRxImu_BoschM5_10_Z(const CANRxFrame& frame) {
	float accZ = getShiftedLSB_intel(frame, 4);
	efiPrintf("CAN_rx MM5_10_Z %f", accZ);
	engine->sensors.accelerometer.z = accZ * MM5_10_ACC_QUANT;
}

static void processCanRxImu(const CANRxFrame& frame) {
/*
	if (CAN_SID(frame) == 0x130) {
		float a = getShiftedLSB_intel(frame, 0);
		float b = getShiftedLSB_intel(frame, 4);
		efiPrintf("CAN_rx 130 %f %f", a, b);
	}

	if (engineConfiguration->imuType == IMU_VAG) {
		if (CAN_SID(frame) == VAG_YAW_RATE_G_LAT) {
			efiPrintf("CAN_rx VAG_YAW_RATE_G_LAT");
		} else if (CAN_SID(frame) == VAG_YAW_ACCEL_G_LONG) {
			efiPrintf("CAN_rx VAG_YAW_ACCEL_G_LONG");
		}
	}
	*/

	if (engineConfiguration->imuType == IMU_MM5_10) {
		if (CAN_SID(frame) == MM5_10_YAW_Y) {
			processCanRxImu_BoschM5_10_YawY(frame);
		} else if (CAN_SID(frame) == MM5_10_ROLL_X) {
			processCanRxImu_BoschM5_10_RollX(frame);
		} else if (CAN_SID(frame) == MM5_10_Z) {
			processCanRxImu_BoschM5_10_Z(frame);
		}
	} else if (engineConfiguration->imuType == IMU_TYPE_MB_A0065422618) {
		if (CAN_SID(frame) == MM5_10_MB_YAW_Y_CANID) {
			processCanRxImu_BoschM5_10_YawY(frame);
		} else if (CAN_SID(frame) == MM5_10_MB_ROLL_X_CANID) {
			processCanRxImu_BoschM5_10_RollX(frame);
		}
	}
}

void processCanRxMessage(const size_t busIndex, const CANRxFrame &frame, efitick_t nowNt) {
	if (engineConfiguration->verboseCan && busIndex == 0) {
		printPacket(busIndex, frame);
	} else if (engineConfiguration->verboseCan2 && busIndex == 1) {
		printPacket(busIndex, frame);
	}


	serviceCanSubscribers(frame, nowNt);

	// todo: convert to CanListener or not?
	//Vss is configurable, should we handle it here:
	processCanRxVss(frame, nowNt);

	// todo: convert to CanListener or not?
	processCanRxImu(frame);

	processLuaCan(busIndex, frame);

#if EFI_CANBUS_SLAVE
	if (CAN_EID(frame) == engineConfiguration->verboseCanBaseAddress + CAN_SENSOR_1_OFFSET) {
		int16_t mapScaled = *reinterpret_cast<const int16_t*>(&frame.data8[0]);
		canMap = mapScaled / (1.0 * PACK_MULT_PRESSURE);
	} else
#endif
	{
		obdOnCanPacketRx(frame, busIndex);
	}

#if EFI_WIDEBAND_FIRMWARE_UPDATE
	// Bootloader acks with address 0x727573 aka ascii "rus"
	if (CAN_EID(frame) == 0x727573) {
		handleWidebandBootloaderAck();
	}
#endif
#if EFI_USE_OPENBLT
	if ((CAN_SID(frame) == 0x667) && (frame.DLC == 2)) {
		/* TODO: gracefull shutdown? */
		if (((busIndex == 0) && (engineConfiguration->canOpenBLT)) ||
			((busIndex == 1) && (engineConfiguration->can2OpenBLT))) {
			jump_to_openblt();
		}
	}
#endif
}

#endif // EFI_CAN_SUPPORT
