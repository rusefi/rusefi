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


static bool writeToFlashNowImpl();

static uint32_t pendingWrites = 0;

#define FLASH_WRITER_POLL_INTERVAL_MS	100

chibios_rt::Mailbox<msg_t, 16> flashWriterMb;

#if (EFI_STORAGE_MFS == TRUE) || (EFI_STORAGE_SD == TRUE)
/* in case of MFS or SD card we need more stack */
static THD_WORKING_AREA(flashWriteStack, 3 * UTILITY_THREAD_STACK_SIZE);
#else
static THD_WORKING_AREA(flashWriteStack, UTILITY_THREAD_STACK_SIZE);
#endif

// force settings save independently of mcuCanFlashWhileRunning()
#define MSG_FLASH_NOW	BIT(30)
#define MSG_ID_MASK		0x1f

static bool flashRequestWriteID(msg_t id, bool forced)
{
	if (forced) {
		id |= MSG_FLASH_NOW;
	}
	// Note: we are not sure about caller context and using universal (but not optimal) chSysGetStatusAndLockX()
	chibios_rt::CriticalSectionLocker csl;

	// Signal the flash writer thread to wake up and write at its leisure
	return (flashWriterMb.postI(id) == MSG_OK);
}

static bool flashWriteID(uint32_t id)
{
	// Do the actual flash write operation for given ID
	if (id == EFI_SETTINGS_RECORD_ID) {
		return writeToFlashNowImpl();
	} else if (id == EFI_LTFT_RECORD_ID) {
		engine->module<LongTermFuelTrim>()->store();
		return true;
	} else {
		efiPrintf("Requested to write unknown record id %ld", id);
		// to clear pending bit
		return true;
	}
	return true;
}
#endif // EFI_CONFIGURATION_STORAGE

/* If any setting storage is exist or we are in unit test */
#if EFI_CONFIGURATION_STORAGE || defined(EFI_UNIT_TEST)
bool flashAllowWriteID(uint32_t id)
{
#if (EFI_STORAGE_INT_FLASH == TRUE) || defined(EFI_UNIT_TEST)
	if ((id == EFI_SETTINGS_RECORD_ID) ||
		(id == EFI_SETTINGS_BACKUP_RECORD_ID)) {
		// special case, settings can be stored in internal flash

		// writing internal flash can cause cpu freeze
		// check if HW support flash writing while executing
		if (mcuCanFlashWhileRunning()) {
			return true;
		}

#if EFI_SHAFT_POSITION_INPUT
		// MCU does not support write while executing, check if engine is stopped
		if (engine->triggerCentral.directSelfStimulation || engine->rpmCalculator.isStopped()) {
			return true;
		} else {
			// rusEfi usually runs on hardware which halts execution while writing to internal flash, so we
			// postpone writes to until engine is stopped. Writes in case of self-stimulation are fine.
			return false;
		}
#endif // EFI_SHAFT_POSITION_INPUT
	}
#endif // EFI_STORAGE_INT_FLASH

	// TODO: we expect every other ID to be stored in external flash...
	return true;
}
#endif // EFI_CONFIGURATION_STORAGE || defined(EFI_UNIT_TEST)

/* If any setting storage is exist */
#if EFI_CONFIGURATION_STORAGE
static void flashWriteThread(void*) {
	chRegSetThreadName("flash writer");

	while (true) {
		msg_t ret;
		msg_t msg;
		bool force = false;

		// Wait for a request to come in or timeout
		ret = flashWriterMb.fetch(&msg, TIME_MS2I(FLASH_WRITER_POLL_INTERVAL_MS));
		if (ret == MSG_OK) {
			if (msg & MSG_FLASH_NOW) {
				force = true;
			}
			uint32_t id = msg & MSG_ID_MASK;

			pendingWrites |= BIT(id);
			if (force) {
				// skip flashAllowWriteID() check
				if (flashWriteID(id)) {
					pendingWrites &= ~BIT(id);
				}
			}
		}

		// check if we can write some of pendind IDs...
		for (size_t id = 0; (id < 32) && pendingWrites; id++) {
			if ((pendingWrites & BIT(id)) == 0) {
				continue;
			}

			if (!flashAllowWriteID(id)) {
				continue;
			}

			if (flashWriteID(id)) {
				pendingWrites &= ~BIT(id);
			}
		}
	}
}

void writeToFlashNow() {
	efiPrintf("Scheduling FORCED write");

	flashRequestWriteID(EFI_SETTINGS_RECORD_ID, true);
}

void setNeedToWriteConfiguration() {
	efiPrintf("Scheduling write");

	flashRequestWriteID(EFI_SETTINGS_RECORD_ID, false);
}

bool settingsLtftRequestWriteToFlash() {
	return flashRequestWriteID(EFI_LTFT_RECORD_ID, false);
}

bool getNeedToWriteConfiguration() {
	return (pendingWrites & BIT(EFI_SETTINGS_RECORD_ID)) != 0;
}

static bool writeToFlashNowImpl() {
	engine->configBurnTimer.reset();

	// Set up the container
	persistentState.size = sizeof(persistentState);
	persistentState.version = FLASH_DATA_VERSION;
	persistentState.crc = persistentState.getCrc();

	// Do actual write
	auto result1 = storageWrite(EFI_SETTINGS_RECORD_ID, (uint8_t *)&persistentState, sizeof(persistentState));
	auto result2 = storageWrite(EFI_SETTINGS_BACKUP_RECORD_ID, (uint8_t *)&persistentState, sizeof(persistentState));

	resetMaxValues();

	return ((result1 == StorageStatus::Ok) && (result2 == StorageStatus::Ok));
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

	chThdCreateStatic(flashWriteStack, sizeof(flashWriteStack), PRIO_FLASH_WRITE, flashWriteThread, nullptr);
}

#endif /* EFI_CONFIGURATION_STORAGE */
