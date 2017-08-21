/*
 * @file obd2.cpp
 *
 * ISO 15765-4
 * http://en.wikipedia.org/wiki/OBD-II_PIDs
 *
 * @date Jun 9, 2015
 * @author Andrey Belomutskiy, (c) 2012-2017
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

#include "main.h"
#include "obd2.h"
#include "can_hw.h"

extern CANTxFrame txmsg;

static LoggingWithStorage logger("obd2");

static void handleGetDataRequest(CANRxFrame *rx) {
	if (rx->data8[2] == PID_SUPPORTED_PIDS_REQUEST) {
		scheduleMsg(&logger, "Got lookup request");

		commonTxInit(OBD_TEST_RESPONSE);
		txmsg.data8[0] = 6; // 6 bytes
		txmsg.data8[1] = 0x41; // mode 1
		txmsg.data8[2] = PID_SUPPORTED_PIDS_REQUEST;

		setTxBit(3, 8 - PID_ENGINE_LOAD);
		setTxBit(3, 8 - PID_COOLANT_TEMP);

		setTxBit(4, 16 - PID_RPM);
		setTxBit(4, 16 - PID_TIMING_ADVANCE);
		setTxBit(4, 16 - PID_INTAKE_TEMP);

		setTxBit(5, 24 - PID_THROTTLE);

		sendCanMessage();
	} else if (rx->data8[2] == PID_COOLANT_TEMP) {
		scheduleMsg(&logger, "Got CLT request");

		int clt = 67; // todo: use real value

		commonTxInit(OBD_TEST_RESPONSE);
		txmsg.data8[0] = 3; // 3 bytes
		txmsg.data8[1] = 0x41; // mode 1
		txmsg.data8[2] = PID_COOLANT_TEMP;
		txmsg.data8[3] = clt + 40;
		sendCanMessage();
	} else if (rx->data8[2] == PID_RPM) {
		scheduleMsg(&logger, "Got RPM request");
		unsigned int rpm = 2000*4; //	rotation/min.	((A*256)+B)/4 todo: use real value
		commonTxInit(OBD_TEST_RESPONSE);
		txmsg.data8[0] = 4; // 4 bytes
		txmsg.data8[1] = 0x41; // mode 1
		txmsg.data8[2] = PID_RPM;
		txmsg.data8[3] = ((rpm>>8) & 0xFF);
		txmsg.data8[4] = ((rpm) & 0xFF);
		sendCanMessage();
		
	} else if (rx->data8[2] == PID_TIMING_ADVANCE) {
		scheduleMsg(&logger, "Got timing request");
	} else if (rx->data8[2] == PID_SPEED) {
		scheduleMsg(&logger, "Got speed request");
		commonTxInit(OBD_TEST_RESPONSE);
		txmsg.data8[0] = 3; // 3 bytes
		txmsg.data8[1] = 0x41; // mode 1
		txmsg.data8[2] = PID_INTAKE_MAP;
		txmsg.data8[3] = 123; // todo: real speed
		sendCanMessage();
	} else if (rx->data8[2] == PID_FUEL_PRESSURE) {
		scheduleMsg(&logger, "Got fuel pressure request");
	} else if (rx->data8[2] == PID_INTAKE_MAP) {
		scheduleMsg(&logger, "Got MAP request");
		commonTxInit(OBD_TEST_RESPONSE);
		txmsg.data8[0] = 3; // 3 bytes
		txmsg.data8[1] = 0x41; // mode 1
		txmsg.data8[2] = PID_INTAKE_MAP;
		txmsg.data8[3] = 46; // todo: real MAP
		sendCanMessage();
	} else {
		scheduleMsg(&logger, "Got unhandled request");
	}

}

void obdOnCanPacketRx(CANRxFrame *rx) {
	if (rx->SID != OBD_TEST_REQUEST) {
		return;
	}
	if (rx->data8[0] == 2 && rx->data8[1] == OBD_CURRENT_DATA) {
		handleGetDataRequest(rx);
	} else if (rx->data8[0] == 1 && rx->data8[1] == OBD_STORED_DIAGNOSTIC_TROUBLE_CODES) {
		scheduleMsg(&logger, "Got stored DTC request");
	} else {
		scheduleMsg(&logger, "Got unhandled OBD message");
	}
}

