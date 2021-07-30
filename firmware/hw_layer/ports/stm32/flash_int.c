/**
 *
 * http://www.chibios.com/forum/viewtopic.php?f=8&t=820
 * https://github.com/tegesoft/flash-stm32f407
 *
 * @file    flash_int.c
 * @brief	Lower-level code related to internal flash memory
 */

#include "global.h"
#include "os_access.h"

#if EFI_INTERNAL_FLASH

#include "flash_int.h"
#include <string.h>

#ifdef STM32H7XX
	// Use bank 2 on H7
	#define FLASH_CR FLASH->CR2
	#define FLASH_SR FLASH->SR2
	#define FLASH_KEYR FLASH->KEYR2

	// I have no idea why ST changed the register name from STRT -> START
	#define FLASH_CR_STRT FLASH_CR_START

	#undef FLASH_BASE
	// This is the start of the second bank, since H7 sector numbers are bank relative
	#define FLASH_BASE 0x08100000

	// QW bit supercedes the older BSY bit
	#define intFlashWaitWhileBusy() do { __DSB(); } while (FLASH_SR & FLASH_SR_QW);
#else
	#define FLASH_CR FLASH->CR
	#define FLASH_SR FLASH->SR
	#define FLASH_KEYR FLASH->KEYR

	// Wait for the flash operation to finish
	#define intFlashWaitWhileBusy() do { __DSB(); } while (FLASH->SR & FLASH_SR_BSY);
#endif

flashaddr_t intFlashSectorBegin(flashsector_t sector) {
	flashaddr_t address = FLASH_BASE;
	while (sector > 0) {
		--sector;
		address += flashSectorSize(sector);
	}
	return address;
}

flashaddr_t intFlashSectorEnd(flashsector_t sector) {
	return intFlashSectorBegin(sector + 1);
}

flashsector_t intFlashSectorAt(flashaddr_t address) {
	flashsector_t sector = 0;
	while (address >= intFlashSectorEnd(sector))
		++sector;
	return sector;
}

/**
 * @brief Unlock the flash memory for write access.
 * @return HAL_SUCCESS  Unlock was successful.
 * @return HAL_FAILED    Unlock failed.
 */
static bool intFlashUnlock(void) {
	/* Check if unlock is really needed */
	if (!(FLASH_CR & FLASH_CR_LOCK))
		return HAL_SUCCESS;

	/* Write magic unlock sequence */
	FLASH_KEYR = 0x45670123;
	FLASH_KEYR = 0xCDEF89AB;

	/* Check if unlock was successful */
	if (FLASH_CR & FLASH_CR_LOCK)
		return HAL_FAILED;
	return HAL_SUCCESS;
}

/**
 * @brief Lock the flash memory for write access.
 */
#define intFlashLock() { FLASH_CR |= FLASH_CR_LOCK; }

#ifdef STM32F7XX
static bool isDualBank(void) {
	// cleared bit indicates dual bank
	return (FLASH->OPTCR & FLASH_OPTCR_nDBANK) == 0;
}
#endif

int intFlashSectorErase(flashsector_t sector) {
	uint8_t sectorRegIdx = sector;
#ifdef STM32F7XX
	// On dual bank STM32F7, sector index doesn't match register value.
	// High bit indicates bank, low 4 bits indicate sector within bank.
	// Since each bank has 12 sectors, increment second-bank sector idx
	// by 4 so that the first sector of the second bank (12) ends up with
	// index 16 (0b10000)
	if (isDualBank() && sectorRegIdx >= 12) {
		sectorRegIdx -= 12;
		/* bit 4 defines bank.
		 * Sectors starting from 12 are in bank #2 */
		sectorRegIdx |= 0x10;
	}
#endif

	/* Unlock flash for write access */
	if (intFlashUnlock() == HAL_FAILED)
		return FLASH_RETURN_NO_PERMISSION;

	/* Wait for any busy flags. */
	intFlashWaitWhileBusy();

	/* Setup parallelism before any program/erase */
	FLASH_CR &= ~FLASH_CR_PSIZE_MASK;
	FLASH_CR |= FLASH_CR_PSIZE_VALUE;

	/* Start deletion of sector.
	 * SNB(4:1) is defined as:
	 * 00000 sector 0
	 * 00001 sector 1
	 * ...
	 * 01011 sector 11 (the end of 1st bank, 1Mb border)
	 * 10000 sector 12 (start of 2nd bank)
	 * ...
	 * 11011 sector 23 (the end of 2nd bank, 2Mb border)
	 * others not allowed */
	FLASH_CR &= ~FLASH_CR_SNB_Msk;
	FLASH_CR |= (sectorRegIdx << FLASH_CR_SNB_Pos) & FLASH_CR_SNB_Msk;
	/* sector erase */
	FLASH_CR |= FLASH_CR_SER;
	/* start erase operation */
	FLASH_CR |= FLASH_CR_STRT;

	/* Wait until it's finished. */
	intFlashWaitWhileBusy();

	/* Sector erase flag does not clear automatically. */
	FLASH_CR &= ~FLASH_CR_SER;

	/* Lock flash again */
	intFlashLock()
	;

	/* Check deleted sector for errors */
	if (intFlashIsErased(intFlashSectorBegin(sector), flashSectorSize(sector)) == FALSE)
		return FLASH_RETURN_BAD_FLASH; /* Sector is not empty despite the erase cycle! */

	/* Successfully deleted sector */
	return FLASH_RETURN_SUCCESS;
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

int intFlashRead(flashaddr_t address, char* buffer, size_t size) {
#if CORTEX_MODEL == 7
	// If we have a cache, invalidate the relevant cache lines.
	// They may still contain old data, leading us to read invalid data.
	SCB_InvalidateDCache_by_Addr((uint32_t*)address, size);
#endif

	memcpy(buffer, (char*) address, size);
	return FLASH_RETURN_SUCCESS;
}

#ifdef STM32H7XX
int intFlashWrite(flashaddr_t address, const char* buffer, size_t size) {
	/* Unlock flash for write access */
	if (intFlashUnlock() == HAL_FAILED)
		return FLASH_RETURN_NO_PERMISSION;

	/* Wait for any busy flags */
	intFlashWaitWhileBusy();

	/* Setup parallelism before program */
	FLASH_CR &= ~FLASH_CR_PSIZE_MASK;
	FLASH_CR |= FLASH_CR_PSIZE_VALUE;

	// Round up to the next number of full 32 byte words
	size_t flashWordCount = (size - 1) / 32 + 1;

	// Read units of flashdata_t from the buffer, writing to flash
	const flashdata_t* pRead = (const flashdata_t*)buffer;
	flashdata_t* pWrite = (flashdata_t*)address;

	for (size_t word = 0; word < flashWordCount; word++) {
		/* Enter flash programming mode */
		FLASH_CR |= FLASH_CR_PG;

		// Flush pipelines
		__ISB();
		__DSB();

		// Write 32 bytes
		for (size_t i = 0; i < 8; i++) {
			*pWrite++ = *pRead++;
		}
		
		// Flush pipelines
		__ISB();
		__DSB();

		/* Wait for completion */
		intFlashWaitWhileBusy();

		/* Exit flash programming mode */
		FLASH_CR &= ~FLASH_CR_PG;

		// Flush pipelines
		__ISB();
		__DSB();
	}

	/* Lock flash again */
	intFlashLock();

	return FLASH_RETURN_SUCCESS;
}

#else // not STM32H7XX
static void intFlashWriteData(flashaddr_t address, const flashdata_t data) {
	/* Enter flash programming mode */
	FLASH->CR |= FLASH_CR_PG;

	/* Write the data */
	*(flashdata_t*) address = data;

	// Cortex-M7 (STM32F7/H7) can execute out order - need to force a full flush
	// so that we actually wait for the operation to complete!
#if CORTEX_MODEL == 7
	__DSB();
#endif

	/* Wait for completion */
	intFlashWaitWhileBusy();

	/* Exit flash programming mode */
	FLASH->CR &= ~FLASH_CR_PG;
}

int intFlashWrite(flashaddr_t address, const char* buffer, size_t size) {
	/* Unlock flash for write access */
	if (intFlashUnlock() == HAL_FAILED)
		return FLASH_RETURN_NO_PERMISSION;

	/* Wait for any busy flags */
	intFlashWaitWhileBusy();

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
		intFlashWriteData(alignedFlashAddress, tmp);

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
		intFlashWriteData(address, *(const flashdata_t*) buffer);
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
		intFlashWriteData(address, tmp);
	}

	/* Lock flash again */
	intFlashLock()
	;

	return FLASH_RETURN_SUCCESS;
}
#endif

#endif /* EFI_INTERNAL_FLASH */
