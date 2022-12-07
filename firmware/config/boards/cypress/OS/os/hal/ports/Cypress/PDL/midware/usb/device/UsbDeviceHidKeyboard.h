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
/** \file UsbDeviceHidKeyboard.h
 **
 ** CONST USB_ASSISTANT_OFF
 **
 ** A detailed description is available at 
 ** @link UsbDeviceHidKeyboardGroup USB Device Keyboard Module description @endlink
 **
 ** History:
 **   - 2013-05-08  1.0  MSc  First Version
 **   - 2013-10-14  1.1  MSc  PDL support added
 *****************************************************************************/

#ifndef __USBDEVICEHIDKEYBOARD_H__
#define __USBDEVICEHIDKEYBOARD_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "usb.h"

#ifndef USBDEVICEHIDKEYBOARD_ENABLED
    #define USBDEVICEHIDKEYBOARD_ENABLED OFF
#endif
     
#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEHIDKEYBOARD_ENABLED == ON))

#include "usbdevice.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup UsbDeviceHidKeyboardGroup USB Device Middleware: HID Keyboard
 **
 ** Provided functions of USB Device Keyboard module:
 ** 
 ** - UsbDeviceHidKeyboard_Init()
 ** - UsbDeviceHidKeyboard_SetModifierKey()
 ** - UsbDeviceHidKeyboard_ClearModifierKey()
 ** - UsbDeviceHidKeyboard_KeyPress()
 ** - UsbDeviceHidKeyboard_KeyDown()
 ** - UsbDeviceHidKeyboard_KeyUp()
 ** - UsbDeviceHidKeyboard_IsConnected()
 **
 ** Used to enumerate a keyboard device
 **
 ******************************************************************************/
//@{
    
extern const uint8_t u8AsciiToKeyCode[];

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/



#ifndef MODIFIERKEY_LEFT_CTRL
#define MODIFIERKEY_LEFT_CTRL   (1 << 0)
#endif
#ifndef MODIFIERKEY_LEFT_SHIFT
#define MODIFIERKEY_LEFT_SHIFT  (1 << 1)
#endif
#ifndef MODIFIERKEY_LEFT_ALT
#define MODIFIERKEY_LEFT_ALT    (1 << 2)
#endif
#ifndef MODIFIERKEY_LEFT_GUI
#define MODIFIERKEY_LEFT_GUI    (1 << 3)
#endif

#ifndef MODIFIERKEY_RIGHT_CTRL
#define MODIFIERKEY_RIGHT_CTRL   (1 << 4)
#endif
#ifndef MODIFIERKEY_RIGHT_SHIFT
#define MODIFIERKEY_RIGHT_SHIFT  (1 << 5)
#endif
#ifndef MODIFIERKEY_RIGHT_ALT
#define MODIFIERKEY_RIGHT_ALT    (1 << 6)
#endif
#ifndef MODIFIERKEY_RIGHT_GUI
#define MODIFIERKEY_RIGHT_GUI    (1 << 7)
#endif

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/

typedef struct stc_usbdevice_keyboard
{
    uint8_t u8ModifierKeys;
    uint8_t u8Reserved;
    uint8_t u8KeyCode1;
    uint8_t u8KeyCode2;
    uint8_t u8KeyCode3;
    uint8_t u8KeyCode4;
    uint8_t u8KeyCode5;
    uint8_t u8KeyCode6;
} stc_usbdevice_keyboard_t;

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/



/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

void UsbDeviceHidKeyboard_Init(stc_usbn_t* pstcUsb);
void UsbDeviceHidKeyboard_SetModifierKey(uint8_t u8ModifierKey);
void UsbDeviceHidKeyboard_ClearModifierKey(uint8_t u8ModifierKey);
void UsbDeviceHidKeyboard_KeyPress(uint8_t u8Index, uint8_t u8KeyCode);
void UsbDeviceHidKeyboard_KeyDown(uint8_t u8Index, uint8_t u8KeyCode);
void UsbDeviceHidKeyboard_KeyUp(uint8_t u8Index, uint8_t u8KeyCode);

boolean_t UsbDeviceHidKeyboard_IsConnected(void);
#ifdef __cplusplus
}
#endif

//@} // UsbDeviceHidKeyboardGroup
#else
    #define UsbDeviceHidKeyboard_Init(x) ;
#endif

#endif /* __UsbDeviceHidKeyboard__*/

