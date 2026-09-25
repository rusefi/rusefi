#include "pch.h"
#include "flash_int.h"
#include "hal_efl.h"
#include <cassert>
#include <limits>

alignas(32) uint8_t testFlash[32 * 1024];
EFlashDriver EFLD1 = {FLASH_STOP};
static const flash_descriptor_t bank = {testFlash, 16 * 1024, 4096, 4};
static unsigned programs, erases, starts;
static bool failStart, failProgram, failErase, missingDescriptor;
static size_t firstSector, lastSector;
const flash_descriptor_t* flashGetDescriptor(EFlashDriver*) { return missingDescriptor ? nullptr : &bank; }
int eflStart(EFlashDriver* driver, const void* config) {
    assert(config == nullptr); ++starts;
    if (failStart) { return -1; }
    driver->state = 1; return MSG_OK;
}
int flashProgram(EFlashDriver*, size_t offset, size_t size, const uint8_t* data) {
    ++programs;
    if (failProgram) { return -1; }
    std::memcpy(testFlash + offset, data, size);
    return FLASH_NO_ERROR;
}
int flashStartEraseSector(EFlashDriver*, size_t sector) {
    if (erases++ == 0) { firstSector = sector; }
    lastSector = sector;
    return failErase ? -1 : FLASH_NO_ERROR;
}
int flashQueryErase(EFlashDriver*, uint32_t*) { return FLASH_NO_ERROR; }
void chThdSleepMilliseconds(uint32_t) {}
int main() {
    std::memset(testFlash, 0xff, sizeof(testFlash));
    const auto base = FLASH_BASE;
    char data[8] = {1,2,3,4,5,6,7,8}, copy[8];
    assert(flashSizeKb() == 16);
    assert(intFlashRead(base, copy, sizeof(copy)) == FLASH_RETURN_SUCCESS);
    assert(starts == 0); // Reads only need the descriptor initialized by halInit.
    assert(intFlashIsErased(base, bank.size));
    assert(intFlashWrite(base + 4, data, 8) == FLASH_RETURN_SUCCESS);
    assert(programs == 1 && starts == 1);
    assert(intFlashCompare(base + 4, data, 8));
    assert(intFlashRead(base + 4, copy, 8) == FLASH_RETURN_SUCCESS);
    assert(std::memcmp(copy, data, 8) == 0);
    assert(!intFlashIsErased(base, bank.size));
    assert(intFlashWrite(base + 1, data, 4) == FLASH_RETURN_ALIGNERROR);
    assert(intFlashWrite(base, data, 7) == FLASH_RETURN_ALIGNERROR);
    assert(intFlashErase(base, 0) == FLASH_RETURN_SUCCESS && erases == 0);
    assert(intFlashErase(base + 4096, 8192) == FLASH_RETURN_SUCCESS);
    assert(erases == 2 && firstSector == 1 && lastSector == 2);
    assert(flashSectorSize(4) == 0);
    for (auto address : {base - 1, base + bank.size, std::numeric_limits<uintptr_t>::max() - 3}) {
        assert(intFlashWrite(address, data, 8) == FLASH_RETURN_NO_PERMISSION);
        assert(intFlashErase(address, 8) == FLASH_RETURN_NO_PERMISSION);
        assert(intFlashRead(address, copy, 8) == FLASH_RETURN_NO_PERMISSION);
        assert(!intFlashCompare(address, data, 8));
        assert(!intFlashIsErased(address, 8));
    }
    assert(intFlashErase(base + 4, std::numeric_limits<size_t>::max()) == FLASH_RETURN_NO_PERMISSION);
    assert(programs == 1 && erases == 2);
    failProgram = true;
    assert(intFlashWrite(base, data, 4) == FLASH_RETURN_OPERROR);
    failErase = true;
    assert(intFlashErase(base, 4) == FLASH_RETURN_OPERROR);
    EFLD1.state = FLASH_STOP; failStart = true;
    assert(intFlashWrite(base, data, 4) == FLASH_RETURN_OPERROR);
    missingDescriptor = true;
    assert(flashSizeKb() == 0 && flashSectorSize(0) == 0);
    assert(intFlashWrite(base, data, 4) == FLASH_RETURN_NO_PERMISSION);
}
