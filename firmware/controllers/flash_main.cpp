/**
 * @file    flash_main.c
 * @brief	Higher-level logic of saving data into internal flash memory
 *
 *
 * @date Sep 19, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include <ch.h>
#include <hal.h>
#include <string.h>
#include "flash_main.h"
#include "eficonsole.h"
#include "flash.h"
#include "rusefi.h"

//#include "tunerstudio.h"
#include "engine_controller.h"

#include "datalogging.h"

#include "audi_aan.h"
#include "dodge_neon.h"
#include "ford_aspire.h"
#include "ford_fiesta.h"
#include "ford_1995_inline_6.h"
#include "snow_blower.h"
#include "nissan_primera.h"
#include "honda_accord.h"
#include "GY6_139QMB.h"

#include "ec2.h"

static engine_type_e defaultEngineType = FORD_ASPIRE_1996;

static Logging logger;

extern persistent_config_container_s persistentState;

extern engine_configuration_s *engineConfiguration;
extern board_configuration_s *boardConfiguration;

extern engine_configuration2_s * engineConfiguration2;

#define FLASH_ADDR 0x08060000

#define PERSISTENT_SIZE sizeof(persistent_config_container_s)

crc_t flashStateCrc(persistent_config_container_s *state) {
	return calc_crc((const crc_t*) &state->persistentConfiguration, sizeof(persistent_config_s));
}

void writeToFlash(void) {
	persistentState.size = PERSISTENT_SIZE;
	persistentState.version = FLASH_DATA_VERSION;
	scheduleMsg(&logger, "FLASH_DATA_VERSION=%d", persistentState.version);
	crc_t result = flashStateCrc(&persistentState);
	persistentState.value = result;
	scheduleMsg(&logger, "Reseting flash, size=%d", PERSISTENT_SIZE);
	flashErase(FLASH_ADDR, PERSISTENT_SIZE);
	scheduleMsg(&logger, "Flashing with CRC=%d", result);
	efitimems_t nowMs = currentTimeMillis();
	result = flashWrite(FLASH_ADDR, (const char *) &persistentState, PERSISTENT_SIZE);
	scheduleMsg(&logger, "Flash programmed in (ms): %d", currentTimeMillis() - nowMs);
	scheduleMsg(&logger, "Flashed: %d", result);
}

static int isValidCrc(persistent_config_container_s *state) {
	if (state->version != FLASH_DATA_VERSION) {
		scheduleMsg(&logger, "Unexpected flash version: %d", state->version);
		return FALSE;
	}
	crc_t result = flashStateCrc(state);
	int isValidCrc = result == state->value;
	if (!isValidCrc)
		scheduleMsg(&logger, "CRC got %d while %d expected", result, state->value);
	return isValidCrc;
}

static void doResetConfiguration(void) {
	resetConfigurationExt(&logger, engineConfiguration->engineType, engineConfiguration, engineConfiguration2,
			boardConfiguration);
}

static void readFromFlash(void) {
	printMsg(&logger, "readFromFlash()");

	flashRead(FLASH_ADDR, (char *) &persistentState, PERSISTENT_SIZE);

	setDefaultNonPersistentConfiguration(engineConfiguration2);

	if (!isValidCrc(&persistentState) || persistentState.size != PERSISTENT_SIZE) {
		printMsg(&logger, "Need to reset flash to default");
		resetConfigurationExt(&logger, defaultEngineType, engineConfiguration, engineConfiguration2,
				boardConfiguration);
	} else {
		printMsg(&logger, "Got valid configuration from flash!");
		applyNonPersistentConfiguration(&logger, engineConfiguration, engineConfiguration2,
				engineConfiguration->engineType);
	}
	// we can only change the state after the CRC check
	engineConfiguration->firmwareVersion = getRusEfiVersion();
}

void initFlash(void) {
	print("initFlash()\r\n");
	initLogging(&logger, "Flash memory");

	addConsoleAction("readconfig", readFromFlash);
	addConsoleAction("writeconfig", writeToFlash);
	addConsoleAction("resetconfig", doResetConfiguration);

	readFromFlash();
}
