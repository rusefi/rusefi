/**
 * @file flash_int.cpp
 * @brief AT32F435 bank-1 programming through the ChibiOS EFL driver.
 *
 * OpenBLT's adapter assembles whole words before calling intFlashWrite.
 * Bank 2 is reserved for settings and is never exposed by this interface.
 */
#include "pch.h"
#include "flash_int.h"

#if HAL_USE_EFL
#include "hal_efl.h"
#include "hal_flash.h"

static const flash_descriptor_t* bankDescriptor() {
	// halInit initializes EFLD1 and selects the descriptor for the actual chip.
	return flashGetDescriptor(&EFLD1);
}

static bool isInBank(flashaddr_t address, size_t size) {
	const auto* bank = bankDescriptor();
	if (bank == nullptr) {
		return false;
	}
	const auto base = reinterpret_cast<uintptr_t>(bank->address);
	return address >= base && address - base <= bank->size
		&& size <= bank->size - (address - base);
}

static bool ensureStarted() {
	return EFLD1.state != FLASH_STOP || eflStart(&EFLD1, nullptr) == MSG_OK;
}

int intFlashErase(flashaddr_t address, size_t size) {
	if (!isInBank(address, size)) {
		return FLASH_RETURN_NO_PERMISSION;
	}
	if (size == 0) {
		return FLASH_RETURN_SUCCESS;
	}
	if (!ensureStarted()) {
		return FLASH_RETURN_OPERROR;
	}
	const auto* bank = bankDescriptor();
	const auto offset = address - reinterpret_cast<uintptr_t>(bank->address);
	const auto first = offset / bank->sectors_size;
	const auto last = (offset + size - 1) / bank->sectors_size;
	for (auto sector = first; sector <= last; ++sector) {
		auto err = flashStartEraseSector(&EFLD1, sector);
		if (err != FLASH_NO_ERROR) {
			return FLASH_RETURN_OPERROR;
		}
		uint32_t waitMs = AT32_FLASH_WAIT_TIME_MS;
		do {
			chThdSleepMilliseconds(waitMs);
			err = flashQueryErase(&EFLD1, &waitMs);
		} while (err == FLASH_BUSY_ERASING);
		if (err != FLASH_NO_ERROR) {
			return FLASH_RETURN_OPERROR;
		}
#if HAL_USE_WDG
		// Multi-sector erase can outlast the watchdog's interval.
		wdgResetI(&WDGD1);
#endif
	}
	return FLASH_RETURN_SUCCESS;
}

int intFlashWrite(flashaddr_t address, const char* buffer, size_t size) {
	if (!isInBank(address, size)) {
		return FLASH_RETURN_NO_PERMISSION;
	}
	if (size == 0) {
		return FLASH_RETURN_SUCCESS;
	}
	if ((address & 3U) != 0 || (size & 3U) != 0) {
		return FLASH_RETURN_ALIGNERROR;
	}
	if (!ensureStarted()) {
		return FLASH_RETURN_OPERROR;
	}
	const auto offset = address - reinterpret_cast<uintptr_t>(bankDescriptor()->address);
	return flashProgram(&EFLD1, offset, size, reinterpret_cast<const uint8_t*>(buffer)) == FLASH_NO_ERROR
		? FLASH_RETURN_SUCCESS : FLASH_RETURN_OPERROR;
}

bool intFlashIsErased(flashaddr_t address, size_t size) {
	if (!isInBank(address, size)) {
		return false;
	}
	for (size_t i = 0; i < size; ++i) {
		if (*reinterpret_cast<const volatile uint8_t*>(address + i) != 0xff) {
			return false;
		}
	}
	return true;
}

bool intFlashCompare(flashaddr_t address, const char* buffer, size_t size) {
	return isInBank(address, size)
		&& memcmp(reinterpret_cast<const void*>(address), buffer, size) == 0;
}

int intFlashRead(flashaddr_t address, char* buffer, size_t size) {
	if (!isInBank(address, size)) {
		return FLASH_RETURN_NO_PERMISSION;
	}
	memcpy(buffer, reinterpret_cast<const void*>(address), size);
	return FLASH_RETURN_SUCCESS;
}

size_t flashSectorSize(flashsector_t sector) {
	const auto* bank = bankDescriptor();
	return bank != nullptr && sector < bank->sectors_count ? bank->sectors_size : 0;
}

size_t flashSizeKb() {
	// The programming/checksum interface exposes bank 1, not physical total flash.
	const auto* bank = bankDescriptor();
	return bank != nullptr ? bank->size / 1024 : 0;
}
#endif
