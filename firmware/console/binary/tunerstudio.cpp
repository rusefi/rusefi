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
 * 			One of the known ECU id strings.
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
 *
 * @date Oct 22, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
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
#include "fl_protocol.h"
#include "bluetooth.h"

#include <string.h>
#include "engine_configuration.h"
#include "injector_central.h"
#include "svnversion.h"
#include "loggingcentral.h"
#include "status_loop.h"
#include "mmc_card.h"
#if EFI_SIMULATOR || defined(__DOXYGEN__)
#include "rusEfiFunctionalTest.h"
#endif

#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)

EXTERN_ENGINE
;

extern persistent_config_container_s persistentState;

extern short currentPageId;

/**
 * note the use-case where text console port is switched into
 * binary port
 */
LoggingWithStorage tsLogger("binary");

/**
 * this is a local copy of the configuration. Any changes to this copy
 * have no effect until this copy is explicitly propagated to the main working copy
 */
persistent_config_s configWorkingCopy;

extern persistent_config_container_s persistentState;

static efitimems_t previousWriteReportMs = 0;

static ts_channel_s tsChannel;

static uint16_t BINARY_RESPONSE = (uint16_t) SWAP_UINT16(BINARY_SWITCH_TAG);

// this thread wants a bit extra stack
static THD_WORKING_AREA(tsThreadStack, 3 * UTILITY_THREAD_STACK_SIZE);

extern TunerStudioOutputChannels tsOutputChannels;

extern tunerstudio_counters_s tsState;

static void resetTs(void) {
	memset(&tsState, 0, sizeof(tsState));
}

static void printErrorCounters(void) {
	scheduleMsg(&tsLogger, "TunerStudio size=%d / total=%d / errors=%d / H=%d / O=%d / P=%d / B=%d",
			sizeof(tsOutputChannels), tsState.totalCounter, tsState.errorCounter, tsState.queryCommandCounter,
			tsState.outputChannelsCommandCounter, tsState.readPageCommandsCounter, tsState.burnCommandCounter);
	scheduleMsg(&tsLogger, "TunerStudio W=%d / C=%d / P=%d / page=%d", tsState.writeValueCommandCounter,
			tsState.writeChunkCommandCounter, tsState.pageCommandCounter, currentPageId);
//	scheduleMsg(&tsLogger, "page size=%d", getTunerStudioPageSize(currentPageId));
}

void printTsStats(void) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	if (!isCommandLineConsoleOverTTL()) {
		scheduleMsg(&tsLogger, "TS RX on %s", hwPortname(engineConfiguration->binarySerialRxPin));

		scheduleMsg(&tsLogger, "TS TX on %s @%d", hwPortname(engineConfiguration->binarySerialTxPin),
				boardConfiguration->tunerStudioSerialSpeed);
	}
#endif /* EFI_PROD_CODE */

	printErrorCounters();

//	scheduleMsg(logger, "analogChartFrequency %d",
//			(int) (&engineConfiguration->analogChartFrequency) - (int) engineConfiguration);
//
//	int fuelMapOffset = (int) (&engineConfiguration->fuelTable) - (int) engineConfiguration;
//	scheduleMsg(logger, "fuelTable %d", fuelMapOffset);
//
//	int offset = (int) (&boardConfiguration->hip9011Gain) - (int) engineConfiguration;
//	scheduleMsg(&tsLogger, "hip9011Gain %d", offset);
//
//	offset = (int) (&engineConfiguration->crankingCycleBins) - (int) engineConfiguration;
//	scheduleMsg(&tsLogger, "crankingCycleBins %d", offset);
//
//	offset = (int) (&engineConfiguration->engineCycle) - (int) engineConfiguration;
//	scheduleMsg(&tsLogger, "engineCycle %d", offset);
}

static void setTsSpeed(int value) {
	boardConfiguration->tunerStudioSerialSpeed = value;
	printTsStats();
}

#if EFI_BLUETOOTH_SETUP || defined(__DOXYGEN__)
// Bluetooth HC-05 module initialization start (it waits for disconnect and then communicates to the module)
static void bluetoothHC05(const char *baudRate, const char *name, const char *pinCode) {
	bluetoothStart(&tsChannel, BLUETOOTH_HC_05, baudRate, name, pinCode);
}
// Bluetooth HC-06 module initialization start (it waits for disconnect and then communicates to the module)
static void bluetoothHC06(const char *baudRate, const char *name, const char *pinCode) {
	bluetoothStart(&tsChannel, BLUETOOTH_HC_06, baudRate, name, pinCode);
}
// Bluetooth SPP-C module initialization start (it waits for disconnect and then communicates to the module)
static void bluetoothSPP(const char *baudRate, const char *name, const char *pinCode) {
	bluetoothStart(&tsChannel, BLUETOOTH_SPP, baudRate, name, pinCode);
}
#endif  /* EFI_BLUETOOTH_SETUP */

void tunerStudioDebug(const char *msg) {
#if EFI_TUNER_STUDIO_VERBOSE || defined(__DOXYGEN__)
	scheduleMsg(&tsLogger, "%s", msg);
#endif
}

char *getWorkingPageAddr(int pageIndex) {
	switch (pageIndex) {
	case 0:
		return (char*) &configWorkingCopy.engineConfiguration;
//	case 1:
//		return (char*) &configWorkingCopy.ve2Table;
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
		return TOTAL_CONFIG_SIZE;
//	case 1:
//	case 2:
//	case 3:
//	case 4:
//	case 5:
//	case 6:
//		return PAGE_1_SIZE;
//	case 2:
//	case 3:
//	case 4:
//		return 1024;
	}
	return 0;
}

static void sendOkResponse(ts_channel_s *tsChannel, ts_response_format_e mode) {
	sr5SendResponse(tsChannel, mode, NULL, 0);
}

void handlePageSelectCommand(ts_channel_s *tsChannel, ts_response_format_e mode, uint16_t pageId) {
	tsState.pageCommandCounter++;

	currentPageId = pageId;
	scheduleMsg(&tsLogger, "PAGE %d", currentPageId);
	sendOkResponse(tsChannel, mode);
}

/**
 * Some changes like changing VE table or timing table are applied right away, meaning
 * that the values are copied from communication copy into actual engine control copy right away.
 * We call these parameters 'soft parameters'
 *
 * This is needed to support TS online auto-tune.
 *
 * On the contrary, 'hard parameters' are waiting for the Burn button to be clicked and configuration version
 * would be increased and much more complicated logic would be executed.
 */
static void onlineTuneBytes(int currentPageId, uint32_t offset, int count) {
	UNUSED(currentPageId);
	if (offset > sizeof(engine_configuration_s)) {
		int maxSize = sizeof(persistent_config_s) - offset;
		if (count > maxSize) {
			warning(CUSTOM_TS_OVERFLOW, "TS overflow %d %d", offset, count);
			return;
		}
		scheduleMsg(&tsLogger, "applying soft change from %d length %d", offset, count);
		memcpy(((char*) &persistentState.persistentConfiguration) + offset, ((char*) &configWorkingCopy) + offset,
				count);
	}
}

/**
 * read log file content for rusEfi console
 */
static void handleReadFileContent(ts_channel_s *tsChannel, short fileId, short offset, short length) {
#if EFI_FILE_LOGGING || defined(__DOXYGEN__)
	readLogFileContent(tsChannel->crcReadBuffer, fileId, offset, length);
#endif /* EFI_FILE_LOGGING */
}

/**
 * This command is needed to make the whole transfer a bit faster
 * @note See also handleWriteValueCommand
 */
void handleWriteChunkCommand(ts_channel_s *tsChannel, ts_response_format_e mode, short offset, short count,
		void *content) {
	tsState.writeChunkCommandCounter++;

	scheduleMsg(&tsLogger, "WRITE CHUNK mode=%d p=%d o=%d s=%d", mode, currentPageId, offset, count);

	if (offset > getTunerStudioPageSize(currentPageId)) {
		scheduleMsg(&tsLogger, "ERROR invalid offset %d", offset);
		tunerStudioError("ERROR: out of range");
		offset = 0;
	}

	if (count > getTunerStudioPageSize(currentPageId)) {
		tunerStudioError("ERROR: unexpected count");
		scheduleMsg(&tsLogger, "ERROR unexpected count %d", count);
		count = 0;
	}

	uint8_t * addr = (uint8_t *) (getWorkingPageAddr(currentPageId) + offset);
	memcpy(addr, content, count);
	onlineTuneBytes(currentPageId, offset, count);

	sendOkResponse(tsChannel, mode);
}

void handleCrc32Check(ts_channel_s *tsChannel, ts_response_format_e mode, uint16_t pageId, uint16_t offset,
		uint16_t count) {
	tsState.crc32CheckCommandCounter++;

	count = SWAP_UINT16(count);

	count = getTunerStudioPageSize(pageId);

	scheduleMsg(&tsLogger, "CRC32 request: pageId %d offset %d size %d", pageId, offset, count);

	uint32_t crc = SWAP_UINT32(crc32((void * ) getWorkingPageAddr(0), count));

	scheduleMsg(&tsLogger, "CRC32 response: %x", crc);

	sr5SendResponse(tsChannel, mode, (const uint8_t *) &crc, 4);
}

/**
 * 'Write' command receives a single value at a given offset
 * @note Writing values one by one is pretty slow
 */
void handleWriteValueCommand(ts_channel_s *tsChannel, ts_response_format_e mode, uint16_t page, uint16_t offset,
		uint8_t value) {
	UNUSED(tsChannel);
	UNUSED(mode);
	tsState.writeValueCommandCounter++;

	currentPageId = page;

	tunerStudioDebug("got W (Write)"); // we can get a lot of these

#if EFI_TUNER_STUDIO_VERBOSE || defined(__DOXYGEN__)
//	scheduleMsg(logger, "Page number %d\r\n", pageId); // we can get a lot of these
#endif

//	int size = sizeof(TunerStudioWriteValueRequest);
//	scheduleMsg(logger, "Reading %d\r\n", size);

	if (offset > getTunerStudioPageSize(currentPageId)) {
		tunerStudioError("ERROR: out of range2");
		scheduleMsg(&tsLogger, "ERROR offset %d", offset);
		offset = 0;
		return;
	}

	efitimems_t nowMs = currentTimeMillis();
	if (nowMs - previousWriteReportMs > 5) {
		previousWriteReportMs = nowMs;
		scheduleMsg(&tsLogger, "page %d offset %d: value=%d", currentPageId, offset, value);
	}

	getWorkingPageAddr(currentPageId)[offset] = value;

	onlineTuneBytes(currentPageId, offset, 1);

//	scheduleMsg(logger, "va=%d", configWorkingCopy.boardConfiguration.idleValvePin);
}

static void sendErrorCode(ts_channel_s *tsChannel) {
	sr5WriteCrcPacket(tsChannel, TS_RESPONSE_CRC_FAILURE, NULL, 0);
}

void handlePageReadCommand(ts_channel_s *tsChannel, ts_response_format_e mode, uint16_t pageId, uint16_t offset,
		uint16_t count) {
	tsState.readPageCommandsCounter++;
	currentPageId = pageId;

#if EFI_TUNER_STUDIO_VERBOSE || defined(__DOXYGEN__)
	scheduleMsg(&tsLogger, "READ mode=%d page=%d offset=%d size=%d", mode, (int) currentPageId, offset, count);
#endif

	if (currentPageId >= PAGE_COUNT) {
		// something is not right here
		currentPageId = 0;
		tunerStudioError("ERROR: invalid page number");
		return;
	}

	int size = getTunerStudioPageSize(currentPageId);

	if (size < offset + count) {
		scheduleMsg(&tsLogger, "invalid offset/count %d/%d", offset, count);
		sendErrorCode(tsChannel);
		return;
	}

	const uint8_t *addr = (const uint8_t *) (getWorkingPageAddr(currentPageId) + offset);
	sr5SendResponse(tsChannel, mode, addr, count);
#if EFI_TUNER_STUDIO_VERBOSE || defined(__DOXYGEN__)
//	scheduleMsg(&tsLogger, "Sending %d done", count);
#endif
}

void requestBurn(void) {
#if EFI_INTERNAL_FLASH || defined(__DOXYGEN__)
	setNeedToWriteConfiguration();
#endif
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
}

static void sendResponseCode(ts_response_format_e mode, ts_channel_s *tsChannel, const uint8_t responseCode) {
	if (mode == TS_CRC) {
		sr5WriteCrcPacket(tsChannel, responseCode, NULL, 0);
	}
}

/**
 * 'Burn' command is a command to commit the changes
 */
void handleBurnCommand(ts_channel_s *tsChannel, ts_response_format_e mode, uint16_t page) {
	efitimems_t nowMs = currentTimeMillis();
	tsState.burnCommandCounter++;

	scheduleMsg(&tsLogger, "got B (Burn) %s", mode == TS_PLAIN ? "plain" : "CRC");

	currentPageId = page;

#if EFI_TUNER_STUDIO_VERBOSE || defined(__DOXYGEN__)
	// pointless since we only have one page now
//	scheduleMsg(logger, "Page number %d", currentPageId);
#endif

// todo: how about some multi-threading?
	memcpy(&persistentState.persistentConfiguration, &configWorkingCopy, sizeof(persistent_config_s));

	requestBurn();
	sendResponseCode(mode, tsChannel, TS_RESPONSE_BURN_OK);
	scheduleMsg(&tsLogger, "BURN in %dms", currentTimeMillis() - nowMs);
}

static TunerStudioReadRequest readRequest;
static TunerStudioWriteChunkRequest writeChunkRequest;
static TunerStudioOchRequest ochRequest;
static short int pageIn;

static bool isKnownCommand(char command) {
	return command == TS_HELLO_COMMAND || command == TS_READ_COMMAND || command == TS_OUTPUT_COMMAND
			|| command == TS_PAGE_COMMAND || command == TS_BURN_COMMAND || command == TS_SINGLE_WRITE_COMMAND
			|| command == TS_LEGACY_HELLO_COMMAND || command == TS_CHUNK_WRITE_COMMAND || command == TS_EXECUTE
			|| command == TS_IO_TEST_COMMAND
			|| command == TS_GET_FILE_RANGE
			|| command == TS_TOOTH_COMMAND
			|| command == TS_GET_TEXT || command == TS_CRC_CHECK_COMMAND
			|| command == TS_GET_FIRMWARE_VERSION;
}


void runBinaryProtocolLoop(ts_channel_s *tsChannel, bool isConsoleRedirect) {
	int wasReady = false;

	bool isFirstByte = true;

	while (true) {
		int isReady = sr5IsReady(isConsoleRedirect);
		if (!isReady) {
			chThdSleepMilliseconds(10);
			wasReady = false;
			continue;
		}

		if (!wasReady) {
			wasReady = true;
//			scheduleSimpleMsg(&logger, "ts channel is now ready ", hTimeNow());
		}

		tsState.totalCounter++;

		uint8_t firstByte;
		int received = sr5ReadData(tsChannel, &firstByte, 1);
#if EFI_SIMULATOR || defined(__DOXYGEN__)
			logMsg("received %d\r\n", received);
#endif


		if (received != 1) {
//			tunerStudioError("ERROR: no command");
#if EFI_BLUETOOTH_SETUP || defined(__DOXYGEN__)
			// assume there's connection loss and notify the bluetooth init code
			bluetoothSoftwareDisconnectNotify();
#endif  /* EFI_BLUETOOTH_SETUP */
			continue;
		}
		onDataArrived();

		if (isFirstByte) {
			if (isStartOfFLProtocol(firstByte)) {

			}

		}
		isFirstByte = false;

//		scheduleMsg(logger, "Got first=%x=[%c]", firstByte, firstByte);
		if (handlePlainCommand(tsChannel, firstByte))
			continue;

		uint8_t secondByte;
		received = sr5ReadData(tsChannel, &secondByte, 1);
		if (received != 1) {
			tunerStudioError("TS: ERROR: no second byte");
			continue;
		}
//		scheduleMsg(logger, "Got secondByte=%x=[%c]", secondByte, secondByte);

		uint32_t incomingPacketSize = firstByte * 256 + secondByte;

		if (incomingPacketSize == BINARY_SWITCH_TAG) {
			// we are here if we get a binary switch request while already in binary mode. We will just ignore it.
			sr5WriteData(tsChannel, (const uint8_t *) &BINARY_RESPONSE, 2);
			continue;
		}

		if (incomingPacketSize == 0 || incomingPacketSize > (sizeof(tsChannel->crcReadBuffer) - CRC_WRAPPING_SIZE)) {
			scheduleMsg(&tsLogger, "TunerStudio: invalid size: %d", incomingPacketSize);
			tunerStudioError("ERROR: CRC header size");
			sendErrorCode(tsChannel);
			continue;
		}

		received = sr5ReadData(tsChannel, (uint8_t* )tsChannel->crcReadBuffer, 1);
		if (received != 1) {
			tunerStudioError("ERROR: did not receive command");
			continue;
		}

		char command = tsChannel->crcReadBuffer[0];
		if (!isKnownCommand(command)) {
			scheduleMsg(&tsLogger, "unexpected command %x", command);
			sendErrorCode(tsChannel);
			continue;
		}

#if EFI_SIMULATOR || defined(__DOXYGEN__)
			logMsg("command %c\r\n", command);
#endif


//		scheduleMsg(logger, "TunerStudio: reading %d+4 bytes(s)", incomingPacketSize);

		received = sr5ReadData(tsChannel, (uint8_t * ) (tsChannel->crcReadBuffer + 1),
				incomingPacketSize + CRC_VALUE_SIZE - 1);
		int expectedSize = incomingPacketSize + CRC_VALUE_SIZE - 1;
		if (received != expectedSize) {
			scheduleMsg(&tsLogger, "Got only %d bytes while expecting %d for command %c", received,
					expectedSize, command);
			tunerStudioError("ERROR: not enough bytes in stream");
			sendResponseCode(TS_CRC, tsChannel, TS_RESPONSE_UNDERRUN);
			continue;
		}

		uint32_t expectedCrc = *(uint32_t*) (tsChannel->crcReadBuffer + incomingPacketSize);

		expectedCrc = SWAP_UINT32(expectedCrc);

		uint32_t actualCrc = crc32(tsChannel->crcReadBuffer, incomingPacketSize);
		if (actualCrc != expectedCrc) {
			scheduleMsg(&tsLogger, "TunerStudio: CRC %x %x %x %x", tsChannel->crcReadBuffer[incomingPacketSize + 0],
					tsChannel->crcReadBuffer[incomingPacketSize + 1], tsChannel->crcReadBuffer[incomingPacketSize + 2],
					tsChannel->crcReadBuffer[incomingPacketSize + 3]);

			scheduleMsg(&tsLogger, "TunerStudio: command %c actual CRC %x/expected %x", tsChannel->crcReadBuffer[0],
					actualCrc, expectedCrc);
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

static THD_FUNCTION(tsThreadEntryPoint, arg) {
	(void) arg;
	chRegSetThreadName("tunerstudio thread");

	startTsPort(&tsChannel);

	runBinaryProtocolLoop(&tsChannel, false);
}

void syncTunerStudioCopy(void) {
	memcpy(&configWorkingCopy, &persistentState.persistentConfiguration, sizeof(persistent_config_s));
}

tunerstudio_counters_s tsState;
TunerStudioOutputChannels tsOutputChannels;

short currentPageId;

void tunerStudioError(const char *msg) {
	tunerStudioDebug(msg);
	printErrorCounters();
	tsState.errorCounter++;
}

/**
 * Query with CRC takes place while re-establishing connection
 * Query without CRC takes place on TunerStudio startup
 */
void handleQueryCommand(ts_channel_s *tsChannel, ts_response_format_e mode) {
	tsState.queryCommandCounter++;
#if EFI_TUNER_STUDIO_VERBOSE || defined(__DOXYGEN__)
	scheduleMsg(&tsLogger, "got S/H (queryCommand) mode=%d", mode);
	printTsStats();
#endif
	sr5SendResponse(tsChannel, mode, (const uint8_t *) TS_SIGNATURE, strlen(TS_SIGNATURE) + 1);
}

/**
 * @brief 'Output' command sends out a snapshot of current values
 */
void handleOutputChannelsCommand(ts_channel_s *tsChannel, ts_response_format_e mode, uint16_t offset, uint16_t count) {


	if (sizeof(TunerStudioOutputChannels) < offset + count) {
		scheduleMsg(&tsLogger, "invalid offset/count %d/%d", offset, count);
		sendErrorCode(tsChannel);
		return;
	}


	tsState.outputChannelsCommandCounter++;
	prepareTunerStudioOutputs();
	// this method is invoked too often to print any debug information
	sr5SendResponse(tsChannel, mode, ((const uint8_t *) &tsOutputChannels) + offset, count);
}

#define TEST_RESPONSE_TAG " ts_p_alive\r\n"

void handleTestCommand(ts_channel_s *tsChannel) {
	tsState.testCommandCounter++;
	static char testOutputBuffer[12];
	/**
	 * this is NOT a standard TunerStudio command, this is my own
	 * extension of the protocol to simplify troubleshooting
	 */
	tunerStudioDebug("got T (Test)");
	sr5WriteData(tsChannel, (const uint8_t *) VCS_VERSION, sizeof(VCS_VERSION));
	chsnprintf(testOutputBuffer, sizeof(testOutputBuffer), " %d %d", engine->engineState.lastErrorCode, tsState.testCommandCounter);
	sr5WriteData(tsChannel, (const uint8_t *) testOutputBuffer, strlen(testOutputBuffer));
	/**
	 * Please note that this response is a magic constant used by dev console for protocol detection
	 * @see EngineState#TS_PROTOCOL_TAG
	 */
	sr5WriteData(tsChannel, (const uint8_t *) TEST_RESPONSE_TAG, sizeof(TEST_RESPONSE_TAG));
}

extern CommandHandler console_line_callback;

static void handleGetVersion(ts_channel_s *tsChannel, ts_response_format_e mode) {
	static char versionBuffer[32];
	chsnprintf(versionBuffer, sizeof(versionBuffer), "rusEFI v%d@%s", getRusEfiVersion(), VCS_VERSION);
	sr5SendResponse(tsChannel, mode, (const uint8_t *) versionBuffer, strlen(versionBuffer) + 1);
}

static void handleGetText(ts_channel_s *tsChannel) {
	tsState.textCommandCounter++;

	printOverallStatus(getTimeNowSeconds());

	int outputSize;
	char *output = swapOutputBuffers(&outputSize);
#if EFI_SIMULATOR || defined(__DOXYGEN__)
			logMsg("get test sending [%d]\r\n", outputSize);
#endif

	sr5WriteCrcPacket(tsChannel, TS_RESPONSE_COMMAND_OK, output, outputSize);
#if EFI_SIMULATOR || defined(__DOXYGEN__)
			logMsg("sent [%d]\r\n", outputSize);
#endif
}

static void handleExecuteCommand(ts_channel_s *tsChannel, char *data, int incomingPacketSize) {
	sr5WriteCrcPacket(tsChannel, TS_RESPONSE_COMMAND_OK, NULL, 0);
	data[incomingPacketSize] = 0;
	char *trimmed = efiTrim(data);
#if EFI_SIMULATOR || defined(__DOXYGEN__)
			logMsg("execute [%s]\r\n", trimmed);
#endif
	(console_line_callback)(trimmed);
}

/**
 * @return true if legacy command was processed, false otherwise
 */
bool handlePlainCommand(ts_channel_s *tsChannel, uint8_t command) {
	if (command == TS_HELLO_COMMAND || command == TS_HELLO_COMMAND_DEPRECATED) {
		scheduleMsg(&tsLogger, "Got naked Query command");
		handleQueryCommand(tsChannel, TS_PLAIN);
		return true;
	} else if (command == TS_TEST_COMMAND || command == 'T') {
		handleTestCommand(tsChannel);
		return true;
	} else if (command == TS_PAGE_COMMAND) {
		int received = sr5ReadData(tsChannel, (uint8_t * )&pageIn, sizeof(pageIn));
		if (received != sizeof(pageIn)) {
			tunerStudioError("ERROR: not enough for PAGE");
			return true;
		}
		handlePageSelectCommand(tsChannel, TS_PLAIN, pageIn);
		return true;
	} else if (command == TS_BURN_COMMAND) {
		scheduleMsg(&tsLogger, "Got naked BURN");
		uint16_t page;
		int recieved = sr5ReadData(tsChannel, (uint8_t * )&page, sizeof(page));
		if (recieved != sizeof(page)) {
			tunerStudioError("ERROR: Not enough for plain burn");
			return true;
		}
		handleBurnCommand(tsChannel, TS_PLAIN, page);
		return true;
	} else if (command == TS_CHUNK_WRITE_COMMAND) {
		scheduleMsg(&tsLogger, "Got naked CHUNK_WRITE");
		int received = sr5ReadData(tsChannel, (uint8_t * )&writeChunkRequest, sizeof(writeChunkRequest));
		if (received != sizeof(writeChunkRequest)) {
			scheduleMsg(&tsLogger, "ERROR: Not enough for plain chunk write header: %d", received);
			tsState.errorCounter++;
			return true;
		}
		received = sr5ReadData(tsChannel, (uint8_t * )&tsChannel->crcReadBuffer, writeChunkRequest.count);
		if (received != writeChunkRequest.count) {
			scheduleMsg(&tsLogger, "ERROR: Not enough for plain chunk write content: %d while expecting %d", received,
					writeChunkRequest.count);
			tsState.errorCounter++;
			return true;
		}
		currentPageId = writeChunkRequest.page;

		handleWriteChunkCommand(tsChannel, TS_PLAIN, writeChunkRequest.offset, writeChunkRequest.count,
				(uint8_t *) &tsChannel->crcReadBuffer);
		return true;
	} else if (command == TS_READ_COMMAND) {
		//scheduleMsg(logger, "Got naked READ PAGE???");
		int received = sr5ReadData(tsChannel, (uint8_t * )&readRequest, sizeof(readRequest));
		if (received != sizeof(readRequest)) {
			tunerStudioError("Not enough for plain read header");
			return true;
		}
		handlePageReadCommand(tsChannel, TS_PLAIN, readRequest.page, readRequest.offset, readRequest.count);
		return true;
	} else if (command == TS_OUTPUT_COMMAND) {
		int received = sr5ReadData(tsChannel, (uint8_t * )&ochRequest, sizeof(ochRequest));
		if (received != sizeof(ochRequest)) {
			tunerStudioError("Not enough for OutputChannelsCommand");
			return true;
		}

		//scheduleMsg(logger, "Got naked Channels???");
		handleOutputChannelsCommand(tsChannel, TS_PLAIN, ochRequest.offset, ochRequest.count);
		return true;
	} else if (command == TS_LEGACY_HELLO_COMMAND) {
		tunerStudioDebug("ignoring LEGACY_HELLO_COMMAND");
		return true;
	} else if (command == TS_COMMAND_F) {
		tunerStudioDebug("not ignoring F");
		sr5WriteData(tsChannel, (const uint8_t *) PROTOCOL, strlen(PROTOCOL));
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
	} else if (command == TS_GET_FIRMWARE_VERSION) {
		handleGetVersion(tsChannel, TS_CRC);
	} else if (command == TS_GET_TEXT) {
		handleGetText(tsChannel);
	} else if (command == TS_EXECUTE) {
		handleExecuteCommand(tsChannel, data, incomingPacketSize - 1);
	} else if (command == TS_OUTPUT_COMMAND) {
		uint16_t offset = *(uint16_t *) (data);
		uint16_t count = *(uint16_t *) (data + 2);
		handleOutputChannelsCommand(tsChannel, TS_CRC, offset, count);
	} else if (command == TS_PAGE_COMMAND) {
		uint16_t page = *(uint16_t *) data;
		handlePageSelectCommand(tsChannel, TS_CRC, page);
	} else if (command == TS_GET_FILE_RANGE) {
		short fileId = *(uint16_t *) data;
		uint16_t offset = *(uint16_t *) (data + 2);
		uint16_t length = *(uint16_t *) (data + 4);
		handleReadFileContent(tsChannel, fileId, offset, length);
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
	} else if (command == TS_CRC_CHECK_COMMAND) {
		uint16_t page = *(uint16_t *) data;
		uint16_t offset = *(uint16_t *) (data + 2);
		uint16_t count = *(uint16_t *) (data + 4);
		handleCrc32Check(tsChannel, TS_CRC, page, offset, count);
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
	} else if (command == TS_IO_TEST_COMMAND) {
		uint16_t subsystem = SWAP_UINT16(*(short*)&data[0]);
		uint16_t index = SWAP_UINT16(*(short*)&data[2]);

		if (engineConfiguration->debugMode == DBG_BENCH_TEST) {
			tsOutputChannels.debugIntField1++;
			tsOutputChannels.debugIntField2 = subsystem;
			tsOutputChannels.debugIntField3 = index;

		}

#if EFI_PROD_CODE || defined(__DOXYGEN__)
		runIoTest(subsystem, index);
#endif /* EFI_PROD_CODE */
		sendOkResponse(tsChannel, TS_CRC);
	} else {
		tunerStudioError("ERROR: ignoring unexpected command");
		return false;
	}
	return true;
}

void startTunerStudioConnectivity(void) {
	if (sizeof(persistent_config_s) != getTunerStudioPageSize(0))
		firmwareError(CUSTOM_OBD_TS_PAGE_MISMATCH, "TS page size mismatch: %d/%d", sizeof(persistent_config_s), getTunerStudioPageSize(0));

	if (sizeof(TunerStudioOutputChannels) != TS_OUTPUT_SIZE)
		firmwareError(CUSTOM_OBD_TS_OUTPUT_MISMATCH, "TS outputs size mismatch: %d/%d", sizeof(TunerStudioOutputChannels), TS_OUTPUT_SIZE);

	memset(&tsState, 0, sizeof(tsState));
	syncTunerStudioCopy();

	addConsoleAction("tsinfo", printTsStats);
	addConsoleAction("reset_ts", resetTs);
	addConsoleActionI("set_ts_speed", setTsSpeed);
	
#if EFI_BLUETOOTH_SETUP || defined(__DOXYGEN__)
	// Usage:   "bluetooth_hc06 <baud> <name> <pincode>"
	// Example: "bluetooth_hc06 38400 rusefi 1234"
	addConsoleActionSSS("bluetooth_hc05", bluetoothHC05);
	addConsoleActionSSS("bluetooth_hc06", bluetoothHC06);
	addConsoleActionSSS("bluetooth_spp", bluetoothSPP);
	addConsoleAction("bluetooth_cancel", bluetoothCancel);
#endif /* EFI_BLUETOOTH_SETUP */

	chThdCreateStatic(tsThreadStack, sizeof(tsThreadStack), NORMALPRIO, (tfunc_t)tsThreadEntryPoint, NULL);
}

#endif
