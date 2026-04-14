/*
* @file extra_flash_pages.cpp
 *
 * @date: apr 10, 2026
 * @author FDSoftware
 */

#include "pch.h"
#include "extra_flash_pages.h"
#include "storage.h"

void loadExtraPages() {

	// When extracting a new config page from the main config, add an
	// initXxx() call here
}

void burnExtraFlashPages() {
#if EFI_PROD_CODE
	// When extracting a new config page from the main config, add a
	// storageWrite() call here
#endif // EFI_PROD_CODE
}
