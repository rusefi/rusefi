/*
* @file extra_flash_pages.cpp
 *
 * @date: apr 10, 2026
 * @author FDSoftware
 */

#include "pch.h"
#include "extra_flash_pages.h"
#include "second_tables.h"
#include "lua_config_page.h"
#include "flash_main.h"
#include "persistent_configuration.h"

// Extra pages live at fixed offsets within the primary settings sector (defined in
// extra_flash_pages.h). A fixed offset is critical: if it were derived from
// sizeof(persistent_config_container_s) it would shift whenever the config struct grows,
// causing the read address to change across firmware updates and corrupting stored data.
// PAGE4_SECTOR_OFFSET (72 KB) satisfies STM32H7's 32-byte flash-word alignment and leaves
// headroom above the current largest config; the Lua page sits above it (see header).
static_assert(sizeof(persistent_config_container_s) <= PAGE4_SECTOR_OFFSET,
	"persistent_config_container_s exceeds PAGE4_SECTOR_OFFSET — increase the offset");

void resetExtraPages() {
	secondTablesSetDefaults();
	luaConfigPageSetDefaults();

	// When extracting a new config page from the main config, add a
	// resetXxx() call here
}

void loadExtraPages() {
	loadSecondTables();
	loadLuaConfigPage();

	// When extracting a new config page from the main config, add a
	// loadXxx() call here
}

void loadExtraPage(StorageItemId id) {
	if (id == EFI_SECOND_TABLES_RECORD_ID) {
		loadSecondTables();
	} else if (id == EFI_LUA_PAGE_RECORD_ID) {
		loadLuaConfigPage();
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

	luaConfigPagePrepareForStorage();
	storageWrite(EFI_LUA_PAGE_RECORD_ID,
		luaConfigPageGetStoragePtr(),
		luaConfigPageGetStorageSize());

	// When extracting a new config page from the main config, add a
	// storageWrite() call here
#endif // EFI_CONFIGURATION_STORAGE
}

void* getExtraPageAddr(StorageItemId id) {
	if (id == EFI_SECOND_TABLES_RECORD_ID) {
		return secondTablesGetTsPage();
	} else if (id == EFI_LUA_PAGE_RECORD_ID) {
		return luaConfigPageGetTsPage();
	}

	// When extracting a new config page from the main config, add an
	// else-if branch here
	return nullptr;
}

size_t getExtraPageSize(StorageItemId id) {
	if (id == EFI_SECOND_TABLES_RECORD_ID) {
		return secondTablesGetTsPageSize();
	} else if (id == EFI_LUA_PAGE_RECORD_ID) {
		return luaConfigPageGetTsPageSize();
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
	} else if (id == EFI_LUA_PAGE_RECORD_ID) {
		luaConfigPagePrepareForStorage();
		storageWrite(EFI_LUA_PAGE_RECORD_ID,
			luaConfigPageGetStoragePtr(),
			luaConfigPageGetStorageSize());
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
	} else if (id == EFI_LUA_PAGE_RECORD_ID) {
		return LUA_PAGE_SECTOR_OFFSET;
	}

	// When adding a new extra page, add an else-if branch here
	// returning its sector offset.
	return 0;
}
