/**
 * @file	mpu_util.cpp
 *
 * @date Feb 26, 2021
 * @author Matthew Kennedy, (c) 2021
 */

#include "pch.h"

#include "flash_int.h"

bool mcuCanFlashWhileRunning() {
	// We only support dual bank H7, so always allow flash while running.
	return true;
}

size_t flashSectorSize(flashsector_t /* sector */) {
	// All sectors on H7 are 128k
	return 128 * 1024;
}

uintptr_t getFlashAddrFirstCopy() {
#ifdef STM32H723xx
	return 0x080E0000;
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
