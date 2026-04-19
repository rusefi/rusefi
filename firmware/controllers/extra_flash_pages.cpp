/*
* @file extra_flash_pages.cpp
 *
 * @date: apr 10, 2026
 * @author FDSoftware
 */

#include "pch.h"
#include "extra_flash_pages.h"
#include "second_tables.h"
#include "flash_main.h"
#include "persistent_configuration.h"

// Page 4 lives at a fixed offset within the primary settings sector.
// A fixed offset is critical: if the offset were derived from sizeof(persistent_config_container_s)
// it would shift whenever the config struct grows, causing the read address to change across
// firmware updates and corrupting stored page 4 data.
// 72 KB (73728 bytes) satisfies STM32H7's 32-byte flash-word alignment (73728 % 32 == 0),
// fits within a 128 KB flash sector alongside page 4 (73728 + ~1216 < 131072),
// and leaves ~8 KB of headroom above the current largest config (proteus_f7 at 65308 bytes).
static constexpr size_t PAGE4_SECTOR_OFFSET = 72u * 1024u;
static_assert(sizeof(persistent_config_container_s) <= PAGE4_SECTOR_OFFSET,
	"persistent_config_container_s exceeds PAGE4_SECTOR_OFFSET — increase the offset");

void loadExtraPages() {
	loadSecondTables();

	// When extracting a new config page from the main config, add a
	// loadXxx() call here
}

void loadExtraPage(StorageItemId id) {
	if (id == EFI_SECOND_TABLES_RECORD_ID) {
		loadSecondTables();
	}

	// When extracting a new config page from the main config, add an
	// if/else-if branch here dispatching to loadXxx()
}

void burnExtraFlashPages() {
#if EFI_CONFIGURATION_STORAGE
	secondTablesPrepareForStorage();
	storageWrite(EFI_SECOND_TABLES_RECORD_ID,
		secondTablesGetStoragePtr(),
		secondTablesGetStorageSize());

	// When extracting a new config page from the main config, add a
	// storageWrite() call here
#endif // EFI_CONFIGURATION_STORAGE
}

void* getExtraPageAddr(StorageItemId id) {
	if (id == EFI_SECOND_TABLES_RECORD_ID) {
		return secondTablesGetTsPage();
	}

	// When extracting a new config page from the main config, add an
	// else-if branch here
	return nullptr;
}

size_t getExtraPageSize(StorageItemId id) {
	if (id == EFI_SECOND_TABLES_RECORD_ID) {
		return secondTablesGetTsPageSize();
	}

	// When extracting a new config page from the main config, add an
	// else-if branch here
	return 0;
}

void burnExtraFlashPage(StorageItemId id) {
#if EFI_CONFIGURATION_STORAGE
#if (EFI_STORAGE_INT_FLASH == TRUE) && (EFI_STORAGE_MFS != TRUE) && !EFI_SIMULATOR
	// INT_FLASH boards: extra pages share a flash sector with the main config.
	// A full config burn erases the sector, then burnExtraFlashPages()
	// writes all extra pages into the now-blank region in one pass.
	(void)id;
	writeToFlashNow();
#else
	// MFS/SD boards or simulator: write the specific page directly to all backends.
	if (id == EFI_SECOND_TABLES_RECORD_ID) {
		secondTablesPrepareForStorage();
		storageWrite(EFI_SECOND_TABLES_RECORD_ID,
			secondTablesGetStoragePtr(),
			secondTablesGetStorageSize());
	}

	// When extracting a new config page from the main config, add an
	// if/else-if branch here: prepare the page and call storageWrite()
#endif
#else
	(void)id;
#endif // EFI_CONFIGURATION_STORAGE
}

size_t getExtraPageFlashOffset(StorageItemId id) {
	if (id == EFI_SECOND_TABLES_RECORD_ID) {
		return PAGE4_SECTOR_OFFSET;
	}

	// When adding a new extra page, add an else-if branch here
	// returning its sector offset.
	return 0;
}