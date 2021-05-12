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
/*****************************************************************************/
/** \file UsbDeviceHidCom.h
 **
 ** CONST USB_ASSISTANT_OFF
 **
 **
 ** A detailed description is available at 
 ** @link UsbDeviceHidComGroup USB Device HID Com Module description @endlink
 **
 ** - See README.TXT for project description
 ** - USB Human Interface Device Communication
 **
 ** History:
 **   - 2012-07-20    1.0  MSc  First version for FM3 USB library
 **   - 2013-10-14    1.1  MSc  PDL support added
 *****************************************************************************/

#ifndef __USBDEVICEHIDCOM_H__
#define __USBDEVICEHIDCOM_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/

#include "usb.h"

#ifndef USBDEVICEHIDCOM_ENABLED
    #define USBDEVICEHIDCOM_ENABLED OFF
#endif
     
#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEHIDCOM_ENABLED == ON))

#include "usbdevice.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup UsbDeviceHidComGroup USB Device Middleware: HID Com - Data Communicaton
 **
 ** Provided functions of USB Device HID Communication module:
 ** 
 ** - UsbDeviceHidCom_Init()
 ** - UsbDeviceHidCom_GetReceivedData()
 ** - UsbDeviceHidCom_Send()
 ** - UsbDeviceHidCom_IsSending()
 ** - UsbDeviceHidCom_DataSent()
 **
 ** Used to send or receive 64 byte block data
 **
 ******************************************************************************/
//@{
    
/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/******************************************************************************/
/* Local variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Global function prototypes ('static')                                      */
/******************************************************************************/

void UsbDeviceHidCom_Init(stc_usbn_t* pstcUsb);
uint32_t UsbDeviceHidCom_GetReceivedData(uint8_t* pu8OutBuffer);
boolean_t UsbDeviceHidCom_Send(uint8_t* pu8Buffer, en_usbsend_mode_t enMode);
boolean_t UsbDeviceHidCom_IsSending(void);
boolean_t UsbDeviceHidCom_DataSent(void);
#ifdef __cplusplus
}
#endif

//@} // UsbDeviceHidComGroup
#else
    #define UsbDeviceHidCom_Init(x) ;
#endif

#endif /*__USBDEVICEHIDCOM_H__ */
