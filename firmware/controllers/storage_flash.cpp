/**
 * @file    storage_flash.cpp
 * @brief   Storage interface to internal MCU flash
 *
 * @date Sep 19, 2013
 * @author Andrey Belomutskiy, (c) 2012-2025
 */

#include "pch.h"

#include "storage.h"

#if EFI_STORAGE_INT_FLASH == TRUE

#include "mpu_util.h"
#include "flash_int.h"

class SettingStorageFlash : public SettingStorageBase {
public:
	bool isReady() override;
	bool isIdSupported(size_t id) override;
	StorageStatus store(size_t id, const uint8_t *ptr, size_t size) override;
	StorageStatus read(size_t id, uint8_t *ptr, size_t size) override;
	StorageStatus format() override;

private:
	flashaddr_t getIdAddress(size_t id);
};

flashaddr_t SettingStorageFlash::getIdAddress(size_t id) {
	if (id == EFI_SETTINGS_RECORD_ID) {
		return getFlashAddrFirstCopy();
	} else if (id == EFI_SETTINGS_BACKUP_RECORD_ID) {
		return getFlashAddrSecondCopy();
	}

	return 0;
}

bool SettingStorageFlash::isReady() {
	return true;
}

bool SettingStorageFlash::isIdSupported(size_t id) {
	return (getIdAddress(id) != 0);
}

StorageStatus SettingStorageFlash::store(size_t id, const uint8_t *ptr, size_t size) {
	flashaddr_t addr = getIdAddress(id);

	if (addr == 0) {
		return StorageStatus::NotSupported;
	}

	efiPrintf("Flash: Writing storage ID %d  @0x%x... %d bytes", id, addr, size);
	efitick_t startNt = getTimeNowNt();

	if (!mcuCanFlashWhileRunning()) {
		// there's no wdgStop() for STM32, so we cannot disable it.
		// we just set a long timeout of 5 secs to wait until flash is done.
		startWatchdog(WATCHDOG_FLASH_TIMEOUT_MS);
	}

	StorageStatus status = StorageStatus::Ok;

	auto err = intFlashErase(addr, size);
	if (FLASH_RETURN_SUCCESS != err) {
		efiPrintf("Flash: failed to erase flash at 0x%08x: %d", addr, err);
		status = StorageStatus::Failed;
	}

	if (status == StorageStatus::Ok) {
		err = intFlashWrite(addr, (const char*)ptr, size);
		if (FLASH_RETURN_SUCCESS != err) {
			efiPrintf("Flash: failed to write flash at 0x%08x: %d", addr, err);
			status = StorageStatus::Failed;
		}
	}

	efitick_t endNt = getTimeNowNt();
	int elapsed_Ms = US2MS(NT2US(endNt - startNt));

	if (!mcuCanFlashWhileRunning()) {
		// restart the watchdog with the default timeout
		startWatchdog();
	}

	efiPrintf("Flash: Write done after %d mS", elapsed_Ms);

	return status;
}

StorageStatus SettingStorageFlash::read(size_t id, uint8_t *ptr, size_t size) {
	flashaddr_t addr = getIdAddress(id);

	if (addr == 0) {
		return StorageStatus::NotSupported;
	}

	efiPrintf("Flash: Reading storage ID %d @0x%x ... %d bytes", id, addr, size);

	intFlashRead(addr, (char *)ptr, size);

	efiPrintf("Flash: Reading done");

	return StorageStatus::Ok;
}

StorageStatus SettingStorageFlash::format() {
	/* TODO: actually erase settings erea(s)? */

	return StorageStatus::NotSupported;
}

static SettingStorageFlash storageFlash;

bool initStorageFlash() {
	return storageRegisterStorage(STORAGE_INT_FLASH, &storageFlash);
}

#endif //EFI_STORAGE_SD
