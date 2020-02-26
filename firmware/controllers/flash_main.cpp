/**
 * @file    flash_main.cpp
 * @brief	Higher-level logic of saving data into internal flash memory
 *
 *
 * @date Sep 19, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "global.h"
#if EFI_INTERNAL_FLASH
#include "os_access.h"
#include "flash_main.h"
#include "eficonsole.h"

#include "flash.h"
#include "engine_math.h"

// this message is part of console API, see FLASH_SUCCESS_MSG in java code
#define FLASH_SUCCESS_MSG "FLASH_SUCESS"

#if EFI_TUNER_STUDIO
#include "tunerstudio.h"
#endif


#include "engine_controller.h"

static bool needToWriteConfiguration = false;

EXTERN_ENGINE;
static Logging* logger;

extern persistent_config_container_s persistentState;

extern engine_configuration_s *engineConfiguration;

/**
 * this address needs to be above 'flash' region available for firmware
 * todo: an ideal solution would be to define this address in the .ld / .icf mapping file
 */

#define PERSISTENT_SIZE sizeof(persistent_config_container_s)

/**
 * https://sourceforge.net/p/rusefi/tickets/335/
 *
 * In order to preserve at least one copy of the tune in case of electrical issues address of second configuration copy
 * should be in a different sector of flash since complete flash sectors are erased on write.
 */

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

void writeToFlashNow(void) {
	scheduleMsg(logger, " !!!!!!!!!!!!!!!!!!!! BE SURE NOT WRITE WITH IGNITION ON !!!!!!!!!!!!!!!!!!!!");
	persistentState.size = PERSISTENT_SIZE;
	persistentState.version = FLASH_DATA_VERSION;
	scheduleMsg(logger, "flash compatible with %d", persistentState.version);
	crc_t crcResult = flashStateCrc(&persistentState);
	persistentState.value = crcResult;
	scheduleMsg(logger, "Reseting flash: size=%d", PERSISTENT_SIZE);
	flashErase(getFlashAddrFirstCopy(), PERSISTENT_SIZE);
	scheduleMsg(logger, "Flashing with CRC=%d", crcResult);
	efitimems_t nowMs = currentTimeMillis();
	int result = flashWrite(getFlashAddrFirstCopy(), (const char *) &persistentState, PERSISTENT_SIZE);
	flashErase(getFlashAddrSecondCopy(), PERSISTENT_SIZE);
	flashWrite(getFlashAddrSecondCopy(), (const char *) &persistentState, PERSISTENT_SIZE);
	scheduleMsg(logger, "Flash programmed in %dms", currentTimeMillis() - nowMs);
	bool isSuccess = result == FLASH_RETURN_SUCCESS;
	if (isSuccess) {
		scheduleMsg(logger, FLASH_SUCCESS_MSG);
	} else {
		scheduleMsg(logger, "Flashing failed");
	}
	assertEngineReference();

#if EFI_SHAFT_POSITION_INPUT
	resetMaxValues();
#endif
}

static bool isValidCrc(persistent_config_container_s *state) {
	crc_t result = flashStateCrc(state);
	int isValidCrc_b = result == state->value;
	return isValidCrc_b;
}

static void doResetConfiguration(void) {
	resetConfigurationExt(logger, engineConfiguration->engineType PASS_ENGINE_PARAMETER_SUFFIX);
}

persisted_configuration_state_e flashState;

static persisted_configuration_state_e doReadConfiguration(flashaddr_t address, Logging * logger) {
	printMsg(logger, "readFromFlash %x", address);
	flashRead(address, (char *) &persistentState, PERSISTENT_SIZE);

	if (!isValidCrc(&persistentState)) {
		return CRC_FAILED;
	} else if (persistentState.version != FLASH_DATA_VERSION || persistentState.size != PERSISTENT_SIZE) {
		return INCOMPATIBLE_VERSION;
	} else {
		return PC_OK;
	}
}

/**
 * this method could and should be executed before we have any
 * connectivity so no console output here
 */
persisted_configuration_state_e readConfiguration(Logging * logger) {
	efiAssert(CUSTOM_ERR_ASSERT, getCurrentRemainingStack() > EXPECTED_REMAINING_STACK, "read f", PC_ERROR);
	persisted_configuration_state_e result = doReadConfiguration(getFlashAddrFirstCopy(), logger);
	if (result != PC_OK) {
		printMsg(logger, "Reading second configuration copy");
		result = doReadConfiguration(getFlashAddrSecondCopy(), logger);
	}

	if (result == CRC_FAILED) {
		warning(CUSTOM_ERR_FLASH_CRC_FAILED, "flash CRC failed");
		resetConfigurationExt(logger, DEFAULT_ENGINE_TYPE PASS_ENGINE_PARAMETER_SUFFIX);
	} else if (result == INCOMPATIBLE_VERSION) {
		resetConfigurationExt(logger, engineConfiguration->engineType PASS_ENGINE_PARAMETER_SUFFIX);
	} else {
		/**
		 * At this point we know that CRC and version number is what we expect. Safe to assume it's a valid configuration.
		 */
		applyNonPersistentConfiguration(logger PASS_ENGINE_PARAMETER_SUFFIX);
	}
	// we can only change the state after the CRC check
	engineConfiguration->byFirmwareVersion = getRusEfiVersion();
	validateConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
	return result;
}

void readFromFlash(void) {
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

static void writeConfigCommand() {
#if EFI_TUNER_STUDIO
	// on start-up rusEfi would read from working copy of TS while
	// we have a lot of console commands which write into real copy of configuration directly
	// we have a bit of a mess here
	syncTunerStudioCopy();
#endif /* EFI_TUNER_STUDIO */
	writeToFlashNow();
}

void initFlash(Logging *sharedLogger) {
	logger = sharedLogger;

	addConsoleAction("readconfig", readFromFlash);
	/**
	 * This would write NOW (you should not be doing this while connected to real engine)
	 */
	addConsoleAction(CMD_WRITECONFIG, writeConfigCommand);
#if EFI_TUNER_STUDIO
	/**
	 * This would schedule write to flash once the engine is stopped
	 */
	addConsoleAction("burnconfig", requestBurn);
#endif
	addConsoleAction("resetconfig", doResetConfiguration);
	addConsoleAction("rewriteconfig", rewriteConfig);
}

#endif /* EFI_INTERNAL_FLASH */
