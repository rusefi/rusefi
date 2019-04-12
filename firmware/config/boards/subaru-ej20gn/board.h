/*
	ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
 * This file has been automatically generated using ChibiStudio board
 * generator plugin. Do not edit manually.
 */

#ifndef BOARD_H
#define BOARD_H

/*
 * Board identifier.
 */
#define BOARD_NAME                  "Subaru EJ20G/STM32F765 for RusEFI"

#define BOARD_TLE6240_COUNT         1
#define BOARD_MC33972_COUNT			1

#define BOARD_EXT_GPIOCHIPS			(BOARD_TLE6240_COUNT + BOARD_MC33972_COUNT)

/* additional space for pins on gpioext */
#define BOARD_EXT_PINREPOPINS		(16 + 22)

/*
 * Board oscillators-related settings.
 * NOTE: LSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                32768U
#endif

#define STM32_LSEDRV                (3U << 3U)

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                12000000U
#endif

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   330U

/*
 * MCU type as defined in the ST header.
 * this declaration for stm32_registry.h
 */
#ifndef STM32F765xx
#define STM32F765xx
#endif

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2U))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2U))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2U))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2U))

#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))

#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))

#define PIN_OSPEED_VERYLOW(n)       (0U << ((n) * 2U))
#define PIN_OSPEED_LOW(n)           (1U << ((n) * 2U))
#define PIN_OSPEED_MEDIUM(n)        (2U << ((n) * 2U))
#define PIN_OSPEED_HIGH(n)          (3U << ((n) * 2U))

#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))

#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))

// See https://github.com/rusefi/rusefi/issues/397
#define DEFAULT_GPIO_SPEED			PIN_OSPEED_HIGH

/*
 * GPIOA setup:
 *
 * PA0  -
 * PA1  -
 * PA2  -
 * PA3  -
 * PA4  -
 * PA5  -
 * PA6  -
 * PA7  -
 * PA8  -
 * PA9  - USART1_TX - boot/console - TTL level on XP4.2
 * PA10 - USART1_RX - boot/console - TTL level on XP4.3
 * PA11 - USB_FS_D- - boot/console/TS - XS3
 * PA12 - USB_FS_D+ - boot/console/TS - XS3
 * PA13 -
 * PA14 -
 * PA15 -
 */
#define VAL_GPIOA_MODER     (/*PIN_MODE_ALTERNATE(0) */	PIN_MODE_OUTPUT(0)	| \
							 /*PIN_MODE_ALTERNATE(1) */ PIN_MODE_OUTPUT(1)	| \
							 PIN_MODE_ANALOG(2)		| \
							 PIN_MODE_ANALOG(3)		| \
							 PIN_MODE_ANALOG(4)		| \
							 PIN_MODE_ANALOG(5)		| \
							 PIN_MODE_ANALOG(6)		| \
							 PIN_MODE_ANALOG(7)		| \
							 PIN_MODE_ALTERNATE(8)	| \
							 PIN_MODE_ALTERNATE(9)	| \
							 PIN_MODE_ALTERNATE(10)	| \
							 PIN_MODE_ALTERNATE(11)	| \
							 PIN_MODE_ALTERNATE(12)	| \
							 PIN_MODE_ALTERNATE(13)	| \
							 PIN_MODE_ALTERNATE(14)	| \
							 PIN_MODE_OUTPUT(15))
#define VAL_GPIOA_OTYPER    (PIN_OTYPE_PUSHPULL( 0)	| \
							 PIN_OTYPE_PUSHPULL( 1)	| \
							 PIN_OTYPE_PUSHPULL( 2)	| \
							 PIN_OTYPE_PUSHPULL( 3)	| \
							 PIN_OTYPE_PUSHPULL( 4)	| \
							 PIN_OTYPE_PUSHPULL( 5)	| \
							 PIN_OTYPE_PUSHPULL( 6)	| \
							 PIN_OTYPE_PUSHPULL( 7)	| \
							 PIN_OTYPE_PUSHPULL( 8)	| \
							 PIN_OTYPE_PUSHPULL( 9)	| \
							 PIN_OTYPE_PUSHPULL(10)	| \
							 PIN_OTYPE_PUSHPULL(11)	| \
							 PIN_OTYPE_PUSHPULL(12)	| \
							 PIN_OTYPE_PUSHPULL(13)	| \
							 PIN_OTYPE_PUSHPULL(14)	| \
							 PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOA_OSPEEDR	(DEFAULT_GPIO_SPEED( 0)	| \
							 DEFAULT_GPIO_SPEED( 1)	| \
							 DEFAULT_GPIO_SPEED( 2)	| \
							 DEFAULT_GPIO_SPEED( 3)	| \
							 DEFAULT_GPIO_SPEED( 4)	| \
							 DEFAULT_GPIO_SPEED( 5)	| \
							 DEFAULT_GPIO_SPEED( 6)	| \
							 DEFAULT_GPIO_SPEED( 7)	| \
							 DEFAULT_GPIO_SPEED( 8)	| \
							 DEFAULT_GPIO_SPEED( 9)	| \
							 DEFAULT_GPIO_SPEED(10)	| \
							 DEFAULT_GPIO_SPEED(11)	| \
							 DEFAULT_GPIO_SPEED(12)	| \
							 DEFAULT_GPIO_SPEED(13)	| \
							 DEFAULT_GPIO_SPEED(14)	| \
							 DEFAULT_GPIO_SPEED(15))
#define VAL_GPIOA_PUPDR		(PIN_PUPDR_FLOATING( 0)	| \
							 PIN_PUPDR_FLOATING( 1)	| \
							 PIN_PUPDR_FLOATING( 2)	| \
							 PIN_PUPDR_FLOATING( 3)	| \
							 PIN_PUPDR_FLOATING( 4)	| \
							 PIN_PUPDR_FLOATING( 5)	| \
							 PIN_PUPDR_FLOATING( 6)	| \
							 PIN_PUPDR_FLOATING( 7)	| \
							 PIN_PUPDR_FLOATING( 8)	| \
							 PIN_PUPDR_PULLUP( 9)	| \
							 PIN_PUPDR_PULLUP(10)	| \
							 PIN_PUPDR_FLOATING(11)	| \
							 PIN_PUPDR_FLOATING(12)	| \
							 PIN_PUPDR_PULLUP(13)	| \
							 PIN_PUPDR_PULLDOWN(14)	| \
							 PIN_PUPDR_PULLUP(15))
#define VAL_GPIOA_ODR		(PIN_ODR_LOW(0)			| \
							 PIN_ODR_LOW(1)			| \
							 PIN_ODR_HIGH(15))
#define VAL_GPIOA_AFRL		(PIN_AFIO_AF( 0,  1U)	| \
							 PIN_AFIO_AF( 1,  1U)	| \
							 PIN_AFIO_AF( 2,  0U)	| \
							 PIN_AFIO_AF( 3,  0U)	| \
							 PIN_AFIO_AF( 4,  0U)	| \
							 PIN_AFIO_AF( 5,  0U)	| \
							 PIN_AFIO_AF( 6,  0U)	| \
							 PIN_AFIO_AF( 7,  0U))
#define VAL_GPIOA_AFRH		(PIN_AFIO_AF( 8,  0U)	| \
							 PIN_AFIO_AF( 9,  7U)	| \
							 PIN_AFIO_AF(10,  7U)	| \
							 PIN_AFIO_AF(11, 10U)	| \
							 PIN_AFIO_AF(12, 10U)	| \
							 PIN_AFIO_AF(13,  0U)	| \
							 PIN_AFIO_AF(14,  0U)	| \
							 PIN_AFIO_AF(15,  0U))

/*
 * GPIOB setup:
 *
 * PB0  -
 * PB1  -
 * PB2  - boot mode - set JP2 to pull high.
 * PB3  -
 * PB4  - SPI4_CS3 - MC33792 chip select
 * PB5  -
 * PB6  -
 * PB7  -
 * PB8  -
 * PB9  -
 * PB10 -
 * PB11 -
 * PB12 -
 * PB13 -
 * PB14 -
 * PB15 -
 */
#define VAL_GPIOB_MODER     (PIN_MODE_ANALOG(0)		| \
							 PIN_MODE_ANALOG(1)		| \
							 PIN_MODE_INPUT(2)		| \
							 /*PIN_MODE_ALTERNATE(3)*/ PIN_MODE_OUTPUT(3)	| \
							 PIN_MODE_OUTPUT(4)		| \
							 /*PIN_MODE_ALTERNATE(5)*/ PIN_MODE_OUTPUT(5)	| \
							 PIN_MODE_ALTERNATE(6)	| \
							 PIN_MODE_OUTPUT(7)		| \
							 /*PIN_MODE_ALTERNATE(8)*/  PIN_MODE_OUTPUT(8)	| \
							 /*PIN_MODE_ALTERNATE(9)*/  PIN_MODE_OUTPUT(9)	| \
							 /*PIN_MODE_ALTERNATE(10)*/ PIN_MODE_OUTPUT(10)	| \
							 /*PIN_MODE_ALTERNATE(11)*/ PIN_MODE_OUTPUT(11)	| \
							 PIN_MODE_ALTERNATE(12)	| \
							 PIN_MODE_INPUT(13)		| \
							 PIN_MODE_ALTERNATE(14)	| \
							 PIN_MODE_ALTERNATE(15))
#define VAL_GPIOB_OTYPER    (PIN_OTYPE_PUSHPULL( 0)	| \
							 PIN_OTYPE_PUSHPULL( 1)	| \
							 PIN_OTYPE_PUSHPULL( 2)	| \
							 PIN_OTYPE_PUSHPULL( 3)	| \
							 PIN_OTYPE_PUSHPULL( 4)	| \
							 PIN_OTYPE_PUSHPULL( 5)	| \
							 PIN_OTYPE_PUSHPULL( 6)	| \
							 PIN_OTYPE_PUSHPULL( 7)	| \
							 PIN_OTYPE_PUSHPULL( 8)	| \
							 PIN_OTYPE_PUSHPULL( 9)	| \
							 PIN_OTYPE_PUSHPULL(10)	| \
							 PIN_OTYPE_PUSHPULL(11)	| \
							 PIN_OTYPE_PUSHPULL(12)	| \
							 PIN_OTYPE_PUSHPULL(13)	| \
							 PIN_OTYPE_PUSHPULL(14)	| \
							 PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOB_OSPEEDR	(DEFAULT_GPIO_SPEED( 0)	| \
							 DEFAULT_GPIO_SPEED( 1)	| \
							 DEFAULT_GPIO_SPEED( 2)	| \
							 DEFAULT_GPIO_SPEED( 3)	| \
							 DEFAULT_GPIO_SPEED( 4)	| \
							 DEFAULT_GPIO_SPEED( 5)	| \
							 DEFAULT_GPIO_SPEED( 6)	| \
							 DEFAULT_GPIO_SPEED( 7)	| \
							 DEFAULT_GPIO_SPEED( 8)	| \
							 DEFAULT_GPIO_SPEED( 9)	| \
							 DEFAULT_GPIO_SPEED(10)	| \
							 DEFAULT_GPIO_SPEED(11)	| \
							 DEFAULT_GPIO_SPEED(12)	| \
							 DEFAULT_GPIO_SPEED(13)	| \
							 DEFAULT_GPIO_SPEED(14)	| \
							 DEFAULT_GPIO_SPEED(15))
#define VAL_GPIOB_PUPDR		(PIN_PUPDR_FLOATING( 0)	| \
							 PIN_PUPDR_FLOATING( 1)	| \
							 PIN_PUPDR_PULLDOWN( 2)	| \
							 PIN_PUPDR_FLOATING( 3)	| \
							 PIN_PUPDR_FLOATING( 4)	| \
							 PIN_PUPDR_FLOATING( 5)	| \
							 PIN_PUPDR_FLOATING( 6)	| \
							 PIN_PUPDR_FLOATING( 7)	| \
							 PIN_PUPDR_FLOATING( 8)	| \
							 PIN_PUPDR_FLOATING( 9)	| \
							 PIN_PUPDR_FLOATING(10)	| \
							 PIN_PUPDR_FLOATING(11)	| \
							 PIN_PUPDR_FLOATING(12)	| \
							 PIN_PUPDR_FLOATING(13)	| \
							 PIN_PUPDR_PULLDOWN(14)	| \
							 PIN_PUPDR_FLOATING(15))
#define VAL_GPIOB_ODR		(PIN_ODR_LOW(3)			| \
							 PIN_ODR_HIGH(4)		| \
							 PIN_ODR_LOW(5)			| \
							 PIN_ODR_LOW(8)			| \
							 PIN_ODR_LOW(9)			| \
							 PIN_ODR_LOW(10)		| \
							 PIN_ODR_LOW(11))
#define VAL_GPIOB_AFRL		(PIN_AFIO_AF( 0,  0U)	| \
							 PIN_AFIO_AF( 1,  0U)	| \
							 PIN_AFIO_AF( 2,  0U)	| \
							 PIN_AFIO_AF( 3,  1U)	| \
							 PIN_AFIO_AF( 4,  0U)	| \
							 PIN_AFIO_AF( 5,  2U)	| \
							 PIN_AFIO_AF( 6, 10U)	| \
							 PIN_AFIO_AF( 7,  0U))
#define VAL_GPIOB_AFRH		(PIN_AFIO_AF( 8,  2U)	| \
							 PIN_AFIO_AF( 9,  2U)	| \
							 PIN_AFIO_AF(10,  1U)	| \
							 PIN_AFIO_AF(11,  1U)	| \
							 PIN_AFIO_AF(12, 12U)	| \
							 PIN_AFIO_AF(13,  0U)	| \
							 PIN_AFIO_AF(14, 12U)	| \
							 PIN_AFIO_AF(15, 12U))

/*
 * GPIOC setup:
 *
 * PC0  -
 * PC1  -
 * PC2  -
 * PC3  -
 * PC4  -
 * PC5  -
 * PC6  -
 * PC7  -
 * PC8  -
 * PC9  -
 * PC10 -
 * PC11 -
 * PC12 -
 * PC13 -
 * PC14 -
 * PC15 -
 */
#define VAL_GPIOC_MODER     (PIN_MODE_ANALOG(0)		| \
							 PIN_MODE_ANALOG(1)		| \
							 PIN_MODE_ANALOG(2)		| \
							 PIN_MODE_ANALOG(3)		| \
							 PIN_MODE_ANALOG(4)		| \
							 PIN_MODE_ANALOG(5)		| \
							 /*PIN_MODE_ALTERNATE(6)*/  PIN_MODE_OUTPUT(6)	| \
							 /*PIN_MODE_ALTERNATE(7)*/  PIN_MODE_OUTPUT(7)	| \
							 /*PIN_MODE_ALTERNATE(8)*/  PIN_MODE_INPUT(8)	| \
							 /*PIN_MODE_ALTERNATE(9)*/  PIN_MODE_INPUT(9)	| \
							 PIN_MODE_ALTERNATE(10)	| \
							 PIN_MODE_ALTERNATE(11)	| \
							 PIN_MODE_ALTERNATE(12)	| \
							 PIN_MODE_OUTPUT(13)	| \
							 PIN_MODE_OUTPUT(14)	| \
							 PIN_MODE_OUTPUT(15))
#define VAL_GPIOC_OTYPER    (PIN_OTYPE_PUSHPULL( 0)	| \
							 PIN_OTYPE_PUSHPULL( 1)	| \
							 PIN_OTYPE_PUSHPULL( 2)	| \
							 PIN_OTYPE_PUSHPULL( 3)	| \
							 PIN_OTYPE_PUSHPULL( 4)	| \
							 PIN_OTYPE_PUSHPULL( 5)	| \
							 PIN_OTYPE_PUSHPULL( 6)	| \
							 PIN_OTYPE_PUSHPULL( 7)	| \
							 PIN_OTYPE_PUSHPULL( 8)	| \
							 PIN_OTYPE_PUSHPULL( 9)	| \
							 PIN_OTYPE_PUSHPULL(10)	| \
							 PIN_OTYPE_PUSHPULL(11)	| \
							 PIN_OTYPE_PUSHPULL(12)	| \
							 PIN_OTYPE_PUSHPULL(13)	| \
							 PIN_OTYPE_PUSHPULL(14)	| \
							 PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOC_OSPEEDR	(DEFAULT_GPIO_SPEED( 0)	| \
							 DEFAULT_GPIO_SPEED( 1)	| \
							 DEFAULT_GPIO_SPEED( 2)	| \
							 DEFAULT_GPIO_SPEED( 3)	| \
							 DEFAULT_GPIO_SPEED( 4)	| \
							 DEFAULT_GPIO_SPEED( 5)	| \
							 DEFAULT_GPIO_SPEED( 6)	| \
							 DEFAULT_GPIO_SPEED( 7)	| \
							 DEFAULT_GPIO_SPEED( 8)	| \
							 DEFAULT_GPIO_SPEED( 9)	| \
							 DEFAULT_GPIO_SPEED(10)	| \
							 DEFAULT_GPIO_SPEED(11)	| \
							 DEFAULT_GPIO_SPEED(12)	| \
							 DEFAULT_GPIO_SPEED(13)	| \
							 DEFAULT_GPIO_SPEED(14)	| \
							 DEFAULT_GPIO_SPEED(15))
#define VAL_GPIOC_PUPDR		(PIN_PUPDR_FLOATING( 0)	| \
							 PIN_PUPDR_FLOATING( 1)	| \
							 PIN_PUPDR_FLOATING( 2)	| \
							 PIN_PUPDR_FLOATING( 3)	| \
							 PIN_PUPDR_FLOATING( 4)	| \
							 PIN_PUPDR_FLOATING( 5)	| \
							 PIN_PUPDR_FLOATING( 6)	| \
							 PIN_PUPDR_FLOATING( 7)	| \
							 PIN_PUPDR_PULLUP( 8)	| \
							 PIN_PUPDR_PULLUP( 9)	| \
							 PIN_PUPDR_FLOATING(10)	| \
							 PIN_PUPDR_PULLUP(11)	| \
							 PIN_PUPDR_FLOATING(12)	| \
							 PIN_PUPDR_FLOATING(13)	| \
							 PIN_PUPDR_FLOATING(14)	| \
							 PIN_PUPDR_FLOATING(15))
#define VAL_GPIOC_ODR		(PIN_ODR_LOW(6)			| \
							 PIN_ODR_LOW(7)			| \
							 PIN_ODR_HIGH(13)		| \
							 PIN_ODR_HIGH(14)		| \
							 PIN_ODR_HIGH(15))
#define VAL_GPIOC_AFRL		(PIN_AFIO_AF( 0,  0U)	| \
							 PIN_AFIO_AF( 1,  0U)	| \
							 PIN_AFIO_AF( 2,  0U)	| \
							 PIN_AFIO_AF( 3,  0U)	| \
							 PIN_AFIO_AF( 4,  0U)	| \
							 PIN_AFIO_AF( 5,  0U)	| \
							 PIN_AFIO_AF( 6,  3U)	| \
							 PIN_AFIO_AF( 7,  3U))
#define VAL_GPIOC_AFRH		(PIN_AFIO_AF( 8,  4U)	| \
							 PIN_AFIO_AF( 9,  4U)	| \
							 PIN_AFIO_AF(10,  6U)	| \
							 PIN_AFIO_AF(11,  6U)	| \
							 PIN_AFIO_AF(12,  6U)	| \
							 PIN_AFIO_AF(13,  0U)	| \
							 PIN_AFIO_AF(14,  0U)	| \
							 PIN_AFIO_AF(15,  0U))

/*
 * GPIOD setup:
 *
 * PD0  -
 * PD1  -
 * PD2  - USB_HS 5V en out
 * PD3  - USB HS 5V Over current input
 * PD4  - USB FS 5V Over current input
 * PD5  - USB FS 5V en out
 * PD6  -
 * PD7  -
 * PD8  -
 * PD9  -
 * PD10 -
 * PD11 -
 * PD12 -
 * PD13 -
 * PD14 -
 * PD15 -
 */
#define VAL_GPIOD_MODER     (PIN_MODE_ALTERNATE(0)	| \
							 PIN_MODE_ALTERNATE(1)	| \
							 PIN_MODE_OUTPUT(2)		| \
							 PIN_MODE_INPUT(3)		| \
							 PIN_MODE_INPUT(4)		| \
							 PIN_MODE_OUTPUT(5)		| \
							 PIN_MODE_ALTERNATE(6)	| \
							 PIN_MODE_ALTERNATE(7)	| \
							 PIN_MODE_ALTERNATE(8)	| \
							 PIN_MODE_ALTERNATE(9)	| \
							 PIN_MODE_OUTPUT(10)	| \
							 PIN_MODE_ALTERNATE(11)	| \
							 PIN_MODE_ALTERNATE(12)	| \
							 PIN_MODE_ALTERNATE(13)	| \
							 /*PIN_MODE_ALTERNATE(14)*/  PIN_MODE_OUTPUT(14)	| \
							 /*PIN_MODE_ALTERNATE(15)*/  PIN_MODE_OUTPUT(15))
#define VAL_GPIOD_OTYPER    (PIN_OTYPE_PUSHPULL( 0)	| \
							 PIN_OTYPE_PUSHPULL( 1)	| \
							 PIN_OTYPE_PUSHPULL( 2)	| \
							 PIN_OTYPE_PUSHPULL( 3)	| \
							 PIN_OTYPE_PUSHPULL( 4)	| \
							 PIN_OTYPE_PUSHPULL( 5)	| \
							 PIN_OTYPE_PUSHPULL( 6)	| \
							 PIN_OTYPE_PUSHPULL( 7)	| \
							 PIN_OTYPE_PUSHPULL( 8)	| \
							 PIN_OTYPE_PUSHPULL( 9)	| \
							 PIN_OTYPE_PUSHPULL(10)	| \
							 PIN_OTYPE_PUSHPULL(11)	| \
							 PIN_OTYPE_PUSHPULL(12)	| \
							 PIN_OTYPE_PUSHPULL(13)	| \
							 PIN_OTYPE_PUSHPULL(14)	| \
							 PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOD_OSPEEDR	(DEFAULT_GPIO_SPEED( 0)	| \
							 DEFAULT_GPIO_SPEED( 1)	| \
							 DEFAULT_GPIO_SPEED( 2)	| \
							 DEFAULT_GPIO_SPEED( 3)	| \
							 DEFAULT_GPIO_SPEED( 4)	| \
							 DEFAULT_GPIO_SPEED( 5)	| \
							 DEFAULT_GPIO_SPEED( 6)	| \
							 DEFAULT_GPIO_SPEED( 7)	| \
							 DEFAULT_GPIO_SPEED( 8)	| \
							 DEFAULT_GPIO_SPEED( 9)	| \
							 DEFAULT_GPIO_SPEED(10)	| \
							 DEFAULT_GPIO_SPEED(11)	| \
							 DEFAULT_GPIO_SPEED(12)	| \
							 DEFAULT_GPIO_SPEED(13)	| \
							 DEFAULT_GPIO_SPEED(14)	| \
							 DEFAULT_GPIO_SPEED(15))
#define VAL_GPIOD_PUPDR		(PIN_PUPDR_PULLUP( 0)	| \
							 PIN_PUPDR_FLOATING( 1)	| \
							 PIN_PUPDR_FLOATING( 2)	| \
							 PIN_PUPDR_PULLUP( 3)	| \
							 PIN_PUPDR_PULLUP( 4)	| \
							 PIN_PUPDR_FLOATING( 5)	| \
							 PIN_PUPDR_FLOATING( 6)	| \
							 PIN_PUPDR_FLOATING( 7)	| \
							 PIN_PUPDR_FLOATING( 8)	| \
							 PIN_PUPDR_PULLUP( 9)	| \
							 PIN_PUPDR_FLOATING(10)	| \
							 PIN_PUPDR_PULLUP(11)	| \
							 PIN_PUPDR_PULLUP(12)	| \
							 PIN_PUPDR_PULLUP(13)	| \
							 PIN_PUPDR_FLOATING(14)	| \
							 PIN_PUPDR_FLOATING(15))
#define VAL_GPIOD_ODR		(PIN_ODR_LOW(2)			| \
							 PIN_ODR_LOW(5)			| \
							 PIN_ODR_HIGH(10)		| \
							 PIN_ODR_LOW(14)		| \
							 PIN_ODR_LOW(15))
#define VAL_GPIOD_AFRL		(PIN_AFIO_AF( 0,  9U)	| \
							 PIN_AFIO_AF( 1,  9U)	| \
							 PIN_AFIO_AF( 2,  0U)	| \
							 PIN_AFIO_AF( 3,  0U)	| \
							 PIN_AFIO_AF( 4,  0U)	| \
							 PIN_AFIO_AF( 5,  0U)	| \
							 PIN_AFIO_AF( 6, 11U)	| \
							 PIN_AFIO_AF( 7, 11U))
#define VAL_GPIOD_AFRH		(PIN_AFIO_AF( 8,  7U)	| \
							 PIN_AFIO_AF( 9,  7U)	| \
							 PIN_AFIO_AF(10,  0U)	| \
							 PIN_AFIO_AF(11,  9U)	| \
							 PIN_AFIO_AF(12,  9U)	| \
							 PIN_AFIO_AF(13,  9U)	| \
							 PIN_AFIO_AF(14,  2U)	| \
							 PIN_AFIO_AF(15,  2U))

/*
 * GPIOE setup:
 *
 * PE0  -
 * PE1  -
 * PE2  -
 * PE3  -
 * PE4  -
 * PE5  -
 * PE6  - SPI4_MOSI
 * PE7  -
 * PE8  -
 * PE9  -
 * PE10 -
 * PE11 -
 * PE12 -
 * PE13 -
 * PE14 -
 * PE15 -
 */
#define VAL_GPIOE_MODER     (PIN_MODE_ALTERNATE(0)	| \
							 PIN_MODE_ALTERNATE(1)	| \
							 PIN_MODE_ALTERNATE(2)	| \
							 PIN_MODE_OUTPUT(3)		| \
							 PIN_MODE_OUTPUT(4)		| \
							 PIN_MODE_ALTERNATE(5)	| \
							 PIN_MODE_ALTERNATE(6)	| \
							 PIN_MODE_ALTERNATE(7)	| \
							 PIN_MODE_ALTERNATE(8)	| \
							 PIN_MODE_OUTPUT(9)	| \
							 /*PIN_MODE_ALTERNATE(10)*/  PIN_MODE_OUTPUT(10)	| \
							 /*PIN_MODE_ALTERNATE(11)*/  PIN_MODE_OUTPUT(11)	| \
							 PIN_MODE_ALTERNATE(12)	| \
							 /*PIN_MODE_ALTERNATE(13)*/  PIN_MODE_OUTPUT(13)	| \
							 /*PIN_MODE_ALTERNATE(14)*/  PIN_MODE_OUTPUT(14)	| \
							 PIN_MODE_INPUT(15))
#define VAL_GPIOE_OTYPER    (PIN_OTYPE_PUSHPULL( 0)	| \
							 PIN_OTYPE_PUSHPULL( 1)	| \
							 PIN_OTYPE_PUSHPULL( 2)	| \
							 PIN_OTYPE_PUSHPULL( 3)	| \
							 PIN_OTYPE_PUSHPULL( 4)	| \
							 PIN_OTYPE_PUSHPULL( 5)	| \
							 PIN_OTYPE_PUSHPULL( 6)	| \
							 PIN_OTYPE_PUSHPULL( 7)	| \
							 PIN_OTYPE_PUSHPULL( 8)	| \
							 PIN_OTYPE_PUSHPULL( 9)	| \
							 PIN_OTYPE_PUSHPULL(10)	| \
							 PIN_OTYPE_PUSHPULL(11)	| \
							 PIN_OTYPE_PUSHPULL(12)	| \
							 PIN_OTYPE_PUSHPULL(13)	| \
							 PIN_OTYPE_PUSHPULL(14)	| \
							 PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOE_OSPEEDR	(DEFAULT_GPIO_SPEED( 0)	| \
							 DEFAULT_GPIO_SPEED( 1)	| \
							 DEFAULT_GPIO_SPEED( 2)	| \
							 DEFAULT_GPIO_SPEED( 3)	| \
							 DEFAULT_GPIO_SPEED( 4)	| \
							 DEFAULT_GPIO_SPEED( 5)	| \
							 DEFAULT_GPIO_SPEED( 6)	| \
							 DEFAULT_GPIO_SPEED( 7)	| \
							 DEFAULT_GPIO_SPEED( 8)	| \
							 DEFAULT_GPIO_SPEED( 9)	| \
							 DEFAULT_GPIO_SPEED(10)	| \
							 DEFAULT_GPIO_SPEED(11)	| \
							 DEFAULT_GPIO_SPEED(12)	| \
							 DEFAULT_GPIO_SPEED(13)	| \
							 DEFAULT_GPIO_SPEED(14)	| \
							 DEFAULT_GPIO_SPEED(15))
#define VAL_GPIOE_PUPDR		(PIN_PUPDR_PULLUP( 0)	| \
							 PIN_PUPDR_FLOATING( 1)	| \
							 PIN_PUPDR_PULLUP( 2)	| \
							 PIN_PUPDR_FLOATING( 3)	| \
							 PIN_PUPDR_FLOATING( 4)	| \
							 PIN_PUPDR_PULLUP( 5)	| \
							 PIN_PUPDR_FLOATING( 6)	| \
							 PIN_PUPDR_PULLUP( 7)	| \
							 PIN_PUPDR_FLOATING( 8)	| \
							 PIN_PUPDR_FLOATING( 9)	| \
							 PIN_PUPDR_FLOATING(10)	| \
							 PIN_PUPDR_FLOATING(11)	| \
							 PIN_PUPDR_FLOATING(12)	| \
							 PIN_PUPDR_FLOATING(13)	| \
							 PIN_PUPDR_FLOATING(14)	| \
							 PIN_PUPDR_PULLUP(15))
#define VAL_GPIOE_ODR		(PIN_ODR_LOW(3)			| \
							 PIN_ODR_HIGH(4)		| \
							 PIN_ODR_LOW(9))
#define VAL_GPIOE_AFRL		(PIN_AFIO_AF( 0,  8U)	| \
							 PIN_AFIO_AF( 1,  8U)	| \
							 PIN_AFIO_AF( 2,  9U)	| \
							 PIN_AFIO_AF( 3,  0U)	| \
							 PIN_AFIO_AF( 4,  5U)	| \
							 PIN_AFIO_AF( 5,  5U)	| \
							 PIN_AFIO_AF( 6,  5U)	| \
							 PIN_AFIO_AF( 7,  8U))
#define VAL_GPIOE_AFRH		(PIN_AFIO_AF( 8,  8U)	| \
							 PIN_AFIO_AF( 9,  0U)	| \
							 PIN_AFIO_AF(10,  1U)	| \
							 PIN_AFIO_AF(11,  1U)	| \
							 PIN_AFIO_AF(12,  5U)	| \
							 PIN_AFIO_AF(13,  1U)	| \
							 PIN_AFIO_AF(14,  1U)	| \
							 PIN_AFIO_AF(15,  0U))

/*
 * GPIOF setup:
 *
 * PF0  -
 * PF1  -
 * PF2  -
 * PF3  -
 * PF4  -
 * PF5  -
 * PF6  -
 * PF7  -
 * PF8  -
 * PF9  -
 * PF10 -
 * PF11 -
 * PF12 -
 * PF13 -
 * PF14 - self simulation pin 0 (actually i2c)
 * PF15 - self simulation pin 1 (actually i2c)
 */
#define VAL_GPIOF_MODER     (PIN_MODE_OUTPUT(0)		| \
							 PIN_MODE_OUTPUT(1)		| \
							 PIN_MODE_OUTPUT(2)		| \
							 PIN_MODE_ANALOG(3)		| \
							 PIN_MODE_ANALOG(4)		| \
							 PIN_MODE_ANALOG(5)		| \
							 PIN_MODE_ANALOG(6)		| \
							 PIN_MODE_ANALOG(7)		| \
							 /*PIN_MODE_ALTERNATE(8)*/  PIN_MODE_OUTPUT(8)	| \
							 /*PIN_MODE_ALTERNATE(9)*/  PIN_MODE_OUTPUT(9)	| \
							 PIN_MODE_ALTERNATE(10)	| \
							 PIN_MODE_OUTPUT(11)	| \
							 PIN_MODE_OUTPUT(12)	| \
							 PIN_MODE_INPUT(13)		| \
							 /* PIN_MODE_ALTERNATE(14)	| \ */ PIN_MODE_OUTPUT(14)	| \
							 /* PIN_MODE_ALTERNATE(15)) */ PIN_MODE_OUTPUT(15))
#define VAL_GPIOF_OTYPER    (PIN_OTYPE_PUSHPULL( 0)	| \
							 PIN_OTYPE_PUSHPULL( 1)	| \
							 PIN_OTYPE_PUSHPULL( 2)	| \
							 PIN_OTYPE_PUSHPULL( 3)	| \
							 PIN_OTYPE_PUSHPULL( 4)	| \
							 PIN_OTYPE_PUSHPULL( 5)	| \
							 PIN_OTYPE_PUSHPULL( 6)	| \
							 PIN_OTYPE_PUSHPULL( 7)	| \
							 PIN_OTYPE_PUSHPULL( 8)	| \
							 PIN_OTYPE_PUSHPULL( 9)	| \
							 PIN_OTYPE_PUSHPULL(10)	| \
							 PIN_OTYPE_PUSHPULL(11)	| \
							 PIN_OTYPE_PUSHPULL(12)	| \
							 PIN_OTYPE_PUSHPULL(13)	| \
							 /* PIN_OTYPE_OPENDRAIN(14)| \ */ PIN_OTYPE_PUSHPULL(14) | \
							 /* PIN_OTYPE_OPENDRAIN(15)) */ PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOF_OSPEEDR	(DEFAULT_GPIO_SPEED( 0)	| \
							 DEFAULT_GPIO_SPEED( 1)	| \
							 DEFAULT_GPIO_SPEED( 2)	| \
							 DEFAULT_GPIO_SPEED( 3)	| \
							 DEFAULT_GPIO_SPEED( 4)	| \
							 DEFAULT_GPIO_SPEED( 5)	| \
							 DEFAULT_GPIO_SPEED( 6)	| \
							 DEFAULT_GPIO_SPEED( 7)	| \
							 DEFAULT_GPIO_SPEED( 8)	| \
							 DEFAULT_GPIO_SPEED( 9)	| \
							 DEFAULT_GPIO_SPEED(10)	| \
							 DEFAULT_GPIO_SPEED(11)	| \
							 DEFAULT_GPIO_SPEED(12)	| \
							 DEFAULT_GPIO_SPEED(13)	| \
							 DEFAULT_GPIO_SPEED(14)	| \
							 DEFAULT_GPIO_SPEED(15))
#define VAL_GPIOF_PUPDR		(PIN_PUPDR_FLOATING( 0)	| \
							 PIN_PUPDR_FLOATING( 1)	| \
							 PIN_PUPDR_FLOATING( 2)	| \
							 PIN_PUPDR_FLOATING( 3)	| \
							 PIN_PUPDR_FLOATING( 4)	| \
							 PIN_PUPDR_FLOATING( 5)	| \
							 PIN_PUPDR_FLOATING( 6)	| \
							 PIN_PUPDR_FLOATING( 7)	| \
							 PIN_PUPDR_FLOATING( 8)	| \
							 PIN_PUPDR_FLOATING( 9)	| \
							 PIN_PUPDR_FLOATING(10)	| \
							 PIN_PUPDR_FLOATING(11)	| \
							 PIN_PUPDR_FLOATING(12)	| \
							 PIN_PUPDR_PULLUP(13)	| \
							 PIN_PUPDR_PULLUP(14)	| \
							 PIN_PUPDR_PULLUP(15))
#define VAL_GPIOF_ODR		(PIN_ODR_HIGH(0)		| \
							 PIN_ODR_HIGH(1)		| \
							 PIN_ODR_HIGH(2)		| \
							 PIN_ODR_LOW(8)			| \
							 PIN_ODR_LOW(9)			| \
							 PIN_ODR_HIGH(11)		| \
							 PIN_ODR_HIGH(12)		| \
							 PIN_ODR_HIGH(14)		| \
							 PIN_ODR_HIGH(15))
#define VAL_GPIOF_AFRL		(PIN_AFIO_AF( 0,  0U)	| \
							 PIN_AFIO_AF( 1,  0U)	| \
							 PIN_AFIO_AF( 2,  0U)	| \
							 PIN_AFIO_AF( 3,  0U)	| \
							 PIN_AFIO_AF( 4,  0U)	| \
							 PIN_AFIO_AF( 5,  0U)	| \
							 PIN_AFIO_AF( 6,  0U)	| \
							 PIN_AFIO_AF( 7,  0U))
#define VAL_GPIOF_AFRH		(PIN_AFIO_AF( 8,  9U)	| \
							 PIN_AFIO_AF( 9,  9U)	| \
							 PIN_AFIO_AF(10,  9U)	| \
							 PIN_AFIO_AF(11,  0U)	| \
							 PIN_AFIO_AF(12,  0U)	| \
							 PIN_AFIO_AF(13,  0U)	| \
							 /*PIN_AFIO_AF(14,  4U)	| \ */ PIN_AFIO_AF(14,  0U)	| \
							 /*PIN_AFIO_AF(15,  4U)) */ PIN_AFIO_AF(15,  0U))

/*
 * GPIOG setup:
 *
 * PG0  -
 * PG1  -
 * PG2  -
 * PG3  -
 * PG4  -
 * PG5  -
 * PG6  - LD1 - active low
 * PG7  - LD3 - active low
 * PG8  - LD2 - active PIN_ODR_LOW
 * PG9  -
 * PG10 -
 * PG11 -
 * PG12 -
 * PG13 -
 * PG14 -
 * PG15 -
 */
#define VAL_GPIOG_MODER     (PIN_MODE_OUTPUT(0)		| \
							 PIN_MODE_INPUT(1)		| \
							 PIN_MODE_OUTPUT(2)		| \
							 PIN_MODE_OUTPUT(3)		| \
							 PIN_MODE_OUTPUT(4)		| \
							 PIN_MODE_OUTPUT(5)		| \
							 PIN_MODE_OUTPUT(6)		| \
							 PIN_MODE_OUTPUT(7)		| \
							 PIN_MODE_OUTPUT(8)	| \
							 PIN_MODE_ALTERNATE(9)	| \
							 PIN_MODE_ALTERNATE(10)	| \
							 PIN_MODE_ALTERNATE(11)	| \
							 PIN_MODE_ALTERNATE(12)	| \
							 PIN_MODE_OUTPUT(13)	| \
							 PIN_MODE_ALTERNATE(14)	| \
							 PIN_MODE_OUTPUT(15))
#define VAL_GPIOG_OTYPER 	(PIN_OTYPE_PUSHPULL( 0)	| \
							 PIN_OTYPE_PUSHPULL( 1)	| \
							 PIN_OTYPE_PUSHPULL( 2)	| \
							 PIN_OTYPE_PUSHPULL( 3)	| \
							 PIN_OTYPE_PUSHPULL( 4)	| \
							 PIN_OTYPE_PUSHPULL( 5)	| \
							 PIN_OTYPE_OPENDRAIN( 6)| \
							 PIN_OTYPE_OPENDRAIN( 7)| \
							 PIN_OTYPE_OPENDRAIN( 8)| \
							 PIN_OTYPE_PUSHPULL( 9)	| \
							 PIN_OTYPE_PUSHPULL(10)	| \
							 PIN_OTYPE_PUSHPULL(11)	| \
							 PIN_OTYPE_PUSHPULL(12)	| \
							 PIN_OTYPE_PUSHPULL(13)	| \
							 PIN_OTYPE_OPENDRAIN(14)| \
							 PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOG_OSPEEDR	(DEFAULT_GPIO_SPEED( 0)	| \
							 DEFAULT_GPIO_SPEED( 1)	| \
							 DEFAULT_GPIO_SPEED( 2)	| \
							 DEFAULT_GPIO_SPEED( 3)	| \
							 DEFAULT_GPIO_SPEED( 4)	| \
							 DEFAULT_GPIO_SPEED( 5)	| \
							 DEFAULT_GPIO_SPEED( 6)	| \
							 DEFAULT_GPIO_SPEED( 7)	| \
							 DEFAULT_GPIO_SPEED( 8)	| \
							 DEFAULT_GPIO_SPEED( 9)	| \
							 DEFAULT_GPIO_SPEED(10)	| \
							 DEFAULT_GPIO_SPEED(11)	| \
							 DEFAULT_GPIO_SPEED(12)	| \
							 DEFAULT_GPIO_SPEED(13)	| \
							 DEFAULT_GPIO_SPEED(14)	| \
							 DEFAULT_GPIO_SPEED(15))
#define VAL_GPIOG_PUPDR		(PIN_PUPDR_FLOATING( 0)	| \
							 PIN_PUPDR_PULLUP( 1)	| \
							 PIN_PUPDR_FLOATING( 2)	| \
							 PIN_PUPDR_FLOATING( 3)	| \
							 PIN_PUPDR_FLOATING( 4)	| \
							 PIN_PUPDR_FLOATING( 5)	| \
							 PIN_PUPDR_FLOATING( 6)	| \
							 PIN_PUPDR_FLOATING( 7)	| \
							 PIN_PUPDR_FLOATING( 8)	| \
							 PIN_PUPDR_PULLUP( 9)	| \
							 PIN_PUPDR_PULLUP(10)	| \
							 PIN_PUPDR_PULLUP(11)	| \
							 PIN_PUPDR_PULLUP(12)	| \
							 PIN_PUPDR_FLOATING(13)	| \
							 PIN_PUPDR_PULLUP(14)	| \
							 PIN_PUPDR_FLOATING(15))
#define VAL_GPIOG_ODR		(PIN_ODR_LOW(0)			| \
							 PIN_ODR_LOW(2)			| \
							 PIN_ODR_LOW(3)			| \
							 PIN_ODR_LOW(4)			| \
							 PIN_ODR_LOW(5)			| \
							 PIN_ODR_HIGH(6)		| \
							 PIN_ODR_HIGH(7)		| \
							 PIN_ODR_HIGH(8)		| \
							 PIN_ODR_HIGH(13)		| \
							 PIN_ODR_LOW(15))
#define VAL_GPIOG_AFRL		(PIN_AFIO_AF( 0,  0U)	| \
							 PIN_AFIO_AF( 1,  0U)	| \
							 PIN_AFIO_AF( 2,  0U)	| \
							 PIN_AFIO_AF( 3,  0U)	| \
							 PIN_AFIO_AF( 4,  0U)	| \
							 PIN_AFIO_AF( 5,  0U)	| \
							 PIN_AFIO_AF( 6,  0U)	| \
							 PIN_AFIO_AF( 7,  0U))
#define VAL_GPIOG_AFRH		(PIN_AFIO_AF( 8,  0U)	| \
							 PIN_AFIO_AF( 9, 11U)	| \
							 PIN_AFIO_AF(10, 11U)	| \
							 PIN_AFIO_AF(11, 10U)	| \
							 PIN_AFIO_AF(12, 11U)	| \
							 PIN_AFIO_AF(13,  0U)	| \
							 PIN_AFIO_AF(14,  8U)	| \
							 PIN_AFIO_AF(15,  0U))

/*
 * GPIOH setup:
 *
 * PH0  - OSC_IN                    (input floating).
 * PH1  - OSC_OUT                   (input floating).
 */
#define VAL_GPIOH_MODER		(PIN_MODE_INPUT(0)		| \
							 PIN_MODE_INPUT(1))
#define VAL_GPIOH_OTYPER	(PIN_OTYPE_PUSHPULL( 0)	| \
							 PIN_OTYPE_PUSHPULL( 1))
#define VAL_GPIOH_OSPEEDR	(DEFAULT_GPIO_SPEED( 0)	| \
							 DEFAULT_GPIO_SPEED( 1))
#define VAL_GPIOH_PUPDR		(PIN_PUPDR_FLOATING( 0)	| \
							 PIN_PUPDR_FLOATING( 1))
#define VAL_GPIOH_ODR 		(PIN_ODR_HIGH( 0)		| \
							 PIN_ODR_HIGH( 1))
#define VAL_GPIOH_AFRL 		(PIN_AFIO_AF( 0, 0U)	| \
							 PIN_AFIO_AF( 1, 0U))
#define VAL_GPIOH_AFRH      (0)

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
  void setBoardConfigurationOverrides(void);
  void setPinConfigurationOverrides(void);
  void setSerialConfigurationOverrides(void);
  void setSdCardConfigurationOverrides(void);
  void setAdcChannelOverrides(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* BOARD_H */
