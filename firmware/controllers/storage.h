/**
 * @file    storage.h
 * @brief
 *
 * @date Jan 4, 2025
 * @author Andrey Gusakov
 */

#pragma once

#ifndef EFI_STORAGE_MFS_EXTERNAL
#define EFI_STORAGE_MFS_EXTERNAL FALSE
#endif

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
	virtual bool isIdSupported(size_t id) = 0;
	virtual StorageStatus store(size_t id, const uint8_t *ptr, size_t size) = 0;
	virtual StorageStatus read(size_t id, uint8_t *ptr, size_t size) = 0;
	virtual StorageStatus format() = 0;
};

StorageStatus storageWrite(int id, const uint8_t *ptr, size_t size);
StorageStatus storageRead(int id, uint8_t *ptr, size_t size);

void initStorage();

// IDs used as MFS record ids and internal RusEFI ids
enum StorageItemId {
	/* 0 is reserved due to MFS limitation */
	EFI_SETTINGS_RECORD_ID = 1,
	EFI_LTFT_RECORD_ID = 2,

	EFI_STORAGE_TOTAL_ITEMS
};
