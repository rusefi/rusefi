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

#include "flash_int.h"
#include "engine_math.h"

#if EFI_TUNER_STUDIO
#include "tunerstudio.h"
#endif

#include "runtime_state.h"

#include "engine_controller.h"

static bool needToWriteConfiguration = false;

EXTERN_ENGINE;
static Logging* logger;

extern persistent_config_container_s persistentState;

extern engine_configuration_s *engineConfiguration;

/**
 * https://sourceforge.net/p/rusefi/tickets/335/
 *
 * In order to preserve at least one copy of the tune in case of electrical issues address of second configuration copy
 * should be in a different sector of flash since complete flash sectors are erased on write.
 */

crc_t flashStateCrc(persistent_config_container_s *state) {
	return calc_crc((const crc_t*) &state->persistentConfiguration, sizeof(persistent_config_s));
}

#if EFI_FLASH_WRITE_THREAD
chibios_rt::BinarySemaphore flashWriteSemaphore(/*taken =*/ true);

static THD_WORKING_AREA(flashWriteStack, UTILITY_THREAD_STACK_SIZE);
static void flashWriteThread(void*) {
	while (true) {
		// Wait for a request to come in
		flashWriteSemaphore.wait();

		// Do the actual flash write operation
		writeToFlashNow();
	}
}
#endif // EFI_FLASH_WRITE_THREAD

void setNeedToWriteConfiguration(void) {
	scheduleMsg(logger, "Scheduling configuration write");
	needToWriteConfiguration = true;

#if EFI_FLASH_WRITE_THREAD
	// Signal the flash writer thread to wake up and write at its leisure
	flashWriteSemaphore.signal();
#endif // EFI_FLASH_WRITE_THREAD
}

bool getNeedToWriteConfiguration(void) {
	return needToWriteConfiguration;
}

void writeToFlashIfPending() {
// with a flash write thread, the schedule happens directly from
// setNeedToWriteConfiguration, so there's nothing to do here
#if !EFI_FLASH_WRITE_THREAD
	if (!getNeedToWriteConfiguration()) {
		return;
	}

	writeToFlashNow();
#endif
}

// Erase and write a copy of the configuration at the specified address
template <typename TStorage>
int eraseAndFlashCopy(flashaddr_t storageAddress, const TStorage& data)
{
	if (FLASH_RETURN_SUCCESS != intFlashErase(storageAddress, sizeof(TStorage))) {
		firmwareError("problem erasing flash at %#08x", storageAddress);
		return;
	}

	return intFlashWrite(storageAddress, reinterpret_cast<const char*>(&data), sizeof(TStorage));
}

void writeToFlashNow(void) {
	scheduleMsg(logger, "Writing pending configuration...");

	// Set up the container
	persistentState.size = sizeof(persistentState);
	persistentState.version = FLASH_DATA_VERSION;
	persistentState.value = flashStateCrc(&persistentState);

	// Flash two copies
	int result1 = eraseAndFlashCopy(getFlashAddrFirstCopy(), persistentState);
	int result2 = eraseAndFlashCopy(getFlashAddrSecondCopy(), persistentState);

	// handle success/failure
	bool isSuccess = (result1 == FLASH_RETURN_SUCCESS) && (result2 == FLASH_RETURN_SUCCESS);

	if (isSuccess) {
		scheduleMsg(logger, "FLASH_SUCCESS");
	} else {
		scheduleMsg(logger, "Flashing failed");
	}
	assertEngineReference();

	resetMaxValues();

	// Write complete, clear the flag
	needToWriteConfiguration = false;
}

static bool isValidCrc(persistent_config_container_s *state) {
	crc_t result = flashStateCrc(state);
	int isValidCrc_b = result == state->value;
	return isValidCrc_b;
}

static void doResetConfiguration(void) {
	resetConfigurationExt(logger, engineConfiguration->engineType PASS_ENGINE_PARAMETER_SUFFIX);
}

typedef enum {
	PC_OK = 0,
	CRC_FAILED = 1,
	INCOMPATIBLE_VERSION = 2,
	RESET_REQUESTED = 3,
	PC_ERROR = 4
} persisted_configuration_state_e;

static persisted_configuration_state_e doReadConfiguration(flashaddr_t address, Logging * logger) {
	scheduleMsg(logger, "readFromFlash %x", address);
	intFlashRead(address, (char *) &persistentState, sizeof(persistentState));

	if (!isValidCrc(&persistentState)) {
		return CRC_FAILED;
	} else if (persistentState.version != FLASH_DATA_VERSION || persistentState.size != sizeof(persistentState)) {
		return INCOMPATIBLE_VERSION;
	} else {
		return PC_OK;
	}
}

/**
 * this method could and should be executed before we have any
 * connectivity so no console output here
 */
static persisted_configuration_state_e readConfiguration(Logging* logger) {
	efiAssert(CUSTOM_ERR_ASSERT, getCurrentRemainingStack() > EXPECTED_REMAINING_STACK, "read f", PC_ERROR);
	persisted_configuration_state_e result = doReadConfiguration(getFlashAddrFirstCopy(), logger);
	if (result != PC_OK) {
		scheduleMsg(logger, "Reading second configuration copy");
		result = doReadConfiguration(getFlashAddrSecondCopy(), logger);
	}

	if (result == CRC_FAILED) {
	    // we are here on first boot on brand new chip
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
	memset(persistentState.persistentConfiguration.warning_message , 0, ERROR_BUFFER_SIZE);
	validateConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
	return result;
}

void readFromFlash() {
	persisted_configuration_state_e result = readConfiguration(logger);

	if (result == CRC_FAILED) {
		scheduleMsg(logger, "Need to reset flash to default due to CRC");
	} else if (result == INCOMPATIBLE_VERSION) {
		scheduleMsg(logger, "Resetting due to version mismatch but preserving engine type [%d]", engineConfiguration->engineType);
	} else {
		scheduleMsg(logger, "Read valid configuration from flash!");
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

#if EFI_FLASH_WRITE_THREAD
	chThdCreateStatic(flashWriteStack, sizeof(flashWriteStack), PRIO_FLASH_WRITE, flashWriteThread, nullptr);
#endif
}

#endif /* EFI_INTERNAL_FLASH */
