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
 
#include "../stm32f4ems/efifeatures.h"

#pragma once
#define EFI_BOOST_CONTROL TRUE

// Warning! This is a test config!

#undef EFI_USE_CCM
#define EFI_USE_CCM TRUE

#undef EFI_POTENTIOMETER
#define EFI_POTENTIOMETER FALSE

#undef EFI_MAX_31855
#define EFI_MAX_31855 FALSE

#undef EFI_MCP_3208
#define EFI_MCP_3208 FALSE

#undef EFI_MC33816
#define EFI_MC33816 FALSE

#undef EFI_DENSO_ADC
#define EFI_DENSO_ADC FALSE

#undef EFI_MEMS
#define EFI_MEMS FALSE

#ifndef BOARD_TLE6240_COUNT
#define BOARD_TLE6240_COUNT         1
#endif

#ifndef BOARD_MC33972_COUNT
#define BOARD_MC33972_COUNT			1
#endif

#ifndef BOARD_TLE8888_COUNT
#define BOARD_TLE8888_COUNT 	1
#endif

#ifndef BOARD_MC33810_COUNT
#define BOARD_MC33810_COUNT		0
#endif

#undef EFI_CAN_SUPPORT
#define EFI_CAN_SUPPORT TRUE

#undef EFI_AUX_SERIAL
#define EFI_AUX_SERIAL TRUE

#undef EFI_HD44780_LCD
#define EFI_HD44780_LCD TRUE

#undef EFI_LCD
#define EFI_LCD FALSE

/**
 * Do we need file logging (like SD card) logic?
 */
#undef EFI_FILE_LOGGING
#define EFI_FILE_LOGGING FALSE

#undef EFI_USB_SERIAL
#define EFI_USB_SERIAL TRUE

/**
 * Do we need GPS logic?
 */
#undef EFI_UART_GPS
#define EFI_UART_GPS FALSE

// todo: start using consoleUartDevice? Not sure
#undef EFI_CONSOLE_SERIAL_DEVICE

#undef EFI_CONSOLE_UART_DEVICE

// todo: our "DMA-half" ChibiOS patch not implemented for USARTv2/STM32F7
#undef TS_UART_DMA_MODE
#define TS_UART_DMA_MODE FALSE

#undef PRIMARY_UART_DMA_MODE
#define PRIMARY_UART_DMA_MODE FALSE

#undef TS_UART_DEVICE
//#define TS_UART_DEVICE (&UARTD3)

#undef TS_SERIAL_DEVICE
#define TS_SERIAL_DEVICE (&SD3)

#define AUX_SERIAL_DEVICE (&SD6)

// todo: add DMA-mode for Console?
#if (TS_UART_DMA_MODE || TS_UART_MODE)
#undef EFI_CONSOLE_SERIAL_DEVICE
#endif

// todo: start using consoleSerialTxPin? Not sure
#undef EFI_CONSOLE_TX_BRAIN_PIN
#define EFI_CONSOLE_TX_BRAIN_PIN GPIOD_8

// todo: start using consoleSerialRxPin? Not sure
#undef EFI_CONSOLE_RX_BRAIN_PIN
#define EFI_CONSOLE_RX_BRAIN_PIN GPIOD_9

// todo: temporary ignore errors, this is a test config
#define EFI_PRINT_ERRORS_AS_WARNINGS TRUE
