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


static bool writeToFlashNow();

static uint32_t pendingWrites = 0;

#define FLASH_WRITER_POLL_INTERVAL_MS	100

chibios_rt::Mailbox<msg_t, 16> flashWriterMb;

#if EFI_STORAGE_MFS == TRUE
/* in case of MFS we need more stack */
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
		return writeToFlashNow();
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

static bool flashAllowWriteID(uint32_t id)
{
	if (id == EFI_SETTINGS_RECORD_ID) {
		// special case, settings can be stored in internal flash

		// writing internal flash can cause cpu freeze
		// check if HW support flash writing while executing
		if (mcuCanFlashWhileRunning()) {
			return true;
		}

#if EFI_SHAFT_POSITION_INPUT
		// MCU does not support write while executing, check if engine is stopped
		if (engine->triggerCentral.directSelfStimulation || engine->rpmCalculator.isStopped()) {
			// rusEfi usually runs on hardware which halts execution while writing to internal flash, so we
			// postpone writes to until engine is stopped. Writes in case of self-stimulation are fine.
			return true;
		}
#endif
	}

	// TODO: we expect every other ID to be stored in external flash...
	return true;
}

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

bool settingsNeedToWriteConfiguration(bool forced) {
	efiPrintf("Scheduling %sconfiguration write", forced ? "FORCED " : "");

	return flashRequestWriteID(EFI_SETTINGS_RECORD_ID, forced);
}

bool settingsLtftRequestWriteToFlash() {
	return flashRequestWriteID(EFI_LTFT_RECORD_ID, false);
}

bool getNeedToWriteConfiguration() {
	return (pendingWrites & BIT(EFI_SETTINGS_RECORD_ID)) != 0;
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

static bool writeToFlashNow() {
	engine->configBurnTimer.reset();

	if (burnWithoutFlash) {
		return true;
	}

	// Set up the container
	persistentState.size = sizeof(persistentState);
	persistentState.version = FLASH_DATA_VERSION;
	persistentState.crc = persistentState.getCrc();

	bool isSuccess = false;
	efiPrintf("Writing pending configuration... %d bytes", sizeof(persistentState));
	efitick_t startNt = getTimeNowNt();

	// Do actual write
#if EFI_STORAGE_MFS == TRUE
	/* In case of MFS:
	 * do we need to have two copies?
	 * do we need to protect it with CRC? */
	if (storageWrite(EFI_SETTINGS_RECORD_ID, (uint8_t *)&persistentState, sizeof(persistentState)) == StorageStatus::Ok) {
		isSuccess = true;
	}
#endif

#if EFI_STORAGE_INT_FLASH == TRUE
	if (!mcuCanFlashWhileRunning()) {
		// there's no wdgStop() for STM32, so we cannot disable it.
		// we just set a long timeout of 5 secs to wait until flash is done.
		startWatchdog(WATCHDOG_FLASH_TIMEOUT_MS);
	}

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
	if ((result1 == FLASH_RETURN_SUCCESS) && (result2 == FLASH_RETURN_SUCCESS)) {
		isSuccess = true;
	}

	if (!mcuCanFlashWhileRunning()) {
		// restart the watchdog with the default timeout
		startWatchdog();
	}
#endif
	efitick_t endNt = getTimeNowNt();
	int elapsed_Ms = US2MS(NT2US(endNt - startNt));

	efiPrintf("Flashing %s after %d mS", isSuccess ? "SUCCESS" : "FAILED", elapsed_Ms);

	resetMaxValues();

	return true;
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

	if ((ret == StorageStatus::Ok) && (validatePersistentState() == StorageStatus::Ok)) {
		return StorageStatus::Ok;
	}
#endif

#if EFI_STORAGE_INT_FLASH == TRUE
	StorageStatus firstCopy = readOneConfigurationCopy(getFlashAddrFirstCopy());

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

	return StorageStatus::NotFound;
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

static void doWriteConfigurationToFlash() {
	// force settings write to storage
	settingsNeedToWriteConfiguration(true);
}

static void doResetConfiguration() {
	resetConfigurationExt(engineConfiguration->engineType);
}

static void doRewriteConfig() {
	doResetConfiguration();
	// force settings write to storage
	settingsNeedToWriteConfiguration(true);
}

static void doWriteLTFT() {
	settingsLtftRequestWriteToFlash();
}

void initFlash() {
	// Init storage(s) if any
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
