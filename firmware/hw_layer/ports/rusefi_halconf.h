#pragma once

#include "efifeatures.h"

/*===========================================================================*/
/* Conditional EFI feature settings                                          */
/*===========================================================================*/

// If EFI_CAN_SUPPORT, enable CAN driver
#define HAL_USE_CAN EFI_CAN_SUPPORT

// If EFI_USB_SERIAL, enable USB features
#define HAL_USE_USB EFI_USB_SERIAL
#define HAL_USE_SERIAL_USB EFI_USB_SERIAL

// If EFI_FILE_LOGGING, enable MMC SPI driver
#define HAL_USE_MMC_SPI EFI_FILE_LOGGING

// If USB and File logging, enable USB Mass Storage & community
#define HAL_USE_USB_MSD (EFI_FILE_LOGGING && EFI_USB_SERIAL)
#define HAL_USE_COMMUNITY (EFI_FILE_LOGGING && EFI_USB_SERIAL)
// only the MSD driver requires USB_USE_WAIT
#define USB_USE_WAIT (EFI_FILE_LOGGING && EFI_USB_SERIAL)

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

// SPI
#define SPI_USE_WAIT                TRUE
#define SPI_USE_MUTUAL_EXCLUSION    TRUE
