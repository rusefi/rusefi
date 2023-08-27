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

// Use bank 2 on H7
// TODO: Support bank 1 as well for the bootloader to work properly!
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
	FLASH->CCR2 = 0xffffffff;
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

int intFlashSectorErase(flashsector_t sector) {
	int ret;
	uint8_t sectorRegIdx = sector;

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

#endif /* EFI_INTERNAL_FLASH */
