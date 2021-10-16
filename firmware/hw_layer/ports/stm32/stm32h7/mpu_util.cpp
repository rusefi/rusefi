/**
 * @file	mpu_util.cpp
 *
 * @date Feb 26, 2021
 * @author Matthew Kennedy, (c) 2021
 */

#include "flash_int.h"

bool allowFlashWhileRunning() {
	// We only support dual bank H7, so always allow flash while running.
	return true;
}

size_t flashSectorSize(flashsector_t sector) {
	// All sectors on H7 are 128k
	return 128 * 1024;
}

uintptr_t getFlashAddrFirstCopy() {
	return 0x08100000;
}

uintptr_t getFlashAddrSecondCopy() {
	// Second copy is one sector past the first
	return getFlashAddrFirstCopy() + 128 * 1024;
}
