/**
 * @file    flash_main.cpp
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

#include "engine_controller.h"

#include "datalogging.h"

#include "ec2.h"

static engine_type_e defaultEngineType = FORD_ASPIRE_1996;

static bool needToWriteConfiguration = false;

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

void setNeedToWriteConfiguration(void) {
	scheduleMsg(&logger, "Scheduling configuration write");
	needToWriteConfiguration = true;
}

bool getNeedToWriteConfiguration(void) {
	return needToWriteConfiguration;
}

void writeToFlashIfPending() {
	if (!getNeedToWriteConfiguration()) {
		return;
	}
	// todo: technically we need a lock here, realistically we should be fine.
	needToWriteConfiguration = false;
	scheduleMsg(&logger, "Writing pending configuration");
	writeToFlash();
}

void writeToFlash(void) {
#if EFI_INTERNAL_FLASH
	persistentState.size = PERSISTENT_SIZE;
	persistentState.version = FLASH_DATA_VERSION;
	scheduleMsg(&logger, "flash compatible with %d", persistentState.version);
	crc_t result = flashStateCrc(&persistentState);
	persistentState.value = result;
	scheduleMsg(&logger, "Reseting flash: size=%d", PERSISTENT_SIZE);
	flashErase(FLASH_ADDR, PERSISTENT_SIZE);
	scheduleMsg(&logger, "Flashing with CRC=%d", result);
	efitimems_t nowMs = currentTimeMillis();
	result = flashWrite(FLASH_ADDR, (const char *) &persistentState, PERSISTENT_SIZE);
	scheduleMsg(&logger, "Flash programmed in (ms): %d", currentTimeMillis() - nowMs);
	scheduleMsg(&logger, "Flashing result: %d", result);
#endif /* EFI_INTERNAL_FLASH */
}

static bool isValidCrc(persistent_config_container_s *state) {
	crc_t result = flashStateCrc(state);
	int isValidCrc_b = result == state->value;
	if (!isValidCrc_b) {
		scheduleMsg(&logger, "CRC got %d while %d expected", result, state->value);
	}
	return isValidCrc_b;
}

static void doResetConfiguration(void) {
	resetConfigurationExt(&logger, engineConfiguration->engineType, engineConfiguration, engineConfiguration2);
}

void readFromFlash(void) {
	printMsg(&logger, "readFromFlash()");

	flashRead(FLASH_ADDR, (char *) &persistentState, PERSISTENT_SIZE);

	//setDefaultNonPersistentConfiguration(engineConfiguration2);

	if (!isValidCrc(&persistentState) || persistentState.size != PERSISTENT_SIZE) {
		printMsg(&logger, "Need to reset flash to default due to CRC");
		resetConfigurationExt(&logger, defaultEngineType, engineConfiguration, engineConfiguration2);
	} else if (persistentState.version != FLASH_DATA_VERSION) {
		printMsg(&logger, "Need to reset flash to default due to version change");
		resetConfigurationExt(&logger, defaultEngineType, engineConfiguration, engineConfiguration2);

	} else {
		printMsg(&logger, "Got valid configuration from flash!");
		applyNonPersistentConfiguration(&logger, engineConfiguration, engineConfiguration2);
	}
	// we can only change the state after the CRC check
	engineConfiguration->firmwareVersion = getRusEfiVersion();
}

void initFlash(void) {
	initLogging(&logger, "Flash memory");

	addConsoleAction("readconfig", readFromFlash);
	addConsoleAction("writeconfig", writeToFlash);
	addConsoleAction("resetconfig", doResetConfiguration);
}
