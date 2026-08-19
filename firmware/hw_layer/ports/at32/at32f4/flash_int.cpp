/**
 * @file	flash_int.cpp
 * @brief	AT32F435 internal flash low-level API on top of the ChibiOS EFL driver
 *
 * The OpenBLT bootloader writes the application image through the same
 * flash_int.h interface the STM32 ports implement with raw FLASH registers.
 * On AT32F4 the STM32 flash registers do not exist; use the EFL driver
 * (bank 1) instead - the very same driver the application's MFS settings
 * storage runs on, so the program/erase paths are proven on this chip.
 *
 * @date Aug 19, 2026
 * @author rusEFI
 */

#include "pch.h"
#include "flash_int.h"

#if HAL_USE_EFL
#include "hal_efl.h"
#include "hal_flash.h"

static bool isEflStarted = false;

static void ensureStarted() {
	if (!isEflStarted) {
		eflObjectInit(&EFLD1);
		EFlashConfig eflConfig = {};
		eflStart(&EFLD1, &eflConfig);
		isEflStarted = true;
	}
}

static uint32_t getBankBase() {
	const flash_descriptor_t* desc = flashGetDescriptor(&EFLD1);
	return (uint32_t)desc->address;
}

static uint32_t getBankSize() {
	const flash_descriptor_t* desc = flashGetDescriptor(&EFLD1);
	return desc->size;
}

static bool isInBank(flashaddr_t address, size_t size) {
	uint32_t base = getBankBase();
	return address >= base && (address + size) <= (base + getBankSize());
}

int intFlashErase(flashaddr_t address, size_t size) {
	ensureStarted();

	if (!isInBank(address, size)) {
		return FLASH_RETURN_NO_PERMISSION;
	}

	const flash_descriptor_t* desc = flashGetDescriptor(&EFLD1);
	uint32_t base = getBankBase();

	uint32_t firstSector = (address - base) / desc->sectors_size;
	uint32_t lastSector = (address + size - 1 - base) / desc->sectors_size;

	for (uint32_t sector = firstSector; sector <= lastSector; sector++) {
		flash_error_t err = flashStartEraseSector(&EFLD1, sector);
		if (err != FLASH_NO_ERROR) {
			return FLASH_RETURN_OPERROR;
		}

		uint32_t msec = AT32_FLASH_WAIT_TIME_MS;
		do {
			chThdSleepMilliseconds(msec);
			err = flashQueryErase(&EFLD1, &msec);
		} while (err == FLASH_BUSY_ERASING);

		if (err != FLASH_NO_ERROR) {
			return FLASH_RETURN_OPERROR;
		}
	}

	return FLASH_RETURN_SUCCESS;
}

int intFlashWrite(flashaddr_t address, const char* buffer, size_t size) {
	ensureStarted();

	if (!isInBank(address, size)) {
		return FLASH_RETURN_NO_PERMISSION;
	}

	flash_error_t err = flashProgram(&EFLD1, address - getBankBase(), size, (const uint8_t*)buffer);
	if (err != FLASH_NO_ERROR) {
		return FLASH_RETURN_OPERROR;
	}

	return FLASH_RETURN_SUCCESS;
}

bool intFlashIsErased(flashaddr_t address, size_t size) {
	if (!isInBank(address, size)) {
		return false;
	}

	for (size_t i = 0; i < size; i++) {
		if (*reinterpret_cast<const volatile uint8_t*>(address + i) != 0xFF) {
			return false;
		}
	}

	return true;
}

bool intFlashCompare(flashaddr_t address, const char* buffer, size_t size) {
	if (!isInBank(address, size)) {
		return false;
	}

	return memcmp(reinterpret_cast<const void*>(address), buffer, size) == 0;
}

int intFlashRead(flashaddr_t source, char* destination, size_t size) {
	if (!isInBank(source, size)) {
		return FLASH_RETURN_NO_PERMISSION;
	}

	memcpy(destination, reinterpret_cast<const void*>(source), size);
	return FLASH_RETURN_SUCCESS;
}

size_t flashSectorSize(flashsector_t sector) {
	(void)sector;
	const flash_descriptor_t* desc = flashGetDescriptor(&EFLD1);
	return desc->sectors_size;
}

size_t flashSizeKb() {
	// AT32F435ZM: 4032 KB total (bank1 2048 KB + bank2 1984 KB).
	return 4032;
}

#else // HAL_USE_EFL

#error "AT32 flash_int requires HAL_USE_EFL"

#endif // HAL_USE_EFL
