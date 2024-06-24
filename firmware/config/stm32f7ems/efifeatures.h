/**
 * @file efifeatures.h
 *
 * @brief In this header we can configure which firmware modules are used.
 *
 * STM32F7 config is inherited from STM32F4. This file contains only differences between F4 and F7.
 * This is more consistent way to maintain these config 'branches' and add new features.
 *
 * @date Aug 29, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

 #pragma once

// Disable ini ramdisk as a mitigation of https://github.com/rusefi/rusefi/issues/3775
// See STM32F7.ld for more info
#ifndef EFI_EMBED_INI_MSD
#define EFI_EMBED_INI_MSD FALSE
#endif

#ifndef ENABLE_PERF_TRACE
#define ENABLE_PERF_TRACE TRUE
#endif

#ifndef EFI_CONSOLE_TX_BRAIN_PIN
// todo: kill default & move into board configuration?
#define EFI_CONSOLE_TX_BRAIN_PIN Gpio::D8
#endif

#ifndef EFI_CONSOLE_RX_BRAIN_PIN
#define EFI_CONSOLE_RX_BRAIN_PIN Gpio::D9
#endif

// see also EFI_EMBED_INI_MSD which is disabled above
#ifndef EFI_USE_COMPRESSED_INI_MSD
#define EFI_USE_COMPRESSED_INI_MSD TRUE
#endif

#ifndef LUA_USER_HEAP
#define LUA_USER_HEAP 100000
#endif

// UART driver not implemented on F7
#ifndef AUX_SERIAL_DEVICE
#define AUX_SERIAL_DEVICE (&SD6)
#endif

// todo: our "DMA-half" ChibiOS patch not implemented for USARTv2/STM32F7/STM32H7
#ifndef EFI_USE_UART_DMA
#define EFI_USE_UART_DMA FALSE
#endif

// F7 may have dual bank, so flash on its own (low priority) thread so as to not block any other operations
#ifndef EFI_FLASH_WRITE_THREAD
#define EFI_FLASH_WRITE_THREAD TRUE
#endif

// note order of include - first we set F7 defaults (above) and only later we apply F4 defaults
#include "../stm32f4ems/efifeatures.h"
