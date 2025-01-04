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
#include "mfs_storage.h"
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
	initStorageMfs();
#endif // EFI_STORAGE_MFS
}

#endif // EFI_CONFIGURATION_STORAGE
