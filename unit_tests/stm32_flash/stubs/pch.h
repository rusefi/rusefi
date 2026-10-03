#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <sys/types.h>

#define TRUE 1
#define FALSE 0
#define HAL_SUCCESS 0
#define HAL_FAILED 1
#define STM32H7XX
#define STM32_VDD 330
#define CORTEX_MODEL 7

// Register bits from the STM32H7 CMSIS headers. The tests run the production
// driver against ordinary memory; no MCU or fixed-address host mapping needed.
#define FLASH_CR_LOCK (1U << 0)
#define FLASH_CR_PG (1U << 1)
#define FLASH_CR_SER (1U << 2)
#define FLASH_CR_PSIZE_0 (1U << 4)
#define FLASH_CR_PSIZE_1 (1U << 5)
#define FLASH_CR_START (1U << 7)
#define FLASH_CR_SNB_Pos 8
#define FLASH_CR_SNB_Msk (7U << FLASH_CR_SNB_Pos)
#define FLASH_SR_QW (1U << 2)
#define FLASH_SR_WRPERR (1U << 17)
#define FLASH_SR_PGSERR (1U << 18)
#define FLASH_SR_STRBERR (1U << 19)
#define FLASH_SR_INCERR (1U << 21)
#define FLASH_SR_OPERR (1U << 22)
#define FLASH_SR_RDPERR (1U << 23)
#define FLASH_SR_RDSERR (1U << 24)
#define FLASH_SR_DBECCERR (1U << 26)
#define FLASH_SR_CRCRDERR (1U << 28)
#define PWR_CR1_PLS_Msk (7U << 5)
#define PWR_CR1_PLS_LEV0 0U
#define PWR_CR1_PLS_LEV5 (5U << 5)
#define PWR_CR1_PVDEN (1U << 4)
#define PWR_CSR1_PVDO (1U << 4)
#define PWR_CR_PLS_LEV5 (5U << 5)

struct FlashRegisters {
    uint32_t CR1 = 0, SR1 = 0, KEYR1 = 0, CCR1 = 0;
#ifdef STM32H743xx
    uint32_t CR2 = 0, SR2 = 0, KEYR2 = 0, CCR2 = 0;
#endif
};
struct PowerRegisters { uint32_t CR1 = 0, CSR1 = 0; };
extern FlashRegisters flashRegisters;
extern PowerRegisters powerRegisters;
#define FLASH (&flashRegisters)
#define PWR (&powerRegisters)

alignas(32) extern uint8_t flashMemory[2 * 1024 * 1024];
#define FLASH_BANK1_BASE reinterpret_cast<uintptr_t>(flashMemory)
#define FLASH_BANK2_BASE (FLASH_BANK1_BASE + 1024 * 1024)
#define FLASH_BASE FLASH_BANK1_BASE
void flashBarrier();
#define __DSB() flashBarrier()
#define __ISB() do {} while (false)
inline void SCB_InvalidateDCache_by_Addr(uint32_t*, size_t) {}
inline size_t minI(size_t a, size_t b) { return std::min(a, b); }
inline size_t flashSizeKb() {
#ifdef STM32H743xx
    return 2048;
#else
    return 1024;
#endif
}
inline uint32_t crc32(const void*, size_t) { return 0; }
inline uint32_t crc32inc(const void*, uint32_t, size_t) { return 0; }
