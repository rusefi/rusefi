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
	// SD card is not mounted
	NotAvailable,
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
	EFI_SECOND_TABLES_RECORD_ID = 4,
	EFI_LUA_PAGE_RECORD_ID = 5,
	EFI_TOOTH_PROFILE_RECORD_ID = 6,
	EFI_VR_MODEL_RECORD_ID = 7,

	EFI_STORAGE_TOTAL_ITEMS
};

// exported for unit tests only
bool storageAllowWriteID(StorageItemId id);

// Board-provided handlers for the tooth profile record (m74_9 stores the
// learned per-tooth period profile there). Weak defaults live in
// storage_weaks.cpp. Declared WITHOUT 'weak': a weak declaration at the
// storage manager dispatch call site lets GCC LTO bind the call to a local
// weak body (const folding, which noinline does not prevent) BEFORE the
// linker can select a board's strong override - the dispatch silently
// becomes a no-op and the board handlers get dead-code-eliminated.
bool toothProfileStorageWrite();
bool toothProfileStorageRead();

// Board-provided handlers for the VR amplitude model record (m74_9 stores
// the learned per-level gap shift table + the calibration scalar there).
// Same weak-default/LTO reasoning as the tooth profile handlers above.
bool vrModelStorageWrite();
bool vrModelStorageRead();

// read and write storate item. executed in caller context
StorageStatus storageWrite(StorageItemId id, const uint8_t *ptr, size_t size);
StorageStatus storageRead(StorageItemId id, uint8_t *ptr, size_t size);

// request storage manager to read or write given ID from its own context when storage is ready
bool storageRequestWriteID(StorageItemId id, bool forced);
bool storageReqestReadID(StorageItemId id);

bool storageRegisterStorage(StorageType type, SettingStorageBase *storage);
bool storageUnregisterStorage(StorageType type);

bool storageIsStorageRegistered(StorageType type);

// request storage manager to attach or deattach storage from its own context
bool storagRequestRegisterStorage(StorageType id);
bool storagRequestUnregisterStorage(StorageType id);

/**
 * @return true if an persistentState write is pending
 */
bool getNeedToWriteConfiguration();

/**
 * @return true if any storage write is queued or currently executing
 */
bool storageIsBusy();

/**
 * Wait for queued and in-flight storage writes to complete, up to timeoutMs.
 * Note that deferred writes only execute once the engine stops, so this can
 * time out while the engine is running.
 * @return true if storage is idle, false on timeout
 */
bool storageWaitIdle(unsigned int timeoutMs);

// Bound for storageWaitIdle() before a reboot: covers a full double-copy
// settings write plus MFS worst case garbage collection
#define STORAGE_WAIT_IDLE_TIMEOUT_MS 10000

void initStorage();
