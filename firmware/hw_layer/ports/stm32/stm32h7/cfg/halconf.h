/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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

/**
 * @file    templates/halconf.h
 * @brief   HAL configuration header.
 * @details HAL configuration file, this file allows to enable or disable the
 *          various device drivers from your application. You may also use
 *          this file in order to override the device drivers default settings.
 *
 * @addtogroup HAL_CONF
 * @{
 */

#ifndef HALCONF_H
#define HALCONF_H

#include "rusefi_halconf.h"

/**
 * @brief   Enables the cryptographic subsystem.
 */
#if !defined(HAL_USE_CRY) || defined(__DOXYGEN__)
#define HAL_USE_CRY                         FALSE
#endif

/**
 * @brief   Enables the EFlash subsystem.
 */
#if !defined(HAL_USE_EFL) || defined(__DOXYGEN__)
#define HAL_USE_EFL                         FALSE
#endif

/**
 * @brief   Enables the I2C subsystem.
 */
#if !defined(HAL_USE_I2C) || defined(__DOXYGEN__)
#define HAL_USE_I2C                         FALSE
#endif

/**
 * @brief   Enables the ICU subsystem.
 */
#if !defined(HAL_USE_ICU) || defined(__DOXYGEN__)
#define HAL_USE_ICU                         TRUE
#endif

/**
 * @brief   Enables the PWM subsystem.
 */
#if !defined(HAL_USE_PWM) || defined(__DOXYGEN__)
#define HAL_USE_PWM                         TRUE
#endif

/**
 * @brief   Enables the RTC subsystem.
 */
#if !defined(HAL_USE_RTC) || defined(__DOXYGEN__)
#define HAL_USE_RTC                         TRUE
#endif


/**
 * @brief   Enables the SERIAL subsystem.
 */
#if !defined(HAL_USE_SERIAL) || defined(__DOXYGEN__)
#define HAL_USE_SERIAL                      TRUE
#endif

/**
 * @brief   Enables the SIO subsystem.
 */
#if !defined(HAL_USE_SIO) || defined(__DOXYGEN__)
#define HAL_USE_SIO                         FALSE
#endif


/**
 * @brief   Enables the TRNG subsystem.
 */
#if !defined(HAL_USE_TRNG) || defined(__DOXYGEN__)
#define HAL_USE_TRNG                        FALSE
#endif

/**
 * @brief   Enables the UART subsystem.
 */
#if !defined(HAL_USE_UART) || defined(__DOXYGEN__)
/* Configured in efifeatures.h */
#if defined(TS_PRIMARY_UxART_PORT)
#define HAL_USE_UART                TRUE
#else
#define HAL_USE_UART                FALSE
#endif
#endif

/**
 * @brief   Enables the WSPI subsystem.
 */
#if !defined(HAL_USE_WSPI) || defined(__DOXYGEN__)
#define HAL_USE_WSPI                        FALSE
#endif

/**
 * @brief   Enables the WDG subsystem.
 */
#if !defined(HAL_USE_WDG) || defined(__DOXYGEN__)
#define HAL_USE_WDG                 TRUE
#endif

/*===========================================================================*/
/* PAL driver related settings.                                              */
/*===========================================================================*/

/**
 * @brief   Enables synchronous APIs.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(PAL_USE_WAIT) || defined(__DOXYGEN__)
#define PAL_USE_WAIT                        FALSE
#endif

/*===========================================================================*/
/* CAN driver related settings.                                              */
/*===========================================================================*/

/**
 * @brief   Enforces the driver to use direct callbacks rather than OSAL events.
 */
#if !defined(CAN_ENFORCE_USE_CALLBACKS) || defined(__DOXYGEN__)
#define CAN_ENFORCE_USE_CALLBACKS           FALSE
#endif

/*===========================================================================*/
/* CRY driver related settings.                                              */
/*===========================================================================*/

/**
 * @brief   Enables the SW fall-back of the cryptographic driver.
 * @details When enabled, this option, activates a fall-back software
 *          implementation for algorithms not supported by the underlying
 *          hardware.
 * @note    Fall-back implementations may not be present for all algorithms.
 */
#if !defined(HAL_CRY_USE_FALLBACK) || defined(__DOXYGEN__)
#define HAL_CRY_USE_FALLBACK                FALSE
#endif

/**
 * @brief   Makes the driver forcibly use the fall-back implementations.
 */
#if !defined(HAL_CRY_ENFORCE_FALLBACK) || defined(__DOXYGEN__)
#define HAL_CRY_ENFORCE_FALLBACK            FALSE
#endif

/*===========================================================================*/
/* SDC driver related settings.                                              */
/*===========================================================================*/

/**
 * @brief   Include support for MMC cards.
 * @note    MMC support is not yet implemented so this option must be kept
 *          at @p FALSE.
 */
#if !defined(SDC_MMC_SUPPORT) || defined(__DOXYGEN__)
#define SDC_MMC_SUPPORT                     FALSE
#endif

/**
 * @brief   OCR initialization constant for V20 cards.
 */
#if !defined(SDC_INIT_OCR_V20) || defined(__DOXYGEN__)
#define SDC_INIT_OCR_V20                    0x50FF8000U
#endif

/**
 * @brief   OCR initialization constant for non-V20 cards.
 */
#if !defined(SDC_INIT_OCR) || defined(__DOXYGEN__)
#define SDC_INIT_OCR                        0x80100000U
#endif

/*===========================================================================*/
/* SPI driver related settings.                                              */
/*===========================================================================*/

/**
 * @brief   Enables circular transfers APIs.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(SPI_USE_CIRCULAR) || defined(__DOXYGEN__)
#define SPI_USE_CIRCULAR                    FALSE
#endif

/**
 * @brief   Handling method for SPI CS line.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(SPI_SELECT_MODE) || defined(__DOXYGEN__)
#define SPI_SELECT_MODE                     SPI_SELECT_MODE_PAD
#endif

/*===========================================================================*/
/* UART driver related settings.                                             */
/*===========================================================================*/

/**
 * @brief   Enables synchronous APIs.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(UART_USE_WAIT) || defined(__DOXYGEN__)
/* Configured in efifeatures.h */
#if defined(TS_PRIMARY_UxART_PORT)
#define UART_USE_WAIT               TRUE
#else
#define UART_USE_WAIT               FALSE
#endif
#endif


#endif /* HALCONF_H */

/** @} */
