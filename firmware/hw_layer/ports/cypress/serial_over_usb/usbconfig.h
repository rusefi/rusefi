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
/** \file usbconfig.h
 **
 ** Part of FSEU USB Driver Module
 **
 ** History:
 **   - 2012-07-17    2.0  MSc  New Version for use with M3 L3 USB driver
 **   - 2012-10-02    2.1  MSc  use of external interrupts without L3 implemented
 **   - 2012-01-31    2.2  MSc  DMA settings added
 **   - 2013-06-04    2.3  MSc  FM4 support added
 **   - 2013-09-23    2.4  MSc  Version for PDL
 *****************************************************************************/

#ifndef __USBCONFIG_H__
#define __USBCONFIG_H__

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/

//-------- <<< Use Configuration Wizard in Context Menu>>> -----------------
// <h>USB Debug
// =======================
//
//   <o>Use USB debug functionality via UART
//      <0=> OFF
//      <1=> ON
#ifndef USEUSBDBG
    #define USEUSBDBG            0
#endif

//   <o>Precompiler Warning Level
//      <0=> no warnings
//      <1=> hard warnings
//      <2=> all warnings
#ifndef USBWARNLEVEL
    #define USBWARNLEVEL         0           //0: no warnings, 1: hard warnings, 2: all warnings
#endif
// </h>

// <h>Use of Headerfiles
// =======================
//
//   <o>Use usbdescriptors.h
//      <0=> OFF
//      <1=> ON
#ifndef USE_USBDESCRIPTORS_H 
    #define USE_USBDESCRIPTORS_H 1           //<- 1 use usbdescriptors.h file for USB descriptors settings, 0 for disabling
#endif

//   <o>Use usbdevicehw.h
//      <0=> OFF
//      <1=> ON
#ifndef USE_USBDEVICEHW_H
    #define USE_USBDEVICEHW_H    1           //<- 1 use usbdevicehw.h file for gpio settings, 0 for disabling
#endif

//   <o>Use usbhosthw.h
//      <0=> OFF
//      <1=> ON
#ifndef USE_USBHOSTHW_H 
    #define USE_USBHOSTHW_H      1           //<- 1 use usbhosthw.h file for gpio settings, 0 for disabling
#endif

//   <o>Use sbhostclassdrivertable.h
//      <0=> OFF
//      <1=> ON
#ifndef USE_USBHOSTCLASSDRIVERTABLE_H
    #define USE_USBHOSTCLASSDRIVERTABLE_H 0  //<- 1 use usbhostclassdrivertable.h file
#endif
// </h>

// <h>Use USB within a low level library 
// =======================
//
//   <o>Use with L3 (old library for FM3 MCUs)
//      <0=> OFF
//      <1=> ON
#ifndef USB_USE_L3
#define USB_USE_L3                                        0                   //<- 1 use as part of L3 library, 0 for using without L3 library
#endif

//   <o>Use with PDL
//      <0=> OFF
//      <1=> ON
#define USB_USE_PDL                                       0                   //<- 1 use as part of PDL library, 0 for using without PDL library
//</h>


#if (USB_USE_PDL == 0) && (USB_USE_L3 == 0)
/* START Middleware Modules */

/* DEVICE */     
// <h>USB Device middleware modules
// =======================
//
//   <o>USB Device CDC
//      <0=> OFF
//      <1=> ON
#ifndef USBDEVICECDCCOM_ENABLED
#define USBDEVICECDCCOM_ENABLED                           ON                  //Middleware USB CDC Communication Class
#endif

//   <o>USB Device HID (data communication)
//      <0=> OFF
//      <1=> ON
#ifndef USBDEVICEHIDCOM_ENABLED
    #define USBDEVICEHIDCOM_ENABLED          OFF
#endif

//   <o>USB Device HID Joystick
//      <0=> OFF
//      <1=> ON
#ifndef USBDEVICEHIDJOYSTICK_ENABLED
    #define USBDEVICEHIDJOYSTICK_ENABLED     OFF
#endif

//   <o>USB Device HID Keyboard
//      <0=> OFF
//      <1=> ON
#ifndef USBDEVICEHIDKEYBOARD_ENABLED    
    #define USBDEVICEHIDKEYBOARD_ENABLED     OFF
#endif

//   <o>USB Device HID Mouse
//      <0=> OFF
//      <1=> ON
#ifndef USBDEVICEHIDMOUSE_ENABLED    
    #define USBDEVICEHIDMOUSE_ENABLED        OFF
#endif

//   <o>USB Device LibUSB
//      <0=> OFF
//      <1=> ON
#ifndef USBDEVICELIBUSB_ENABLED    
    #define USBDEVICELIBUSB_ENABLED          OFF
#endif

//   <o>USB Device Printer
//      <0=> OFF
//      <1=> ON
#ifndef USBDEVICEPRINTER_ENABLED    
    #define USBDEVICEPRINTER_ENABLED         OFF
#endif

//   <o>USB Device Mass Storage
//      <0=> OFF
//      <1=> ON
#ifndef USBDEVICEMASSSTORAGE_ENABLED    
    #define USBDEVICEMASSSTORAGE_ENABLED     OFF
#endif
//</h>



/* HOST */
// <h>USB Host middleware modules
// =======================
//
//   <o>USB Host HID (Data communication)
//      <0=> OFF
//      <1=> ON
#ifndef USBHOSTHIDCOM_ENABLED
#define USBHOSTHIDCOM_ENABLED                             OFF
#endif

//   <o>USB Host HID Keyboard
//      <0=> OFF
//      <1=> ON
#ifndef USBHOSTHIDKEYBOARD_ENABLED
#define USBHOSTHIDKEYBOARD_ENABLED                        OFF /* [andreika] */
#endif

//   <o>USB Host HID Mouse
//      <0=> OFF
//      <1=> ON
#ifndef USBHOSTHIDMOUSE_ENABLED    
#define USBHOSTHIDMOUSE_ENABLED                           OFF /* [andreika] */
#endif

//   <o>USB Host Mass Storage
//      <0=> OFF
//      <1=> ON
#ifndef USBHOSTMASSSTORAGE_ENABLED    
#define USBHOSTMASSSTORAGE_ENABLED                        OFF /* [andreika] */
#endif

//   <o>USB Host NDIS
//      <0=> OFF
//      <1=> ON
#ifndef USBHOSTNDIS_ENABLED    
#define USBHOSTNDIS_ENABLED                               OFF /* [andreika] */
#endif

//   <o>USB Host Printer
//      <0=> OFF
//      <1=> ON
#ifndef USBHOSTPRINTER_ENABLED  
#define USBHOSTPRINTER_ENABLED                            OFF /* [andreika] */
#endif
//</h>
     
/* END Middleware Modules */
     
/* only used if USB is used without L3 or PDL*/
#define USB0_HOST_ENABLED                                 0
#define USB0_DEVICE_ENABLED                               1
#define USB1_HOST_ENABLED                                 0
#define USB1_DEVICE_ENABLED                               0
#define USB0_DEVICE_IRQ_ENABLED 1
#define USB0_HOST_IRQ_ENABLED   1
#define USB1_DEVICE_IRQ_ENABLED 1
#define USB1_HOST_IRQ_ENABLED   1
#define USB_USE_EXT_INT         0
#define IRQ_LEVEL_USB0          3  
#define IRQ_LEVEL_USB1          3  
#define USB_USES_DMA            0  
#define USB_USES_DMA_0          1
#define USB_USES_DMA_1          1
#define USB_USES_DMA_2          0
#define USB_USES_DMA_3          0
#define USB_USES_DMA_4          0
#define USB_USES_DMA_5          0
#define USB_USES_DMA_6          0
#define USB_USES_DMA_7          0

#endif //(USB_USE_PDL == 0) && (USB_USE_L3 == 0)

#endif
