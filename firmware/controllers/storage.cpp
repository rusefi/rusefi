/**
 * @file    storage.cpp
 * @brief   adapter for unify access to MFS and legacy 'internal flash' storage drivers
 *
 * @date Jan 4, 2025
 * @author Andrey Gusakov
 */

#include "pch.h"

#include "storage.h"

/* If any setting storage is exist */
#if EFI_CONFIGURATION_STORAGE

#if EFI_STORAGE_MFS == TRUE
#include "storage_mfs.h"
#endif

static constexpr size_t storageCount =
#if EFI_STORAGE_MFS == TRUE
	1 +
#endif
	0;

static SettingStorageBase *storages[storageCount];

#define for_all_storages	SettingStorageBase* storage = nullptr; \
	for (size_t i = 0; (i < storageCount) && ((storage = storages[i]) != nullptr); i++)

StorageStatus storageWrite(int id, const uint8_t *ptr, size_t size)
{
	bool success = false;
	StorageStatus status = StorageStatus::NotSupported;

	for_all_storages {
		if (!storage->isIdSupported(id)) {
			continue;
		}

		status = storage->store(id, ptr, size);
		if (status == StorageStatus::Ok) {
			success = true;
		}
	}

	return (success ? StorageStatus::Ok : status);
}

StorageStatus storageRead(int id, uint8_t *ptr, size_t size)
{
	bool success = false;
	StorageStatus status = StorageStatus::NotSupported;

	for_all_storages {
		if (!storage->isIdSupported(id)) {
			continue;
		}

		status = storage->read(id, ptr, size);
		if (status == StorageStatus::Ok) {
			success = true;
		}
	}

	return (success ? StorageStatus::Ok : status);
}

void initStorage()
{
	size_t n = 0;

	// may be unused
	(void)n;
#if EFI_STORAGE_MFS == TRUE
	// Set long timeout to watchdog as this code is called before any thread is started
	// and no one is feeding watchdog
	startWatchdog(WATCHDOG_FLASH_TIMEOUT_MS);

	storages[n] = initStorageMfs();
	if (storages[n]) {
		n++;
	}

	// restart the watchdog with the default timeout
	startWatchdog();
#endif // EFI_STORAGE_MFS
}

#endif // EFI_CONFIGURATION_STORAGE
