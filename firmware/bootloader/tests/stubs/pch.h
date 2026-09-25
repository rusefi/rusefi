#pragma once
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <sys/types.h>
#define HAL_SUCCESS 0
#define STM32_VDD 330
#define minI(a, b) std::min<size_t>(a, b)
alignas(32) extern uint8_t testFlash[];
#define FLASH_BASE reinterpret_cast<uintptr_t>(testFlash)
size_t flashSizeKb();
uint32_t crc32(const void*, size_t);
uint32_t crc32inc(const void*, uint32_t, size_t);
