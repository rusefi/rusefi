#include "pch.h"
#include "flash_int.h"

extern "C" {
	#include "boot.h"
	#include "flash.h"
}

#if defined(STM32H7XX) || defined(AT32F435xx)
//Since a 10-bit ECC code is associated to each 256-bit data Flash word,
//only write operations by 256 bits are executed in the non-volatile memory.
//The AT32F435 flash likewise rejects a second program to a word that is not
//fully erased (EPPERR), so each 32-bit word may be programmed exactly once:
//buffer the incoming bytes and flush whole words on line boundaries.
#ifdef STM32H7XX
#define FLASH_ECC_LINE_SIZE 	(256 / 8)
#else
#define FLASH_ECC_LINE_SIZE 	(32 / 8)
#endif

static blt_int8u flashBuffer[FLASH_ECC_LINE_SIZE];
static blt_addr flashAddr = 0x0;
static blt_bool flashWriteFailed = BLT_FALSE;

#define FLASH_WRITE_STEP		FLASH_ECC_LINE_SIZE
#define FLASH_WRITE_ADDR_MASK	(~(FLASH_WRITE_STEP - 1))

static void FlashBufferReset()
{
	memset(flashBuffer, 0xff, sizeof(flashBuffer));
	flashAddr = 0;
}

static blt_bool FlashBufferFlush()
{
	if (flashWriteFailed) {
		return BLT_FALSE;
	}
	if (flashAddr == 0x0) {
		return BLT_TRUE;
	}

	int result = intFlashWrite(flashAddr, (const char*)flashBuffer, sizeof(flashBuffer));

	FlashBufferReset();

	flashWriteFailed = (result != FLASH_RETURN_SUCCESS);
	return flashWriteFailed ? BLT_FALSE : BLT_TRUE;
}

static blt_bool FlashBufferedWrite(blt_addr addr, blt_int32u len, blt_int8u *data)
{
	while (len) {
		if ((addr & FLASH_WRITE_ADDR_MASK) != flashAddr) {
			// crossing flash line boundary - program the buffered line
			if (!FlashBufferFlush()) {
				return BLT_FALSE;
			}

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
#if defined(STM32H7XX) || defined(AT32F435xx)
	FlashBufferReset();
	flashWriteFailed = BLT_FALSE;
#endif
}

blt_addr FlashGetUserProgOffset() {
#ifdef STM32H7XX
	return 128 * 1024;
#else // not STM32H7
	return 32 * 1024;
#endif
}

blt_addr FlashGetUserProgBaseAddress() {
	return FLASH_BASE + FlashGetUserProgOffset();
}

#if defined(AT32F435xx)
static bool isApplicationRange(blt_addr addr, blt_int32u len) {
	const auto base = FlashGetUserProgBaseAddress();
	const auto end = FLASH_BASE + flashSizeKb() * 1024;
	return addr >= base && addr <= end && len <= end - addr;
}
#endif

blt_bool FlashWrite(blt_addr addr, blt_int32u len, blt_int8u *data) {
	// don't allow overwriting the bootloader
	if (addr < FlashGetUserProgBaseAddress()
#if defined(AT32F435xx)
		|| !isApplicationRange(addr, len)
#endif
	) {
		return BLT_FALSE;
	}

#if defined(STM32H7XX) || defined(AT32F435xx)
	return FlashBufferedWrite(addr, len, data);
#else // not STM32H7 / AT32F435
	return (FLASH_RETURN_SUCCESS == intFlashWrite(addr, (const char*)data, len)) ? BLT_TRUE : BLT_FALSE;
#endif
}

blt_bool FlashErase(blt_addr addr, blt_int32u len) {
	// don't allow erasing the bootloader
	if (addr < FlashGetUserProgBaseAddress()
#if defined(AT32F435xx)
		|| !isApplicationRange(addr, len)
#endif
	) {
		return BLT_FALSE;
	}

#if defined(STM32H7XX) || defined(AT32F435xx)
	if (len == 0) {
		return BLT_TRUE;
	}
	// Preserve a pending line outside the erased range for hosts that interleave
	// segment erase/program operations. An overlapping erase abandons that line.
	if (flashAddr != 0) {
		const bool before = flashAddr < addr && addr - flashAddr >= sizeof(flashBuffer);
		const bool after = flashAddr >= addr && flashAddr - addr >= len;
		if ((before || after) && !FlashBufferFlush()) {
			return BLT_FALSE;
		}
	}
	FlashBufferReset();
	flashWriteFailed = BLT_FALSE;
#endif
	if (!intFlashIsErased(addr, len)) {
		return (FLASH_RETURN_SUCCESS == intFlashErase(addr, len)) ? BLT_TRUE : BLT_FALSE;
	}

	return BLT_TRUE;
}

blt_bool FlashDone() {
#if defined(STM32H7XX) || defined(AT32F435xx)
	return FlashBufferFlush();
#else
	return BLT_TRUE;
#endif
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

	if (imageSize < checksumOffset + 8 || imageSize > (flashSizeKb() * 1024 - FlashGetUserProgOffset())) {
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
