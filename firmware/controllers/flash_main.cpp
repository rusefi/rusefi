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

#if (EFI_FLASH_SPI == TRUE)
#include "m25q.h"
#endif

#if EFI_TUNER_STUDIO
#include "tunerstudio.h"
#endif


#include "engine_controller.h"

static bool needToWriteConfiguration = false;

EXTERN_ENGINE;
static Logging* logger;

extern persistent_config_container_s persistentState;

extern engine_configuration_s *engineConfiguration;

/* Select internal vs external flash */
#if (EFI_FLASH_SPI == TRUE)

extern M25QDriver W25Flash;
#define FLASH_DEVICE W25Flash

#else

#define FLASH_DEVICE EFLD1

#endif

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

// Erase and write a copy of the configuration at the specified address
template <typename TStorage>
int eraseAndFlashCopy(flashaddr_t storageAddress, const TStorage& data)
{
#if (HAL_USE_FLASH == TRUE)
	flash_error_t ferr;
	const flash_descriptor_t *fdesc;
	flash_offset_t offset;
	flash_sector_t sector;

	/* Base Flash should be argument */
	BaseFlash *bf = getBaseFlash(&FLASH_DEVICE);

	fdesc = flashGetDescriptor(bf);

	offset = (flash_offset_t)storageAddress;
	if (fdesc->address) {
		offset -= (flash_offset_t)fdesc->address;
	}

	if (fdesc->sectors) {
		/* device with non-uniform sectors size */
		for (sector = 0; sector < fdesc->sectors_count; sector++) {
			if (flashGetSectorOffset(bf, sector) == offset)
				break;
		}
		if (sector == fdesc->sectors_count)
			return FLASH_RETURN_NO_PERMISSION;
	} else if (fdesc->sectors_size) {
		/* device with uniform sector size */
		sector = offset / fdesc->sectors_size;
		/* check if aligned */
		if ((sector * fdesc->sectors_size) != offset)
			return FLASH_RETURN_NO_PERMISSION;
	} else {
		return FLASH_RETURN_BAD_FLASH;
	}

	if (flashGetSectorSize(bf, sector) < sizeof(data)) {
		return FLASH_RETURN_BAD_FLASH;
	}

	ferr = flashStartEraseSector(bf, sector);
	if (ferr != FLASH_NO_ERROR) {
		return FLASH_RETURN_BAD_FLASH;
	}

	ferr = flashWaitErase(bf);
	if (ferr != FLASH_NO_ERROR) {
		return FLASH_RETURN_BAD_FLASH;
	}

	ferr = flashProgram(bf, offset, sizeof(data),  reinterpret_cast<const uint8_t*>(&data));
	if (ferr != FLASH_NO_ERROR) {
		return FLASH_RETURN_BAD_FLASH;
	}

	return FLASH_RETURN_SUCCESS;
#else
	intFlashErase(storageAddress, sizeof(TStorage));
	return intFlashWrite(storageAddress, reinterpret_cast<const char*>(&data), sizeof(TStorage));
#endif
}

void writeToFlashNow(void) {
	scheduleMsg(logger, " !!!!!!!!!!!!!!!!!!!! BE SURE NOT WRITE WITH IGNITION ON !!!!!!!!!!!!!!!!!!!!");

	// Set up the container
	persistentState.size = sizeof(persistentState);
	persistentState.version = FLASH_DATA_VERSION;
	persistentState.value = flashStateCrc(&persistentState);

	// Flash two copies
	efitick_t nowNt = getTimeNowNt();
	int result1 = eraseAndFlashCopy(getFlashAddrFirstCopy(), persistentState);
	scheduleMsg(logger, " Copy 1 writen within %d uS", NT2US(getTimeNowNt() - nowNt));
	nowNt = getTimeNowNt();
	int result2 = eraseAndFlashCopy(getFlashAddrSecondCopy(), persistentState);
	scheduleMsg(logger, " Copy 2 writen within %d uS", NT2US(getTimeNowNt() - nowNt));

	// handle success/failure
	bool isSuccess = (result1 == FLASH_RETURN_SUCCESS) && (result2 == FLASH_RETURN_SUCCESS);

	if (isSuccess) {
		scheduleMsg(logger, "FLASH_SUCCESS");
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
	efitick_t nowNt = getTimeNowNt();
#if (HAL_USE_FLASH == TRUE)
	/* Base Flash should be argument */
	BaseFlash *bf = getBaseFlash(&FLASH_DEVICE);
	const flash_descriptor_t *fdesc = flashGetDescriptor(bf);
	flash_offset_t offset = (flash_offset_t)address;

	if (fdesc->address) {
		offset -= (flash_offset_t)fdesc->address;
	}

	(void)flashRead(bf, offset, sizeof(persistentState), (uint8_t *)&persistentState);
#else
	intFlashRead(address, (char *) &persistentState, sizeof(persistentState));
#endif
	scheduleMsg(logger, " Read setting from flash takes %d uS", NT2US(getTimeNowNt() - nowNt));

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
persisted_configuration_state_e readConfiguration(Logging * logger) {
	efiAssert(CUSTOM_ERR_ASSERT, getCurrentRemainingStack() > EXPECTED_REMAINING_STACK, "read f", PC_ERROR);
	persisted_configuration_state_e result = doReadConfiguration(getFlashAddrFirstCopy(), logger);
	if (result != PC_OK) {
		printMsg(logger, "Reading second configuration copy");
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
