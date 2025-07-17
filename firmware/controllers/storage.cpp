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

#if EFI_STORAGE_INT_FLASH == TRUE
#include "storage_flash.h"
#endif

#if EFI_STORAGE_MFS == TRUE
#include "storage_mfs.h"
#endif

#if EFI_STORAGE_SD == TRUE
#include "storage_sd.h"
#endif

static constexpr size_t storagesCount = STORAGE_TOTAL;

static SettingStorageBase *storages[storagesCount];

static const char *storageTypeToName(StorageType type)
{
	switch (type) {
	case STORAGE_INT_FLASH:
		return "INT_FLASH";
	case STORAGE_MFS_INT_FLASH:
		return "MFS_INT_FLASH";
	case STORAGE_MFS_EXT_FLASH:
		return "MFS_EXT_FLASH";
	case STORAGE_SD_CARD:
		return "SD_CARD";
	default:
		break;
	}

	return "UNK";
}

#define for_all_storages	SettingStorageBase* storage = nullptr; \
	for (size_t i = 0; i < storagesCount; i++) \
		if ((storage = storages[i]) != nullptr)

bool storageIsIdAvailable(int id)
{
	for_all_storages {
		if ((storage->isReady()) && (storage->isIdSupported(id))) {
			return true;
		}
	}

	return false;
}

StorageStatus storageWrite(int id, const uint8_t *ptr, size_t size)
{
	bool success = false;
	StorageStatus status = StorageStatus::NotSupported;

	for_all_storages {
		if ((!storage->isReady()) || (!storage->isIdSupported(id))) {
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
		if ((!storage->isReady()) || (!storage->isIdSupported(id))) {
			continue;
		}

		status = storage->read(id, ptr, size);
		if (status == StorageStatus::Ok) {
			success = true;
		}
	}

	return (success ? StorageStatus::Ok : status);
}

bool storageRegisterStorage(StorageType type, SettingStorageBase *storage)
{
	if (type >= STORAGE_TOTAL) {
		return false;
	}

	if (storages[type] != nullptr) {
		/* already registered */
		efiPrintf("Trying to register already exist storage %s", storageTypeToName(type));
		return false;
	}

	storages[type] = storage;
	efiPrintf("Storage %s registered", storageTypeToName(type));

	/* TODO: notificate storage manager about newly added storage */

	return true;
}

bool storageUnregisterStorage(StorageType type)
{
	if (type >= STORAGE_TOTAL) {
		return false;
	}

	if (storages[type] == nullptr) {
		/* already unregistered */
		efiPrintf("Trying to unregister non-exist storage %s", storageTypeToName(type));
		return false;
	}

	storages[type] = nullptr;
	efiPrintf("Storage %s unregistered", storageTypeToName(type));

	return true;
}

void initStorage()
{
#if EFI_STORAGE_INT_FLASH == TRUE
	initStorageFlash();
#endif // STORAGE_SD_CARD

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
