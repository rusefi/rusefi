/**
 * @file    storage.h
 * @brief
 *
 * @date Jan 4, 2025
 * @author Andrey Gusakov
 */

#pragma once

// Storage status
enum class StorageStatus {
	Ok,
	CrcFailed,
	IncompatibleVersion,
	NotSupported,
	// all is well, but we're on a fresh chip with blank memory
	NotFound,
	// Write failed
	Failed
};

class SettingStorageBase {
public:
	/* is storage ready? */
	virtual bool isReady() = 0;
	/* does storage able to srore given ID? */
	virtual bool isIdSupported(size_t id) = 0;
	/* store given ID */
	virtual StorageStatus store(size_t id, const uint8_t *ptr, size_t size) = 0;
	/* read given ID */
	virtual StorageStatus read(size_t id, uint8_t *ptr, size_t size) = 0;
	/* format/esare storage */
	virtual StorageStatus format() = 0;
};

enum StorageType {
	STORAGE_INT_FLASH = 0,
	STORAGE_MFS_INT_FLASH = 1,
	STORAGE_MFS_EXT_FLASH = 2,
	STORAGE_SD_CARD = 3,

	STORAGE_TOTAL
};

// IDs used as MFS record ids and internal RusEFI ids
enum StorageItemId {
	/* 0 is reserved due to MFS limitation */
	EFI_SETTINGS_RECORD_ID = 1,
	EFI_SETTINGS_BACKUP_RECORD_ID = 2,
	EFI_LTFT_RECORD_ID = 3,

	EFI_STORAGE_TOTAL_ITEMS
};

bool storageIsIdAvailable(int id);
StorageStatus storageWrite(int id, const uint8_t *ptr, size_t size);
StorageStatus storageRead(int id, uint8_t *ptr, size_t size);

bool storageRegisterStorage(StorageType type, SettingStorageBase *storage);
bool storageUnregisterStorage(StorageType type);

void initStorage();
