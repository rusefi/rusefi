/**
 * @file	tunerstudio.cpp
 * @brief	Binary protocol implementation
 *
 * This implementation would not happen without the documentation
 * provided by Jon Zeeff (jon@zeeff.com)
 *
 *
 * @brief Integration with EFI Analytics Tuner Studio software
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
 * @author Andrey Belomutskiy, (c) 2012-2015
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
 *
 * @author Andrey Belomutskiy, (c) 2012-2015
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
 *
 */

#include "main.h"

#include "engine_state.h"
#include "tunerstudio.h"

#include "main_trigger_callback.h"
#include "flash_main.h"

#include "tunerstudio_io.h"
#include "tunerstudio_configuration.h"
#include "malfunction_central.h"
#include "console_io.h"
#include "crc.h"

#include <string.h>
#include "engine_configuration.h"
#include "svnversion.h"
#include "loggingcentral.h"

#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)

EXTERN_ENGINE
;

extern short currentPageId;

// in MS, that's 10 seconds
#define TS_READ_TIMEOUT 10000

Logging *tsLogger;

extern persistent_config_s configWorkingCopy;
extern persistent_config_container_s persistentState;

static efitimems_t previousWriteReportMs = 0;

ts_channel_s tsChannel;

extern uint8_t crcWriteBuffer[300];

static int ts_serial_ready(void) {
#if EFI_PROD_CODE
	if (isSerialOverUart()) {
		// TS uses USB when console uses serial
		return is_usb_serial_ready();
	} else {
		// TS uses serial when console uses USB
		return true;
	}
#else
	return true;
#endif
}

// this thread wants a bit extra stack
static THD_WORKING_AREA(tsThreadStack, UTILITY_THREAD_STACK_SIZE + 200);

extern TunerStudioOutputChannels tsOutputChannels;

extern tunerstudio_counters_s tsState;

static void resetTs(void) {
	memset(&tsState, 0, sizeof(tsState));
}

void printTsStats(void) {
#if EFI_PROD_CODE
	if (!isSerialOverUart()) {
		scheduleMsg(tsLogger, "TS RX on %s%d/TX on %s%d @%d", portname(TS_SERIAL_RX_PORT), TS_SERIAL_RX_PIN,
				portname(TS_SERIAL_TX_PORT), TS_SERIAL_TX_PIN, boardConfiguration->tunerStudioSerialSpeed);
	}
#endif /* EFI_PROD_CODE */
	scheduleMsg(tsLogger, "TunerStudio size=%d / total=%d / errors=%d / H=%d / O=%d / P=%d / B=%d",
			sizeof(tsOutputChannels), tsState.tsCounter, tsState.errorCounter, tsState.queryCommandCounter,
			tsState.outputChannelsCommandCounter, tsState.readPageCommandsCounter, tsState.burnCommandCounter);
	scheduleMsg(tsLogger, "TunerStudio W=%d / C=%d / P=%d / page=%d", tsState.writeValueCommandCounter,
			tsState.writeChunkCommandCounter, tsState.pageCommandCounter, currentPageId);
	scheduleMsg(tsLogger, "page size=%d", getTunerStudioPageSize(currentPageId));

//	scheduleMsg(logger, "analogChartFrequency %d",
//			(int) (&engineConfiguration->analogChartFrequency) - (int) engineConfiguration);
//
//	int fuelMapOffset = (int) (&engineConfiguration->fuelTable) - (int) engineConfiguration;
//	scheduleMsg(logger, "fuelTable %d", fuelMapOffset);
//
//	int offset = (int) (&boardConfiguration->hip9011Gain) - (int) engineConfiguration;
//	scheduleMsg(tsLogger, "hip9011Gain %d", offset);
//
//	offset = (int) (&engineConfiguration->crankingCycleBins) - (int) engineConfiguration;
//	scheduleMsg(tsLogger, "crankingCycleBins %d", offset);
//
//	offset = (int) (&engineConfiguration->engineCycle) - (int) engineConfiguration;
//	scheduleMsg(tsLogger, "engineCycle %d", offset);
}

static void setTsSpeed(int value) {
	boardConfiguration->tunerStudioSerialSpeed = value;
	printTsStats();
}

void tunerStudioDebug(const char *msg) {
#if EFI_TUNER_STUDIO_VERBOSE
	scheduleMsg(tsLogger, "%s", msg);
	printTsStats();
#endif
}

char *getWorkingPageAddr(int pageIndex) {
	switch (pageIndex) {
	case 0:
		return (char*) &configWorkingCopy.engineConfiguration;
	case 1:
		return (char*) &configWorkingCopy.ve2Table;
//	case 2:
//		return (char*) &configWorkingCopy.fuelTable;
//	case 3:
//		return (char*) &configWorkingCopy.ignitionTable;
//	case 4:
//		return (char*) &configWorkingCopy.veTable;
//	case 5:
//		return (char*) &configWorkingCopy.afrTable;
//	case 6:
//		return (char*) &configWorkingCopy.injectionPhase;
//	case 2: // fuelTable
//	case 3: // ignitionTable
//	case 4: // veTable
//	case 5: // afrTable
//		return (char*) &configWorkingCopy.engineConfiguration + PAGE_0_SIZE + (pageIndex - 2) * 1024;
	}
	return NULL;
}

int getTunerStudioPageSize(int pageIndex) {
	switch (pageIndex) {
	case 0:
		return PAGE_0_SIZE;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		return PAGE_1_SIZE;
//	case 2:
//	case 3:
//	case 4:
//		return 1024;
	}
	return 0;
}

void handlePageSelectCommand(ts_channel_s *tsChannel, ts_response_format_e mode, uint16_t pageId) {
	tsState.pageCommandCounter++;

	currentPageId = pageId;
	scheduleMsg(tsLogger, "PAGE %d", currentPageId);
	tsSendResponse(tsChannel, mode, NULL, 0);
}

/**
 * This command is needed to make the whole transfer a bit faster
 * @note See also handleWriteValueCommand
 */
void handleWriteChunkCommand(ts_channel_s *tsChannel, ts_response_format_e mode, short offset, short count, void *content) {
	tsState.writeChunkCommandCounter++;

	scheduleMsg(tsLogger, "WRITE CHUNK m=%d p=%d o=%d s=%d", mode, currentPageId, offset, count);

	if (offset > getTunerStudioPageSize(currentPageId)) {
		scheduleMsg(tsLogger, "ERROR invalid offset %d", offset);
		tunerStudioError("ERROR: out of range");
		offset = 0;
	}

	if (count > getTunerStudioPageSize(currentPageId)) {
		tunerStudioError("ERROR: unexpected count");
		scheduleMsg(tsLogger, "ERROR unexpected count %d", count);
		count = 0;
	}

	uint8_t * addr = (uint8_t *) (getWorkingPageAddr(currentPageId) + offset);
	memcpy(addr, content, count);

	tsSendResponse(tsChannel, mode, NULL, 0);
	printTsStats();
}

/**
 * 'Write' command receives a single value at a given offset
 * @note Writing values one by one is pretty slow
 */
void handleWriteValueCommand(ts_channel_s *tsChannel, ts_response_format_e mode, uint16_t page, uint16_t offset, uint8_t value) {
	tsState.writeValueCommandCounter++;

	currentPageId = page;

//tunerStudioDebug("got W (Write)"); // we can get a lot of these

#if EFI_TUNER_STUDIO_VERBOSE
//	scheduleMsg(logger, "Page number %d\r\n", pageId); // we can get a lot of these
#endif

//	int size = sizeof(TunerStudioWriteValueRequest);
//	scheduleMsg(logger, "Reading %d\r\n", size);

	if (offset > getTunerStudioPageSize(currentPageId)) {
		tunerStudioError("ERROR: out of range2");
		scheduleMsg(tsLogger, "ERROR offset %d", offset);
		offset = 0;
		return;
	}

	efitimems_t nowMs = currentTimeMillis();
	if (nowMs - previousWriteReportMs > 5) {
		previousWriteReportMs = nowMs;
		scheduleMsg(tsLogger, "page %d offset %d: value=%d", currentPageId, offset, value);
	}

	getWorkingPageAddr(currentPageId)[offset] = value;

//	scheduleMsg(logger, "va=%d", configWorkingCopy.boardConfiguration.idleValvePin);
}

static void sendErrorCode(ts_channel_s *tsChannel) {
	tunerStudioWriteCrcPacket(tsChannel, TS_RESPONSE_CRC_FAILURE, NULL, 0);
}

void handlePageReadCommand(ts_channel_s *tsChannel, ts_response_format_e mode, uint16_t pageId, uint16_t offset, uint16_t count) {
	tsState.readPageCommandsCounter++;
	currentPageId = pageId;

#if EFI_TUNER_STUDIO_VERBOSE
	scheduleMsg(tsLogger, "READ m=%d p=%d o=%d c=%d", mode, (int) currentPageId, offset,
			count);
	printTsStats();
#endif

	if (currentPageId >= PAGE_COUNT) {
		// something is not right here
		currentPageId = 0;
		tunerStudioError("ERROR: invalid page number");
		return;
	}

	int size = getTunerStudioPageSize(currentPageId);

	if (size < offset + count) {
		scheduleMsg(tsLogger, "invalid offset/count %d/%d", offset, count);
		sendErrorCode(tsChannel);
		return;
	}

	const uint8_t *addr = (const uint8_t *) (getWorkingPageAddr(currentPageId) + offset);
	tsSendResponse(tsChannel, mode, addr, count);
#if EFI_TUNER_STUDIO_VERBOSE
	scheduleMsg(tsLogger, "Sending %d done", count);
#endif
}

void requestBurn(void) {
#if EFI_INTERNAL_FLASH
	setNeedToWriteConfiguration();
#endif
	incrementGlobalConfigurationVersion();
}

/**
 * 'Burn' command is a command to commit the changes
 */
void handleBurnCommand(ts_channel_s *tsChannel, ts_response_format_e mode, uint16_t page) {
	efitimems_t nowMs = currentTimeMillis();
	tsState.burnCommandCounter++;

	tunerStudioDebug("got B (Burn)");

	currentPageId = page;

#if EFI_TUNER_STUDIO_VERBOSE
	// pointless since we only have one page now
//	scheduleMsg(logger, "Page number %d", currentPageId);
#endif

// todo: how about some multi-threading?
	memcpy(&persistentState.persistentConfiguration, &configWorkingCopy, sizeof(persistent_config_s));

	requestBurn();
	tunerStudioWriteCrcPacket(tsChannel, TS_RESPONSE_BURN_OK, NULL, 0);
	scheduleMsg(tsLogger, "BURN in %dms", currentTimeMillis() - nowMs);
}

static TunerStudioReadRequest readRequest;
static TunerStudioWriteChunkRequest writeChunkRequest;
static short int pageIn;

static bool isKnownCommand(char command) {
	return command == TS_HELLO_COMMAND || command == TS_READ_COMMAND || command == TS_OUTPUT_COMMAND
			|| command == TS_PAGE_COMMAND || command == TS_BURN_COMMAND || command == TS_SINGLE_WRITE_COMMAND
			|| command == TS_LEGACY_HELLO_COMMAND || command == TS_CHUNK_WRITE_COMMAND
			|| command == TS_EXECUTE || command == TS_GET_TEXT;
}

static uint8_t firstByte;
static uint8_t secondByte;

void runBinaryProtocolLoop(ts_channel_s *tsChannel) {
	int wasReady = false;
	while (true) {
		int isReady = ts_serial_ready();
		if (!isReady) {
			chThdSleepMilliseconds(10);
			wasReady = false;
			continue;
		}

		if (!wasReady) {
			wasReady = true;
//			scheduleSimpleMsg(&logger, "ts channel is now ready ", hTimeNow());
		}

		tsState.tsCounter++;

		int recieved = chSequentialStreamRead(tsChannel->channel, &firstByte, 1);
		if (recieved != 1) {
			tunerStudioError("ERROR: no command");
			continue;
		}
//		scheduleMsg(logger, "Got first=%x=[%c]", firstByte, firstByte);
		if (handlePlainCommand(tsChannel, firstByte))
			continue;

		recieved = chSequentialStreamRead(tsChannel->channel, &secondByte, 1);
		if (recieved != 1) {
			tunerStudioError("ERROR: no second");
			continue;
		}
//		scheduleMsg(logger, "Got secondByte=%x=[%c]", secondByte, secondByte);

		uint32_t incomingPacketSize = firstByte * 256 + secondByte;

		if(incomingPacketSize==BINARY_SWITCH_TAG) {
			// we are here if we get a binary switch request while already in binary mode. We will just ignore it.
			continue;
		}

		if (incomingPacketSize == 0 || incomingPacketSize > (sizeof(tsChannel->crcReadBuffer) - CRC_WRAPPING_SIZE)) {
			scheduleMsg(tsLogger, "TunerStudio: invalid size: %d", incomingPacketSize);
			tunerStudioError("ERROR: CRC header size");
			sendErrorCode(tsChannel);
			continue;
		}

		recieved = chnReadTimeout(tsChannel->channel, (uint8_t*)tsChannel->crcReadBuffer, 1, MS2ST(TS_READ_TIMEOUT));
		if (recieved != 1) {
			tunerStudioError("ERROR: did not receive command");
			continue;
		}

		char command = tsChannel->crcReadBuffer[0];
		if (!isKnownCommand(command)) {
			scheduleMsg(tsLogger, "unexpected command %x", command);
			sendErrorCode(tsChannel);
			continue;
		}

//		scheduleMsg(logger, "TunerStudio: reading %d+4 bytes(s)", incomingPacketSize);

		recieved = chnReadTimeout(tsChannel->channel, (uint8_t * ) (tsChannel->crcReadBuffer + 1),
				incomingPacketSize + CRC_VALUE_SIZE - 1, MS2ST(TS_READ_TIMEOUT));
		int expectedSize = incomingPacketSize + CRC_VALUE_SIZE - 1;
		if (recieved != expectedSize) {
			scheduleMsg(tsLogger, "got ONLY %d for packet size %d/%d for command %c", recieved, incomingPacketSize,
					expectedSize, command);
			tunerStudioError("ERROR: not enough");
			continue;
		}

		uint32_t expectedCrc = *(uint32_t*) (tsChannel->crcReadBuffer + incomingPacketSize);

		expectedCrc = SWAP_UINT32(expectedCrc);

		uint32_t actualCrc = crc32(tsChannel->crcReadBuffer, incomingPacketSize);
		if (actualCrc != expectedCrc) {
			scheduleMsg(tsLogger, "TunerStudio: CRC %x %x %x %x", tsChannel->crcReadBuffer[incomingPacketSize + 0],
					tsChannel->crcReadBuffer[incomingPacketSize + 1], tsChannel->crcReadBuffer[incomingPacketSize + 2],
					tsChannel->crcReadBuffer[incomingPacketSize + 3]);

			scheduleMsg(tsLogger, "TunerStudio: command %c actual CRC %x/expected %x", tsChannel->crcReadBuffer[0], actualCrc,
					expectedCrc);
			tunerStudioError("ERROR: CRC issue");
			continue;
		}

//		scheduleMsg(logger, "TunerStudio: P00-07 %x %x %x %x %x %x %x %x", crcIoBuffer[0], crcIoBuffer[1],
//				crcIoBuffer[2], crcIoBuffer[3], crcIoBuffer[4], crcIoBuffer[5], crcIoBuffer[6], crcIoBuffer[7]);

		int success = tunerStudioHandleCrcCommand(tsChannel, tsChannel->crcReadBuffer, incomingPacketSize);
		if (!success)
			print("got unexpected TunerStudio command %x:%c\r\n", command, command);

	}
}

static msg_t tsThreadEntryPoint(void *arg) {
	(void) arg;
	chRegSetThreadName("tunerstudio thread");

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	startTsPort();
#endif

	runBinaryProtocolLoop(&tsChannel);

#if defined __GNUC__
	return 0;
#endif
}

void syncTunerStudioCopy(void) {
	memcpy(&configWorkingCopy, &persistentState.persistentConfiguration, sizeof(persistent_config_s));
}

tunerstudio_counters_s tsState;
TunerStudioOutputChannels tsOutputChannels;
/**
 * this is a local copy of the configuration. Any changes to this copy
 * have no effect until this copy is explicitly propagated to the main working copy
 */
persistent_config_s configWorkingCopy;

short currentPageId;

void tunerStudioError(const char *msg) {
	tunerStudioDebug(msg);
	tsState.errorCounter++;
}

/**
 * Query with CRC takes place while re-establishing connection
 * Query without CRC takes place on TunerStudio startup
 */
void handleQueryCommand(ts_channel_s *tsChannel, ts_response_format_e mode) {
	tsState.queryCommandCounter++;
#if EFI_TUNER_STUDIO_VERBOSE
	scheduleMsg(tsLogger, "got S/H (queryCommand) mode=%d", mode);
	printTsStats();
#endif
	tsSendResponse(tsChannel, mode, (const uint8_t *) TS_SIGNATURE, strlen(TS_SIGNATURE) + 1);
}

/**
 * @brief 'Output' command sends out a snapshot of current values
 */
void handleOutputChannelsCommand(ts_channel_s *tsChannel, ts_response_format_e mode) {
	tsState.outputChannelsCommandCounter++;
	// this method is invoked too often to print any debug information
	tsSendResponse(tsChannel, mode, (const uint8_t *) &tsOutputChannels, sizeof(TunerStudioOutputChannels));
}

void handleTestCommand(ts_channel_s *tsChannel) {
	/**
	 * this is NOT a standard TunerStudio command, this is my own
	 * extension of the protocol to simplify troubleshooting
	 */
	tunerStudioDebug("got T (Test)");
	tunerStudioWriteData(tsChannel, (const uint8_t *)VCS_VERSION, sizeof(VCS_VERSION));
	/**
	 * Please note that this response is a magic constant used by dev console for protocol detection
	 * @see EngineState#TS_PROTOCOL_TAG
	 */
	tunerStudioWriteData(tsChannel, (const uint8_t *) " ts_p_alive\r\n", 8);
}

extern CommandHandler console_line_callback;

static void handleGetText(ts_channel_s *tsChannel) {
	int outputSize;
	char *output = swapOutputBuffers(&outputSize);

	tunerStudioWriteCrcPacket(tsChannel, TS_RESPONSE_COMMAND_OK, output, outputSize);
}

static void handleExecuteCommand(ts_channel_s *tsChannel, char *data, int incomingPacketSize) {
	tunerStudioWriteCrcPacket(tsChannel, TS_RESPONSE_COMMAND_OK, NULL, 0);
	data[incomingPacketSize] = 0;
	(console_line_callback)(data);
}

/**
 * @return true if legacy command was processed, false otherwise
 */
bool handlePlainCommand(ts_channel_s *tsChannel, uint8_t command) {
	if (command == TS_HELLO_COMMAND || command == TS_HELLO_COMMAND_DEPRECATED) {
		scheduleMsg(tsLogger, "Got naked Query command");
		handleQueryCommand(tsChannel, TS_PLAIN);
		return true;
	} else if (command == 't' || command == 'T') {
		handleTestCommand(tsChannel);
		return true;
	} else if (command == TS_PAGE_COMMAND) {
		int recieved = chSequentialStreamRead(tsChannel->channel, (uint8_t * )&pageIn, sizeof(pageIn));
		// todo: validate 'recieved' value
		handlePageSelectCommand(tsChannel, TS_PLAIN, pageIn);
		return true;
	} else if (command == TS_BURN_COMMAND) {
		scheduleMsg(tsLogger, "Got naked BURN");
		uint16_t page;
		int recieved = chSequentialStreamRead(tsChannel->channel, (uint8_t * )&page,
				sizeof(page));
		if (recieved != sizeof(page)) {
			tunerStudioError("ERROR: Not enough for plain burn");
			return true;
		}
		handleBurnCommand(tsChannel, TS_PLAIN, page);
		return true;
	} else if (command == TS_CHUNK_WRITE_COMMAND) {
		scheduleMsg(tsLogger, "Got naked CHUNK_WRITE");
		int recieved = chSequentialStreamRead(tsChannel->channel, (uint8_t * )&writeChunkRequest,
				sizeof(writeChunkRequest));
		if (recieved != sizeof(writeChunkRequest)) {
			scheduleMsg(tsLogger, "ERROR: Not enough for plain chunk write header: %d", recieved);
			tsState.errorCounter++;
			return true;
		}
		recieved = chSequentialStreamRead(tsChannel->channel, (uint8_t * )&tsChannel->crcReadBuffer, writeChunkRequest.count);
		if (recieved != writeChunkRequest.count) {
			scheduleMsg(tsLogger, "ERROR: Not enough for plain chunk write content: %d while expecting %d", recieved, writeChunkRequest.count);
			tsState.errorCounter++;
			return true;
		}
		currentPageId = writeChunkRequest.page;

		handleWriteChunkCommand(tsChannel, TS_PLAIN, writeChunkRequest.offset, writeChunkRequest.count, (uint8_t * )&tsChannel->crcReadBuffer);
		return true;
	} else if (command == TS_READ_COMMAND) {
		//scheduleMsg(logger, "Got naked READ PAGE???");
		int recieved = chSequentialStreamRead(tsChannel->channel, (uint8_t * )&readRequest, sizeof(readRequest));
		if (recieved != sizeof(readRequest)) {
			tunerStudioError("Not enough for plain read header");
			return true;
		}
		handlePageReadCommand(tsChannel, TS_PLAIN, readRequest.page, readRequest.offset, readRequest.count);
		return true;
	} else if (command == TS_OUTPUT_COMMAND) {
		//scheduleMsg(logger, "Got naked Channels???");
		handleOutputChannelsCommand(tsChannel, TS_PLAIN);
		return true;
	} else if (command == TS_LEGACY_HELLO_COMMAND) {
		tunerStudioDebug("ignoring LEGACY_HELLO_COMMAND");
		return true;
	} else if (command == TS_COMMAND_F) {
		tunerStudioDebug("not ignoring F");
		tunerStudioWriteData(tsChannel, (const uint8_t *) PROTOCOL, strlen(PROTOCOL));
		return true;
	} else {
		return false;
	}
}

int tunerStudioHandleCrcCommand(ts_channel_s *tsChannel, char *data, int incomingPacketSize) {
	char command = data[0];
	data++;
	if (command == TS_HELLO_COMMAND || command == TS_HELLO_COMMAND_DEPRECATED) {
		tunerStudioDebug("got Query command");
		handleQueryCommand(tsChannel, TS_CRC);
	} else if (command == TS_GET_TEXT) {
		handleGetText(tsChannel);
	} else if (command == TS_EXECUTE) {
		handleExecuteCommand(tsChannel, data, incomingPacketSize);
	} else if (command == TS_OUTPUT_COMMAND) {
		handleOutputChannelsCommand(tsChannel, TS_CRC);
	} else if (command == TS_PAGE_COMMAND) {
		uint16_t page = *(uint16_t *) data;
		handlePageSelectCommand(tsChannel, TS_CRC, page);
	} else if (command == TS_CHUNK_WRITE_COMMAND) {
		currentPageId = *(uint16_t *) data;
		uint16_t offset = *(uint16_t *) (data + 2);
		uint16_t count = *(uint16_t *) (data + 4);
		handleWriteChunkCommand(tsChannel, TS_CRC, offset, count, data + sizeof(TunerStudioWriteChunkRequest));
	} else if (command == TS_SINGLE_WRITE_COMMAND) {
		uint16_t page = *(uint16_t *) data;
		uint16_t offset = *(uint16_t *) (data + 2);
		uint8_t value = data[4];
		handleWriteValueCommand(tsChannel, TS_CRC, page, offset, value);
	} else if (command == TS_BURN_COMMAND) {
		uint16_t page = *(uint16_t *) data;
		handleBurnCommand(tsChannel, TS_CRC, page);
	} else if (command == TS_READ_COMMAND) {
		uint16_t page = *(uint16_t *) data;
		uint16_t offset = *(uint16_t *) (data + 2);
		uint16_t count = *(uint16_t *) (data + 4);
		handlePageReadCommand(tsChannel, TS_CRC, page, offset, count);
	} else if (command == 't' || command == 'T') {
		handleTestCommand(tsChannel);
	} else if (command == TS_LEGACY_HELLO_COMMAND) {
		/**
		 * 'Q' is the query command used for compatibility with older ECUs
		 */
		tunerStudioDebug("ignoring Q");
	} else if (command == TS_COMMAND_F) {
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
		return false;
	}
	return true;
}

/**
 * we use 'blockingFactor = 256' in rusefi.ini
 * todo: should we just do (256 + CRC_WRAPPING_SIZE) ?
 */

static uint8_t tsCrcWriteBuffer[300];

void startTunerStudioConnectivity(Logging *sharedLogger) {
	tsLogger = sharedLogger;

	if (sizeof(engine_configuration_s) != getTunerStudioPageSize(0))
		firmwareError("TS page size mismatch: %d/%d", sizeof(engine_configuration_s), getTunerStudioPageSize(0));

	if (sizeof(TunerStudioOutputChannels) != TS_OUTPUT_SIZE)
		firmwareError("TS outputs size mismatch: %d/%d", sizeof(TunerStudioOutputChannels), TS_OUTPUT_SIZE);

	memset(&tsState, 0, sizeof(tsState));
	syncTunerStudioCopy();

	addConsoleAction("tsinfo", printTsStats);
	addConsoleAction("reset_ts", resetTs);
	addConsoleActionI("set_ts_speed", setTsSpeed);

	tsChannel.channel = getTsSerialDevice();
	tsChannel.writeBuffer = tsCrcWriteBuffer;

	chThdCreateStatic(tsThreadStack, sizeof(tsThreadStack), NORMALPRIO, tsThreadEntryPoint, NULL);
}

#endif
