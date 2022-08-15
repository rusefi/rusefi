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

#include "pch.h"
#include "os_access.h"

#include "tunerstudio.h"
#include "tunerstudio_impl.h"

#include "main_trigger_callback.h"
#include "flash_main.h"

#include "tunerstudio_io.h"
#include "malfunction_central.h"
#include "console_io.h"
#include "bluetooth.h"
#include "tunerstudio_io.h"
#include "tooth_logger.h"
#include "electronic_throttle.h"

#include <string.h>
#include "bench_test.h"
#include "svnversion.h"
#include "status_loop.h"
#include "mmc_card.h"

#include "signature.h"

#if EFI_SIMULATOR
#include "rusEfiFunctionalTest.h"
#endif /* EFI_SIMULATOR */

static void printErrorCounters() {
	efiPrintf("TunerStudio size=%d / total=%d / errors=%d / H=%d / O=%d / P=%d / B=%d",
			sizeof(engine->outputChannels), tsState.totalCounter, tsState.errorCounter, tsState.queryCommandCounter,
			tsState.outputChannelsCommandCounter, tsState.readPageCommandsCounter, tsState.burnCommandCounter);
	efiPrintf("TunerStudio W=%d / C=%d / P=%d", tsState.writeValueCommandCounter,
			tsState.writeChunkCommandCounter, tsState.pageCommandCounter);
}

#if EFI_TUNER_STUDIO

/* 1S */
#define TS_COMMUNICATION_TIMEOUT	TIME_MS2I(1000)
/* 10mS when receiving byte by byte */
#define TS_COMMUNICATION_TIMEOUT_SHORT	TIME_MS2I(10)

static efitimems_t previousWriteReportMs = 0;

static void resetTs() {
	memset(&tsState, 0, sizeof(tsState));
}

static void printTsStats(void) {
#if EFI_PROD_CODE
#ifdef EFI_CONSOLE_RX_BRAIN_PIN
	efiPrintf("Primary UART RX", hwPortname(EFI_CONSOLE_RX_BRAIN_PIN));
	efiPrintf("Primary UART TX", hwPortname(EFI_CONSOLE_TX_BRAIN_PIN));
#endif

	if (false) {
		// todo: is this code needed somewhere else?
		efiPrintf("TS RX on %s", hwPortname(engineConfiguration->binarySerialRxPin));

		efiPrintf("TS TX on %s @%d", hwPortname(engineConfiguration->binarySerialTxPin),
				engineConfiguration->tunerStudioSerialSpeed);
	}
#endif /* EFI_PROD_CODE */

	printErrorCounters();
}

static void setTsSpeed(int value) {
	engineConfiguration->tunerStudioSerialSpeed = value;
	printTsStats();
}

#endif // EFI_TUNER_STUDIO

void tunerStudioDebug(TsChannelBase* tsChannel, const char *msg) {
#if EFI_TUNER_STUDIO_VERBOSE
	efiPrintf("%s: %s", tsChannel->name, msg);
#endif /* EFI_TUNER_STUDIO_VERBOSE */
}

uint8_t* getWorkingPageAddr() {
	return (uint8_t*)engineConfiguration;
}

void sendOkResponse(TsChannelBase *tsChannel, ts_response_format_e mode) {
	tsChannel->sendResponse(mode, NULL, 0);
}

void sendErrorCode(TsChannelBase *tsChannel, uint8_t code) {
	tsChannel->writeCrcPacket(code, nullptr, 0);
}

void TunerStudio::sendErrorCode(TsChannelBase* tsChannel, uint8_t code) {
	::sendErrorCode(tsChannel, code);
}

void TunerStudio::handlePageSelectCommand(TsChannelBase *tsChannel, ts_response_format_e mode) {
	tsState.pageCommandCounter++;

	sendOkResponse(tsChannel, mode);
}

bool validateOffsetCount(size_t offset, size_t count, TsChannelBase* tsChannel);

extern bool rebootForPresetPending;

/**
 * This command is needed to make the whole transfer a bit faster
 * @note See also handleWriteValueCommand
 */
void TunerStudio::handleWriteChunkCommand(TsChannelBase* tsChannel, ts_response_format_e mode, uint16_t offset, uint16_t count,
		void *content) {
	tsState.writeChunkCommandCounter++;
	if (isLockedFromUser()) {
		sendErrorCode(tsChannel, TS_RESPONSE_UNRECOGNIZED_COMMAND);
		return;
	}

	efiPrintf("WRITE CHUNK mode=%d o=%d s=%d", mode, offset, count);

	if (validateOffsetCount(offset, count, tsChannel)) {
		return;
	}

	// Skip the write if a preset was just loaded - we don't want to overwrite it
	if (!rebootForPresetPending) {
		uint8_t * addr = (uint8_t *) (getWorkingPageAddr() + offset);
		memcpy(addr, content, count);
	}

	sendOkResponse(tsChannel, mode);
}

#if EFI_TUNER_STUDIO

void TunerStudio::handleCrc32Check(TsChannelBase *tsChannel, ts_response_format_e mode, uint16_t offset, uint16_t count) {
	tsState.crc32CheckCommandCounter++;

	// Ensure we are reading from in bounds
	if (validateOffsetCount(offset, count, tsChannel)) {
		return;
	}

	const uint8_t* start = getWorkingPageAddr() + offset;

	uint32_t crc = SWAP_UINT32(crc32(start, count));
	tsChannel->sendResponse(mode, (const uint8_t *) &crc, 4);
}

/**
 * 'Write' command receives a single value at a given offset
 * @note Writing values one by one is pretty slow
 */
void TunerStudio::handleWriteValueCommand(TsChannelBase* tsChannel, ts_response_format_e mode, uint16_t offset, uint8_t value) {
	UNUSED(tsChannel);
	UNUSED(mode);

	tsState.writeValueCommandCounter++;
	if (isLockedFromUser()) {
		sendErrorCode(tsChannel, TS_RESPONSE_UNRECOGNIZED_COMMAND);
		return;
	}

	tunerStudioDebug(tsChannel, "got W (Write)"); // we can get a lot of these

	if (validateOffsetCount(offset, 1, tsChannel)) {
		return;
	}

	efitimems_t nowMs = currentTimeMillis();
	if (nowMs - previousWriteReportMs > 5) {
		previousWriteReportMs = nowMs;
		efiPrintf("offset %d: value=%d", offset, value);
	}

	// Skip the write if a preset was just loaded - we don't want to overwrite it
	if (!rebootForPresetPending) {
		getWorkingPageAddr()[offset] = value;
	}
}

void TunerStudio::handlePageReadCommand(TsChannelBase* tsChannel, ts_response_format_e mode, uint16_t offset, uint16_t count) {
	tsState.readPageCommandsCounter++;

	if (rebootForPresetPending) {
		sendErrorCode(tsChannel, TS_RESPONSE_UNRECOGNIZED_COMMAND);
		return;
	}

#if EFI_TUNER_STUDIO_VERBOSE
	efiPrintf("READ mode=%d offset=%d size=%d", mode, offset, count);
#endif

	if (validateOffsetCount(offset, count, tsChannel)) {
		return;
	}

	uint8_t* addr;
	if (isLockedFromUser()) {
		// to have rusEFI console happy just send all zeros within a valid packet
		addr = (uint8_t*)&tsChannel->scratchBuffer + SCRATCH_BUFFER_PREFIX_SIZE;
		memset(addr, 0, count);
	} else {
		addr = getWorkingPageAddr() + offset;
	}
	tsChannel->sendResponse(mode, addr, count);
#if EFI_TUNER_STUDIO_VERBOSE
//	efiPrintf("Sending %d done", count);
#endif
}

#endif // EFI_TUNER_STUDIO

void requestBurn(void) {
#if !EFI_UNIT_TEST
	onBurnRequest();

#if EFI_INTERNAL_FLASH
	setNeedToWriteConfiguration();
#endif
#endif // !EFI_UNIT_TEST
}

static void sendResponseCode(ts_response_format_e mode, TsChannelBase *tsChannel, const uint8_t responseCode) {
	if (mode == TS_CRC) {
		tsChannel->writeCrcPacket(responseCode, nullptr, 0);
	}
}

/**
 * 'Burn' command is a command to commit the changes
 */
static void handleBurnCommand(TsChannelBase* tsChannel, ts_response_format_e mode) {
	efitimems_t nowMs = currentTimeMillis();
	tsState.burnCommandCounter++;

	efiPrintf("got B (Burn) %s", mode == TS_PLAIN ? "plain" : "CRC");

	// Skip the burn if a preset was just loaded - we don't want to overwrite it
	if (!rebootForPresetPending) {
		requestBurn();
	}

	sendResponseCode(mode, tsChannel, TS_RESPONSE_BURN_OK);
	efiPrintf("BURN in %dms", currentTimeMillis() - nowMs);
}

#if EFI_TUNER_STUDIO && (EFI_PROD_CODE || EFI_SIMULATOR)

static bool isKnownCommand(char command) {
	return command == TS_HELLO_COMMAND || command == TS_READ_COMMAND || command == TS_OUTPUT_COMMAND
			|| command == TS_PAGE_COMMAND || command == TS_BURN_COMMAND || command == TS_SINGLE_WRITE_COMMAND
			|| command == TS_CHUNK_WRITE_COMMAND || command == TS_EXECUTE
			|| command == TS_IO_TEST_COMMAND
			|| command == TS_SET_LOGGER_SWITCH
			|| command == TS_GET_LOGGER_GET_BUFFER
			|| command == TS_GET_COMPOSITE_BUFFER_DONE_DIFFERENTLY
			|| command == TS_GET_TEXT
			|| command == TS_CRC_CHECK_COMMAND
			|| command == TS_GET_FIRMWARE_VERSION
			|| command == TS_PERF_TRACE_BEGIN
			|| command == TS_PERF_TRACE_GET_BUFFER
			|| command == TS_GET_CONFIG_ERROR;
}

/**
 * rusEfi own test command
 */
static void handleTestCommand(TsChannelBase* tsChannel) {
	tsState.testCommandCounter++;
	char testOutputBuffer[64];
	/**
	 * this is NOT a standard TunerStudio command, this is my own
	 * extension of the protocol to simplify troubleshooting
	 */
	tunerStudioDebug(tsChannel, "got T (Test)");
	tsChannel->write((const uint8_t*)VCS_VERSION, sizeof(VCS_VERSION));

	chsnprintf(testOutputBuffer, sizeof(testOutputBuffer), " %d %d", engine->engineState.warnings.lastErrorCode, tsState.testCommandCounter);
	tsChannel->write((const uint8_t*)testOutputBuffer, strlen(testOutputBuffer));

	chsnprintf(testOutputBuffer, sizeof(testOutputBuffer), " uptime=%ds ", (int)getTimeNowSeconds());
	tsChannel->write((const uint8_t*)testOutputBuffer, strlen(testOutputBuffer));

	chsnprintf(testOutputBuffer, sizeof(testOutputBuffer),  __DATE__ " %s\r\n", PROTOCOL_TEST_RESPONSE_TAG);
	tsChannel->write((const uint8_t*)testOutputBuffer, strlen(testOutputBuffer));

	if (hasFirmwareError()) {
		const char* error = getCriticalErrorMessage();
		chsnprintf(testOutputBuffer, sizeof(testOutputBuffer), "error=%s\r\n", error);
		tsChannel->write((const uint8_t*)testOutputBuffer, strlen(testOutputBuffer));
	}
	tsChannel->flush();
}

/**
 * this command is part of protocol initialization
 *
 * Query with CRC takes place while re-establishing connection
 * Query without CRC takes place on TunerStudio startup
 */
void TunerStudio::handleQueryCommand(TsChannelBase* tsChannel, ts_response_format_e mode) {
	tsState.queryCommandCounter++;
#if EFI_TUNER_STUDIO_VERBOSE
	efiPrintf("got S/H (queryCommand) mode=%d", mode);
	printTsStats();
#endif
	const char *signature = getTsSignature();
	tsChannel->sendResponse(mode, (const uint8_t *)signature, strlen(signature) + 1);
}

/**
 * handle non CRC wrapped command
 *
 * @return true if legacy command was processed, false otherwise
 */
bool TunerStudio::handlePlainCommand(TsChannelBase* tsChannel, uint8_t command) {
	// Bail fast if guaranteed not to be a plain command
	if (command == 0) {
		return false;
	} else if (command == TS_HELLO_COMMAND || command == TS_QUERY_COMMAND) {
		// We interpret 'Q' as TS_HELLO_COMMAND, since TS uses hardcoded 'Q' during ECU detection (scan all serial ports)
		efiPrintf("Got naked Query command");
		handleQueryCommand(tsChannel, TS_PLAIN);
		return true;
	} else if (command == TS_TEST_COMMAND || command == 'T') {
		handleTestCommand(tsChannel);
		return true;
	} else if (command == TS_COMMAND_F) {
		/**
		 * http://www.msextra.com/forums/viewtopic.php?f=122&t=48327
		 * Response from TS support: This is an optional command		 *
		 * "The F command is used to find what ini. file needs to be loaded in TunerStudio to match the controller.
		 * If you are able to just make your firmware ignore the command that would work.
		 * Currently on some firmware versions the F command is not used and is just ignored by the firmware as a unknown command."
		 */

		tunerStudioDebug(tsChannel, "not ignoring F");
		tsChannel->write((const uint8_t *)TS_PROTOCOL, strlen(TS_PROTOCOL));
		tsChannel->flush();
		return true;
	} else {
		// This wasn't a valid command
		return false;
	}
}

TunerStudio tsInstance;

static int tsProcessOne(TsChannelBase* tsChannel) {
	validateStack("communication", STACK_USAGE_COMMUNICATION, 128);

	if (!tsChannel->isReady()) {
		chThdSleepMilliseconds(10);
		return -1;
	}

	tsState.totalCounter++;

	uint8_t firstByte;
	size_t received = tsChannel->readTimeout(&firstByte, 1, TS_COMMUNICATION_TIMEOUT);
#if EFI_SIMULATOR
		logMsg("received %d\r\n", received);
#endif

	if (received != 1) {
//			tunerStudioError("ERROR: no command");
#if EFI_BLUETOOTH_SETUP
		// no data in a whole second means time to disconnect BT
		// assume there's connection loss and notify the bluetooth init code
		bluetoothSoftwareDisconnectNotify();
#endif  /* EFI_BLUETOOTH_SETUP */
		tsChannel->in_sync = false;
		return -1;
	}

	if (tsInstance.handlePlainCommand(tsChannel, firstByte)) {
		return -1;
	}

	uint8_t secondByte;
	/* second byte should be received within minimal delay */
	received = tsChannel->readTimeout(&secondByte, 1, TS_COMMUNICATION_TIMEOUT_SHORT);
	if (received != 1) {
		tunerStudioError(tsChannel, "TS: ERROR: no second byte");
		tsChannel->in_sync = false;
		return -1;
	}

	uint16_t incomingPacketSize = firstByte << 8 | secondByte;
	size_t expectedSize = incomingPacketSize + CRC_VALUE_SIZE;

	if (incomingPacketSize == 0 || expectedSize > sizeof(tsChannel->scratchBuffer)) {
		if (tsChannel->in_sync) {
			efiPrintf("process_ts: channel=%s invalid size: %d", tsChannel->name, incomingPacketSize);
			tunerStudioError(tsChannel, "process_ts: ERROR: CRC header size");
			/* send error only if previously we were in sync */
			sendErrorCode(tsChannel, TS_RESPONSE_UNDERRUN);
		}
		tsChannel->in_sync = false;
		return -1;
	}

	char command;
	if (tsChannel->in_sync) {
		/* we are in sync state, packet size should be correct so lets receive full packet and then check if command is supported
		 * otherwise (if abort reception in middle of packet) it will break syncronization and cause error on next packet */
		received = tsChannel->readTimeout((uint8_t*)(tsChannel->scratchBuffer), expectedSize, TS_COMMUNICATION_TIMEOUT);
		command = tsChannel->scratchBuffer[0];

		if (received != expectedSize) {
			/* print and send error as we were in sync */
			efiPrintf("Got only %d bytes while expecting %d for command %c", received,
					expectedSize, command);
			tunerStudioError(tsChannel, "ERROR: not enough bytes in stream");
			sendErrorCode(tsChannel, TS_RESPONSE_UNDERRUN);
			tsChannel->in_sync = false;
			return -1;
		}

		if (!isKnownCommand(command)) {
			/* print and send error as we were in sync */
			efiPrintf("unexpected command %x", command);
			sendErrorCode(tsChannel, TS_RESPONSE_UNRECOGNIZED_COMMAND);
			tsChannel->in_sync = false;
			return -1;
		}
	} else {
		/* receive only command byte to check if it is supported */
		received = tsChannel->readTimeout((uint8_t*)(tsChannel->scratchBuffer), 1, TS_COMMUNICATION_TIMEOUT_SHORT);
		command = tsChannel->scratchBuffer[0];

		if (!isKnownCommand(command)) {
			/* do not report any error as we are not in sync */
			return -1;
		}

		received = tsChannel->readTimeout((uint8_t*)(tsChannel->scratchBuffer) + 1, expectedSize - 1, TS_COMMUNICATION_TIMEOUT);
		if (received != expectedSize - 1) {
			/* do not report any error as we are not in sync */
			return -1;
		}
	}

#if EFI_SIMULATOR
	logMsg("command %c\r\n", command);
#endif

	uint32_t expectedCrc = *(uint32_t*) (tsChannel->scratchBuffer + incomingPacketSize);

	expectedCrc = SWAP_UINT32(expectedCrc);

	uint32_t actualCrc = crc32(tsChannel->scratchBuffer, incomingPacketSize);
	if (actualCrc != expectedCrc) {
		/* send error only if previously we were in sync */
		if (tsChannel->in_sync) {
			efiPrintf("TunerStudio: command %c actual CRC %x/expected %x", tsChannel->scratchBuffer[0],
					actualCrc, expectedCrc);
			tunerStudioError(tsChannel, "ERROR: CRC issue");
			sendErrorCode(tsChannel, TS_RESPONSE_CRC_FAILURE);
			tsChannel->in_sync = false;
		}
		return -1;
	}

	/* we were able to receive known command with correct crc and size! */
	tsChannel->in_sync = true;

	int success = tsInstance.handleCrcCommand(tsChannel, tsChannel->scratchBuffer, incomingPacketSize);

	if (!success) {
		efiPrintf("got unexpected TunerStudio command %x:%c", command, command);
		return -1;
	}

	return 0;
}

void TunerstudioThread::ThreadTask() {
	auto channel = setupChannel();

	// No channel configured for this thread, cancel.
	if (!channel || !channel->isConfigured()) {
		return;
	}

	// Until the end of time, process incoming messages.
	while (true) {
		if (tsProcessOne(channel) == 0) {
			onDataArrived(true);
		} else {
			onDataArrived(false);
		}
	}
}

#endif // EFI_TUNER_STUDIO

tunerstudio_counters_s tsState;

void tunerStudioError(TsChannelBase* tsChannel, const char *msg) {
	tunerStudioDebug(tsChannel, msg);
	printErrorCounters();
	tsState.errorCounter++;
}

#if EFI_TUNER_STUDIO

#if EFI_PROD_CODE || EFI_SIMULATOR

extern CommandHandler console_line_callback;

static void handleGetVersion(TsChannelBase* tsChannel) {
	char versionBuffer[32];
	chsnprintf(versionBuffer, sizeof(versionBuffer), "rusEFI v%d@%s", getRusEfiVersion(), VCS_VERSION);
	tsChannel->sendResponse(TS_CRC, (const uint8_t *) versionBuffer, strlen(versionBuffer) + 1);
}

#if EFI_TEXT_LOGGING
static void handleGetText(TsChannelBase* tsChannel) {
	tsState.textCommandCounter++;

	printOverallStatus();

	size_t outputSize;
	const char* output = swapOutputBuffers(&outputSize);
#if EFI_SIMULATOR
			logMsg("get test sending [%d]\r\n", outputSize);
#endif

	tsChannel->writeCrcPacket(TS_RESPONSE_COMMAND_OK, reinterpret_cast<const uint8_t*>(output), outputSize, true);
#if EFI_SIMULATOR
			logMsg("sent [%d]\r\n", outputSize);
#endif
}
#endif // EFI_TEXT_LOGGING

void TunerStudio::handleExecuteCommand(TsChannelBase* tsChannel, char *data, int incomingPacketSize) {
	data[incomingPacketSize] = 0;
	char *trimmed = efiTrim(data);
#if EFI_SIMULATOR
			logMsg("execute [%s]\r\n", trimmed);
#endif
	(console_line_callback)(trimmed);

	tsChannel->writeCrcPacket(TS_RESPONSE_COMMAND_OK, nullptr, 0);
}

int TunerStudio::handleCrcCommand(TsChannelBase* tsChannel, char *data, int incomingPacketSize) {
	ScopePerf perf(PE::TunerStudioHandleCrcCommand);

	char command = data[0];
	data++;

	const uint16_t* data16 = reinterpret_cast<uint16_t*>(data);

	uint16_t offset = data16[0];
	uint16_t count = data16[1];

	switch(command)
	{
	case TS_OUTPUT_COMMAND:
		cmdOutputChannels(tsChannel, offset, count);
		break;
	case TS_HELLO_COMMAND:
		tunerStudioDebug(tsChannel, "got Query command");
		handleQueryCommand(tsChannel, TS_CRC);
		break;
	case TS_GET_FIRMWARE_VERSION:
		handleGetVersion(tsChannel);
		break;
#if EFI_TEXT_LOGGING
	case TS_GET_TEXT:
		handleGetText(tsChannel);
		break;
#endif // EFI_TEXT_LOGGING
	case TS_EXECUTE:
		handleExecuteCommand(tsChannel, data, incomingPacketSize - 1);
		break;
	case TS_PAGE_COMMAND:
		handlePageSelectCommand(tsChannel, TS_CRC);
		break;
	case TS_CHUNK_WRITE_COMMAND:
		handleWriteChunkCommand(tsChannel, TS_CRC, offset, count, data + sizeof(TunerStudioWriteChunkRequest));
		break;
	case TS_SINGLE_WRITE_COMMAND:
		{
			uint8_t value = data[4];
			handleWriteValueCommand(tsChannel, TS_CRC, offset, value);
		}
		break;
	case TS_CRC_CHECK_COMMAND:
		handleCrc32Check(tsChannel, TS_CRC, offset, count);
		break;
	case TS_BURN_COMMAND:
		handleBurnCommand(tsChannel, TS_CRC);
		break;
	case TS_READ_COMMAND:
		handlePageReadCommand(tsChannel, TS_CRC, offset, count);
		break;
	case TS_TEST_COMMAND:
		[[fallthrough]];
	case 'T':
		handleTestCommand(tsChannel);
		break;
	case TS_IO_TEST_COMMAND:
		{
			uint16_t subsystem = SWAP_UINT16(data16[0]);
			uint16_t index = SWAP_UINT16(data16[1]);

			if (engineConfiguration->debugMode == DBG_BENCH_TEST) {
				engine->outputChannels.debugIntField1++;
				engine->outputChannels.debugIntField2 = subsystem;
				engine->outputChannels.debugIntField3 = index;
			}

#if EFI_PROD_CODE && EFI_ENGINE_CONTROL
		executeTSCommand(subsystem, index);
#endif /* EFI_PROD_CODE */
			sendOkResponse(tsChannel, TS_CRC);
		}
		break;
#if EFI_TOOTH_LOGGER
	case TS_SET_LOGGER_SWITCH:
		switch(data[0]) {
		case TS_COMPOSITE_ENABLE:
			EnableToothLogger();
			break;
		case TS_COMPOSITE_DISABLE:
			DisableToothLogger();
			break;
		default:
			// dunno what that was, send NAK
			return false;
		}

		sendOkResponse(tsChannel, TS_CRC);

		break;
	case TS_GET_COMPOSITE_BUFFER_DONE_DIFFERENTLY:
		EnableToothLoggerIfNotEnabled();
		// falls through
	case TS_GET_LOGGER_GET_BUFFER:
		{
			auto toothBuffer = GetToothLoggerBuffer();

			if (toothBuffer) {
				tsChannel->sendResponse(TS_CRC, toothBuffer.Value.Buffer, toothBuffer.Value.Length, true);
			} else {
				// TS asked for a tooth logger buffer, but we don't have one to give it.
				sendErrorCode(tsChannel, TS_RESPONSE_OUT_OF_RANGE);
			}
		}

		break;
#endif /* EFI_TOOTH_LOGGER */
#if ENABLE_PERF_TRACE
	case TS_PERF_TRACE_BEGIN:
		perfTraceEnable();
		sendOkResponse(tsChannel, TS_CRC);
		break;
	case TS_PERF_TRACE_GET_BUFFER:
		{
			auto trace = perfTraceGetBuffer();
			tsChannel->sendResponse(TS_CRC, trace.Buffer, trace.Size, true);
		}

		break;
#endif /* ENABLE_PERF_TRACE */
	case TS_GET_CONFIG_ERROR: {
		const char* configError = getCriticalErrorMessage();
#if HW_CHECK_MODE
		// analog input errors are returned as firmware error in QC mode
		if (!hasFirmwareError()) {
			strcpy((char*)configError, "FACTORY_MODE_PLEASE_CONTACT_SUPPORT");
		}
#endif // HW_CHECK_MODE
		tsChannel->sendResponse(TS_CRC, reinterpret_cast<const uint8_t*>(configError), strlen(configError), true);
		break;
	}
	default:
		sendErrorCode(tsChannel, TS_RESPONSE_UNRECOGNIZED_COMMAND);
		tunerStudioError(tsChannel, "ERROR: ignoring unexpected command");
		return false;
	}

	return true;
}

#endif // EFI_PROD_CODE || EFI_SIMULATOR

void startTunerStudioConnectivity(void) {
	// Assert tune & output channel struct sizes
	static_assert(sizeof(persistent_config_s) == TOTAL_CONFIG_SIZE, "TS datapage size mismatch");
// useful trick if you need to know how far off is the static_assert
//	char (*__kaboom)[sizeof(persistent_config_s)] = 1;
// another useful trick
//  static_assert(offsetof (engine_configuration_s,HD44780_e) == 700);

	memset(&tsState, 0, sizeof(tsState));

	addConsoleAction("tsinfo", printTsStats);
	addConsoleAction("reset_ts", resetTs);
	addConsoleActionI("set_ts_speed", setTsSpeed);
	
#if EFI_BLUETOOTH_SETUP
	// module initialization start (it waits for disconnect and then communicates to the module)
	// Usage:   "bluetooth_hc06 <baud> <name> <pincode>"
	// Example: "bluetooth_hc06 38400 rusefi 1234"
	// bluetooth_jdy 115200 alphax 1234
	addConsoleActionSSS("bluetooth_hc05", [](const char *baudRate, const char *name, const char *pinCode) {
		bluetoothStart(BLUETOOTH_HC_05, baudRate, name, pinCode);
	});
	addConsoleActionSSS("bluetooth_hc06", [](const char *baudRate, const char *name, const char *pinCode) {
		bluetoothStart(BLUETOOTH_HC_06, baudRate, name, pinCode);
	});
	addConsoleActionSSS("bluetooth_bk", [](const char *baudRate, const char *name, const char *pinCode) {
		bluetoothStart(BLUETOOTH_BK3231, baudRate, name, pinCode);
	});
	addConsoleActionSSS("bluetooth_jdy", [](const char *baudRate, const char *name, const char *pinCode) {
		bluetoothStart(BLUETOOTH_JDY_3x, baudRate, name, pinCode);
	});
	addConsoleAction("bluetooth_cancel", bluetoothCancel);
#endif /* EFI_BLUETOOTH_SETUP */
}

#endif
