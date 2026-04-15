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

void loadExtraPages() {
	initSecondTables();

	// When extracting a new config page from the main config, add an
	// initXxx() call here
}

void loadExtraPage(StorageItemId id) {
	if (id == EFI_SECOND_TABLES_RECORD_ID) {
		initSecondTables();
	}

	// When extracting a new config page from the main config, add an
	// if/else-if branch here dispatching to initXxx()
}

void burnExtraFlashPages() {
#if EFI_PROD_CODE
	secondTablesPrepareForStorage();
	storageWrite(EFI_SECOND_TABLES_RECORD_ID,
		secondTablesGetStoragePtr(),
		secondTablesGetStorageSize());

	// When extracting a new config page from the main config, add a
	// storageWrite() call here
#endif // EFI_PROD_CODE
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
#if EFI_PROD_CODE
#if (EFI_STORAGE_INT_FLASH == TRUE) && (EFI_STORAGE_MFS != TRUE)
	// INT_FLASH boards: extra pages share a flash sector with the main config.
	// A full config burn erases the sector, then burnExtraFlashPages()
	// writes all extra pages into the now-blank region in one pass.
	(void)id;
	writeToFlashNow();
#else
	// MFS or SD-only boards: write the specific page directly to all backends.
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
#endif // EFI_PROD_CODE
}