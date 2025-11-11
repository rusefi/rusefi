#include "pch.h"
#include "flash_int.h"

extern "C" {
	#include "boot.h"
	#include "flash.h"
}

#ifdef STM32H7XX
//Since a 10-bit ECC code is associated to each 256-bit data Flash word,
//only write operations by 256 bits are executed in the non-volatile memory.
#define FLASH_ECC_LINE_SIZE 	(256 / 8)

static blt_int8u flashBuffer[FLASH_ECC_LINE_SIZE];
static blt_addr flashAddr = 0x0;

#define FLASH_WRITE_STEP		FLASH_ECC_LINE_SIZE
#define FLASH_WRITE_ADDR_MASK	(~(FLASH_WRITE_STEP - 1))

static void FlashBufferReset()
{
	memset(flashBuffer, 0xff, sizeof(flashBuffer));
	flashAddr = 0;
}

static blt_bool FlashBufferFlush()
{
	if (flashAddr == 0x0) {
		return BLT_TRUE;
	}

	int result = intFlashWrite(flashAddr, (const char*)flashBuffer, sizeof(flashBuffer));

	FlashBufferReset();

	return (result == FLASH_RETURN_SUCCESS) ? BLT_TRUE : BLT_FALSE;
}

static blt_bool FlashBufferedWrite(blt_addr addr, blt_int32u len, blt_int8u *data)
{
	while (len) {
		if ((addr & FLASH_WRITE_ADDR_MASK) != flashAddr) {
			// crossing ECC line boundary
			FlashBufferFlush();

			flashAddr = addr & FLASH_WRITE_ADDR_MASK;
		}

		off_t off = addr - flashAddr;
		size_t chunk = minI(sizeof(flashBuffer) - off, len);
		memcpy(flashBuffer + off, data, chunk);

		addr += chunk;
		data += chunk;
		len -= chunk;
	}

	return BLT_TRUE;
}

#endif

void FlashInit() {
	// Flash already init by ChibiOS
#ifdef STM32H7XX
	FlashBufferReset();
#endif
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

#ifdef STM32H7XX
	return FlashBufferedWrite(addr, len, data);
#else // not STM32H7
	return (FLASH_RETURN_SUCCESS == intFlashWrite(addr, (const char*)data, len)) ? BLT_TRUE : BLT_FALSE;
#endif
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
#ifdef STM32H7XX
	FlashBufferFlush();
#endif
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

blt_bool isFlashDualBank(void) {
#ifdef STM32F7XX
	// cleared bit indicates dual bank
	return (FLASH->OPTCR & FLASH_OPTCR_nDBANK) == 0 ? BLT_TRUE : BLT_FALSE;
#else
	return BLT_TRUE;
#endif
}
