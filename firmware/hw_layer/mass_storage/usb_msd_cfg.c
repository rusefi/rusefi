/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ch.h"
#include "hal.h"
#include "usb_msd_cfg.h"
#include "usb_msd.h"




/*
 * USB Device Descriptor.
 */
static const uint8_t msd_device_descriptor_data[18] = {
  USB_DESC_DEVICE       (0x0200,        /* bcdUSB (2.0).                    */
                         0x00,          /* bDeviceClass (None).             */
                         0x00,          /* bDeviceSubClass.                 */
                         0x00,          /* bDeviceProtocol.                 */
                         0x40,          /* Control Endpoint Size.           */
                         0x0483,        /* idVendor (ST).                   */
                         0x5742,        /* idProduct.                       */
                         0x0100,        /* bcdDevice.                       */
                         1,             /* iManufacturer.                   */
                         2,             /* iProduct.                        */
                         3,             /* iSerialNumber.                   */
                         1)             /* bNumConfigurations.              */
};

/*
 * Device Descriptor wrapper.
 */
static const USBDescriptor msd_device_descriptor = {
  sizeof msd_device_descriptor_data,
  msd_device_descriptor_data
};

/* Configuration Descriptor tree*/
static const uint8_t msd_configuration_descriptor_data[] = {
  /* Configuration Descriptor.*/
  USB_DESC_CONFIGURATION(0x0020,        /* wTotalLength.                    */
                         0x01,          /* bNumInterfaces.                  */
                         0x01,          /* bConfigurationValue.             */
                         0,             /* iConfiguration.                  */
                         0xC0,          /* bmAttributes (self powered).     */
                         0x32),         /* bMaxPower (100mA).               */
  /* Interface Descriptor.*/
  USB_DESC_INTERFACE    (USB_MSD_INTERFACE_NUMBER,          /* bInterfaceNumber.                */
                         0x00,          /* bAlternateSetting.               */
                         0x02,          /* bNumEndpoints.                   */
                         0x08,          /* bInterfaceClass (Mass Storage)   */
                         0x06,          /* bInterfaceSubClass (SCSI
                                           Transparent storage class)       */
                         0x50,          /* bInterfaceProtocol (Bulk Only)   */
                         0),            /* iInterface. (none)               */
  /* Mass Storage Data In Endpoint Descriptor.*/
  USB_DESC_ENDPOINT     (USB_MS_DATA_EP|0x80,
                         0x02,          /* bmAttributes (Bulk).             */
                         USB_MS_EP_SIZE,/* wMaxPacketSize.                  */
                         0x05),         /* bInterval. 1ms                   */
  /* Mass Storage Data In Endpoint Descriptor.*/
  USB_DESC_ENDPOINT     (USB_MS_DATA_EP,
                         0x02,          /* bmAttributes (Bulk).             */
                         USB_MS_EP_SIZE,/* wMaxPacketSize.                  */
                         0x05)          /* bInterval. 1ms                   */
};

/*
 * Configuration Descriptor wrapper.
 */
static const USBDescriptor msd_configuration_descriptor = {
  sizeof msd_configuration_descriptor_data,
  msd_configuration_descriptor_data
};

/*
 * U.S. English language identifier.
 */
static const uint8_t msd_string0[] = {
  USB_DESC_BYTE(4),                     /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  USB_DESC_WORD(0x0409)                 /* wLANGID (U.S. English).          */
};

/*
 * Vendor string.
 */
static const uint8_t msd_string1[] = {
  USB_DESC_BYTE(38),                    /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
  'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
  'c', 0, 's', 0
};

/*
 * Device Description string.
 */
static const uint8_t msd_string2[] = {
  USB_DESC_BYTE(58),                    /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  'r', 0, 'u', 0, 's', 0, 'E', 0, 'f', 0, 'i', 0, 
  ' ', 0, 'M', 0, 'a', 0, 's', 0, 's', 0, ' ', 0,
  'S', 0, 't', 0, 'o', 0, 'r', 0, 'a', 0, 'g', 0, 'e', 0, ' ', 0,
  'D', 0, 'e', 0, 'v', 0, 'i', 0, 'c', 0, 'e', 0
};

static const uint8_t msd_string3[] = {
  USB_DESC_BYTE(26),                     /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  'A', 0, 'E', 0, 'C', 0, 'C', 0, 'E', 0, 'C', 0, 'C', 0, 'C', 0, 'C', 0,
  '0' + CH_KERNEL_MAJOR, 0,
  '0' + CH_KERNEL_MINOR, 0,
  '0' + CH_KERNEL_PATCH, 0
};

/*
 * Strings wrappers array.
 */
static const USBDescriptor msd_strings[] = {
  {sizeof msd_string0, msd_string0},
  {sizeof msd_string1, msd_string1},
  {sizeof msd_string2, msd_string2},
  {sizeof msd_string3, msd_string3}
};

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
    return &msd_device_descriptor;
  case USB_DESCRIPTOR_CONFIGURATION:
    return &msd_configuration_descriptor;
  case USB_DESCRIPTOR_STRING:
    if (dindex < 4)
      return &msd_strings[dindex];
  }
  return NULL;
}



/**
 * @brief   IN EP1 state.
 */
static USBInEndpointState ep1InState;
static USBOutEndpointState ep1OutState;

/**
 * @brief   EP1 initialization structure (IN only).
 */
static const USBEndpointConfig epDataConfig = {
  USB_EP_MODE_TYPE_BULK,
  NULL,
  msdBulkInCallbackComplete,
  msdBulkOutCallbackComplete,
  USB_MS_EP_SIZE,
  USB_MS_EP_SIZE,
  &ep1InState,
  &ep1OutState,
  1,
  NULL
};


/*
 * Handles the USB driver global events.
 */
static void usb_event(USBDriver *usbp, usbevent_t event) {
    USBMassStorageDriver *msdp = (USBMassStorageDriver *)usbp->in_params[USB_MS_DATA_EP - 1];
  switch (event) {
  case USB_EVENT_RESET:
    msdp->reconfigured_or_reset_event = TRUE;
    return;
  case USB_EVENT_ADDRESS:
    return;
  case USB_EVENT_CONFIGURED:
    chSysLockFromISR();
    msdp->reconfigured_or_reset_event = TRUE;
    usbInitEndpointI(usbp, msdp->ms_ep_number, &epDataConfig);
    /* Kick-start the thread */
    chBSemSignalI(&msdp->bsem);

    /* signal that the device is connected */
    chEvtBroadcastI(&msdp->evt_connected);
    chSysUnlockFromISR();

    return;
  case USB_EVENT_SUSPEND:
    return;
  case USB_EVENT_WAKEUP:
    return;
  case USB_EVENT_STALLED:
    return;
  }
  return;
}

const USBConfig msd_usb_config = {
    usb_event,
    get_descriptor,
    msdRequestsHook,
    NULL
};

