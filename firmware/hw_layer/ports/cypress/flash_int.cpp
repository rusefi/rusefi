/**
 *
 * @file    flash.c
 * @brief	Lower-level code for Cypress related to internal flash memory
 * @author  andreika <prometheus.pcb@gmail.com>
 */

#include "pch.h"

#if EFI_INTERNAL_FLASH

#include "flash_int.h"
#include <string.h>


// todo: add DualFlash support

//#define CYPRESS_FLASH_DEBUG

typedef uint32_t flashdata_t;

static volatile uint32_t mainFlashMap[] = {
	0x00000000, 0x00002000, 0x00004000, 0x00006000, 0x00008000, 
	0x00010000, 0x00020000, 0x00030000, 0x00040000, 0x00050000, 
	0x00060000, 0x00070000, 0x00080000, 0x00090000, 0x000A0000, 
	0x000B0000, 0x000C0000, 0x000D0000, 0x000E0000, 0x000F0000, 
	0x00100000, 0x00102000, 0x00104000, 0x00106000, 0x00108000, 
	0x00110000, 0x00120000, 0x00130000, 0x00140000, 0x00150000, 
	0x00160000, 0x00170000, 0x00180000, 
	// todo: add upper 40k flash area
};

bool flashUnlock(void) {
	return true;
}

bool flashLock(void) {
	return true;
}

#define CYPRESS_FLASH_WORD_ALIGNMENT 2

static int alignToWord(int v) {
	return (v + CYPRESS_FLASH_WORD_ALIGNMENT - 1) & ~(CYPRESS_FLASH_WORD_ALIGNMENT - 1);
}

static  __attribute__((optimize("O0"))) int flashSectorEraseAtAddress(volatile uint32_t sectorStart) {
	return MFlash_SectorErase((uint16_t*)sectorStart) != Ok ? FLASH_RETURN_BAD_FLASH : FLASH_RETURN_SUCCESS;
}

int  __attribute__((optimize("O0"))) intFlashErase(flashaddr_t address, size_t size) {
	// todo: this is a temporary hack
	// todo: why the code below doesn't work with -O2?!
	if (flashSectorEraseAtAddress(address) != FLASH_RETURN_SUCCESS) {
		return FLASH_RETURN_BAD_FLASH;
	}
#if 0
	volatile int i;
	size = alignToWord(size);
	
	volatile int numSectors = (sizeof(mainFlashMap) / sizeof(mainFlashMap[0])) - 1;
	// list through all sectors and erase those inside the given memory area
	for (i = 0; i < numSectors; i++) {
		volatile uint32_t sectorStart = mainFlashMap[i];
		volatile uint32_t sectorEnd = mainFlashMap[i + 1] - 1;
		// if the sector overlaps the address range
		if (sectorStart < (address + size) && sectorEnd >= address) {
			if (flashSectorEraseAtAddress(sectorStart) != FLASH_RETURN_SUCCESS) {
				return FLASH_RETURN_BAD_FLASH;
			}
			// check if erased
			size_t sectorSize = sectorEnd - sectorStart + 1;
			if (flashIsErased(sectorStart, sectorSize) == FALSE)
				return FLASH_RETURN_BAD_FLASH; /* Sector is not empty despite the erase cycle! */

		}
	}
#endif
	/* Successfully deleted sector */
	return FLASH_RETURN_SUCCESS;
}

int intFlashWrite(flashaddr_t address, const char* buffer, size_t size) {
	uint32_t sizeInWords = alignToWord(size) >> 1;
	return MFlash_WriteData16Bit((uint16_t*)address, (uint16_t*)buffer, sizeInWords) == Ok ? FLASH_RETURN_SUCCESS : FLASH_RETURN_BAD_FLASH;
	//return MFlash_WriteData16Bit_Fm0Type3CrSecureArea((uint16_t*)address, (uint16_t*)buffer, sizeInWords) == Ok ? 0 : -1;
}

bool intFlashIsErased(flashaddr_t address, size_t size) {
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
	memcpy(destination, (char*) source, size);
	return FLASH_RETURN_SUCCESS;
}

#endif /* EFI_INTERNAL_FLASH */
