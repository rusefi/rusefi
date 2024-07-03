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


#include "tunerstudio.h"
#include "tunerstudio_impl.h"

#include "main_trigger_callback.h"
#include "flash_main.h"

#include "tunerstudio_io.h"
#include "malfunction_central.h"
#include "console_io.h"
#include "bluetooth.h"
#include "tunerstudio_io.h"
#include "trigger_scope.h"
#include "electronic_throttle.h"
#include "live_data.h"

#include <string.h>
#include "bench_test.h"
#include "gitversion.h"
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

#endif // EFI_TUNER_STUDIO

void tunerStudioDebug(TsChannelBase* tsChannel, const char *msg) {
#if EFI_TUNER_STUDIO_VERBOSE
	efiPrintf("%s: %s", tsChannel->getName(), msg);
#endif /* EFI_TUNER_STUDIO_VERBOSE */
}

uint8_t* getWorkingPageAddr() {
	return (uint8_t*)engineConfiguration;
}

static void sendOkResponse(TsChannelBase *tsChannel) {
	tsChannel->writeCrcResponse(TS_RESPONSE_OK);
}

void sendErrorCode(TsChannelBase *tsChannel, uint8_t code) {
	efiPrintf("TS <- Err: %d", code);

	tsChannel->writeCrcResponse(code);
}

void TunerStudio::sendErrorCode(TsChannelBase* tsChannel, uint8_t code) {
	::sendErrorCode(tsChannel, code);
}

void TunerStudio::handlePageSelectCommand(TsChannelBase *tsChannel) {
	tsState.pageCommandCounter++;

	efiPrintf("TS -> Set page (no-op)");

	sendOkResponse(tsChannel);
}

bool validateOffsetCount(size_t offset, size_t count, TsChannelBase* tsChannel);

extern bool rebootForPresetPending;

/**
 * This command is needed to make the whole transfer a bit faster
 * @note See also handleWriteValueCommand
 */
void TunerStudio::handleWriteChunkCommand(TsChannelBase* tsChannel, uint16_t offset, uint16_t count,
		void *content) {
	tsState.writeChunkCommandCounter++;
	if (isLockedFromUser()) {
		sendErrorCode(tsChannel, TS_RESPONSE_UNRECOGNIZED_COMMAND);
		return;
	}

	efiPrintf("TS -> Write chunk offset %d count %d", offset, count);

	if (validateOffsetCount(offset, count, tsChannel)) {
		return;
	}

	// Skip the write if a preset was just loaded - we don't want to overwrite it
	if (!rebootForPresetPending) {
		uint8_t * addr = (uint8_t *) (getWorkingPageAddr() + offset);
		memcpy(addr, content, count);
	}
	// Force any board configuration options that humans shouldn't be able to change
	setBoardConfigOverrides();

	sendOkResponse(tsChannel);
}

#if EFI_TUNER_STUDIO

void TunerStudio::handleCrc32Check(TsChannelBase *tsChannel, uint16_t offset, uint16_t count) {
	tsState.crc32CheckCommandCounter++;

	// Ensure we are reading from in bounds
	if (validateOffsetCount(offset, count, tsChannel)) {
		return;
	}

	const uint8_t* start = getWorkingPageAddr() + offset;

	uint32_t crc = crc32(start, count);
	efiPrintf("TS <- Get CRC offset %d count %d result %08x", offset, count, (unsigned int)crc);

	crc = SWAP_UINT32(crc);
	tsChannel->copyAndWriteSmallCrcPacket((const uint8_t *) &crc, sizeof(crc));
}

/**
 * 'Write' command receives a single value at a given offset
 * @note Writing values one by one is pretty slow
 */
void TunerStudio::handleWriteValueCommand(TsChannelBase* tsChannel, uint16_t offset, uint8_t value) {
	tsState.writeValueCommandCounter++;
	if (isLockedFromUser()) {
		sendErrorCode(tsChannel, TS_RESPONSE_UNRECOGNIZED_COMMAND);
		return;
	}

	efiPrintf("TS -> Write value offset %d value %d", offset, value);

	if (validateOffsetCount(offset, 1, tsChannel)) {
		return;
	}

	// Skip the write if a preset was just loaded - we don't want to overwrite it
	if (!rebootForPresetPending) {
		getWorkingPageAddr()[offset] = value;
	}
	// Force any board configuration options that humans shouldn't be able to change
	setBoardConfigOverrides();
}

void TunerStudio::handlePageReadCommand(TsChannelBase* tsChannel, uint16_t offset, uint16_t count) {
	tsState.readPageCommandsCounter++;

	if (rebootForPresetPending) {
		sendErrorCode(tsChannel, TS_RESPONSE_UNRECOGNIZED_COMMAND);
		return;
	}

	efiPrintf("TS <- Read chunk offset %d count %d", offset, count);

	if (validateOffsetCount(offset, count, tsChannel)) {
		return;
	}

	uint8_t* addr;
	if (isLockedFromUser()) {
		// to have rusEFI console happy just send all zeros within a valid packet
		addr = (uint8_t*)&tsChannel->scratchBuffer;
		memset(addr, 0, count);
	} else {
		addr = getWorkingPageAddr() + offset;
	}
	tsChannel->writeCrcPacketLocked(addr, count);
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

/**
 * 'Burn' command is a command to commit the changes
 */
static void handleBurnCommand(TsChannelBase* tsChannel) {
	Timer t;
	t.reset();

	tsState.burnCommandCounter++;

	efiPrintf("TS -> Burn");

	// Skip the burn if a preset was just loaded - we don't want to overwrite it
	if (!rebootForPresetPending) {
		requestBurn();
	}

	tsChannel->writeCrcResponse(TS_RESPONSE_BURN_OK);
	efiPrintf("Burned in %.1fms", t.getElapsedSeconds() * 1e3);
}

#if EFI_TUNER_STUDIO && (EFI_PROD_CODE || EFI_SIMULATOR)

static bool isKnownCommand(char command) {
	return command == TS_HELLO_COMMAND || command == TS_READ_COMMAND || command == TS_OUTPUT_COMMAND
			|| command == TS_PAGE_COMMAND || command == TS_BURN_COMMAND || command == TS_SINGLE_WRITE_COMMAND
			|| command == TS_CHUNK_WRITE_COMMAND || command == TS_EXECUTE
			|| command == TS_IO_TEST_COMMAND
			|| command == TS_SET_LOGGER_SWITCH
			|| command == TS_GET_TEXT
			|| command == TS_CRC_CHECK_COMMAND
			|| command == TS_GET_FIRMWARE_VERSION
			|| command == TS_PERF_TRACE_BEGIN
			|| command == TS_PERF_TRACE_GET_BUFFER
			|| command == TS_GET_CONFIG_ERROR
			|| command == TS_QUERY_BOOTLOADER;
}

/**
 * this command is part of protocol initialization
 *
 * Query with CRC takes place while re-establishing connection
 * Query without CRC takes place on TunerStudio startup
 */
void TunerStudio::handleQueryCommand(TsChannelBase* tsChannel, ts_response_format_e mode) {
	tsState.queryCommandCounter++;

	const char *signature = getTsSignature();

	efiPrintf("TS <- Query signature: %s", signature);
	printErrorCounters();

	auto buffer = (const uint8_t *)signature;
	size_t size = strlen(signature) + 1;

	if (mode == TS_CRC) {
		tsChannel->copyAndWriteSmallCrcPacket(buffer, size);
	} else {
		tsChannel->write(buffer, size, true);
		tsChannel->flush();
	}
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
		handleQueryCommand(tsChannel, TS_PLAIN);
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
		efiPrintf("TS: didn't understand single byte comamnd %d (char '%c')", command, command);
		return false;
	}
}

TunerStudio tsInstance;

static int tsProcessOne(TsChannelBase* tsChannel) {
	validateStack("communication", ObdCode::STACK_USAGE_COMMUNICATION, 128);

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
		if (tsChannel == getBluetoothChannel()) {
			// no data in a whole second means time to disconnect BT
			// assume there's connection loss and notify the bluetooth init code
			bluetoothSoftwareDisconnectNotify(getBluetoothChannel());
		}
#endif  /* EFI_BLUETOOTH_SETUP */
		tsChannel->in_sync = false;
		return -1;
	}

	uint8_t secondByte;
	/* second byte should be received within minimal delay */
	received = tsChannel->readTimeout(&secondByte, 1, TS_COMMUNICATION_TIMEOUT_SHORT);
	if (received != 1) {
		// Second byte timed out: it may be a single-byte "plain" command
		if (!tsInstance.handlePlainCommand(tsChannel, firstByte)) {
			// This wasn't understood as a plain command, so it's an error
			tunerStudioError(tsChannel, "TS: ERROR: no second byte");
			tsChannel->in_sync = false;
		}

		return -1;
	}

	uint16_t incomingPacketSize = firstByte << 8 | secondByte;
	size_t expectedSize = incomingPacketSize + CRC_VALUE_SIZE;

	if (incomingPacketSize == 0 || expectedSize > sizeof(tsChannel->scratchBuffer)) {
		if (tsChannel->in_sync) {
			efiPrintf("process_ts: channel=%s invalid size: %d", tsChannel->getName(), incomingPacketSize);
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
					(unsigned int)actualCrc, (unsigned int)expectedCrc);
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
			onDataArrived();
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
	chsnprintf(versionBuffer, sizeof(versionBuffer), "FOME " QUOTE(SHORT_BOARD_NAME) " %d@" GIT_HASH_SHORT, getRusEfiVersion());
	tsChannel->copyAndWriteSmallCrcPacket((const uint8_t *) versionBuffer, strlen(versionBuffer) + 1);
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

	tsChannel->writeCrcPacketLocked(reinterpret_cast<const uint8_t*>(output), outputSize);
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

	tsChannel->writeCrcResponse(TS_RESPONSE_OK);
}

int TunerStudio::handleCrcCommand(TsChannelBase* tsChannel, uint8_t* data, int incomingPacketSize) {
	ScopePerf perf(PE::TunerStudioHandleCrcCommand);

	char command = data[0];
	data++;

	const uint16_t* data16 = reinterpret_cast<uint16_t*>(data);

	uint16_t offset = data16[0];
	uint16_t count = data16[1];

	switch(command)
	{
	case TS_OUTPUT_COMMAND:
		if (incomingPacketSize == 1) {
			// Read command with no offset/count, read the whole thing
			offset = 0;
			count = TS_TOTAL_OUTPUT_SIZE;
		}

		cmdOutputChannels(tsChannel, offset, count);
		break;
	case TS_HELLO_COMMAND:
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
		handleExecuteCommand(tsChannel, reinterpret_cast<char*>(data), incomingPacketSize - 1);
		break;
	case TS_PAGE_COMMAND:
		handlePageSelectCommand(tsChannel);
		break;
	case TS_CHUNK_WRITE_COMMAND:
		handleWriteChunkCommand(tsChannel, offset, count, data + sizeof(TunerStudioWriteChunkRequest));
		break;
	case TS_SINGLE_WRITE_COMMAND:
		{
			uint8_t value = data[4];
			handleWriteValueCommand(tsChannel, offset, value);
		}
		break;
	case TS_CRC_CHECK_COMMAND:
		handleCrc32Check(tsChannel, offset, count);
		break;
	case TS_BURN_COMMAND:
		handleBurnCommand(tsChannel);
		break;
	case TS_READ_COMMAND:
		handlePageReadCommand(tsChannel, offset, count);
		break;
	case TS_IO_TEST_COMMAND:
		{
			uint16_t subsystem = SWAP_UINT16(data16[0]);
			uint16_t index = SWAP_UINT16(data16[1]);

#if (EFI_PROD_CODE || EFI_SIMULATOR) && EFI_ENGINE_CONTROL
			executeTSCommand(subsystem, index);
#endif /* EFI_PROD_CODE */
			sendOkResponse(tsChannel);
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
		case TS_COMPOSITE_READ:
			{
				auto toothBuffer = GetToothLoggerBufferNonblocking();

				if (toothBuffer) {
					tsChannel->writeCrcPacketLocked(reinterpret_cast<const uint8_t*>(toothBuffer->buffer), toothBuffer->nextIdx * sizeof(composite_logger_s));

					ReturnToothLoggerBuffer(toothBuffer);
				} else {
					// TS asked for a tooth logger buffer, but we don't have one to give it.
					sendErrorCode(tsChannel, TS_RESPONSE_OUT_OF_RANGE);
				}
			}
			break;
#ifdef TRIGGER_SCOPE
		case TS_TRIGGER_SCOPE_ENABLE:
			triggerScopeEnable();
			break;
		case TS_TRIGGER_SCOPE_DISABLE:
			triggerScopeDisable();
			break;
		case TS_TRIGGER_SCOPE_READ:
			{
				const auto& buffer = triggerScopeGetBuffer();

				if (buffer) {
					tsChannel->writeCrcPacketLocked(buffer.get<uint8_t>(), buffer.size());
				} else {
					// TS asked for a tooth logger buffer, but we don't have one to give it.
					sendErrorCode(tsChannel, TS_RESPONSE_OUT_OF_RANGE);
				}
			}
			break;
#endif // TRIGGER_SCOPE
		default:
			// dunno what that was, send NAK
			return false;
		}

		sendOkResponse(tsChannel);

		break;
#endif /* EFI_TOOTH_LOGGER */
#if ENABLE_PERF_TRACE
	case TS_PERF_TRACE_BEGIN:
		perfTraceEnable();
		sendOkResponse(tsChannel);
		break;
	case TS_PERF_TRACE_GET_BUFFER:
		{
			auto trace = perfTraceGetBuffer();
			tsChannel->writeCrcPacketLocked(trace.get<uint8_t>(), trace.size());
		}

		break;
#endif /* ENABLE_PERF_TRACE */
	case TS_GET_CONFIG_ERROR: {
		const char* configError = getCriticalErrorMessage();
		tsChannel->writeCrcPacketLocked(reinterpret_cast<const uint8_t*>(configError), strlen(configError));
		break;
	}
	case TS_QUERY_BOOTLOADER: {
		uint8_t bldata = TS_QUERY_BOOTLOADER_NONE;
#if EFI_USE_OPENBLT
		bldata = TS_QUERY_BOOTLOADER_OPENBLT;
#endif

		tsChannel->copyAndWriteSmallCrcPacket(&bldata, 1);
		break;
	}
	default:
		sendErrorCode(tsChannel, TS_RESPONSE_UNRECOGNIZED_COMMAND);
static char tsErrorBuff[80];
		chsnprintf(tsErrorBuff, sizeof(tsErrorBuff), "ERROR: ignoring unexpected command %d [%c]", command, command);
		tunerStudioError(tsChannel, tsErrorBuff);
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

	addConsoleAction("tsinfo", printErrorCounters);

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
#endif /* EFI_BLUETOOTH_SETUP */
}

#endif
