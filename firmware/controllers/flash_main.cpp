/**
 * @file    flash_main.cpp
 * @brief	Higher-level logic of saving data into internal flash memory
 *
 *
 * @date Sep 19, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if EFI_INTERNAL_FLASH
#include "os_access.h"
#include "flash_main.h"
#include "eficonsole.h"

#include "flash_int.h"

#if EFI_TUNER_STUDIO
#include "tunerstudio.h"
#endif

#if EFI_STORAGE_EXT_SNOR == TRUE
#include "hal_serial_nor.h"
#include "hal_mfs.h"
#endif

#include "runtime_state.h"

static bool needToWriteConfiguration = false;

extern persistent_config_container_s persistentState;

extern engine_configuration_s *engineConfiguration;

/* if we store settings externally */
#if EFI_STORAGE_EXT_SNOR == TRUE

/* Some fields in following struct is used for DMA transfers, so do no cache */
NO_CACHE SNORDriver snor1;

const WSPIConfig WSPIcfg1 = {
	.end_cb			= NULL,
	.error_cb		= NULL,
	.dcr			= STM32_DCR_FSIZE(23U) |	/* 8MB device.          */
					  STM32_DCR_CSHT(1U)		/* NCS 2 cycles delay.  */
};

const SNORConfig snorcfg1 = {
	.busp			= &WSPID1,
	.buscfg			= &WSPIcfg1
};

/* Managed Flash Storage stuff */
MFSDriver mfsd;

const MFSConfig mfsd_nor_config = {
	.flashp			= (BaseFlash *)&snor1,
	.erased			= 0xFFFFFFFFU,
	.bank_size		= 64 * 1024U,
	.bank0_start	= 0U,
	.bank0_sectors	= 128U,	/* 128 * 4 K = 0.5 Mb */
	.bank1_start	= 128U,
	.bank1_sectors	= 128U
};

#define EFI_MSF_SETTINGS_RECORD_ID		1

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

#if EFI_FLASH_WRITE_THREAD
chibios_rt::BinarySemaphore flashWriteSemaphore(/*taken =*/ true);

static THD_WORKING_AREA(flashWriteStack, UTILITY_THREAD_STACK_SIZE);
static void flashWriteThread(void*) {
	chRegSetThreadName("flash writer");

	while (true) {
		// Wait for a request to come in
		flashWriteSemaphore.wait();

		// Do the actual flash write operation
		writeToFlashNow();
	}
}
#endif // EFI_FLASH_WRITE_THREAD

void setNeedToWriteConfiguration(void) {
	efiPrintf("Scheduling configuration write");
	needToWriteConfiguration = true;

#if EFI_FLASH_WRITE_THREAD
	if (allowFlashWhileRunning()) {
		// Signal the flash writer thread to wake up and write at its leisure
		flashWriteSemaphore.signal();
	}
#endif // EFI_FLASH_WRITE_THREAD
}

bool getNeedToWriteConfiguration(void) {
	return needToWriteConfiguration;
}

void writeToFlashIfPending() {
	// with a flash write thread, the schedule happens directly from
	// setNeedToWriteConfiguration, so there's nothing to do here
	if (allowFlashWhileRunning() || !getNeedToWriteConfiguration()) {
		// Allow sensor timeouts again now that we're done (and a little time has passed)
		Sensor::inhibitTimeouts(false);
		return;
	}

	// Prevent sensor timeouts while flashing
	Sensor::inhibitTimeouts(true);
	writeToFlashNow();
}

// Erase and write a copy of the configuration at the specified address
template <typename TStorage>
int eraseAndFlashCopy(flashaddr_t storageAddress, const TStorage& data) {
	// error already reported, return
	if (!storageAddress) {
		return FLASH_RETURN_SUCCESS;
	}

	auto err = intFlashErase(storageAddress, sizeof(TStorage));
	if (FLASH_RETURN_SUCCESS != err) {
		firmwareError(OBD_PCM_Processor_Fault, "Failed to erase flash at 0x%08x: %d", storageAddress, err);
		return err;
	}

	err = intFlashWrite(storageAddress, reinterpret_cast<const char*>(&data), sizeof(TStorage));
	if (FLASH_RETURN_SUCCESS != err) {
		firmwareError(OBD_PCM_Processor_Fault, "Failed to write flash at 0x%08x: %d", storageAddress, err);
		return err;
	}

	return err;
}

bool burnWithoutFlash = false;

void writeToFlashNow(void) {
	bool isSuccess = false;

	if (burnWithoutFlash) {
		needToWriteConfiguration = false;
		return;
	}
	efiPrintf("Writing pending configuration...");

	// Set up the container
	persistentState.size = sizeof(persistentState);
	persistentState.version = FLASH_DATA_VERSION;
	persistentState.value = flashStateCrc(&persistentState);

#if EFI_STORAGE_EXT_SNOR == TRUE
	mfs_error_t err;
	/* In case of MFS:
	 * do we need to have two copies?
	 * do we need to protect it with CRC? */

	err = mfsWriteRecord(&mfsd, EFI_MSF_SETTINGS_RECORD_ID,
						 sizeof(persistentState), (uint8_t *)&persistentState);

	if (err == MFS_NO_ERROR)
		isSuccess = true;
#endif

#if EFI_STORAGE_INT_FLASH == TRUE
	// Flash two copies
	int result1 = eraseAndFlashCopy(getFlashAddrFirstCopy(), persistentState);
	int result2 = eraseAndFlashCopy(getFlashAddrSecondCopy(), persistentState);

	// handle success/failure
	isSuccess = (result1 == FLASH_RETURN_SUCCESS) && (result2 == FLASH_RETURN_SUCCESS);
#endif

	if (isSuccess) {
		efiPrintf("FLASH_SUCCESS");
	} else {
		efiPrintf("Flashing failed");
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
	resetConfigurationExt(engineConfiguration->engineType PASS_ENGINE_PARAMETER_SUFFIX);
}

typedef enum {
	PC_OK = 0,
	CRC_FAILED = 1,
	INCOMPATIBLE_VERSION = 2,
	RESET_REQUESTED = 3,
	PC_ERROR = 4
} persisted_configuration_state_e;

static persisted_configuration_state_e doReadConfiguration(flashaddr_t address) {
	efiPrintf("readFromFlash %x", address);

	// error already reported, return
	if (!address) {
		return CRC_FAILED;
	}

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
static persisted_configuration_state_e readConfiguration() {
	persisted_configuration_state_e result = CRC_FAILED;

	efiAssert(CUSTOM_ERR_ASSERT, getCurrentRemainingStack() > EXPECTED_REMAINING_STACK, "read f", PC_ERROR);

#if EFI_STORAGE_EXT_SNOR == TRUE
	mfs_error_t err;
	size_t settings_size = sizeof(persistentState);
	err = mfsReadRecord(&mfsd, EFI_MSF_SETTINGS_RECORD_ID,
						&settings_size, (uint8_t *)&persistentState);

	if ((err == MFS_NO_ERROR) && (sizeof(persistentState) == settings_size))
		result = PC_OK;
#endif

#if EFI_STORAGE_INT_FLASH == TRUE
	auto firstCopyAddr = getFlashAddrFirstCopy();
	auto secondyCopyAddr = getFlashAddrSecondCopy();

	result = doReadConfiguration(firstCopyAddr);

	if (result != PC_OK) {
		efiPrintf("Reading second configuration copy");
		result = doReadConfiguration(secondyCopyAddr);
	}
#endif

	return result;
}

void readFromFlash() {
	persisted_configuration_state_e result = PC_OK;

#if HW_CHECK_MODE
	/*
	 * getFlashAddr does device validation, we want validation to be invoked even while we are
	 * HW_CHECK_MODE mode where we would not need actual address
	 * todo: rename method to emphasis the fact of validation check?
	 */
	auto firstCopyAddr = getFlashAddrFirstCopy();
	auto secondyCopyAddr = getFlashAddrSecondCopy();

	resetConfigurationExt(DEFAULT_ENGINE_TYPE PASS_ENGINE_PARAMETER_SUFFIX);
#else
	result = readConfiguration();
#endif

	if (result == CRC_FAILED) {
	    // we are here on first boot on brand new chip
		warning(CUSTOM_ERR_FLASH_CRC_FAILED, "flash CRC failed");
		resetConfigurationExt(DEFAULT_ENGINE_TYPE PASS_ENGINE_PARAMETER_SUFFIX);
	} else if (result == INCOMPATIBLE_VERSION) {
		resetConfigurationExt(engineConfiguration->engineType PASS_ENGINE_PARAMETER_SUFFIX);
	} else {
		/**
		 * At this point we know that CRC and version number is what we expect. Safe to assume it's a valid configuration.
		 */
		applyNonPersistentConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
	}

	// we can only change the state after the CRC check
	engineConfiguration->byFirmwareVersion = getRusEfiVersion();
	memset(persistentState.persistentConfiguration.warning_message , 0, ERROR_BUFFER_SIZE);
	validateConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);

	if (result == CRC_FAILED) {
		efiPrintf("Need to reset flash to default due to CRC");
	} else if (result == INCOMPATIBLE_VERSION) {
		efiPrintf("Resetting due to version mismatch but preserving engine type [%d]", engineConfiguration->engineType);
	} else {
		efiPrintf("Read valid configuration from flash!");
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

void initFlash() {
#if EFI_STORAGE_EXT_SNOR == TRUE
	mfs_error_t err;

	/* Initializing and starting snor1 driver.*/
	snorObjectInit(&snor1);
	snorStart(&snor1, &snorcfg1);

	/* MFS */
	mfsObjectInit(&mfsd);
	err = mfsStart(&mfsd, &mfsd_nor_config);
	if (err != MFS_NO_ERROR) {
		/* hm...? */
	}
#endif

	addConsoleAction("readconfig", readFromFlash);
	/**
	 * This would write NOW (you should not be doing this while connected to real engine)
	 */
	addConsoleAction(CMD_WRITECONFIG, writeConfigCommand);
#if EFI_TUNER_STUDIO
	/**
	 * This would schedule write to flash once the engine is stopped
	 */
	addConsoleAction(CMD_BURNCONFIG, requestBurn);
#endif
	addConsoleAction("resetconfig", doResetConfiguration);
	addConsoleAction("rewriteconfig", rewriteConfig);

#if EFI_FLASH_WRITE_THREAD
	if (allowFlashWhileRunning()) {
		chThdCreateStatic(flashWriteStack, sizeof(flashWriteStack), PRIO_FLASH_WRITE, flashWriteThread, nullptr);
	}
#endif
}

#endif /* EFI_INTERNAL_FLASH */
