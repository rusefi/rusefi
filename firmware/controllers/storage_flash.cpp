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
#include "extra_flash_pages.h"

// Compute the flash address for an extra page from its sector offset.
// Returns 0 (unsupported) on platforms where piggybacking doesn't work.
static flashaddr_t getExtraPageFlashAddr(StorageItemId id) {
	size_t offset = getExtraPageFlashOffset(id);
	if (offset == 0) {
		return 0;
	}

#if defined(STM32F7XX) && !defined(EFI_FLASH_USE_1500_OF_2MB)
	// STM32F7 DualBank-2MB (without the extended-flash layout) places the primary
	// settings at sector 12 — a region of 16 KB sectors.  Extra pages at 72+ KB
	// would land outside the sectors erased by the main-config write, AND inside
	// the region overwritten by the backup copy.
	// These boards carry an SD card; extra pages are persisted there instead.
	(void)offset;
	return 0;
#else
	const uintptr_t first = getFlashAddrFirstCopy();
	return first ? (first + offset) : 0;
#endif
}

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

	// Extra pages — address computed from their sector offset
	flashaddr_t extraAddr = getExtraPageFlashAddr(static_cast<StorageItemId>(id));
	if (extraAddr != 0) {
		return extraAddr;
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

	if (getExtraPageFlashOffset(static_cast<StorageItemId>(id)) > 0) {
#if EFI_SIMULATOR
		// Simulator: each address maps to a separate file, so there is no
		// shared-sector constraint — just overwrite directly.
		const auto err = intFlashWrite(addr, reinterpret_cast<const char*>(ptr), size);
		return (err == FLASH_RETURN_SUCCESS) ? StorageStatus::Ok : StorageStatus::Failed;
#else
		// Extra pages share their sector with the main config. They must only be
		// written immediately after a main config write has erased the sector.
		// If the area is not blank, the caller must trigger a full config burn instead.
		if (!intFlashIsErased(addr, size)) {
			return StorageStatus::Failed;
		}
		const auto err = intFlashWrite(addr, reinterpret_cast<const char*>(ptr), size);
		return (err == FLASH_RETURN_SUCCESS) ? StorageStatus::Ok : StorageStatus::Failed;
#endif
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

	if (getExtraPageFlashOffset(static_cast<StorageItemId>(id)) > 0) {
		// If the area is still blank, the extra page has never been saved — signal to use defaults.
		if (intFlashIsErased(addr, size)) {
			return StorageStatus::NotFound;
		}
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
