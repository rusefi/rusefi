// file extra_flash_pages.h

#pragma once

/**
 * Load all extra pages from storage into RAM.
 * Called unconditionally at startup (before TunerStudio starts), like the main config
 *
 * To add a new extra page: add one initXxx() call in extra_flash_pages.cpp.
 * rusefi.cpp never needs to change.
 */
void loadExtraPages();

/**
 * Called from writeToFlashNowImpl() immediately after the main-config sector
 * has been erased and the main config written.  The shared sector region is
 * now blank, so each extra page can be written directly.
 *
 * To add a new extra page: add one storageWrite() call in extra_flash_pages.cpp.
 * flash_main.cpp never needs to change.
 */
void burnExtraFlashPages();
