/**
 * @file efifeatures.h
 *
 * @brief Configure which firmware modules are used.
 *
 * This configuration is for the MicroRusEFI board.
 *
 */
 
//#include "../../stm32f4ems/efifeatures.h"
#include "../skeleton/efifeatures.h"

#ifndef EFIFEATURES_MICRORUSEFI_H_
#define EFIFEATURES_MICRORUSEFI_H_

// CCM RAM on the STM32
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

#define BOARD_TLE6240_COUNT	0
#define BOARD_MC33972_COUNT	0
#undef  BOARD_TLE8888_COUNT
#define BOARD_TLE8888_COUNT 	1

// This would better summed elsewhere.
#define BOARD_EXT_GPIOCHIPS (BOARD_TLE6240_COUNT + BOARD_MC33972_COUNT + BOARD_TLE8888_COUNT)
// Number of external pins to keep track of
#undef BOARD_EXT_PINREPOPINS
#define BOARD_EXT_PINREPOPINS 28

#define EFI_ANALOG_SENSORS TRUE

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

// GPS reporting NMEA protocol on a serial port
#undef EFI_UART_GPS
#define EFI_UART_GPS FALSE

// consoleUartDevice is unused but provided on UART4 Tx:PC10 Rx:PC11
// USART3 would work on the same pins but is reserved for future LIN bus use.
//  ToDo: Fix so that UART4 will work here.

#undef HAL_USE_SERIAL_USB
#undef EFI_TUNER_STUDIO
#undef EFI_CONSOLE_SERIAL_DEVICE
#undef EFI_CONSOLE_UART_DEVICE
#undef TS_SERIAL_DEVICE
#undef TS_UART_DEVICE
#undef TS_UART_MODE

// Define this to use console-over-USB
#define HAL_USE_SERIAL_USB TRUE
#define EFI_TUNER_STUDIO TRUE
// Two ways to use a physical USART/UART
// #define EFI_CONSOLE_SERIAL_DEVICE (&SD1)
// #define EFI_CONSOLE_UART_DEVICE

#undef EFI_UART_ECHO_TEST_MODE
#define EFI_UART_ECHO_TEST_MODE FALSE
#define TS_UART_DEVICE (&UARTD3)
#define TS_SERIAL_DEVICE (&SD3)


// todo: document limitations of DMA mode for the UART.
#undef TS_UART_DMA_MODE
#define TS_UART_DMA_MODE FALSE

// todo: add DMA-mode for Console?
#if (TS_UART_DMA_MODE || TS_UART_MODE)
#undef EFI_CONSOLE_SERIAL_DEVICE
#endif

#undef EFI_CONSOLE_AF
#undef TS_SERIAL_AF
#undef EFI_CONSOLE_TX_PIN
#undef EFI_CONSOLE_TX_PORT
#undef EFI_CONSOLE_RX_PORT
#undef EFI_CONSOLE_RX_PIN

// todo: start using consoleSerialTxPin? Not sure
#define TS_SERIAL_AF 7

// USART3 is Alternate Function 7, UART4 is AF8
#define EFI_CONSOLE_AF 7
#define EFI_CONSOLE_TX_PORT GPIOC
#define EFI_CONSOLE_TX_PIN 10
#define EFI_CONSOLE_RX_PORT GPIOC
#define EFI_CONSOLE_RX_PIN 11

#define EFI_PRINT_ERRORS_AS_WARNINGS TRUE

#endif /* EFIFEATURES_MICRORUSEFI_H_ */

/*
 * Local variables:
 *  c-basic-indent: 4
 *  tab-width: 4
 * End:
 */
