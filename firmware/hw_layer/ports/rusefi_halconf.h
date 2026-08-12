// file rusefi_halconf.h

#pragma once

#define _CHIBIOS_HAL_CONF_
#define _CHIBIOS_HAL_CONF_VER_8_4_

#include "mcuconf.h"

#include "efifeatures.h"
#include "thread_priority.h"

/*===========================================================================*/
/* Conditional EFI feature settings                                          */
/*===========================================================================*/

// If EFI_CAN_SUPPORT, enable CAN driver
#define HAL_USE_CAN EFI_CAN_SUPPORT

// If EFI_USB_SERIAL, enable USB features
#define HAL_USE_USB EFI_USB_SERIAL
#define HAL_USE_SERIAL_USB EFI_USB_SERIAL

// If EFI_FILE_LOGGING and SDC, enable SDIO/SDMMC driver
#if defined(EFI_SDC_DEVICE) && EFI_FILE_LOGGING
	#define HAL_USE_SDC TRUE
	#define FATFS_HAL_DEVICE EFI_SDC_DEVICE
#else
	#define HAL_USE_SDC FALSE
#endif

/**
 * @brief   Number of initialization attempts before rejecting the card.
 * @note    Attempts are performed at 10mS intervals.
 */
#if !defined(SDC_INIT_RETRY) || defined(__DOXYGEN__)
#define SDC_INIT_RETRY                      100
#endif

/**
 * @brief   Delays insertions.
 * @details If enabled this options inserts delays into the MMC waiting
 *          routines releasing some extra CPU time for the threads with
 *          lower priority, this may slow down the driver a bit however.
 */
#if !defined(SDC_NICE_WAITING) || defined(__DOXYGEN__)
#define SDC_NICE_WAITING                    TRUE
#endif

// If EFI_FILE_LOGGING but not SDC, use SPI instead
#if !defined(EFI_SDC_DEVICE) && EFI_FILE_LOGGING
	#define HAL_USE_MMC_SPI TRUE
#else
	#define HAL_USE_MMC_SPI FALSE
#endif

// If USB and File logging, enable USB Mass Storage & community
#ifndef HAL_USE_USB_MSD
#define HAL_USE_USB_MSD (EFI_FILE_LOGGING && EFI_USB_SERIAL)
#endif // HAL_USE_USB_MSD
#define HAL_USE_COMMUNITY (EFI_FILE_LOGGING && EFI_USB_SERIAL) || HAL_USE_EEPROM
#define USB_MSD_LUN_COUNT 2

// juse enable wait if USB is enabled
#define USB_USE_WAIT HAL_USE_USB

// Ethernet
#define HAL_USE_MAC EFI_ETHERNET

/*===========================================================================*/
/* Required rusEFI settings                                                  */
/*===========================================================================*/

// Enable flags
#ifdef EFI_BOOTLOADER
	// Disable stuff we don't need in the bootloader
	#define HAL_USE_ADC                 FALSE
	#define HAL_USE_GPT                 FALSE
#else
	// We are not the bootloader, enable stuff!
	#ifndef HAL_USE_ADC
	#define HAL_USE_ADC                 TRUE
	#endif // HAL_USE_ADC
	#ifndef HAL_USE_GPT
	#define HAL_USE_GPT                 TRUE
	#endif // HAL_USE_GPT
#endif

#define HAL_USE_PAL                 TRUE

/*===========================================================================*/
/* PAL driver related settings.                                              */
/*===========================================================================*/

/**
 * @brief   No DAC subsystem.
 */
#if !defined(HAL_USE_DAC) || defined(__DOXYGEN__)
#define HAL_USE_DAC                 FALSE
#endif

/**
 * @brief   Enables the SPI subsystem.
 */
#if !defined(HAL_USE_SPI) || defined(__DOXYGEN__)
#define HAL_USE_SPI                         TRUE
#endif

/**
 * @brief   Enables the SDC subsystem.
 */
#if !defined(HAL_USE_SDC) || defined(__DOXYGEN__)
#define HAL_USE_SDC                         FALSE
#endif

/*===========================================================================*/
/* SERIAL driver related settings.                                           */
/*===========================================================================*/

/**
 * @brief   Default bit rate.
 * @details Configuration parameter, this is the baud rate selected for the
 *          default configuration.
 */
#if !defined(SERIAL_DEFAULT_BITRATE) || defined(__DOXYGEN__)
#define SERIAL_DEFAULT_BITRATE              38400
#endif

/**
 * @brief   Serial buffers size.
 * @details Configuration parameter, you can change the depth of the queue
 *          buffers depending on the requirements of your application.
 * @note    The default is 16 bytes for both the transmission and receive
 *          buffers.
 */
#if !defined(SERIAL_BUFFERS_SIZE) || defined(__DOXYGEN__)
#define SERIAL_BUFFERS_SIZE         16
#endif

/**
 * @brief   Enables the @p uartAcquireBus() and @p uartReleaseBus() APIs.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(UART_USE_MUTUAL_EXCLUSION) || defined(__DOXYGEN__)
#define UART_USE_MUTUAL_EXCLUSION           FALSE
#endif

// Options for individual drivers

// ADC
#define ADC_USE_WAIT                TRUE
#define ADC_USE_MUTUAL_EXCLUSION    FALSE
#define CAN_USE_SLEEP_MODE          FALSE

// CAN
#define CAN_USE_SLEEP_MODE          FALSE

// MMC SPI
#define MMC_NICE_WAITING            TRUE

// PAL
#define PAL_USE_CALLBACKS           TRUE

// USB Serial
#ifndef SERIAL_USB_BUFFERS_RX_SIZE
#define SERIAL_USB_BUFFERS_RX_SIZE     64
#endif

#if (SERIAL_USB_BUFFERS_RX_SIZE != 64)
#error Please keep SERIAL_USB_BUFFERS_SIZE until https://forum.chibios.org/viewtopic.php?f=35&t=6395 is properly fixed!
#endif

#ifndef SERIAL_USB_BUFFERS_RX_NUMBER
#define SERIAL_USB_BUFFERS_RX_NUMBER   2
#endif

#ifndef SERIAL_USB_BUFFERS_TX_SIZE
#define SERIAL_USB_BUFFERS_TX_SIZE     512
#endif

#ifndef SERIAL_USB_BUFFERS_TX_NUMBER
#define SERIAL_USB_BUFFERS_TX_NUMBER   2
#endif

// USB Mass Storage
#if EFI_USE_COMPRESSED_INI_MSD
// if enabled, we do gzip decompression on the MSD thread - it requires more stack space
#define USB_MSD_THREAD_WA_SIZE 2048
#endif

// SPI
#define SPI_USE_WAIT                TRUE
#define SPI_USE_MUTUAL_EXCLUSION    TRUE

// Extra field in the UART driver's struct to store a reference to the DMA receive buffer object
#define UART_DRIVER_EXT_FIELDS void* dmaAdapterInstance;

// Ethernet MAC
#define MAC_USE_ZERO_COPY FALSE
#define MAC_USE_EVENTS TRUE

#include "error_handling_c.h"

// WARNING:
// this while loop has non-determinited timeout! Current value is almost random.
// Please consider CPU speed, IRQ load, expected and worst case event wait time!
// Currently this is used instead of simple while (condition) loop in polling SPI driver.
// We exect problems when SPI clock is low and CPU speed is high
#define LIMITED_WHILE_LOOP(condition, ...) \
  { int limit = 1000000 ;                  \
    while (condition) {                    \
      if (limit-- == 0) {                  \
        criticalErrorC(__VA_ARGS__);       \
        break;                             \
      }                                    \
    }                                      \
  }
