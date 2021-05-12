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
/** \file UsbDeviceHidJoystick.h
 **
 ** CONST USB_ASSISTANT_OFF
 **
 ** A detailed description is available at 
 ** @link UsbDeviceHidJoystickGroup USB Device Joystick Module description @endlink
 **
 ** History:
 **   - 2013-10-14    1.1  MSc  PDL support added
 *****************************************************************************/

#ifndef __USBDEVICEHIDJOYSTICK_H__
#define __USBDEVICEHIDJOYSTICK_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "usb.h"

#ifndef USBDEVICEHIDJOYSTICK_ENABLED
    #define USBDEVICEHIDJOYSTICK_ENABLED OFF
#endif
     
#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEHIDJOYSTICK_ENABLED == ON))

#include "usbdevice.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup UsbDeviceHidJoystickGroup USB Device Middleware: HID Joystick
 **
 ** Provided functions of USB Device Joystick module:
 ** 
 ** - UsbDeviceHidJoystick_Init()
 ** - UsbDeviceHidJoystick_ChangeThrottle()
 ** - UsbDeviceHidJoystick_ChangeX()
 ** - UsbDeviceHidJoystick_ChangeY()
 ** - UsbDeviceHidJoystick_ChangeXY()
 ** - UsbDeviceHidJoystick_ChangeButton1()
 ** - UsbDeviceHidJoystick_ChangeButton2()
 ** - UsbDeviceHidJoystick_ChangeButton3()
 ** - UsbDeviceHidJoystick_ChangeButton4()
 ** - UsbDeviceHidJoystick_ChangeHatUp()
 ** - UsbDeviceHidJoystick_ChangeHatRight()
 ** - UsbDeviceHidJoystick_ChangeHatDown()
 ** - UsbDeviceHidJoystick_ChangeHatLeft()
 ** - UsbDeviceHidJoystick_ChangeHatNoDirection()
 ** - UsbDeviceHidJoystick_IsConnected()
 **
 ** Used to enumerate a joystick device
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

void UsbDeviceHidJoystick_Init(stc_usbn_t* pstcUsb);



/**
 ******************************************************************************
 ** Used to change the throttle value
 **
 ** \param iThrottle signed byte value
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeThrottle(int8_t iThrottle);


/**
 ******************************************************************************
 ** Used to change the X value
 **
 ** \param iX signed byte value
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeX(int8_t iX);


/**
 ******************************************************************************
 ** Used to change the Y value
 **
 ** \param iY signed byte value
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeY(int8_t iY);


/**
 ******************************************************************************
 ** Used to change the X and Y value
 **
 ** \param iX signed byte value
 **
 ** \param iY signed byte value
 **
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeXY(int8_t iX, int8_t iY);


/**
 ******************************************************************************
 ** Used to change the button 1 value
 **
 ** \param bButtonValue TRUE = pressed, FALSE = released
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeButton1(boolean_t bButtonValue);


/**
 ******************************************************************************
 ** Used to change the button 2 value
 **
 ** \param bButtonValue TRUE = pressed, FALSE = released
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeButton2(boolean_t bButtonValue);


/**
 ******************************************************************************
 ** Used to change the button 3 value
 **
 ** \param bButtonValue TRUE = pressed, FALSE = released
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeButton3(boolean_t bButtonValue);


/**
 ******************************************************************************
 ** Used to change the button 4 value
 **
 ** \param bButtonValue TRUE = pressed, FALSE = released
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeButton4(boolean_t bButtonValue);


/**
 ******************************************************************************
 ** Used to change the hat direction UP
 **
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeHatUp(void);


/**
 ******************************************************************************
 ** Used to change the hat direction RIGHT
 **
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeHatRight(void);


/**
 ******************************************************************************
 ** Used to change the hat direction DOWN
 **
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeHatDown(void);


/**
 ******************************************************************************
 ** Used to change the hat direction LEFT
 **
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeHatLeft(void);


/**
 ******************************************************************************
 ** Used to change the hat direction no direction
 **
 *****************************************************************************/
void UsbDeviceHidJoystick_ChangeHatNoDirection(void);

/**
 ******************************************************************************
 ** Return connected state
 **
 ** \return TRUE if ready
 **
 *****************************************************************************/
boolean_t UsbDeviceHidJoystick_IsConnected(void);

#ifdef __cplusplus
}
#endif

//@} // UsbDeviceHidJoystickGroup
#else
    #define UsbDeviceHidJoystick_Init(x) ;
#endif
#endif /* __UsbDeviceHidJoystickCLASS_H__*/
