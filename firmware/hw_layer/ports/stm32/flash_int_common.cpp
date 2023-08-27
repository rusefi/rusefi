// Common logic for flash memory between F4/F7 and H7 implementations

#include "pch.h"

#if EFI_INTERNAL_FLASH

#include "flash_int.h"

flashaddr_t intFlashSectorEnd(flashsector_t sector) {
	return intFlashSectorBegin(sector + 1);
}

flashsector_t intFlashSectorAt(flashaddr_t address) {
	flashsector_t sector = 0;
	while (address >= intFlashSectorEnd(sector))
		++sector;
	return sector;
}

int intFlashErase(flashaddr_t address, size_t size) {
	while (size > 0) {
		flashsector_t sector = intFlashSectorAt(address);
		int err = intFlashSectorErase(sector);
		if (err != FLASH_RETURN_SUCCESS)
			return err;
		address = intFlashSectorEnd(sector);
		size_t sector_size = flashSectorSize(sector);
		if (sector_size >= size)
			break;
		size -= sector_size;
	}

	return FLASH_RETURN_SUCCESS;
}

bool intFlashIsErased(flashaddr_t address, size_t size) {
#if CORTEX_MODEL == 7
	// If we have a cache, invalidate the relevant cache lines.
	// They may still contain old data, leading us to believe that the 
	// flash erase failed.
	SCB_InvalidateDCache_by_Addr((uint32_t*)address, size);
#endif

	/* Check for default set bits in the flash memory
	 * For efficiency, compare flashdata_t values as much as possible,
	 * then, fallback to byte per byte comparison. */
	while (size >= sizeof(flashdata_t)) {
		if (*(volatile flashdata_t*) address != (flashdata_t) (-1)) // flashdata_t being unsigned, -1 is 0xFF..FF
			return false;
		address += sizeof(flashdata_t);
		size -= sizeof(flashdata_t);
	}
	while (size > 0) {
		if (*(char*) address != 0xFF)
			return false;
		++address;
		--size;
	}

	return TRUE;
}

bool intFlashCompare(flashaddr_t address, const char* buffer, size_t size) {
	/* For efficiency, compare flashdata_t values as much as possible,
	 * then, fallback to byte per byte comparison. */
	while (size >= sizeof(flashdata_t)) {
		if (*(volatile flashdata_t*) address != *(flashdata_t*) buffer)
			return FALSE;
		address += sizeof(flashdata_t);
		buffer += sizeof(flashdata_t);
		size -= sizeof(flashdata_t);
	}
	while (size > 0) {
		if (*(volatile char*) address != *buffer)
			return FALSE;
		++address;
		++buffer;
		--size;
	}

	return TRUE;
}

int intFlashRead(flashaddr_t source, char* destination, size_t size) {
#if CORTEX_MODEL == 7
	// If we have a cache, invalidate the relevant cache lines.
	// They may still contain old data, leading us to read invalid data.
	SCB_InvalidateDCache_by_Addr((uint32_t*)source, size);
#endif

	memcpy(destination, (char*) source, size);
	return FLASH_RETURN_SUCCESS;
}

#endif // EFI_INTERNAL_FLASH
