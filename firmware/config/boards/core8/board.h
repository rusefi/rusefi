/**
 * @file boards/core8/board.h
 *
 * @author Ben Brazdziunas, 2022
 */

#define BOARD_NAME "core8"

#ifndef BOARD_IO_H
#define BOARD_IO_H

#undef EFI_RTC
#define EFI_RTC TRUE

#undef EFI_USB_SERIAL
#define EFI_USB_SERIAL TRUE

#undef EFI_USB_AF
#define EFI_USB_AF 10U

#undef EFI_USB_SERIAL_DM
#define EFI_USB_SERIAL_DM Gpio::A11

#undef EFI_USB_SERIAL_DP
#define EFI_USB_SERIAL_DP Gpio::A12

#undef STM32_SERIAL_USE_USART1
#define STM32_SERIAL_USE_USART1 FALSE

#undef STM32_UART_USE_USART1
#define STM32_UART_USE_USART1 TRUE

#undef TS_PRIMARY_PORT
#define TS_PRIMARY_PORT UARTD1

#undef EFI_CONSOLE_TX_BRAIN_PIN
#define EFI_CONSOLE_TX_BRAIN_PIN Gpio::A9

#undef EFI_CONSOLE_RX_BRAIN_PIN
#define EFI_CONSOLE_RX_BRAIN_PIN Gpio::A10

#undef EFI_SDC_DEVICE
#define EFI_SDC_DEVICE SDCD1

// #undef HAL_USE_USB_MSD
// #define HAL_USE_USB_MSD FALSE

#undef LED_CRITICAL_ERROR_BRAIN_PIN
#define LED_CRITICAL_ERROR_BRAIN_PIN Gpio::G11

// Ignore USB VBUS pin (we're never a host, only a device)
#define BOARD_OTG_NOVBUSSENS TRUE

/*
 * Board oscillators-related settings.
 * NOTE: LSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK 32768U
#endif

#define STM32_LSEDRV (3U << 3U)

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD 300U

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n) (0U << ((n)*2U))
#define PIN_MODE_OUTPUT(n) (1U << ((n)*2U))
#define PIN_MODE_ALTERNATE(n) (2U << ((n)*2U))
#define PIN_MODE_ANALOG(n) (3U << ((n)*2U))
#define PIN_ODR_LOW(n) (0U << (n))
#define PIN_ODR_HIGH(n) (1U << (n))
#define PIN_OTYPE_PUSHPULL(n) (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n) (1U << (n))
#define PIN_OSPEED_VERYLOW(n) (0U << ((n)*2U))
#define PIN_OSPEED_LOW(n) (1U << ((n)*2U))
#define PIN_OSPEED_MEDIUM(n) (2U << ((n)*2U))
#define PIN_OSPEED_HIGH(n) (3U << ((n)*2U))
#define PIN_PUPDR_FLOATING(n) (0U << ((n)*2U))
#define PIN_PUPDR_PULLUP(n) (1U << ((n)*2U))
#define PIN_PUPDR_PULLDOWN(n) (2U << ((n)*2U))
#define PIN_AFIO_AF(n, v) ((v) << (((n) % 8U) * 4U))

#define VAL_GPIOA_MODER (PIN_MODE_INPUT(0) |  \
						 PIN_MODE_INPUT(1) |  \
						 PIN_MODE_INPUT(2) |  \
						 PIN_MODE_INPUT(3) |  \
						 PIN_MODE_INPUT(4) |  \
						 PIN_MODE_INPUT(5) |  \
						 PIN_MODE_INPUT(6) |  \
						 PIN_MODE_INPUT(7) |  \
						 PIN_MODE_INPUT(8) |  \
						 PIN_MODE_INPUT(9) |  \
						 PIN_MODE_INPUT(10) | \
						 PIN_MODE_INPUT(11) | \
						 PIN_MODE_INPUT(12) | \
						 PIN_MODE_INPUT(13) | \
						 PIN_MODE_INPUT(14) | \
						 PIN_MODE_INPUT(15))

#define VAL_GPIOA_OTYPER (PIN_OTYPE_PUSHPULL(0) |  \
						  PIN_OTYPE_PUSHPULL(1) |  \
						  PIN_OTYPE_PUSHPULL(2) |  \
						  PIN_OTYPE_PUSHPULL(3) |  \
						  PIN_OTYPE_PUSHPULL(4) |  \
						  PIN_OTYPE_PUSHPULL(5) |  \
						  PIN_OTYPE_PUSHPULL(6) |  \
						  PIN_OTYPE_PUSHPULL(7) |  \
						  PIN_OTYPE_PUSHPULL(8) |  \
						  PIN_OTYPE_PUSHPULL(9) |  \
						  PIN_OTYPE_PUSHPULL(10) | \
						  PIN_OTYPE_PUSHPULL(11) | \
						  PIN_OTYPE_PUSHPULL(12) | \
						  PIN_OTYPE_PUSHPULL(13) | \
						  PIN_OTYPE_PUSHPULL(14) | \
						  PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOA_OSPEEDR (PIN_OSPEED_HIGH(0) |  \
						   PIN_OSPEED_HIGH(1) |  \
						   PIN_OSPEED_HIGH(2) |  \
						   PIN_OSPEED_HIGH(3) |  \
						   PIN_OSPEED_HIGH(4) |  \
						   PIN_OSPEED_HIGH(5) |  \
						   PIN_OSPEED_HIGH(6) |  \
						   PIN_OSPEED_HIGH(7) |  \
						   PIN_OSPEED_HIGH(8) |  \
						   PIN_OSPEED_HIGH(9) |  \
						   PIN_OSPEED_HIGH(10) | \
						   PIN_OSPEED_HIGH(11) | \
						   PIN_OSPEED_HIGH(12) | \
						   PIN_OSPEED_HIGH(13) | \
						   PIN_OSPEED_HIGH(14) | \
						   PIN_OSPEED_HIGH(15))
#define VAL_GPIOA_PUPDR (PIN_PUPDR_PULLDOWN(0) |  \
						 PIN_PUPDR_PULLDOWN(1) |  \
						 PIN_PUPDR_PULLDOWN(2) |  \
						 PIN_PUPDR_PULLDOWN(3) |  \
						 PIN_PUPDR_PULLDOWN(4) |  \
						 PIN_PUPDR_PULLDOWN(5) |  \
						 PIN_PUPDR_PULLDOWN(6) |  \
						 PIN_PUPDR_PULLDOWN(7) |  \
						 PIN_PUPDR_PULLDOWN(8) |  \
						 PIN_PUPDR_PULLDOWN(9) |  \
						 PIN_PUPDR_PULLDOWN(10) | \
						 PIN_PUPDR_PULLDOWN(11) | \
						 PIN_PUPDR_PULLDOWN(12) | \
						 PIN_PUPDR_PULLDOWN(13) | \
						 PIN_PUPDR_PULLDOWN(14) | \
						 PIN_PUPDR_PULLDOWN(15))
#define VAL_GPIOA_ODR (PIN_ODR_LOW(0) |  \
					   PIN_ODR_LOW(1) |  \
					   PIN_ODR_LOW(2) |  \
					   PIN_ODR_LOW(3) |  \
					   PIN_ODR_LOW(4) |  \
					   PIN_ODR_LOW(5) |  \
					   PIN_ODR_LOW(6) |  \
					   PIN_ODR_LOW(7) |  \
					   PIN_ODR_LOW(8) |  \
					   PIN_ODR_LOW(9) |  \
					   PIN_ODR_LOW(10) | \
					   PIN_ODR_LOW(11) | \
					   PIN_ODR_LOW(12) | \
					   PIN_ODR_LOW(13) | \
					   PIN_ODR_LOW(14) | \
					   PIN_ODR_LOW(15))
#define VAL_GPIOA_AFRL (PIN_AFIO_AF(0, 0U) | \
						PIN_AFIO_AF(1, 0U) | \
						PIN_AFIO_AF(2, 0U) | \
						PIN_AFIO_AF(3, 0U) | \
						PIN_AFIO_AF(4, 0U) | \
						PIN_AFIO_AF(5, 0U) | \
						PIN_AFIO_AF(6, 0U) | \
						PIN_AFIO_AF(7, 0U))

#define VAL_GPIOA_AFRH (PIN_AFIO_AF(8, 0U) |  \
						PIN_AFIO_AF(9, 0U) |  \
						PIN_AFIO_AF(10, 0U) | \
						PIN_AFIO_AF(11, 0U) | \
						PIN_AFIO_AF(12, 0U) | \
						PIN_AFIO_AF(13, 0U) | \
						PIN_AFIO_AF(14, 0U) | \
						PIN_AFIO_AF(15, 0U))

/*
 * GPIOB setup:
 */
#define VAL_GPIOB_MODER (PIN_MODE_INPUT(0) |     \
						 PIN_MODE_INPUT(1) |     \
						 PIN_MODE_INPUT(2) |     \
						 PIN_MODE_INPUT(3) |     \
						 PIN_MODE_INPUT(4) |     \
						 PIN_MODE_ALTERNATE(5) | \
						 PIN_MODE_ALTERNATE(6) | \
						 PIN_MODE_INPUT(7) |     \
						 PIN_MODE_INPUT(8) |     \
						 PIN_MODE_INPUT(9) |     \
						 PIN_MODE_INPUT(10) |    \
						 PIN_MODE_INPUT(11) |    \
						 PIN_MODE_INPUT(12) |    \
						 PIN_MODE_INPUT(13) |    \
						 PIN_MODE_INPUT(14) |    \
						 PIN_MODE_INPUT(15))
#define VAL_GPIOB_OTYPER (PIN_OTYPE_PUSHPULL(0) |  \
						  PIN_OTYPE_PUSHPULL(1) |  \
						  PIN_OTYPE_PUSHPULL(2) |  \
						  PIN_OTYPE_PUSHPULL(3) |  \
						  PIN_OTYPE_PUSHPULL(4) |  \
						  PIN_OTYPE_PUSHPULL(5) |  \
						  PIN_OTYPE_PUSHPULL(6) |  \
						  PIN_OTYPE_PUSHPULL(7) |  \
						  PIN_OTYPE_PUSHPULL(8) |  \
						  PIN_OTYPE_PUSHPULL(9) |  \
						  PIN_OTYPE_PUSHPULL(10) | \
						  PIN_OTYPE_PUSHPULL(11) | \
						  PIN_OTYPE_PUSHPULL(12) | \
						  PIN_OTYPE_PUSHPULL(13) | \
						  PIN_OTYPE_PUSHPULL(14) | \
						  PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOB_OSPEEDR (PIN_OSPEED_HIGH(0) |  \
						   PIN_OSPEED_HIGH(1) |  \
						   PIN_OSPEED_HIGH(2) |  \
						   PIN_OSPEED_HIGH(3) |  \
						   PIN_OSPEED_HIGH(4) |  \
						   PIN_OSPEED_HIGH(5) |  \
						   PIN_OSPEED_HIGH(6) |  \
						   PIN_OSPEED_HIGH(7) |  \
						   PIN_OSPEED_HIGH(8) |  \
						   PIN_OSPEED_HIGH(9) |  \
						   PIN_OSPEED_HIGH(10) | \
						   PIN_OSPEED_HIGH(11) | \
						   PIN_OSPEED_HIGH(12) | \
						   PIN_OSPEED_HIGH(13) | \
						   PIN_OSPEED_HIGH(14) | \
						   PIN_OSPEED_HIGH(15))
#define VAL_GPIOB_PUPDR (PIN_PUPDR_PULLDOWN(0) |  \
						 PIN_PUPDR_PULLDOWN(1) |  \
						 PIN_PUPDR_PULLDOWN(2) |  \
						 PIN_PUPDR_PULLDOWN(3) |  \
						 PIN_PUPDR_PULLDOWN(4) |  \
						 PIN_PUPDR_PULLDOWN(5) |  \
						 PIN_PUPDR_PULLDOWN(6) |  \
						 PIN_PUPDR_PULLDOWN(7) |  \
						 PIN_PUPDR_PULLDOWN(8) |  \
						 PIN_PUPDR_PULLDOWN(9) |  \
						 PIN_PUPDR_PULLDOWN(10) | \
						 PIN_PUPDR_PULLDOWN(11) | \
						 PIN_PUPDR_PULLDOWN(12) | \
						 PIN_PUPDR_PULLDOWN(13) | \
						 PIN_PUPDR_PULLDOWN(14) | \
						 PIN_PUPDR_PULLDOWN(15))
#define VAL_GPIOB_ODR (PIN_ODR_LOW(0) |  \
					   PIN_ODR_LOW(1) |  \
					   PIN_ODR_LOW(2) |  \
					   PIN_ODR_LOW(3) |  \
					   PIN_ODR_LOW(4) |  \
					   PIN_ODR_LOW(5) |  \
					   PIN_ODR_LOW(6) |  \
					   PIN_ODR_LOW(7) |  \
					   PIN_ODR_LOW(8) |  \
					   PIN_ODR_LOW(9) |  \
					   PIN_ODR_LOW(10) | \
					   PIN_ODR_LOW(11) | \
					   PIN_ODR_LOW(12) | \
					   PIN_ODR_LOW(13) | \
					   PIN_ODR_LOW(14) | \
					   PIN_ODR_LOW(15))
#define VAL_GPIOB_AFRL (PIN_AFIO_AF(0, 0U) | \
						PIN_AFIO_AF(1, 0U) | \
						PIN_AFIO_AF(2, 0U) | \
						PIN_AFIO_AF(3, 0U) | \
						PIN_AFIO_AF(4, 0U) | \
						PIN_AFIO_AF(5, 9U) | \
						PIN_AFIO_AF(6, 9U) | \
						PIN_AFIO_AF(7, 0U))
#define VAL_GPIOB_AFRH (PIN_AFIO_AF(8, 0U) |  \
						PIN_AFIO_AF(9, 0U) |  \
						PIN_AFIO_AF(10, 0U) | \
						PIN_AFIO_AF(11, 0U) | \
						PIN_AFIO_AF(12, 0U) | \
						PIN_AFIO_AF(13, 0U) | \
						PIN_AFIO_AF(14, 0U) | \
						PIN_AFIO_AF(15, 0U))

/*
 * GPIOC setup:
 */
#define VAL_GPIOC_MODER (PIN_MODE_INPUT(0) |      \
						 PIN_MODE_INPUT(1) |      \
						 PIN_MODE_INPUT(2) |      \
						 PIN_MODE_INPUT(3) |      \
						 PIN_MODE_INPUT(4) |      \
						 PIN_MODE_INPUT(5) |      \
						 PIN_MODE_INPUT(6) |      \
						 PIN_MODE_INPUT(7) |      \
						 PIN_MODE_ALTERNATE(8) |  \
						 PIN_MODE_ALTERNATE(9) |  \
						 PIN_MODE_ALTERNATE(10) | \
						 PIN_MODE_ALTERNATE(11) | \
						 PIN_MODE_ALTERNATE(12) | \
						 PIN_MODE_INPUT(13) |     \
						 PIN_MODE_INPUT(14) |     \
						 PIN_MODE_INPUT(15))
#define VAL_GPIOC_OTYPER (PIN_OTYPE_PUSHPULL(0) |  \
						  PIN_OTYPE_PUSHPULL(1) |  \
						  PIN_OTYPE_PUSHPULL(2) |  \
						  PIN_OTYPE_PUSHPULL(3) |  \
						  PIN_OTYPE_PUSHPULL(4) |  \
						  PIN_OTYPE_PUSHPULL(5) |  \
						  PIN_OTYPE_PUSHPULL(6) |  \
						  PIN_OTYPE_PUSHPULL(7) |  \
						  PIN_OTYPE_PUSHPULL(8) |  \
						  PIN_OTYPE_PUSHPULL(9) |  \
						  PIN_OTYPE_PUSHPULL(10) | \
						  PIN_OTYPE_PUSHPULL(11) | \
						  PIN_OTYPE_PUSHPULL(12) | \
						  PIN_OTYPE_PUSHPULL(13) | \
						  PIN_OTYPE_PUSHPULL(14) | \
						  PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOC_OSPEEDR (PIN_OSPEED_HIGH(0) |  \
						   PIN_OSPEED_HIGH(1) |  \
						   PIN_OSPEED_HIGH(2) |  \
						   PIN_OSPEED_HIGH(3) |  \
						   PIN_OSPEED_HIGH(4) |  \
						   PIN_OSPEED_HIGH(5) |  \
						   PIN_OSPEED_HIGH(6) |  \
						   PIN_OSPEED_HIGH(7) |  \
						   PIN_OSPEED_HIGH(8) |  \
						   PIN_OSPEED_HIGH(9) |  \
						   PIN_OSPEED_HIGH(10) | \
						   PIN_OSPEED_HIGH(11) | \
						   PIN_OSPEED_HIGH(12) | \
						   PIN_OSPEED_HIGH(13) | \
						   PIN_OSPEED_HIGH(14) | \
						   PIN_OSPEED_HIGH(15))
#define VAL_GPIOC_PUPDR (PIN_PUPDR_PULLDOWN(0) |  \
						 PIN_PUPDR_PULLDOWN(1) |  \
						 PIN_PUPDR_PULLDOWN(2) |  \
						 PIN_PUPDR_PULLDOWN(3) |  \
						 PIN_PUPDR_PULLDOWN(4) |  \
						 PIN_PUPDR_PULLDOWN(5) |  \
						 PIN_PUPDR_PULLDOWN(6) |  \
						 PIN_PUPDR_PULLDOWN(7) |  \
						 PIN_PUPDR_FLOATING(8) |  \
						 PIN_PUPDR_FLOATING(9) |  \
						 PIN_PUPDR_FLOATING(10) | \
						 PIN_PUPDR_FLOATING(11) | \
						 PIN_PUPDR_FLOATING(12) | \
						 PIN_PUPDR_PULLDOWN(13) | \
						 PIN_PUPDR_PULLDOWN(14) | \
						 PIN_PUPDR_PULLDOWN(15))
#define VAL_GPIOC_ODR (PIN_ODR_LOW(0) |  \
					   PIN_ODR_LOW(1) |  \
					   PIN_ODR_LOW(2) |  \
					   PIN_ODR_LOW(3) |  \
					   PIN_ODR_LOW(4) |  \
					   PIN_ODR_LOW(5) |  \
					   PIN_ODR_LOW(6) |  \
					   PIN_ODR_LOW(7) |  \
					   PIN_ODR_LOW(8) |  \
					   PIN_ODR_LOW(9) |  \
					   PIN_ODR_LOW(10) | \
					   PIN_ODR_LOW(11) | \
					   PIN_ODR_LOW(12) | \
					   PIN_ODR_LOW(13) | \
					   PIN_ODR_LOW(14) | \
					   PIN_ODR_LOW(15))
#define VAL_GPIOC_AFRL (PIN_AFIO_AF(0, 0U) | \
						PIN_AFIO_AF(1, 0U) | \
						PIN_AFIO_AF(2, 0U) | \
						PIN_AFIO_AF(3, 0U) | \
						PIN_AFIO_AF(4, 0U) | \
						PIN_AFIO_AF(5, 0U) | \
						PIN_AFIO_AF(6, 0U) | \
						PIN_AFIO_AF(7, 0U))

#define VAL_GPIOC_AFRH (PIN_AFIO_AF(8, 12U) |  \
						PIN_AFIO_AF(9, 12U) |  \
						PIN_AFIO_AF(10, 12U) | \
						PIN_AFIO_AF(11, 12U) | \
						PIN_AFIO_AF(12, 12U) | \
						PIN_AFIO_AF(13, 0U) |  \
						PIN_AFIO_AF(14, 0U) |  \
						PIN_AFIO_AF(15, 0U))

/*
 * GPIOD setup:
 */

#define VAL_GPIOD_MODER (PIN_MODE_ALTERNATE(0) | \
						 PIN_MODE_ALTERNATE(1) | \
						 PIN_MODE_ALTERNATE(2) | \
						 PIN_MODE_INPUT(3) |     \
						 PIN_MODE_INPUT(4) |     \
						 PIN_MODE_INPUT(5) |     \
						 PIN_MODE_INPUT(6) |     \
						 PIN_MODE_INPUT(7) |     \
						 PIN_MODE_INPUT(8) |     \
						 PIN_MODE_INPUT(9) |     \
						 PIN_MODE_INPUT(10) |    \
						 PIN_MODE_INPUT(11) |    \
						 PIN_MODE_INPUT(12) |    \
						 PIN_MODE_INPUT(13) |    \
						 PIN_MODE_INPUT(14) |    \
						 PIN_MODE_INPUT(15))
#define VAL_GPIOD_OTYPER (PIN_OTYPE_PUSHPULL(0) |  \
						  PIN_OTYPE_PUSHPULL(1) |  \
						  PIN_OTYPE_PUSHPULL(2) |  \
						  PIN_OTYPE_PUSHPULL(3) |  \
						  PIN_OTYPE_PUSHPULL(4) |  \
						  PIN_OTYPE_PUSHPULL(5) |  \
						  PIN_OTYPE_PUSHPULL(6) |  \
						  PIN_OTYPE_PUSHPULL(7) |  \
						  PIN_OTYPE_PUSHPULL(8) |  \
						  PIN_OTYPE_PUSHPULL(9) |  \
						  PIN_OTYPE_PUSHPULL(10) | \
						  PIN_OTYPE_PUSHPULL(11) | \
						  PIN_OTYPE_PUSHPULL(12) | \
						  PIN_OTYPE_PUSHPULL(13) | \
						  PIN_OTYPE_PUSHPULL(14) | \
						  PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOD_OSPEEDR (PIN_OSPEED_HIGH(0) |  \
						   PIN_OSPEED_HIGH(1) |  \
						   PIN_OSPEED_HIGH(2) |  \
						   PIN_OSPEED_HIGH(3) |  \
						   PIN_OSPEED_HIGH(4) |  \
						   PIN_OSPEED_HIGH(5) |  \
						   PIN_OSPEED_HIGH(6) |  \
						   PIN_OSPEED_HIGH(7) |  \
						   PIN_OSPEED_HIGH(8) |  \
						   PIN_OSPEED_HIGH(9) |  \
						   PIN_OSPEED_HIGH(10) | \
						   PIN_OSPEED_HIGH(11) | \
						   PIN_OSPEED_HIGH(12) | \
						   PIN_OSPEED_HIGH(13) | \
						   PIN_OSPEED_HIGH(14) | \
						   PIN_OSPEED_HIGH(15))
#define VAL_GPIOD_PUPDR (PIN_PUPDR_PULLDOWN(0) |  \
						 PIN_PUPDR_PULLDOWN(1) |  \
						 PIN_PUPDR_PULLDOWN(2) |  \
						 PIN_PUPDR_PULLDOWN(3) |  \
						 PIN_PUPDR_PULLDOWN(4) |  \
						 PIN_PUPDR_PULLDOWN(5) |  \
						 PIN_PUPDR_PULLDOWN(6) |  \
						 PIN_PUPDR_PULLDOWN(7) |  \
						 PIN_PUPDR_PULLDOWN(8) |  \
						 PIN_PUPDR_PULLDOWN(9) |  \
						 PIN_PUPDR_PULLDOWN(10) | \
						 PIN_PUPDR_PULLDOWN(11) | \
						 PIN_PUPDR_PULLDOWN(12) | \
						 PIN_PUPDR_PULLDOWN(13) | \
						 PIN_PUPDR_PULLDOWN(14) | \
						 PIN_PUPDR_PULLDOWN(15))
#define VAL_GPIOD_ODR (PIN_ODR_LOW(0) |  \
					   PIN_ODR_LOW(1) |  \
					   PIN_ODR_LOW(2) |  \
					   PIN_ODR_LOW(3) |  \
					   PIN_ODR_LOW(4) |  \
					   PIN_ODR_LOW(5) |  \
					   PIN_ODR_LOW(6) |  \
					   PIN_ODR_LOW(7) |  \
					   PIN_ODR_LOW(8) |  \
					   PIN_ODR_LOW(9) |  \
					   PIN_ODR_LOW(10) | \
					   PIN_ODR_LOW(11) | \
					   PIN_ODR_LOW(12) | \
					   PIN_ODR_LOW(13) | \
					   PIN_ODR_LOW(14) | \
					   PIN_ODR_LOW(15))
#define VAL_GPIOD_AFRL (PIN_AFIO_AF(0, 9U) |  \
						PIN_AFIO_AF(1, 9U) |  \
						PIN_AFIO_AF(2, 12U) | \
						PIN_AFIO_AF(3, 0U) |  \
						PIN_AFIO_AF(4, 0U) |  \
						PIN_AFIO_AF(5, 0U) |  \
						PIN_AFIO_AF(6, 0U) |  \
						PIN_AFIO_AF(7, 0U))
#define VAL_GPIOD_AFRH (PIN_AFIO_AF(8, 0U) |  \
						PIN_AFIO_AF(9, 0U) |  \
						PIN_AFIO_AF(10, 0U) | \
						PIN_AFIO_AF(11, 0U) | \
						PIN_AFIO_AF(12, 0U) | \
						PIN_AFIO_AF(13, 0U) | \
						PIN_AFIO_AF(14, 0U) | \
						PIN_AFIO_AF(15, 0U))

/*
 * GPIOE setup:
 */
#define VAL_GPIOE_MODER (PIN_MODE_INPUT(0) |  \
						 PIN_MODE_INPUT(1) |  \
						 PIN_MODE_INPUT(2) |  \
						 PIN_MODE_INPUT(3) |  \
						 PIN_MODE_INPUT(4) |  \
						 PIN_MODE_INPUT(5) |  \
						 PIN_MODE_INPUT(6) |  \
						 PIN_MODE_INPUT(7) |  \
						 PIN_MODE_INPUT(8) |  \
						 PIN_MODE_INPUT(9) |  \
						 PIN_MODE_INPUT(10) | \
						 PIN_MODE_INPUT(11) | \
						 PIN_MODE_INPUT(12) | \
						 PIN_MODE_INPUT(13) | \
						 PIN_MODE_INPUT(14) | \
						 PIN_MODE_INPUT(15))
#define VAL_GPIOE_OTYPER (PIN_OTYPE_PUSHPULL(0) |  \
						  PIN_OTYPE_PUSHPULL(1) |  \
						  PIN_OTYPE_PUSHPULL(2) |  \
						  PIN_OTYPE_PUSHPULL(3) |  \
						  PIN_OTYPE_PUSHPULL(4) |  \
						  PIN_OTYPE_PUSHPULL(5) |  \
						  PIN_OTYPE_PUSHPULL(6) |  \
						  PIN_OTYPE_PUSHPULL(7) |  \
						  PIN_OTYPE_PUSHPULL(8) |  \
						  PIN_OTYPE_PUSHPULL(9) |  \
						  PIN_OTYPE_PUSHPULL(10) | \
						  PIN_OTYPE_PUSHPULL(11) | \
						  PIN_OTYPE_PUSHPULL(12) | \
						  PIN_OTYPE_PUSHPULL(13) | \
						  PIN_OTYPE_PUSHPULL(14) | \
						  PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOE_OSPEEDR (PIN_OSPEED_HIGH(0) |  \
						   PIN_OSPEED_HIGH(1) |  \
						   PIN_OSPEED_HIGH(2) |  \
						   PIN_OSPEED_HIGH(3) |  \
						   PIN_OSPEED_HIGH(4) |  \
						   PIN_OSPEED_HIGH(5) |  \
						   PIN_OSPEED_HIGH(6) |  \
						   PIN_OSPEED_HIGH(7) |  \
						   PIN_OSPEED_HIGH(8) |  \
						   PIN_OSPEED_HIGH(9) |  \
						   PIN_OSPEED_HIGH(10) | \
						   PIN_OSPEED_HIGH(11) | \
						   PIN_OSPEED_HIGH(12) | \
						   PIN_OSPEED_HIGH(13) | \
						   PIN_OSPEED_HIGH(14) | \
						   PIN_OSPEED_HIGH(15))
#define VAL_GPIOE_PUPDR (PIN_PUPDR_PULLDOWN(0) |  \
						 PIN_PUPDR_PULLDOWN(1) |  \
						 PIN_PUPDR_PULLDOWN(2) |  \
						 PIN_PUPDR_PULLDOWN(3) |  \
						 PIN_PUPDR_PULLDOWN(4) |  \
						 PIN_PUPDR_PULLDOWN(5) |  \
						 PIN_PUPDR_PULLDOWN(6) |  \
						 PIN_PUPDR_PULLDOWN(7) |  \
						 PIN_PUPDR_PULLDOWN(8) |  \
						 PIN_PUPDR_PULLDOWN(9) |  \
						 PIN_PUPDR_PULLDOWN(10) | \
						 PIN_PUPDR_PULLDOWN(11) | \
						 PIN_PUPDR_PULLDOWN(12) | \
						 PIN_PUPDR_PULLDOWN(13) | \
						 PIN_PUPDR_PULLDOWN(14) | \
						 PIN_PUPDR_PULLDOWN(15))
#define VAL_GPIOE_ODR (PIN_ODR_LOW(0) |  \
					   PIN_ODR_LOW(1) |  \
					   PIN_ODR_LOW(2) |  \
					   PIN_ODR_LOW(3) |  \
					   PIN_ODR_LOW(4) |  \
					   PIN_ODR_LOW(5) |  \
					   PIN_ODR_LOW(6) |  \
					   PIN_ODR_LOW(7) |  \
					   PIN_ODR_LOW(8) |  \
					   PIN_ODR_LOW(9) |  \
					   PIN_ODR_LOW(10) | \
					   PIN_ODR_LOW(11) | \
					   PIN_ODR_LOW(12) | \
					   PIN_ODR_LOW(13) | \
					   PIN_ODR_LOW(14) | \
					   PIN_ODR_LOW(15))
#define VAL_GPIOE_AFRL (PIN_AFIO_AF(0, 0U) | \
						PIN_AFIO_AF(1, 0U) | \
						PIN_AFIO_AF(2, 0U) | \
						PIN_AFIO_AF(3, 0U) | \
						PIN_AFIO_AF(4, 0U) | \
						PIN_AFIO_AF(5, 0U) | \
						PIN_AFIO_AF(6, 0U) | \
						PIN_AFIO_AF(7, 0U))

#define VAL_GPIOE_AFRH (PIN_AFIO_AF(8, 0U) |  \
						PIN_AFIO_AF(9, 0U) |  \
						PIN_AFIO_AF(10, 0U) | \
						PIN_AFIO_AF(11, 0U) | \
						PIN_AFIO_AF(12, 0U) | \
						PIN_AFIO_AF(13, 0U) | \
						PIN_AFIO_AF(14, 0U) | \
						PIN_AFIO_AF(15, 0U))

/*
 * GPIOF setup:
 */
#define VAL_GPIOF_MODER (PIN_MODE_INPUT(0) |  \
						 PIN_MODE_INPUT(1) |  \
						 PIN_MODE_INPUT(2) |  \
						 PIN_MODE_INPUT(3) |  \
						 PIN_MODE_INPUT(4) |  \
						 PIN_MODE_INPUT(5) |  \
						 PIN_MODE_INPUT(6) |  \
						 PIN_MODE_INPUT(7) |  \
						 PIN_MODE_INPUT(8) |  \
						 PIN_MODE_INPUT(9) |  \
						 PIN_MODE_INPUT(10) | \
						 PIN_MODE_INPUT(11) | \
						 PIN_MODE_INPUT(12) | \
						 PIN_MODE_INPUT(13) | \
						 PIN_MODE_INPUT(14) | \
						 PIN_MODE_INPUT(15))
#define VAL_GPIOF_OTYPER (PIN_OTYPE_PUSHPULL(0) |  \
						  PIN_OTYPE_PUSHPULL(1) |  \
						  PIN_OTYPE_PUSHPULL(2) |  \
						  PIN_OTYPE_PUSHPULL(3) |  \
						  PIN_OTYPE_PUSHPULL(4) |  \
						  PIN_OTYPE_PUSHPULL(5) |  \
						  PIN_OTYPE_PUSHPULL(6) |  \
						  PIN_OTYPE_PUSHPULL(7) |  \
						  PIN_OTYPE_PUSHPULL(8) |  \
						  PIN_OTYPE_PUSHPULL(9) |  \
						  PIN_OTYPE_PUSHPULL(10) | \
						  PIN_OTYPE_PUSHPULL(11) | \
						  PIN_OTYPE_PUSHPULL(12) | \
						  PIN_OTYPE_PUSHPULL(13) | \
						  PIN_OTYPE_PUSHPULL(14) | \
						  PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOF_OSPEEDR (PIN_OSPEED_HIGH(0) |  \
						   PIN_OSPEED_HIGH(1) |  \
						   PIN_OSPEED_HIGH(2) |  \
						   PIN_OSPEED_HIGH(3) |  \
						   PIN_OSPEED_HIGH(4) |  \
						   PIN_OSPEED_HIGH(5) |  \
						   PIN_OSPEED_HIGH(6) |  \
						   PIN_OSPEED_HIGH(7) |  \
						   PIN_OSPEED_HIGH(8) |  \
						   PIN_OSPEED_HIGH(9) |  \
						   PIN_OSPEED_HIGH(10) | \
						   PIN_OSPEED_HIGH(11) | \
						   PIN_OSPEED_HIGH(12) | \
						   PIN_OSPEED_HIGH(13) | \
						   PIN_OSPEED_HIGH(14) | \
						   PIN_OSPEED_HIGH(15))
#define VAL_GPIOF_PUPDR (PIN_PUPDR_PULLDOWN(0) |  \
						 PIN_PUPDR_PULLDOWN(1) |  \
						 PIN_PUPDR_PULLDOWN(2) |  \
						 PIN_PUPDR_PULLDOWN(3) |  \
						 PIN_PUPDR_PULLDOWN(4) |  \
						 PIN_PUPDR_PULLDOWN(5) |  \
						 PIN_PUPDR_PULLDOWN(6) |  \
						 PIN_PUPDR_PULLDOWN(7) |  \
						 PIN_PUPDR_PULLDOWN(8) |  \
						 PIN_PUPDR_PULLDOWN(9) |  \
						 PIN_PUPDR_PULLDOWN(10) | \
						 PIN_PUPDR_PULLDOWN(11) | \
						 PIN_PUPDR_PULLDOWN(12) | \
						 PIN_PUPDR_PULLDOWN(13) | \
						 PIN_PUPDR_PULLDOWN(14) | \
						 PIN_PUPDR_PULLDOWN(15))
#define VAL_GPIOF_ODR (PIN_ODR_LOW(0) |  \
					   PIN_ODR_LOW(1) |  \
					   PIN_ODR_LOW(2) |  \
					   PIN_ODR_LOW(3) |  \
					   PIN_ODR_LOW(4) |  \
					   PIN_ODR_LOW(5) |  \
					   PIN_ODR_LOW(6) |  \
					   PIN_ODR_LOW(7) |  \
					   PIN_ODR_LOW(8) |  \
					   PIN_ODR_LOW(9) |  \
					   PIN_ODR_LOW(10) | \
					   PIN_ODR_LOW(11) | \
					   PIN_ODR_LOW(12) | \
					   PIN_ODR_LOW(13) | \
					   PIN_ODR_LOW(14) | \
					   PIN_ODR_LOW(15))
#define VAL_GPIOF_AFRL (PIN_AFIO_AF(0, 0U) | \
						PIN_AFIO_AF(1, 0U) | \
						PIN_AFIO_AF(2, 0U) | \
						PIN_AFIO_AF(3, 0U) | \
						PIN_AFIO_AF(4, 0U) | \
						PIN_AFIO_AF(5, 0U) | \
						PIN_AFIO_AF(6, 0U) | \
						PIN_AFIO_AF(7, 0U))

#define VAL_GPIOF_AFRH (PIN_AFIO_AF(8, 0U) |  \
						PIN_AFIO_AF(9, 0U) |  \
						PIN_AFIO_AF(10, 0U) | \
						PIN_AFIO_AF(11, 0U) | \
						PIN_AFIO_AF(12, 0U) | \
						PIN_AFIO_AF(13, 0U) | \
						PIN_AFIO_AF(14, 0U) | \
						PIN_AFIO_AF(15, 0U))

/*
 * GPIOG setup:
 */
#define VAL_GPIOG_MODER (PIN_MODE_INPUT(0) |  \
						 PIN_MODE_INPUT(1) |  \
						 PIN_MODE_INPUT(2) |  \
						 PIN_MODE_INPUT(3) |  \
						 PIN_MODE_INPUT(4) |  \
						 PIN_MODE_INPUT(5) |  \
						 PIN_MODE_INPUT(6) |  \
						 PIN_MODE_INPUT(7) |  \
						 PIN_MODE_INPUT(8) |  \
						 PIN_MODE_INPUT(9) |  \
						 PIN_MODE_INPUT(10) | \
						 PIN_MODE_INPUT(11) | \
						 PIN_MODE_INPUT(12) | \
						 PIN_MODE_INPUT(13) | \
						 PIN_MODE_INPUT(14) | \
						 PIN_MODE_INPUT(15))
#define VAL_GPIOG_OTYPER (PIN_OTYPE_PUSHPULL(0) |  \
						  PIN_OTYPE_PUSHPULL(1) |  \
						  PIN_OTYPE_PUSHPULL(2) |  \
						  PIN_OTYPE_PUSHPULL(3) |  \
						  PIN_OTYPE_PUSHPULL(4) |  \
						  PIN_OTYPE_PUSHPULL(5) |  \
						  PIN_OTYPE_PUSHPULL(6) |  \
						  PIN_OTYPE_PUSHPULL(7) |  \
						  PIN_OTYPE_PUSHPULL(8) |  \
						  PIN_OTYPE_PUSHPULL(9) |  \
						  PIN_OTYPE_PUSHPULL(10) | \
						  PIN_OTYPE_PUSHPULL(11) | \
						  PIN_OTYPE_PUSHPULL(12) | \
						  PIN_OTYPE_PUSHPULL(13) | \
						  PIN_OTYPE_PUSHPULL(14) | \
						  PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOG_OSPEEDR (PIN_OSPEED_HIGH(0) |  \
						   PIN_OSPEED_HIGH(1) |  \
						   PIN_OSPEED_HIGH(2) |  \
						   PIN_OSPEED_HIGH(3) |  \
						   PIN_OSPEED_HIGH(4) |  \
						   PIN_OSPEED_HIGH(5) |  \
						   PIN_OSPEED_HIGH(6) |  \
						   PIN_OSPEED_HIGH(7) |  \
						   PIN_OSPEED_HIGH(8) |  \
						   PIN_OSPEED_HIGH(9) |  \
						   PIN_OSPEED_HIGH(10) | \
						   PIN_OSPEED_HIGH(11) | \
						   PIN_OSPEED_HIGH(12) | \
						   PIN_OSPEED_HIGH(13) | \
						   PIN_OSPEED_HIGH(14) | \
						   PIN_OSPEED_HIGH(15))
#define VAL_GPIOG_PUPDR (PIN_PUPDR_PULLDOWN(0) |  \
						 PIN_PUPDR_PULLDOWN(1) |  \
						 PIN_PUPDR_PULLDOWN(2) |  \
						 PIN_PUPDR_PULLDOWN(3) |  \
						 PIN_PUPDR_PULLDOWN(4) |  \
						 PIN_PUPDR_PULLDOWN(5) |  \
						 PIN_PUPDR_PULLDOWN(6) |  \
						 PIN_PUPDR_PULLDOWN(7) |  \
						 PIN_PUPDR_PULLDOWN(8) |  \
						 PIN_PUPDR_PULLDOWN(9) |  \
						 PIN_PUPDR_PULLDOWN(10) | \
						 PIN_PUPDR_PULLDOWN(11) | \
						 PIN_PUPDR_PULLDOWN(12) | \
						 PIN_PUPDR_PULLDOWN(13) | \
						 PIN_PUPDR_PULLDOWN(14) | \
						 PIN_PUPDR_PULLDOWN(15))
#define VAL_GPIOG_ODR (PIN_ODR_LOW(0) |  \
					   PIN_ODR_LOW(1) |  \
					   PIN_ODR_LOW(2) |  \
					   PIN_ODR_LOW(3) |  \
					   PIN_ODR_LOW(4) |  \
					   PIN_ODR_LOW(5) |  \
					   PIN_ODR_LOW(6) |  \
					   PIN_ODR_LOW(7) |  \
					   PIN_ODR_LOW(8) |  \
					   PIN_ODR_LOW(9) |  \
					   PIN_ODR_LOW(10) | \
					   PIN_ODR_LOW(11) | \
					   PIN_ODR_LOW(12) | \
					   PIN_ODR_LOW(13) | \
					   PIN_ODR_LOW(14) | \
					   PIN_ODR_LOW(15))
#define VAL_GPIOG_AFRL (PIN_AFIO_AF(0, 0U) | \
						PIN_AFIO_AF(1, 0U) | \
						PIN_AFIO_AF(2, 0U) | \
						PIN_AFIO_AF(3, 0U) | \
						PIN_AFIO_AF(4, 0U) | \
						PIN_AFIO_AF(5, 0U) | \
						PIN_AFIO_AF(6, 0U) | \
						PIN_AFIO_AF(7, 0U))

#define VAL_GPIOG_AFRH (PIN_AFIO_AF(8, 0U) |  \
						PIN_AFIO_AF(9, 0U) |  \
						PIN_AFIO_AF(10, 0U) | \
						PIN_AFIO_AF(11, 0U) | \
						PIN_AFIO_AF(12, 0U) | \
						PIN_AFIO_AF(13, 0U) | \
						PIN_AFIO_AF(14, 0U) | \
						PIN_AFIO_AF(15, 0U))

/*
 * GPIOH setup:
 */
#define VAL_GPIOH_MODER (PIN_MODE_INPUT(0) |  \
						 PIN_MODE_INPUT(1) |  \
						 PIN_MODE_INPUT(2) |  \
						 PIN_MODE_INPUT(3) |  \
						 PIN_MODE_INPUT(4) |  \
						 PIN_MODE_INPUT(5) |  \
						 PIN_MODE_INPUT(6) |  \
						 PIN_MODE_INPUT(7) |  \
						 PIN_MODE_INPUT(8) |  \
						 PIN_MODE_INPUT(9) |  \
						 PIN_MODE_INPUT(10) | \
						 PIN_MODE_INPUT(11) | \
						 PIN_MODE_INPUT(12) | \
						 PIN_MODE_INPUT(13) | \
						 PIN_MODE_INPUT(14) | \
						 PIN_MODE_INPUT(15))
#define VAL_GPIOH_OTYPER (PIN_OTYPE_PUSHPULL(0) |  \
						  PIN_OTYPE_PUSHPULL(1) |  \
						  PIN_OTYPE_PUSHPULL(2) |  \
						  PIN_OTYPE_PUSHPULL(3) |  \
						  PIN_OTYPE_PUSHPULL(4) |  \
						  PIN_OTYPE_PUSHPULL(5) |  \
						  PIN_OTYPE_PUSHPULL(6) |  \
						  PIN_OTYPE_PUSHPULL(7) |  \
						  PIN_OTYPE_PUSHPULL(8) |  \
						  PIN_OTYPE_PUSHPULL(9) |  \
						  PIN_OTYPE_PUSHPULL(10) | \
						  PIN_OTYPE_PUSHPULL(11) | \
						  PIN_OTYPE_PUSHPULL(12) | \
						  PIN_OTYPE_PUSHPULL(13) | \
						  PIN_OTYPE_PUSHPULL(14) | \
						  PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOH_OSPEEDR (PIN_OSPEED_HIGH(0) |  \
						   PIN_OSPEED_HIGH(1) |  \
						   PIN_OSPEED_HIGH(2) |  \
						   PIN_OSPEED_HIGH(3) |  \
						   PIN_OSPEED_HIGH(4) |  \
						   PIN_OSPEED_HIGH(5) |  \
						   PIN_OSPEED_HIGH(6) |  \
						   PIN_OSPEED_HIGH(7) |  \
						   PIN_OSPEED_HIGH(8) |  \
						   PIN_OSPEED_HIGH(9) |  \
						   PIN_OSPEED_HIGH(10) | \
						   PIN_OSPEED_HIGH(11) | \
						   PIN_OSPEED_HIGH(12) | \
						   PIN_OSPEED_HIGH(13) | \
						   PIN_OSPEED_HIGH(14) | \
						   PIN_OSPEED_HIGH(15))
#define VAL_GPIOH_PUPDR (PIN_PUPDR_PULLDOWN(0) |  \
						 PIN_PUPDR_PULLDOWN(1) |  \
						 PIN_PUPDR_PULLDOWN(2) |  \
						 PIN_PUPDR_PULLDOWN(3) |  \
						 PIN_PUPDR_PULLDOWN(4) |  \
						 PIN_PUPDR_PULLDOWN(5) |  \
						 PIN_PUPDR_PULLDOWN(6) |  \
						 PIN_PUPDR_PULLDOWN(7) |  \
						 PIN_PUPDR_PULLDOWN(8) |  \
						 PIN_PUPDR_PULLDOWN(9) |  \
						 PIN_PUPDR_PULLDOWN(10) | \
						 PIN_PUPDR_PULLDOWN(11) | \
						 PIN_PUPDR_PULLDOWN(12) | \
						 PIN_PUPDR_PULLDOWN(13) | \
						 PIN_PUPDR_PULLDOWN(14) | \
						 PIN_PUPDR_PULLDOWN(15))
#define VAL_GPIOH_ODR (PIN_ODR_LOW(0) |  \
					   PIN_ODR_LOW(1) |  \
					   PIN_ODR_LOW(2) |  \
					   PIN_ODR_LOW(3) |  \
					   PIN_ODR_LOW(4) |  \
					   PIN_ODR_LOW(5) |  \
					   PIN_ODR_LOW(6) |  \
					   PIN_ODR_LOW(7) |  \
					   PIN_ODR_LOW(8) |  \
					   PIN_ODR_LOW(9) |  \
					   PIN_ODR_LOW(10) | \
					   PIN_ODR_LOW(11) | \
					   PIN_ODR_LOW(12) | \
					   PIN_ODR_LOW(13) | \
					   PIN_ODR_LOW(14) | \
					   PIN_ODR_LOW(15))
#define VAL_GPIOH_AFRL (PIN_AFIO_AF(0, 0U) | \
						PIN_AFIO_AF(1, 0U) | \
						PIN_AFIO_AF(2, 0U) | \
						PIN_AFIO_AF(3, 0U) | \
						PIN_AFIO_AF(4, 0U) | \
						PIN_AFIO_AF(5, 0U) | \
						PIN_AFIO_AF(6, 0U) | \
						PIN_AFIO_AF(7, 0U))

#define VAL_GPIOH_AFRH (PIN_AFIO_AF(8, 0U) |  \
						PIN_AFIO_AF(9, 0U) |  \
						PIN_AFIO_AF(10, 0U) | \
						PIN_AFIO_AF(11, 0U) | \
						PIN_AFIO_AF(12, 0U) | \
						PIN_AFIO_AF(13, 0U) | \
						PIN_AFIO_AF(14, 0U) | \
						PIN_AFIO_AF(15, 0U))

/*
 * GPIOI setup:
 */
#define VAL_GPIOI_MODER (PIN_MODE_INPUT(0) |  \
						 PIN_MODE_INPUT(1) |  \
						 PIN_MODE_INPUT(2) |  \
						 PIN_MODE_INPUT(3) |  \
						 PIN_MODE_INPUT(4) |  \
						 PIN_MODE_INPUT(5) |  \
						 PIN_MODE_INPUT(6) |  \
						 PIN_MODE_INPUT(7) |  \
						 PIN_MODE_INPUT(8) |  \
						 PIN_MODE_INPUT(9) |  \
						 PIN_MODE_INPUT(10) | \
						 PIN_MODE_INPUT(11) | \
						 PIN_MODE_INPUT(12) | \
						 PIN_MODE_INPUT(13) | \
						 PIN_MODE_INPUT(14) | \
						 PIN_MODE_INPUT(15))
#define VAL_GPIOI_OTYPER (PIN_OTYPE_PUSHPULL(0) |  \
						  PIN_OTYPE_PUSHPULL(1) |  \
						  PIN_OTYPE_PUSHPULL(2) |  \
						  PIN_OTYPE_PUSHPULL(3) |  \
						  PIN_OTYPE_PUSHPULL(4) |  \
						  PIN_OTYPE_PUSHPULL(5) |  \
						  PIN_OTYPE_PUSHPULL(6) |  \
						  PIN_OTYPE_PUSHPULL(7) |  \
						  PIN_OTYPE_PUSHPULL(8) |  \
						  PIN_OTYPE_PUSHPULL(9) |  \
						  PIN_OTYPE_PUSHPULL(10) | \
						  PIN_OTYPE_PUSHPULL(11) | \
						  PIN_OTYPE_PUSHPULL(12) | \
						  PIN_OTYPE_PUSHPULL(13) | \
						  PIN_OTYPE_PUSHPULL(14) | \
						  PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOI_OSPEEDR (PIN_OSPEED_HIGH(0) |  \
						   PIN_OSPEED_HIGH(1) |  \
						   PIN_OSPEED_HIGH(2) |  \
						   PIN_OSPEED_HIGH(3) |  \
						   PIN_OSPEED_HIGH(4) |  \
						   PIN_OSPEED_HIGH(5) |  \
						   PIN_OSPEED_HIGH(6) |  \
						   PIN_OSPEED_HIGH(7) |  \
						   PIN_OSPEED_HIGH(8) |  \
						   PIN_OSPEED_HIGH(9) |  \
						   PIN_OSPEED_HIGH(10) | \
						   PIN_OSPEED_HIGH(11) | \
						   PIN_OSPEED_HIGH(12) | \
						   PIN_OSPEED_HIGH(13) | \
						   PIN_OSPEED_HIGH(14) | \
						   PIN_OSPEED_HIGH(15))
#define VAL_GPIOI_PUPDR (PIN_PUPDR_PULLDOWN(0) |  \
						 PIN_PUPDR_PULLDOWN(1) |  \
						 PIN_PUPDR_PULLDOWN(2) |  \
						 PIN_PUPDR_PULLDOWN(3) |  \
						 PIN_PUPDR_PULLDOWN(4) |  \
						 PIN_PUPDR_PULLDOWN(5) |  \
						 PIN_PUPDR_PULLDOWN(6) |  \
						 PIN_PUPDR_PULLDOWN(7) |  \
						 PIN_PUPDR_PULLDOWN(8) |  \
						 PIN_PUPDR_PULLDOWN(9) |  \
						 PIN_PUPDR_PULLDOWN(10) | \
						 PIN_PUPDR_PULLDOWN(11) | \
						 PIN_PUPDR_PULLDOWN(12) | \
						 PIN_PUPDR_PULLDOWN(13) | \
						 PIN_PUPDR_PULLDOWN(14) | \
						 PIN_PUPDR_PULLDOWN(15))
#define VAL_GPIOI_ODR (PIN_ODR_LOW(0) |  \
					   PIN_ODR_LOW(1) |  \
					   PIN_ODR_LOW(2) |  \
					   PIN_ODR_LOW(3) |  \
					   PIN_ODR_LOW(4) |  \
					   PIN_ODR_LOW(5) |  \
					   PIN_ODR_LOW(6) |  \
					   PIN_ODR_LOW(7) |  \
					   PIN_ODR_LOW(8) |  \
					   PIN_ODR_LOW(9) |  \
					   PIN_ODR_LOW(10) | \
					   PIN_ODR_LOW(11) | \
					   PIN_ODR_LOW(12) | \
					   PIN_ODR_LOW(13) | \
					   PIN_ODR_LOW(14) | \
					   PIN_ODR_LOW(15))
#define VAL_GPIOI_AFRL (PIN_AFIO_AF(0, 0U) | \
						PIN_AFIO_AF(1, 0U) | \
						PIN_AFIO_AF(2, 0U) | \
						PIN_AFIO_AF(3, 0U) | \
						PIN_AFIO_AF(4, 0U) | \
						PIN_AFIO_AF(5, 0U) | \
						PIN_AFIO_AF(6, 0U) | \
						PIN_AFIO_AF(7, 0U))

#define VAL_GPIOI_AFRH (PIN_AFIO_AF(8, 0U) |  \
						PIN_AFIO_AF(9, 0U) |  \
						PIN_AFIO_AF(10, 0U) | \
						PIN_AFIO_AF(11, 0U) | \
						PIN_AFIO_AF(12, 0U) | \
						PIN_AFIO_AF(13, 0U) | \
						PIN_AFIO_AF(14, 0U) | \
						PIN_AFIO_AF(15, 0U))

#endif /* BOARD_IO_H */