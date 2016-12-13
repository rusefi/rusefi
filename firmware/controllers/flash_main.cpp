/**
 * @file    flash_main.cpp
 * @brief	Higher-level logic of saving data into internal flash memory
 *
 *
 * @date Sep 19, 2013
 * @author Andrey Belomutskiy, (c) 2012-2016
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
	bool isSuccess = result == FLASH_RETURN_SUCCESS;
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
	return isValidCrc_b;
}

static void doResetConfiguration(void) {
	resetConfigurationExt(logger, engineConfiguration->engineType PASS_ENGINE_PARAMETER);
}

persisted_configuration_state_e flashState;

/**
 * this method could and should be executed before we have any
 * connectivity so no console output here
 */
persisted_configuration_state_e readConfiguration(Logging * logger) {
	efiAssert(getRemainingStack(chThdSelf()) > 256, "read f", PC_ERROR);
	flashRead(FLASH_ADDR, (char *) &persistentState, PERSISTENT_SIZE);

	persisted_configuration_state_e result;
	if (!isValidCrc(&persistentState)) {
		result = CRC_FAILED;
		warning(CUSTOM_ERR_FLASH_CRC_FAILED, "flash CRC failed");
		resetConfigurationExt(logger, DEFAULT_ENGINE_TYPE PASS_ENGINE_PARAMETER);
	} else if (persistentState.version != FLASH_DATA_VERSION || persistentState.size != PERSISTENT_SIZE) {
		result = INCOMPATIBLE_VERSION;
		resetConfigurationExt(logger, engineConfiguration->engineType PASS_ENGINE_PARAMETER);
	} else {
		/**
		 * At this point we know that CRC and version number is what we expect. Safe to assume it's a valid configuration.
		 */
		result = OK;
		applyNonPersistentConfiguration(logger PASS_ENGINE_PARAMETER);
	}
	// we can only change the state after the CRC check
	engineConfiguration->firmwareVersion = getRusEfiVersion();
	return result;
}

void readFromFlash(void) {
	printMsg(logger, "readFromFlash()");
	persisted_configuration_state_e result = readConfiguration(logger);

	if (result == CRC_FAILED) {
		printMsg(logger, "Need to reset flash to default due to CRC");
	} else if (result == INCOMPATIBLE_VERSION) {
		printMsg(logger, "Resetting but saving engine type [%d]", engineConfiguration->engineType);
	} else {
		printMsg(logger, "Got valid configuration from flash!");
	}
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
