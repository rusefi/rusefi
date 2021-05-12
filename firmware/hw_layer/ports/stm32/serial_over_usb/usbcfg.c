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

#include "global.h"
#include "os_access.h"
#include "mmc_card.h"

#if HAL_USE_SERIAL_USB

/* Virtual serial port over USB.*/
SerialUSBDriver SDU1;

/*
 * Endpoints to be used for USBD1.
 */
#define USBD1_DATA_REQUEST_EP           2
#define USBD1_DATA_AVAILABLE_EP         2
#define USBD1_INTERRUPT_REQUEST_EP      3

#if HAL_USE_USB_MSD
	// Descriptor that includes MSD is larger and has an extra interface
	#define DESCRIPTOR_SIZE 98
	#define NUM_INTERFACES 3
	#define USB_MSD_EP_SIZE 64

	#define MSD_IF 0
	#define CDC_INT_IF 1
	#define CDC_DATA_IF 2
#else
	// Alternate descriptor is smaller, no MSD
	#define DESCRIPTOR_SIZE 75
	#define NUM_INTERFACES 2

	// only two interfaces
	#define CDC_INT_IF 0
	#define CDC_DATA_IF 1
#endif

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
  USB_DESC_CONFIGURATION(DESCRIPTOR_SIZE,/* wTotalLength.                    */
                         NUM_INTERFACES,/* bNumInterfaces.                  */
                         0x01,          /* bConfigurationValue.             */
                         0,             /* iConfiguration.                  */
                         0xC0,          /* bmAttributes (self powered).     */
                         100),           /* bMaxPower (200mA).              */
#if HAL_USE_USB_MSD
  USB_DESC_INTERFACE    (MSD_IF,        /* bInterfaceNumber.                */
                         0x00,          /* bAlternateSetting.               */
                         0x02,          /* bNumEndpoints.                   */
                         0x08,          /* bInterfaceClass (mass storage)   */
                         0x06,          /* bInterfaceSubClass (SCSI
                                              transparent storage class).   */
                         0x50,          /* bInterfaceProtocol (Bulk Only).  */
                         0),            /* iInterface.                      */
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
  USB_DESC_INTERFACE_ASSOCIATION(CDC_INT_IF, /* bFirstInterface.                  */
                                 0x02, /* bInterfaceCount.                  */
                                 0x02, /* bFunctionClass (CDC).             */
                                 0x02, /* bFunctionSubClass.  (2)           */
                                 0x01, /* bFunctionProtocol (1)             */
                                 2),   /* iInterface.                       */
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
  USB_DESC_BYTE         (CDC_DATA_IF),  /* bDataInterface.                */
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
  USB_DESC_ENDPOINT     (USBD1_INTERRUPT_REQUEST_EP|0x80,
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
  USB_DESC_ENDPOINT     (USBD1_DATA_AVAILABLE_EP,       /* bEndpointAddress.*/
                         0x02,          /* bmAttributes (Bulk).             */
                         0x0040,        /* wMaxPacketSize.                  */
                         0x00),         /* bInterval.                       */
  /* Endpoint 2 Descriptor.*/
  USB_DESC_ENDPOINT     (USBD1_DATA_REQUEST_EP|0x80,    /* bEndpointAddress.*/
                         0x02,          /* bmAttributes (Bulk).             */
                         0x0040,        /* wMaxPacketSize.                  */
                         0x00)          /* bInterval.                       */
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

/*
 * Device Description string.
 */
static const uint8_t vcom_string2[] = {
  USB_DESC_BYTE(58),                    /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  'r', 0, 'u', 0, 's', 0, 'E', 0, 'F', 0, 'I', 0, ' ', 0, 'E', 0,
  'n', 0, 'g', 0, 'i', 0, 'n', 0, 'e', 0, ' ', 0, 'M', 0, 'a', 0,
  'n', 0, 'a', 0, 'g', 0, 'e', 0, 'm', 0, 'e', 0, 'n', 0, 't', 0,
  ' ', 0, 'E', 0, 'C', 0, 'U', 0
};

/*
 * Serial Number string.
 */
static uint8_t vcom_string3[] = {
  USB_DESC_BYTE(50),                     /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  '0', 0, '1', 0, '2', 0, '3', 0, '4', 0, '5', 0, '6', 0, '7', 0,
  '8', 0, '9', 0, 'A', 0, 'B', 0, 'C', 0, 'D', 0, 'E', 0, 'F', 0,
  '0', 0, '1', 0, '2', 0, '3', 0, '4', 0, '5', 0, '6', 0, '7', 0,
};

/*
 * Strings wrappers array.
 */
static const USBDescriptor vcom_strings[] = {
  {sizeof vcom_string0, vcom_string0},
  {sizeof vcom_string1, vcom_string1},
  {sizeof vcom_string2, vcom_string2},
  {sizeof vcom_string3, vcom_string3}
};

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
    if (dindex < 4)
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
	USB_EP_MODE_TYPE_BULK,
	NULL,
	NULL,
	NULL,
	USB_MSD_EP_SIZE,
	USB_MSD_EP_SIZE,
	&msdInstate,
	&msdOutstate,
	4,
	NULL
	};
#endif //HAL_USE_MSD

// IN CDC data state.
static USBInEndpointState cdcDataInstate;
// OUT CDC data state.
static USBOutEndpointState cdcDataOutstate;
// CDC data initialization structure (both IN and OUT).
static const USBEndpointConfig cdcDataEpConfig = {
  USB_EP_MODE_TYPE_BULK,
  NULL,
  sduDataTransmitted,
  sduDataReceived,
  0x0040,
  0x0040,
  &cdcDataInstate,
  &cdcDataOutstate,
  4,
  NULL
};

// IN CDC interrupt state.
static USBInEndpointState cdcInterruptInstate;
// CDC interrupt initialization structure (IN only).
static const USBEndpointConfig cdcInterruptEpConfig = {
  USB_EP_MODE_TYPE_INTR,
  NULL,
  sduInterruptTransmitted,
  NULL,
  0x0010,
  0x0000,
  &cdcInterruptInstate,
  NULL,
  1,
  NULL
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

    usbInitEndpointI(usbp, USBD1_DATA_REQUEST_EP, &cdcDataEpConfig);
    usbInitEndpointI(usbp, USBD1_INTERRUPT_REQUEST_EP, &cdcInterruptEpConfig);

    /* Resetting the state of the CDC subsystem.*/
    sduConfigureHookI(&SDU1);

    #if HAL_USE_USB_MSD
        // Tell the MMC thread to wake up and mount the card as a USB device
        onUsbConnectedNotifyMmcI();
    #endif

    chSysUnlockFromISR();
    return;
  case USB_EVENT_RESET:
    /* Falls into.*/
  case USB_EVENT_UNCONFIGURED:
    /* Falls into.*/
  case USB_EVENT_SUSPEND:
    chSysLockFromISR();

    /* Disconnection event on suspend.*/
    sduSuspendHookI(&SDU1);

    chSysUnlockFromISR();
    return;
  case USB_EVENT_WAKEUP:
    chSysLockFromISR();

    /* Disconnection event on suspend.*/
    sduWakeupHookI(&SDU1);

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
  sduSOFHookI(&SDU1);
  osalSysUnlockFromISR();
}

bool msd_request_hook_new(USBDriver *usbp);

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
  usb_event,
  get_descriptor,
  hybridRequestHook,
  sof_handler
};

/*
 * Serial over USB driver configuration.
 */
const SerialUSBConfig serusbcfg = {
  .usbp = &USBD1,
  .bulk_in = USBD1_DATA_REQUEST_EP,
  .bulk_out = USBD1_DATA_AVAILABLE_EP,
  .int_in = USBD1_INTERRUPT_REQUEST_EP
};

#endif /* EFI_USB_SERIAL */
