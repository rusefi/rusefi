/*******************************************************************************
* Copyright (C) 2013-2016, Cypress Semiconductor Corporation or a              *
* subsidiary of Cypress Semiconductor Corporation.  All rights reserved.       *
*                                                                              *
* This software, including source code, documentation and related              *
* materials ("Software"), is owned by Cypress Semiconductor Corporation or     *
* one of its subsidiaries ("Cypress") and is protected by and subject to       *
* worldwide patent protection (United States and foreign), United States       *
* copyright laws and international treaty provisions. Therefore, you may use   *
* this Software only as provided in the license agreement accompanying the     *
* software package from which you obtained this Software ("EULA").             *
*                                                                              *
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,     *
* non-transferable license to copy, modify, and compile the                    *
* Software source code solely for use in connection with Cypress's             *
* integrated circuit products.  Any reproduction, modification, translation,   *
* compilation, or representation of this Software except as specified          *
* above is prohibited without the express written permission of Cypress.       *
*                                                                              *
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO                         *
* WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING,                         *
* BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED                                 *
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A                              *
* PARTICULAR PURPOSE. Cypress reserves the right to make                       *
* changes to the Software without notice. Cypress does not assume any          *
* liability arising out of the application or use of the Software or any       *
* product or circuit described in the Software. Cypress does not               *
* authorize its products for use in any products where a malfunction or        *
* failure of the Cypress product may reasonably be expected to result in       *
* significant property damage, injury or death ("High Risk Product"). By       *
* including Cypress's product in a High Risk Product, the manufacturer         *
* of such system or application assumes all risk of such use and in doing      *
* so agrees to indemnify Cypress against all liability.                        *
*******************************************************************************/
/******************************************************************************/
/** \file UsbDescriptors.h
 **
 ** USB Descriptors File
 **
 ** History:
 **   - 2020-1-27  1.0  MSc  Automatically Created by Spansion USB Wizard
 *****************************************************************************/

#ifndef __USBDESCRIPTORS_H__
#define __USBDESCRIPTORS_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/



/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/

#define USB_DEVDESC_SIZE 18
#define USB_CNFGDESC_SIZE 53
#define USB_FUNC_EP0_SIZE 64
#define CLASSNAME "UsbDeviceCdcCom"


/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/

#define USBDESCRIPTORS_STRINGDESCRIPTOR_COUNT (uint32_t)(sizeof(pstcStringDescriptors) / sizeof(pstcStringDescriptors[0]))




/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/
// [andreika]: use ST-compatible vendor+product IDs
const uint8_t au8DeviceDescriptor[18] = 
{
    ///DEVICE DESCRIPTOR
    0x12,    ///bLength: Length of this descriptor
    0x01,    ///bDescriptorType: Device Descriptor Type
    0x10,    ///bcdUSB: USB Version
    0x01,    ///bcdUSB: USB Version
    0x02,    ///bDeviceClass: Class Code: COMMUNICATIONS_AND_CDC_CONTROL
    0x00,    ///bDeviceSubClass: Sub Class Code
    0x00,    ///bDeviceProtocol: Protocol Code
    0x40,    ///bMaxPacketSize0: Maximum size of endpoint 0
    0x83,    ///idVendor: Vendor ID
    0x04,    ///idVendor: Vendor ID
    0x40,    ///idProduct: Product ID
    0x57,    ///idProduct: Product ID
    0x00,    ///bcdDevice: Release Number
    0x02,    ///bcdDevice: Release Number
    0x01,    ///iManufacture: String-Index of Manufacture
    0x02,    ///iProduct: String-Index of Product
    0x03,    ///iSerialNumber: String-Index of Serial Number
    0x01    ///bNumConfigurations: Number of possible configurations
};

const uint8_t au8ConfigDescriptor[53] =
{        ///NEW CONFIG DESCRIPTOR(1)
        0x09,    ///bLength: Length of this descriptor
        0x02,    ///bDescriptorType: Config Descriptor Type
        0x35,    ///wTotalLength: Total Length with all interface- and endpoint descriptors
        0x00,    ///wTotalLength: Total Length with all interface- and endpoint descriptors
        0x02,    ///bNumInterfaces: Number of interfaces
        0x01,    ///iConfigurationValue: Number of this configuration
        0x00,    ///iConfiguration: String index of this configuration
        0xC0,    ///bmAttributes: Bus-Powered, Remote-Wakeup not supported
        0xFA,    ///MaxPower: (in 2mA)
            ///NEW INTERFACE DESCRIPTOR(0)
            0x09,    ///bLength: Length of this descriptor
            0x04,    ///bDescriptorType: Interface Descriptor Type
            0x00,    ///bInterfaceNumber: Interface Number
            0x00,    ///bAlternateSetting: Alternate setting for this interface
            0x01,    ///bNumEndpoints: Number of endpoints in this interface excluding endpoint 0
            0x02,    ///iInterfaceClass: Class Code: COMMUNICATIONS_AND_CDC_CONTROL
            0x02,    ///iInterfaceSubClass: SubClass Code
            0x01,    ///bInterfaceProtocol: Protocol Code
            0x00,    ///iInterface: String index
                ///NEW FUNCTION DESCRIPTOR(0)
                0x05,    ///bLength: Length of this descriptor
                0x24,    ///bDescriptorType: Class Specific Interface Descriptor Type
                0x06,    ///bDescriptorSubtype: Union Functional descriptor
                0x00,    ///Master Interface (Control)
                0x01,    ///Slave Interface (Data)
                ///NEW ENDPOINT DESCRIPTOR(1)
                0x07,    ///bLength: Length of this descriptor
                0x05,    ///bDescriptorType: Endpoint Descriptor Type
                0x82,    ///bEndpointAddress: Endpoint address (IN,EP2)
                0x03,    ///bmAttributes: Transfer Type: INTERRUPT_TRANSFER
                0x40,    ///wMaxPacketSize: Endpoint Size
                0x00,    ///wMaxPacketSize: Endpoint Size
                0xFF,    ///bIntervall: Polling Intervall
            ///NEW INTERFACE DESCRIPTOR(1)
            0x09,    ///bLength: Length of this descriptor
            0x04,    ///bDescriptorType: Interface Descriptor Type
            0x01,    ///bInterfaceNumber: Interface Number
            0x00,    ///bAlternateSetting: Alternate setting for this interface
            0x02,    ///bNumEndpoints: Number of endpoints in this interface excluding endpoint 0
            0x0A,    ///iInterfaceClass: Class Code: CDC_DATA
            0x00,    ///iInterfaceSubClass: SubClass Code
            0x00,    ///bInterfaceProtocol: Protocol Code
            0x00,    ///iInterface: String index
                ///NEW ENDPOINT DESCRIPTOR(0)
                0x07,    ///bLength: Length of this descriptor
                0x05,    ///bDescriptorType: Endpoint Descriptor Type
                0x03,    ///bEndpointAddress: Endpoint address (OUT,EP3)
                0x02,    ///bmAttributes: Transfer Type: BULK_TRANSFER
                0x40,    ///wMaxPacketSize: Endpoint Size
                0x00,    ///wMaxPacketSize: Endpoint Size
                0x00,    ///bIntervall: Polling Intervall
                ///NEW ENDPOINT DESCRIPTOR(1)
                0x07,    ///bLength: Length of this descriptor
                0x05,    ///bDescriptorType: Endpoint Descriptor Type
                0x81,    ///bEndpointAddress: Endpoint address (IN,EP1)
                0x02,    ///bmAttributes: Transfer Type: BULK_TRANSFER
                0x40,    ///wMaxPacketSize: Endpoint Size
                0x00,    ///wMaxPacketSize: Endpoint Size
                0x00    ///bIntervall: Polling Intervall
};

const uint8_t au8ReportDescriptor0[1] = { 0 }; // Not used
const uint8_t au8ReportDescriptor1[1] = { 0 }; // Not used
const uint8_t au8ReportDescriptor2[1] = { 0 }; // Not used


const stc_usbdevice_stringdescriptor_t pstcStringDescriptors[] =
{
{(uint8_t*)"Spansion International Inc.",NULL},    //Manufacturer String
{(uint8_t*)"rusEFI ECU Comm Port",NULL},    //Product String
{(uint8_t*)"1.0",NULL},    //Serial Number String
};


const stc_usbdevice_reportdescriptor_t astcReportDescriptors[3] =
{
    {(uint8_t*)au8ReportDescriptor0,sizeof(au8ReportDescriptor0)},
    {(uint8_t*)au8ReportDescriptor1,sizeof(au8ReportDescriptor1)},
    {(uint8_t*)au8ReportDescriptor2,sizeof(au8ReportDescriptor2)},
};

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/





#endif /* __USBDESCRIPTORS_H__ */
