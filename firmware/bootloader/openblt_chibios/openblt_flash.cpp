#include "pch.h"
#include "flash_int.h"

extern "C" {
	#include "boot.h"
	#include "flash.h"
}

void FlashInit() {
	// Flash already init by ChibiOS
}

blt_addr FlashGetUserProgBaseAddress() {
#ifdef STM32H7XX
	return FLASH_BASE + 128 * 1024;
#else // not STM32H7
	return FLASH_BASE + 32 * 1024;
#endif
}

blt_bool FlashWrite(blt_addr addr, blt_int32u len, blt_int8u *data) {
	// don't allow overwriting the bootloader
	if (addr < FlashGetUserProgBaseAddress()) {
		return BLT_FALSE;
	}

	return (FLASH_RETURN_SUCCESS == intFlashWrite(addr, (const char*)data, len)) ? BLT_TRUE : BLT_FALSE;

	return BLT_TRUE;
}

blt_bool FlashErase(blt_addr addr, blt_int32u len) {
	// don't allow erasing the bootloader
	if (addr < FlashGetUserProgBaseAddress()) {
		return BLT_FALSE;
	}

	if (!intFlashIsErased(addr, len)) {
		return (FLASH_RETURN_SUCCESS == intFlashErase(addr, len)) ? BLT_TRUE : BLT_FALSE;
	}

	return BLT_TRUE;
}

blt_bool FlashDone() {
	return BLT_TRUE;
}

blt_bool FlashWriteChecksum() {
	return BLT_TRUE;
}

blt_bool FlashVerifyChecksum() {
	// Naive check: if the first block is blank, there's no code there
	return intFlashIsErased(FlashGetUserProgBaseAddress(), 4) ? BLT_FALSE : BLT_TRUE;
}

blt_bool isFlashDualBank(void) {
#ifdef STM32F7XX
	// cleared bit indicates dual bank
	return (FLASH->OPTCR & FLASH_OPTCR_nDBANK) == 0 ? BLT_TRUE : BLT_FALSE;
#else
	return BLT_TRUE;
#endif
}
