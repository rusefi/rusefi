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
/************************************************************************/
/** \file usbdevicehw.h
 **
 ** - See README.TXT for project description
 ** - pre release for a simple universal usb function library
 **
 ** History:
 **   - 2010-03-30    1.0  MSc  First version  (works with 16FX,FR80)
 **   - 2011-03-30    1.1  MSc  New HW description style
 **   - 2016-06-23    1.2  MSc  Updated for use with MCU Templates >= v2.0
 *****************************************************************************/

#ifndef __USBDEVICEHW_H__
#define __USBDEVICEHW_H__

#define DEVICE0VBUS_DISABLEISR                            
#define DEVICE0VBUS_ENABLEISR                             
#define DEVICE0VBUS_ISRISSET                              0
#define DEVICE0VBUS_CLEARISRFLAG                          
#define DEVICE0VBUS_SETLOWDETECT                          
#define DEVICE0VBUS_SETHIGHDETECT                         
#define DEVICE0VBUS_HIGHDETECT                            Gpio1pin_Get( GPIO1PIN_P60 )
#define DEVICE0VBUS_INIT                                  Gpio1pin_InitIn( GPIO1PIN_P60, Gpio1pin_InitPullup( 0u ) )
#define DEVICE0VBUS_DEINIT                                
#define DEVICE0VBUS_ENABLED                               1

#define DEVICE0HCONX_INIT                                 
#define DEVICE0HCONX_SET                                  
#define DEVICE0HCONX_CLEAR                                
#define DEVICE0HCONX_ENABLED                              1

#define DEVICE1VBUS_DISABLEISR                            
#define DEVICE1VBUS_ENABLEISR                             
#define DEVICE1VBUS_ISRISSET                              0
#define DEVICE1VBUS_CLEARISRFLAG                          
#define DEVICE1VBUS_SETLOWDETECT                          
#define DEVICE1VBUS_SETHIGHDETECT                         
#define DEVICE1VBUS_HIGHDETECT                            (0)
#define DEVICE1VBUS_INIT                                  
#define DEVICE1VBUS_DEINIT                                
#define DEVICE1VBUS_ENABLED                               0

#define DEVICE1HCONX_INIT                                 
#define DEVICE1HCONX_SET                                  
#define DEVICE1HCONX_CLEAR                                
#define DEVICE1HCONX_ENABLED                              0


#endif
