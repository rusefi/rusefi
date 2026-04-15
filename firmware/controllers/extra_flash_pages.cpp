/*
* @file extra_flash_pages.cpp
 *
 * @date: apr 10, 2026
 * @author FDSoftware
 */

#include "pch.h"
#include "extra_flash_pages.h"
#include "flash_main.h"

void loadExtraPages() {

	// When extracting a new config page from the main config, add an
	// initXxx() call here
}

void loadExtraPage(StorageItemId id) {
	(void)id;

	// When extracting a new config page from the main config, add an
	// if/else-if branch here dispatching to initXxx()
}

void burnExtraFlashPages() {
#if EFI_PROD_CODE
	// When extracting a new config page from the main config, add a
	// storageWrite() call here
#endif // EFI_PROD_CODE
}

void burnExtraFlashPage(const StorageItemId id) {
#if EFI_PROD_CODE
#if (EFI_STORAGE_INT_FLASH == TRUE) && (EFI_STORAGE_MFS != TRUE)
	// INT_FLASH boards: extra pages share a flash sector with the main config.
	// A full config burn erases the sector, then burnExtraFlashPages()
	// writes all extra pages into the now-blank region in one pass.
	(void)id;
	writeToFlashNow();
#else
	// MFS or SD-only boards: write the specific page directly to all backends.
	(void)id;

	// When extracting a new config page from the main config, add an
	// if/else-if branch here: prepare the page and call storageWrite()
#endif
#else
	(void)id;
#endif // EFI_PROD_CODE
}

void* getExtraPageAddr(StorageItemId id) {
	(void)id;

	// When extracting a new config page from the main config, add an
	// if/else-if branch here returning the page's TS data pointer
	return nullptr;
}

size_t getExtraPageSize(StorageItemId id) {
	(void)id;

	// When extracting a new config page from the main config, add an
	// if/else-if branch here returning the page's TS data size
	return 0;
}
