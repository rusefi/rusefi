#include "pch.h"
#include "flash_int.h"
extern "C" {
#include "types.h"
#include "flash.h"
}
#include <cassert>
#include <iostream>

alignas(32) uint8_t testFlash[256 * 1024];
static bool failWrite;
static unsigned writes;
#ifdef STM32H7XX
static constexpr size_t lineSize = 32;
#else
static constexpr size_t lineSize = 4;
#endif
size_t flashSizeKb() { return sizeof(testFlash) / 1024; }
uint32_t crc32(const void*, size_t) { return 0; }
uint32_t crc32inc(const void*, uint32_t, size_t) { return 0; }
bool intFlashIsErased(flashaddr_t addr, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (*reinterpret_cast<uint8_t*>(addr + i) != 0xff) { return false; }
    }
    return true;
}
int intFlashErase(flashaddr_t addr, size_t size) {
    std::memset(reinterpret_cast<void*>(addr), 0xff, size);
    return FLASH_RETURN_SUCCESS;
}
int intFlashWrite(flashaddr_t addr, const char* data, size_t size) {
    ++writes;
    if (failWrite || (addr % lineSize) || size != lineSize || !intFlashIsErased(addr, size)) {
        return FLASH_RETURN_OPERROR;
    }
    std::memcpy(reinterpret_cast<void*>(addr), data, size);
    return FLASH_RETURN_SUCCESS;
}
static void reset() {
    std::memset(testFlash, 0xff, sizeof(testFlash));
    writes = 0;
    failWrite = false;
    FlashInit();
}
int main() {
    reset();
    const auto base = FlashGetUserProgBaseAddress();
    uint8_t data[77];
    for (size_t i = 0; i < sizeof(data); ++i) { data[i] = static_cast<uint8_t>(i); }
    for (size_t i = 0; i < sizeof(data); i += 7) {
        assert(FlashWrite(base + i, 7, data + i));
    }
    assert(FlashDone());
    assert(std::memcmp(reinterpret_cast<void*>(base), data, sizeof(data)) == 0);
    assert(writes == (sizeof(data) + lineSize - 1) / lineSize);
    assert(*reinterpret_cast<uint8_t*>(base + sizeof(data)) == 0xff);
    assert(!FlashWrite(base - 1, 1, data));

#ifdef AT32F435xx
    assert(!FlashWrite(FLASH_BASE + sizeof(testFlash), 1, data));
    assert(!FlashErase(FLASH_BASE + sizeof(testFlash), 4));
    assert(!FlashWrite(base, UINT32_MAX, data));
#endif
    reset();
    // Reject a corrupt image size before CRC length arithmetic can underflow.
    std::memset(reinterpret_cast<void*>(base), 0, 64);
    assert(!FlashVerifyChecksum());

    reset();
    assert(FlashWrite(base, 1, data));
    failWrite = true;
    assert(!FlashDone());
    assert(!FlashDone()); // Failure remains latched until a new session/erase.

    reset();
    assert(FlashWrite(base, 1, data));
    failWrite = true;
    assert(!FlashWrite(base + lineSize, 1, data));
    assert(!FlashDone());

    reset();
    assert(FlashWrite(base, 1, data));
    assert(FlashErase(base, 4096));
    assert(FlashDone()); // Erase discards the unflushed data.
    assert(intFlashIsErased(base, lineSize));
    reset();
    assert(FlashWrite(base, 1, data));
    assert(FlashErase(base + 4096, 4096));
    assert(FlashDone());
    assert(*reinterpret_cast<uint8_t*>(base) == data[0]); // Unrelated erase preserves pending data.
    std::cout << "flash adapter checks passed\n";
}
