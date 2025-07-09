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

#include "storage.h"

#include "runtime_state.h"

static bool needToWriteConfiguration = false;


chibios_rt::Mailbox<msg_t, 16> flashWriterMb;

#if EFI_STORAGE_MFS == TRUE
/* in case of MFS we need more stack */
static THD_WORKING_AREA(flashWriteStack, 3 * UTILITY_THREAD_STACK_SIZE);
#else
static THD_WORKING_AREA(flashWriteStack, UTILITY_THREAD_STACK_SIZE);
#endif

static void flashWriteThread(void*) {
	chRegSetThreadName("flash writer");

	while (true) {
		msg_t ret;
		msg_t msg;
		// Wait for a request to come in
		ret = flashWriterMb.fetch(&msg, TIME_INFINITE);
		if (ret != MSG_OK) {
			continue;
		}

		// Do the actual flash write operation for given ID
		if (msg == EFI_SETTINGS_RECORD_ID) {
			writeToFlashNow();
		} else if (msg == EFI_LTFT_RECORD_ID) {
			engine->module<LongTermFuelTrim>()->store();
		} else {
			efiPrintf("Requested to write unknown record id %ld", msg);
		}
	}
}

// Allow saving setting to flash while engine is runnig.
bool allowFlashWhileRunning() {
	// either MCU supports flashing while executing
	// either we store settings in external storage
	return (mcuCanFlashWhileRunning() || (EFI_STORAGE_MFS_EXTERNAL == TRUE));
}

void setNeedToWriteConfiguration() {
	efiPrintf("Scheduling configuration write");
	needToWriteConfiguration = true;

	if (allowFlashWhileRunning()) {
		// Signal the flash writer thread to wake up and write at its leisure
		msg_t id = EFI_SETTINGS_RECORD_ID;
		flashWriterMb.post(id, TIME_IMMEDIATE);
	}
}

void settingsLtftRequestWriteToFlash() {
	if (allowFlashWhileRunning()) {
		// Signal the flash writer thread to wake up and write at its leisure
		msg_t id = EFI_LTFT_RECORD_ID;
		flashWriterMb.post(id, TIME_IMMEDIATE);
	}
}

bool getNeedToWriteConfiguration() {
	return needToWriteConfiguration;
}

void writeToFlashIfPending() {
	if (allowFlashWhileRunning()) {
		return;
	}

	if (!getNeedToWriteConfiguration()) {
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

	if (burnWithoutFlash) {
		needToWriteConfiguration = false;
		return;
	}

	// Set up the container
	persistentState.size = sizeof(persistentState);
	persistentState.version = FLASH_DATA_VERSION;
	persistentState.crc = persistentState.getCrc();

	// there's no wdgStop() for STM32, so we cannot disable it.
	// we just set a long timeout of 5 secs to wait until flash is done.
	startWatchdog(WATCHDOG_FLASH_TIMEOUT_MS);

	// Do actual write
#if EFI_STORAGE_MFS == TRUE
	/* In case of MFS:
	 * do we need to have two copies?
	 * do we need to protect it with CRC? */
	if (storageWrite(EFI_SETTINGS_RECORD_ID, (uint8_t *)&persistentState, sizeof(persistentState)) == StorageStatus::Ok) {
		// Never used:
		//isSuccess = true;
	}
#endif

#if EFI_STORAGE_INT_FLASH == TRUE
	bool isSuccess = false;
	efiPrintf("Writing pending configuration... %d bytes", sizeof(persistentState));
	efitick_t startNt = getTimeNowNt();

	/**
	 * https://sourceforge.net/p/rusefi/tickets/335/
	 *
	 * In order to preserve at least one copy of the tune in case of electrical issues address of second configuration copy
	 * should be in a different sector of flash since complete flash sectors are erased on write.
	 */
	int result1 = eraseAndFlashCopy(getFlashAddrFirstCopy(), persistentState);
	int result2 = FLASH_RETURN_SUCCESS;
	/* Only if second copy is supported */
	if (getFlashAddrSecondCopy()) {
		result2 = eraseAndFlashCopy(getFlashAddrSecondCopy(), persistentState);
	}

	// handle success/failure
	isSuccess = (result1 == FLASH_RETURN_SUCCESS) && (result2 == FLASH_RETURN_SUCCESS);

	if (isSuccess) {
		efitick_t endNt = getTimeNowNt();
		int elapsed_Ms = US2MS(NT2US(endNt - startNt));

		efiPrintf("FLASH_SUCCESS after %d mS", elapsed_Ms);
	} else {
		efiPrintf("Flashing failed");
	}
#endif

	// restart the watchdog with the default timeout
	startWatchdog();

	resetMaxValues();

	// Write complete, clear the flag
	needToWriteConfiguration = false;
}

static void doResetConfiguration() {
	resetConfigurationExt(engineConfiguration->engineType);
}

static StorageStatus validatePersistentState() {
	auto flashCrc = persistentState.getCrc();

	if (flashCrc != persistentState.crc) {
		// If the stored crc is all 1s, that probably means the flash is actually blank, not that the crc failed.
		if (persistentState.crc == ((decltype(persistentState.crc))-1)) {
			return StorageStatus::NotFound;
		} else {
			return StorageStatus::CrcFailed;
		}
	} else if (persistentState.version != FLASH_DATA_VERSION || persistentState.size != sizeof(persistentState)) {
		return StorageStatus::IncompatibleVersion;
	} else {
        return StorageStatus::Ok;
    }
}

#if EFI_STORAGE_INT_FLASH == TRUE
/**
 * Read single copy of rusEFI configuration from interan flash using custom driver
 */
static StorageStatus readOneConfigurationCopy(flashaddr_t address) {
	efiPrintf("readFromFlash %x", address);

	// error already reported, return
	if (!address) {
		return StorageStatus::NotFound;
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
static StorageStatus readConfiguration() {
#if EFI_STORAGE_MFS == TRUE
	StorageStatus ret = storageRead(EFI_SETTINGS_RECORD_ID, (uint8_t *)&persistentState, sizeof(persistentState));

	if (ret == StorageStatus::Ok) {
		return validatePersistentState();
	}

	return ret;
#endif

#if EFI_STORAGE_INT_FLASH == TRUE
	auto firstCopyAddr = getFlashAddrFirstCopy();

	StorageStatus firstCopy = readOneConfigurationCopy(firstCopyAddr);

	if (firstCopy == StorageStatus::Ok) {
		// First copy looks OK, don't even need to check second copy.
		return firstCopy;
	}

	auto secondyCopyAddr = getFlashAddrSecondCopy();
	/* no second copy? */
	if (secondyCopyAddr == 0x0) {
		return firstCopy;
	}

	efiPrintf("Reading second configuration copy");
	return readOneConfigurationCopy(secondyCopyAddr);
#endif

	// In case of neither of those cases, return that things went OK?
	return StorageStatus::Ok;
}

void readFromFlash() {
	StorageStatus result = readConfiguration();

	switch (result) {
		case StorageStatus::CrcFailed:
			warning(ObdCode::CUSTOM_ERR_FLASH_CRC_FAILED, "flash CRC failed");
			efiPrintf("Need to reset flash to default due to CRC mismatch");
			[[fallthrough]];
		case StorageStatus::NotFound:
		case StorageStatus::Failed:
			resetConfigurationExt(DEFAULT_ENGINE_TYPE);
			break;
		case StorageStatus::IncompatibleVersion:
			// Preserve engine type from old config
			efiPrintf("Resetting due to version mismatch but preserving engine type [%d]", (int)engineConfiguration->engineType);
			resetConfigurationExt(engineConfiguration->engineType);
			break;
		case StorageStatus::Ok:
			// At this point we know that CRC and version number is what we expect. Safe to assume it's a valid configuration.
			applyNonPersistentConfiguration();
			efiPrintf("Read valid configuration from flash!");
			break;
	}

	// we can only change the state after the CRC check
	engineConfiguration->byFirmwareVersion = getRusEfiVersion();
	engine->preCalculate();
}

static void rewriteConfig() {
	doResetConfiguration();
	writeToFlashNow();
}

void initFlash() {
	// Init storage(s) if any
	initStorage();

	addConsoleAction("readconfig", readFromFlash);
	/**
	 * This would write NOW (you should not be doing this while connected to real engine)
	 */
	addConsoleAction(CMD_WRITECONFIG, writeToFlashNow);

	addConsoleAction("ltftwrite", settingsLtftRequestWriteToFlash);
#if EFI_TUNER_STUDIO
	/**
	 * This would schedule write to flash once the engine is stopped
	 */
	addConsoleAction(CMD_BURNCONFIG, requestBurn);
#endif
	addConsoleAction("resetconfig", doResetConfiguration);
	addConsoleAction("rewriteconfig", rewriteConfig);

	chThdCreateStatic(flashWriteStack, sizeof(flashWriteStack), PRIO_FLASH_WRITE, flashWriteThread, nullptr);
}

#endif /* EFI_CONFIGURATION_STORAGE */
