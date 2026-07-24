/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio
    Modified for RusEFI

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

#include "pch.h"
#include "mmc_card.h"

#if HAL_USE_SERIAL_USB

#include "usbcfg.h"

#if STM32_USB_USE_OTG1
  #define USBD USBD1
  #define STM32_OTG_ENDPOINTS STM32_OTG1_ENDPOINTS
#elif STM32_USB_USE_OTG2
  #define USBD USBD2
  #define STM32_OTG_ENDPOINTS STM32_OTG2_ENDPOINTS
#else
  #error Serial over USB needs OTG1 or OTG2 to be enabled
#endif

#if HAL_USE_USB_MSD
	#define MSD_IF 0
	#define CDC_INT_IF 1
	#define CDC_DATA_IF 2

	#define USB_MSD_EP_SIZE 64
#else
	#define CDC_INT_IF 0
	#define CDC_DATA_IF 1
#endif

/* Virtual serial port(s) over USB.*/
SerialUSBDriver SDU[NUM_CDC_INSTANCES];

/*
 * Endpoints to be used for CDC ACM.
 */
#if HAL_USE_USB_MSD
  #define USB_CDC_INTERRUPT_REQUEST_EP  (USB_MSD_DATA_EP + 1)
  #define USB_CDC_DATA_REQUEST_EP       (USB_MSD_DATA_EP + 2)
  #define USB_CDC_DATA_AVAILABLE_EP     (USB_MSD_DATA_EP + 2)
#else
  #define USB_CDC_INTERRUPT_REQUEST_EP  (1)
  #define USB_CDC_DATA_REQUEST_EP       (2)
  #define USB_CDC_DATA_AVAILABLE_EP     (2)
#endif

// Sharing same Interrupt EP definetly does not work on linux
#ifndef USB_CDC_INT_EP_HACK
#define USB_CDC_INT_EP_HACK FALSE
#endif

#if USB_CDC_INT_EP_HACK
  #define USB_CDC_DATA_EP_STEP   1
  // Share same EP with CDC interface 0
  #define USB_CDC_DATA_INT_STEP  0
#else
  #define USB_CDC_DATA_EP_STEP   2
  #define USB_CDC_DATA_INT_STEP  2
#endif

// Control and Data interfaces
#define CDC_INTEFACES_PER_INSTANCE   2

#if HAL_USE_USB_CDC_2
  #define CDC_INT_IF_2                   (CDC_INT_IF + CDC_INTEFACES_PER_INSTANCE)
  #define CDC_DATA_IF_2                  (CDC_DATA_IF + CDC_INTEFACES_PER_INSTANCE)
  #define USB_CDC_DATA_REQUEST_EP_2      (USB_CDC_DATA_REQUEST_EP + USB_CDC_DATA_EP_STEP)
  #define USB_CDC_DATA_AVAILABLE_EP_2    (USB_CDC_DATA_AVAILABLE_EP + USB_CDC_DATA_EP_STEP)
  #define USB_CDC_INTERRUPT_REQUEST_EP_2 (USB_CDC_INTERRUPT_REQUEST_EP + USB_CDC_DATA_INT_STEP)
  #if (USB_CDC_DATA_REQUEST_EP_2 >= STM32_OTG_ENDPOINTS) || (USB_CDC_INTERRUPT_REQUEST_EP_2 >= STM32_OTG_ENDPOINTS)
    #error "No enough endpoints"
  #endif
#endif

#if HAL_USE_USB_CDC_3
  #define CDC_INT_IF_3                   (CDC_INT_IF_2 + CDC_INTEFACES_PER_INSTANCE)
  #define CDC_DATA_IF_3                  (CDC_DATA_IF_2 + CDC_INTEFACES_PER_INSTANCE)
  #define USB_CDC_DATA_REQUEST_EP_3      (USB_CDC_DATA_REQUEST_EP_2 + USB_CDC_DATA_EP_STEP)
  #define USB_CDC_DATA_AVAILABLE_EP_3    (USB_CDC_DATA_AVAILABLE_EP_2 + USB_CDC_DATA_EP_STEP)
  #define USB_CDC_INTERRUPT_REQUEST_EP_3 (USB_CDC_INTERRUPT_REQUEST_EP_2 + USB_CDC_DATA_INT_STEP)
  #if (USB_CDC_DATA_REQUEST_EP_3 >= STM32_OTG_ENDPOINTS) || (USB_CDC_INTERRUPT_REQUEST_EP_3 >= STM32_OTG_ENDPOINTS)
    #error "No enough endpoints"
  #endif
#endif

#if HAL_USE_USB_CDC_4
  #define CDC_INT_IF_4                   (CDC_INT_IF_3 + CDC_INTEFACES_PER_INSTANCE)
  #define CDC_DATA_IF_4                  (CDC_DATA_IF_3 + CDC_INTEFACES_PER_INSTANCE)
  #define USB_CDC_DATA_REQUEST_EP_4      (USB_CDC_DATA_REQUEST_EP_3 + USB_CDC_DATA_EP_STEP)
  #define USB_CDC_DATA_AVAILABLE_EP_4    (USB_CDC_DATA_AVAILABLE_EP_3 + USB_CDC_DATA_EP_STEP)
  #define USB_CDC_INTERRUPT_REQUEST_EP_4 (USB_CDC_INTERRUPT_REQUEST_EP_3 + USB_CDC_DATA_INT_STEP)
  #if (USB_CDC_DATA_REQUEST_EP_4 >= STM32_OTG_ENDPOINTS) || (USB_CDC_INTERRUPT_REQUEST_EP_4 >= STM32_OTG_ENDPOINTS)
    #error "No enough endpoints"
  #endif
#endif

/* --- Individual Descriptor Sizes in Bytes --- */
#define USB_CONFIG_DESC_SIZE        9
#define USB_INTERFACE_DESC_SIZE     9
#define USB_ENDPOINT_DESC_SIZE      7
#define USB_IAD_DESC_SIZE           8

/* CDC Functional Descriptors sizes */
#define CDC_HEADER_FUNC_SIZE        5
#define CDC_CALL_MGMT_FUNC_SIZE     5
#define CDC_ACM_FUNC_SIZE           4
#define CDC_UNION_FUNC_SIZE         5

/* Total size of one complete CDC ACM instance:
   1 IAD (8) + 1 INT Interface (9) + 4 Functional Descriptors (5+5+4+5)
   + 1 INT Endpoint (7) + 1 DATA Interface (9) + 2 BULK Endpoints (7+7) = 58 bytes */
#define CDC_ACM_INSTANCE_SIZE ( \
    USB_IAD_DESC_SIZE + \
    USB_INTERFACE_DESC_SIZE + \
    CDC_HEADER_FUNC_SIZE + \
    CDC_CALL_MGMT_FUNC_SIZE + \
    CDC_ACM_FUNC_SIZE + \
    CDC_UNION_FUNC_SIZE + \
    USB_ENDPOINT_DESC_SIZE + \
    USB_INTERFACE_DESC_SIZE + \
    (USB_ENDPOINT_DESC_SIZE * 2) \
)

/* Total size of one complete MSD instance:
   1 Interface (9) + 2 BULK Endpoints (7+7) = 23 bytes */
#define USB_MSD_INSTANCE_SIZE ( \
    USB_INTERFACE_DESC_SIZE + \
    (USB_ENDPOINT_DESC_SIZE * 2) \
)

/* --- Master Conditional Descriptor Size Calculation --- */
#define DESCRIPTOR_SIZE ( \
    USB_CONFIG_DESC_SIZE + \
    (HAL_USE_USB_MSD ? USB_MSD_INSTANCE_SIZE : 0) + \
    CDC_ACM_INSTANCE_SIZE + \
    (HAL_USE_USB_CDC_2 ? CDC_ACM_INSTANCE_SIZE : 0) + \
    (HAL_USE_USB_CDC_3 ? CDC_ACM_INSTANCE_SIZE : 0) + \
    (HAL_USE_USB_CDC_4 ? CDC_ACM_INSTANCE_SIZE : 0) \
)

#define NUM_INTERFACES ( (HAL_USE_USB_MSD ? 1 : 0) + \
                         (CDC_INTEFACES_PER_INSTANCE) + \
                         (HAL_USE_USB_CDC_2 ? CDC_INTEFACES_PER_INSTANCE : 0) + \
                         (HAL_USE_USB_CDC_3 ? CDC_INTEFACES_PER_INSTANCE : 0) + \
                         (HAL_USE_USB_CDC_4 ? CDC_INTEFACES_PER_INSTANCE : 0) )

/*
 * USB Device Descriptor.
 */
static const uint8_t vcom_device_descriptor_data[18] = {
  USB_DESC_DEVICE       (0x0110,        /* bcdUSB (1.1).                    */
                         0xEF,          /* bDeviceClass ( misc ).           */
                         0x02,          /* bDeviceSubClass.                 */
                         0x01,          /* bDeviceProtocol.                 */
                         0x40,          /* bMaxPacketSize.                  */
                         0x0483,        /* idVendor (ST).                   */
                         0x5740,        /* idProduct.                       */
                         0x0200,        /* bcdDevice.                       */
                         1,             /* iManufacturer.                   */
                         2,             /* iProduct.                        */
                         3,             /* iSerialNumber.                   */
                         1)             /* bNumConfigurations.              */
};

/*
 * Device Descriptor wrapper.
 */
static const USBDescriptor vcom_device_descriptor = {
  sizeof vcom_device_descriptor_data,
  vcom_device_descriptor_data
};

/* Configuration Descriptor tree for a CDC.*/
static const uint8_t vcom_configuration_descriptor_data[DESCRIPTOR_SIZE] = {
  /* Configuration Descriptor.*/
  USB_DESC_CONFIGURATION(DESCRIPTOR_SIZE,/* wTotalLength.                   */
                         NUM_INTERFACES,/* bNumInterfaces.                  */
                         0x01,          /* bConfigurationValue.             */
                         0,             /* iConfiguration.                  */
                         0x80,          /* bmAttributes (bus powered).      */
                         200),          /* bMaxPower (400mA).               */
#if HAL_USE_USB_MSD
  // MSD
  USB_DESC_INTERFACE    (MSD_IF,        /* bInterfaceNumber.                */
                         0x00,          /* bAlternateSetting.               */
                         0x02,          /* bNumEndpoints.                   */
                         0x08,          /* bInterfaceClass (mass storage)   */
                         0x06,          /* bInterfaceSubClass (SCSI
                                              transparent storage class).   */
                         0x50,          /* bInterfaceProtocol (Bulk Only).  */
                         4),            /* iInterface.                      */
  /* Mass Storage Data In Endpoint Descriptor.*/
  USB_DESC_ENDPOINT     (USB_MSD_DATA_EP | 0x80,
                         0x02,          /* bmAttributes (Bulk).             */
                         64,            /* wMaxPacketSize.                  */
                         0x00),         /* bInterval. 1ms                   */
  /* Mass Storage Data Out Endpoint Descriptor.*/
  USB_DESC_ENDPOINT     (USB_MSD_DATA_EP,
                         0x02,          /* bmAttributes (Bulk).             */
                         64,            /* wMaxPacketSize.                  */
                         0x00),         /* bInterval. 1ms                   */
#endif // HAL_USE_USB_MSD
  // CDC
  /* IAD Descriptor - describes that EP2+3 belong to CDC */
  USB_DESC_INTERFACE_ASSOCIATION(CDC_INT_IF, /* bFirstInterface.            */
                        CDC_INTEFACES_PER_INSTANCE, /* bInterfaceCount.     */
                        0x02,           /* bFunctionClass (CDC).            */
                        0x02,           /* bFunctionSubClass.  (2)          */
                        0x01,           /* bFunctionProtocol (1)            */
                        5),             /* iInterface.                      */
  /* Interface Descriptor.*/
  USB_DESC_INTERFACE    (CDC_INT_IF,    /* bInterfaceNumber.                */
                         0x00,          /* bAlternateSetting.               */
                         0x01,          /* bNumEndpoints.                   */
                         0x02,          /* bInterfaceClass (Communications
                                           Interface Class, CDC section
                                           4.2).                            */
                         0x02,          /* bInterfaceSubClass (Abstract
                                         Control Model, CDC section 4.3).   */
                         0x01,          /* bInterfaceProtocol (AT commands,
                                           CDC section 4.4).                */
                         0),            /* iInterface.                      */
  /* Header Functional Descriptor (CDC section 5.2.3).*/
  USB_DESC_BYTE         (5),            /* bLength.                         */
  USB_DESC_BYTE         (0x24),         /* bDescriptorType (CS_INTERFACE).  */
  USB_DESC_BYTE         (0x00),         /* bDescriptorSubtype (Header
                                           Functional Descriptor.           */
  USB_DESC_BCD          (0x0110),       /* bcdCDC.                          */
  /* Call Management Functional Descriptor. */
  USB_DESC_BYTE         (5),            /* bFunctionLength.                 */
  USB_DESC_BYTE         (0x24),         /* bDescriptorType (CS_INTERFACE).  */
  USB_DESC_BYTE         (0x01),         /* bDescriptorSubtype (Call Management
                                           Functional Descriptor).          */
  USB_DESC_BYTE         (0x00),         /* bmCapabilities (D0+D1).          */
  USB_DESC_BYTE         (CDC_DATA_IF),  /* bDataInterface.                  */
  /* ACM Functional Descriptor.*/
  USB_DESC_BYTE         (4),            /* bFunctionLength.                 */
  USB_DESC_BYTE         (0x24),         /* bDescriptorType (CS_INTERFACE).  */
  USB_DESC_BYTE         (0x02),         /* bDescriptorSubtype (Abstract
                                           Control Management Descriptor).  */
  USB_DESC_BYTE         (0x02),         /* bmCapabilities.                  */
  /* Union Functional Descriptor.*/
  USB_DESC_BYTE         (5),            /* bFunctionLength.                 */
  USB_DESC_BYTE         (0x24),         /* bDescriptorType (CS_INTERFACE).  */
  USB_DESC_BYTE         (0x06),         /* bDescriptorSubtype (Union
                                           Functional Descriptor).          */
  USB_DESC_BYTE         (CDC_INT_IF),   /* bMasterInterface (Communication
                                           Class Interface).                */
  USB_DESC_BYTE         (CDC_DATA_IF),  /* bSlaveInterface0 (Data Class
                                           Interface).                      */
  /* Endpoint 3 Descriptor.*/
  USB_DESC_ENDPOINT     (USB_CDC_INTERRUPT_REQUEST_EP|0x80,
                         0x03,          /* bmAttributes (Interrupt).        */
                         0x0008,        /* wMaxPacketSize.                  */
                         0xFF),         /* bInterval.                       */
  /* Interface Descriptor.*/
  USB_DESC_INTERFACE    (CDC_DATA_IF,   /* bInterfaceNumber.                */
                         0x00,          /* bAlternateSetting.               */
                         0x02,          /* bNumEndpoints.                   */
                         0x0A,          /* bInterfaceClass (Data Class
                                           Interface, CDC section 4.5).     */
                         0x00,          /* bInterfaceSubClass (CDC section
                                           4.6).                            */
                         0x00,          /* bInterfaceProtocol (CDC section
                                           4.7).                            */
                         0x00),         /* iInterface.                      */
  /* Endpoint 2 Descriptor.*/
  USB_DESC_ENDPOINT     (USB_CDC_DATA_AVAILABLE_EP,     /* bEndpointAddress.*/
                         0x02,          /* bmAttributes (Bulk).             */
                         0x0040,        /* wMaxPacketSize.                  */
                         0x00),         /* bInterval.                       */
  /* Endpoint 2 Descriptor.*/
  USB_DESC_ENDPOINT     (USB_CDC_DATA_REQUEST_EP|0x80,  /* bEndpointAddress.*/
                         0x02,          /* bmAttributes (Bulk).             */
                         0x0040,        /* wMaxPacketSize.                  */
                         0x00)          /* bInterval.                       */
#if HAL_USE_USB_CDC_2
  ,
  // === CDC ACM INSTANCE 2 ===
  USB_DESC_INTERFACE_ASSOCIATION(CDC_INT_IF_2, CDC_INTEFACES_PER_INSTANCE, 0x02, 0x02, 0x01, 6),
  USB_DESC_INTERFACE    (CDC_INT_IF_2, 0x00, 0x01, 0x02, 0x02, 0x01, 0),
  USB_DESC_BYTE         (5), 0x24, 0x00, USB_DESC_BCD(0x0110),
  USB_DESC_BYTE         (5), 0x24, 0x01, 0x00, CDC_DATA_IF_2,
  USB_DESC_BYTE         (4), 0x24, 0x02, 0x02,
  USB_DESC_BYTE         (5), 0x24, 0x06, CDC_INT_IF_2, CDC_DATA_IF_2,
  USB_DESC_ENDPOINT     (USB_CDC_INTERRUPT_REQUEST_EP_2|0x80, 0x03, 0x0008, 0xFF),
  USB_DESC_INTERFACE    (CDC_DATA_IF_2, 0x00, 0x02, 0x0A, 0x00, 0x00, 0x00),
  USB_DESC_ENDPOINT     (USB_CDC_DATA_AVAILABLE_EP_2, 0x02, 0x0040, 0x00),
  USB_DESC_ENDPOINT     (USB_CDC_DATA_REQUEST_EP_2|0x80, 0x02, 0x0040, 0x00),
#endif // HAL_USE_USB_CDC_2

#if HAL_USE_USB_CDC_3
  ,
  // === CDC ACM INSTANCE 3 ===
  USB_DESC_INTERFACE_ASSOCIATION(CDC_INT_IF_3, CDC_INTEFACES_PER_INSTANCE, 0x02, 0x02, 0x01, 7),
  USB_DESC_INTERFACE    (CDC_INT_IF_3, 0x00, 0x01, 0x02, 0x02, 0x01, 0),
  USB_DESC_BYTE         (5), 0x24, 0x00, USB_DESC_BCD(0x0110),
  USB_DESC_BYTE         (5), 0x24, 0x01, 0x00, CDC_DATA_IF_3,
  USB_DESC_BYTE         (4), 0x24, 0x02, 0x02,
  USB_DESC_BYTE         (5), 0x24, 0x06, CDC_INT_IF_3, CDC_DATA_IF_3,
  USB_DESC_ENDPOINT     (USB_CDC_INTERRUPT_REQUEST_EP_3|0x80, 0x03, 0x0008, 0xFF),
  USB_DESC_INTERFACE    (CDC_DATA_IF_3, 0x00, 0x02, 0x0A, 0x00, 0x00, 0x00),
  USB_DESC_ENDPOINT     (USB_CDC_DATA_AVAILABLE_EP_3, 0x02, 0x0040, 0x00),
  USB_DESC_ENDPOINT     (USB_CDC_DATA_REQUEST_EP_3|0x80, 0x02, 0x0040, 0x00),
#endif // HAL_USE_USB_CDC_3

#if HAL_USE_USB_CDC_4
  ,
  // === CDC ACM INSTANCE 4 ===
  USB_DESC_INTERFACE_ASSOCIATION(CDC_INT_IF_4, CDC_INTEFACES_PER_INSTANCE, 0x02, 0x02, 0x01, 8),
  USB_DESC_INTERFACE    (CDC_INT_IF_4, 0x00, 0x01, 0x02, 0x02, 0x01, 0),
  USB_DESC_BYTE         (5), 0x24, 0x00, USB_DESC_BCD(0x0110),
  USB_DESC_BYTE         (5), 0x24, 0x01, 0x00, CDC_DATA_IF_4,
  USB_DESC_BYTE         (4), 0x24, 0x02, 0x02,
  USB_DESC_BYTE         (5), 0x24, 0x06, CDC_INT_IF_4, CDC_DATA_IF_4,
  USB_DESC_ENDPOINT     (USB_CDC_INTERRUPT_REQUEST_EP_4|0x80, 0x03, 0x0008, 0xFF),
  USB_DESC_INTERFACE    (CDC_DATA_IF_4, 0x00, 0x02, 0x0A, 0x00, 0x00, 0x00),
  USB_DESC_ENDPOINT     (USB_CDC_DATA_AVAILABLE_EP_4, 0x02, 0x0040, 0x00),
  USB_DESC_ENDPOINT     (USB_CDC_DATA_REQUEST_EP_4|0x80, 0x02, 0x0040, 0x00),
#endif // HAL_USE_USB_CDC_4
};

/*
 * Configuration Descriptor wrapper.
 */
static const USBDescriptor vcom_configuration_descriptor = {
  sizeof vcom_configuration_descriptor_data,
  vcom_configuration_descriptor_data
};

/*
 * U.S. English language identifier.
 */
static const uint8_t vcom_string0[] = {
  USB_DESC_BYTE(4),                     /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  USB_DESC_WORD(0x0409)                 /* wLANGID (U.S. English).          */
};

/*
 * Vendor string.
 */
static const uint8_t vcom_string1[] = {
  USB_DESC_BYTE(22),                    /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  'r', 0, 'u', 0, 's', 0, 'E', 0, 'F', 0, 'I', 0, ' ', 0, 'L', 0,
  'L', 0, 'C', 0
};

#ifndef USB_DESCRIPTOR_STRING_CONTENT
/* "rusEFI Engine Management ECU" */
#define USB_DESCRIPTOR_STRING_CONTENT 'r', 0, 'u', 0, 's', 0, 'E', 0, 'F', 0, 'I', 0, ' ', 0, 'E', 0, \
  'n', 0, 'g', 0, 'i', 0, 'n', 0, 'e', 0, ' ', 0, 'M', 0, 'a', 0, \
  'n', 0, 'a', 0, 'g', 0, 'e', 0, 'm', 0, 'e', 0, 'n', 0, 't', 0, \
  ' ', 0, 'E', 0, 'C', 0, 'U', 0
#endif

#ifndef USB_DESCRIPTOR_B_LENGTH
#define USB_DESCRIPTOR_B_LENGTH (sizeof((uint8_t[]){ USB_DESCRIPTOR_STRING_CONTENT }) + 2)
#endif

/*
 * Device Description string.
 */
static const uint8_t vcom_string2[] = {
  USB_DESC_BYTE(USB_DESCRIPTOR_B_LENGTH),                    /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  USB_DESCRIPTOR_STRING_CONTENT
};

/*
 * Serial Number string.
 */

#ifdef BOARD_SERIAL
static_assert(strlen(BOARD_SERIAL) == 24, "BOARD_SERIAL incorrect length, should be 24 chars");
static const uint8_t vcom_string3[] = {
  USB_DESC_BYTE(50),                     /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  BOARD_SERIAL[ 0], 0, BOARD_SERIAL[ 1], 0, BOARD_SERIAL[ 2], 0, BOARD_SERIAL[ 3], 0,
  BOARD_SERIAL[ 4], 0, BOARD_SERIAL[ 5], 0, BOARD_SERIAL[ 6], 0, BOARD_SERIAL[ 7], 0,
  BOARD_SERIAL[ 8], 0, BOARD_SERIAL[ 9], 0, BOARD_SERIAL[10], 0, BOARD_SERIAL[11], 0,
  BOARD_SERIAL[12], 0, BOARD_SERIAL[13], 0, BOARD_SERIAL[14], 0, BOARD_SERIAL[15], 0,
  BOARD_SERIAL[16], 0, BOARD_SERIAL[17], 0, BOARD_SERIAL[18], 0, BOARD_SERIAL[19], 0,
  BOARD_SERIAL[20], 0, BOARD_SERIAL[21], 0, BOARD_SERIAL[22], 0, BOARD_SERIAL[23], 0
};
#else
static uint8_t vcom_string3[] = {
  USB_DESC_BYTE(50),                     /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  '0', 0, '1', 0, '2', 0, '3', 0, '4', 0, '5', 0, '6', 0, '7', 0,
  '8', 0, '9', 0, 'A', 0, 'B', 0, 'C', 0, 'D', 0, 'E', 0, 'F', 0,
  '0', 0, '1', 0, '2', 0, '3', 0, '4', 0, '5', 0, '6', 0, '7', 0
};
#endif

/* "RusEFI MSD" */
static const uint8_t msd_string4[] = {
  USB_DESC_BYTE(10 * 2 + 2),            /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  'r', 0, 'u', 0, 's', 0, 'E', 0, 'F', 0, 'I', 0, ' ', 0, 'M', 0,
  'S', 0, 'D', 0
};

/* "RusEFI Virtual COM Port" */
static const uint8_t vcom_string5[] = {
  USB_DESC_BYTE(23 * 2 + 2),            /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  'r', 0, 'u', 0, 's', 0, 'E', 0, 'F', 0, 'I', 0, ' ', 0, 'V', 0,
  'i', 0, 'r', 0, 't', 0, 'u', 0, 'a', 0, 'l', 0, ' ', 0, 'C', 0,
  'O', 0, 'M', 0, ' ', 0, 'P', 0, 'o', 0, 'r', 0, 't', 0
};

/* "RusEFI CAN Sniffer" */
static const uint8_t vcom_aux_name[] = {
  USB_DESC_BYTE(18 * 2 + 2),            /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  'r', 0, 'u', 0, 's', 0, 'E', 0, 'F', 0, 'I', 0, ' ', 0, 'C', 0,
  'A', 0, 'N', 0, ' ', 0, 'S', 0, 'n', 0, 'i', 0, 'f', 0, 'f', 0,
  'e', 0, 'r', 0
};

/*
 * Strings wrappers array.
 */
static const USBDescriptor vcom_strings[] = {
  { sizeof(vcom_string0), vcom_string0 },
  { sizeof(vcom_string1), vcom_string1 },
  { sizeof(vcom_string2), vcom_string2 },
  { sizeof(vcom_string3), vcom_string3 },
  { sizeof(msd_string4), msd_string4 },
  { sizeof(vcom_string5), vcom_string5 }
#if HAL_USE_USB_CDC_2
  ,
  { sizeof(vcom_aux_name), vcom_aux_name }
#endif
#if HAL_USE_USB_CDC_3
  ,
  { sizeof(vcom_aux_name), vcom_aux_name }
#endif
#if HAL_USE_USB_CDC_4
  ,
  { sizeof(vcom_aux_name), vcom_aux_name }
#endif
};

#ifndef BOARD_SERIAL
static char nib2char(uint8_t nibble) {
	if (nibble > 0x9) {
		return nibble - 0xA + 'A';
	} else {
		return nibble + '0';
	}
}

void usbPopulateSerialNumber(const uint8_t* serialNumber, size_t bytes) {
	if (bytes > 12) {
		bytes = 12;
	}

	// Skip the first two bytes (metadata)
	uint8_t* dst = &vcom_string3[2];

	for (size_t i = 0; i < bytes; i++) {
		uint8_t byte = serialNumber[i];

		uint8_t lowNibble = byte & 0xF;
		uint8_t highNibble = byte >> 4;

		// Descriptor strings are UCS16, so write every other byte
		dst[4 * i] = nib2char(highNibble);
		dst[4 * i + 2] = nib2char(lowNibble);
	}
}
#endif // BOARD_SERIAL

/*
 * Handles the GET_DESCRIPTOR callback. All required descriptors must be
 * handled here.
 */
static const USBDescriptor *get_descriptor(USBDriver *usbp,
                                           uint8_t dtype,
                                           uint8_t dindex,
                                           uint16_t lang) {

  (void)usbp;
  (void)lang;
  switch (dtype) {
  case USB_DESCRIPTOR_DEVICE:
    return &vcom_device_descriptor;
  case USB_DESCRIPTOR_CONFIGURATION:
    return &vcom_configuration_descriptor;
  case USB_DESCRIPTOR_STRING:
    if (dindex < efi::size(vcom_strings))
      return &vcom_strings[dindex];
  }
  return NULL;
}

#if HAL_USE_USB_MSD
/**
 * @brief   IN MSD state
 */
static USBInEndpointState msdInstate;

/**
 * @brief   OUT MSD state
 */
static USBOutEndpointState msdOutstate;

/**
 * @brief   MSD initialization structure (both IN and OUT).
 */
static const USBEndpointConfig msdEpConfig = {
  .ep_mode        = USB_EP_MODE_TYPE_BULK,
  .setup_cb       = NULL,
  .in_cb          = NULL,
  .out_cb         = NULL,
  .in_maxsize     = USB_MSD_EP_SIZE,
  .out_maxsize    = USB_MSD_EP_SIZE,
  .in_state       = &msdInstate,
  .out_state      = &msdOutstate,
  .in_multiplier  = 4,
  .setup_buf      = NULL
};
#endif //HAL_USE_MSD

/**
 * @brief   IN CDC state
 */
static USBInEndpointState cdcDataInstate[NUM_CDC_INSTANCES];
/**
 * @brief   OUT CDC state
 */
static USBOutEndpointState cdcDataOutstate[NUM_CDC_INSTANCES];
/**
 * @brief   IN CDC interrupt state
 */
static USBInEndpointState cdcInterruptInstate[NUM_CDC_INSTANCES];

/**
 * Determines the space allocated for the TXFIFO as multiples of
 *          the packet size (@p in_maxsize).
 */
#define DEFAULT_CDC_IN_MULTIPLIER   2

/**
 * @brief   CDC initialization structure (both IN and OUT)
 */
static const USBEndpointConfig cdcDataEpConfig[NUM_CDC_INSTANCES] = {
{
  .ep_mode        = USB_EP_MODE_TYPE_BULK,
  .setup_cb       = NULL,
  .in_cb          = sduDataTransmitted,
  .out_cb         = sduDataReceived,
  .in_maxsize     = 0x0040,
  .out_maxsize    = 0x0040,
  .in_state       = &cdcDataInstate[0],
  .out_state      = &cdcDataOutstate[0],
  .in_multiplier  = DEFAULT_CDC_IN_MULTIPLIER,
  .setup_buf      = NULL
}
#if HAL_USE_USB_CDC_2
,{
  .ep_mode        = USB_EP_MODE_TYPE_BULK,
  .setup_cb       = NULL,
  .in_cb          = sduDataTransmitted,
  .out_cb         = sduDataReceived,
  .in_maxsize     = 0x0040,
  .out_maxsize    = 0x0040,
  .in_state       = &cdcDataInstate[1],
  .out_state      = &cdcDataOutstate[1],
  .in_multiplier  = DEFAULT_CDC_IN_MULTIPLIER,
  .setup_buf      = NULL
}
#endif
#if HAL_USE_USB_CDC_3
,{
  .ep_mode        = USB_EP_MODE_TYPE_BULK,
  .setup_cb       = NULL,
  .in_cb          = sduDataTransmitted,
  .out_cb         = sduDataReceived,
  .in_maxsize     = 0x0040,
  .out_maxsize    = 0x0040,
  .in_state       = &cdcDataInstate[2],
  .out_state      = &cdcDataOutstate[2],
  .in_multiplier  = DEFAULT_CDC_IN_MULTIPLIER,
  .setup_buf      = NULL
}
#endif
#if HAL_USE_USB_CDC_4
,{
  .ep_mode        = USB_EP_MODE_TYPE_BULK,
  .setup_cb       = NULL,
  .in_cb          = sduDataTransmitted,
  .out_cb         = sduDataReceived,
  .in_maxsize     = 0x0040,
  .out_maxsize    = 0x0040,
  .in_state       = &cdcDataInstate[3],
  .out_state      = &cdcDataOutstate[3],
  .in_multiplier  = DEFAULT_CDC_IN_MULTIPLIER,
  .setup_buf      = NULL
}
#endif
};

/**
 * @brief   CDC interrupt initialization structure (IN only)
 */
static const USBEndpointConfig cdcInterruptEpConfig[NUM_CDC_INSTANCES] = {
{
  .ep_mode        = USB_EP_MODE_TYPE_INTR,
  .setup_cb       = NULL,
  .in_cb          = sduInterruptTransmitted,
  .out_cb         = NULL,
  .in_maxsize     = 0x0010,
  .out_maxsize    = 0x0000,
  .in_state       = &cdcInterruptInstate[0],
  .out_state      = NULL,
  .in_multiplier  = 1,
  .setup_buf      = NULL
}
#if HAL_USE_USB_CDC_2
,{
  .ep_mode        = USB_EP_MODE_TYPE_INTR,
  .setup_cb       = NULL,
  .in_cb          = sduInterruptTransmitted,
  .out_cb         = NULL,
  .in_maxsize     = 0x0010,
  .out_maxsize    = 0x0000,
  .in_state       = &cdcInterruptInstate[1],
  .out_state      = NULL,
  .in_multiplier  = 1,
  .setup_buf      = NULL
}
#endif
#if HAL_USE_USB_CDC_3
,{
  .ep_mode        = USB_EP_MODE_TYPE_INTR,
  .setup_cb       = NULL,
  .in_cb          = sduInterruptTransmitted,
  .out_cb         = NULL,
  .in_maxsize     = 0x0010,
  .out_maxsize    = 0x0000,
  .in_state       = &cdcInterruptInstate[2],
  .out_state      = NULL,
  .in_multiplier  = 1,
  .setup_buf      = NULL
}
#endif
#if HAL_USE_USB_CDC_4
,{
  .ep_mode        = USB_EP_MODE_TYPE_INTR,
  .setup_cb       = NULL,
  .in_cb          = sduInterruptTransmitted,
  .out_cb         = NULL,
  .in_maxsize     = 0x0010,
  .out_maxsize    = 0x0000,
  .in_state       = &cdcInterruptInstate[3],
  .out_state      = NULL,
  .in_multiplier  = 1,
  .setup_buf      = NULL
}
#endif
};

/*
 * Handles the USB driver global events.
 */
static void usb_event(USBDriver *usbp, usbevent_t event) {
  switch (event) {
  case USB_EVENT_ADDRESS:
    return;
  case USB_EVENT_CONFIGURED:
    chSysLockFromISR();

    /* Enables the endpoints specified into the configuration.
       Note, this callback is invoked from an ISR so I-Class functions
       must be used.*/
#if HAL_USE_USB_MSD
    usbInitEndpointI(usbp, USB_MSD_DATA_EP, &msdEpConfig);
#endif

    for (size_t i = 0; i < NUM_CDC_INSTANCES; i++) {
      usbInitEndpointI(usbp, serusbcfg[i].bulk_in, &cdcDataEpConfig[i]);
      if (serusbcfg[i].int_in != serusbcfg[0].int_in) {
        // See USB_CDC_INT_EP_HACK
        usbInitEndpointI(usbp, serusbcfg[i].int_in, &cdcInterruptEpConfig[i]);
      }
    }

    /* Resetting the state of the CDC subsystem.*/
    for (size_t i = 0; i < NUM_CDC_INSTANCES; i++) {
      sduConfigureHookI(&SDU[i]);
    }

    #if HAL_USE_USB_MSD
        // Tell the MMC thread to wake up and mount the card as a USB device
        onUsbConnectedNotifyMmcI(true);
    #endif

    chSysUnlockFromISR();
    return;
  case USB_EVENT_RESET:
    /* Falls into.*/
  case USB_EVENT_UNCONFIGURED:
    /* Falls into.*/
  case USB_EVENT_SUSPEND:
    chSysLockFromISR();

    #if HAL_USE_USB_MSD
      onUsbConnectedNotifyMmcI(false);
    #endif

    /* Disconnection event on suspend.*/
    for (size_t i = 0; i < NUM_CDC_INSTANCES; i++) {
      sduSuspendHookI(&SDU[i]);
    }

    chSysUnlockFromISR();
    return;
  case USB_EVENT_WAKEUP:
    chSysLockFromISR();

    /* Disconnection event on suspend.*/
    for (size_t i = 0; i < NUM_CDC_INSTANCES; i++) {
      sduWakeupHookI(&SDU[i]);
    }

    chSysUnlockFromISR();
    return;
  case USB_EVENT_STALLED:
    return;
  }
  return;
}

/*
 * Handles the USB driver global events.
 */
static void sof_handler(USBDriver *usbp) {

  (void)usbp;

  osalSysLockFromISR();
  for (size_t i = 0; i < NUM_CDC_INSTANCES; i++) {
    sduSOFHookI(&SDU[i]);
  }
  osalSysUnlockFromISR();
}

// We need a custom hook to handle both MSD and CDC at the same time
static bool hybridRequestHook(USBDriver *usbp) {
#if HAL_USE_USB_MSD
	// Try the MSD driver first
	if (msd_request_hook_new(usbp)) {
		return true;
	}
#endif // HAL_USE_USB_MSD

	// if not MSD, it must be serial
	return sduRequestsHook(usbp);
}

/*
 * USB driver configuration.
 */
const USBConfig usbcfg = {
  .event_cb = usb_event,
  .get_descriptor_cb = get_descriptor,
  .requests_hook_cb = hybridRequestHook,
  .sof_cb = sof_handler
};

/*
 * Serial over USB driver configuration.
 */
const SerialUSBConfig serusbcfg[NUM_CDC_INSTANCES] = {
{
  .usbp = &USBD,
  .bulk_in = USB_CDC_DATA_REQUEST_EP,
  .bulk_out = USB_CDC_DATA_AVAILABLE_EP,
  .int_in = USB_CDC_INTERRUPT_REQUEST_EP
}
#if HAL_USE_USB_CDC_2
,{
  .usbp = &USBD,
  .bulk_in = USB_CDC_DATA_REQUEST_EP_2,
  .bulk_out = USB_CDC_DATA_AVAILABLE_EP_2,
  .int_in = USB_CDC_INTERRUPT_REQUEST_EP_2
}
#endif
#if HAL_USE_USB_CDC_3
,{
  .usbp = &USBD,
  .bulk_in = USB_CDC_DATA_REQUEST_EP_3,
  .bulk_out = USB_CDC_DATA_AVAILABLE_EP_3,
  .int_in = USB_CDC_INTERRUPT_REQUEST_EP_3
}
#endif
#if HAL_USE_USB_CDC_4
,{
  .usbp = &USBD,
  .bulk_in = USB_CDC_DATA_REQUEST_EP_4,
  .bulk_out = USB_CDC_DATA_AVAILABLE_EP_4,
  .int_in = USB_CDC_INTERRUPT_REQUEST_EP_4
}
#endif
};

#endif /* EFI_USB_SERIAL */
