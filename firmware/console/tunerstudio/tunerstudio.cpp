/**
 * @file	tunerstudio.cpp
 * @brief Integration with EFI Analytics Tuner Studio software
 *
 * todo: merge this file with tunerstudio_algo.c?
 *
 * @date Aug 26, 2013
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
 *
 */

#include "main.h"

#include "engine_state.h"
#include "tunerstudio.h"

#include "main_trigger_callback.h"
#include "flash_main.h"

#include "tunerstudio_algo.h"
#include "tunerstudio_configuration.h"
#include "malfunction_central.h"
#include "console_io.h"
#include "crc.h"

#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)

#if EFI_PROD_CODE
#include "pin_repository.h"
#include "usbconsole.h"
#include "map_averaging.h"
extern SerialUSBDriver SDU1;
#define CONSOLE_DEVICE &SDU1

#define TS_SERIAL_UART_DEVICE &SD3

static SerialConfig tsSerialConfig = { 0, 0, USART_CR2_STOP1_BITS | USART_CR2_LINEN, 0 };
#endif /* EFI_PROD_CODE */

EXTERN_ENGINE;

#define MAX_PAGE_ID 0

// in MS, that's 10 seconds
#define TS_READ_TIMEOUT 10000

#define PROTOCOL  "001"

BaseChannel * getTsSerialDevice(void) {
#if EFI_PROD_CODE
	if (isSerialOverUart()) {
		// if console uses UART then TS uses USB
		return (BaseChannel *) &SDU1;
	} else {
		return (BaseChannel *) TS_SERIAL_UART_DEVICE;
	}
#else
	return (BaseChannel *) TS_SIMULATOR_PORT;
#endif
}

static Logging logger;

extern persistent_config_s configWorkingCopy;
extern persistent_config_container_s persistentState;

static efitimems_t previousWriteReportMs = 0;

static int ts_serail_ready(void) {
#if EFI_PROD_CODE
	if (isSerialOverUart()) {
		// TS uses USB when console uses serial
		return is_usb_serial_ready();
	} else {
		// TS uses serial when console uses USB
		return TRUE;
	}
#else
	return TRUE;
#endif
}

// this thread wants a bit extra stack
static THD_WORKING_AREA(TS_WORKING_AREA, UTILITY_THREAD_STACK_SIZE + 200);

static int tsCounter = 0;

//static TunerStudioWriteValueRequest writeValueRequest;
//static TunerStudioWriteChunkRequest writeChunkRequest;

extern TunerStudioOutputChannels tsOutputChannels;

extern TunerStudioState tsState;

static void printStats(void) {
#if EFI_PROD_CODE
	if (!isSerialOverUart()) {
		scheduleMsg(&logger, "TS RX on %s%d/TX on %s%d @%d", portname(TS_SERIAL_RX_PORT), TS_SERIAL_RX_PIN,
				portname(TS_SERIAL_TX_PORT), TS_SERIAL_TX_PIN, boardConfiguration->tunerStudioSerialSpeed);
	}
#endif /* EFI_PROD_CODE */
	scheduleMsg(&logger, "TunerStudio total/error counter=%d/%d H=%d / O counter=%d size=%d / P=%d / B=%d", tsCounter, tsState.errorCounter, tsState.queryCommandCounter, tsState.outputChannelsCommandCounter,
			sizeof(tsOutputChannels), tsState.readPageCommandsCounter, tsState.burnCommandCounter);
	scheduleMsg(&logger, "TunerStudio W counter=%d / C = %d / P = %d / current page %d", tsState.writeValueCommandCounter, tsState.writeChunkCommandCounter,
			tsState.pageCommandCounter, tsState.currentPageId);
	scheduleMsg(&logger, "page size=%d", sizeof(engine_configuration_s));

//	scheduleMsg(&logger, "analogChartFrequency %d",
//			(int) (&engineConfiguration->analogChartFrequency) - (int) engineConfiguration);
//
//	int fuelMapOffset = (int) (&engineConfiguration->fuelTable) - (int) engineConfiguration;
//	scheduleMsg(&logger, "fuelTable %d", fuelMapOffset);
//
	int offset = (int) (&boardConfiguration->le_formulas) - (int) engineConfiguration;
	scheduleMsg(&logger, "le_formulas %d", offset);
//
	offset = (int) (&engineConfiguration->crankingCycleBins) - (int) engineConfiguration;
	scheduleMsg(&logger, "crankingCycleBins %d", offset);

	offset = (int) (&engineConfiguration->engineCycle) - (int) engineConfiguration;
	scheduleMsg(&logger, "engineCycle %d", offset);
}

static void setTsSpeed(int value) {
	boardConfiguration->tunerStudioSerialSpeed = value;
	printStats();
}

void tunerStudioWriteData(const uint8_t * buffer, int size) {
	chSequentialStreamWrite(getTsSerialDevice(), buffer, size);
}

void tunerStudioDebug(const char *msg) {
#if EFI_TUNER_STUDIO_VERBOSE
	scheduleMsg(&logger, "%s", msg);
	printStats();
#endif
}

char *getWorkingPageAddr(int pageIndex) {
	switch (pageIndex) {
	case 0:
		return (char*) &configWorkingCopy.engineConfiguration;
//	case 1:
//		return (char*) &configWorkingCopy.boardConfiguration;
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
//	case 1:
//		return sizeof(configWorkingCopy.boardConfiguration);
//	case 2:
//	case 3:
//	case 4:
//		return 1024;
	}
	return 0;
}

void handlePageSelectCommand(ts_response_format_e mode, uint16_t pageId) {
	tsState.pageCommandCounter++;

	tsState.currentPageId = pageId;
	scheduleMsg(&logger, "page %d selected", tsState.currentPageId);
	tsSendResponse(mode, NULL, 0);
}

/**
 * This command is needed to make the whole transfer a bit faster
 * @note See also handleWriteValueCommand
 */
void handleWriteChunkCommand(ts_response_format_e mode, short offset, short count, void *content) {
	tsState.writeChunkCommandCounter++;

	scheduleMsg(&logger, "receiving page %d chunk offset %d size %d", tsState.currentPageId, offset, count);

	if (offset > getTunerStudioPageSize(tsState.currentPageId)) {
		scheduleMsg(&logger, "ERROR offset %d", offset);
		tunerStudioError("ERROR: out of range");
		offset = 0;
	}

	if (count > getTunerStudioPageSize(tsState.currentPageId)) {
		tunerStudioError("ERROR: unexpected count");
		scheduleMsg(&logger, "ERROR count %d", count);
		count = 0;
	}

	uint8_t * addr = (uint8_t *) (getWorkingPageAddr(tsState.currentPageId) + offset);
	memcpy(addr, content, count);

	tsSendResponse(mode, NULL, 0);
}

/**
 * 'Write' command receives a single value at a given offset
 * @note Writing values one by one is pretty slow
 */
void handleWriteValueCommand(ts_response_format_e mode, uint16_t page, uint16_t offset, uint8_t value) {
	tsState.writeValueCommandCounter++;

	tsState.currentPageId = page;

//tunerStudioDebug("got W (Write)"); // we can get a lot of these

#if EFI_TUNER_STUDIO_VERBOSE
//	scheduleMsg(&logger, "Page number %d\r\n", pageId); // we can get a lot of these
#endif

//	int size = sizeof(TunerStudioWriteValueRequest);
//	scheduleMsg(&logger, "Reading %d\r\n", size);

	if (offset > getTunerStudioPageSize(tsState.currentPageId)) {
		tunerStudioError("ERROR: out of range2");
		scheduleMsg(&logger, "ERROR offset %d", offset);
		offset = 0;
		return;
	}

	efitimems_t nowMs = currentTimeMillis();
	if (nowMs - previousWriteReportMs > 5) {
		previousWriteReportMs = nowMs;
		scheduleMsg(&logger, "page %d offset %d: value=%d", tsState.currentPageId, offset, value);
	}

	getWorkingPageAddr(tsState.currentPageId)[offset] = value;

//	scheduleMsg(&logger, "va=%d", configWorkingCopy.boardConfiguration.idleValvePin);
}

static void sendErrorCode(void) {
	tunerStudioWriteCrcPacket(TS_RESPONSE_CRC_FAILURE, NULL, 0);
}

void handlePageReadCommand(ts_response_format_e mode, uint16_t pageId, uint16_t offset, uint16_t count) {
	tsState.readPageCommandsCounter++;
	tunerStudioDebug("got R (Read page)");
	tsState.currentPageId = pageId;

#if EFI_TUNER_STUDIO_VERBOSE
	scheduleMsg(&logger, "Page requested: page %d offset=%d count=%d", (int)tsState.currentPageId, offset, count);
#endif

	if (tsState.currentPageId > MAX_PAGE_ID) {
		scheduleMsg(&logger, "invalid Page number %x", tsState.currentPageId);

		// something is not right here
		tsState.currentPageId = 0;
		tunerStudioError("ERROR: invalid page");
		return;
	}

	int size = getTunerStudioPageSize(tsState.currentPageId);

	if (size < offset + count) {
		scheduleMsg(&logger, "invalid offset/count %d/%d", offset, count);
		sendErrorCode();
		return;
	}

	const uint8_t *addr = (const uint8_t *) (getWorkingPageAddr(tsState.currentPageId) + offset);
	tsSendResponse(mode, addr, count);
#if EFI_TUNER_STUDIO_VERBOSE
	scheduleMsg(&logger, "Sending %d done", count);
#endif
}

/**
 * 'Burn' command is a command to commit the changes
 */
void handleBurnCommand(ts_response_format_e mode, uint16_t page) {
	efitimems_t nowMs = currentTimeMillis();
	tsState.burnCommandCounter++;

	tunerStudioDebug("got B (Burn)");

	tsState.currentPageId = page;

#if EFI_TUNER_STUDIO_VERBOSE
	// pointless since we only have one page now
//	scheduleMsg(&logger, "Page number %d", tsState.currentPageId);
#endif

// todo: how about some multi-threading?
	memcpy(&persistentState.persistentConfiguration, &configWorkingCopy, sizeof(persistent_config_s));

#if EFI_INTERNAL_FLASH
	setNeedToWriteConfiguration();
#endif
	incrementGlobalConfigurationVersion();
	tunerStudioWriteCrcPacket(TS_RESPONSE_BURN_OK, NULL, 0);
	scheduleMsg(&logger, "burned in (ms): %d", currentTimeMillis() - nowMs);
}

static TunerStudioReadRequest readRequest;
static short int pageIn;

static bool handlePlainCommand(uint8_t command) {
	if (command == TS_HELLO_COMMAND) {
		scheduleMsg(&logger, "Got naked Query command");
		handleQueryCommand(TS_PLAIN);
		return true;
	} else if (command == 't' || command == 'T') {
		handleTestCommand();
		return true;
	} else if (command == TS_PAGE_COMMAND) {
		int recieved = chSequentialStreamRead(getTsSerialDevice(), (uint8_t *)&pageIn, sizeof(pageIn));
		// todo: validate 'recieved' value
		handlePageSelectCommand(TS_PLAIN, pageIn);
		return true;
	} else if (command == TS_READ_COMMAND) {
		//scheduleMsg(&logger, "Got naked READ PAGE???");
		int recieved = chSequentialStreamRead(getTsSerialDevice(), (uint8_t *)&readRequest, sizeof(readRequest));
		if (recieved != sizeof(readRequest)) {
			// todo: handler error
			return true;
		}
		handlePageReadCommand(TS_PLAIN, readRequest.page, readRequest.offset, readRequest.count);
		return true;
	} else if (command == TS_OUTPUT_COMMAND) {
		//scheduleMsg(&logger, "Got naked Channels???");
		handleOutputChannelsCommand(TS_PLAIN);
		return true;
	} else if (command == 'F') {
		tunerStudioDebug("not ignoring F");
		tunerStudioWriteData((const uint8_t *) PROTOCOL, strlen(PROTOCOL));
		return true;
	} else {
		return false;
	}
}

static bool isKnownCommand(char command) {
	return command == TS_HELLO_COMMAND || command == TS_READ_COMMAND || command == TS_OUTPUT_COMMAND
			|| command == TS_PAGE_COMMAND || command == TS_BURN_COMMAND || command == TS_SINGLE_WRITE_COMMAND
			|| command == TS_CHUNK_WRITE_COMMAND;
}

static uint8_t firstByte;
static uint8_t secondByte;

#define CRC_VALUE_SIZE 4
// todo: double-check this
#define CRC_WRAPPING_SIZE 7

/**
 * we use 'blockingFactor = 256' in rusefi.ini
 * todo: should we just do (256 + CRC_WRAPPING_SIZE) ?
 */

static uint8_t crcIoBuffer[300];




static msg_t tsThreadEntryPoint(void *arg) {
	(void) arg;
	chRegSetThreadName("tunerstudio thread");

	int wasReady = false;
	while (true) {
		int isReady = ts_serail_ready();
		if (!isReady) {
			chThdSleepMilliseconds(10);
			wasReady = false;
			continue;
		}

		if (!wasReady) {
			wasReady = TRUE;
//			scheduleSimpleMsg(&logger, "ts channel is now ready ", hTimeNow());
		}

		tsCounter++;

		int recieved = chSequentialStreamRead(getTsSerialDevice(), &firstByte, 1);
		if (recieved != 1) {
			tunerStudioError("ERROR: no command");
			continue;
		}
//		scheduleMsg(&logger, "Got first=%x=[%c]", firstByte, firstByte);
		if (handlePlainCommand(firstByte))
			continue;

		recieved = chSequentialStreamRead(getTsSerialDevice(), &secondByte, 1);
		if (recieved != 1) {
			tunerStudioError("ERROR: no second");
			continue;
		}
//		scheduleMsg(&logger, "Got secondByte=%x=[%c]", secondByte, secondByte);

		uint32_t incomingPacketSize = firstByte * 256 + secondByte;

		if (incomingPacketSize == 0 || incomingPacketSize > (sizeof(crcIoBuffer) - CRC_WRAPPING_SIZE)) {
			scheduleMsg(&logger, "TunerStudio: invalid size: %d", incomingPacketSize);
			tunerStudioError("ERROR: size");
			sendErrorCode();
			continue;
		}

		recieved = chnReadTimeout(getTsSerialDevice(), crcIoBuffer, 1, MS2ST(TS_READ_TIMEOUT));
		if (recieved != 1) {
			tunerStudioError("ERROR: did not receive command");
			continue;
		}

		char command = crcIoBuffer[0];
		if (!isKnownCommand(command)) {
			scheduleMsg(&logger, "unexpected command %x", command);
			sendErrorCode();
			continue;
		}

//		scheduleMsg(&logger, "TunerStudio: reading %d+4 bytes(s)", incomingPacketSize);

		recieved = chnReadTimeout(getTsSerialDevice(), (uint8_t * ) (crcIoBuffer + 1), incomingPacketSize + CRC_VALUE_SIZE - 1,
				MS2ST(TS_READ_TIMEOUT));
		int expectedSize = incomingPacketSize + CRC_VALUE_SIZE - 1;
		if (recieved != expectedSize) {
			scheduleMsg(&logger, "got ONLY %d for packet size %d/%d for command %c", recieved, incomingPacketSize,
					expectedSize, command);
			tunerStudioError("ERROR: not enough");
			continue;
		}

		uint32_t expectedCrc = *(uint32_t*) (crcIoBuffer + incomingPacketSize);

		expectedCrc = SWAP_UINT32(expectedCrc);

		uint32_t actualCrc = crc32(crcIoBuffer, incomingPacketSize);
		if (actualCrc != expectedCrc) {
			scheduleMsg(&logger, "TunerStudio: CRC %x %x %x %x", crcIoBuffer[incomingPacketSize + 0],
					crcIoBuffer[incomingPacketSize + 1], crcIoBuffer[incomingPacketSize + 2],
					crcIoBuffer[incomingPacketSize + 3]);

			scheduleMsg(&logger, "TunerStudio: command %c actual CRC %x/expected %x", crcIoBuffer[0], actualCrc,
					expectedCrc);
			tunerStudioError("ERROR: CRC issue");
			continue;
		}

//		scheduleMsg(&logger, "TunerStudio: P00-07 %x %x %x %x %x %x %x %x", crcIoBuffer[0], crcIoBuffer[1],
//				crcIoBuffer[2], crcIoBuffer[3], crcIoBuffer[4], crcIoBuffer[5], crcIoBuffer[6], crcIoBuffer[7]);

		int success = tunerStudioHandleCrcCommand(crcIoBuffer, incomingPacketSize);
		if (!success)
			print("got unexpected TunerStudio command %x:%c\r\n", command, command);

	}
#if defined __GNUC__
	return 0;
#endif
}

void syncTunerStudioCopy(void) {
	memcpy(&configWorkingCopy, &persistentState.persistentConfiguration, sizeof(persistent_config_s));
}

/**
 * Adds size to the beginning of a packet and a crc32 at the end. Then send the packet.
 */
void tunerStudioWriteCrcPacket(const uint8_t command, const void *buf, const uint16_t size) {
	// todo: max size validation
	*(uint16_t *) crcIoBuffer = SWAP_UINT16(size + 1);   // packet size including command
	*(uint8_t *) (crcIoBuffer + 2) = command;
	if (size != 0)
		memcpy(crcIoBuffer + 3, buf, size);
	// CRC on whole packet
	uint32_t crc = crc32((void *) (crcIoBuffer + 2), (uint32_t) (size + 1));
	*(uint32_t *) (crcIoBuffer + 2 + 1 + size) = SWAP_UINT32(crc);

//	scheduleMsg(&logger, "TunerStudio: CRC command %x size %d", command, size);

	tunerStudioWriteData(crcIoBuffer, size + 2 + 1 + 4);      // with size, command and CRC
}

void startTunerStudioConnectivity(void) {
	initLogging(&logger, "tuner studio");

	if (sizeof(engine_configuration_s) != getTunerStudioPageSize(0))
		firmwareError("TS page size mismatch: %d/%d", sizeof(engine_configuration_s), getTunerStudioPageSize(0));

	if (sizeof(TunerStudioOutputChannels) != TS_OUTPUT_SIZE)
		firmwareError("TS outputs size mismatch: %d/%d", sizeof(TunerStudioOutputChannels), TS_OUTPUT_SIZE);

	memset(&tsState, 0, sizeof(tsState));
#if EFI_PROD_CODE
	if (isSerialOverUart()) {
		print("TunerStudio over USB serial");
		usb_serial_start();
	} else {

		print("TunerStudio over USART");
		mySetPadMode("tunerstudio rx", TS_SERIAL_RX_PORT, TS_SERIAL_RX_PIN, PAL_MODE_ALTERNATE(TS_SERIAL_AF));
		mySetPadMode("tunerstudio tx", TS_SERIAL_TX_PORT, TS_SERIAL_TX_PIN, PAL_MODE_ALTERNATE(TS_SERIAL_AF));

		tsSerialConfig.speed = boardConfiguration->tunerStudioSerialSpeed;

		sdStart(TS_SERIAL_UART_DEVICE, &tsSerialConfig);
	}
#endif /* EFI_PROD_CODE */
	syncTunerStudioCopy();

	addConsoleAction("tsinfo", printStats);
	addConsoleActionI("set_ts_speed", setTsSpeed);

	chThdCreateStatic(TS_WORKING_AREA, sizeof(TS_WORKING_AREA), NORMALPRIO, tsThreadEntryPoint, NULL);
}

#endif /* EFI_TUNER_STUDIO */
