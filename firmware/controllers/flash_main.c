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

static engine_type_e defaultEngineType = FORD_ASPIRE_1996;
//static engine_type_e defaultEngineType = DODGE_NEON_1995;

static Logging logger;

#if defined __GNUC__
FlashState flashState __attribute__((section(".ccm")));
#else
FlashState flashState;
#endif

engine_configuration_s *engineConfiguration = &flashState.persistentConfiguration.engineConfiguration;
board_configuration_s *boardConfiguration = &flashState.persistentConfiguration.boardConfiguration;

extern engine_configuration2_s * engineConfiguration2;

#define FLASH_ADDR 0x08060000

#define FLASH_USAGE sizeof(FlashState)

crc flashStateCrc(FlashState *state) {
	return calc_crc((const crc*) &state->persistentConfiguration, sizeof(persistent_config_s));
}

void writeToFlash(void) {
	flashState.version = FLASH_DATA_VERSION;
	scheduleMsg(&logger, "FLASH_DATA_VERSION=%d", flashState.version);
	crc result = flashStateCrc(&flashState);
	flashState.value = result;
	scheduleMsg(&logger, "Reseting flash=%d", FLASH_USAGE);
	flashErase(FLASH_ADDR, FLASH_USAGE);
	scheduleMsg(&logger, "Flashing with CRC=%d", result);
	efitimems_t nowMs = currentTimeMillis();
	result = flashWrite(FLASH_ADDR, (const char *) &flashState, FLASH_USAGE);
	scheduleMsg(&logger, "Flash programmed in (ms): %d", currentTimeMillis() - nowMs);
	scheduleMsg(&logger, "Flashed: %d", result);
}

static int isValidCrc(FlashState *state) {
	if (state->version != FLASH_DATA_VERSION) {
		scheduleMsg(&logger, "Unexpected flash version: %d", state->version);
		return FALSE;
	}
	crc result = flashStateCrc(state);
	int isValidCrc = result == state->value;
	if (!isValidCrc)
		scheduleMsg(&logger, "CRC got %d while %d expected", result, state->value);
	return isValidCrc;
}

static void doResetConfiguration(void) {
	resetConfigurationExt(engineConfiguration->engineType, engineConfiguration, engineConfiguration2, boardConfiguration);
}

static void readFromFlash(void) {

	flashRead(FLASH_ADDR, (char *) &flashState, FLASH_USAGE);

	setDefaultNonPersistentConfiguration(engineConfiguration2);

	if (!isValidCrc(&flashState)) {
		scheduleMsg(&logger, "Need to reset flash to default");
		resetConfigurationExt(defaultEngineType, engineConfiguration, engineConfiguration2, boardConfiguration);
	} else {
		scheduleMsg(&logger, "Got valid configuration from flash!");
		applyNonPersistentConfiguration(engineConfiguration, engineConfiguration2, engineConfiguration->engineType);
	}
	// we can only change the state after the CRC check
	engineConfiguration->firmwareVersion = getRusEfiVersion();
}

void initFlash(void) {
	initLogging(&logger, "Flash memory");
	print("initFlash()\r\n");


	addConsoleAction("readconfig", readFromFlash);
	addConsoleAction("writeconfig", writeToFlash);
	addConsoleAction("resetconfig", doResetConfiguration);

	readFromFlash();
}
