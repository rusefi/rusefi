/**
 *
 * http://www.chibios.com/forum/viewtopic.php?f=8&t=820
 * https://github.com/tegesoft/flash-stm32f407
 *
 * @file    flash_int.cpp
 * @brief	Lower-level code related to internal flash memory
 */

#include "pch.h"

#ifndef EFI_STORAGE_INT_FLASH_DRIVER
#define EFI_STORAGE_INT_FLASH_DRIVER TRUE
#endif

#if defined(EFI_BOOTLOADER) || EFI_STORAGE_INT_FLASH_DRIVER

#include "flash_int.h"
#include <string.h>

// Keep sector numbers relative to the beginning of flash. The H743 application
// and OpenBLT updates use bank 1, while settings normally use bank 2.
struct FlashBankRegisters {
	volatile uint32_t& cr;
	volatile uint32_t& sr;
	volatile uint32_t& keyr;
#ifdef STM32H7XX
	volatile uint32_t& ccr;
#endif
};

static FlashBankRegisters intFlashBank([[maybe_unused]] flashaddr_t address) {
#ifdef STM32H7XX
#ifdef STM32H743xx
	if (address >= FLASH_BANK2_BASE) {
		return {FLASH->CR2, FLASH->SR2, FLASH->KEYR2, FLASH->CCR2};
	}
#endif
	return {FLASH->CR1, FLASH->SR1, FLASH->KEYR1, FLASH->CCR1};
#else
	return {FLASH->CR, FLASH->SR, FLASH->KEYR};
#endif
}

static void intFlashWaitWhileBusy(const FlashBankRegisters& bank) {
	// Complete outstanding stores before polling the controller.
	do {
		__DSB();
#ifdef STM32H7XX
	} while (bank.sr & FLASH_SR_QW);
#else
	} while (bank.sr & FLASH_SR_BSY);
#endif
}

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

static void intFlashClearErrors(const FlashBankRegisters& bank) {
#ifdef STM32H7XX
	bank.ccr = 0xffffffff;
#else
	bank.sr = 0x0000ffff;
#endif
	__DSB();
}

static int intFlashCheckErrors(const FlashBankRegisters& bank) {
	uint32_t sr = bank.sr;

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
#ifdef FLASH_SR_RDSERR
	if (sr & FLASH_SR_RDSERR)
		return FLASH_RETURN_SECURITYERROR;
#endif
#ifdef FLASH_SR_RDPERR
	if (sr & FLASH_SR_RDPERR)
		return FLASH_RETURN_SECURITYERROR;
#endif
#ifdef FLASH_SR_WRPERR
	if (sr & FLASH_SR_WRPERR)
		return FLASH_RETURN_SECURITYERROR;
#endif
#ifdef FLASH_SR_CRCRDERR
	if (sr & FLASH_SR_CRCRDERR)
		return FLASH_RETURN_CRCERROR;
#endif

#ifdef STM32H7XX
	if (sr & FLASH_SR_STRBERR) {
		return FLASH_RETURN_ALIGNERROR;
	}
	if (sr & FLASH_SR_INCERR) {
		return FLASH_RETURN_PSEQERROR;
	}
	if (sr & FLASH_SR_DBECCERR) {
		return FLASH_RETURN_BAD_FLASH;
	}
#endif
	return FLASH_RETURN_SUCCESS;
}

/**
 * @brief Unlock the flash memory for write access.
 * @return HAL_SUCCESS  Unlock was successful.
 * @return HAL_FAILED    Unlock failed.
 */
static bool intFlashUnlock(const FlashBankRegisters& bank) {
	/* Check if unlock is really needed */
	if (!(bank.cr & FLASH_CR_LOCK))
		return HAL_SUCCESS;

	/* Write magic unlock sequence */
	bank.keyr = 0x45670123;
	bank.keyr = 0xCDEF89AB;
	__DSB();

	/* Check if unlock was successful */
	if (bank.cr & FLASH_CR_LOCK)
		return HAL_FAILED;
	return HAL_SUCCESS;
}

/**
 * @brief Lock the flash memory for write access.
 */
static void intFlashLock(const FlashBankRegisters& bank) {
	bank.cr |= FLASH_CR_LOCK;
}

#ifdef STM32F7XX
static bool isDualBank(void) {
#ifdef FLASH_OPTCR_nDBANK
	// cleared bit indicates dual bank
	return (FLASH->OPTCR & FLASH_OPTCR_nDBANK) == 0;
#else
	return 0;
#endif
}
#endif

/**
 * @brief Erase the flash @p sector.
 * @details The sector is checked for errors after erase.
 * @note The sector is deleted regardless of its current state.
 *
 * @param sector Sector which is going to be erased.
 * @return FLASH_RETURN_SUCCESS         No error erasing the sector.
 * @return FLASH_RETURN_BAD_FLASH       Flash cell error.
 * @return FLASH_RETURN_NO_PERMISSION   Access denied.
 */
static int intFlashSectorErase(flashsector_t sector) {
	const auto bank = intFlashBank(intFlashSectorBegin(sector));
	int ret;
	uint8_t sectorRegIdx = sector;
#ifdef STM32H743xx
	// H743 has eight 128 KiB sectors per bank; SNB is bank relative.
	if (intFlashSectorBegin(sector) >= FLASH_BANK2_BASE) {
		sectorRegIdx -= (FLASH_BANK2_BASE - FLASH_BASE) / flashSectorSize(sector);
	}
#endif
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
	if (intFlashUnlock(bank) == HAL_FAILED)
		return FLASH_RETURN_NO_PERMISSION;

	/* Wait for any busy flags. */
	intFlashWaitWhileBusy(bank);

	/* Clearing error status bits.*/
	intFlashClearErrors(bank);

	/* Setup parallelism before any program/erase */
	bank.cr &= ~FLASH_CR_PSIZE_MASK;
	bank.cr |= FLASH_CR_PSIZE_VALUE;

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
	bank.cr &= ~FLASH_CR_SNB_Msk;
	bank.cr |= (sectorRegIdx << FLASH_CR_SNB_Pos) & FLASH_CR_SNB_Msk;
	/* sector erase */
	bank.cr |= FLASH_CR_SER;
	/* start erase operation */
#ifdef STM32H7XX
	bank.cr |= FLASH_CR_START;
#else
	bank.cr |= FLASH_CR_STRT;
#endif

	/* Wait until it's finished. */
	intFlashWaitWhileBusy(bank);

	/* Sector erase flag does not clear automatically. */
	bank.cr &= ~FLASH_CR_SER;

	/* Lock flash again */
	intFlashLock(bank);

	ret = intFlashCheckErrors(bank);
	if (ret != FLASH_RETURN_SUCCESS)
		return ret;

	/* Check deleted sector for errors */
	if (intFlashIsErased(intFlashSectorBegin(sector), flashSectorSize(sector)) == FALSE)
		return FLASH_RETURN_BAD_FLASH; /* Sector is not empty despite the erase cycle! */

	/* Successfully deleted sector */
	return FLASH_RETURN_SUCCESS;
}

/* Programmable voltage detector helpers */
static void intFlashSetPVD() {
#if defined(STM32F4XX)
	/* Set threshold */
	PWR->CR = (PWR->CR & ~PWR_CR_PLS_Msk) | PWR_CR_PLS_VALUE;
	/* Enable PVD */
	PWR->CR |= PWR_CR_PVDE;
#elif defined(STM32F7XX)
	/* Set threshold */
	PWR->CR1 = (PWR->CR1 & ~PWR_CR1_PLS_Msk) | PWR_CR1_PLS_VALUE;
	/* Enable PVD */
	PWR->CR1 |= PWR_CR1_PVDE;
#elif defined(STM32H7XX)
	/* Set threshold */
	PWR->CR1 = (PWR->CR1 & ~PWR_CR1_PLS_Msk) | PWR_CR1_PLS_VALUE;
	/* Enable PVD */
	PWR->CR1 |= PWR_CR1_PVDEN;
#endif
}

static bool intFlashGetPVDStatus() {
	/* Return true if Vdd is higher then selected threshold */
#if defined(STM32F4XX)
	return !(PWR->CSR & PWR_CSR_PVDO);
#else
	return !(PWR->CSR1 & PWR_CSR1_PVDO);
#endif
}

int intFlashErase(flashaddr_t address, size_t size) {
	if (size == 0) {
		return FLASH_RETURN_SUCCESS;
	}
#ifdef STM32H7XX
	const size_t flashSize = flashSizeKb() * 1024;
	if (address < FLASH_BASE || address - FLASH_BASE >= flashSize || size > flashSize - (address - FLASH_BASE)) {
		return FLASH_RETURN_BAD_FLASH;
	}
#endif
	flashaddr_t endAddress = address + size - 1;
	while (address <= endAddress) {
		flashsector_t sector = intFlashSectorAt(address);
		int err = intFlashSectorErase(sector);
		if (err != FLASH_RETURN_SUCCESS)
			return err;
		address = intFlashSectorEnd(sector);
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

#ifdef STM32H7XX
int intFlashWrite(flashaddr_t address, const char* buffer, size_t size) {
	constexpr size_t flashWordSize = 32;
	if (size == 0) {
		return FLASH_RETURN_SUCCESS;
	}
	if (address % flashWordSize != 0) {
		return FLASH_RETURN_ALIGNERROR;
	}
	const size_t flashSize = flashSizeKb() * 1024;
	if (address < FLASH_BASE || address - FLASH_BASE >= flashSize || size > flashSize - (address - FLASH_BASE)) {
		return FLASH_RETURN_BAD_FLASH;
	}

	intFlashSetPVD();
	while (size > 0) {
		if (!intFlashGetPVDStatus()) {
			return FLASH_RETURN_LOWVOLTAGEERROR;
		}

		// Select again for each word so a write may cross the bank boundary.
		const auto bank = intFlashBank(address);
		if (intFlashUnlock(bank) == HAL_FAILED) {
			return FLASH_RETURN_NO_PERMISSION;
		}
		intFlashWaitWhileBusy(bank);
		intFlashClearErrors(bank);
		bank.cr = (bank.cr & ~FLASH_CR_PSIZE_MASK) | FLASH_CR_PSIZE_VALUE;

		// Pad the final flash word without reading past the caller's buffer.
		// memcpy also permits an unaligned source buffer.
		uint32_t data[flashWordSize / sizeof(uint32_t)];
		memset(data, 0xff, sizeof(data));
		const size_t chunk = size < flashWordSize ? size : flashWordSize;
		memcpy(data, buffer, chunk);

		bank.cr |= FLASH_CR_PG;
		__ISB();
		__DSB();

		volatile uint32_t* destination = reinterpret_cast<volatile uint32_t*>(address);
		for (size_t i = 0; i < flashWordSize / sizeof(uint32_t); i++) {
			destination[i] = data[i];
		}
		__ISB();
		__DSB();
		intFlashWaitWhileBusy(bank);
		bank.cr &= ~FLASH_CR_PG;
		__DSB();
		__ISB();

		const int result = intFlashCheckErrors(bank);
		intFlashLock(bank);
		if (result != FLASH_RETURN_SUCCESS) {
			return result;
		}

		address += flashWordSize;
		buffer += chunk;
		size -= chunk;
	}

	return FLASH_RETURN_SUCCESS;
}

#else // not STM32H7XX
static int intFlashWriteData(flashaddr_t address, const flashdata_t data) {
	const auto bank = intFlashBank(address);
	/* Clearing error status bits.*/
	intFlashClearErrors(bank);

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
	intFlashWaitWhileBusy(bank);

	/* Exit flash programming mode */
	FLASH->CR &= ~FLASH_CR_PG;

	return intFlashCheckErrors(bank);
}

int intFlashWrite(flashaddr_t address, const char* buffer, size_t size) {
	const auto bank = intFlashBank(address);
	intFlashSetPVD();
	if (!intFlashGetPVDStatus()) {
		return FLASH_RETURN_LOWVOLTAGEERROR;
	}

	int ret = FLASH_RETURN_SUCCESS;

	/* Unlock flash for write access */
	if (intFlashUnlock(bank) == HAL_FAILED)
		return FLASH_RETURN_NO_PERMISSION;

	/* Wait for any busy flags */
	intFlashWaitWhileBusy(bank);

	/* Setup parallelism before any program/erase */
	FLASH->CR &= ~FLASH_CR_PSIZE_MASK;
	FLASH->CR |= FLASH_CR_PSIZE_VALUE;

	while (size) {
		if (!intFlashGetPVDStatus()) {
			intFlashLock(bank);
			return FLASH_RETURN_LOWVOLTAGEERROR;
		}

		/* Check if the flash address is correctly aligned */
		size_t alignOffset = address % sizeof(flashdata_t);
		//print("flash alignOffset=%d\r\n", alignOffset);
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
		} else if (size >= sizeof(flashdata_t)) {
			/* Now, address is correctly aligned. One can copy data directly from
			 * buffer's data to flash memory until the size of the data remaining to be
			 * copied requires special treatment. */
			//print("flash write size=%d\r\n", size);
			ret = intFlashWriteData(address, *(const flashdata_t*) buffer);
			if (ret != FLASH_RETURN_SUCCESS)
				goto exit;
			address += sizeof(flashdata_t);
			buffer += sizeof(flashdata_t);
			size -= sizeof(flashdata_t);
		} else /* if (size > 0) */ {
			/* Now, address is correctly aligned, but the remaining data are to
			 * small to fill a entier flashdata_t. Thus, one must read data already
			 * in flash and update them with buffer's data before writing an entire
			 * flashdata_t to flash memory. */
			flashdata_t tmp = *(volatile flashdata_t*) address;
			memcpy(&tmp, buffer, size);
			ret = intFlashWriteData(address, tmp);
			if (ret != FLASH_RETURN_SUCCESS)
				goto exit;
			size = 0;
		}
	}

exit:
	/* Lock flash again */
	intFlashLock(bank);

	return ret;
}
#endif

#endif /* EFI_STORAGE_INT_FLASH */
