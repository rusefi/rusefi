/**
 * @file	mpu_util.cpp
 *
 * @date Jul 27, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "flash_int.h"

bool allowFlashWhileRunning() {
	// Never allow flash while running on F4, dual bank not implemented.
	return false;
}

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

uintptr_t getFlashAddrFirstCopy() {
	return 0x080E0000;
}

uintptr_t getFlashAddrSecondCopy() {
	return 0x080C0000;
}
