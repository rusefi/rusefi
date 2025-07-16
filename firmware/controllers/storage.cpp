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

StorageStatus storageWrite(int id, const uint8_t *ptr, size_t size)
{
#if EFI_STORAGE_MFS == TRUE
	return mfsStorageWrite(id, ptr, size);
#endif // EFI_STORAGE_MFS

	return StorageStatus::Failed;
}

StorageStatus storageRead(int id, uint8_t *ptr, size_t size)
{
#if EFI_STORAGE_MFS == TRUE
	return mfsStorageRead(id, ptr, size);
#endif // EFI_STORAGE_MFS

	return StorageStatus::NotFound;
}

void initStorage()
{
#if EFI_STORAGE_MFS == TRUE
	// Set long timeout to watchdog as this code is called before any thread is started
	// and no one is feeding watchdog
	startWatchdog(WATCHDOG_FLASH_TIMEOUT_MS);

	initStorageMfs();

	// restart the watchdog with the default timeout
	startWatchdog();
#endif // EFI_STORAGE_MFS
}

#endif // EFI_CONFIGURATION_STORAGE
