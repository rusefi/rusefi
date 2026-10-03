#include "pch.h"
#include "flash_int.h"
#include <cassert>
#include <iostream>
#include <vector>

alignas(32) uint8_t flashMemory[2 * 1024 * 1024];
static unsigned writeCalls;
static int writeResult;
static std::vector<uint8_t> written;
int intFlashWrite(flashaddr_t, const char* data, size_t size) {
    writeCalls++;
    written.insert(written.end(), data, data + size);
    return writeResult;
}
int intFlashErase(flashaddr_t, size_t) { return FLASH_RETURN_SUCCESS; }
bool intFlashIsErased(flashaddr_t, size_t) { return true; }

#include "../../firmware/bootloader/openblt_chibios/openblt_flash.cpp"

int main() {
    uint8_t data[64];
    for (size_t i = 0; i < sizeof(data); i++) {
        data[i] = static_cast<uint8_t>(i);
    }
    const auto app = FlashGetUserProgBaseAddress();
    FlashInit();
    writeResult = FLASH_RETURN_WPERROR;
    assert(FlashWrite(app, sizeof(data), data) == BLT_FALSE);
    assert(writeCalls == 1);
    assert(FlashDone() == BLT_FALSE);
    assert(FlashWrite(app + 64, 32, data) == BLT_FALSE);
    assert(writeCalls == 1); // Failure stays latched until a new session.

    FlashInit();
    assert(FlashWrite(app, 16, data) == BLT_TRUE);
    assert(FlashDone() == BLT_FALSE);
    assert(writeCalls == 2);
    assert(FlashDone() == BLT_FALSE);
    assert(writeCalls == 2);

    FlashInit();
    writeResult = FLASH_RETURN_SUCCESS;
    writeCalls = 0;
    written.clear();
    // Uneven transport packets must produce complete flash words in order.
    assert(FlashWrite(app, 7, data) == BLT_TRUE);
    assert(FlashWrite(app + 7, 42, data + 7) == BLT_TRUE);
    assert(FlashWrite(app + 49, 15, data + 49) == BLT_TRUE);
    assert(FlashDone() == BLT_TRUE);
    assert(writeCalls == 2);
    assert(written == std::vector<uint8_t>(data, data + sizeof(data)));
    assert(FlashDone() == BLT_TRUE);
    assert(writeCalls == 2);

    FlashInit();
    written.clear();
    assert(FlashWrite(app, 3, data) == BLT_TRUE);
    assert(FlashDone() == BLT_TRUE);
    assert(written.size() == 32);
    assert(std::equal(written.begin(), written.begin() + 3, data));
    assert(std::all_of(written.begin() + 3, written.end(), [](uint8_t b) { return b == 0xff; }));
    assert(FlashWrite(app - 32, 32, data) == BLT_FALSE);
    assert(FlashErase(app - 32, 32) == BLT_FALSE);
    std::cout << "OpenBLT: error propagation, session reset, buffering and protection passed\n";
}
