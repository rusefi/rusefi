/**
 * @file    flash_main.cpp
 * @brief	Higher-level logic of saving data into internal flash memory
 *
 *
 * @date Sep 19, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

/* If any setting storage is exist */
#if EFI_CONFIGURATION_STORAGE

#include "mpu_util.h"
#include "flash_main.h"
#include "eficonsole.h"

#include "flash_int.h"

#if EFI_TUNER_STUDIO
#include "tunerstudio.h"
#endif

#if EFI_STORAGE_MFS == TRUE
#include "hal_mfs.h"
#endif

#include "runtime_state.h"

static bool needToWriteConfiguration = false;

/* if we use ChibiOS MFS for settings */
#if EFI_STORAGE_MFS == TRUE

/* Managed Flash Storage driver */
MFSDriver mfsd;

#define EFI_MFS_SETTINGS_RECORD_ID		1

extern void boardInitMfs(void);
extern const MFSConfig *boardGetMfsConfig(void);

#endif

/**
 * https://sourceforge.net/p/rusefi/tickets/335/
 *
 * In order to preserve at least one copy of the tune in case of electrical issues address of second configuration copy
 * should be in a different sector of flash since complete flash sectors are erased on write.
 */

static uint32_t flashStateCrc(const persistent_config_container_s& state) {
	return crc32(&state.persistentConfiguration, sizeof(persistent_config_s));
}

#if EFI_FLASH_WRITE_THREAD
chibios_rt::BinarySemaphore flashWriteSemaphore(/*taken =*/ true);

#if EFI_STORAGE_MFS == TRUE
/* in case of MFS we need more stack */
static THD_WORKING_AREA(flashWriteStack, 3 * UTILITY_THREAD_STACK_SIZE);
#else
static THD_WORKING_AREA(flashWriteStack, UTILITY_THREAD_STACK_SIZE);
#endif
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

void setNeedToWriteConfiguration() {
	efiPrintf("Scheduling configuration write");
	needToWriteConfiguration = true;

#if EFI_FLASH_WRITE_THREAD
	if (allowFlashWhileRunning() || (EFI_STORAGE_MFS_EXTERNAL == TRUE)) {
		// Signal the flash writer thread to wake up and write at its leisure
		flashWriteSemaphore.signal();
	}
#endif // EFI_FLASH_WRITE_THREAD
}

bool getNeedToWriteConfiguration() {
	return needToWriteConfiguration;
}

void writeToFlashIfPending() {
#if EFI_FLASH_WRITE_THREAD
	// with a flash write thread, the schedule happens directly from
	// setNeedToWriteConfiguration, so there's nothing to do here
	return;
#endif
	if (allowFlashWhileRunning() || !getNeedToWriteConfiguration()) {
		// Allow sensor timeouts again now that we're done (and a little time has passed)
		Sensor::inhibitTimeouts(false);
		return;
	}

	// Prevent sensor timeouts while flashing
	Sensor::inhibitTimeouts(true);
	writeToFlashNow();
	// we do not want to allow sensor timeouts right away, we re-enable next time method is invoked
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
		criticalError("Failed to erase flash at 0x%08x: %d", storageAddress, err);
		return err;
	}

	err = intFlashWrite(storageAddress, reinterpret_cast<const char*>(&data), sizeof(TStorage));
	if (FLASH_RETURN_SUCCESS != err) {
		criticalError("Failed to write flash at 0x%08x: %d", storageAddress, err);
		return err;
	}

	return err;
}

bool burnWithoutFlash = false;

void writeToFlashNow() {
	engine->configBurnTimer.reset();
	bool isSuccess = false;

	if (burnWithoutFlash) {
		needToWriteConfiguration = false;
		return;
	}
	efiPrintf("Writing pending configuration... %d bytes", sizeof(persistentState));
	efitick_t startNt = getTimeNowNt();

	// Set up the container
	persistentState.size = sizeof(persistentState);
	persistentState.version = FLASH_DATA_VERSION;
	persistentState.crc = flashStateCrc(persistentState);

	// there's no wdgStop() for STM32, so we cannot disable it.
	// we just set a long timeout of 5 secs to wait until flash is done.
	startWatchdog(WATCHDOG_FLASH_TIMEOUT_MS);

#if EFI_STORAGE_MFS == TRUE
	mfs_error_t err;
	/* In case of MFS:
	 * do we need to have two copies?
	 * do we need to protect it with CRC? */

	err = mfsWriteRecord(&mfsd, EFI_MFS_SETTINGS_RECORD_ID,
						 sizeof(persistentState), (uint8_t *)&persistentState);

	if (err >= MFS_NO_ERROR)
		isSuccess = true;
#endif

#if EFI_STORAGE_INT_FLASH == TRUE
	// Flash two copies
	int result1 = eraseAndFlashCopy(getFlashAddrFirstCopy(), persistentState);
	int result2 = FLASH_RETURN_SUCCESS;
	/* Only if second copy is supported */
	if (getFlashAddrSecondCopy()) {
		result2 = eraseAndFlashCopy(getFlashAddrSecondCopy(), persistentState);
	}

	// handle success/failure
	isSuccess = (result1 == FLASH_RETURN_SUCCESS) && (result2 == FLASH_RETURN_SUCCESS);
#endif

	// restart the watchdog with the default timeout
	startWatchdog();

	if (isSuccess) {
		efitick_t endNt = getTimeNowNt();
		int elapsed_Ms = US2MS(NT2US(endNt - startNt));

#if EFI_STORAGE_MFS == TRUE
		efiPrintf("FLASH_SUCCESS after %d mS MFS status %d", elapsed_Ms, err);
#else
		efiPrintf("FLASH_SUCCESS after %d mS", elapsed_Ms);
#endif
	} else {
		efiPrintf("Flashing failed");
	}

	resetMaxValues();

	// Write complete, clear the flag
	needToWriteConfiguration = false;
}

static void doResetConfiguration() {
	resetConfigurationExt(engineConfiguration->engineType);
}

enum class FlashState {
	Ok,
	CrcFailed,
	IncompatibleVersion,
	// all is well, but we're on a fresh chip with blank memory
	BlankChip,
};

static FlashState validatePersistentState() {
	auto flashCrc = flashStateCrc(persistentState);

	if (flashCrc != persistentState.crc) {
		// If the stored crc is all 1s, that probably means the flash is actually blank, not that the crc failed.
		if (persistentState.crc == ((decltype(persistentState.crc))-1)) {
			return FlashState::BlankChip;
		} else {
			return FlashState::CrcFailed;
		}
	} else if (persistentState.version != FLASH_DATA_VERSION || persistentState.size != sizeof(persistentState)) {
		return FlashState::IncompatibleVersion;
	} else {
        return FlashState::Ok;
    }
}

#if EFI_STORAGE_INT_FLASH == TRUE
/**
 * Read single copy of rusEFI configuration from interan flash using custom driver
 */
static FlashState readOneConfigurationCopy(flashaddr_t address) {
	efiPrintf("readFromFlash %x", address);

	// error already reported, return
	if (!address) {
		return FlashState::BlankChip;
	}

	intFlashRead(address, (char *) &persistentState, sizeof(persistentState));

	return validatePersistentState();
}
#endif

/**
 * this method could and should be executed before we have any
 * connectivity so no console output here
 *
 * in this method we read first copy of configuration in flash. if that first copy has CRC or other issues we read second copy.
 */
static FlashState readConfiguration() {
#if EFI_STORAGE_MFS == TRUE
	size_t settings_size = sizeof(persistentState);
	mfs_error_t err = mfsReadRecord(&mfsd, EFI_MFS_SETTINGS_RECORD_ID,
						&settings_size, (uint8_t *)&persistentState);

	if (err >= MFS_NO_ERROR) {
		// readed size is not exactly the same
		if (settings_size != sizeof(persistentState))
			return FlashState::IncompatibleVersion;
		return validatePersistentState();
	} else {
		return FlashState::BlankChip;
	}
#endif

#if EFI_STORAGE_INT_FLASH == TRUE
	auto firstCopyAddr = getFlashAddrFirstCopy();
	auto secondyCopyAddr = getFlashAddrSecondCopy();

	FlashState firstCopy = readOneConfigurationCopy(firstCopyAddr);

	if (firstCopy == FlashState::Ok) {
		// First copy looks OK, don't even need to check second copy.
		return firstCopy;
	}

	/* no second copy? */
	if (getFlashAddrSecondCopy() == 0x0) {
		return firstCopy;
	}

	efiPrintf("Reading second configuration copy");
	return readOneConfigurationCopy(secondyCopyAddr);
#endif

	// In case of neither of those cases, return that things went OK?
	return FlashState::Ok;
}

void readFromFlash() {
#if HW_CHECK_MODE
	/*
	 * getFlashAddr does device validation, we want validation to be invoked even while we are
	 * HW_CHECK_MODE mode where we would not need actual address
	 * todo: rename method to emphasis the fact of validation check?
	 */
	getFlashAddrFirstCopy();
	getFlashAddrSecondCopy();

	resetConfigurationExt(DEFAULT_ENGINE_TYPE);

	FlashState result = FlashState::Ok;
#else
	FlashState result = readConfiguration();
#endif

	switch (result) {
		case FlashState::CrcFailed:
			warning(ObdCode::CUSTOM_ERR_FLASH_CRC_FAILED, "flash CRC failed");
			efiPrintf("Need to reset flash to default due to CRC mismatch");
			[[fallthrough]];
		case FlashState::BlankChip:
			resetConfigurationExt(DEFAULT_ENGINE_TYPE);
			break;
		case FlashState::IncompatibleVersion:
			// Preserve engine type from old config
			efiPrintf("Resetting due to version mismatch but preserving engine type [%d]", (int)engineConfiguration->engineType);
			resetConfigurationExt(engineConfiguration->engineType);
			break;
		case FlashState::Ok:
			// At this point we know that CRC and version number is what we expect. Safe to assume it's a valid configuration.
			applyNonPersistentConfiguration();
			efiPrintf("Read valid configuration from flash!");
			break;
	}

	// we can only change the state after the CRC check
	engineConfiguration->byFirmwareVersion = getRusEfiVersion();
	memset(persistentState.persistentConfiguration.warning_message , 0, sizeof(persistentState.persistentConfiguration.warning_message));
	engine->preCalculate();
}

static void rewriteConfig() {
	doResetConfiguration();
	writeToFlashNow();
}

#if EFI_STORAGE_MFS == TRUE
static void eraseConfig() {
	efitick_t startNt = getTimeNowNt();

	mfs_error_t err;
	err = mfsErase(&mfsd);

	efitick_t endNt = getTimeNowNt();
	int elapsed_Ms = US2MS(NT2US(endNt - startNt));
	efiPrintf("erase done %d mS err %d", elapsed_Ms, err);
}
#endif

void initFlash() {
#if EFI_STORAGE_MFS == TRUE
	boardInitMfs();
	const MFSConfig *mfsConfig = boardGetMfsConfig();

	/* MFS */
	mfsObjectInit(&mfsd);
	mfs_error_t err = mfsStart(&mfsd, mfsConfig);
	if (err < MFS_NO_ERROR) {
		/* hm...? */
	}

	addConsoleAction("eraseconfig", eraseConfig);
#endif

	addConsoleAction("readconfig", readFromFlash);
	/**
	 * This would write NOW (you should not be doing this while connected to real engine)
	 */
	addConsoleAction(CMD_WRITECONFIG, writeToFlashNow);
#if EFI_TUNER_STUDIO
	/**
	 * This would schedule write to flash once the engine is stopped
	 */
	addConsoleAction(CMD_BURNCONFIG, requestBurn);
#endif
	addConsoleAction("resetconfig", doResetConfiguration);
	addConsoleAction("rewriteconfig", rewriteConfig);

#if EFI_FLASH_WRITE_THREAD
	if (allowFlashWhileRunning() || (EFI_STORAGE_MFS_EXTERNAL == TRUE)) {
		chThdCreateStatic(flashWriteStack, sizeof(flashWriteStack), PRIO_FLASH_WRITE, flashWriteThread, nullptr);
	}
#endif
}

#endif /* EFI_CONFIGURATION_STORAGE */
