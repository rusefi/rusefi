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

#include "mfs_storage.h"
#endif

#include "runtime_state.h"

/* TODO: move to efifeatures.h */
#ifndef EFI_STORAGE_MFS_EXTERNAL
#define EFI_STORAGE_MFS_EXTERNAL FALSE
#endif

/* TODO: move to efifeatures.h */
#ifndef EFI_FLASH_WRITE_THREAD
#define EFI_FLASH_WRITE_THREAD FALSE
#endif

// Sanity check
#if (EFI_STORAGE_MFS_EXTERNAL == TRUE) && (EFI_FLASH_WRITE_THREAD == FALSE)
	#error EFI_FLASH_WRITE_THREAD should be enabled if MFS is used for external flash
#endif

static bool needToWriteConfiguration = false;

/* if we use ChibiOS MFS for settings */
#if EFI_STORAGE_MFS == TRUE

/* Managed Flash Storage driver */
MFSDriver mfsd;

extern void boardInitMfs(void);
extern const MFSConfig *boardGetMfsConfig(void);

#endif

/**
 * https://sourceforge.net/p/rusefi/tickets/335/
 *
 * In order to preserve at least one copy of the tune in case of electrical issues address of second configuration copy
 * should be in a different sector of flash since complete flash sectors are erased on write.
 */

#if (EFI_FLASH_WRITE_THREAD == TRUE)
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
		} else {
			efiPrintf("Requested to write unknown record id %ld", msg);
		}
	}
}
#endif // EFI_FLASH_WRITE_THREAD

// Allow saving setting to flash while engine is runnig.
static bool allowFlashWhileRunning() {
	// either MCU supports flashing while executing
	// either we store settings in external storage
	return (mcuCanFlashWhileRunning() || (EFI_STORAGE_MFS_EXTERNAL == TRUE));
}

void setNeedToWriteConfiguration() {
	efiPrintf("Scheduling configuration write");
	needToWriteConfiguration = true;

#if (EFI_FLASH_WRITE_THREAD == TRUE)
	if (allowFlashWhileRunning()) {
		// Signal the flash writer thread to wake up and write at its leisure
		msg_t id = EFI_SETTINGS_RECORD_ID;
		flashWriterMb.post(id, TIME_IMMEDIATE);
	}
#endif // EFI_FLASH_WRITE_THREAD
}

bool getNeedToWriteConfiguration() {
	return needToWriteConfiguration;
}

void writeToFlashIfPending() {
#if (EFI_FLASH_WRITE_THREAD == TRUE)
	// with a flash write thread, the schedule happens directly from
	// setNeedToWriteConfiguration and writing happens from flash thread,
	// so there's nothing to do here
	if (allowFlashWhileRunning()) {
		return;
	}
#endif
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

#if EFI_STORAGE_MFS == TRUE
int mfsStorageWrite(int id, const uint8_t *ptr, size_t size) {
	efiPrintf("Writing storage ID %d ... %d bytes", id, size);
	efitick_t startNt = getTimeNowNt();

	// TODO: add watchdog disable and enable in case MFS is on internal flash and one bank
	mfs_error_t err = mfsWriteRecord(&mfsd, id, size, ptr);

	efitick_t endNt = getTimeNowNt();
	int elapsed_Ms = US2MS(NT2US(endNt - startNt));

	if (err >= MFS_NO_ERROR) {
		efiPrintf("Write done with no errors after %d mS MFS status %d", elapsed_Ms, err);
	} else {
		efiPrintf("Write FAILED after %d with MFS status %d", elapsed_Ms, err);

		return -1;
	}

	return 0;
}

FlashState mfsStorageRead(int id, uint8_t *ptr, size_t size) {
	efiPrintf("Reading storage ID %d ... %d bytes", id, size);

	size_t readed_size = size;
	mfs_error_t err = mfsReadRecord(&mfsd, id, &readed_size, ptr);

	if (err >= MFS_NO_ERROR) {
		if (readed_size != size) {
			efiPrintf("Incorrect size expected %d readed %d", size, readed_size);
			return FlashState::IncompatibleVersion;
		}
		efiPrintf("Reding done with no errors and MFS status %d", err);
	} else {
		efiPrintf("Read FAILED with MFS status %d", err);

		// TODO: or corrupted?
		return FlashState::BlankChip;
	}
	return FlashState::Ok;
}

int mfsStorageFormat()
{
	efitick_t startNt = getTimeNowNt();

	mfs_error_t err;
	err = mfsErase(&mfsd);

	efitick_t endNt = getTimeNowNt();
	int elapsed_Ms = US2MS(NT2US(endNt - startNt));
	efiPrintf("MFS erase done %d mS err %d", elapsed_Ms, err);

	return (err >= MFS_NO_ERROR) ? 0 : -1;
}

#endif //EFI_STORAGE_MFS

void writeToFlashNow() {
	engine->configBurnTimer.reset();

	if (burnWithoutFlash) {
		needToWriteConfiguration = false;
		return;
	}

#if EFI_STORAGE_INT_FLASH == TRUE
	bool isSuccess = false;
	efiPrintf("Writing pending configuration... %d bytes", sizeof(persistentState));
	efitick_t startNt = getTimeNowNt();
#endif

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
	if (mfsStorageWrite(EFI_SETTINGS_RECORD_ID, (uint8_t *)&persistentState, sizeof(persistentState)) == 0) {
		// Never used:
		//isSuccess = true;
	}
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

#if EFI_STORAGE_INT_FLASH == TRUE
	if (isSuccess) {
		efitick_t endNt = getTimeNowNt();
		int elapsed_Ms = US2MS(NT2US(endNt - startNt));

		efiPrintf("FLASH_SUCCESS after %d mS", elapsed_Ms);
	} else {
		efiPrintf("Flashing failed");
	}
#endif

	resetMaxValues();

	// Write complete, clear the flag
	needToWriteConfiguration = false;
}

static void doResetConfiguration() {
	resetConfigurationExt(engineConfiguration->engineType);
}

static FlashState validatePersistentState() {
	auto flashCrc = persistentState.getCrc();

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
	FlashState ret = mfsStorageRead(EFI_SETTINGS_RECORD_ID, (uint8_t *)&persistentState, sizeof(persistentState));

	if (ret == FlashState::Ok) {
		return validatePersistentState();
	}

	return ret;
#endif

#if EFI_STORAGE_INT_FLASH == TRUE
	auto firstCopyAddr = getFlashAddrFirstCopy();

	FlashState firstCopy = readOneConfigurationCopy(firstCopyAddr);

	if (firstCopy == FlashState::Ok) {
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
	return FlashState::Ok;
}

void readFromFlash() {
	FlashState result = readConfiguration();

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
static void eraseStorage() {
	mfsStorageFormat();
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

	addConsoleAction("erasestorage", eraseStorage);
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

#if (EFI_FLASH_WRITE_THREAD == TRUE)
	if (allowFlashWhileRunning()) {
		chThdCreateStatic(flashWriteStack, sizeof(flashWriteStack), PRIO_FLASH_WRITE, flashWriteThread, nullptr);
	} else {
		efiPrintf("EFI_FLASH_WRITE_THREAD is enabled, but not used");
	}
#endif
}

#endif /* EFI_CONFIGURATION_STORAGE */
