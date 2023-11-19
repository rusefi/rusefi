/*
    ChibiOS - Copyright (C) 2006..2020 Giovanni Di Sirio

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

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*
 * Setup for STMicroelectronics STM32 Nucleo144-F429ZI board.
 */

/*
 * Board identifier.
 */
#define BOARD_NAME                  "Itelma M74.9"

/*
 * Board oscillators-related settings.
 * NOTE: LSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                0U
#endif

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                8000000U
#endif

// M74.9 has crystal/ceramic resonator
//#define STM32_HSE_BYPASS

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   300U

/*
 * MCU type as defined in the AT header.
 */
#define AT32F435xx

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

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
/* Normal */
#define PIN_OSPEED_VERYLOW(n)       (0U << ((n) * 2U))
/* Normal */
#define PIN_OSPEED_MEDIUM(n)        (3U << ((n) * 2U))
/* Large */
#define PIN_OSPEED_HIGH(n)          (1U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))

/*
 * GPIOA setup:
 *
 * PA0  - PIN0                      (input pullup).
 * PA1  - PIN1                      (input pullup).
 * PA2  - PIN2                      (input pullup).
 * PA3  - PIN3                      (input pullup).
 * PA4  - PIN4                      (input pullup).
 * PA5  - PIN5                      (input pullup).
 * PA6  - PIN6                      (input pullup).
 * PA7  - PIN7                      (input pullup).
 * PA8  - PIN8                      (input pullup).
 * PA9  - PIN9                      (input pullup).
 * PA10 - PIN10                     (input pullup).
 * PA11 - PIN11                     (input pullup).
 * PA12 - PIN12                     (input pullup).
 * PA13 - PIN13                     SWDIO (routed to JTAG connector pin 2)
 * PA14 - PIN14                     SWCLK (routed to JTAG connector pin 4)
 * PA15 - PIN15                     JTDI (routed to JTAG connector pin 8)
 */

#define VAL_GPIOA_MODER             (PIN_MODE_INPUT(0) |           \
                                     PIN_MODE_INPUT(1) |           \
                                     PIN_MODE_INPUT(2) |           \
                                     PIN_MODE_INPUT(3) |           \
                                     PIN_MODE_INPUT(4) |           \
                                     PIN_MODE_INPUT(5) |           \
                                     PIN_MODE_INPUT(6) |           \
                                     PIN_MODE_INPUT(7) |           \
                                     PIN_MODE_INPUT(8) |           \
                                     PIN_MODE_INPUT(9) |           \
                                     PIN_MODE_INPUT(10) |          \
                                     PIN_MODE_INPUT(11) |          \
                                     PIN_MODE_INPUT(12) |          \
                                     PIN_MODE_ALTERNATE(13) |      \
                                     PIN_MODE_ALTERNATE(14) |      \
                                     PIN_MODE_ALTERNATE(15))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(0) |       \
                                     PIN_OTYPE_PUSHPULL(1) |       \
                                     PIN_OTYPE_PUSHPULL(2) |       \
                                     PIN_OTYPE_PUSHPULL(3) |       \
                                     PIN_OTYPE_PUSHPULL(4) |       \
                                     PIN_OTYPE_PUSHPULL(5) |       \
                                     PIN_OTYPE_PUSHPULL(6) |       \
                                     PIN_OTYPE_PUSHPULL(7) |       \
                                     PIN_OTYPE_PUSHPULL(8) |       \
                                     PIN_OTYPE_PUSHPULL(9) |       \
                                     PIN_OTYPE_PUSHPULL(10) |      \
                                     PIN_OTYPE_PUSHPULL(11) |      \
                                     PIN_OTYPE_PUSHPULL(12) |      \
                                     PIN_OTYPE_PUSHPULL(13) |      \
                                     PIN_OTYPE_PUSHPULL(14) |      \
                                     PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_VERYLOW(0) |       \
                                     PIN_OSPEED_VERYLOW(1) |       \
                                     PIN_OSPEED_VERYLOW(2) |       \
                                     PIN_OSPEED_VERYLOW(3) |       \
                                     PIN_OSPEED_VERYLOW(4) |       \
                                     PIN_OSPEED_VERYLOW(5) |       \
                                     PIN_OSPEED_VERYLOW(6) |       \
                                     PIN_OSPEED_VERYLOW(7) |       \
                                     PIN_OSPEED_VERYLOW(8) |       \
                                     PIN_OSPEED_VERYLOW(9) |       \
                                     PIN_OSPEED_VERYLOW(10) |      \
                                     PIN_OSPEED_VERYLOW(11) |      \
                                     PIN_OSPEED_VERYLOW(12) |      \
                                     PIN_OSPEED_HIGH(13) |         \
                                     PIN_OSPEED_HIGH(14) |         \
                                     PIN_OSPEED_HIGH(15))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_PULLUP(0) |         \
                                     PIN_PUPDR_PULLUP(1) |         \
                                     PIN_PUPDR_PULLUP(2) |         \
                                     PIN_PUPDR_PULLUP(3) |         \
                                     PIN_PUPDR_PULLUP(4) |         \
                                     PIN_PUPDR_PULLUP(5) |         \
                                     PIN_PUPDR_PULLUP(6) |         \
                                     PIN_PUPDR_PULLUP(7) |         \
                                     PIN_PUPDR_PULLUP(8) |         \
                                     PIN_PUPDR_PULLUP(9) |         \
                                     PIN_PUPDR_PULLUP(10) |        \
                                     PIN_PUPDR_PULLUP(11) |        \
                                     PIN_PUPDR_PULLUP(12) |        \
                                     PIN_PUPDR_PULLUP(13) |        \
                                     PIN_PUPDR_PULLUP(14) |        \
                                     PIN_PUPDR_PULLUP(15))
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(0) |             \
                                     PIN_ODR_HIGH(1) |             \
                                     PIN_ODR_HIGH(2) |             \
                                     PIN_ODR_HIGH(3) |             \
                                     PIN_ODR_HIGH(4) |             \
                                     PIN_ODR_HIGH(5) |             \
                                     PIN_ODR_HIGH(6) |             \
                                     PIN_ODR_HIGH(7) |             \
                                     PIN_ODR_HIGH(8) |             \
                                     PIN_ODR_HIGH(9) |             \
                                     PIN_ODR_HIGH(10) |            \
                                     PIN_ODR_HIGH(11) |            \
                                     PIN_ODR_HIGH(12) |            \
                                     PIN_ODR_HIGH(13) |            \
                                     PIN_ODR_HIGH(14) |            \
                                     PIN_ODR_HIGH(15))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(0, 0U) |          \
                                     PIN_AFIO_AF(1, 0U) |          \
                                     PIN_AFIO_AF(2, 0U) |          \
                                     PIN_AFIO_AF(3, 0U) |          \
                                     PIN_AFIO_AF(4, 0U) |          \
                                     PIN_AFIO_AF(5, 0U) |          \
                                     PIN_AFIO_AF(6, 0U) |          \
                                     PIN_AFIO_AF(7, 0U))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(8, 0U) |          \
                                     PIN_AFIO_AF(9, 0U) |          \
                                     PIN_AFIO_AF(10, 0U) |         \
                                     PIN_AFIO_AF(11, 0U) |         \
                                     PIN_AFIO_AF(12, 0U) |         \
                                     PIN_AFIO_AF(13, 0U) |         \
                                     PIN_AFIO_AF(14, 0U) |         \
                                     PIN_AFIO_AF(15, 0U))

/*
 * GPIOB setup:
 *
 * PB0  - PIN0                      (input pullup).
 * PB1  - PIN1                      (input pullup).
 * PB2  - PIN2                      (input pullup).
 * PB3  - PIN3                      JTDO/SWO (routed to JTAG connector pin 6)
 * PB4  - PIN4                      JNTRST (routed to JTAG connector pin 7)
 * PB5  - PIN5                      (input pullup).
 * PB6  - PIN6                      (input pullup).
 * PB7  - PIN7                      (input pullup).
 * PB8  - PIN8                      (input pullup).
 * PB9  - PIN9                      (input pullup).
 * PB10 - PIN10                     (input pullup).
 * PB11 - PIN11                     (input pullup).
 * PB12 - PIN12                     (input pullup).
 * PB13 - PIN13                     (input pullup).
 * PB14 - PIN14                     (input pullup).
 * PB15 - PIN15                     (input pullup).
 */
#define VAL_GPIOB_MODER             (PIN_MODE_INPUT(0) |           \
                                     PIN_MODE_INPUT(1) |           \
                                     PIN_MODE_INPUT(2) |           \
                                     PIN_MODE_ALTERNATE(3) |       \
                                     PIN_MODE_ALTERNATE(4) |       \
                                     PIN_MODE_INPUT(5) |           \
                                     PIN_MODE_INPUT(6) |           \
                                     PIN_MODE_INPUT(7) |           \
                                     PIN_MODE_INPUT(8) |           \
                                     PIN_MODE_INPUT(9) |           \
                                     PIN_MODE_INPUT(10) |          \
                                     PIN_MODE_INPUT(11) |          \
                                     PIN_MODE_INPUT(12) |          \
                                     PIN_MODE_INPUT(13) |          \
                                     PIN_MODE_INPUT(14) |          \
                                     PIN_MODE_INPUT(15))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(0) |       \
                                     PIN_OTYPE_PUSHPULL(1) |       \
                                     PIN_OTYPE_PUSHPULL(2) |       \
                                     PIN_OTYPE_PUSHPULL(3) |       \
                                     PIN_OTYPE_PUSHPULL(4) |       \
                                     PIN_OTYPE_PUSHPULL(5) |       \
                                     PIN_OTYPE_PUSHPULL(6) |       \
                                     PIN_OTYPE_PUSHPULL(7) |       \
                                     PIN_OTYPE_PUSHPULL(8) |       \
                                     PIN_OTYPE_PUSHPULL(9) |       \
                                     PIN_OTYPE_PUSHPULL(10) |      \
                                     PIN_OTYPE_PUSHPULL(11) |      \
                                     PIN_OTYPE_PUSHPULL(12) |      \
                                     PIN_OTYPE_PUSHPULL(13) |      \
                                     PIN_OTYPE_PUSHPULL(14) |      \
                                     PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_VERYLOW(0) |       \
                                     PIN_OSPEED_VERYLOW(1) |       \
                                     PIN_OSPEED_VERYLOW(2) |       \
                                     PIN_OSPEED_HIGH(3) |          \
                                     PIN_OSPEED_HIGH(4) |          \
                                     PIN_OSPEED_VERYLOW(5) |       \
                                     PIN_OSPEED_VERYLOW(6) |       \
                                     PIN_OSPEED_VERYLOW(7) |       \
                                     PIN_OSPEED_VERYLOW(8) |       \
                                     PIN_OSPEED_VERYLOW(9) |       \
                                     PIN_OSPEED_VERYLOW(10) |      \
                                     PIN_OSPEED_VERYLOW(11) |      \
                                     PIN_OSPEED_VERYLOW(12) |      \
                                     PIN_OSPEED_VERYLOW(13) |      \
                                     PIN_OSPEED_VERYLOW(14) |      \
                                     PIN_OSPEED_VERYLOW(15))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_PULLUP(0) |         \
                                     PIN_PUPDR_PULLUP(1) |         \
                                     PIN_PUPDR_PULLUP(2) |         \
                                     PIN_PUPDR_PULLUP(3) |         \
                                     PIN_PUPDR_PULLUP(4) |         \
                                     PIN_PUPDR_PULLUP(5) |         \
                                     PIN_PUPDR_PULLUP(6) |         \
                                     PIN_PUPDR_PULLUP(7) |         \
                                     PIN_PUPDR_PULLUP(8) |         \
                                     PIN_PUPDR_PULLUP(9) |         \
                                     PIN_PUPDR_PULLUP(10) |        \
                                     PIN_PUPDR_PULLUP(11) |        \
                                     PIN_PUPDR_PULLUP(12) |        \
                                     PIN_PUPDR_PULLUP(13) |        \
                                     PIN_PUPDR_PULLUP(14) |        \
                                     PIN_PUPDR_PULLUP(15))
#define VAL_GPIOB_ODR               (PIN_ODR_HIGH(0) |             \
                                     PIN_ODR_HIGH(1) |             \
                                     PIN_ODR_HIGH(2) |             \
                                     PIN_ODR_HIGH(3) |             \
                                     PIN_ODR_HIGH(4) |             \
                                     PIN_ODR_HIGH(5) |             \
                                     PIN_ODR_HIGH(6) |             \
                                     PIN_ODR_HIGH(7) |             \
                                     PIN_ODR_HIGH(8) |             \
                                     PIN_ODR_HIGH(9) |             \
                                     PIN_ODR_HIGH(10) |            \
                                     PIN_ODR_HIGH(11) |            \
                                     PIN_ODR_HIGH(12) |            \
                                     PIN_ODR_HIGH(13) |            \
                                     PIN_ODR_HIGH(14) |            \
                                     PIN_ODR_HIGH(15))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(0, 0U) |          \
                                     PIN_AFIO_AF(1, 0U) |          \
                                     PIN_AFIO_AF(2, 0U) |          \
                                     PIN_AFIO_AF(3, 0U) |          \
                                     PIN_AFIO_AF(4, 0U) |          \
                                     PIN_AFIO_AF(5, 0U) |          \
                                     PIN_AFIO_AF(6, 0U) |          \
                                     PIN_AFIO_AF(7, 0U))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(8, 0U) |          \
                                     PIN_AFIO_AF(9, 0U) |          \
                                     PIN_AFIO_AF(10, 0U) |         \
                                     PIN_AFIO_AF(11, 0U) |         \
                                     PIN_AFIO_AF(12, 0U) |         \
                                     PIN_AFIO_AF(13, 0U) |         \
                                     PIN_AFIO_AF(14, 0U) |         \
                                     PIN_AFIO_AF(15, 0U))

/*
 * GPIOC setup:
 *
 * PC0  - PIN0                      (input pullup).
 * PC1  - PIN1                      (input pullup).
 * PC2  - PIN2                      (input pullup).
 * PC3  - PIN3                      (input pullup).
 * PC4  - PIN4                      (input pullup).
 * PC5  - PIN5                      (input pullup).
 * PC6  - PIN6                      (input pullup).
 * PC7  - PIN7                      (input pullup).
 * PC8  - PIN8                      (input pullup).
 * PC9  - PIN9                      (input pullup).
 * PC10 - PIN10                     (input pullup).
 * PC11 - PIN11                     (input pullup).
 * PC12 - PIN12                     (input pullup).
 * PC13 - PIN13                     (input pullup).
 * PC14 - PIN14                     (input pullup).
 * PC15 - PIN15                     (input pullup).
 */
#define VAL_GPIOC_MODER             (PIN_MODE_INPUT(0) |           \
                                     PIN_MODE_INPUT(1) |           \
                                     PIN_MODE_INPUT(2) |           \
                                     PIN_MODE_INPUT(3) |           \
                                     PIN_MODE_INPUT(4) |           \
                                     PIN_MODE_INPUT(5) |           \
                                     PIN_MODE_INPUT(6) |           \
                                     PIN_MODE_INPUT(7) |           \
                                     PIN_MODE_INPUT(8) |           \
                                     PIN_MODE_INPUT(9) |           \
                                     PIN_MODE_INPUT(10) |          \
                                     PIN_MODE_INPUT(11) |          \
                                     PIN_MODE_INPUT(12) |          \
                                     PIN_MODE_INPUT(13) |          \
                                     PIN_MODE_INPUT(14) |          \
                                     PIN_MODE_INPUT(15))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(0) |       \
                                     PIN_OTYPE_PUSHPULL(1) |       \
                                     PIN_OTYPE_PUSHPULL(2) |       \
                                     PIN_OTYPE_PUSHPULL(3) |       \
                                     PIN_OTYPE_PUSHPULL(4) |       \
                                     PIN_OTYPE_PUSHPULL(5) |       \
                                     PIN_OTYPE_PUSHPULL(6) |       \
                                     PIN_OTYPE_PUSHPULL(7) |       \
                                     PIN_OTYPE_PUSHPULL(8) |       \
                                     PIN_OTYPE_PUSHPULL(9) |       \
                                     PIN_OTYPE_PUSHPULL(10) |      \
                                     PIN_OTYPE_PUSHPULL(11) |      \
                                     PIN_OTYPE_PUSHPULL(12) |      \
                                     PIN_OTYPE_PUSHPULL(13) |      \
                                     PIN_OTYPE_PUSHPULL(14) |      \
                                     PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_VERYLOW(0) |       \
                                     PIN_OSPEED_VERYLOW(1) |       \
                                     PIN_OSPEED_VERYLOW(2) |       \
                                     PIN_OSPEED_VERYLOW(3) |       \
                                     PIN_OSPEED_VERYLOW(4) |       \
                                     PIN_OSPEED_VERYLOW(5) |       \
                                     PIN_OSPEED_VERYLOW(6) |       \
                                     PIN_OSPEED_VERYLOW(7) |       \
                                     PIN_OSPEED_VERYLOW(8) |       \
                                     PIN_OSPEED_VERYLOW(9) |       \
                                     PIN_OSPEED_VERYLOW(10) |      \
                                     PIN_OSPEED_VERYLOW(11) |      \
                                     PIN_OSPEED_VERYLOW(12) |      \
                                     PIN_OSPEED_VERYLOW(13) |      \
                                     PIN_OSPEED_VERYLOW(14) |      \
                                     PIN_OSPEED_VERYLOW(15))
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_PULLUP(0) |         \
                                     PIN_PUPDR_PULLUP(1) |         \
                                     PIN_PUPDR_PULLUP(2) |         \
                                     PIN_PUPDR_PULLUP(3) |         \
                                     PIN_PUPDR_PULLUP(4) |         \
                                     PIN_PUPDR_PULLUP(5) |         \
                                     PIN_PUPDR_PULLUP(6) |         \
                                     PIN_PUPDR_PULLUP(7) |         \
                                     PIN_PUPDR_PULLUP(8) |         \
                                     PIN_PUPDR_PULLUP(9) |         \
                                     PIN_PUPDR_PULLUP(10) |        \
                                     PIN_PUPDR_PULLUP(11) |        \
                                     PIN_PUPDR_PULLUP(12) |        \
                                     PIN_PUPDR_PULLUP(13) |        \
                                     PIN_PUPDR_PULLUP(14) |        \
                                     PIN_PUPDR_PULLUP(15))
#define VAL_GPIOC_ODR               (PIN_ODR_HIGH(0) |             \
                                     PIN_ODR_HIGH(1) |             \
                                     PIN_ODR_HIGH(2) |             \
                                     PIN_ODR_HIGH(3) |             \
                                     PIN_ODR_HIGH(4) |             \
                                     PIN_ODR_HIGH(5) |             \
                                     PIN_ODR_HIGH(6) |             \
                                     PIN_ODR_HIGH(7) |             \
                                     PIN_ODR_HIGH(8) |             \
                                     PIN_ODR_HIGH(9) |             \
                                     PIN_ODR_HIGH(10) |            \
                                     PIN_ODR_HIGH(11) |            \
                                     PIN_ODR_HIGH(12) |            \
                                     PIN_ODR_HIGH(13) |            \
                                     PIN_ODR_HIGH(14) |            \
                                     PIN_ODR_HIGH(15))
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(0, 0U) |          \
                                     PIN_AFIO_AF(1, 0U) |          \
                                     PIN_AFIO_AF(2, 0U) |          \
                                     PIN_AFIO_AF(3, 0U) |          \
                                     PIN_AFIO_AF(4, 0U) |          \
                                     PIN_AFIO_AF(5, 0U) |          \
                                     PIN_AFIO_AF(6, 0U) |          \
                                     PIN_AFIO_AF(7, 0U))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(8, 0U) |          \
                                     PIN_AFIO_AF(9, 0U) |          \
                                     PIN_AFIO_AF(10, 0U) |         \
                                     PIN_AFIO_AF(11, 0U) |         \
                                     PIN_AFIO_AF(12, 0U) |         \
                                     PIN_AFIO_AF(13, 0U) |         \
                                     PIN_AFIO_AF(14, 0U) |         \
                                     PIN_AFIO_AF(15, 0U))

/*
 * GPIOD setup:
 *
 * PD0  - PIN0                      (input pullup).
 * PD1  - PIN1                      (input pullup).
 * PD2  - PIN2                      (input pullup).
 * PD3  - PIN3                      (input pullup).
 * PD4  - PIN4                      (input pullup).
 * PD5  - PIN5                      (input pullup).
 * PD6  - PIN6                      (input pullup).
 * PD7  - PIN7                      (input pullup).
 * PD8  - PIN8                      (input pullup).
 * PD9  - PIN9                      (input pullup).
 * PD10 - PIN10                     (input pullup).
 * PD11 - PIN11                     (input pullup).
 * PD12 - PIN12                     (input pullup).
 * PD13 - PIN13                     (input pullup).
 * PD14 - PIN14                     (input pullup).
 * PD15 - PIN15                     (input pullup).
 */
#define VAL_GPIOD_MODER             (PIN_MODE_INPUT(0) |           \
                                     PIN_MODE_INPUT(1) |           \
                                     PIN_MODE_INPUT(2) |           \
                                     PIN_MODE_INPUT(3) |           \
                                     PIN_MODE_INPUT(4) |           \
                                     PIN_MODE_INPUT(5) |           \
                                     PIN_MODE_INPUT(6) |           \
                                     PIN_MODE_INPUT(7) |           \
                                     PIN_MODE_INPUT(8) |           \
                                     PIN_MODE_INPUT(9) |           \
                                     PIN_MODE_INPUT(10) |          \
                                     PIN_MODE_INPUT(11) |          \
                                     PIN_MODE_INPUT(12) |          \
                                     PIN_MODE_INPUT(13) |          \
                                     PIN_MODE_INPUT(14) |          \
                                     PIN_MODE_INPUT(15))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(0) |       \
                                     PIN_OTYPE_PUSHPULL(1) |       \
                                     PIN_OTYPE_PUSHPULL(2) |       \
                                     PIN_OTYPE_PUSHPULL(3) |       \
                                     PIN_OTYPE_PUSHPULL(4) |       \
                                     PIN_OTYPE_PUSHPULL(5) |       \
                                     PIN_OTYPE_PUSHPULL(6) |       \
                                     PIN_OTYPE_PUSHPULL(7) |       \
                                     PIN_OTYPE_PUSHPULL(8) |       \
                                     PIN_OTYPE_PUSHPULL(9) |       \
                                     PIN_OTYPE_PUSHPULL(10) |      \
                                     PIN_OTYPE_PUSHPULL(11) |      \
                                     PIN_OTYPE_PUSHPULL(12) |      \
                                     PIN_OTYPE_PUSHPULL(13) |      \
                                     PIN_OTYPE_PUSHPULL(14) |      \
                                     PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_VERYLOW(0) |       \
                                     PIN_OSPEED_VERYLOW(1) |       \
                                     PIN_OSPEED_VERYLOW(2) |       \
                                     PIN_OSPEED_VERYLOW(3) |       \
                                     PIN_OSPEED_VERYLOW(4) |       \
                                     PIN_OSPEED_VERYLOW(5) |       \
                                     PIN_OSPEED_VERYLOW(6) |       \
                                     PIN_OSPEED_VERYLOW(7) |       \
                                     PIN_OSPEED_VERYLOW(8) |       \
                                     PIN_OSPEED_VERYLOW(9) |       \
                                     PIN_OSPEED_VERYLOW(10) |      \
                                     PIN_OSPEED_VERYLOW(11) |      \
                                     PIN_OSPEED_VERYLOW(12) |      \
                                     PIN_OSPEED_VERYLOW(13) |      \
                                     PIN_OSPEED_VERYLOW(14) |      \
                                     PIN_OSPEED_VERYLOW(15))
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_PULLUP(0) |         \
                                     PIN_PUPDR_PULLUP(1) |         \
                                     PIN_PUPDR_PULLUP(2) |         \
                                     PIN_PUPDR_PULLUP(3) |         \
                                     PIN_PUPDR_PULLUP(4) |         \
                                     PIN_PUPDR_PULLUP(5) |         \
                                     PIN_PUPDR_PULLUP(6) |         \
                                     PIN_PUPDR_PULLUP(7) |         \
                                     PIN_PUPDR_PULLUP(8) |         \
                                     PIN_PUPDR_PULLUP(9) |         \
                                     PIN_PUPDR_PULLUP(10) |        \
                                     PIN_PUPDR_PULLUP(11) |        \
                                     PIN_PUPDR_PULLUP(12) |        \
                                     PIN_PUPDR_PULLUP(13) |        \
                                     PIN_PUPDR_PULLUP(14) |        \
                                     PIN_PUPDR_PULLUP(15))
#define VAL_GPIOD_ODR               (PIN_ODR_HIGH(0) |             \
                                     PIN_ODR_HIGH(1) |             \
                                     PIN_ODR_HIGH(2) |             \
                                     PIN_ODR_HIGH(3) |             \
                                     PIN_ODR_HIGH(4) |             \
                                     PIN_ODR_HIGH(5) |             \
                                     PIN_ODR_HIGH(6) |             \
                                     PIN_ODR_HIGH(7) |             \
                                     PIN_ODR_HIGH(8) |             \
                                     PIN_ODR_HIGH(9) |             \
                                     PIN_ODR_HIGH(10) |            \
                                     PIN_ODR_HIGH(11) |            \
                                     PIN_ODR_HIGH(12) |            \
                                     PIN_ODR_HIGH(13) |            \
                                     PIN_ODR_HIGH(14) |            \
                                     PIN_ODR_HIGH(15))
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(0, 0U) |          \
                                     PIN_AFIO_AF(1, 0U) |          \
                                     PIN_AFIO_AF(2, 0U) |          \
                                     PIN_AFIO_AF(3, 0U) |          \
                                     PIN_AFIO_AF(4, 0U) |          \
                                     PIN_AFIO_AF(5, 0U) |          \
                                     PIN_AFIO_AF(6, 0U) |          \
                                     PIN_AFIO_AF(7, 0U))
#define VAL_GPIOD_AFRH              (PIN_AFIO_AF(8, 0U) |          \
                                     PIN_AFIO_AF(9, 0U) |          \
                                     PIN_AFIO_AF(10, 0U) |         \
                                     PIN_AFIO_AF(11, 0U) |         \
                                     PIN_AFIO_AF(12, 0U) |         \
                                     PIN_AFIO_AF(13, 0U) |         \
                                     PIN_AFIO_AF(14, 0U) |         \
                                     PIN_AFIO_AF(15, 0U))

/*
 * GPIOE setup:
 *
 * PE0  - PIN0                      (input pullup).
 * PE1  - PIN1                      (input pullup).
 * PE2  - PIN2                      (input pullup).
 * PE3  - PIN3                      (input pullup).
 * PE4  - PIN4                      (input pullup).
 * PE5  - PIN5                      (input pullup).
 * PE6  - PIN6                      (input pullup).
 * PE7  - PIN7                      (input pullup).
 * PE8  - PIN8                      (input pullup).
 * PE9  - PIN9                      (input pullup).
 * PE10 - PIN10                     (input pullup).
 * PE11 - PIN11                     (input pullup).
 * PE12 - SPI1_CS   - L9779 - GPIO mode
 * PE13 - SPI1_SCK  - L9779 - AF4
 * PE14 - SPI1_MISO - L9779 - AF4
 * PE15 - SPI1_MOSI - L9779 - AF4
 */
#define VAL_GPIOE_MODER             (PIN_MODE_INPUT(0) |           \
                                     PIN_MODE_INPUT(1) |           \
                                     PIN_MODE_INPUT(2) |           \
                                     PIN_MODE_INPUT(3) |           \
                                     PIN_MODE_INPUT(4) |           \
                                     PIN_MODE_INPUT(5) |           \
                                     PIN_MODE_INPUT(6) |           \
                                     PIN_MODE_INPUT(7) |           \
                                     PIN_MODE_INPUT(8) |           \
                                     PIN_MODE_INPUT(9) |           \
                                     PIN_MODE_INPUT(10) |          \
                                     PIN_MODE_INPUT(11) |          \
                                     PIN_MODE_OUTPUT(12) |         \
                                     PIN_MODE_OUTPUT(13) |         \
                                     PIN_MODE_INPUT(14) |          \
                                     PIN_MODE_OUTPUT(15))
#define VAL_GPIOE_OTYPER            (PIN_OTYPE_PUSHPULL(0) |       \
                                     PIN_OTYPE_PUSHPULL(1) |       \
                                     PIN_OTYPE_PUSHPULL(2) |       \
                                     PIN_OTYPE_PUSHPULL(3) |       \
                                     PIN_OTYPE_PUSHPULL(4) |       \
                                     PIN_OTYPE_PUSHPULL(5) |       \
                                     PIN_OTYPE_PUSHPULL(6) |       \
                                     PIN_OTYPE_PUSHPULL(7) |       \
                                     PIN_OTYPE_PUSHPULL(8) |       \
                                     PIN_OTYPE_PUSHPULL(9) |       \
                                     PIN_OTYPE_PUSHPULL(10) |      \
                                     PIN_OTYPE_PUSHPULL(11) |      \
                                     PIN_OTYPE_PUSHPULL(12) |      \
                                     PIN_OTYPE_PUSHPULL(13) |      \
                                     PIN_OTYPE_PUSHPULL(14) |      \
                                     PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOE_OSPEEDR           (PIN_OSPEED_VERYLOW(0) |       \
                                     PIN_OSPEED_VERYLOW(1) |       \
                                     PIN_OSPEED_VERYLOW(2) |       \
                                     PIN_OSPEED_VERYLOW(3) |       \
                                     PIN_OSPEED_VERYLOW(4) |       \
                                     PIN_OSPEED_VERYLOW(5) |       \
                                     PIN_OSPEED_VERYLOW(6) |       \
                                     PIN_OSPEED_VERYLOW(7) |       \
                                     PIN_OSPEED_VERYLOW(8) |       \
                                     PIN_OSPEED_VERYLOW(9) |       \
                                     PIN_OSPEED_VERYLOW(10) |      \
                                     PIN_OSPEED_VERYLOW(11) |      \
                                     PIN_OSPEED_VERYLOW(12) |      \
                                     PIN_OSPEED_VERYLOW(13) |      \
                                     PIN_OSPEED_VERYLOW(14) |      \
                                     PIN_OSPEED_VERYLOW(15))
#define VAL_GPIOE_PUPDR             (PIN_PUPDR_PULLUP(0) |         \
                                     PIN_PUPDR_PULLUP(1) |         \
                                     PIN_PUPDR_PULLUP(2) |         \
                                     PIN_PUPDR_PULLUP(3) |         \
                                     PIN_PUPDR_PULLUP(4) |         \
                                     PIN_PUPDR_PULLUP(5) |         \
                                     PIN_PUPDR_PULLUP(6) |         \
                                     PIN_PUPDR_PULLUP(7) |         \
                                     PIN_PUPDR_PULLUP(8) |         \
                                     PIN_PUPDR_PULLUP(9) |         \
                                     PIN_PUPDR_PULLUP(10) |        \
                                     PIN_PUPDR_PULLUP(11) |        \
                                     PIN_PUPDR_PULLUP(12) |        \
                                     PIN_PUPDR_PULLUP(13) |        \
                                     PIN_PUPDR_PULLUP(14) |        \
                                     PIN_PUPDR_PULLUP(15))
#define VAL_GPIOE_ODR               (PIN_ODR_HIGH(0) |             \
                                     PIN_ODR_HIGH(1) |             \
                                     PIN_ODR_HIGH(2) |             \
                                     PIN_ODR_HIGH(3) |             \
                                     PIN_ODR_HIGH(4) |             \
                                     PIN_ODR_HIGH(5) |             \
                                     PIN_ODR_HIGH(6) |             \
                                     PIN_ODR_HIGH(7) |             \
                                     PIN_ODR_HIGH(8) |             \
                                     PIN_ODR_HIGH(9) |             \
                                     PIN_ODR_HIGH(10) |            \
                                     PIN_ODR_HIGH(11) |            \
                                     PIN_ODR_HIGH(12) |            \
                                     PIN_ODR_HIGH(13) |            \
                                     PIN_ODR_HIGH(14) |            \
                                     PIN_ODR_HIGH(15))
#define VAL_GPIOE_AFRL              (PIN_AFIO_AF(0, 0U) |          \
                                     PIN_AFIO_AF(1, 0U) |          \
                                     PIN_AFIO_AF(2, 0U) |          \
                                     PIN_AFIO_AF(3, 0U) |          \
                                     PIN_AFIO_AF(4, 0U) |          \
                                     PIN_AFIO_AF(5, 0U) |          \
                                     PIN_AFIO_AF(6, 0U) |          \
                                     PIN_AFIO_AF(7, 0U))
#define VAL_GPIOE_AFRH              (PIN_AFIO_AF(8, 0U) |          \
                                     PIN_AFIO_AF(9, 0U) |          \
                                     PIN_AFIO_AF(10, 0U) |         \
                                     PIN_AFIO_AF(11, 0U) |         \
                                     PIN_AFIO_AF(12, 0U) |         \
                                     PIN_AFIO_AF(13, 4U) |         \
                                     PIN_AFIO_AF(14, 4U) |         \
                                     PIN_AFIO_AF(15, 4U))

/*
 * GPIOF setup:
 *
 * PF0  - PIN0                      (input pullup).
 * PF1  - PIN1                      (input pullup).
 * PF2  - PIN2                      (input pullup).
 * PF3  - PIN3                      (input pullup).
 * PF4  - PIN4                      (input pullup).
 * PF5  - PIN5                      (input pullup).
 * PF6  - PIN6                      (input pullup).
 * PF7  - PIN7                      (input pullup).
 * PF8  - PIN8                      (input pullup).
 * PF9  - PIN9                      (input pullup).
 * PF10 - PIN10                     (input pullup).
 * PF11 - PIN11                     (input pullup).
 * PF12 - PIN12                     (input pullup).
 * PF13 - PIN13                     (input pullup).
 * PF14 - PIN14                     (input pullup).
 * PF15 - PIN15                     (input pullup).
 */
#define VAL_GPIOF_MODER             (PIN_MODE_INPUT(0) |           \
                                     PIN_MODE_INPUT(1) |           \
                                     PIN_MODE_INPUT(2) |           \
                                     PIN_MODE_INPUT(3) |           \
                                     PIN_MODE_INPUT(4) |           \
                                     PIN_MODE_INPUT(5) |           \
                                     PIN_MODE_INPUT(6) |           \
                                     PIN_MODE_INPUT(7) |           \
                                     PIN_MODE_INPUT(8) |           \
                                     PIN_MODE_INPUT(9) |           \
                                     PIN_MODE_INPUT(10) |          \
                                     PIN_MODE_INPUT(11) |          \
                                     PIN_MODE_INPUT(12) |          \
                                     PIN_MODE_INPUT(13) |          \
                                     PIN_MODE_INPUT(14) |          \
                                     PIN_MODE_INPUT(15))
#define VAL_GPIOF_OTYPER            (PIN_OTYPE_PUSHPULL(0) |       \
                                     PIN_OTYPE_PUSHPULL(1) |       \
                                     PIN_OTYPE_PUSHPULL(2) |       \
                                     PIN_OTYPE_PUSHPULL(3) |       \
                                     PIN_OTYPE_PUSHPULL(4) |       \
                                     PIN_OTYPE_PUSHPULL(5) |       \
                                     PIN_OTYPE_PUSHPULL(6) |       \
                                     PIN_OTYPE_PUSHPULL(7) |       \
                                     PIN_OTYPE_PUSHPULL(8) |       \
                                     PIN_OTYPE_PUSHPULL(9) |       \
                                     PIN_OTYPE_PUSHPULL(10) |      \
                                     PIN_OTYPE_PUSHPULL(11) |      \
                                     PIN_OTYPE_PUSHPULL(12) |      \
                                     PIN_OTYPE_PUSHPULL(13) |      \
                                     PIN_OTYPE_PUSHPULL(14) |      \
                                     PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOF_OSPEEDR           (PIN_OSPEED_VERYLOW(0) |       \
                                     PIN_OSPEED_VERYLOW(1) |       \
                                     PIN_OSPEED_VERYLOW(2) |       \
                                     PIN_OSPEED_VERYLOW(3) |       \
                                     PIN_OSPEED_VERYLOW(4) |       \
                                     PIN_OSPEED_VERYLOW(5) |       \
                                     PIN_OSPEED_VERYLOW(6) |       \
                                     PIN_OSPEED_VERYLOW(7) |       \
                                     PIN_OSPEED_VERYLOW(8) |       \
                                     PIN_OSPEED_VERYLOW(9) |       \
                                     PIN_OSPEED_VERYLOW(10) |      \
                                     PIN_OSPEED_VERYLOW(11) |      \
                                     PIN_OSPEED_VERYLOW(12) |      \
                                     PIN_OSPEED_VERYLOW(13) |      \
                                     PIN_OSPEED_VERYLOW(14) |      \
                                     PIN_OSPEED_VERYLOW(15))
#define VAL_GPIOF_PUPDR             (PIN_PUPDR_PULLUP(0) |         \
                                     PIN_PUPDR_PULLUP(1) |         \
                                     PIN_PUPDR_PULLUP(2) |         \
                                     PIN_PUPDR_PULLUP(3) |         \
                                     PIN_PUPDR_PULLUP(4) |         \
                                     PIN_PUPDR_PULLUP(5) |         \
                                     PIN_PUPDR_PULLUP(6) |         \
                                     PIN_PUPDR_PULLUP(7) |         \
                                     PIN_PUPDR_PULLUP(8) |         \
                                     PIN_PUPDR_PULLUP(9) |         \
                                     PIN_PUPDR_PULLUP(10) |        \
                                     PIN_PUPDR_PULLUP(11) |        \
                                     PIN_PUPDR_PULLUP(12) |        \
                                     PIN_PUPDR_PULLUP(13) |        \
                                     PIN_PUPDR_PULLUP(14) |        \
                                     PIN_PUPDR_PULLUP(15))
#define VAL_GPIOF_ODR               (PIN_ODR_HIGH(0) |             \
                                     PIN_ODR_HIGH(1) |             \
                                     PIN_ODR_HIGH(2) |             \
                                     PIN_ODR_HIGH(3) |             \
                                     PIN_ODR_HIGH(4) |             \
                                     PIN_ODR_HIGH(5) |             \
                                     PIN_ODR_HIGH(6) |             \
                                     PIN_ODR_HIGH(7) |             \
                                     PIN_ODR_HIGH(8) |             \
                                     PIN_ODR_HIGH(9) |             \
                                     PIN_ODR_HIGH(10) |            \
                                     PIN_ODR_HIGH(11) |            \
                                     PIN_ODR_HIGH(12) |            \
                                     PIN_ODR_HIGH(13) |            \
                                     PIN_ODR_HIGH(14) |            \
                                     PIN_ODR_HIGH(15))
#define VAL_GPIOF_AFRL              (PIN_AFIO_AF(0, 0U) |          \
                                     PIN_AFIO_AF(1, 0U) |          \
                                     PIN_AFIO_AF(2, 0U) |          \
                                     PIN_AFIO_AF(3, 0U) |          \
                                     PIN_AFIO_AF(4, 0U) |          \
                                     PIN_AFIO_AF(5, 0U) |          \
                                     PIN_AFIO_AF(6, 0U) |          \
                                     PIN_AFIO_AF(7, 0U))
#define VAL_GPIOF_AFRH              (PIN_AFIO_AF(8, 0U) |          \
                                     PIN_AFIO_AF(9, 0U) |          \
                                     PIN_AFIO_AF(10, 0U) |         \
                                     PIN_AFIO_AF(11, 0U) |         \
                                     PIN_AFIO_AF(12, 0U) |         \
                                     PIN_AFIO_AF(13, 0U) |         \
                                     PIN_AFIO_AF(14, 0U) |         \
                                     PIN_AFIO_AF(15, 0U))

/*
 * GPIOG setup:
 *
 * PG0  - PIN0                      (input pullup).
 * PG1  - PIN1                      (input pullup).
 * PG2  - PIN2                      (input pullup).
 * PG3  - PIN3                      (input pullup).
 * PG4  - PIN4                      (input pullup).
 * PG5  - PIN5                      (input pullup).
 * PG6  - PIN6                      (input pullup).
 * PG7  - PIN7                      (input pullup).
 * PG8  - PIN8                      (input pullup).
 * PG9  - PIN9                      (input pullup).
 * PG10 - PIN10                     (input pullup).
 * PG11 - PIN11                     (input pullup).
 * PG12 - PIN12                     (input pullup).
 * PG13 - PIN13                     (input pullup).
 * PG14 - PIN14                     (input pullup).
 * PG15 - PIN15                     (input pullup).
 */
#define VAL_GPIOG_MODER             (PIN_MODE_INPUT(0) |           \
                                     PIN_MODE_INPUT(1) |           \
                                     PIN_MODE_INPUT(2) |           \
                                     PIN_MODE_INPUT(3) |           \
                                     PIN_MODE_INPUT(4) |           \
                                     PIN_MODE_INPUT(5) |           \
                                     PIN_MODE_INPUT(6) |           \
                                     PIN_MODE_INPUT(7) |           \
                                     PIN_MODE_INPUT(8) |           \
                                     PIN_MODE_INPUT(9) |           \
                                     PIN_MODE_INPUT(10) |          \
                                     PIN_MODE_INPUT(11) |          \
                                     PIN_MODE_INPUT(12) |          \
                                     PIN_MODE_INPUT(13) |          \
                                     PIN_MODE_INPUT(14) |          \
                                     PIN_MODE_INPUT(15))
#define VAL_GPIOG_OTYPER            (PIN_OTYPE_PUSHPULL(0) |       \
                                     PIN_OTYPE_PUSHPULL(1) |       \
                                     PIN_OTYPE_PUSHPULL(2) |       \
                                     PIN_OTYPE_PUSHPULL(3) |       \
                                     PIN_OTYPE_PUSHPULL(4) |       \
                                     PIN_OTYPE_PUSHPULL(5) |       \
                                     PIN_OTYPE_PUSHPULL(6) |       \
                                     PIN_OTYPE_PUSHPULL(7) |       \
                                     PIN_OTYPE_PUSHPULL(8) |       \
                                     PIN_OTYPE_PUSHPULL(9) |       \
                                     PIN_OTYPE_PUSHPULL(10) |      \
                                     PIN_OTYPE_PUSHPULL(11) |      \
                                     PIN_OTYPE_PUSHPULL(12) |      \
                                     PIN_OTYPE_PUSHPULL(13) |      \
                                     PIN_OTYPE_PUSHPULL(14) |      \
                                     PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOG_OSPEEDR           (PIN_OSPEED_VERYLOW(0) |       \
                                     PIN_OSPEED_VERYLOW(1) |       \
                                     PIN_OSPEED_VERYLOW(2) |       \
                                     PIN_OSPEED_VERYLOW(3) |       \
                                     PIN_OSPEED_VERYLOW(4) |       \
                                     PIN_OSPEED_VERYLOW(5) |       \
                                     PIN_OSPEED_VERYLOW(6) |       \
                                     PIN_OSPEED_VERYLOW(7) |       \
                                     PIN_OSPEED_VERYLOW(8) |       \
                                     PIN_OSPEED_VERYLOW(9) |       \
                                     PIN_OSPEED_VERYLOW(10) |      \
                                     PIN_OSPEED_VERYLOW(11) |      \
                                     PIN_OSPEED_VERYLOW(12) |      \
                                     PIN_OSPEED_VERYLOW(13) |      \
                                     PIN_OSPEED_VERYLOW(14) |      \
                                     PIN_OSPEED_VERYLOW(15))
#define VAL_GPIOG_PUPDR             (PIN_PUPDR_PULLUP(0) |         \
                                     PIN_PUPDR_PULLUP(1) |         \
                                     PIN_PUPDR_PULLUP(2) |         \
                                     PIN_PUPDR_PULLUP(3) |         \
                                     PIN_PUPDR_PULLUP(4) |         \
                                     PIN_PUPDR_PULLUP(5) |         \
                                     PIN_PUPDR_PULLUP(6) |         \
                                     PIN_PUPDR_PULLUP(7) |         \
                                     PIN_PUPDR_PULLUP(8) |         \
                                     PIN_PUPDR_PULLUP(9) |         \
                                     PIN_PUPDR_PULLUP(10) |        \
                                     PIN_PUPDR_PULLUP(11) |        \
                                     PIN_PUPDR_PULLUP(12) |        \
                                     PIN_PUPDR_PULLUP(13) |        \
                                     PIN_PUPDR_PULLUP(14) |        \
                                     PIN_PUPDR_PULLUP(15))
#define VAL_GPIOG_ODR               (PIN_ODR_HIGH(0) |             \
                                     PIN_ODR_HIGH(1) |             \
                                     PIN_ODR_HIGH(2) |             \
                                     PIN_ODR_HIGH(3) |             \
                                     PIN_ODR_HIGH(4) |             \
                                     PIN_ODR_HIGH(5) |             \
                                     PIN_ODR_HIGH(6) |             \
                                     PIN_ODR_HIGH(7) |             \
                                     PIN_ODR_HIGH(8) |             \
                                     PIN_ODR_HIGH(9) |             \
                                     PIN_ODR_HIGH(10) |            \
                                     PIN_ODR_HIGH(11) |            \
                                     PIN_ODR_HIGH(12) |            \
                                     PIN_ODR_HIGH(13) |            \
                                     PIN_ODR_HIGH(14) |            \
                                     PIN_ODR_HIGH(15))
#define VAL_GPIOG_AFRL              (PIN_AFIO_AF(0, 0U) |          \
                                     PIN_AFIO_AF(1, 0U) |          \
                                     PIN_AFIO_AF(2, 0U) |          \
                                     PIN_AFIO_AF(3, 0U) |          \
                                     PIN_AFIO_AF(4, 0U) |          \
                                     PIN_AFIO_AF(5, 0U) |          \
                                     PIN_AFIO_AF(6, 0U) |          \
                                     PIN_AFIO_AF(7, 0U))
#define VAL_GPIOG_AFRH              (PIN_AFIO_AF(8, 0U) |          \
                                     PIN_AFIO_AF(9, 0U) |          \
                                     PIN_AFIO_AF(10, 0U) |         \
                                     PIN_AFIO_AF(11, 0U) |         \
                                     PIN_AFIO_AF(12, 0U) |         \
                                     PIN_AFIO_AF(13, 0U) |         \
                                     PIN_AFIO_AF(14, 0U) |         \
                                     PIN_AFIO_AF(15, 0U))

/*
 * GPIOH setup:
 *
 * PH0  - OSC_IN                    (input floating).
 * PH1  - OSC_OUT                   (input floating).
 * PH2  - PIN2                      (input pullup).
 * PH3  - PIN3                      (input pullup).
 * PH4  - PIN4                      (input pullup).
 * PH5  - PIN5                      (input pullup).
 * PH6  - PIN6                      (input pullup).
 * PH7  - PIN7                      (input pullup).
 * PH8  - PIN8                      (input pullup).
 * PH9  - PIN9                      (input pullup).
 * PH10 - PIN10                     (input pullup).
 * PH11 - PIN11                     (input pullup).
 * PH12 - PIN12                     (input pullup).
 * PH13 - PIN13                     (input pullup).
 * PH14 - PIN14                     (input pullup).
 * PH15 - PIN15                     (input pullup).
 */
#define VAL_GPIOH_MODER             (PIN_MODE_INPUT(0) |           \
                                     PIN_MODE_INPUT(1) |           \
                                     PIN_MODE_INPUT(2) |           \
                                     PIN_MODE_INPUT(3) |           \
                                     PIN_MODE_INPUT(4) |           \
                                     PIN_MODE_INPUT(5) |           \
                                     PIN_MODE_INPUT(6) |           \
                                     PIN_MODE_INPUT(7) |           \
                                     PIN_MODE_INPUT(8) |           \
                                     PIN_MODE_INPUT(9) |           \
                                     PIN_MODE_INPUT(10) |          \
                                     PIN_MODE_INPUT(11) |          \
                                     PIN_MODE_INPUT(12) |          \
                                     PIN_MODE_INPUT(13) |          \
                                     PIN_MODE_INPUT(14) |          \
                                     PIN_MODE_INPUT(15))
#define VAL_GPIOH_OTYPER            (PIN_OTYPE_PUSHPULL(0) |       \
                                     PIN_OTYPE_PUSHPULL(1) |       \
                                     PIN_OTYPE_PUSHPULL(2) |       \
                                     PIN_OTYPE_PUSHPULL(3) |       \
                                     PIN_OTYPE_PUSHPULL(4) |       \
                                     PIN_OTYPE_PUSHPULL(5) |       \
                                     PIN_OTYPE_PUSHPULL(6) |       \
                                     PIN_OTYPE_PUSHPULL(7) |       \
                                     PIN_OTYPE_PUSHPULL(8) |       \
                                     PIN_OTYPE_PUSHPULL(9) |       \
                                     PIN_OTYPE_PUSHPULL(10) |      \
                                     PIN_OTYPE_PUSHPULL(11) |      \
                                     PIN_OTYPE_PUSHPULL(12) |      \
                                     PIN_OTYPE_PUSHPULL(13) |      \
                                     PIN_OTYPE_PUSHPULL(14) |      \
                                     PIN_OTYPE_PUSHPULL(15))
#define VAL_GPIOH_OSPEEDR           (PIN_OSPEED_HIGH(0) |          \
                                     PIN_OSPEED_HIGH(1) |          \
                                     PIN_OSPEED_VERYLOW(2) |       \
                                     PIN_OSPEED_VERYLOW(3) |       \
                                     PIN_OSPEED_VERYLOW(4) |       \
                                     PIN_OSPEED_VERYLOW(5) |       \
                                     PIN_OSPEED_VERYLOW(6) |       \
                                     PIN_OSPEED_VERYLOW(7) |       \
                                     PIN_OSPEED_VERYLOW(8) |       \
                                     PIN_OSPEED_VERYLOW(9) |       \
                                     PIN_OSPEED_VERYLOW(10) |      \
                                     PIN_OSPEED_VERYLOW(11) |      \
                                     PIN_OSPEED_VERYLOW(12) |      \
                                     PIN_OSPEED_VERYLOW(13) |      \
                                     PIN_OSPEED_VERYLOW(14) |      \
                                     PIN_OSPEED_VERYLOW(15))
#define VAL_GPIOH_PUPDR             (PIN_PUPDR_FLOATING(0) |       \
                                     PIN_PUPDR_FLOATING(1) |       \
                                     PIN_PUPDR_PULLUP(2) |         \
                                     PIN_PUPDR_PULLUP(3) |         \
                                     PIN_PUPDR_PULLUP(4) |         \
                                     PIN_PUPDR_PULLUP(5) |         \
                                     PIN_PUPDR_PULLUP(6) |         \
                                     PIN_PUPDR_PULLUP(7) |         \
                                     PIN_PUPDR_PULLUP(8) |         \
                                     PIN_PUPDR_PULLUP(9) |         \
                                     PIN_PUPDR_PULLUP(10) |        \
                                     PIN_PUPDR_PULLUP(11) |        \
                                     PIN_PUPDR_PULLUP(12) |        \
                                     PIN_PUPDR_PULLUP(13) |        \
                                     PIN_PUPDR_PULLUP(14) |        \
                                     PIN_PUPDR_PULLUP(15))
#define VAL_GPIOH_ODR               (PIN_ODR_HIGH(0) |             \
                                     PIN_ODR_HIGH(1) |             \
                                     PIN_ODR_HIGH(2) |             \
                                     PIN_ODR_HIGH(3) |             \
                                     PIN_ODR_HIGH(4) |             \
                                     PIN_ODR_HIGH(5) |             \
                                     PIN_ODR_HIGH(6) |             \
                                     PIN_ODR_HIGH(7) |             \
                                     PIN_ODR_HIGH(8) |             \
                                     PIN_ODR_HIGH(9) |             \
                                     PIN_ODR_HIGH(10) |            \
                                     PIN_ODR_HIGH(11) |            \
                                     PIN_ODR_HIGH(12) |            \
                                     PIN_ODR_HIGH(13) |            \
                                     PIN_ODR_HIGH(14) |            \
                                     PIN_ODR_HIGH(15))
#define VAL_GPIOH_AFRL              (PIN_AFIO_AF(0, 0U) |          \
                                     PIN_AFIO_AF(1, 0U) |          \
                                     PIN_AFIO_AF(2, 0U) |          \
                                     PIN_AFIO_AF(3, 0U) |          \
                                     PIN_AFIO_AF(4, 0U) |          \
                                     PIN_AFIO_AF(5, 0U) |          \
                                     PIN_AFIO_AF(6, 0U) |          \
                                     PIN_AFIO_AF(7, 0U))
#define VAL_GPIOH_AFRH              (PIN_AFIO_AF(8, 0U) |          \
                                     PIN_AFIO_AF(9, 0U) |          \
                                     PIN_AFIO_AF(10, 0U) |         \
                                     PIN_AFIO_AF(11, 0U) |         \
                                     PIN_AFIO_AF(12, 0U) |         \
                                     PIN_AFIO_AF(13, 0U) |         \
                                     PIN_AFIO_AF(14, 0U) |         \
                                     PIN_AFIO_AF(15, 0U))

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* BOARD_H */
