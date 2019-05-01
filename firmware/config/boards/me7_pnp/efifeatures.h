/**
 * @file efifeatures.h
 *
 * @brief In this header we can configure which firmware modules are used.
 *
 * STM32F7 config is inherited from STM32F4. This file contains only differences between F4 and F7.
 * This is more consistent way to maintain these config 'branches' and add new features.
 *
 * @date Aug 29, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
 
#include "../stm32f4ems/efifeatures.h"

#ifndef EFIFEATURES_STM32F7_H_
#define EFIFEATURES_STM32F7_H_
#if !defined(EFI_ENABLE_ASSERTS)
 #define EFI_ENABLE_ASSERTS FALSE
#endif /* EFI_ENABLE_ASSERTS */
// Warning! This is a test config!

// todo: reconfigure RAM sections for STM32F7
#undef EFI_USE_CCM
#define EFI_USE_CCM FALSE


#ifndef EFI_INTERNAL_ADC
#define EFI_INTERNAL_ADC TRUE
#endif


#undef EFI_POTENTIOMETER
#define EFI_POTENTIOMETER FALSE
#define EFI_ELECTRONIC_THROTTLE_BODY TRUE
#undef EFI_MAX_31855
#define EFI_MAX_31855 FALSE

#undef EFI_MCP_3208
#define EFI_MCP_3208 FALSE

#undef EFI_HIP_9011
#define EFI_HIP_9011 FALSE

#undef EFI_CJ125
#define EFI_CJ125 FALSE

#undef EFI_DENSO_ADC
#define EFI_DENSO_ADC FALSE

#undef EFI_MEMS
#define EFI_MEMS FALSE

#ifndef BOARD_TLE6240_COUNT
#define BOARD_TLE6240_COUNT         0
#endif

#ifndef BOARD_MC33972_COUNT
#define BOARD_MC33972_COUNT			0
#endif

#ifndef BOARD_TLE8888_COUNT
#define BOARD_TLE8888_COUNT 	1
#endif

// todo: move this outside of efifeatures.h
#define BOARD_EXT_GPIOCHIPS			(BOARD_TLE6240_COUNT + BOARD_MC33972_COUNT + BOARD_TLE8888_COUNT)


#undef EFI_CAN_SUPPORT
#define EFI_CAN_SUPPORT TRUE

#undef EFI_HD44780_LCD
#define EFI_HD44780_LCD FALSE

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
#undef EFI_CONSOLE_UART_DEVICE
#define EFI_CONSOLE_UART_DEVICE (&SD3)

// todo: our "DMA-half" ChibiOS patch not implemented for USARTv2/STM32F7
#undef TS_UART_DMA_MODE
#define TS_UART_DMA_MODE FALSE

#undef TS_UART_DEVICE
#define TS_UART_DEVICE (&UARTD3)
#undef TS_SERIAL_DEVICE
#define TS_SERIAL_DEVICE (&SD3)

// todo: add DMA-mode for Console?
#if (TS_UART_DMA_MODE || TS_UART_MODE)
#undef EFI_CONSOLE_UART_DEVICE
#endif

// todo: start using consoleSerialTxPin? Not sure
#undef EFI_CONSOLE_TX_PORT
#define EFI_CONSOLE_TX_PORT GPIOB
#undef EFI_CONSOLE_TX_PIN
#define EFI_CONSOLE_TX_PIN 10
// todo: start using consoleSerialRxPin? Not sure
#undef EFI_CONSOLE_RX_PORT
#define EFI_CONSOLE_RX_PORT GPIOB
#undef EFI_CONSOLE_RX_PIN
#define EFI_CONSOLE_RX_PIN 11

// LED_ERROR_BRAIN_PIN should match LED_ERROR_PORT/LED_ERROR_PIN
#undef LED_ERROR_BRAIN_PIN
#define LED_ERROR_BRAIN_PIN GPIOB_14
#undef LED_ERROR_PORT
#define LED_ERROR_PORT GPIOB
#undef LED_ERROR_PIN
#define LED_ERROR_PIN 14

// todo: temporary ignore errors, this is a test config
#define EFI_PRINT_ERRORS_AS_WARNINGS TRUE

#endif /* EFIFEATURES_STM32F7_H_ */
