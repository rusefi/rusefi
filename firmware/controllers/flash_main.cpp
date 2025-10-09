/**
 * @file    flash_main.cpp
 * @brief	Higher-level logic of saving data into internal flash memory
 *
 *
 * @date Sep 19, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if EFI_CONFIGURATION_STORAGE || defined(EFI_UNIT_TEST)
#include "storage.h"
#endif

/* If any setting storage is exist */
#if EFI_CONFIGURATION_STORAGE

#include "mpu_util.h"
#include "flash_main.h"
#include "eficonsole.h"

#include "flash_int.h"

#if EFI_TUNER_STUDIO
#include "tunerstudio.h"
#endif


#include "runtime_state.h"

#endif // EFI_CONFIGURATION_STORAGE

/* If any setting storage is exist */
#if EFI_CONFIGURATION_STORAGE

void writeToFlashNow() {
	efiPrintf("Scheduling FORCED write");

	storageRequestWriteID(EFI_SETTINGS_RECORD_ID, true);
}

void setNeedToWriteConfiguration() {
	efiPrintf("Scheduling write");

	storageRequestWriteID(EFI_SETTINGS_RECORD_ID, false);
}

bool settingsLtftRequestWriteToFlash() {
	return storageRequestWriteID(EFI_LTFT_RECORD_ID, false);
}

/* TODO: extract to persistentState method */
bool writeToFlashNowImpl() {
	engine->configBurnTimer.reset();

	// Set up the container
	persistentState.size = sizeof(persistentState);
	persistentState.version = FLASH_DATA_VERSION;
	persistentState.crc = persistentState.getCrc();

	// Do actual write
	auto result1 = storageWrite(EFI_SETTINGS_RECORD_ID, (uint8_t *)&persistentState, sizeof(persistentState));
	auto result2 = storageWrite(EFI_SETTINGS_BACKUP_RECORD_ID, (uint8_t *)&persistentState, sizeof(persistentState));

	resetMaxValues();

	// Some MCU have no enough flash to store two copies. First one is mandatory.
	return ((result1 == StorageStatus::Ok) &&
			((result2 == StorageStatus::Ok) || (result2 == StorageStatus::NotSupported)));
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

/**
 * this method could and should be executed before we have any
 * connectivity so no console output here
 *
 * in this method we read first copy of configuration in flash. if that first copy has CRC or other issues we read second copy.
 */
static StorageStatus readConfiguration() {
	auto firstCopy = storageRead(EFI_SETTINGS_RECORD_ID, (uint8_t *)&persistentState, sizeof(persistentState));
	if (firstCopy == StorageStatus::Ok) {
		firstCopy = validatePersistentState();
		if (firstCopy == StorageStatus::Ok) {
			return StorageStatus::Ok;
		}
	}

	auto secondCopy = storageRead(EFI_SETTINGS_BACKUP_RECORD_ID, (uint8_t *)&persistentState, sizeof(persistentState));
	if (secondCopy == StorageStatus::Ok) {
		secondCopy = validatePersistentState();
		if (secondCopy == StorageStatus::Ok) {
			return StorageStatus::Ok;
		}
	}

	return firstCopy;
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
		case StorageStatus::NotSupported:
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

static void doWriteConfigurationToFlash() {
	// force settings write to storage
	writeToFlashNow();
}

static void doResetConfiguration() {
	resetConfigurationExt(engineConfiguration->engineType);
}

static void doRewriteConfig() {
	doResetConfiguration();
	// force settings write to storage
	writeToFlashNow();
}

static void doWriteLTFT() {
	settingsLtftRequestWriteToFlash();
}

void initFlash() {
	// Init storage(s) if any
	// Note: may take some time
	initStorage();

	addConsoleAction("readconfig", readFromFlash);
	/**
	 * This would write NOW (you should not be doing this while connected to real engine)
	 */
	addConsoleAction(CMD_WRITECONFIG, doWriteConfigurationToFlash);

	addConsoleAction("ltftwrite", doWriteLTFT);
#if EFI_TUNER_STUDIO
	/**
	 * This would schedule write to flash once the engine is stopped
	 */
	addConsoleAction(CMD_BURNCONFIG, requestBurn);
#endif
	addConsoleAction("resetconfig", doResetConfiguration);
	addConsoleAction("rewriteconfig", doRewriteConfig);
}

#endif /* EFI_CONFIGURATION_STORAGE */
