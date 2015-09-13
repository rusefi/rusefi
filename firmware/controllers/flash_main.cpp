/**
 * @file    flash_main.cpp
 * @brief	Higher-level logic of saving data into internal flash memory
 *
 *
 * @date Sep 19, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include <main.h>

#include "flash_main.h"
#include "eficonsole.h"
#include "flash.h"
#include "rusefi.h"

// this message is part of console API, see FLASH_SUCCESS_MSG in java code
#define FLASH_SUCCESS_MSG "FLASH_SUCESS"

#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
#include "tunerstudio.h"
#endif

#if EFI_INTERNAL_FLASH || defined(__DOXYGEN__)

#include "engine_controller.h"
#include "datalogging.h"
#include "engine.h"

static bool needToWriteConfiguration = false;

EXTERN_ENGINE;
static Logging* logger;

extern persistent_config_container_s persistentState;

extern engine_configuration_s *engineConfiguration;

#define FLASH_ADDR 0x08060000

#define PERSISTENT_SIZE sizeof(persistent_config_container_s)

crc_t flashStateCrc(persistent_config_container_s *state) {
	return calc_crc((const crc_t*) &state->persistentConfiguration, sizeof(persistent_config_s));
}

void setNeedToWriteConfiguration(void) {
	scheduleMsg(logger, "Scheduling configuration write");
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
	scheduleMsg(logger, "Writing pending configuration");
	writeToFlashNow();
}

extern uint32_t maxLockTime;

void writeToFlashNow(void) {
	scheduleMsg(logger, " !!!!!!!!!!!!!!!!!!!! BE SURE NOT WRITE WITH IGNITION ON !!!!!!!!!!!!!!!!!!!!");
	persistentState.size = PERSISTENT_SIZE;
	persistentState.version = FLASH_DATA_VERSION;
	scheduleMsg(logger, "flash compatible with %d", persistentState.version);
	crc_t crcResult = flashStateCrc(&persistentState);
	persistentState.value = crcResult;
	scheduleMsg(logger, "Reseting flash: size=%d", PERSISTENT_SIZE);
	flashErase(FLASH_ADDR, PERSISTENT_SIZE);
	scheduleMsg(logger, "Flashing with CRC=%d", crcResult);
	efitimems_t nowMs = currentTimeMillis();
	int result = flashWrite(FLASH_ADDR, (const char *) &persistentState, PERSISTENT_SIZE);
	scheduleMsg(logger, "Flash programmed in %dms", currentTimeMillis() - nowMs);
	bool_t isSuccess = result == FLASH_RETURN_SUCCESS;
	if (isSuccess) {
		scheduleMsg(logger, FLASH_SUCCESS_MSG);
	} else {
		scheduleMsg(logger, "Flashing failed");
	}
	maxLockTime = 0;
}

static bool isValidCrc(persistent_config_container_s *state) {
	crc_t result = flashStateCrc(state);
	int isValidCrc_b = result == state->value;
	if (!isValidCrc_b) {
		scheduleMsg(logger, "CRC got %d while %d expected", result, state->value);
	}
	return isValidCrc_b;
}

static void doResetConfiguration(void) {
	resetConfigurationExt(logger, engineConfiguration->engineType PASS_ENGINE_PARAMETER);
}

static bool hasValidEngineType(engine_configuration_s *engineConfiguration) {
	uint32_t ordinal = (uint32_t)engineConfiguration->engineType;
	return ordinal < ET_UNUSED && engineConfiguration->headerMagicValue == HEADER_MAGIC_NUMBER;
}

void readFromFlash(void) {
        efiAssertVoid(getRemainingStack(chThdSelf()) > 256, "read f");
	printMsg(logger, "readFromFlash()");
	flashRead(FLASH_ADDR, (char *) &persistentState, PERSISTENT_SIZE);

	if (!isValidCrc(&persistentState)) {
		printMsg(logger, "Need to reset flash to default due to CRC");
		resetConfigurationExt(logger, DEFAULT_ENGINE_TYPE PASS_ENGINE_PARAMETER);
	} else if (persistentState.version == FLASH_DATA_VERSION && persistentState.size == PERSISTENT_SIZE) {
		printMsg(logger, "Got valid configuration from flash!");
		applyNonPersistentConfiguration(logger PASS_ENGINE_PARAMETER);
	} else if (hasValidEngineType(engineConfiguration)) {
		printMsg(logger, "Resetting but saving engine type [%d]", engineConfiguration->engineType);
		resetConfigurationExt(logger, engineConfiguration->engineType PASS_ENGINE_PARAMETER);
	} else {
		printMsg(logger, "Need to reset flash to default due to version change");
		resetConfigurationExt(logger, DEFAULT_ENGINE_TYPE PASS_ENGINE_PARAMETER);
	}
	// we can only change the state after the CRC check
	engineConfiguration->firmwareVersion = getRusEfiVersion();
}

static void rewriteConfig(void) {
	doResetConfiguration();
	writeToFlashNow();
}

void initFlash(Logging *sharedLogger) {
	logger = sharedLogger;

	addConsoleAction("readconfig", readFromFlash);
	/**
	 * This would write NOW (you should not be doing this while connected to real engine)
	 */
	addConsoleAction("writeconfig", writeToFlashNow);
#if EFI_TUNER_STUDIO || defined(__DOXYGEN__)
	/**
	 * This would schedule write to flash once the engine is stopped
	 */
	addConsoleAction("burnconfig", requestBurn);
#endif
	addConsoleAction("resetconfig", doResetConfiguration);
	addConsoleAction("rewriteconfig", rewriteConfig);
}

#endif /* EFI_INTERNAL_FLASH */
