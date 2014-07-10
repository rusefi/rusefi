/**
 * @file	tunerstudio_algo.c
 * @brief	Tuner Studio plain protocol implementation
 *
 * This implementation would not happen without the documentation
 * provided by Jon Zeeff (jon@zeeff.com)
 *
 * Tuner Studio has a really simple protocol, a minimal implementation
 * capable of displaying current engine state on the gauges would
 * require only two commands: queryCommand and ochGetCommand
 *
 * queryCommand:
 * 		Communication initialization command. TunerStudio sends a single byte H
 * 		ECU response:
 * 			One of the known ECU id strings. We are using "MShift v0.01" id string.
 *
 * ochGetCommand:
 * 		Request for output channels state.TunerStudio sends a single byte O
 * 		ECU response:
 * 			A snapshot of output channels as described in [OutputChannels] section of the .ini file
 * 			The length of this block is 'ochBlockSize' property of the .ini file
 *
 * These two commands are enough to get working gauges. In order to start configuring the ECU using
 * tuner studio, three more commands should be implemented:
 *
 * todo: merge this file with tunerstudio.c?
 *
 *
 * @date Oct 22, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
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

#include <string.h>
#include "tunerstudio_algo.h"
#include "tunerstudio_configuration.h"
#include "engine_configuration.h"
#include "tunerstudio.h"

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif

TunerStudioState tsState;
TunerStudioOutputChannels tsOutputChannels;
/**
 * this is a local copy of the configuration. Any changes to this copy
 * have no effect until this copy is explicitly propagated to the main working copy
 */
persistent_config_s configWorkingCopy;

void tunerStudioError(const char *msg) {
	tunerStudioDebug(msg);
	tsState.errorCounter++;
}

int tunerStudioHandleCrcCommand(char *data, int incomingPacketSize) {
	char command = data[0];
	data++;
	if (command == TS_HELLO_COMMAND) {
		tunerStudioDebug("got CRC Query");
		handleQueryCommand(TS_CRC);
	} else if (command == TS_OUTPUT_COMMAND) {
		handleOutputChannelsCommand(TS_CRC);
	} else if (command == TS_PAGE_COMMAND) {
		uint16_t page = *(uint16_t *) data;
		handlePageSelectCommand(TS_CRC, page);
	} else if (command == TS_CHUNK_WRITE_COMMAND) {
		uint16_t offset = *(uint16_t *) data;
		uint16_t count = *(uint16_t *) (data + 2);
		handleWriteChunkCommand(TS_CRC, offset, count, data + 4);
	} else if (command == TS_SINGLE_WRITE_COMMAND) {
		uint16_t page = *(uint16_t *) data;
		uint16_t offset = *(uint16_t *) (data + 2);
		uint8_t value = data[4];
		handleWriteValueCommand(TS_CRC, page, offset, value);
	} else if (command == TS_BURN_COMMAND) {
		uint16_t page = *(uint16_t *) data;
		handleBurnCommand(TS_CRC, page);
	} else if (command == TS_READ_COMMAND) {
		uint16_t page = *(uint16_t *) data;
		uint16_t offset = *(uint16_t *) (data + 2);
		uint16_t count = *(uint16_t *) (data + 4);
		handlePageReadCommand(TS_CRC, page, offset, count);
	} else if (command == 't' || command == 'T') {
		handleTestCommand();
	} else if (command == 'F') {
		tunerStudioDebug("ignoring F");
		/**
		 * http://www.msextra.com/forums/viewtopic.php?f=122&t=48327
		 * Response from TS support: This is an optional command		 *
		 * "The F command is used to find what ini. file needs to be loaded in TunerStudio to match the controller.
		 * If you are able to just make your firmware ignore the command that would work.
		 * Currently on some firmware versions the F command is not used and is just ignored by the firmware as a unknown command."
		 */
	} else {
		tunerStudioError("ERROR: ignoring unexpected command");
		return FALSE;
	}
	return TRUE;
}

void tsSendResponse(ts_response_format_e mode, const uint8_t * buffer, int size) {
	if (mode == TS_CRC) {
		tunerStudioWriteCrcPacket(TS_RESPONSE_OK, buffer, size);
	} else {
		if (size > 0)
			tunerStudioWriteData(buffer, size);
	}
}

/**
 * Query with CRC takes place while re-establishing connection
 * Query without CRC takes place on TunerStudio startup
 */
void handleQueryCommand(ts_response_format_e mode) {
	tsState.queryCommandCounter++;
	tunerStudioDebug("got H (queryCommand)");
	tsSendResponse(mode, (const uint8_t *) TS_SIGNATURE, strlen(TS_SIGNATURE) + 1);
}

/**
 * @brief 'Output' command sends out a snapshot of current values
 */
void handleOutputChannelsCommand(ts_response_format_e mode) {
	tsState.outputChannelsCommandCounter++;
	// this method is invoked too often to print any debug information
	tsSendResponse(mode, (const uint8_t *) &tsOutputChannels, sizeof(TunerStudioOutputChannels));
}

void handleTestCommand(void) {
	/**
	 * this is NOT a standard TunerStudio command, this is my own
	 * extension of the protocol to simplify troubleshooting
	 */
	tunerStudioDebug("got T (Test)");
	tunerStudioWriteData((const uint8_t *) "alive\r\n", 7);
}
