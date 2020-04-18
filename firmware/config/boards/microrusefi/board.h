/**
 * @file boards/microrusefi/board.h
 *
 *
 * @brief Low level configuration defaults for the microRusefi board
 *
 * @author Andrey Gusakov, (c) 2020
 */

#ifndef BOARD_H
#define BOARD_H

/*
 * Setup for MicroRusEFI F4/F7 board.
 */

#define EFI_USB_AF 10U
#define EFI_USB_SERIAL_ID GPIOA_10
#define EFI_USB_SERIAL_DM GPIOA_11
#define EFI_USB_SERIAL_DP GPIOA_12

// Ignore USB VBUS pin (we're never a host, only a device)
#define BOARD_OTG_NOVBUSSENS TRUE

/*
 * input-floating is the default pin mode. input-output boards should provision appropriate pull-ups/pull-downs.
 */
#define EFI_PIN_MODE_DEFAULT PIN_MODE_INPUT
#define EFI_DR_DEFAULT PIN_PUPDR_FLOATING

/*
 * Board oscillators-related settings.
 * NOTE: LSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                32768U
#endif

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                8000000U
#endif

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   300U

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
#define DEFAULT_GPIO_SPEED PIN_OSPEED_HIGH

/*
 * GPIOA setup:
 *
 * PA0  - AT1_MCU                   (analog).
 * PA1  - AT2_MCU                   (analog).
 * PA2  - AT3_MCU                   (analog).
 * PA3  - AT4_MCU                   (analog).
 * PA4  - AV10_MCU                  (analog).
 * PA5  - CAM_MCU                   (input floating).
 * PA6  - AV2_MCU                   (analog).
 * PA7  - AV3_MCU                   (analog).
 * PA8  - DIR                       (input floating).
 * PA9  - VBUS_FS                   (input floating).
 * PA10 - OTG_FS_ID                 (alternate 10).
 * PA11 - OTG_FS_DM                 (alternate 10).
 * PA12 - OTG_FS_DP                 (alternate 10).
 * PA13 - SWDIO                     (alternate 0).
 * PA14 - SWCLK                     (alternate 0).
 * PA15 - PA15 (J4)                 (input pulldown).
 */
#define VAL_GPIOA_MODER             (PIN_MODE_ANALOG(0) | \
                                     PIN_MODE_ANALOG(1) | \
                                     PIN_MODE_ANALOG(2) | \
                                     PIN_MODE_ANALOG(3) | \
                                     PIN_MODE_ANALOG(4) | \
                                     PIN_MODE_INPUT(5) | \
                                     PIN_MODE_ANALOG(6) | \
                                     PIN_MODE_ANALOG(7) | \
                                     PIN_MODE_INPUT(8) | \
                                     PIN_MODE_INPUT(9) | \
                                     PIN_MODE_ALTERNATE(10) | \
                                     PIN_MODE_ALTERNATE(11) | \
                                     PIN_MODE_ALTERNATE(12) | \
                                     PIN_MODE_ALTERNATE(13) | \
                                     PIN_MODE_ALTERNATE(14) | \
                                     PIN_MODE_INPUT(15))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(0) | \
                                     PIN_OTYPE_PUSHPULL(1) | \
                                     PIN_OTYPE_PUSHPULL(2) | \
                                     PIN_OTYPE_PUSHPULL(3) | \
                                     PIN_OTYPE_PUSHPULL(4) | \
                                     PIN_OTYPE_PUSHPULL(5) | \
                                     PIN_OTYPE_PUSHPULL(6) | \
                                     PIN_OTYPE_PUSHPULL(7) | \
                                     PIN_OTYPE_PUSHPULL(8) | \
                                     PIN_OTYPE_PUSHPULL(9) | \
                                     PIN_OTYPE_PUSHPULL(10) | \
                                     PIN_OTYPE_PUSHPULL(11) | \
                                     PIN_OTYPE_PUSHPULL(12) | \
                                     PIN_OTYPE_PUSHPULL(13) | \
                                     PIN_OTYPE_PUSHPULL(14) | \
                                     PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOA_OSPEEDR           (DEFAULT_GPIO_SPEED(0) | \
                                     DEFAULT_GPIO_SPEED(1) | \
                                     DEFAULT_GPIO_SPEED(2) | \
                                     DEFAULT_GPIO_SPEED(3) | \
                                     DEFAULT_GPIO_SPEED(4) | \
                                     DEFAULT_GPIO_SPEED(5) | \
                                     DEFAULT_GPIO_SPEED(6) | \
                                     DEFAULT_GPIO_SPEED(7) | \
                                     DEFAULT_GPIO_SPEED(8) | \
                                     DEFAULT_GPIO_SPEED(9) | \
                                     DEFAULT_GPIO_SPEED(10) | \
                                     DEFAULT_GPIO_SPEED(11) | \
                                     DEFAULT_GPIO_SPEED(12) | \
                                     DEFAULT_GPIO_SPEED(13) | \
                                     DEFAULT_GPIO_SPEED(14) | \
                                     DEFAULT_GPIO_SPEED(15))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_FLOATING(0) | \
                                     PIN_PUPDR_FLOATING(1) | \
                                     PIN_PUPDR_FLOATING(2) | \
                                     PIN_PUPDR_FLOATING(3) | \
                                     PIN_PUPDR_FLOATING(4) | \
                                     PIN_PUPDR_FLOATING(5) | \
                                     PIN_PUPDR_FLOATING(6) | \
                                     PIN_PUPDR_FLOATING(7) | \
                                     PIN_PUPDR_FLOATING(8) | \
                                     PIN_PUPDR_FLOATING(9) | \
                                     PIN_PUPDR_FLOATING(10) | \
                                     PIN_PUPDR_FLOATING(11) | \
                                     PIN_PUPDR_FLOATING(12) | \
                                     PIN_PUPDR_FLOATING(13) | \
                                     PIN_PUPDR_FLOATING(14) | \
                                     PIN_PUPDR_PULLDOWN(15))
#define VAL_GPIOA_ODR               (PIN_ODR_LOW(0) | \
                                     PIN_ODR_LOW(1) | \
                                     PIN_ODR_LOW(2) | \
                                     PIN_ODR_LOW(3) | \
                                     PIN_ODR_LOW(4) | \
                                     PIN_ODR_LOW(5) | \
                                     PIN_ODR_LOW(6) | \
                                     PIN_ODR_LOW(7) | \
                                     PIN_ODR_LOW(8) | \
                                     PIN_ODR_LOW(9) | \
                                     PIN_ODR_LOW(10) | \
                                     PIN_ODR_LOW(11) | \
                                     PIN_ODR_LOW(12) | \
                                     PIN_ODR_LOW(13) | \
                                     PIN_ODR_LOW(14) | \
                                     PIN_ODR_LOW(15))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(0, 0U) | \
                                     PIN_AFIO_AF(1, 0U) | \
                                     PIN_AFIO_AF(2, 0U) | \
                                     PIN_AFIO_AF(3, 0U) | \
                                     PIN_AFIO_AF(4, 0U) | \
                                     PIN_AFIO_AF(5, 0U) | \
                                     PIN_AFIO_AF(6, 0U) | \
                                     PIN_AFIO_AF(7, 0U))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(8, 0U) | \
                                     PIN_AFIO_AF(9, 0U) | \
                                     PIN_AFIO_AF(10, 10U) | \
                                     PIN_AFIO_AF(11, 10U) | \
                                     PIN_AFIO_AF(12, 10U) | \
                                     PIN_AFIO_AF(13, 0U) | \
                                     PIN_AFIO_AF(14, 0U) | \
                                     PIN_AFIO_AF(15, 0U))

/*
 * GPIOB setup:
 *
 * PB0  - AV8_MCU                   (analog).
 * PB1  - AV9_MCU                   (analog).
 * PB2  - BOOT1 - grounded          (input floating).
 * PB3  - SPI3 SCK                  (alternate 6).
 * PB4  - SPI3 MISO                 (alternate 6, pullup).
 * PB5  - SPI3 MOSI                 (alternate 6).
 * PB6  - CAN2 TX                   (alternate 9).
 * PB7  - J8                        (input pulldown).
 * PB8  - J4                        (input pulldown).
 * PB9  - J4                        (input pulldown).
 * PB10 - WP                        (input pulldown).
 * PB11 - HOLD                      (input pullup).
 * PB12 - CAN2 RX                   (alternate 9, pullup).
 * PB13 - SPI2 SCK                  (alternate 5).
 * PB14 - SPI2 MISO                 (alternate 6, pullup).
 * PB15 - SPI2 MOSI                 (alternate 6).
 */
#define VAL_GPIOB_MODER             (PIN_MODE_ANALOG(0) | \
                                     PIN_MODE_ANALOG(1) | \
                                     PIN_MODE_INPUT(2) | \
                                     PIN_MODE_ALTERNATE(3) | \
                                     PIN_MODE_ALTERNATE(4) | \
                                     PIN_MODE_ALTERNATE(5) | \
                                     PIN_MODE_ALTERNATE(6) | \
                                     PIN_MODE_INPUT(7) | \
                                     PIN_MODE_INPUT(8) | \
                                     PIN_MODE_INPUT(9) | \
                                     PIN_MODE_INPUT(10) | \
                                     PIN_MODE_INPUT(11) | \
                                     PIN_MODE_ALTERNATE(12) | \
                                     PIN_MODE_ALTERNATE(13) | \
                                     PIN_MODE_ALTERNATE(14) | \
                                     PIN_MODE_ALTERNATE(15))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(0) | \
                                     PIN_OTYPE_PUSHPULL(1) | \
                                     PIN_OTYPE_PUSHPULL(2) | \
                                     PIN_OTYPE_PUSHPULL(3) | \
                                     PIN_OTYPE_PUSHPULL(4) | \
                                     PIN_OTYPE_PUSHPULL(5) | \
                                     PIN_OTYPE_PUSHPULL(6) | \
                                     PIN_OTYPE_PUSHPULL(7) | \
                                     PIN_OTYPE_PUSHPULL(8) | \
                                     PIN_OTYPE_PUSHPULL(9) | \
                                     PIN_OTYPE_PUSHPULL(10) | \
                                     PIN_OTYPE_PUSHPULL(11) | \
                                     PIN_OTYPE_PUSHPULL(12) | \
                                     PIN_OTYPE_PUSHPULL(13) | \
                                     PIN_OTYPE_PUSHPULL(14) | \
                                     PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOB_OSPEEDR           (DEFAULT_GPIO_SPEED(0) | \
                                     DEFAULT_GPIO_SPEED(1) | \
                                     DEFAULT_GPIO_SPEED(2) | \
                                     DEFAULT_GPIO_SPEED(3) | \
                                     DEFAULT_GPIO_SPEED(4) | \
                                     DEFAULT_GPIO_SPEED(5) | \
                                     DEFAULT_GPIO_SPEED(6) | \
                                     DEFAULT_GPIO_SPEED(7) | \
                                     DEFAULT_GPIO_SPEED(8) | \
                                     DEFAULT_GPIO_SPEED(9) | \
                                     DEFAULT_GPIO_SPEED(10) | \
                                     DEFAULT_GPIO_SPEED(11) | \
                                     DEFAULT_GPIO_SPEED(12) | \
                                     DEFAULT_GPIO_SPEED(13) | \
                                     DEFAULT_GPIO_SPEED(14) | \
                                     DEFAULT_GPIO_SPEED(15))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_FLOATING(0) | \
                                     PIN_PUPDR_FLOATING(1) | \
                                     PIN_PUPDR_FLOATING(2) | \
                                     PIN_PUPDR_FLOATING(3) | \
                                     PIN_PUPDR_PULLUP(4) | \
                                     PIN_PUPDR_FLOATING(5) | \
                                     PIN_PUPDR_FLOATING(6) | \
                                     PIN_PUPDR_PULLDOWN(7) | \
                                     PIN_PUPDR_PULLDOWN(8) | \
                                     PIN_PUPDR_PULLDOWN(9) | \
                                     PIN_PUPDR_PULLDOWN(10) | \
                                     PIN_PUPDR_PULLUP(11) | \
                                     PIN_PUPDR_PULLUP(12) | \
                                     PIN_PUPDR_FLOATING(13) | \
                                     PIN_PUPDR_PULLUP(14) | \
                                     PIN_PUPDR_FLOATING(15))
#define VAL_GPIOB_ODR               (PIN_ODR_HIGH(0) | \
                                     PIN_ODR_HIGH(1) | \
                                     PIN_ODR_HIGH(2) | \
                                     PIN_ODR_HIGH(3) | \
                                     PIN_ODR_HIGH(4) | \
                                     PIN_ODR_HIGH(5) | \
                                     PIN_ODR_HIGH(6) | \
                                     PIN_ODR_HIGH(7) | \
                                     PIN_ODR_HIGH(8) | \
                                     PIN_ODR_HIGH(9) | \
                                     PIN_ODR_HIGH(10) | \
                                     PIN_ODR_HIGH(11) | \
                                     PIN_ODR_HIGH(12) | \
                                     PIN_ODR_HIGH(13) | \
                                     PIN_ODR_HIGH(14) | \
                                     PIN_ODR_HIGH(15))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(0, 0U) | \
                                     PIN_AFIO_AF(1, 0U) | \
                                     PIN_AFIO_AF(2, 0U) | \
                                     PIN_AFIO_AF(3, 0U) | \
                                     PIN_AFIO_AF(4, 0U) | \
                                     PIN_AFIO_AF(5, 0U) | \
                                     PIN_AFIO_AF(6, 4U) | \
                                     PIN_AFIO_AF(7, 0U))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(8, 0U) | \
                                     PIN_AFIO_AF(9, 0U) | \
                                     PIN_AFIO_AF(10, 0U) | \
                                     PIN_AFIO_AF(11, 0U) | \
                                     PIN_AFIO_AF(12, 0U) | \
                                     PIN_AFIO_AF(13, 0U) | \
                                     PIN_AFIO_AF(14, 0U) | \
                                     PIN_AFIO_AF(15, 0U))

/*
 * GPIOC setup:
 *
 * PC0  - AV1_MCU                   (analog).
 * PC1  - Vbat_MCU                  (analog).
 * PC2  - AV4_MCU                   (analog).
 * PC3  - AV5_MCU                   (analog).
 * PC4  - AV6_MCU                   (analog).
 * PC5  - AV7_MCU                   (analog).
 * PC6  - CRANK                     (input floating).
 * PC7  - PWM                       (input floating).
 * PC8  - DIS                       (input floating).
 * PC9  - SO (ETB diagnostic)       (input pullup).
 * PC10 - J4                        (input pulldown).
 * PC11 - J4                        (input pulldown).
 * PC12 - J4                        (input pulldown).
 * PC13 - J9                        (input pulldown).
 * PC14 - 32768 OSC                 (input floating).
 * PC15 - 32768 OSC                 (input floating).
 */
#define VAL_GPIOC_MODER             (PIN_MODE_ANALOG(0) | \
                                     PIN_MODE_ANALOG(1) | \
                                     PIN_MODE_ANALOG(2) | \
                                     PIN_MODE_ANALOG(3) | \
                                     PIN_MODE_ANALOG(4) | \
                                     PIN_MODE_ANALOG(5) | \
                                     PIN_MODE_INPUT(6) | \
                                     PIN_MODE_INPUT(7) | \
                                     PIN_MODE_INPUT(8) | \
                                     PIN_MODE_INPUT(9) | \
                                     PIN_MODE_INPUT(10) | \
                                     PIN_MODE_INPUT(11) | \
                                     PIN_MODE_INPUT(12) | \
                                     PIN_MODE_INPUT(13) | \
                                     PIN_MODE_INPUT(14) | \
                                     PIN_MODE_INPUT(15))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(0) |\
                                     PIN_OTYPE_PUSHPULL(1) | \
                                     PIN_OTYPE_PUSHPULL(2) | \
                                     PIN_OTYPE_PUSHPULL(3) | \
                                     PIN_OTYPE_PUSHPULL(4) | \
                                     PIN_OTYPE_PUSHPULL(5) | \
                                     PIN_OTYPE_PUSHPULL(6) | \
                                     PIN_OTYPE_PUSHPULL(7) | \
                                     PIN_OTYPE_PUSHPULL(8) | \
                                     PIN_OTYPE_PUSHPULL(9) | \
                                     PIN_OTYPE_PUSHPULL(10) | \
                                     PIN_OTYPE_PUSHPULL(11) | \
                                     PIN_OTYPE_PUSHPULL(10) | \
                                     PIN_OTYPE_PUSHPULL(13) | \
                                     PIN_OTYPE_PUSHPULL(14) | \
                                     PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOC_OSPEEDR           (DEFAULT_GPIO_SPEED(0) | \
                                     DEFAULT_GPIO_SPEED(1) | \
                                     DEFAULT_GPIO_SPEED(2) | \
                                     DEFAULT_GPIO_SPEED(3) | \
                                     DEFAULT_GPIO_SPEED(4) | \
                                     DEFAULT_GPIO_SPEED(5) | \
                                     DEFAULT_GPIO_SPEED(6) | \
                                     DEFAULT_GPIO_SPEED(7) | \
                                     DEFAULT_GPIO_SPEED(8) | \
                                     DEFAULT_GPIO_SPEED(9) | \
                                     DEFAULT_GPIO_SPEED(10) | \
                                     DEFAULT_GPIO_SPEED(11) | \
                                     DEFAULT_GPIO_SPEED(12) | \
                                     DEFAULT_GPIO_SPEED(13) | \
                                     DEFAULT_GPIO_SPEED(14) | \
                                     DEFAULT_GPIO_SPEED(15))
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_FLOATING(0) | \
                                     PIN_PUPDR_FLOATING(1) | \
                                     PIN_PUPDR_FLOATING(2) | \
                                     PIN_PUPDR_FLOATING(3) | \
                                     PIN_PUPDR_FLOATING(4) | \
                                     PIN_PUPDR_FLOATING(5) | \
                                     PIN_PUPDR_FLOATING(6) | \
                                     PIN_PUPDR_FLOATING(7) | \
                                     PIN_PUPDR_FLOATING(8) | \
                                     PIN_PUPDR_PULLUP(9) | \
                                     PIN_PUPDR_PULLDOWN(10) | \
                                     PIN_PUPDR_PULLDOWN(11) | \
                                     PIN_PUPDR_PULLDOWN(12) | \
                                     PIN_PUPDR_PULLDOWN(13) | \
                                     PIN_PUPDR_FLOATING(14) | \
                                     PIN_PUPDR_FLOATING(15))
#define VAL_GPIOC_ODR               (PIN_ODR_HIGH(0) | \
                                     PIN_ODR_HIGH(1) | \
                                     PIN_ODR_HIGH(2) | \
                                     PIN_ODR_HIGH(3) | \
                                     PIN_ODR_HIGH(4) | \
                                     PIN_ODR_HIGH(5) | \
                                     PIN_ODR_HIGH(6) | \
                                     PIN_ODR_HIGH(7) | \
                                     PIN_ODR_HIGH(8) | \
                                     PIN_ODR_HIGH(9) | \
                                     PIN_ODR_HIGH(10) | \
                                     PIN_ODR_HIGH(11) | \
                                     PIN_ODR_HIGH(12) | \
                                     PIN_ODR_HIGH(13) | \
                                     PIN_ODR_HIGH(14) | \
                                     PIN_ODR_HIGH(15))
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(0, 0U) |\
                                     PIN_AFIO_AF(1, 0U) | \
                                     PIN_AFIO_AF(2, 0U) | \
                                     PIN_AFIO_AF(3, 0U) | \
                                     PIN_AFIO_AF(4, 0U) | \
                                     PIN_AFIO_AF(5, 0U) | \
                                     PIN_AFIO_AF(6, 0U) | \
                                     PIN_AFIO_AF(7, 0U))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(8, 0U) | \
                                     PIN_AFIO_AF(9, 0U) | \
                                     PIN_AFIO_AF(10, 0U) | \
                                     PIN_AFIO_AF(11, 0U) | \
                                     PIN_AFIO_AF(12, 0U) | \
                                     PIN_AFIO_AF(13, 0U) | \
                                     PIN_AFIO_AF(14, 0U) | \
                                     PIN_AFIO_AF(15, 0U))

/*
 * GPIOD setup:
 *
 * PD0  - J16                       (input pulldown).
 * PD1  - IGN4                      (input floating).
 * PD2  - IGN3                      (input floating).
 * PD3  - IGN2                      (input floating).
 * PD4  - IGN1                      (input floating).
 * PD5  - CS TLE                    (input pullup).
 * PD6  - GP6                       (input floating).
 * PD7  - GP6                       (input floating).
 * PD8  - LIN TX                    (alternate floating).
 * PD9  - LIN RX                    (alternate pullup).
 * PD10 - J18                       (input pulldown).
 * PD11 - J17                       (input pulldown).
 * PD12 - lowside AV6               (input floating).
 * PD13 - lowside AV7               (input floating).
 * PD14 - lowside AV8               (input floating).
 * PD15 - lowside AV9               (input floating).
 */
#define VAL_GPIOD_MODER             (PIN_MODE_INPUT(0) | \
                                     PIN_MODE_INPUT(1) | \
                                     PIN_MODE_INPUT(2) | \
                                     PIN_MODE_INPUT(3) | \
                                     PIN_MODE_INPUT(4) | \
                                     PIN_MODE_INPUT(5) | \
                                     PIN_MODE_INPUT(6) | \
                                     PIN_MODE_INPUT(7) | \
                                     PIN_MODE_ALTERNATE(8) | \
                                     PIN_MODE_ALTERNATE(9) | \
                                     PIN_MODE_INPUT(10) | \
                                     PIN_MODE_INPUT(11) | \
                                     PIN_MODE_INPUT(12) | \
                                     PIN_MODE_INPUT(13) | \
                                     PIN_MODE_INPUT(14) | \
                                     PIN_MODE_INPUT(15))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(0) | \
                                     PIN_OTYPE_PUSHPULL(1) | \
                                     PIN_OTYPE_PUSHPULL(2) | \
                                     PIN_OTYPE_PUSHPULL(3) | \
                                     PIN_OTYPE_PUSHPULL(4) | \
                                     PIN_OTYPE_PUSHPULL(5) | \
                                     PIN_OTYPE_PUSHPULL(6) | \
                                     PIN_OTYPE_PUSHPULL(7) | \
                                     PIN_OTYPE_PUSHPULL(8) | \
                                     PIN_OTYPE_PUSHPULL(9) | \
                                     PIN_OTYPE_PUSHPULL(10) | \
                                     PIN_OTYPE_PUSHPULL(11) | \
                                     PIN_OTYPE_PUSHPULL(10) | \
                                     PIN_OTYPE_PUSHPULL(13) | \
                                     PIN_OTYPE_PUSHPULL(14) | \
                                     PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOD_OSPEEDR           (DEFAULT_GPIO_SPEED(0) | \
                                     DEFAULT_GPIO_SPEED(1) | \
                                     DEFAULT_GPIO_SPEED(2) | \
                                     DEFAULT_GPIO_SPEED(3) | \
                                     DEFAULT_GPIO_SPEED(4) | \
                                     DEFAULT_GPIO_SPEED(5) | \
                                     DEFAULT_GPIO_SPEED(6) | \
                                     DEFAULT_GPIO_SPEED(7) | \
                                     DEFAULT_GPIO_SPEED(8) | \
                                     DEFAULT_GPIO_SPEED(9) | \
                                     DEFAULT_GPIO_SPEED(10) | \
                                     DEFAULT_GPIO_SPEED(11) | \
                                     DEFAULT_GPIO_SPEED(12) | \
                                     DEFAULT_GPIO_SPEED(13) | \
                                     DEFAULT_GPIO_SPEED(14) | \
                                     DEFAULT_GPIO_SPEED(15))
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_PULLDOWN(0) | \
                                     PIN_PUPDR_FLOATING(1) | \
                                     PIN_PUPDR_FLOATING(2) | \
                                     PIN_PUPDR_FLOATING(3) | \
                                     PIN_PUPDR_FLOATING(4) | \
                                     PIN_PUPDR_PULLUP(5) | \
                                     PIN_PUPDR_FLOATING(6) | \
                                     PIN_PUPDR_FLOATING(7) | \
                                     PIN_PUPDR_FLOATING(8) | \
                                     PIN_PUPDR_PULLUP(9) | \
                                     PIN_PUPDR_PULLDOWN(10) | \
                                     PIN_PUPDR_PULLDOWN(11) | \
                                     PIN_PUPDR_FLOATING(12) | \
                                     PIN_PUPDR_FLOATING(13) | \
                                     PIN_PUPDR_FLOATING(14) | \
                                     PIN_PUPDR_FLOATING(15))
#define VAL_GPIOD_ODR               (PIN_ODR_HIGH(0) | \
                                     PIN_ODR_HIGH(1) | \
                                     PIN_ODR_HIGH(2) | \
                                     PIN_ODR_HIGH(3) | \
                                     PIN_ODR_HIGH(4) | \
                                     PIN_ODR_HIGH(5) | \
                                     PIN_ODR_HIGH(6) | \
                                     PIN_ODR_HIGH(7) | \
                                     PIN_ODR_HIGH(8) | \
                                     PIN_ODR_HIGH(9) | \
                                     PIN_ODR_HIGH(10) | \
                                     PIN_ODR_HIGH(11) | \
                                     PIN_ODR_HIGH(12) | \
                                     PIN_ODR_HIGH(13) | \
                                     PIN_ODR_HIGH(14) | \
                                     PIN_ODR_HIGH(15))
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(0, 0U) |\
                                     PIN_AFIO_AF(1, 0U) | \
                                     PIN_AFIO_AF(2, 0U) | \
                                     PIN_AFIO_AF(3, 0U) | \
                                     PIN_AFIO_AF(4, 0U) | \
                                     PIN_AFIO_AF(5, 0U) | \
                                     PIN_AFIO_AF(6, 0U) | \
                                     PIN_AFIO_AF(7, 0U))
#define VAL_GPIOD_AFRH              (PIN_AFIO_AF(8, 7U) | \
                                     PIN_AFIO_AF(9, 7U) | \
                                     PIN_AFIO_AF(10, 0U) | \
                                     PIN_AFIO_AF(11, 0U) | \
                                     PIN_AFIO_AF(12, 0U) | \
                                     PIN_AFIO_AF(13, 0U) | \
                                     PIN_AFIO_AF(14, 0U) | \
                                     PIN_AFIO_AF(15, 0U))

/*
 * GPIOE setup:
 *
 * PE0  - J6                        (input pulldown).
 * PE1  - LED-YELLOW                (input floating).
 * PE2  - LED-BLUE                  (input floating).
 * PE3  - LED-RED                   (output high) - to indicate hang in early code.
 * PE4  - LED-YELLOW                (input floating).
 * PE5  - J11                       (input pulldown).
 * PE6  - J10                       (input pulldown).
 * PE7  - TLE IN12                  (input pulldown).
 * PE8  - TLE IN11                  (input pulldown).
 * PE9  - TLE IN10                  (input pulldown).
 * PE10 - TLE IN9                   (input pulldown).
 * PE11 - TLE INJ4                  (input pulldown).
 * PE12 - TLE INJ3                  (input pulldown).
 * PE13 - TLE INJ2                  (input pulldown).
 * PE14 - TLE INJ1                  (input pulldown).
 * PE15 - FASH CS                   (input pullup).
 */

#define VAL_GPIOE_MODER             (PIN_MODE_INPUT(0) | \
                                     PIN_MODE_INPUT(1) | \
                                     PIN_MODE_INPUT(2) | \
                                     PIN_MODE_OUTPUT(3) | \
                                     PIN_MODE_INPUT(4) | \
                                     PIN_MODE_INPUT(5) | \
                                     PIN_MODE_INPUT(6) | \
                                     PIN_MODE_INPUT(7) | \
                                     PIN_MODE_INPUT(8) | \
                                     PIN_MODE_INPUT(9) | \
                                     PIN_MODE_INPUT(10) | \
                                     PIN_MODE_INPUT(11) | \
                                     PIN_MODE_INPUT(12) | \
                                     PIN_MODE_INPUT(13) | \
                                     PIN_MODE_INPUT(14) | \
                                     PIN_MODE_INPUT(15))
#define VAL_GPIOE_OTYPER            (PIN_OTYPE_PUSHPULL(0) | \
                                     PIN_OTYPE_PUSHPULL(1) | \
                                     PIN_OTYPE_PUSHPULL(2) | \
                                     PIN_OTYPE_PUSHPULL(3) | \
                                     PIN_OTYPE_PUSHPULL(4) | \
                                     PIN_OTYPE_PUSHPULL(5) | \
                                     PIN_OTYPE_PUSHPULL(6) | \
                                     PIN_OTYPE_PUSHPULL(7) | \
                                     PIN_OTYPE_PUSHPULL(8) | \
                                     PIN_OTYPE_PUSHPULL(9) | \
                                     PIN_OTYPE_PUSHPULL(10) | \
                                     PIN_OTYPE_PUSHPULL(11) | \
                                     PIN_OTYPE_PUSHPULL(10) | \
                                     PIN_OTYPE_PUSHPULL(13) | \
                                     PIN_OTYPE_PUSHPULL(14) | \
                                     PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOE_OSPEEDR           (DEFAULT_GPIO_SPEED(0) | \
                                     DEFAULT_GPIO_SPEED(1) | \
                                     DEFAULT_GPIO_SPEED(2) | \
                                     DEFAULT_GPIO_SPEED(3) | \
                                     DEFAULT_GPIO_SPEED(4) | \
                                     DEFAULT_GPIO_SPEED(5) | \
                                     DEFAULT_GPIO_SPEED(6) | \
                                     DEFAULT_GPIO_SPEED(7) | \
                                     DEFAULT_GPIO_SPEED(8) | \
                                     DEFAULT_GPIO_SPEED(9) | \
                                     DEFAULT_GPIO_SPEED(10) | \
                                     DEFAULT_GPIO_SPEED(11) | \
                                     DEFAULT_GPIO_SPEED(12) | \
                                     DEFAULT_GPIO_SPEED(13) | \
                                     DEFAULT_GPIO_SPEED(14) | \
                                     DEFAULT_GPIO_SPEED(15))
#define VAL_GPIOE_PUPDR             (PIN_PUPDR_PULLDOWN(0) | \
                                     PIN_PUPDR_FLOATING(1) | \
                                     PIN_PUPDR_FLOATING(2) | \
                                     PIN_PUPDR_FLOATING(3) | \
                                     PIN_PUPDR_FLOATING(4) | \
                                     PIN_PUPDR_PULLDOWN(5) | \
                                     PIN_PUPDR_PULLDOWN(6) | \
                                     PIN_PUPDR_PULLDOWN(7) | \
                                     PIN_PUPDR_PULLDOWN(8) | \
                                     PIN_PUPDR_PULLDOWN(9) | \
                                     PIN_PUPDR_PULLDOWN(10) | \
                                     PIN_PUPDR_PULLDOWN(11) | \
                                     PIN_PUPDR_PULLDOWN(12) | \
                                     PIN_PUPDR_PULLDOWN(13) | \
                                     PIN_PUPDR_PULLDOWN(14) | \
                                     PIN_PUPDR_PULLUP(15))
#define VAL_GPIOE_ODR               (PIN_ODR_HIGH(0) | \
                                     PIN_ODR_HIGH(1) | \
                                     PIN_ODR_HIGH(2) | \
                                     PIN_ODR_HIGH(3) | \
                                     PIN_ODR_HIGH(4) | \
                                     PIN_ODR_HIGH(5) | \
                                     PIN_ODR_HIGH(6) | \
                                     PIN_ODR_HIGH(7) | \
                                     PIN_ODR_HIGH(8) | \
                                     PIN_ODR_HIGH(9) | \
                                     PIN_ODR_HIGH(10) | \
                                     PIN_ODR_HIGH(11) | \
                                     PIN_ODR_HIGH(12) | \
                                     PIN_ODR_HIGH(13) | \
                                     PIN_ODR_HIGH(14) | \
                                     PIN_ODR_HIGH(15))
#define VAL_GPIOE_AFRL              (PIN_AFIO_AF(0, 0U) |\
                                     PIN_AFIO_AF(1, 0U) | \
                                     PIN_AFIO_AF(2, 0U) | \
                                     PIN_AFIO_AF(3, 0U) | \
                                     PIN_AFIO_AF(4, 0U) | \
                                     PIN_AFIO_AF(5, 0U) | \
                                     PIN_AFIO_AF(6, 0U) | \
                                     PIN_AFIO_AF(7, 0U))
#define VAL_GPIOE_AFRH              (PIN_AFIO_AF(8, 0U) | \
                                     PIN_AFIO_AF(9, 0U) | \
                                     PIN_AFIO_AF(10, 0U) | \
                                     PIN_AFIO_AF(11, 0U) | \
                                     PIN_AFIO_AF(12, 0U) | \
                                     PIN_AFIO_AF(13, 0U) | \
                                     PIN_AFIO_AF(14, 0U) | \
                                     PIN_AFIO_AF(15, 0U))
/*
 * GPIOF setup:
 *
 * No pin is populated on 100 pin chip. Set all input floating.
 */
#define VAL_GPIOF_MODER             (EFI_PIN_MODE_DEFAULT(0) | \
                                     EFI_PIN_MODE_DEFAULT(1) | \
                                     EFI_PIN_MODE_DEFAULT(2) | \
                                     EFI_PIN_MODE_DEFAULT(3) | \
                                     EFI_PIN_MODE_DEFAULT(4) | \
                                     EFI_PIN_MODE_DEFAULT(5) | \
                                     EFI_PIN_MODE_DEFAULT(6) | \
                                     EFI_PIN_MODE_DEFAULT(7) | \
                                     EFI_PIN_MODE_DEFAULT(8) | \
                                     EFI_PIN_MODE_DEFAULT(9) | \
                                     EFI_PIN_MODE_DEFAULT(10) | \
                                     EFI_PIN_MODE_DEFAULT(11) | \
                                     EFI_PIN_MODE_DEFAULT(12) | \
                                     EFI_PIN_MODE_DEFAULT(13) | \
                                     EFI_PIN_MODE_DEFAULT(14) | \
                                     EFI_PIN_MODE_DEFAULT(15))
#define VAL_GPIOF_OTYPER            (PIN_OTYPE_PUSHPULL(0) | \
                                     PIN_OTYPE_PUSHPULL(1) | \
                                     PIN_OTYPE_PUSHPULL(2) | \
                                     PIN_OTYPE_PUSHPULL(3) | \
                                     PIN_OTYPE_PUSHPULL(4) | \
                                     PIN_OTYPE_PUSHPULL(5) | \
                                     PIN_OTYPE_PUSHPULL(6) | \
                                     PIN_OTYPE_PUSHPULL(7) | \
                                     PIN_OTYPE_PUSHPULL(8) | \
                                     PIN_OTYPE_PUSHPULL(9) | \
                                     PIN_OTYPE_PUSHPULL(10) | \
                                     PIN_OTYPE_PUSHPULL(11) | \
                                     PIN_OTYPE_PUSHPULL(12) | \
                                     PIN_OTYPE_PUSHPULL(13) | \
                                     PIN_OTYPE_PUSHPULL(14) | \
                                     PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOF_OSPEEDR           (DEFAULT_GPIO_SPEED(0) | \
                                     DEFAULT_GPIO_SPEED(1) | \
                                     DEFAULT_GPIO_SPEED(2) | \
                                     DEFAULT_GPIO_SPEED(3) | \
                                     DEFAULT_GPIO_SPEED(4) | \
                                     DEFAULT_GPIO_SPEED(5) | \
                                     DEFAULT_GPIO_SPEED(6) | \
                                     DEFAULT_GPIO_SPEED(7) | \
                                     DEFAULT_GPIO_SPEED(8) | \
                                     DEFAULT_GPIO_SPEED(9) | \
                                     DEFAULT_GPIO_SPEED(10) | \
                                     DEFAULT_GPIO_SPEED(11) | \
                                     DEFAULT_GPIO_SPEED(12) | \
                                     DEFAULT_GPIO_SPEED(13) | \
                                     DEFAULT_GPIO_SPEED(14) | \
                                     DEFAULT_GPIO_SPEED(15))
#define VAL_GPIOF_PUPDR             (EFI_DR_DEFAULT(0) | \
                                     EFI_DR_DEFAULT(1) | \
                                     EFI_DR_DEFAULT(2) | \
                                     EFI_DR_DEFAULT(3) | \
                                     EFI_DR_DEFAULT(4) | \
                                     EFI_DR_DEFAULT(5) | \
                                     EFI_DR_DEFAULT(6) | \
                                     EFI_DR_DEFAULT(7) | \
                                     EFI_DR_DEFAULT(8) | \
                                     EFI_DR_DEFAULT(9) | \
                                     EFI_DR_DEFAULT(10) | \
                                     EFI_DR_DEFAULT(11) | \
                                     EFI_DR_DEFAULT(12) | \
                                     EFI_DR_DEFAULT(13) | \
                                     EFI_DR_DEFAULT(14) | \
                                     EFI_DR_DEFAULT(15))
#define VAL_GPIOF_ODR               (PIN_ODR_HIGH(0) | \
                                     PIN_ODR_HIGH(1) | \
                                     PIN_ODR_HIGH(2) | \
                                     PIN_ODR_HIGH(3) | \
                                     PIN_ODR_HIGH(4) | \
                                     PIN_ODR_HIGH(5) | \
                                     PIN_ODR_HIGH(6) | \
                                     PIN_ODR_HIGH(7) | \
                                     PIN_ODR_HIGH(8) | \
                                     PIN_ODR_HIGH(9) | \
                                     PIN_ODR_HIGH(10) | \
                                     PIN_ODR_HIGH(11) | \
                                     PIN_ODR_HIGH(12) | \
                                     PIN_ODR_HIGH(13) | \
                                     PIN_ODR_HIGH(14) | \
                                     PIN_ODR_HIGH(15))
#define VAL_GPIOF_AFRL              (PIN_AFIO_AF(0, 0U) | \
                                     PIN_AFIO_AF(1, 0U) | \
                                     PIN_AFIO_AF(2, 0U) | \
                                     PIN_AFIO_AF(3, 0U) | \
                                     PIN_AFIO_AF(4, 0U) | \
                                     PIN_AFIO_AF(5, 0U) | \
                                     PIN_AFIO_AF(6, 0U) | \
                                     PIN_AFIO_AF(7, 0U))
#define VAL_GPIOF_AFRH              (PIN_AFIO_AF(8, 0U) | \
                                     PIN_AFIO_AF(9, 0U) | \
                                     PIN_AFIO_AF(10, 0U) | \
                                     PIN_AFIO_AF(11, 0U) | \
                                     PIN_AFIO_AF(12, 0U) | \
                                     PIN_AFIO_AF(13, 0U) | \
                                     PIN_AFIO_AF(14, 0U) | \
                                     PIN_AFIO_AF(15, 0U))

/*
 * GPIOG setup:
 *
 * No pin is populated on 100 pin chip. Set all input floating.
 */
#define VAL_GPIOG_MODER             (EFI_PIN_MODE_DEFAULT(0) | \
                                     EFI_PIN_MODE_DEFAULT(1) | \
                                     EFI_PIN_MODE_DEFAULT(2) | \
                                     EFI_PIN_MODE_DEFAULT(3) | \
                                     EFI_PIN_MODE_DEFAULT(4) | \
                                     EFI_PIN_MODE_DEFAULT(5) | \
                                     EFI_PIN_MODE_DEFAULT(6) | \
                                     EFI_PIN_MODE_DEFAULT(7) | \
                                     EFI_PIN_MODE_DEFAULT(8) | \
                                     EFI_PIN_MODE_DEFAULT(9) | \
                                     EFI_PIN_MODE_DEFAULT(10) | \
                                     EFI_PIN_MODE_DEFAULT(11) | \
                                     EFI_PIN_MODE_DEFAULT(12) | \
                                     EFI_PIN_MODE_DEFAULT(13) | \
                                     EFI_PIN_MODE_DEFAULT(14) | \
                                     EFI_PIN_MODE_DEFAULT(15))
#define VAL_GPIOG_OTYPER            (PIN_OTYPE_PUSHPULL(0) | \
                                     PIN_OTYPE_PUSHPULL(1) | \
                                     PIN_OTYPE_PUSHPULL(2) | \
                                     PIN_OTYPE_PUSHPULL(3) | \
                                     PIN_OTYPE_PUSHPULL(4) | \
                                     PIN_OTYPE_PUSHPULL(5) | \
                                     PIN_OTYPE_PUSHPULL(6) | \
                                     PIN_OTYPE_PUSHPULL(7) | \
                                     PIN_OTYPE_PUSHPULL(8) | \
                                     PIN_OTYPE_PUSHPULL(9) | \
                                     PIN_OTYPE_PUSHPULL(10) | \
                                     PIN_OTYPE_PUSHPULL(11) | \
                                     PIN_OTYPE_PUSHPULL(12) | \
                                     PIN_OTYPE_PUSHPULL(13) | \
                                     PIN_OTYPE_PUSHPULL(14) | \
                                     PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOG_OSPEEDR           (DEFAULT_GPIO_SPEED(0) | \
                                     DEFAULT_GPIO_SPEED(1) | \
                                     DEFAULT_GPIO_SPEED(2) | \
                                     DEFAULT_GPIO_SPEED(3) | \
                                     DEFAULT_GPIO_SPEED(4) | \
                                     DEFAULT_GPIO_SPEED(5) | \
                                     DEFAULT_GPIO_SPEED(6) | \
                                     DEFAULT_GPIO_SPEED(7) | \
                                     DEFAULT_GPIO_SPEED(8) | \
                                     DEFAULT_GPIO_SPEED(9) | \
                                     DEFAULT_GPIO_SPEED(10) | \
                                     DEFAULT_GPIO_SPEED(11) | \
                                     DEFAULT_GPIO_SPEED(12) | \
                                     DEFAULT_GPIO_SPEED(13) | \
                                     DEFAULT_GPIO_SPEED(14) | \
                                     DEFAULT_GPIO_SPEED(15))
#define VAL_GPIOG_PUPDR             (EFI_DR_DEFAULT(0) | \
                                     EFI_DR_DEFAULT(1) | \
                                     EFI_DR_DEFAULT(2) | \
                                     EFI_DR_DEFAULT(3) | \
                                     EFI_DR_DEFAULT(4) | \
                                     EFI_DR_DEFAULT(5) | \
                                     EFI_DR_DEFAULT(6) | \
                                     EFI_DR_DEFAULT(7) | \
                                     EFI_DR_DEFAULT(8) | \
                                     EFI_DR_DEFAULT(9) | \
                                     EFI_DR_DEFAULT(10) | \
                                     EFI_DR_DEFAULT(11) | \
                                     EFI_DR_DEFAULT(12) | \
                                     EFI_DR_DEFAULT(13) | \
                                     EFI_DR_DEFAULT(14) | \
                                     EFI_DR_DEFAULT(15))
#define VAL_GPIOG_ODR               (PIN_ODR_HIGH(0) | \
                                     PIN_ODR_HIGH(1) | \
                                     PIN_ODR_HIGH(2) | \
                                     PIN_ODR_HIGH(3) | \
                                     PIN_ODR_HIGH(4) | \
                                     PIN_ODR_HIGH(5) | \
                                     PIN_ODR_HIGH(6) | \
                                     PIN_ODR_HIGH(7) | \
                                     PIN_ODR_HIGH(8) | \
                                     PIN_ODR_HIGH(9) | \
                                     PIN_ODR_HIGH(10) | \
                                     PIN_ODR_HIGH(11) | \
                                     PIN_ODR_HIGH(12) | \
                                     PIN_ODR_HIGH(13) | \
                                     PIN_ODR_HIGH(14) | \
                                     PIN_ODR_HIGH(15))
#define VAL_GPIOG_AFRL              (PIN_AFIO_AF(0, 0U) | \
                                     PIN_AFIO_AF(1, 0U) | \
                                     PIN_AFIO_AF(2, 0U) | \
                                     PIN_AFIO_AF(3, 0U) | \
                                     PIN_AFIO_AF(4, 0U) | \
                                     PIN_AFIO_AF(5, 0U) | \
                                     PIN_AFIO_AF(6, 0U) | \
                                     PIN_AFIO_AF(7, 0U))
#define VAL_GPIOG_AFRH              (PIN_AFIO_AF(8, 0U) | \
                                     PIN_AFIO_AF(9, 0U) | \
                                     PIN_AFIO_AF(10, 0U) | \
                                     PIN_AFIO_AF(11, 0U) | \
                                     PIN_AFIO_AF(12, 0U) | \
                                     PIN_AFIO_AF(13, 0U) | \
                                     PIN_AFIO_AF(14, 0U) | \
                                     PIN_AFIO_AF(15, 0U))

/*
 * GPIOH setup:
 *
 * PH0  - OSC_IN                    (input floating).
 * PH1  - OSC_OUT                   (input floating).
 */
#define VAL_GPIOH_MODER             (EFI_PIN_MODE_DEFAULT(0) | \
                                     EFI_PIN_MODE_DEFAULT(1) | \
                                     EFI_PIN_MODE_DEFAULT(2) | \
                                     EFI_PIN_MODE_DEFAULT(3) | \
                                     EFI_PIN_MODE_DEFAULT(4) | \
                                     EFI_PIN_MODE_DEFAULT(5) | \
                                     EFI_PIN_MODE_DEFAULT(6) | \
                                     EFI_PIN_MODE_DEFAULT(7) | \
                                     EFI_PIN_MODE_DEFAULT(8) | \
                                     EFI_PIN_MODE_DEFAULT(9) | \
                                     EFI_PIN_MODE_DEFAULT(10) | \
                                     EFI_PIN_MODE_DEFAULT(11) | \
                                     EFI_PIN_MODE_DEFAULT(12) | \
                                     EFI_PIN_MODE_DEFAULT(13) | \
                                     EFI_PIN_MODE_DEFAULT(14) | \
                                     EFI_PIN_MODE_DEFAULT(15))
#define VAL_GPIOH_OTYPER            (PIN_OTYPE_PUSHPULL(0) | \
                                     PIN_OTYPE_PUSHPULL(1) | \
                                     PIN_OTYPE_PUSHPULL(2) | \
                                     PIN_OTYPE_PUSHPULL(3) | \
                                     PIN_OTYPE_PUSHPULL(4) | \
                                     PIN_OTYPE_PUSHPULL(5) | \
                                     PIN_OTYPE_PUSHPULL(6) | \
                                     PIN_OTYPE_PUSHPULL(7) | \
                                     PIN_OTYPE_PUSHPULL(8) | \
                                     PIN_OTYPE_PUSHPULL(9) | \
                                     PIN_OTYPE_PUSHPULL(10) | \
                                     PIN_OTYPE_PUSHPULL(11) | \
                                     PIN_OTYPE_PUSHPULL(12) | \
                                     PIN_OTYPE_PUSHPULL(13) | \
                                     PIN_OTYPE_PUSHPULL(14) | \
                                     PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOH_OSPEEDR           (DEFAULT_GPIO_SPEED(0) | \
                                     DEFAULT_GPIO_SPEED(1) | \
                                     DEFAULT_GPIO_SPEED(2) | \
                                     DEFAULT_GPIO_SPEED(3) | \
                                     DEFAULT_GPIO_SPEED(4) | \
                                     DEFAULT_GPIO_SPEED(5) | \
                                     DEFAULT_GPIO_SPEED(6) | \
                                     DEFAULT_GPIO_SPEED(7) | \
                                     DEFAULT_GPIO_SPEED(8) | \
                                     DEFAULT_GPIO_SPEED(9) | \
                                     DEFAULT_GPIO_SPEED(10) | \
                                     DEFAULT_GPIO_SPEED(11) | \
                                     DEFAULT_GPIO_SPEED(12) | \
                                     DEFAULT_GPIO_SPEED(13) | \
                                     DEFAULT_GPIO_SPEED(14) | \
                                     DEFAULT_GPIO_SPEED(15))
#define VAL_GPIOH_PUPDR             (EFI_DR_DEFAULT(0) | \
                                     EFI_DR_DEFAULT(1) | \
                                     EFI_DR_DEFAULT(2) | \
                                     EFI_DR_DEFAULT(3) | \
                                     EFI_DR_DEFAULT(4) | \
                                     EFI_DR_DEFAULT(5) | \
                                     EFI_DR_DEFAULT(6) | \
                                     EFI_DR_DEFAULT(7) | \
                                     EFI_DR_DEFAULT(8) | \
                                     EFI_DR_DEFAULT(9) | \
                                     EFI_DR_DEFAULT(10) | \
                                     EFI_DR_DEFAULT(11) | \
                                     EFI_DR_DEFAULT(12) | \
                                     EFI_DR_DEFAULT(13) | \
                                     EFI_DR_DEFAULT(14) | \
                                     EFI_DR_DEFAULT(15))
#define VAL_GPIOH_ODR               (PIN_ODR_HIGH(0) | \
                                     PIN_ODR_HIGH(1) | \
                                     PIN_ODR_HIGH(2) | \
                                     PIN_ODR_HIGH(3) | \
                                     PIN_ODR_HIGH(4) | \
                                     PIN_ODR_HIGH(5) | \
                                     PIN_ODR_HIGH(6) | \
                                     PIN_ODR_HIGH(7) | \
                                     PIN_ODR_HIGH(8) | \
                                     PIN_ODR_HIGH(9) | \
                                     PIN_ODR_HIGH(10) | \
                                     PIN_ODR_HIGH(11) | \
                                     PIN_ODR_HIGH(12) | \
                                     PIN_ODR_HIGH(13) | \
                                     PIN_ODR_HIGH(14) | \
                                     PIN_ODR_HIGH(15))
#define VAL_GPIOH_AFRL              (PIN_AFIO_AF(0, 0U) | \
                                     PIN_AFIO_AF(1, 0U) | \
                                     PIN_AFIO_AF(2, 0U) | \
                                     PIN_AFIO_AF(3, 0U) | \
                                     PIN_AFIO_AF(4, 0U) | \
                                     PIN_AFIO_AF(5, 0U) | \
                                     PIN_AFIO_AF(6, 0U) | \
                                     PIN_AFIO_AF(7, 0U))
#define VAL_GPIOH_AFRH              (PIN_AFIO_AF(8, 0U) | \
                                     PIN_AFIO_AF(9, 0U) | \
                                     PIN_AFIO_AF(10, 0U) | \
                                     PIN_AFIO_AF(11, 0U) | \
                                     PIN_AFIO_AF(12, 0U) | \
                                     PIN_AFIO_AF(13, 0U) | \
                                     PIN_AFIO_AF(14, 0U) | \
                                     PIN_AFIO_AF(15, 0U))

/*
 * GPIOI setup:
 *
 * No pin is populated on 100 pin chip. Set all input floating.
 */
#define VAL_GPIOI_MODER             (EFI_PIN_MODE_DEFAULT(0) | \
                                     EFI_PIN_MODE_DEFAULT(1) | \
                                     EFI_PIN_MODE_DEFAULT(2) | \
                                     EFI_PIN_MODE_DEFAULT(3) | \
                                     EFI_PIN_MODE_DEFAULT(4) | \
                                     EFI_PIN_MODE_DEFAULT(5) | \
                                     EFI_PIN_MODE_DEFAULT(6) | \
                                     EFI_PIN_MODE_DEFAULT(7) | \
                                     EFI_PIN_MODE_DEFAULT(8) | \
                                     EFI_PIN_MODE_DEFAULT(9) | \
                                     EFI_PIN_MODE_DEFAULT(10) | \
                                     EFI_PIN_MODE_DEFAULT(11) | \
                                     EFI_PIN_MODE_DEFAULT(12) | \
                                     EFI_PIN_MODE_DEFAULT(13) | \
                                     EFI_PIN_MODE_DEFAULT(14) | \
                                     EFI_PIN_MODE_DEFAULT(15))
#define VAL_GPIOI_OTYPER            (PIN_OTYPE_PUSHPULL(0) | \
                                     PIN_OTYPE_PUSHPULL(1) | \
                                     PIN_OTYPE_PUSHPULL(2) | \
                                     PIN_OTYPE_PUSHPULL(3) | \
                                     PIN_OTYPE_PUSHPULL(4) | \
                                     PIN_OTYPE_PUSHPULL(5) | \
                                     PIN_OTYPE_PUSHPULL(6) | \
                                     PIN_OTYPE_PUSHPULL(7) | \
                                     PIN_OTYPE_PUSHPULL(8) | \
                                     PIN_OTYPE_PUSHPULL(9) | \
                                     PIN_OTYPE_PUSHPULL(10) | \
                                     PIN_OTYPE_PUSHPULL(11) | \
                                     PIN_OTYPE_PUSHPULL(12) | \
                                     PIN_OTYPE_PUSHPULL(13) | \
                                     PIN_OTYPE_PUSHPULL(14) | \
                                     PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOI_OSPEEDR           (DEFAULT_GPIO_SPEED(0) | \
                                     DEFAULT_GPIO_SPEED(1) | \
                                     DEFAULT_GPIO_SPEED(2) | \
                                     DEFAULT_GPIO_SPEED(3) | \
                                     DEFAULT_GPIO_SPEED(4) | \
                                     DEFAULT_GPIO_SPEED(5) | \
                                     DEFAULT_GPIO_SPEED(6) | \
                                     DEFAULT_GPIO_SPEED(7) | \
                                     DEFAULT_GPIO_SPEED(8) | \
                                     DEFAULT_GPIO_SPEED(9) | \
                                     DEFAULT_GPIO_SPEED(10) | \
                                     DEFAULT_GPIO_SPEED(11) | \
                                     DEFAULT_GPIO_SPEED(12) | \
                                     DEFAULT_GPIO_SPEED(13) | \
                                     DEFAULT_GPIO_SPEED(14) | \
                                     DEFAULT_GPIO_SPEED(15))
#define VAL_GPIOI_PUPDR             (EFI_DR_DEFAULT(0) | \
                                     EFI_DR_DEFAULT(1) | \
                                     EFI_DR_DEFAULT(2) | \
                                     EFI_DR_DEFAULT(3) | \
                                     EFI_DR_DEFAULT(4) | \
                                     EFI_DR_DEFAULT(5) | \
                                     EFI_DR_DEFAULT(6) | \
                                     EFI_DR_DEFAULT(7) | \
                                     EFI_DR_DEFAULT(8) | \
                                     EFI_DR_DEFAULT(9) | \
                                     EFI_DR_DEFAULT(10) | \
                                     EFI_DR_DEFAULT(11) | \
                                     EFI_DR_DEFAULT(12) | \
                                     EFI_DR_DEFAULT(13) | \
                                     EFI_DR_DEFAULT(14) | \
                                     EFI_DR_DEFAULT(15))
#define VAL_GPIOI_ODR               (PIN_ODR_HIGH(0) | \
                                     PIN_ODR_HIGH(1) | \
                                     PIN_ODR_HIGH(2) | \
                                     PIN_ODR_HIGH(3) | \
                                     PIN_ODR_HIGH(4) | \
                                     PIN_ODR_HIGH(5) | \
                                     PIN_ODR_HIGH(6) | \
                                     PIN_ODR_HIGH(7) | \
                                     PIN_ODR_HIGH(8) | \
                                     PIN_ODR_HIGH(9) | \
                                     PIN_ODR_HIGH(10) | \
                                     PIN_ODR_HIGH(11) | \
                                     PIN_ODR_HIGH(12) | \
                                     PIN_ODR_HIGH(13) | \
                                     PIN_ODR_HIGH(14) | \
                                     PIN_ODR_HIGH(15))
#define VAL_GPIOI_AFRL              (PIN_AFIO_AF(0, 0U) | \
                                     PIN_AFIO_AF(1, 0U) | \
                                     PIN_AFIO_AF(2, 0U) | \
                                     PIN_AFIO_AF(3, 0U) | \
                                     PIN_AFIO_AF(4, 0U) | \
                                     PIN_AFIO_AF(5, 0U) | \
                                     PIN_AFIO_AF(6, 0U) | \
                                     PIN_AFIO_AF(7, 0U))
#define VAL_GPIOI_AFRH              (PIN_AFIO_AF(8, 0U) | \
                                     PIN_AFIO_AF(9, 0U) | \
                                     PIN_AFIO_AF(10, 0U) | \
                                     PIN_AFIO_AF(11, 0U) | \
                                     PIN_AFIO_AF(12, 0U) | \
                                     PIN_AFIO_AF(13, 0U) | \
                                     PIN_AFIO_AF(14, 0U) | \
                                     PIN_AFIO_AF(15, 0U))

#endif /* BOARD_H */
