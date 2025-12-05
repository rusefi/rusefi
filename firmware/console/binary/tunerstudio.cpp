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
 * See also https://www.efianalytics.com/TunerStudio/docs/EFI%20Analytics%20ECU%20Definition%20files.pdf
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
#include "efi_quote.h"

#include <string.h>
#include "bench_test.h"
#include "status_loop.h"
#include "mmc_card.h"

#if EFI_SIMULATOR
#include "rusEfiFunctionalTest.h"
#endif /* EFI_SIMULATOR */

#include "board_overrides.h"

#if EFI_TUNER_STUDIO

// We have TS protocol limitation: offset within one settings page is uin16_t type.
static_assert(sizeof(*config) <= 65536);

static void printErrorCounters() {
	efiPrintf("TunerStudio size=%d / total=%d / errors=%d / H=%d / O=%d / P=%d / B=%d / 9=%d",
			sizeof(engine->outputChannels), tsState.totalCounter, tsState.errorCounter, tsState.queryCommandCounter,
			tsState.outputChannelsCommandCounter, tsState.readPageCommandsCounter, tsState.burnCommandCounter,
			tsState.readScatterCommandsCounter);
	efiPrintf("TunerStudio C=%d",
			tsState.writeChunkCommandCounter);
	efiPrintf("TunerStudio errors: underrun=%d / overrun=%d / crc=%d / unrecognized=%d / outofrange=%d / other=%d",
			tsState.errorUnderrunCounter, tsState.errorOverrunCounter, tsState.errorCrcCounter,
			tsState.errorUnrecognizedCommand, tsState.errorOutOfRange, tsState.errorOther);
}

namespace {
	Timer calibrationsVeWriteTimer;
}

#if 0
static void printScatterList(TsChannelBase* tsChannel) {
	efiPrintf("Scatter list (global)");
	for (size_t i = 0; i < TS_SCATTER_OFFSETS_COUNT; i++) {
		uint16_t packed = tsChannel->highSpeedOffsets[i];
		uint16_t type = packed >> 13;
		uint16_t offset = packed & 0x1FFF;

		if (type == 0)
			continue;
		size_t size = 1 << (type - 1);

		efiPrintf("%02d offset 0x%04x size %d", i, offset, size);
	}
}
#endif

/* 1S */
#define TS_COMMUNICATION_TIMEOUT	TIME_MS2I(1000)
/* 10mS when receiving byte by byte */
#define TS_COMMUNICATION_TIMEOUT_SHORT	TIME_MS2I(10)

static void resetTs() {
	memset(&tsState, 0, sizeof(tsState));
}

static void printTsStats(void) {
#ifdef EFI_CONSOLE_RX_BRAIN_PIN
	efiPrintf("Primary UART RX %s", hwPortname(EFI_CONSOLE_RX_BRAIN_PIN));
	efiPrintf("Primary UART TX %s", hwPortname(EFI_CONSOLE_TX_BRAIN_PIN));
#endif /* EFI_CONSOLE_RX_BRAIN_PIN */

#if EFI_USB_SERIAL
	printUsbConnectorStats();
#endif // EFI_USB_SERIAL

	printErrorCounters();

	// TODO: find way to get all tsChannel
	//printScatterList();
}

static void setTsSpeed(int value) {
	engineConfiguration->tunerStudioSerialSpeed = value;
	printTsStats();
}

void tunerStudioDebug(TsChannelBase* tsChannel, const char *msg) {
#if EFI_TUNER_STUDIO_VERBOSE
	efiPrintf("%s: %s", tsChannel->name, msg);
#endif /* EFI_TUNER_STUDIO_VERBOSE */
}

static uint8_t* getWorkingPageAddr(TsChannelBase* tsChannel, size_t page, size_t offset) {
	// TODO: validate offset?
	switch (page) {
	case TS_PAGE_SETTINGS:
		// TODO: why engineConfiguration, not config
		// TS has access to whole persistent_config_s
		return (uint8_t*)engineConfiguration + offset;
#if EFI_TS_SCATTER
	case TS_PAGE_SCATTER_OFFSETS:
		return (uint8_t *)tsChannel->page1.highSpeedOffsets + offset;
#endif
#if EFI_LTFT_CONTROL
	case TS_PAGE_LTFT_TRIMS:
		return (uint8_t *)ltftGetTsPage() + offset;
#endif
	default:
// technical dept: TS seems to try to read the 3 pages sequentially, does not look like we properly handle 'EFI_TS_SCATTER=FALSE'
		tunerStudioError(tsChannel, "ERROR: page address out of range");
		return nullptr;
	}
}

static constexpr size_t getTunerStudioPageSize(size_t page) {
	switch (page) {
	case TS_PAGE_SETTINGS:
		return TOTAL_CONFIG_SIZE;
#if EFI_TS_SCATTER
	case TS_PAGE_SCATTER_OFFSETS:
		return PAGE_SIZE_1;
#endif
#if EFI_LTFT_CONTROL
	case TS_PAGE_LTFT_TRIMS:
		return ltftGetTsPageSize();
#endif
	default:
		return 0;
	}
}

// Validate whether the specified offset and count would cause an overrun in the tune.
// Returns true if an overrun would occur.
static bool validateOffsetCount(size_t page, size_t offset, size_t count, TsChannelBase* tsChannel) {
	size_t allowedSize = getTunerStudioPageSize(page);
	if (offset + count > allowedSize) {
		efiPrintf("TS: Project mismatch? Too much configuration requested %d+%d>%d", offset, count, allowedSize);
		tunerStudioError(tsChannel, "ERROR: out of range");
		sendErrorCode(tsChannel, TS_RESPONSE_OUT_OF_RANGE, "bad_offset");
		return true;
	}

	return false;
}

static void sendOkResponse(TsChannelBase *tsChannel) {
	tsChannel->sendResponse(TS_CRC, nullptr, 0);
}

void sendErrorCode(TsChannelBase *tsChannel, uint8_t code, const char *msg) {
	//TODO uncomment once I have test it myself
	UNUSED(msg);
	//if (msg != DO_NOT_LOG) {
	//	efiPrintf("TS <- Err: %d [%s]", code, msg);
	//}

	switch (code) {
	case TS_RESPONSE_UNDERRUN:
		tsState.errorUnderrunCounter++;
		break;
	case TS_RESPONSE_OVERRUN:
		tsState.errorOverrunCounter++;
		break;
	case TS_RESPONSE_CRC_FAILURE:
		tsState.errorCrcCounter++;
		break;
	case TS_RESPONSE_UNRECOGNIZED_COMMAND:
		tsState.errorUnrecognizedCommand++;
		break;
	case TS_RESPONSE_OUT_OF_RANGE:
		tsState.errorOutOfRange++;
		break;
	default:
		tsState.errorOther++;
		break;
	}

	tsChannel->writeCrcResponse(code);
}

void TunerStudio::sendErrorCode(TsChannelBase* tsChannel, uint8_t code, const char *msg) {
	::sendErrorCode(tsChannel, code, msg);
}

PUBLIC_API_WEAK bool isBoardAskingTriggerTsRefresh() {
	return false;
}

bool needToTriggerTsRefresh() {
	return !engine->engineTypeChangeTimer.hasElapsedSec(1);
}

void onApplyPreset() {
	engine->engineTypeChangeTimer.reset();
}

PUBLIC_API_WEAK bool isTouchingVe(uint16_t offset, uint16_t count) {
  return isTouchingArea(offset, count, offsetof(persistent_config_s, veTable), sizeof(config->veTable));
}

static void onCalibrationWrite(uint16_t page, uint16_t offset, uint16_t count) {
	if ((page == TS_PAGE_SETTINGS) && isTouchingVe(offset, count)) {
		calibrationsVeWriteTimer.reset();
	}
}

bool isTouchingArea(uint16_t offset, uint16_t count, int areaStart, int areaSize) {
  if (offset + count < areaStart) {
    // we are touching below for instance VE table
    return false;
  }
  if (offset > areaStart + areaSize) {
    // we are touching after for instance VE table
    return false;
  }
  // else - we are touching it!
  return true;
}

/**
 * This command is needed to make the whole transfer a bit faster
 */
void TunerStudio::handleWriteChunkCommand(TsChannelBase* tsChannel, uint16_t page, uint16_t offset, uint16_t count,
		void *content) {
	tsState.writeChunkCommandCounter++;

	efiPrintf("TS -> Page %d write chunk offset %d count %d (output_count=%d)",
		page, offset, count, tsState.outputChannelsCommandCounter);


	if (validateOffsetCount(page, offset, count, tsChannel)) {
		tunerStudioError(tsChannel, "ERROR: WR out of range");
		sendErrorCode(tsChannel, TS_RESPONSE_OUT_OF_RANGE);
		return;
	}

	uint8_t * addr = getWorkingPageAddr(tsChannel, page, offset);
	if (addr == nullptr) {
		sendErrorCode(tsChannel, TS_RESPONSE_OUT_OF_RANGE, "ERROR: WR invalid page");
		return;
	}

	onCalibrationWrite(page, offset, count);

	// Special case
	if (page == TS_PAGE_SETTINGS) {
		if (isLockedFromUser()) {
			sendErrorCode(tsChannel, TS_RESPONSE_UNRECOGNIZED_COMMAND, "locked");
			return;
		}

		// Skip the write if a preset was just loaded - we don't want to overwrite it
		// [tag:popular_vehicle]
		if (!needToTriggerTsRefresh()) {
			memcpy(addr, content, count);
		} else {
			efiPrintf("Ignoring TS -> Page %d write chunk offset %d count %d (output_count=%d)",
				page,
				offset,
				count,
				tsState.outputChannelsCommandCounter
			);
		}
		// Force any board configuration options that humans shouldn't be able to change
		// huh, why is this NOT within above 'needToTriggerTsRefresh()' condition?
		call_board_override(custom_board_ConfigOverrides);
	} else {
		memcpy(addr, content, count);
	}

	sendOkResponse(tsChannel);
}

void TunerStudio::handleCrc32Check(TsChannelBase *tsChannel, uint16_t page, uint16_t offset, uint16_t count) {
	tsState.crc32CheckCommandCounter++;

	// Ensure we are reading from in bounds
	if (validateOffsetCount(page, offset, count, tsChannel)) {
		tunerStudioError(tsChannel, "ERROR: CRC out of range");
		sendErrorCode(tsChannel, TS_RESPONSE_OUT_OF_RANGE);
		return;
	}

	const uint8_t* start = getWorkingPageAddr(tsChannel, page, offset);
	if (start == nullptr) {
		sendErrorCode(tsChannel, TS_RESPONSE_OUT_OF_RANGE, "ERROR: CRC invalid page");
		return;
	}

	uint32_t crc = SWAP_UINT32(crc32(start, count));
	tsChannel->sendResponse(TS_CRC, (const uint8_t *) &crc, 4);
	efiPrintf("TS <- Get CRC page %d offset %d count %d result %08x", page, offset, count, (unsigned int)crc);
	// todo: rename to onConfigCrc?
	ConfigurationWizard::onConfigOnStartUpOrBurn(false);
}

#if EFI_TS_SCATTER
void TunerStudio::handleScatteredReadCommand(TsChannelBase* tsChannel) {
	tsState.readScatterCommandsCounter++;

	int totalResponseSize = 0;
	for (size_t i = 0; i < TS_SCATTER_OFFSETS_COUNT; i++) {
		uint16_t packed = tsChannel->page1.highSpeedOffsets[i];
		uint16_t type = packed >> 13;

		size_t size = type == 0 ? 0 : 1 << (type - 1);
#if EFI_SIMULATOR
//		printf("handleScatteredReadCommand 0x%x %d %d\n", packed, size, offset);
#endif /* EFI_SIMULATOR */
		totalResponseSize += size;
	}
#if EFI_SIMULATOR
//	printf("totalResponseSize %d\n", totalResponseSize);
#endif /* EFI_SIMULATOR */

	// Command part of CRC
	uint32_t crc = tsChannel->writePacketHeader(TS_RESPONSE_OK, totalResponseSize);

	uint8_t dataBuffer[8];
	for (size_t i = 0; i < TS_SCATTER_OFFSETS_COUNT; i++) {
		uint16_t packed = tsChannel->page1.highSpeedOffsets[i];
		uint16_t type = packed >> 13;
		uint16_t offset = packed & 0x1FFF;

		if (type == 0)
			continue;
		size_t size = 1 << (type - 1);

		// write each data point and CRC incrementally
		copyRange(dataBuffer, getLiveDataFragments(), offset, size);
		tsChannel->write(dataBuffer, size, false);
		crc = crc32inc((void*)dataBuffer, crc, size);
	}
#if EFI_SIMULATOR
//	printf("CRC %x\n", crc);
#endif /* EFI_SIMULATOR */
	// now write total CRC
	*(uint32_t*)dataBuffer = SWAP_UINT32(crc);
	tsChannel->write(dataBuffer, 4, true);
	tsChannel->flush();
}
#endif // EFI_TS_SCATTER

void TunerStudio::handlePageReadCommand(TsChannelBase* tsChannel, uint16_t page, uint16_t offset, uint16_t count) {
	tsState.readPageCommandsCounter++;
	efiPrintf("TS <- Page %d read chunk offset %d count %d", page, offset, count);

	if (validateOffsetCount(page, offset, count, tsChannel)) {
		tunerStudioError(tsChannel, "ERROR: RD out of range");
		sendErrorCode(tsChannel, TS_RESPONSE_OUT_OF_RANGE);
		return;
	}

	uint8_t* addr = getWorkingPageAddr(tsChannel, page, offset);
	if (page == TS_PAGE_SETTINGS) {
		if (isLockedFromUser()) {
			// to have rusEFI console happy just send all zeros within a valid packet
			addr = (uint8_t*)&tsChannel->scratchBuffer + TS_PACKET_HEADER_SIZE;
			memset(addr, 0, count);
		}
	}

	if (addr == nullptr) {
		sendErrorCode(tsChannel, TS_RESPONSE_OUT_OF_RANGE, "ERROR: RD invalid page");
		return;
	}

	tsChannel->sendResponse(TS_CRC, addr, count);
#if EFI_TUNER_STUDIO_VERBOSE
//	efiPrintf("Sending %d done", count);
#endif
}
#endif // EFI_TUNER_STUDIO

void requestBurn() {
#if !EFI_UNIT_TEST
	onBurnRequest();

#if EFI_CONFIGURATION_STORAGE
	setNeedToWriteConfiguration();
#endif /* EFI_CONFIGURATION_STORAGE */
#endif // !EFI_UNIT_TEST
}

#if EFI_TUNER_STUDIO
/**
 * 'Burn' command is a command to commit the changes
 */
static void handleBurnCommand(TsChannelBase* tsChannel, uint16_t page) {
	if (page == TS_PAGE_SETTINGS) {
		Timer t;
		t.reset();

		tsState.burnCommandCounter++;

		efiPrintf("TS -> Burn");
		validateConfigOnStartUpOrBurn(true);

		// problem: 'popular vehicles' dialog has 'Burn' which is very NOT helpful on that dialog
		// since users often click both buttons producing a conflict between ECU desire to change settings
		// and TS desire to send TS calibration snapshot into ECU
		// Skip the burn if a preset was just loaded - we don't want to overwrite it
		// [tag:popular_vehicle]
		if (!needToTriggerTsRefresh()) {
			efiPrintf("TS -> Burn, we are allowed to burn");
			requestBurn();
		}
		efiPrintf("Burned in %.1fms", t.getElapsedSeconds() * 1e3);
#if EFI_TS_SCATTER
	} else if (page == TS_PAGE_SCATTER_OFFSETS) {
		/* do nothing */
#endif
	} else {
		sendErrorCode(tsChannel, TS_RESPONSE_OUT_OF_RANGE, "ERROR: Burn invalid page");
		return;
	}

	tsChannel->writeCrcResponse(TS_RESPONSE_BURN_OK);
}

#if (EFI_PROD_CODE || EFI_SIMULATOR)

static bool isKnownCommand(char command) {
	return command == TS_HELLO_COMMAND || command == TS_READ_COMMAND || command == TS_OUTPUT_COMMAND
			|| command == TS_BURN_COMMAND
			|| command == TS_CHUNK_WRITE_COMMAND || command == TS_EXECUTE
			|| command == TS_IO_TEST_COMMAND
#if EFI_SIMULATOR
			|| command == TS_SIMULATE_CAN
#endif // EFI_SIMULATOR
#if EFI_TS_SCATTER
			|| command == TS_GET_SCATTERED_GET_COMMAND
#endif
			|| command == TS_SET_LOGGER_SWITCH
			|| command == TS_GET_COMPOSITE_BUFFER_DONE_DIFFERENTLY
			|| command == TS_GET_TEXT
			|| command == TS_CRC_CHECK_COMMAND
			|| command == TS_GET_FIRMWARE_VERSION
			|| command == TS_PERF_TRACE_BEGIN
			|| command == TS_PERF_TRACE_GET_BUFFER
			|| command == TS_GET_CONFIG_ERROR
			|| command == TS_QUERY_BOOTLOADER;
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
	tsChannel->write((const uint8_t*)QUOTE(SIGNATURE_HASH), sizeof(QUOTE(SIGNATURE_HASH)));

	chsnprintf(testOutputBuffer, sizeof(testOutputBuffer), " %d %d", engine->engineState.warnings.lastErrorCode, tsState.testCommandCounter);
	tsChannel->write((const uint8_t*)testOutputBuffer, strlen(testOutputBuffer));

	chsnprintf(testOutputBuffer, sizeof(testOutputBuffer), " uptime=%ds ", (int)getTimeNowS());
	tsChannel->write((const uint8_t*)testOutputBuffer, strlen(testOutputBuffer));

	chsnprintf(testOutputBuffer, sizeof(testOutputBuffer), __DATE__ " %s\r\n", PROTOCOL_TEST_RESPONSE_TAG);
	tsChannel->write((const uint8_t*)testOutputBuffer, strlen(testOutputBuffer));

	if (hasFirmwareError()) {
		const char* error = getCriticalErrorMessage();
		chsnprintf(testOutputBuffer, sizeof(testOutputBuffer), "error=%s\r\n", error);
		tsChannel->write((const uint8_t*)testOutputBuffer, strlen(testOutputBuffer));
	}
	tsChannel->flush();
}

static void handleGetConfigErorr(TsChannelBase* tsChannel) {
	const char* errorMessage = hasFirmwareError() ? getCriticalErrorMessage() : getConfigErrorMessage();
	if (strlen(errorMessage) == 0) {
		// Check for engine's warning code
		errorMessage = engine->engineState.warnings.getWarningMessage();
	}
	tsChannel->sendResponse(TS_CRC, reinterpret_cast<const uint8_t*>(errorMessage), strlen(errorMessage), true);
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
	assertStack("communication", ObdCode::STACK_USAGE_COMMUNICATION, EXPECTED_REMAINING_STACK, -1);

	if (!tsChannel->isReady()) {
		chThdSleepMilliseconds(10);
		return -1;
	}

	tsState.totalCounter++;

	uint8_t firstByte;
	size_t received = tsChannel->readTimeout(&firstByte, 1, TS_COMMUNICATION_TIMEOUT);
#if EFI_SIMULATOR
	logMsg("received %d\r\n", received);
#endif // EFI_SIMULATOR

	if (received != 1) {
		//tunerStudioError("ERROR: no command");
#if EFI_BLUETOOTH_SETUP
		if (tsChannel == getBluetoothChannel()) {
			// no data in a whole second means time to disconnect BT
			// assume there's connection loss and notify the bluetooth init code
			bluetoothSoftwareDisconnectNotify(getBluetoothChannel());
		}
#endif /* EFI_BLUETOOTH_SETUP */
		tsChannel->in_sync = false;
		return -1;
	}

	if (tsInstance.handlePlainCommand(tsChannel, firstByte)) {
		return 0;
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
	size_t expectedSize = incomingPacketSize + TS_PACKET_TAIL_SIZE;

	if ((incomingPacketSize == 0) || (expectedSize > sizeof(tsChannel->scratchBuffer))) {
		if (tsChannel->in_sync) {
			efiPrintf("process_ts: channel=%s invalid size: %d", tsChannel->name, incomingPacketSize);
			tunerStudioError(tsChannel, "process_ts: ERROR: packet size");
			/* send error only if previously we were in sync */
			sendErrorCode(tsChannel, TS_RESPONSE_OUT_OF_RANGE, "invalid size");
		}
		tsChannel->in_sync = false;
		return -1;
	}

	char command;
	if (tsChannel->in_sync) {
		/* we are in sync state, packet size should be correct so lets receive full packet and then check if command is supported
		 * otherwise (if abort reception in middle of packet) it will break synchronization and cause error on next packet */
		received = tsChannel->readTimeout((uint8_t*)(tsChannel->scratchBuffer), expectedSize, TS_COMMUNICATION_TIMEOUT);
		command = tsChannel->scratchBuffer[0];

		if (received != expectedSize) {
			/* print and send error as we were in sync */
			efiPrintf("Got only %d bytes while expecting %d for command 0x%02x", received,
					expectedSize, command);
			tunerStudioError(tsChannel, "ERROR: not enough bytes in stream");
			// MS serial protocol spec: There was a timeout before all data was received. (25ms per character.)
			sendErrorCode(tsChannel, TS_RESPONSE_UNDERRUN, "underrun");
			tsChannel->in_sync = false;
			return -1;
		}

		if (!isKnownCommand(command)) {
			/* print and send error as we were in sync */
			efiPrintf("unexpected command %x", command);
			sendErrorCode(tsChannel, TS_RESPONSE_UNRECOGNIZED_COMMAND, "unknown");
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
			sendErrorCode(tsChannel, TS_RESPONSE_CRC_FAILURE, "crc_issue");
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

#endif // EFI_PROD_CODE || EFI_SIMULATOR
tunerstudio_counters_s tsState;

void tunerStudioError(TsChannelBase* tsChannel, const char *msg) {
	tunerStudioDebug(tsChannel, msg);
	printErrorCounters();
	tsState.errorCounter++;
}

#if EFI_PROD_CODE || EFI_SIMULATOR

extern CommandHandler console_line_callback;

// see also handleQueryCommand
// see also printVersionForConsole
static void handleGetVersion(TsChannelBase* tsChannel) {
	char versionBuffer[32];
	chsnprintf(versionBuffer, sizeof(versionBuffer), "%s v%d@%u", FRONTEND_TITLE_BAR_NAME, getRusEfiVersion(), SIGNATURE_HASH);
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

	tsChannel->writeCrcPacket(TS_RESPONSE_OK, reinterpret_cast<const uint8_t*>(output), outputSize, true);
#if EFI_SIMULATOR
	logMsg("sent [%d]\r\n", outputSize);
#endif // EFI_SIMULATOR
}
#endif // EFI_TEXT_LOGGING

void TunerStudio::handleExecuteCommand(TsChannelBase* tsChannel, char *data, int incomingPacketSize) {
	data[incomingPacketSize] = 0;
	char *trimmed = efiTrim(data);
#if EFI_SIMULATOR
	logMsg("execute [%s]\r\n", trimmed);
#endif // EFI_SIMULATOR
	(console_line_callback)(trimmed);

	tsChannel->writeCrcResponse(TS_RESPONSE_OK);
}

int TunerStudio::handleCrcCommand(TsChannelBase* tsChannel, char *data, int incomingPacketSize) {
	ScopePerf perf(PE::TunerStudioHandleCrcCommand);

	char command = data[0];
	data++;

	const uint16_t* data16 = reinterpret_cast<uint16_t*>(data);

	// only few command have page argument, default page is 0
	uint16_t page = 0;
	uint16_t offset = 0;
	uint16_t count = 0;

	// command may not have offset field - keep safe default value
	// not used by .ini at the moment TODO actually use that version of the command in the .ini
	if (incomingPacketSize >= 3) {
		offset = data16[0];
	}
	// command may not have count/size filed - keep safe default value
	if (incomingPacketSize >= 5) {
		count = data16[1];
	}

	switch(command)
	{
	case TS_OUTPUT_COMMAND:
		if (incomingPacketSize == 1) {
			// Read command with no offset and size - read whole livedata
			count = TS_TOTAL_OUTPUT_SIZE;
		}
		cmdOutputChannels(tsChannel, offset, count);
		break;
	case TS_OUTPUT_ALL_COMMAND:
		offset = 0;
		count = TS_TOTAL_OUTPUT_SIZE;
		// TS will not use this command until ochBlockSize is bigger than blockingFactor and prefer ochGetCommand :(
		cmdOutputChannels(tsChannel, offset, count);
		break;
	case TS_GET_SCATTERED_GET_COMMAND:
#if EFI_TS_SCATTER
		handleScatteredReadCommand(tsChannel);
#else
		criticalError("Slow/wireless mode not supported");
#endif // EFI_TS_SCATTER
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
	case TS_CHUNK_WRITE_COMMAND:
		/* command with page argument */
		page = data16[0];
		offset = data16[1];
		count = data16[2];
		handleWriteChunkCommand(tsChannel, page, offset, count, data + sizeof(TunerStudioPageRWChunkRequest));
		break;
	case TS_CRC_CHECK_COMMAND:
		/* command with page argument */
		page = data16[0];
		offset = data16[1];
		count = data16[2];
		handleCrc32Check(tsChannel, page, offset, count);
		break;
	case TS_BURN_COMMAND:
		/* command with page argument */
		page = data16[0];
		handleBurnCommand(tsChannel, page);
		break;
	case TS_READ_COMMAND:
		/* command with page argument */
		page = data16[0];
		offset = data16[1];
		count = data16[2];
		handlePageReadCommand(tsChannel, page, offset, count);
		break;
	case TS_TEST_COMMAND:
		[[fallthrough]];
	case 'T':
		handleTestCommand(tsChannel);
		break;
	case TS_GET_CONFIG_ERROR:
		handleGetConfigErorr(tsChannel);
		break;
#if EFI_SIMULATOR
	case TS_SIMULATE_CAN:
		void handleWrapCan(TsChannelBase* tsChannel, char *data, int incomingPacketSize);
		handleWrapCan(tsChannel, data, incomingPacketSize - 1);
		break;
#endif // EFI_SIMULATOR
	case TS_IO_TEST_COMMAND:
#if EFI_SIMULATOR || EFI_PROD_CODE
		//TODO: Why did we process `TS_IO_TEST_COMMAND` only in prod code? I've just turned it on for simulator as well, because
		//	I need test this functionality with simulator as well. We need to review the cases when we really need to turn off
		//	`TS_IO_TEST_COMMAND` processing. Do we really need guards below?
		{
			uint16_t subsystem = SWAP_UINT16(data16[0]);
			uint16_t index = SWAP_UINT16(data16[1]);

			executeTSCommand(subsystem, index);
		}
#endif /* EFI_SIMULATOR || EFI_PROD_CODE */
		sendOkResponse(tsChannel);
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
					tsChannel->sendResponse(TS_CRC, reinterpret_cast<const uint8_t*>(toothBuffer->buffer), toothBuffer->nextIdx * sizeof(composite_logger_s), true);

					ReturnToothLoggerBuffer(toothBuffer);
				} else {
					// TS asked for a tooth logger buffer, but we don't have one to give it.
					sendErrorCode(tsChannel, TS_RESPONSE_OUT_OF_RANGE, DO_NOT_LOG);
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
					tsChannel->sendResponse(TS_CRC, buffer.get<uint8_t>(), buffer.size(), true);
				} else {
					// TS asked for a tooth logger buffer, but we don't have one to give it.
					sendErrorCode(tsChannel, TS_RESPONSE_OUT_OF_RANGE, DO_NOT_LOG);
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
	case TS_GET_COMPOSITE_BUFFER_DONE_DIFFERENTLY:
		{
			EnableToothLoggerIfNotEnabled();

			auto toothBuffer = GetToothLoggerBufferNonblocking();

			if (toothBuffer) {
				tsChannel->sendResponse(TS_CRC, reinterpret_cast<const uint8_t*>(toothBuffer->buffer), toothBuffer->nextIdx * sizeof(composite_logger_s), true);

				ReturnToothLoggerBuffer(toothBuffer);
			} else {
				// TS asked for a tooth logger buffer, but we don't have one to give it.
				sendErrorCode(tsChannel, TS_RESPONSE_OUT_OF_RANGE, DO_NOT_LOG);
			}
		}

		break;
#else // EFI_TOOTH_LOGGER
	case TS_GET_COMPOSITE_BUFFER_DONE_DIFFERENTLY:
		sendErrorCode(tsChannel, TS_RESPONSE_OUT_OF_RANGE, DO_NOT_LOG);
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
			tsChannel->sendResponse(TS_CRC, trace.get<uint8_t>(), trace.size(), true);
		}

		break;
#else
	case TS_PERF_TRACE_BEGIN:
		criticalError("TS_PERF_TRACE not supported");
		break;
	case TS_PERF_TRACE_GET_BUFFER:
		criticalError("TS_PERF_TRACE_GET_BUFFER not supported");
		break;
#endif /* ENABLE_PERF_TRACE */
	case TS_QUERY_BOOTLOADER: {
		uint8_t bldata = TS_QUERY_BOOTLOADER_NONE;
#if EFI_USE_OPENBLT
		bldata = TS_QUERY_BOOTLOADER_OPENBLT;
#endif

		tsChannel->sendResponse(TS_CRC, &bldata, 1, false);
		break;
	}
	default:
		sendErrorCode(tsChannel, TS_RESPONSE_UNRECOGNIZED_COMMAND, "unknown_command");
static char tsErrorBuff[80];
		chsnprintf(tsErrorBuff, sizeof(tsErrorBuff), "ERROR: ignoring unexpected command %d [%c]", command, command);
		tunerStudioError(tsChannel, tsErrorBuff);
		return false;
	}

	return true;
}

#endif // EFI_PROD_CODE || EFI_SIMULATOR

bool isTuningVeNow() {
  int tuningDetector = engineConfiguration->isTuningDetectorEnabled ? 0 : 20;
	return !calibrationsVeWriteTimer.hasElapsedSec(tuningDetector);
}

void startTunerStudioConnectivity() {
	// Assert tune & output channel struct sizes
	static_assert(sizeof(persistent_config_s) == TOTAL_CONFIG_SIZE, "TS datapage size mismatch");
	// useful trick if you need to know how far off is the static_assert
	//char (*__kaboom)[sizeof(persistent_config_s)] = 1;
	// another useful trick
	//static_assert(offsetof (engine_configuration_s,HD44780_e) == 700);

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
	addConsoleActionSSS("bluetooth_jdy31", [](const char *baudRate, const char *name, const char *pinCode) {
		bluetoothStart(BLUETOOTH_JDY_31, baudRate, name, pinCode);
	});
#endif /* EFI_BLUETOOTH_SETUP */
}

#endif // EFI_TUNER_STUDIO
