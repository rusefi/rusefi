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
#include "persistent_configuration.h"

// Page 4 lives at a fixed offset within the primary settings sector.
// A fixed offset is critical: if the offset were derived from sizeof(persistent_config_container_s)
// it would shift whenever the config struct grows, causing the read address to change across
// firmware updates and corrupting stored page 4 data.
// 72 KB (73728 bytes) satisfies STM32H7's 32-byte flash-word alignment (73728 % 32 == 0),
// fits within a 128 KB flash sector alongside page 4 (73728 + 1208 = 74936 < 131072),
// and leaves ~8 KB of headroom above the current largest config (proteus_f7 at 65308 bytes)
// before the assert needs increasing again.
//
// NOTE: This piggyback approach requires page 4 to land inside the same flash sector that
// the main-config write erases.  On STM32F7 DualBank-2MB boards without extended flash
// (e.g. alphax-4K-GDI, uaefi-pro) the primary-settings region starts at sector 12
// (16 KB sectors); a 72 KB offset would fall outside the erased range AND inside the
// region overwritten by the backup copy — making both store and read broken.  Those
// boards always carry an SD card, so page 4 is persisted there instead.
// We gate this at compile time with STM32F7XX && !EFI_FLASH_USE_1500_OF_2MB.
// STM32H743 (also dual-bank / mcuCanFlashWhileRunning) has 128 KB sectors, so the
// piggyback works there and must NOT be disabled.
static constexpr size_t PAGE4_SECTOR_OFFSET = 72u * 1024u;
static_assert(sizeof(persistent_config_container_s) <= PAGE4_SECTOR_OFFSET,
	"persistent_config_container_s exceeds PAGE4_SECTOR_OFFSET — increase the offset");

static flashaddr_t getFlashAddrPage4() {
#if defined(STM32F7XX) && !defined(EFI_FLASH_USE_1500_OF_2MB)
	// STM32F7 DualBank-2MB (without the extended-flash layout) places the primary
	// settings at sector 12 — a region of 16 KB sectors.  Page 4 at 72 KB would
	// land outside the sectors erased by the main-config write, AND inside the
	// region overwritten by the backup copy — so both store and read are broken.
	// These boards always? carry an SD card; page 4 is persisted there instead.
	return 0;
#else
	const uintptr_t first = getFlashAddrFirstCopy();
	return first ? (first + PAGE4_SECTOR_OFFSET) : 0;
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
	} else if (id == EFI_SECOND_TABLES_RECORD_ID) {
		return getFlashAddrPage4();
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

	if (id == EFI_SECOND_TABLES_RECORD_ID) {
		// Page 4 shares its sector with the main config. It must only be written
		// immediately after a main config write has erased the sector.
		// If the area is not blank, the caller must trigger a full config burn instead.
		if (!intFlashIsErased(addr, size)) {
			return StorageStatus::Failed;
		}
		const auto err = intFlashWrite(addr, reinterpret_cast<const char*>(ptr), size);
		return (err == FLASH_RETURN_SUCCESS) ? StorageStatus::Ok : StorageStatus::Failed;
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

	if (id == EFI_SECOND_TABLES_RECORD_ID) {
		// If the area is still blank, page 4 has never been saved — signal to use defaults.
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
