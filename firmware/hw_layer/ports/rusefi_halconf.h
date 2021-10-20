#pragma once

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

// If EFI_FILE_LOGGING but not SDC, use SPI instead
#if !defined(EFI_SDC_DEVICE) && EFI_FILE_LOGGING
	#define HAL_USE_MMC_SPI TRUE
#else
	#define HAL_USE_MMC_SPI FALSE
#endif

// If USB and File logging, enable USB Mass Storage & community
#define HAL_USE_USB_MSD (EFI_FILE_LOGGING && EFI_USB_SERIAL)
#define HAL_USE_COMMUNITY (EFI_FILE_LOGGING && EFI_USB_SERIAL)
#define USB_MSD_LUN_COUNT 2

// only the MSD driver requires USB_USE_WAIT
#define USB_USE_WAIT (EFI_FILE_LOGGING && EFI_USB_SERIAL)

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
	#define HAL_USE_ADC                 TRUE
	#define HAL_USE_GPT                 TRUE
#endif

#define HAL_USE_PAL                 TRUE

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
#define SERIAL_USB_BUFFERS_SIZE     320
#define SERIAL_USB_BUFFERS_NUMBER   2

// USB Mass Storage
#ifdef EFI_USE_COMPRESSED_INI_MSD
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
