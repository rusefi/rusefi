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
 * Setup for STMicroelectronics STM32F4-Discovery board.
 */

/*
 * Board identifier.
 */
#define BOARD_NAME                  "Hellen81"

#define EFI_USB_AF 10U
#define EFI_USB_SERIAL_DM GPIOA_11
#define EFI_USB_SERIAL_DP GPIOA_12

// Ignore USB VBUS pin (we're never a host, only a device)
#define BOARD_OTG_NOVBUSSENS TRUE

/*
 * Default to input mode, with internal PULLDOWN resistor enabled.
 * Pulldowns are more safe because most of MCU outputs on Hellen are high-active.
 */
#define EFI_PIN_MODE_DEFAULT PIN_MODE_INPUT
#define EFI_DR_DEFAULT PIN_PUPDR_PULLDOWN

// See https://github.com/rusefi/rusefi/issues/397
#define DEFAULT_GPIO_SPEED PIN_OSPEED_HIGH

/*
 * Board oscillators-related settings.
 * NOTE: LSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                32768U
#endif

#define STM32_LSEDRV                (3U << 3U)

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                8000000U
#endif

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   300U

/*
 * IO pins assignments.
 */
#define GPIOA_SWDIO                 13
#define GPIOA_SWCLK                 14

#define GPIOB_SWO                   3

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

#define VAL_GPIO_MODER_ALL_DEFAULT  (EFI_PIN_MODE_DEFAULT(0) |           \
                                     EFI_PIN_MODE_DEFAULT(1) |           \
                                     EFI_PIN_MODE_DEFAULT(2) |           \
                                     EFI_PIN_MODE_DEFAULT(3) |           \
                                     EFI_PIN_MODE_DEFAULT(4) |         \
                                     EFI_PIN_MODE_DEFAULT(5) |   \
                                     EFI_PIN_MODE_DEFAULT(6) |           \
                                     EFI_PIN_MODE_DEFAULT(7) |           \
                                     EFI_PIN_MODE_DEFAULT(8) |           \
                                     EFI_PIN_MODE_DEFAULT(9) |           \
                                     EFI_PIN_MODE_DEFAULT(10) |          \
                                     EFI_PIN_MODE_DEFAULT(11) |          \
                                     EFI_PIN_MODE_DEFAULT(12) |          \
                                     EFI_PIN_MODE_DEFAULT(13) |          \
                                     EFI_PIN_MODE_DEFAULT(14) |          \
                                     EFI_PIN_MODE_DEFAULT(15))

#define VAL_GPIO_OTYPER_ALL_DEFAULT (PIN_OTYPE_PUSHPULL(0) |   \
                                     PIN_OTYPE_PUSHPULL(1) |   \
                                     PIN_OTYPE_PUSHPULL(2) |   \
                                     PIN_OTYPE_PUSHPULL(3) |   \
                                     PIN_OTYPE_PUSHPULL(4) |   \
                                     PIN_OTYPE_PUSHPULL(5) |   \
                                     PIN_OTYPE_PUSHPULL(6) |   \
                                     PIN_OTYPE_PUSHPULL(7) |   \
                                     PIN_OTYPE_PUSHPULL(8) |   \
                                     PIN_OTYPE_PUSHPULL(9) |   \
                                     PIN_OTYPE_PUSHPULL(10) |  \
                                     PIN_OTYPE_PUSHPULL(11) |  \
                                     PIN_OTYPE_PUSHPULL(12) |  \
                                     PIN_OTYPE_PUSHPULL(13) |  \
                                     PIN_OTYPE_PUSHPULL(14) |  \
                                     PIN_OTYPE_PUSHPULL(15))

#define VAL_GPIO_OSPEEDR_ALL_DEFAULT (DEFAULT_GPIO_SPEED(0) |          \
                                     DEFAULT_GPIO_SPEED(1) |          \
                                     DEFAULT_GPIO_SPEED(2) |          \
                                     DEFAULT_GPIO_SPEED(3) |          \
                                     DEFAULT_GPIO_SPEED(4) |         \
                                     DEFAULT_GPIO_SPEED(5) |  \
                                     DEFAULT_GPIO_SPEED(6) |          \
                                     DEFAULT_GPIO_SPEED(7) |          \
                                     DEFAULT_GPIO_SPEED(8) |          \
                                     DEFAULT_GPIO_SPEED(9) |          \
                                     DEFAULT_GPIO_SPEED(10) |         \
                                     DEFAULT_GPIO_SPEED(11) |         \
                                     DEFAULT_GPIO_SPEED(12) |          \
                                     DEFAULT_GPIO_SPEED(13) |          \
                                     DEFAULT_GPIO_SPEED(14) |          \
                                     DEFAULT_GPIO_SPEED(15))

#define VAL_GPIO_ODR_ALL_DEFAULT    0

#define VAL_GPIO_PUPDR_ALL_DEFAULT (EFI_DR_DEFAULT(0) |        \
                                     EFI_DR_DEFAULT(1) |       \
                                     EFI_DR_DEFAULT(2) |       \
                                     EFI_DR_DEFAULT(3) |       \
                                     EFI_DR_DEFAULT(4) |       \
                                     EFI_DR_DEFAULT(5) |       \
                                     EFI_DR_DEFAULT(6) |       \
                                     EFI_DR_DEFAULT(7) |       \
                                     EFI_DR_DEFAULT(8) |       \
                                     EFI_DR_DEFAULT(9) |       \
                                     EFI_DR_DEFAULT(10) |      \
                                     EFI_DR_DEFAULT(11) |      \
                                     EFI_DR_DEFAULT(12) |      \
                                     EFI_DR_DEFAULT(13) |      \
                                     EFI_DR_DEFAULT(14) |      \
                                     EFI_DR_DEFAULT(15))

#define VAL_GPIO_AF_ALL_DEFAULT   (PIN_AFIO_AF(0, 0U) |          \
                                   PIN_AFIO_AF(1, 0U) |          \
                                   PIN_AFIO_AF(2, 0U) |          \
                                   PIN_AFIO_AF(3, 0U) |          \
                                   PIN_AFIO_AF(4, 0U) |          \
                                   PIN_AFIO_AF(5, 0U) |          \
                                   PIN_AFIO_AF(6, 0U) |          \
                                   PIN_AFIO_AF(7, 0U))


/*
 * GPIOA setup:
 *
 * PA11 - OTG_FS_DM                 (alternate 10).
 * PA12 - OTG_FS_DP                 (alternate 10).
 * PA13 - SWDIO                     (alternate 0).
 * PA14 - SWCLK                     (alternate 0).
 */
#define VAL_GPIOA_MODER             (EFI_PIN_MODE_DEFAULT(0) |         \
                                     EFI_PIN_MODE_DEFAULT(1) |           \
                                     EFI_PIN_MODE_DEFAULT(2) |           \
                                     EFI_PIN_MODE_DEFAULT(3) |           \
                                     EFI_PIN_MODE_DEFAULT(4) |       \
                                     EFI_PIN_MODE_DEFAULT(5) |        \
                                     EFI_PIN_MODE_DEFAULT(6) |        \
                                     EFI_PIN_MODE_DEFAULT(7) |        \
                                     EFI_PIN_MODE_DEFAULT(8) |           \
                                     EFI_PIN_MODE_DEFAULT(9) |        \
                                     EFI_PIN_MODE_DEFAULT(10) |  \
                                     PIN_MODE_ALTERNATE(11) |  \
                                     PIN_MODE_ALTERNATE(12) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_SWDIO) |      \
                                     PIN_MODE_ALTERNATE(GPIOA_SWCLK) |      \
                                     EFI_PIN_MODE_DEFAULT(15))
#define VAL_GPIOA_OTYPER            VAL_GPIO_OTYPER_ALL_DEFAULT
#define VAL_GPIOA_OSPEEDR           VAL_GPIO_OSPEEDR_ALL_DEFAULT
#define VAL_GPIOA_PUPDR             VAL_GPIO_PUPDR_ALL_DEFAULT
#define VAL_GPIOA_ODR               VAL_GPIO_ODR_ALL_DEFAULT
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(0, 0U) |        \
                                     PIN_AFIO_AF(1, 0U) |          \
                                     PIN_AFIO_AF(2, 0U) |          \
                                     PIN_AFIO_AF(3, 0U) |          \
                                     PIN_AFIO_AF(4, 6U) |          \
                                     PIN_AFIO_AF(5, 5U) |           \
                                     PIN_AFIO_AF(6, 5U) |           \
                                     PIN_AFIO_AF(7, 5U))
#define VAL_GPIOA_AFRH              VAL_GPIO_AF_ALL_DEFAULT

/*
 * GPIOB setup:
 * 
 * Default except SWO configured on PB3
 * 
 */
#define VAL_GPIOB_MODER             (EFI_PIN_MODE_DEFAULT(0) |           \
                                     EFI_PIN_MODE_DEFAULT(1) |           \
                                     EFI_PIN_MODE_DEFAULT(2) |           \
                                     PIN_MODE_ALTERNATE(GPIOB_SWO) |        \
                                     EFI_PIN_MODE_DEFAULT(4) |           \
                                     EFI_PIN_MODE_DEFAULT(5) |           \
                                     EFI_PIN_MODE_DEFAULT(6) |        \
                                     EFI_PIN_MODE_DEFAULT(7) |           \
                                     EFI_PIN_MODE_DEFAULT(8) |           \
                                     EFI_PIN_MODE_DEFAULT(9) |          \
                                     EFI_PIN_MODE_DEFAULT(10) |          \
                                     EFI_PIN_MODE_DEFAULT(11) |          \
                                     EFI_PIN_MODE_DEFAULT(12) |          \
                                     EFI_PIN_MODE_DEFAULT(13) |          \
                                     EFI_PIN_MODE_DEFAULT(14) |          \
                                     EFI_PIN_MODE_DEFAULT(15))
#define VAL_GPIOB_OTYPER            VAL_GPIO_OTYPER_ALL_DEFAULT
#define VAL_GPIOB_OSPEEDR           VAL_GPIO_OSPEEDR_ALL_DEFAULT
#define VAL_GPIOB_PUPDR             VAL_GPIO_PUPDR_ALL_DEFAULT
#define VAL_GPIOB_ODR               VAL_GPIO_ODR_ALL_DEFAULT
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(0, 0U) |          \
                                     PIN_AFIO_AF(1, 0U) |          \
                                     PIN_AFIO_AF(2, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_SWO, 0U) |           \
                                     PIN_AFIO_AF(4, 0U) |          \
                                     PIN_AFIO_AF(5, 0U) |          \
                                     PIN_AFIO_AF(6, 0U) |           \
                                     PIN_AFIO_AF(7, 0U))
#define VAL_GPIOB_AFRH              VAL_GPIO_AF_ALL_DEFAULT

/*
 * GPIOC setup:
 */
#define VAL_GPIOC_MODER             VAL_GPIO_MODER_ALL_DEFAULT
#define VAL_GPIOC_OTYPER            VAL_GPIO_OTYPER_ALL_DEFAULT
#define VAL_GPIOC_OSPEEDR           VAL_GPIO_OSPEEDR_ALL_DEFAULT
#define VAL_GPIOC_PUPDR             VAL_GPIO_PUPDR_ALL_DEFAULT
#define VAL_GPIOC_ODR               VAL_GPIO_ODR_ALL_DEFAULT
#define VAL_GPIOC_AFRL              VAL_GPIO_AF_ALL_DEFAULT
#define VAL_GPIOC_AFRH              VAL_GPIO_AF_ALL_DEFAULT

/*
 * GPIOD setup:
 */
#define VAL_GPIOD_MODER             VAL_GPIO_MODER_ALL_DEFAULT
#define VAL_GPIOD_OTYPER            VAL_GPIO_OTYPER_ALL_DEFAULT
#define VAL_GPIOD_OSPEEDR           VAL_GPIO_OSPEEDR_ALL_DEFAULT
#define VAL_GPIOD_PUPDR             VAL_GPIO_PUPDR_ALL_DEFAULT
#define VAL_GPIOD_ODR               VAL_GPIO_ODR_ALL_DEFAULT
#define VAL_GPIOD_AFRL              VAL_GPIO_AF_ALL_DEFAULT
#define VAL_GPIOD_AFRH              VAL_GPIO_AF_ALL_DEFAULT

/*
 * GPIOE setup:
 */
#define VAL_GPIOE_MODER             VAL_GPIO_MODER_ALL_DEFAULT
#define VAL_GPIOE_OTYPER            VAL_GPIO_OTYPER_ALL_DEFAULT
#define VAL_GPIOE_OSPEEDR           VAL_GPIO_OSPEEDR_ALL_DEFAULT
#define VAL_GPIOE_PUPDR             VAL_GPIO_PUPDR_ALL_DEFAULT
#define VAL_GPIOE_ODR               VAL_GPIO_ODR_ALL_DEFAULT
#define VAL_GPIOE_AFRL              VAL_GPIO_AF_ALL_DEFAULT
#define VAL_GPIOE_AFRH              VAL_GPIO_AF_ALL_DEFAULT

/*
 * GPIOF setup:
 */
#define VAL_GPIOF_MODER             VAL_GPIO_MODER_ALL_DEFAULT
#define VAL_GPIOF_OTYPER            VAL_GPIO_OTYPER_ALL_DEFAULT
#define VAL_GPIOF_OSPEEDR           VAL_GPIO_OSPEEDR_ALL_DEFAULT
#define VAL_GPIOF_PUPDR             VAL_GPIO_PUPDR_ALL_DEFAULT
#define VAL_GPIOF_ODR               VAL_GPIO_ODR_ALL_DEFAULT
#define VAL_GPIOF_AFRL              VAL_GPIO_AF_ALL_DEFAULT
#define VAL_GPIOF_AFRH              VAL_GPIO_AF_ALL_DEFAULT

/*
 * GPIOG setup:
 */
#define VAL_GPIOG_MODER             VAL_GPIO_MODER_ALL_DEFAULT
#define VAL_GPIOG_OTYPER            VAL_GPIO_OTYPER_ALL_DEFAULT
#define VAL_GPIOG_OSPEEDR           VAL_GPIO_OSPEEDR_ALL_DEFAULT
#define VAL_GPIOG_PUPDR             VAL_GPIO_PUPDR_ALL_DEFAULT
#define VAL_GPIOG_ODR               VAL_GPIO_ODR_ALL_DEFAULT
#define VAL_GPIOG_AFRL              VAL_GPIO_AF_ALL_DEFAULT
#define VAL_GPIOG_AFRH              VAL_GPIO_AF_ALL_DEFAULT

/*
 * GPIOH setup:
 */
#define VAL_GPIOH_MODER             VAL_GPIO_MODER_ALL_DEFAULT
#define VAL_GPIOH_OTYPER            VAL_GPIO_OTYPER_ALL_DEFAULT
#define VAL_GPIOH_OSPEEDR           VAL_GPIO_OSPEEDR_ALL_DEFAULT
#define VAL_GPIOH_PUPDR             VAL_GPIO_PUPDR_ALL_DEFAULT
#define VAL_GPIOH_ODR               VAL_GPIO_ODR_ALL_DEFAULT
#define VAL_GPIOH_AFRL              VAL_GPIO_AF_ALL_DEFAULT
#define VAL_GPIOH_AFRH              VAL_GPIO_AF_ALL_DEFAULT

/*
 * GPIOI setup:
 */
#define VAL_GPIOI_MODER             VAL_GPIO_MODER_ALL_DEFAULT
#define VAL_GPIOI_OTYPER            VAL_GPIO_OTYPER_ALL_DEFAULT
#define VAL_GPIOI_OSPEEDR           VAL_GPIO_OSPEEDR_ALL_DEFAULT
#define VAL_GPIOI_PUPDR             VAL_GPIO_PUPDR_ALL_DEFAULT
#define VAL_GPIOI_ODR               VAL_GPIO_ODR_ALL_DEFAULT
#define VAL_GPIOI_AFRL              VAL_GPIO_AF_ALL_DEFAULT
#define VAL_GPIOI_AFRH              VAL_GPIO_AF_ALL_DEFAULT

/*
 * GPIOJ setup:
 */
#define VAL_GPIOJ_MODER             VAL_GPIO_MODER_ALL_DEFAULT
#define VAL_GPIOJ_OTYPER            VAL_GPIO_OTYPER_ALL_DEFAULT
#define VAL_GPIOJ_OSPEEDR           VAL_GPIO_OSPEEDR_ALL_DEFAULT
#define VAL_GPIOJ_PUPDR             VAL_GPIO_PUPDR_ALL_DEFAULT
#define VAL_GPIOJ_ODR               VAL_GPIO_ODR_ALL_DEFAULT
#define VAL_GPIOJ_AFRL              VAL_GPIO_AF_ALL_DEFAULT
#define VAL_GPIOJ_AFRH              VAL_GPIO_AF_ALL_DEFAULT	

/*
 * GPIOK setup:
 */
#define VAL_GPIOK_MODER             VAL_GPIO_MODER_ALL_DEFAULT
#define VAL_GPIOK_OTYPER            VAL_GPIO_OTYPER_ALL_DEFAULT
#define VAL_GPIOK_OSPEEDR           VAL_GPIO_OSPEEDR_ALL_DEFAULT
#define VAL_GPIOK_PUPDR             VAL_GPIO_PUPDR_ALL_DEFAULT
#define VAL_GPIOK_ODR               VAL_GPIO_ODR_ALL_DEFAULT
#define VAL_GPIOK_AFRL              VAL_GPIO_AF_ALL_DEFAULT
#define VAL_GPIOK_AFRH              VAL_GPIO_AF_ALL_DEFAULT	

#endif /* BOARD_H */
