// file extra_flash_pages.h

#pragma once

#include "storage.h"
#include "rusefi/crc.h"

/**
 * Generic CRC-wrapped container for extra flash pages.
 * Each extra page stores a version tag, the page data, and a CRC32 so that
 * stale or corrupted storage can be detected on load.
 *
 * The struct must be kept 32-byte aligned for STM32H7 flash-word writes —
 * each page's .cpp should static_assert this for its own instantiation.
 *
 * Usage:
 *   static ExtraPageContainer<my_page_s, 1> myContainer;
 */
template <typename TData, uint32_t DataVersion>
struct alignas(32) ExtraPageContainer {
	uint32_t version;
	TData data;
	uint32_t crc;

	[[nodiscard]] uint32_t computeCrc() const {
		return crc32(&data, sizeof(TData));
	}

	[[nodiscard]] bool isValid() const {
		return version == DataVersion && crc == computeCrc();
	}

	void prepareForStorage() {
		version = DataVersion;
		crc = computeCrc();
	}
};

/**
 * Load all extra pages from storage into RAM.
 * Called unconditionally at startup (before TunerStudio starts), like the main config
 *
 * To add a new extra page: add one initXxx() call in extra_flash_pages.cpp.
 * rusefi.cpp never needs to change.
 */
void loadExtraPages();

/**
 * Reload a single extra page from storage by record ID.
 */
void loadExtraPage(StorageItemId id);

/**
 * Called from writeToFlashNowImpl() immediately after the main-config sector
 * has been erased and the main config written.  The shared sector region is
 * now blank, so each extra page can be written directly.
 *
 * To add a new extra page: add one storageWrite() call in extra_flash_pages.cpp.
 * flash_main.cpp never needs to change.
 */
void burnExtraFlashPages();

/**
 * Burn a single extra page by storage record ID.
 * This is the single entry point for persisting an individual extra page —
 * it encapsulates the INT_FLASH-vs-direct-write decision so that callers
 * (TunerStudio burn handler, storage thread) don't need to know about
 * storage backend details.
 *
 * On INT_FLASH boards: triggers a full config burn (writeToFlashNow), which
 * erases the shared sector and piggybacks all extra pages via burnExtraFlashPages().
 * On MFS/SD boards: prepares and writes the page directly to all backends.
 */
void burnExtraFlashPage(StorageItemId id);

/**
 * TunerStudio page interface — returns the raw page data pointer and size
 * for a given extra page, so that tunerstudio.cpp dispatches through the
 * framework instead of calling individual page modules directly.
 */
void* getExtraPageAddr(StorageItemId id);
size_t getExtraPageSize(StorageItemId id);

/**
 * Flash storage support — used by storage_flash.cpp for address mapping.
 * Returns the byte offset within the primary settings sector for the given
 * extra page, or 0 if the ID is not a flash-backed extra page.
 *
 * When adding a new extra page, add its offset here so that
 * storage_flash.cpp picks it up automatically.
 */
size_t getExtraPageFlashOffset(StorageItemId id);
