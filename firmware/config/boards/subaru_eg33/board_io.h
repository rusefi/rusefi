/**
 * @file boards/subaru_eg33/board_io.c
 *
 * @date Feb 06, 2021
 * @author Andrey Gusakov, 2021
 */

#ifndef BOARD_IO_H
#define BOARD_IO_H

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
 * PA0  - n.u.
 * PA1  - ignition current sensor for cyl 1, 3, 5, 7
 * PA2  - ignition current sensor for cyl 2, 4, 6, 8
 * PA3  - MAF sensor input
 * PA4  - n.u.
 * PA5  - n.u.
 * PA6  - battery voltage input
 * PA7  - knock sensor input
 * PA8  - knock IC clock out
 * PA9  - USART1_TX - boot/console - TTL level on XP4.2
 * PA10 - USART1_RX - boot/console - TTL level on XP4.3
 * PA11 - USB_FS_D- - boot/console/TS - XS3
 * PA12 - USB_FS_D+ - boot/console/TS - XS3
 * PA13 - SWDIO
 * PA14 - SWCLK
 * PA15 - bluetooth/wifi module reset gpio
 */
#define VAL_GPIOA_MODER     (PIN_OTYPE_PUSHPULL(1)	| \
							 PIN_MODE_ANALOG(2)		| \
							 PIN_MODE_ANALOG(2)		| \
							 PIN_MODE_ANALOG(3)		| \
							 PIN_OTYPE_PUSHPULL(4)	| \
							 PIN_OTYPE_PUSHPULL(5)	| \
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
							 PIN_ODR_LOW(4)			| \
							 PIN_ODR_LOW(5)			| \
							 PIN_ODR_LOW(15))
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
 * PB0  - oxygen sensor #2
 * PB1  - EGR t sensor
 * PB2  - boot mode - pulled low
 * PB3  - ignition #1
 * PB4  - ignition #3
 * PB5  - ignition #7
 * PB6  - QSPI CS
 * PB7  - ignition (1, 3, 5, 7) spark duration input
 * PB8  - ignition #5
 * PB9  - injector #7
 * PB10 - VR diagnostic (crank #2 and cam)
 * PB11 - n.u.
 * PB12 - USB HS ID
 * PB13 - USB HS VBus
 * PB14 - USB HS DM
 * PB15 - USB HS DP
 */
#define VAL_GPIOB_MODER     (PIN_MODE_ANALOG(0)		| \
							 PIN_MODE_ANALOG(1)		| \
							 PIN_MODE_INPUT(2)		| \
							 PIN_MODE_OUTPUT(3)		| \
							 PIN_MODE_OUTPUT(4)		| \
							 PIN_MODE_OUTPUT(5)		| \
							 PIN_MODE_ALTERNATE(6)	| \
							 PIN_MODE_INPUT(7)		| \
							 PIN_MODE_OUTPUT(8)		| \
							 PIN_MODE_OUTPUT(9)		| \
							 PIN_MODE_OUTPUT(10)	| \
							 PIN_MODE_OUTPUT(11)	| \
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
							 PIN_OSPEED_HIGH( 6)	| \
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
							 PIN_PUPDR_PULLUP( 7)	| \
							 PIN_PUPDR_FLOATING( 8)	| \
							 PIN_PUPDR_FLOATING( 9)	| \
							 PIN_PUPDR_FLOATING(10)	| \
							 PIN_PUPDR_FLOATING(11)	| \
							 PIN_PUPDR_FLOATING(12)	| \
							 PIN_PUPDR_FLOATING(13)	| \
							 PIN_PUPDR_PULLDOWN(14)	| \
							 PIN_PUPDR_FLOATING(15))
#define VAL_GPIOB_ODR		(PIN_ODR_LOW(3)			| \
							 PIN_ODR_LOW(4)			| \
							 PIN_ODR_LOW(5)			| \
							 PIN_ODR_LOW(8)			| \
							 PIN_ODR_LOW(9)			| \
							 PIN_ODR_LOW(10)		| \
							 PIN_ODR_LOW(11)		| \
							 PIN_ODR_LOW(13))
#define VAL_GPIOB_AFRL		(PIN_AFIO_AF( 0,  0U)	| \
							 PIN_AFIO_AF( 1,  0U)	| \
							 PIN_AFIO_AF( 2,  0U)	| \
							 PIN_AFIO_AF( 3,  0U)	| \
							 PIN_AFIO_AF( 4,  0U)	| \
							 PIN_AFIO_AF( 5,  0U)	| \
							 PIN_AFIO_AF( 6, 10U)	| \
							 PIN_AFIO_AF( 7,  0U))
#define VAL_GPIOB_AFRH		(PIN_AFIO_AF( 8,  0U)	| \
							 PIN_AFIO_AF( 9,  0U)	| \
							 PIN_AFIO_AF(10,  0U)	| \
							 PIN_AFIO_AF(11,  0U)	| \
							 PIN_AFIO_AF(12, 12U)	| \
							 PIN_AFIO_AF(13,  0U)	| \
							 PIN_AFIO_AF(14, 12U)	| \
							 PIN_AFIO_AF(15, 12U))

/*
 * GPIOC setup:
 *
 * PC0  - Atm P input
 * PC1  - Aux0 input
 * PC2  - Throtle input
 * PC3  - mc33972 analog input
 * PC4  - coolant T input
 * PC5  - oxygen #1 input
 * PC6  - idle open
 * PC7  - USB HS power enable
 * PC8  - idle close
 * PC9  - USB HS overcurrent input
 * PC10 - UART4 TX
 * PC11 - UART4 Rx
 * PC12 - bluetooth/wifi mode
 * PC13 - ignition #4
 * PC14 - ignition #2
 * PC15 - ignition #6
 */
#define VAL_GPIOC_MODER     (PIN_MODE_ANALOG(0)		| \
							 PIN_MODE_ANALOG(1)		| \
							 PIN_MODE_ANALOG(2)		| \
							 PIN_MODE_ANALOG(3)		| \
							 PIN_MODE_ANALOG(4)		| \
							 PIN_MODE_ANALOG(5)		| \
							 PIN_MODE_OUTPUT(6)		| \
							 PIN_MODE_OUTPUT(7)		| \
							 PIN_MODE_OUTPUT(8)		| \
							 PIN_MODE_INPUT(9)		| \
							 PIN_MODE_ALTERNATE(10)	| \
							 PIN_MODE_ALTERNATE(11)	| \
							 PIN_MODE_OUTPUT(12)	| \
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
							 PIN_PUPDR_FLOATING( 8)	| \
							 PIN_PUPDR_PULLUP( 9)	| \
							 PIN_PUPDR_FLOATING(10)	| \
							 PIN_PUPDR_PULLUP(11)	| \
							 PIN_PUPDR_FLOATING(12)	| \
							 PIN_PUPDR_FLOATING(13)	| \
							 PIN_PUPDR_FLOATING(14)	| \
							 PIN_PUPDR_FLOATING(15))
#define VAL_GPIOC_ODR		(PIN_ODR_LOW(6)			| \
							 PIN_ODR_LOW(7)			| \
							 PIN_ODR_LOW(8)			| \
							 PIN_ODR_LOW(12)		| \
							 PIN_ODR_LOW(13)		| \
							 PIN_ODR_LOW(14)		| \
							 PIN_ODR_LOW(15))
#define VAL_GPIOC_AFRL		(PIN_AFIO_AF( 0,  0U)	| \
							 PIN_AFIO_AF( 1,  0U)	| \
							 PIN_AFIO_AF( 2,  0U)	| \
							 PIN_AFIO_AF( 3,  0U)	| \
							 PIN_AFIO_AF( 4,  0U)	| \
							 PIN_AFIO_AF( 5,  0U)	| \
							 PIN_AFIO_AF( 6,  0U)	| \
							 PIN_AFIO_AF( 7,  0U))
#define VAL_GPIOC_AFRH		(PIN_AFIO_AF( 8,  0U)	| \
							 PIN_AFIO_AF( 9,  0U)	| \
							 PIN_AFIO_AF(10,  8U)	| \
							 PIN_AFIO_AF(11,  8U)	| \
							 PIN_AFIO_AF(12,  0U)	| \
							 PIN_AFIO_AF(13,  0U)	| \
							 PIN_AFIO_AF(14,  0U)	| \
							 PIN_AFIO_AF(15,  0U))

/*
 * GPIOD setup:
 *
 * PD0  - CAN1 Rx
 * PD1  - CAN1 Tx
 * PD2  - E-Thtot #2 direction
 * PD3  - E-Thtot #2 PWM
 * PD4  - E-Thtot #1 direction
 * PD5  - E-Thtot #1 PWM
 * PD6  - MMC clk
 * PD7  - MMC cmd
 * PD8  - UART3 Tx
 * PD9  - UART3 Rx
 * PD10 - Power hold to PMIC
 * PD11 - QSPI IO0
 * PD12 - QSPI IO1
 * PD13 - QSPI IO3
 * PD14 - Speed sensor input
 * PD15 - LIN RTS
 */
#define VAL_GPIOD_MODER     (PIN_MODE_ALTERNATE(0)	| \
							 PIN_MODE_ALTERNATE(1)	| \
							 PIN_MODE_OUTPUT(2)		| \
							 PIN_MODE_OUTPUT(3)		| \
							 PIN_MODE_OUTPUT(4)		| \
							 PIN_MODE_OUTPUT(5)		| \
							 PIN_MODE_ALTERNATE(6)	| \
							 PIN_MODE_ALTERNATE(7)	| \
							 PIN_MODE_ALTERNATE(8)	| \
							 PIN_MODE_ALTERNATE(9)	| \
							 PIN_MODE_OUTPUT(10)	| \
							 PIN_MODE_ALTERNATE(11)	| \
							 PIN_MODE_ALTERNATE(12)	| \
							 PIN_MODE_ALTERNATE(13)	| \
							 PIN_MODE_INPUT(14)		| \
							 PIN_MODE_ALTERNATE(15))
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
							 PIN_OSPEED_HIGH( 6)	| \
							 PIN_OSPEED_HIGH( 7)	| \
							 DEFAULT_GPIO_SPEED( 8)	| \
							 DEFAULT_GPIO_SPEED( 9)	| \
							 DEFAULT_GPIO_SPEED(10)	| \
							 PIN_OSPEED_HIGH(11)	| \
							 PIN_OSPEED_HIGH(12)	| \
							 PIN_OSPEED_HIGH(13)	| \
							 DEFAULT_GPIO_SPEED(14)	| \
							 DEFAULT_GPIO_SPEED(15))
#define VAL_GPIOD_PUPDR		(PIN_PUPDR_PULLUP( 0)	| \
							 PIN_PUPDR_FLOATING( 1)	| \
							 PIN_PUPDR_FLOATING( 2)	| \
							 PIN_PUPDR_FLOATING( 3)	| \
							 PIN_PUPDR_FLOATING( 4)	| \
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
							 PIN_ODR_LOW(3)			| \
							 PIN_ODR_LOW(4)			| \
							 PIN_ODR_LOW(5)			| \
							 PIN_ODR_HIGH(10))
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
							 PIN_AFIO_AF(14,  0U)	| \
							 PIN_AFIO_AF(15,  8U))

/*
 * GPIOE setup:
 *
 * PE0  - LIN Rx
 * PE1  - LIN Tx
 * PE2  - QSPI IO2
 * PE3  - injector #2
 * PE4  - injector #4
 * PE5  - injector #6
 * PE6  - injector %8
 * PE7  - n.u.
 * PE8  - n.u.
 * PE9  - Crank position sensor #2
 * PE10 - SPI4 CS2  (5V pulled)
 * PE11 - SPI4 CS1  (5V pulled)
 * PE12 - SPI4 SCK  (5V pulled)
 * PE13 - SPI4 MISO (5V pulled)
 * PE14 - SPI4 MOSI (5V pulled)
 * PE15 - SPI4 CS0  (5V pulled)
 */
#define VAL_GPIOE_MODER     (PIN_MODE_ALTERNATE(0)	| \
							 PIN_MODE_ALTERNATE(1)	| \
							 PIN_MODE_ALTERNATE(2)	| \
							 PIN_MODE_OUTPUT(3)		| \
							 PIN_MODE_OUTPUT(4)		| \
							 PIN_MODE_OUTPUT(5)		| \
							 PIN_MODE_OUTPUT(6)		| \
							 PIN_MODE_OUTPUT(7)		| \
							 PIN_MODE_OUTPUT(8)		| \
							 PIN_MODE_INPUT(9)		| \
							 PIN_MODE_OUTPUT(10)	| \
							 PIN_MODE_OUTPUT(11)	| \
							 PIN_MODE_ALTERNATE(12)	| \
							 PIN_MODE_ALTERNATE(13)	| \
							 PIN_MODE_ALTERNATE(14)	| \
							 PIN_MODE_OUTPUT(15))
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
							 PIN_OTYPE_OPENDRAIN(10)| \
							 PIN_OTYPE_OPENDRAIN(11)| \
							 PIN_OTYPE_OPENDRAIN(12)| \
							 PIN_OTYPE_PUSHPULL(13) /* PIN_OTYPE_OPENDRAIN(13) */| \
							 PIN_OTYPE_OPENDRAIN(14)| \
							 PIN_OTYPE_OPENDRAIN(15))
#define VAL_GPIOE_OSPEEDR	(DEFAULT_GPIO_SPEED( 0)	| \
							 DEFAULT_GPIO_SPEED( 1)	| \
							 PIN_OSPEED_HIGH( 2)	| \
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
							 PIN_PUPDR_FLOATING( 5)	| \
							 PIN_PUPDR_FLOATING( 6)	| \
							 PIN_PUPDR_FLOATING( 7)	| \
							 PIN_PUPDR_FLOATING( 8)	| \
							 PIN_PUPDR_PULLUP( 9)	| \
							 PIN_PUPDR_FLOATING(10)	| \
							 PIN_PUPDR_FLOATING(11)	| \
							 PIN_PUPDR_FLOATING(12)	| \
							 PIN_PUPDR_FLOATING(13)	| \
							 PIN_PUPDR_FLOATING(14)	| \
							 PIN_PUPDR_FLOATING(15))
#define VAL_GPIOE_ODR		(PIN_ODR_LOW(3)			| \
							 PIN_ODR_LOW(4)			| \
							 PIN_ODR_LOW(5)			| \
							 PIN_ODR_LOW(6)			| \
							 PIN_ODR_LOW(7)			| \
							 PIN_ODR_LOW(8)			| \
							 PIN_ODR_HIGH(10)		| \
							 PIN_ODR_HIGH(11)		| \
							 PIN_ODR_HIGH(15))
#define VAL_GPIOE_AFRL		(PIN_AFIO_AF( 0,  8U)	| \
							 PIN_AFIO_AF( 1,  8U)	| \
							 PIN_AFIO_AF( 2,  9U)	| \
							 PIN_AFIO_AF( 3,  0U)	| \
							 PIN_AFIO_AF( 4,  0U)	| \
							 PIN_AFIO_AF( 5,  0U)	| \
							 PIN_AFIO_AF( 6,  0U)	| \
							 PIN_AFIO_AF( 7,  0U))
#define VAL_GPIOE_AFRH		(PIN_AFIO_AF( 8,  0U)	| \
							 PIN_AFIO_AF( 9,  0U)	| \
							 PIN_AFIO_AF(10,  0U)	| \
							 PIN_AFIO_AF(11,  0U)	| \
							 PIN_AFIO_AF(12,  5U)	| \
							 PIN_AFIO_AF(13,  5U)	| \
							 PIN_AFIO_AF(14,  5U)	| \
							 PIN_AFIO_AF(15,  0U))

/*
 * GPIOF setup:
 *
 * PF0  - Ignition (2, 4, 6, 8) MAXI input
 * PF1  - SPI5 CS0
 * PF2  - SPI5 CS1
 * PF3  - optional analog input 3
 * PF4  - optional analog input 2
 * PF5  - optional analog input 1
 * PF6  - optional analog input 0
 * PF7  - SPI5 SCK
 * PF8  - SPI5 MISO
 * PF9  - SPI5 MOSI
 * PF10 - QSPI CLK
 * PF11 - SPI5 CS4
 * PF12 - SPI5 CS5
 * PF13 - n.u.
 * PF14 - SPI5 CS2
 * PF15 - SPI5 CS3
 */
#define VAL_GPIOF_MODER     (PIN_MODE_INPUT(0)		| \
							 PIN_MODE_OUTPUT(1)		| \
							 PIN_MODE_OUTPUT(2)		| \
							 PIN_MODE_ANALOG(3)		| \
							 PIN_MODE_ANALOG(4)		| \
							 PIN_MODE_ANALOG(5)		| \
							 PIN_MODE_ANALOG(6)		| \
							 PIN_MODE_ALTERNATE(7)	| \
							 PIN_MODE_ALTERNATE(8)	| \
							 PIN_MODE_ALTERNATE(9)	| \
							 PIN_MODE_ALTERNATE(10)	| \
							 PIN_MODE_OUTPUT(11)	| \
							 PIN_MODE_OUTPUT(12)	| \
							 PIN_MODE_OUTPUT(13)	| \
							 PIN_MODE_OUTPUT(14)	| \
							 PIN_MODE_OUTPUT(15))
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
							 PIN_OTYPE_PUSHPULL(14) | \
							 PIN_OTYPE_PUSHPULL(15))
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
#define VAL_GPIOF_PUPDR		(PIN_PUPDR_PULLUP( 0)	| \
							 PIN_PUPDR_FLOATING( 1)	| \
							 PIN_PUPDR_FLOATING( 2)	| \
							 PIN_PUPDR_FLOATING( 3)	| \
							 PIN_PUPDR_FLOATING( 4)	| \
							 PIN_PUPDR_FLOATING( 5)	| \
							 PIN_PUPDR_FLOATING( 6)	| \
							 PIN_PUPDR_FLOATING( 7)	| \
							 PIN_PUPDR_PULLUP( 8)	| \
							 PIN_PUPDR_FLOATING( 9)	| \
							 PIN_PUPDR_FLOATING(10)	| \
							 PIN_PUPDR_FLOATING(11)	| \
							 PIN_PUPDR_FLOATING(12)	| \
							 PIN_PUPDR_FLOATING(13)	| \
							 PIN_PUPDR_FLOATING(14)	| \
							 PIN_PUPDR_FLOATING(15))
#define VAL_GPIOF_ODR		(PIN_ODR_HIGH(1)		| \
							 PIN_ODR_HIGH(2)		| \
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
							 PIN_AFIO_AF( 7,  5U))
#define VAL_GPIOF_AFRH		(PIN_AFIO_AF( 8,  5U)	| \
							 PIN_AFIO_AF( 9,  5U)	| \
							 PIN_AFIO_AF(10,  9U)	| \
							 PIN_AFIO_AF(11,  0U)	| \
							 PIN_AFIO_AF(12,  0U)	| \
							 PIN_AFIO_AF(13,  0U)	| \
							 PIN_AFIO_AF(14,  0U)	| \
							 PIN_AFIO_AF(15,  0U))

/*
 * GPIOG setup:
 *
 * PG0  - CAN1 EN
 * PG1  - SW2 irq
 * PG2  - taho out
 * PG3  - 1 wire interface
 * PG4  - mc33972 irq
 * PG5  - mc33972 wake up input
 * PG6  - LD2 - active low
 * PG7  - LD1 - active low
 * PG8  - LD0 - active low
 * PG9  - MMC D0
 * PG10 - MMC D1
 * PG11 - MMC D2
 * PG12 - MMC D3
 * PG13 - ignition (1, 3, 5, 7) MAXI input
 * PG14 - E-Throttle disable
 * PG15 - ignition (1, 3, 5, 7) NOMI input
 */
#define VAL_GPIOG_MODER     (PIN_MODE_OUTPUT(0)		| \
							 PIN_MODE_INPUT(1)		| \
							 PIN_MODE_OUTPUT(2)		| \
							 PIN_MODE_OUTPUT(3)		| \
							 PIN_MODE_INPUT(4)		| \
							 PIN_MODE_INPUT(5)		| \
							 PIN_MODE_OUTPUT(6)		| \
							 PIN_MODE_OUTPUT(7)		| \
							 PIN_MODE_OUTPUT(8)		| \
							 PIN_MODE_ALTERNATE(9)	| \
							 PIN_MODE_ALTERNATE(10)	| \
							 PIN_MODE_ALTERNATE(11)	| \
							 PIN_MODE_ALTERNATE(12)	| \
							 PIN_MODE_INPUT(13)		| \
							 PIN_MODE_OUTPUT(14)	| \
							 PIN_MODE_INPUT(15))
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
							 PIN_OTYPE_PUSHPULL(14)| \
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
							 PIN_PUPDR_PULLUP( 4)	| \
							 PIN_PUPDR_PULLUP( 5)	| \
							 PIN_PUPDR_FLOATING( 6)	| \
							 PIN_PUPDR_FLOATING( 7)	| \
							 PIN_PUPDR_FLOATING( 8)	| \
							 PIN_PUPDR_PULLUP( 9)	| \
							 PIN_PUPDR_PULLUP(10)	| \
							 PIN_PUPDR_PULLUP(11)	| \
							 PIN_PUPDR_PULLUP(12)	| \
							 PIN_PUPDR_PULLUP(13)	| \
							 PIN_PUPDR_FLOATING(14)	| \
							 PIN_PUPDR_PULLUP(15))
#define VAL_GPIOG_ODR		(PIN_ODR_HIGH(0)		| \
							 PIN_ODR_LOW(2)			| \
							 PIN_ODR_HIGH(3)		| \
							 PIN_ODR_HIGH(6)		| \
							 PIN_ODR_HIGH(7)		| \
							 PIN_ODR_HIGH(8)		| \
							 PIN_ODR_HIGH(14))
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
							 PIN_AFIO_AF(14,  0U)	| \
							 PIN_AFIO_AF(15,  0U))

/*
 * GPIOH setup:
 *
 * PH0  - OSC_IN                    (input floating).
 * PH1  - OSC_OUT                   (input floating).
 * PH2  - n.u.
 * PH3  - n.u.
 * PH4  - n.u.
 * PH5  - n.u.
 * PH6  - SW reset
 * PH7  - self shutdown output
 * PH8  - knock sensor IC hold output (5V pulled)
 * PH9  - knock sensor IC tst output (5V pulled)
 * PH10 - Crank position #1
 * PH11 - watchdog
 * PH12 - Cam positio #1
 * PH13 - USB FS overcurrent input
 * PH14 - USB FS power enable
 * PH15 - OLED DC output
 */
#define VAL_GPIOH_MODER		(PIN_MODE_INPUT(0)		| \
							 PIN_MODE_INPUT(1)		| \
							 PIN_MODE_OUTPUT(2)		| \
							 PIN_MODE_OUTPUT(3)		| \
							 PIN_MODE_OUTPUT(4)		| \
							 PIN_MODE_OUTPUT(5)		| \
							 PIN_MODE_OUTPUT(6)		| \
							 PIN_MODE_OUTPUT(7)		| \
							 PIN_MODE_OUTPUT(8)		| \
							 PIN_MODE_OUTPUT(9)		| \
							 PIN_MODE_INPUT(10)		| \
							 PIN_MODE_OUTPUT(11)	| \
							 PIN_MODE_INPUT(12)		| \
							 PIN_MODE_INPUT(13)		| \
							 PIN_MODE_OUTPUT(14)	| \
							 PIN_MODE_OUTPUT(15))
#define VAL_GPIOH_OTYPER	(PIN_OTYPE_PUSHPULL( 0)	| \
							 PIN_OTYPE_PUSHPULL( 1)	| \
							 PIN_OTYPE_PUSHPULL( 2)	| \
							 PIN_OTYPE_PUSHPULL( 3)	| \
							 PIN_OTYPE_PUSHPULL( 4)	| \
							 PIN_OTYPE_PUSHPULL( 5)	| \
							 PIN_OTYPE_PUSHPULL( 6)	| \
							 PIN_OTYPE_PUSHPULL( 7)	| \
							 PIN_OTYPE_OPENDRAIN( 8)| \
							 PIN_OTYPE_OPENDRAIN( 9)| \
							 PIN_OTYPE_PUSHPULL(10)	| \
							 PIN_OTYPE_PUSHPULL(11)	| \
							 PIN_OTYPE_PUSHPULL(12)	| \
							 PIN_OTYPE_PUSHPULL(13)	| \
							 PIN_OTYPE_PUSHPULL(14)	| \
							 PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOH_OSPEEDR	(DEFAULT_GPIO_SPEED( 0)	| \
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
#define VAL_GPIOH_PUPDR		(PIN_PUPDR_FLOATING( 0)	| \
							 PIN_PUPDR_FLOATING( 1)	| \
							 PIN_PUPDR_FLOATING( 2)	| \
							 PIN_PUPDR_FLOATING( 3)	| \
							 PIN_PUPDR_FLOATING( 4)	| \
							 PIN_PUPDR_FLOATING( 5)	| \
							 PIN_PUPDR_FLOATING( 6)	| \
							 PIN_PUPDR_FLOATING( 7)	| \
							 PIN_PUPDR_FLOATING( 8)	| \
							 PIN_PUPDR_FLOATING( 9)	| \
							 PIN_PUPDR_PULLUP(10)	| \
							 PIN_PUPDR_FLOATING(11)	| \
							 PIN_PUPDR_PULLUP(12)	| \
							 PIN_PUPDR_PULLUP(13)	| \
							 PIN_PUPDR_FLOATING(14)	| \
							 PIN_PUPDR_FLOATING(15))
#define VAL_GPIOH_ODR 		(PIN_ODR_HIGH( 0)		| \
							 PIN_ODR_HIGH( 1)		| \
							 PIN_ODR_LOW( 2)		| \
							 PIN_ODR_LOW( 3)		| \
							 PIN_ODR_LOW( 4)		| \
							 PIN_ODR_LOW( 5)		| \
							 PIN_ODR_LOW( 6)		| \
							 PIN_ODR_LOW( 7)		| \
							 PIN_ODR_HIGH( 8)		| \
							 PIN_ODR_HIGH( 9)		| \
							 PIN_ODR_LOW(11)		| \
							 PIN_ODR_LOW(14)		| \
							 PIN_ODR_LOW(15))
#define VAL_GPIOH_AFRL 		(PIN_AFIO_AF( 0,  0U)	| \
							 PIN_AFIO_AF( 1,  0U)	| \
							 PIN_AFIO_AF( 2,  0U)	| \
							 PIN_AFIO_AF( 3,  0U)	| \
							 PIN_AFIO_AF( 4,  0U)	| \
							 PIN_AFIO_AF( 5,  0U)	| \
							 PIN_AFIO_AF( 6,  0U)	| \
							 PIN_AFIO_AF( 7,  0U))
#define VAL_GPIOH_AFRH		(PIN_AFIO_AF( 8,  0U)	| \
							 PIN_AFIO_AF( 9,  0U)	| \
							 PIN_AFIO_AF(10,  0U)	| \
							 PIN_AFIO_AF(11,  0U)	| \
							 PIN_AFIO_AF(12,  0U)	| \
							 PIN_AFIO_AF(13,  0U)	| \
							 PIN_AFIO_AF(14,  0U)	| \
							 PIN_AFIO_AF(15,  0U))

/*
 * GPIOI setup:
 *
 * PI0  - SPI2 CS0
 * PI1  - SPI2 SCK
 * PI2  - SPI2 MISO
 * PI3  - SPI2 MOSI
 * PI4  - injector #5
 * PI5  - injector #3
 * PI6  - injector #1
 * PI7  - SW enable
 * PI8  - ignition (2, 4, 6, 8) spark duration input
 * PI9  - ignition #8
 * PI10 - n.u.
 * PI11 - ignition (2, 4, 6, 8) NOMI input
 */
#define VAL_GPIOI_MODER		(PIN_MODE_OUTPUT(0)		| \
							 PIN_MODE_ALTERNATE(1)	| \
							 PIN_MODE_ALTERNATE(2)	| \
							 PIN_MODE_ALTERNATE(3)	| \
							 PIN_MODE_OUTPUT(4)		| \
							 PIN_MODE_OUTPUT(5)		| \
							 PIN_MODE_OUTPUT(6)		| \
							 PIN_MODE_OUTPUT(7)		| \
							 PIN_MODE_INPUT(8)		| \
							 PIN_MODE_OUTPUT(9)		| \
							 PIN_MODE_OUTPUT(10)	| \
							 PIN_MODE_INPUT(11))
#define VAL_GPIOI_OTYPER	(PIN_OTYPE_PUSHPULL( 0)	| \
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
							 PIN_OTYPE_PUSHPULL(11))
#define VAL_GPIOI_OSPEEDR	(DEFAULT_GPIO_SPEED( 0)	| \
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
							 DEFAULT_GPIO_SPEED(11))
#define VAL_GPIOI_PUPDR		(PIN_PUPDR_FLOATING( 0)	| \
							 PIN_PUPDR_FLOATING( 1)	| \
							 PIN_PUPDR_FLOATING( 2)	| \
							 PIN_PUPDR_FLOATING( 3)	| \
							 PIN_PUPDR_FLOATING( 4)	| \
							 PIN_PUPDR_FLOATING( 5)	| \
							 PIN_PUPDR_FLOATING( 6)	| \
							 PIN_PUPDR_FLOATING( 7)	| \
							 PIN_PUPDR_PULLUP( 8)	| \
							 PIN_PUPDR_FLOATING( 9)	| \
							 PIN_PUPDR_FLOATING(10)	| \
							 PIN_PUPDR_PULLUP(11))
#define VAL_GPIOI_ODR 		(PIN_ODR_HIGH( 0)		| \
							 PIN_ODR_LOW( 4)		| \
							 PIN_ODR_LOW( 5)		| \
							 PIN_ODR_LOW( 6)		| \
							 PIN_ODR_LOW( 7)		| \
							 PIN_ODR_LOW( 9))
#define VAL_GPIOI_AFRL 		(PIN_AFIO_AF( 0,  0U)	| \
							 PIN_AFIO_AF( 1,  5U)	| \
							 PIN_AFIO_AF( 2,  5U)	| \
							 PIN_AFIO_AF( 3,  5U)	| \
							 PIN_AFIO_AF( 4,  0U)	| \
							 PIN_AFIO_AF( 5,  0U)	| \
							 PIN_AFIO_AF( 6,  0U)	| \
							 PIN_AFIO_AF( 7,  0U))
#define VAL_GPIOI_AFRH		(PIN_AFIO_AF( 8,  0U)	| \
							 PIN_AFIO_AF( 9,  0U)	| \
							 PIN_AFIO_AF(10,  0U)	| \
							 PIN_AFIO_AF(11,  0U))

#endif /* BOARD_IO_H */
