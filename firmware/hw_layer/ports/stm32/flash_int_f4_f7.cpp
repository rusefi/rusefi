/**
 *
 * http://www.chibios.com/forum/viewtopic.php?f=8&t=820
 * https://github.com/tegesoft/flash-stm32f407
 *
 * @file    flash_int.c
 * @brief	Lower-level code related to internal flash memory
 */

#include "pch.h"

#if EFI_INTERNAL_FLASH

#include "flash_int.h"
#include <string.h>

#define FLASH_CR FLASH->CR
#define FLASH_SR FLASH->SR
#define FLASH_KEYR FLASH->KEYR

// Wait for the flash operation to finish
#define intFlashWaitWhileBusy() do { __DSB(); } while (FLASH->SR & FLASH_SR_BSY);

flashaddr_t intFlashSectorBegin(flashsector_t sector) {
	flashaddr_t address = FLASH_BASE;
	while (sector > 0) {
		--sector;
		address += flashSectorSize(sector);
	}
	return address;
}

static void intFlashClearErrors(void)
{
	FLASH_SR = 0x0000ffff;
}

static int intFlashCheckErrors(void)
{
	uint32_t sr = FLASH_SR;

#ifdef FLASH_SR_OPERR
	if (sr & FLASH_SR_OPERR)
		return FLASH_RETURN_OPERROR;
#endif
	if (sr & FLASH_SR_WRPERR)
		return FLASH_RETURN_WPERROR;
#ifdef FLASH_SR_PGAERR
	if (sr & FLASH_SR_PGAERR)
		return FLASH_RETURN_ALIGNERROR;
#endif
#ifdef FLASH_SR_PGPERR
	if (sr & FLASH_SR_PGPERR)
		return FLASH_RETURN_PPARALLERROR;
#endif
#ifdef FLASH_SR_ERSERR
	if (sr & FLASH_SR_ERSERR)
		return FLASH_RETURN_ESEQERROR;
#endif
#ifdef FLASH_SR_PGSERR
	if (sr & FLASH_SR_PGSERR)
		return FLASH_RETURN_PSEQERROR;
#endif

	return FLASH_RETURN_SUCCESS;
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
	int ret;
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

	/* Clearing error status bits.*/
	intFlashClearErrors();

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

	ret = intFlashCheckErrors();
	if (ret != FLASH_RETURN_SUCCESS)
		return ret;

	/* Check deleted sector for errors */
	if (intFlashIsErased(intFlashSectorBegin(sector), flashSectorSize(sector)) == FALSE)
		return FLASH_RETURN_BAD_FLASH; /* Sector is not empty despite the erase cycle! */

	/* Successfully deleted sector */
	return FLASH_RETURN_SUCCESS;
}

static int intFlashWriteData(flashaddr_t address, const flashdata_t data) {
	/* Clearing error status bits.*/
	intFlashClearErrors();

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

	return intFlashCheckErrors();
}

int intFlashWrite(flashaddr_t address, const char* buffer, size_t size) {
	int ret = FLASH_RETURN_SUCCESS;

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
		ret = intFlashWriteData(alignedFlashAddress, tmp);
		if (ret != FLASH_RETURN_SUCCESS)
			goto exit;

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
		ret = intFlashWriteData(address, *(const flashdata_t*) buffer);
		if (ret != FLASH_RETURN_SUCCESS)
			goto exit;
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
		ret = intFlashWriteData(address, tmp);
		if (ret != FLASH_RETURN_SUCCESS)
			goto exit;
	}

exit:
	/* Lock flash again */
	intFlashLock()
	;

	return ret;
}

#endif /* EFI_INTERNAL_FLASH */
