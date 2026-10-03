#include "pch.h"
#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

FlashRegisters flashRegisters;
PowerRegisters powerRegisters;
alignas(32) uint8_t flashMemory[2 * 1024 * 1024];
static std::vector<unsigned> programmingBanks;
static std::vector<std::pair<unsigned, unsigned>> erasedSectors;
static uint32_t programError;
static uint32_t eraseError;
static bool rejectUnlock;
static bool programming[2];

// Model only the controller effects used by the driver. This does not emulate
// flash timing, ECC or CPU caches; those still need hardware validation.
static void serviceBank(unsigned bank, uint32_t& cr, uint32_t& sr,
        uint32_t& keyr, uint32_t& ccr) {
    if (keyr == 0xCDEF89AB) {
        if (!rejectUnlock) {
            cr &= ~FLASH_CR_LOCK;
        }
        keyr = 0;
    }
    sr &= ~ccr;
    ccr = 0;
    if ((cr & FLASH_CR_PG) && !programming[bank - 1]) {
        programmingBanks.push_back(bank);
        sr |= programError;
    }
    programming[bank - 1] = cr & FLASH_CR_PG;
    if (cr & FLASH_CR_START) {
        assert(cr & FLASH_CR_SER);
        const unsigned sector = (cr & FLASH_CR_SNB_Msk) >> FLASH_CR_SNB_Pos;
        erasedSectors.emplace_back(bank, sector);
        sr |= eraseError;
        if (!eraseError) {
            memset(flashMemory + (bank - 1) * 1024 * 1024 + sector * 128 * 1024,
                0xff, 128 * 1024);
        }
        cr &= ~FLASH_CR_START;
    }
}

void flashBarrier() {
    serviceBank(1, FLASH->CR1, FLASH->SR1, FLASH->KEYR1, FLASH->CCR1);
#ifdef STM32H743xx
    serviceBank(2, FLASH->CR2, FLASH->SR2, FLASH->KEYR2, FLASH->CCR2);
#endif
}

#include "../../firmware/hw_layer/ports/stm32/flash_int.cpp"

size_t flashSectorSize(flashsector_t) { return 128 * 1024; }

static void reset() {
    flashRegisters = {};
    powerRegisters = {};
    FLASH->CR1 = FLASH_CR_LOCK;
#ifdef STM32H743xx
    FLASH->CR2 = FLASH_CR_LOCK;
#endif
    programmingBanks.clear();
    erasedSectors.clear();
    programError = eraseError = 0;
    rejectUnlock = false;
    programming[0] = programming[1] = false;
    memset(flashMemory, 0xff, sizeof(flashMemory));
}

static void checkLocked() {
    assert(FLASH->CR1 & FLASH_CR_LOCK);
    assert(!(FLASH->CR1 & FLASH_CR_PG));
#ifdef STM32H743xx
    assert(FLASH->CR2 & FLASH_CR_LOCK);
    assert(!(FLASH->CR2 & FLASH_CR_PG));
#endif
}

int main() {
    const auto app = FLASH_BANK1_BASE + 128 * 1024;
    const char data[64] = {1, 2, 3, 4};
    reset();
    assert(intFlashSectorAt(app) == 1);
    assert(intFlashSectorBegin(1) == app);
    assert(intFlashWrite(app, data, sizeof(data)) == FLASH_RETURN_SUCCESS);
    assert((programmingBanks == std::vector<unsigned>{1, 1}));
    assert(memcmp(reinterpret_cast<void*>(app), data, sizeof(data)) == 0);
    checkLocked();

#ifdef STM32H743xx
    reset();
    const auto settings = FLASH_BANK2_BASE;
    assert(intFlashSectorAt(settings) == 8);
    assert(intFlashSectorBegin(8) == settings);
    assert(intFlashWrite(settings, data, sizeof(data)) == FLASH_RETURN_SUCCESS);
    assert((programmingBanks == std::vector<unsigned>{2, 2}));
    checkLocked();

    reset();
    assert(intFlashWrite(settings - 32, data, sizeof(data)) == FLASH_RETURN_SUCCESS);
    assert((programmingBanks == std::vector<unsigned>{1, 2}));
    checkLocked();
#endif

    reset();
    memset(flashMemory, 0x5a, sizeof(flashMemory));
    assert(intFlashErase(app, 32) == FLASH_RETURN_SUCCESS);
    assert((erasedSectors == std::vector<std::pair<unsigned, unsigned>>{{1, 1}}));
    assert(flashMemory[0] == 0x5a); // Bootloader survives.
    assert(flashMemory[1024 * 1024] == 0x5a); // Settings survive.
    assert(intFlashIsErased(app, 128 * 1024));
    checkLocked();

#ifdef STM32H743xx
    reset();
    memset(flashMemory, 0x5a, sizeof(flashMemory));
    assert(intFlashErase(FLASH_BANK2_BASE - 32, 64) == FLASH_RETURN_SUCCESS);
    assert((erasedSectors == std::vector<std::pair<unsigned, unsigned>>{{1, 7}, {2, 0}}));
    assert(flashMemory[0] == 0x5a);
    assert(flashMemory[1024 * 1024 + 128 * 1024] == 0x5a);
    checkLocked();
#endif

    // Return each programming error, stop before the second word, and relock.
    const std::pair<uint32_t, int> errors[] = {
        {FLASH_SR_WRPERR, FLASH_RETURN_WPERROR},
        {FLASH_SR_PGSERR, FLASH_RETURN_PSEQERROR},
        {FLASH_SR_STRBERR, FLASH_RETURN_ALIGNERROR},
        {FLASH_SR_INCERR, FLASH_RETURN_PSEQERROR},
        {FLASH_SR_OPERR, FLASH_RETURN_OPERROR},
        {FLASH_SR_DBECCERR, FLASH_RETURN_BAD_FLASH},
    };
    for (auto [status, result] : errors) {
        reset();
        programError = status;
        assert(intFlashWrite(app, data, sizeof(data)) == result);
        assert(programmingBanks.size() == 1);
        assert(intFlashIsErased(app + 32, 32));
        checkLocked();
#ifdef STM32H743xx
        reset();
        programError = status;
        assert(intFlashWrite(FLASH_BANK2_BASE, data, sizeof(data)) == result);
        assert((programmingBanks == std::vector<unsigned>{2}));
        checkLocked();
#endif
    }

    reset();
    eraseError = FLASH_SR_WRPERR;
    assert(intFlashErase(app, 256 * 1024) == FLASH_RETURN_WPERROR);
    assert(erasedSectors.size() == 1);
    checkLocked();

    reset();
    rejectUnlock = true;
    assert(intFlashWrite(app, data, sizeof(data)) == FLASH_RETURN_NO_PERMISSION);
    assert(programmingBanks.empty());
    checkLocked();

    reset();
    PWR->CSR1 = PWR_CSR1_PVDO;
    assert(intFlashWrite(app, data, sizeof(data)) == FLASH_RETURN_LOWVOLTAGEERROR);
    assert(programmingBanks.empty());
    checkLocked();

    reset();
    // Unaligned source and a short final word must not overread the buffer.
    assert(intFlashWrite(app, data + 1, 3) == FLASH_RETURN_SUCCESS);
    assert(memcmp(reinterpret_cast<void*>(app), data + 1, 3) == 0);
    assert(intFlashIsErased(app + 3, 29));
    checkLocked();

    reset();
    assert(intFlashWrite(app + 1, data, 32) == FLASH_RETURN_ALIGNERROR);
    assert(intFlashWrite(app, nullptr, 0) == FLASH_RETURN_SUCCESS);
    assert(intFlashErase(app, 0) == FLASH_RETURN_SUCCESS);
    assert(intFlashErase(FLASH_BASE - 1, 1) == FLASH_RETURN_BAD_FLASH);
    const auto end = FLASH_BASE + flashSizeKb() * 1024;
    assert(intFlashWrite(end, data, 32) == FLASH_RETURN_BAD_FLASH);
    assert(intFlashWrite(end - 32, data, 64) == FLASH_RETURN_BAD_FLASH);
    assert(intFlashErase(end - 32, 64) == FLASH_RETURN_BAD_FLASH);
    assert(programmingBanks.empty());
    assert(erasedSectors.empty());
    checkLocked();

    std::cout << "Flash driver: bank selection, erase mapping, errors and boundaries passed\n";
}
