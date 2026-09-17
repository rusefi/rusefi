/**
 * @file	mpu_util.cpp
 *
 * @date Feb 26, 2021
 * @author Matthew Kennedy, (c) 2021
 */

#include "pch.h"

#include "flash_int.h"

bool mcuCanFlashWhileRunning() {
#if defined(STM32H723xx) || EFI_FLASH_USE_1500_OF_2MB
	// The larger H743 image shares bank 2 with settings, so erases can stall code.
	return false;
#else
	return true;
#endif
}

size_t flashSectorSize(flashsector_t /* sector */) {
	// All sectors on H7 are 128k
	return 128 * 1024;
}

// [tag:persistent-container-magic-size]
static_assert(sizeof(persistent_config_container_s) <= 128 * 1024, "Settings does not fit into flash sector");

uintptr_t getFlashAddrFirstCopy() {
#ifdef STM32H723xx
	return 0x080E0000;
#elif EFI_FLASH_USE_1500_OF_2MB
	// Right after the first 1.5 MiB, matching 2mb_flash.mk and STM32H743xI.ld.
	return 0x08180000;
#else
	return 0x08100000;
#endif
}

uintptr_t getFlashAddrSecondCopy() {
#ifdef STM32H723xx
	return 0;
#else
	// Second copy is one sector past the first
	return getFlashAddrFirstCopy() + 128 * 1024;
#endif
}
