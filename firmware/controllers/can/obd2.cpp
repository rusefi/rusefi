/*
 * @file obd2.cpp
 *
 * ISO 15765-4
 * http://en.wikipedia.org/wiki/OBD-II_PIDs
 *
 * @date Jun 9, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "pch.h"

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST

#include "obd2.h"
#include "can.h"
#include "can_msg_tx.h"
#include "fuel_math.h"

static const int16_t supportedPids0120[] = { 
	PID_MONITOR_STATUS,
	PID_FUEL_SYSTEM_STATUS,
	PID_ENGINE_LOAD,
	PID_COOLANT_TEMP,
	PID_STFT_BANK1,
	PID_STFT_BANK2,
	PID_INTAKE_MAP,
	PID_RPM,
	PID_SPEED,
	PID_TIMING_ADVANCE,
	PID_INTAKE_TEMP,
	PID_THROTTLE,
	-1
};

static const int16_t supportedPids2140[] = {
	PID_FUEL_AIR_RATIO_1,
	-1
};

static const int16_t supportedPids4160[] = { 
	PID_FUEL_RATE,
	-1
};

void obdSendPacket(int mode, int PID, int numBytes, uint32_t iValue, size_t busIndex) {
	CanTxMessage resp(CanCategory::OBD, OBD_TEST_RESPONSE);

	// Respond on the same bus we got the request from
	resp.busIndex = busIndex;

	// write number of bytes
	resp[0] = (uint8_t)(2 + numBytes);
	// write 2 bytes of header
	resp[1] = (uint8_t)(0x40 + mode);
	resp[2] = (uint8_t)PID;
	// write N data bytes
	for (int i = 8 * (numBytes - 1), j = 3; i >= 0; i -= 8, j++) {
		resp[j] = (uint8_t)((iValue >> i) & 0xff);
	}
}

#define _1_MODE 1

static void obdSendValue(int mode, int PID, int numBytes, float value, size_t busIndex) {
	efiAssertVoid(ObdCode::CUSTOM_ERR_6662, numBytes <= 2, "invalid numBytes");
	int iValue = (int)efiRound(value, 1.0f);
	// clamp to uint8_t (0..255) or uint16_t (0..65535)
	iValue = maxI(minI(iValue, (numBytes == 1) ? 255 : 65535), 0);
	obdSendPacket(mode, PID, numBytes, iValue, busIndex);
}


// #define MOCK_SUPPORTED_PIDS 0xffffffff

void obdWriteSupportedPids(int PID, int bitOffset, const int16_t *supportedPids, size_t busIndex) {
	uint32_t value = 0;
	// gather all 32 bit fields
	for (int i = 0; i < 32 && supportedPids[i] > 0; i++)
		value |= 1 << (31 + bitOffset - supportedPids[i]);

#ifdef MOCK_SUPPORTED_PIDS
	// for OBD debug
	value = MOCK_SUPPORTED_PIDS;
#endif

	obdSendPacket(1, PID, 4, value, busIndex);
}

void handleGetDataRequest(const CANRxFrame& rx, size_t busIndex) {
	int pid = rx.data8[2];
	switch (pid) {
	case PID_SUPPORTED_PIDS_REQUEST_01_20:
		obdWriteSupportedPids(pid, 1, supportedPids0120, busIndex);
		break;
	case PID_SUPPORTED_PIDS_REQUEST_21_40:
		obdWriteSupportedPids(pid, 0x21, supportedPids2140, busIndex);
		break;
	case PID_SUPPORTED_PIDS_REQUEST_41_60:
		obdWriteSupportedPids(pid, 0x41, supportedPids4160, busIndex);
		break;
	case PID_MONITOR_STATUS:
		obdSendPacket(1, pid, 4, 0, busIndex);	// todo: add statuses
		break;
	case PID_FUEL_SYSTEM_STATUS:
		// todo: add statuses
		obdSendValue(_1_MODE, pid, 2, (2<<8)|(0), busIndex);	// 2 = "Closed loop, using oxygen sensor feedback to determine fuel mix"
		break;
	case PID_ENGINE_LOAD:
		obdSendValue(_1_MODE, pid, 1, getFuelingLoad() * ODB_TPS_BYTE_PERCENT, busIndex);
		break;
	case PID_COOLANT_TEMP:
		obdSendValue(_1_MODE, pid, 1, Sensor::getOrZero(SensorType::Clt) + ODB_TEMP_EXTRA, busIndex);
		break;
	case PID_STFT_BANK1:
		obdSendValue(_1_MODE, pid, 1, 128 * engine->engineState.stftCorrection[0], busIndex);
		break;
	case PID_STFT_BANK2:
		obdSendValue(_1_MODE, pid, 1, 128 * engine->engineState.stftCorrection[1], busIndex);
		break;
	case PID_INTAKE_MAP:
		obdSendValue(_1_MODE, pid, 1, Sensor::getOrZero(SensorType::Map), busIndex);
		break;
	case PID_RPM:
		obdSendValue(_1_MODE, pid, 2, Sensor::getOrZero(SensorType::Rpm) * ODB_RPM_MULT, busIndex);	//	rotation/min.	(A*256+B)/4
		break;
	case PID_SPEED:
		obdSendValue(_1_MODE, pid, 1, Sensor::getOrZero(SensorType::VehicleSpeed), busIndex);
		break;
	case PID_TIMING_ADVANCE: {
		float timing = engine->engineState.timingAdvance[0];
		timing = (timing > 360.0f) ? (timing - 720.0f) : timing;
		obdSendValue(_1_MODE, pid, 1, (timing + 64.0f) * 2.0f, busIndex);		// angle before TDC.	(A/2)-64
		break;
		}
	case PID_INTAKE_TEMP:
		obdSendValue(_1_MODE, pid, 1, Sensor::getOrZero(SensorType::Iat) + ODB_TEMP_EXTRA, busIndex);
		break;
	case PID_INTAKE_MAF:
		obdSendValue(_1_MODE, pid, 2, Sensor::getOrZero(SensorType::Maf) * 100.0f, busIndex);	// grams/sec	(A*256+B)/100
		break;
	case PID_THROTTLE:
		obdSendValue(_1_MODE, pid, 1, Sensor::getOrZero(SensorType::Tps1) * ODB_TPS_BYTE_PERCENT, busIndex);	// (A*100/255)
		break;
	case PID_FUEL_AIR_RATIO_1: {
		float lambda = clampF(0, Sensor::getOrZero(SensorType::Lambda1), 1.99f);

		uint16_t scaled = lambda * 32768;

		obdSendPacket(1, pid, 4, scaled << 16, busIndex);
		break;
	} case PID_FUEL_RATE: {
		float gPerSecond = engine->module<TripOdometer>()->getConsumptionGramPerSecond();
		float gPerHour = gPerSecond * 3600;
		float literPerHour = gPerHour * 0.00139f;
		obdSendValue(_1_MODE, pid, 2, literPerHour * 20.0f, busIndex);	//	L/h.	(A*256+B)/20
		break;
	} default:
		// ignore unhandled PIDs
		break;
	}
}

static void handleDtcRequest(int numCodes, ObdCode* dtcCode) {
	// TODO: this appears to be unfinished?
	UNUSED(numCodes);
	UNUSED(dtcCode);

	// int numBytes = numCodes * 2;
	// // write CAN-TP Single Frame header?
	// txmsg.data8[0] = (uint8_t)((0 << 4) | numBytes);
	// for (int i = 0, j = 1; i < numCodes; i++) {
	// 	txmsg.data8[j++] = (uint8_t)((dtcCode[i] >> 8) & 0xff);
	// 	txmsg.data8[j++] = (uint8_t)(dtcCode[i] & 0xff);
	// }
}

#if HAS_CAN_FRAME
void obdOnCanPacketRx(const CANRxFrame& rx, size_t busIndex) {
	if (CAN_SID(rx) != OBD_TEST_REQUEST) {
		return;
	}

	if (rx.data8[0] == _OBD_2 && rx.data8[1] == OBD_CURRENT_DATA) {
		handleGetDataRequest(rx, busIndex);
	} else if (rx.data8[0] == 1 && rx.data8[1] == OBD_STORED_DIAGNOSTIC_TROUBLE_CODES) {
		// todo: implement stored/pending difference?
		handleDtcRequest(1, &engine->engineState.warnings.lastErrorCode);
	} else if (rx.data8[0] == 1 && rx.data8[1] == OBD_PENDING_DIAGNOSTIC_TROUBLE_CODES) {
		// todo: implement stored/pending difference?
		handleDtcRequest(1, &engine->engineState.warnings.lastErrorCode);
	}
}
#endif /* HAS_CAN_FRAME */

#endif /* EFI_CAN_SUPPORT */
