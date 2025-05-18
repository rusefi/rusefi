/**
 * @file    storage.h
 * @brief
 *
 * @date Jan 4, 2025
 * @author Andrey Gusakov
 */

#ifndef EFI_STORAGE_MFS_EXTERNAL
#define EFI_STORAGE_MFS_EXTERNAL FALSE
#endif

// Sanity check
#if (EFI_STORAGE_MFS_EXTERNAL == TRUE) && (EFI_FLASH_WRITE_THREAD == FALSE)
	#error EFI_FLASH_WRITE_THREAD should be enabled if MFS is used for external flash
#endif

// Storage status
enum class StorageStatus {
	Ok,
	CrcFailed,
	IncompatibleVersion,
	// all is well, but we're on a fresh chip with blank memory
	NotFound,
	// Write failed
	Failed
};

StorageStatus storageWrite(int id, const uint8_t *ptr, size_t size);
StorageStatus storageRead(int id, uint8_t *ptr, size_t size);

void initStorage();

// IDs used as MFS record ids and internal RusEFI ids
// Convert to enum/class
#define EFI_SETTINGS_RECORD_ID		1
#define EFI_LTFT_RECORD_ID			2
