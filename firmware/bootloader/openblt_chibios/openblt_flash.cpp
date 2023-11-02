#include "pch.h"
#include "flash_int.h"

#include <rusefi/crc.h>

extern "C" {
	#include "boot.h"
	#include "flash.h"
}

void FlashInit() {
	// Flash already init by ChibiOS
}

blt_addr FlashGetUserProgBaseAddress() {
#ifdef STM32H7XX
	return 0x08020000;
#else // not STM32H7
	return 0x08008000;
#endif
}

blt_bool FlashWrite(blt_addr addr, blt_int32u len, blt_int8u *data) {
	return (FLASH_RETURN_SUCCESS == intFlashWrite(addr, (const char*)data, len)) ? BLT_TRUE : BLT_FALSE;
}

blt_bool FlashErase(blt_addr addr, blt_int32u len) {
	if (intFlashIsErased(addr, len)) {
		// Already blank, we can skip the expensive erase operation
		return BLT_TRUE;
	}

	return (FLASH_RETURN_SUCCESS == intFlashErase(addr, len)) ? BLT_TRUE : BLT_FALSE;
}

blt_bool FlashDone() {
	return BLT_TRUE;
}

blt_bool FlashWriteChecksum() {
	return BLT_TRUE;
}

blt_bool FlashVerifyChecksum() {
	// Naive check: if the first block is blank, there's no code there
	if (intFlashIsErased(FlashGetUserProgBaseAddress(), 4)) {
		return BLT_FALSE;
	}

	static const size_t checksumOffset = 0x1C;

	// Now do the actual CRC check to ensure we didn't get stuck with a half-written firmware image
	uint8_t* start = reinterpret_cast<uint8_t*>(FlashGetUserProgBaseAddress());

	size_t imageSize = *reinterpret_cast<size_t*>(start + checksumOffset + 4);

	if (imageSize > 1024 * 1024) {
		// impossibly large size, invalid
		return BLT_FALSE;
	}

	// part before checksum+size
	uint32_t calcChecksum = crc32(start, checksumOffset);
	// part after checksum+size
	calcChecksum = crc32inc(start + checksumOffset + 4, calcChecksum, imageSize - (checksumOffset + 4));

	uint32_t storedChecksum = *reinterpret_cast<uint32_t*>(start + checksumOffset);

	return calcChecksum == storedChecksum ? BLT_TRUE : BLT_FALSE;
}
