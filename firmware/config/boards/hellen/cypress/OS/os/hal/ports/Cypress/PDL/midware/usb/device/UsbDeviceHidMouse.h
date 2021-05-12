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
/** \file UsbDeviceHidMouse.h
 **
 ** CONST USB_ASSISTANT_OFF
 **
 ** A detailed description is available at 
 ** @link UsbDeviceHidMouseGroup USB Device Mouse Module description @endlink
 **
 ** History:
 **   - 2013-10-14  1.1  MSc  PDL support added 
 *****************************************************************************/

#ifndef __USBCLASS_H__
#define __USBCLASS_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "usb.h"

#ifndef USBDEVICEHIDMOUSE_ENABLED
    #define USBDEVICEHIDMOUSE_ENABLED OFF
#endif
     
#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEHIDMOUSE_ENABLED == ON))

#include "usbdevice.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup UsbDeviceHidMouseGroup USB Device Middleware: HID Mouse
 **
 ** Provided functions of USB Device Mouse module:
 ** 
 ** - UsbDeviceHidMouse_Init()
 ** - UsbDeviceHidMouse_Scroll()
 ** - UsbDeviceHidMouse_RightClick()
 ** - UsbDeviceHidMouse_RightDoubleClick()
 ** - UsbDeviceHidMouse_RightDown()
 ** - UsbDeviceHidMouse_RightUp()
 ** - UsbDeviceHidMouse_LeftClick()
 ** - UsbDeviceHidMouse_LeftDoubleClick()
 ** - UsbDeviceHidMouse_LeftDown()
 ** - UsbDeviceHidMouse_LeftUp()
 ** - UsbDeviceHidMouse_Move()
 ** - UsbDeviceHidMouse_IsConnected()
 **
 ** Used to enumerate a mouse device
 **
 ******************************************************************************/
//@{
    
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/







/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/



/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/



/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

void UsbDeviceHidMouse_Init(stc_usbn_t* pstcUsb);


void UsbDeviceHidMouse_Scroll(int8_t i8Scroll);
void UsbDeviceHidMouse_RightClick(void);
void UsbDeviceHidMouse_RightDoubleClick(void);
void UsbDeviceHidMouse_RightDown(void);
void UsbDeviceHidMouse_RightUp(void);
void UsbDeviceHidMouse_LeftClick(void);
void UsbDeviceHidMouse_LeftDoubleClick(void);
void UsbDeviceHidMouse_LeftDown(void);
void UsbDeviceHidMouse_LeftUp(void);
void UsbDeviceHidMouse_Move(int8_t i8X, int8_t i8Y);
boolean_t UsbDeviceHidMouse_IsConnected(void);
#ifdef __cplusplus
}
#endif

//@} // UsbDeviceHidMouseGroup
#else
    #define UsbDeviceHidMouse_Init(x) ;
#endif
#endif /* __USBCLASS_H__*/
