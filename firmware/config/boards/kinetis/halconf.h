/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

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

#ifndef _HALCONF_H_
#define _HALCONF_H_

#include "rusefi_halconf.h"

/**
 * @brief   Enables the EXT subsystem.
 */
#if !defined(HAL_USE_EXT) || defined(__DOXYGEN__)
#define HAL_USE_EXT                 FALSE
#endif

/**
 * @brief   Enables the I2C subsystem.
 */
#if !defined(HAL_USE_I2C) || defined(__DOXYGEN__)
#define HAL_USE_I2C                 FALSE
#endif

/**
 * @brief   Enables the ICU subsystem.
 */
#if !defined(HAL_USE_ICU) || defined(__DOXYGEN__)
#define HAL_USE_ICU                 FALSE
#endif

/**
 * @brief   Enables the PWM subsystem.
 */
#if !defined(HAL_USE_PWM) || defined(__DOXYGEN__)
#define HAL_USE_PWM                 FALSE
#endif

/**
 * @brief   Enables the RTC subsystem.
 */
#if !defined(HAL_USE_RTC) || defined(__DOXYGEN__)
#define HAL_USE_RTC                 FALSE
#endif

/**
 * @brief   Enables the SDC subsystem.
 */
#if !defined(HAL_USE_SDC) || defined(__DOXYGEN__)
#define HAL_USE_SDC                 FALSE
#endif

/**
 * @brief   Enables the SERIAL subsystem.
 */
#if !defined(HAL_USE_SERIAL) || defined(__DOXYGEN__)
#define HAL_USE_SERIAL              FALSE
#endif

/**
 * @brief   Enables the UART subsystem.
 */
#if !defined(HAL_USE_UART) || defined(__DOXYGEN__)
#define HAL_USE_UART                TRUE
#endif

/**
 * @brief   Enables the USB subsystem.
 */
#if !defined(HAL_USE_USB) || defined(__DOXYGEN__)
#define HAL_USE_USB                 FALSE
#endif


/**
 * @brief   Enables the community overlay.
 */
#if !defined(HAL_USE_COMMUNITY) || defined(__DOXYGEN__)
#define HAL_USE_COMMUNITY           TRUE
#endif

/**
 * @brief   Enables the TIMCAP subsystem.
 */
#if !defined(HAL_USE_COMP) || defined(__DOXYGEN__)
#define HAL_USE_COMP                TRUE
#endif

/*===========================================================================*/
/* PAL driver related settings.                                              */
/*===========================================================================*/

/**
 * @brief   Enables synchronous APIs.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(PAL_USE_WAIT) || defined(__DOXYGEN__)
#define PAL_USE_WAIT                        TRUE
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
#define SDC_MMC_SUPPORT             FALSE
#endif

/*===========================================================================*/
/* SPI driver related settings.                                              */
/*===========================================================================*/

#if !defined(SPI_SELECT_MODE) || defined(__DOXYGEN__)
#define SPI_SELECT_MODE             SPI_SELECT_MODE_LLD
#endif

/*===========================================================================*/
/* UART driver related settings.                                             */
/*===========================================================================*/

/**
 * @brief   Enables synchronous APIs.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(UART_USE_WAIT) || defined(__DOXYGEN__)
#define UART_USE_WAIT               TRUE
#endif

/**
 * @brief   Enables the @p uartAcquireBus() and @p uartReleaseBus() APIs.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(UART_USE_MUTUAL_EXCLUSION) || defined(__DOXYGEN__)
#define UART_USE_MUTUAL_EXCLUSION   FALSE
#endif

#endif /* _HALCONF_H_ */

/** @} */
