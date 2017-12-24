/**
 * @file    flash_main.c
 * @brief	Lower-level code related to internal flash memory
 */

#include "main.h"

#if EFI_INTERNAL_FLASH || defined(__DOXYGEN__)

#include "flash.h"
#include <string.h>

size_t flashSectorSize(flashsector_t sector) {
	// sectors 0..11 are the 1st memory bank (1Mb), and 12..23 are the 2nd (the same structure).
	if (sector <= 3 || (sector >= 12 && sector <= 15))
		return 16 * 1024;
	else if (sector == 4 || sector == 16)
		return 64 * 1024;
	else if ((sector >= 5 && sector <= 11) || (sector >= 17 && sector <= 23))
		return 128 * 1024;
	return 0;
}

flashaddr_t flashSectorBegin(flashsector_t sector) {
	flashaddr_t address = FLASH_BASE;
	while (sector > 0) {
		--sector;
		address += flashSectorSize(sector);
	}
	return address;
}

flashaddr_t flashSectorEnd(flashsector_t sector) {
	return flashSectorBegin(sector + 1);
}

flashsector_t flashSectorAt(flashaddr_t address) {
	flashsector_t sector = 0;
	while (address >= flashSectorEnd(sector))
		++sector;
	return sector;
}

/**
 * @brief Wait for the flash operation to finish.
 */
#define flashWaitWhileBusy() { while (FLASH->SR & FLASH_SR_BSY) {} }

/**
 * @brief Unlock the flash memory for write access.
 * @return HAL_SUCCESS  Unlock was successful.
 * @return HAL_FAILED    Unlock failed.
 */
static bool flashUnlock(void) {
	/* Check if unlock is really needed */
	if (!(FLASH->CR & FLASH_CR_LOCK))
		return HAL_SUCCESS;

	/* Write magic unlock sequence */
	FLASH->KEYR = 0x45670123;
	FLASH->KEYR = 0xCDEF89AB;

	/* Check if unlock was successful */
	if (FLASH->CR & FLASH_CR_LOCK)
		return HAL_FAILED;
	return HAL_SUCCESS;
}

/**
 * @brief Lock the flash memory for write access.
 */
#define flashLock() { FLASH->CR |= FLASH_CR_LOCK; }

int flashSectorErase(flashsector_t sector) {
	/* Unlock flash for write access */
	if (flashUnlock() == HAL_FAILED)
		return FLASH_RETURN_NO_PERMISSION;

	/* Wait for any busy flags. */
	flashWaitWhileBusy();

	/* Setup parallelism before any program/erase */
	FLASH->CR &= ~FLASH_CR_PSIZE_MASK;
	FLASH->CR |= FLASH_CR_PSIZE_VALUE;

	/* Start deletion of sector.
	 * SNB(4:1) is defined as:
	 * 00000 sector 0
	 * 00001 sector 1
	 * ...
	 * 01011 sector 11 (the end of 1st bank, 1Mb border)
	 * 01100 sector 12 (start of 2nd bank)
	 * ...
	 * 10111 sector 23 (the end of 2nd bank, 2Mb border)
	 * others not allowed */
	FLASH->CR &= ~(FLASH_CR_SNB_0 | FLASH_CR_SNB_1 | FLASH_CR_SNB_2 | FLASH_CR_SNB_3 | FLASH_CR_SNB_4);
	if (sector & 0x1)
		FLASH->CR |= FLASH_CR_SNB_0;
	if (sector & 0x2)
		FLASH->CR |= FLASH_CR_SNB_1;
	if (sector & 0x4)
		FLASH->CR |= FLASH_CR_SNB_2;
	if (sector & 0x8)
		FLASH->CR |= FLASH_CR_SNB_3;
	if (sector & 0x10)
		FLASH->CR |= FLASH_CR_SNB_4;
	FLASH->CR |= FLASH_CR_SER;
	FLASH->CR |= FLASH_CR_STRT;

	/* Wait until it's finished. */
	flashWaitWhileBusy();

	/* Sector erase flag does not clear automatically. */
	FLASH->CR &= ~FLASH_CR_SER;

	/* Lock flash again */
	flashLock()
	;

	/* Check deleted sector for errors */
	if (flashIsErased(flashSectorBegin(sector), flashSectorSize(sector)) == FALSE)
		return FLASH_RETURN_BAD_FLASH; /* Sector is not empty despite the erase cycle! */

	/* Successfully deleted sector */
	return FLASH_RETURN_SUCCESS;
}

int flashErase(flashaddr_t address, size_t size) {
	while (size > 0) {
		flashsector_t sector = flashSectorAt(address);
		int err = flashSectorErase(sector);
		if (err != FLASH_RETURN_SUCCESS)
			return err;
		address = flashSectorEnd(sector);
		size_t sector_size = flashSectorSize(sector);
		if (sector_size >= size)
			break;
		size -= sector_size;
	}

	return FLASH_RETURN_SUCCESS;
}

bool flashIsErased(flashaddr_t address, size_t size) {
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

bool flashCompare(flashaddr_t address, const char* buffer, size_t size) {
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

int flashRead(flashaddr_t address, char* buffer, size_t size) {
	memcpy(buffer, (char*) address, size);
	return FLASH_RETURN_SUCCESS;
}

static void flashWriteData(flashaddr_t address, const flashdata_t data) {
	/* Enter flash programming mode */
	FLASH->CR |= FLASH_CR_PG;

	/* Write the data */
	*(flashdata_t*) address = data;

	/* Wait for completion */
	flashWaitWhileBusy();

	/* Exit flash programming mode */
	FLASH->CR &= ~FLASH_CR_PG;
}

int flashWrite(flashaddr_t address, const char* buffer, size_t size) {
	/* Unlock flash for write access */
	if (flashUnlock() == HAL_FAILED)
		return FLASH_RETURN_NO_PERMISSION;

	/* Wait for any busy flags */
	flashWaitWhileBusy();

	/* Setup parallelism before any program/erase */
	FLASH->CR &= ~FLASH_CR_PSIZE_MASK;
	FLASH->CR |= FLASH_CR_PSIZE_VALUE;

	/* Check if the flash address is correctly aligned */
	size_t alignOffset = address % sizeof(flashdata_t);
//	print("flash alignOffset=%d\r\n", alignOffset);
	if (alignOffset != 0) {
		/* Not aligned, thus we have to read the data in flash already present
		 * and update them with buffer's data */

		/* Align the flash address correctly */
		flashaddr_t alignedFlashAddress = address - alignOffset;

		/* Read already present data */
		flashdata_t tmp = *(volatile flashdata_t*) alignedFlashAddress;

		/* Compute how much bytes one must update in the data read */
		size_t chunkSize = sizeof(flashdata_t) - alignOffset;
		if (chunkSize > size)
			chunkSize = size; // this happens when both address and address + size are not aligned

		/* Update the read data with buffer's data */
		memcpy((char*) &tmp + alignOffset, buffer, chunkSize);

		/* Write the new data in flash */
		flashWriteData(alignedFlashAddress, tmp);

		/* Advance */
		address += chunkSize;
		buffer += chunkSize;
		size -= chunkSize;
	}

	/* Now, address is correctly aligned. One can copy data directly from
	 * buffer's data to flash memory until the size of the data remaining to be
	 * copied requires special treatment. */
	while (size >= sizeof(flashdata_t)) {
//		print("flash write size=%d\r\n", size);
		flashWriteData(address, *(const flashdata_t*) buffer);
		address += sizeof(flashdata_t);
		buffer += sizeof(flashdata_t);
		size -= sizeof(flashdata_t);
	}

	/* Now, address is correctly aligned, but the remaining data are to
	 * small to fill a entier flashdata_t. Thus, one must read data already
	 * in flash and update them with buffer's data before writing an entire
	 * flashdata_t to flash memory. */
	if (size > 0) {
		flashdata_t tmp = *(volatile flashdata_t*) address;
		memcpy(&tmp, buffer, size);
		flashWriteData(address, tmp);
	}

	/* Lock flash again */
	flashLock()
	;

	return FLASH_RETURN_SUCCESS;
}

#endif /* EFI_INTERNAL_FLASH */
