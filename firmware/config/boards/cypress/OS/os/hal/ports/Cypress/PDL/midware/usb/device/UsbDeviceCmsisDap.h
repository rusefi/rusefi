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
/** \file UsbDeviceCmsisDap.h
 **
 ** CMSIS-DAP HID Communication Backend 
 ** for protocol layer visit http://www.arm.com
 **
 ** History:
 **   - 2014-02-28    1.0  MSc  First public version
 *****************************************************************************/

#ifndef __USBDEVICECMSISDAP_H__
#define __USBDEVICECMSISDAP_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/

#include "usb.h"
#include "usbdevice.h"

#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICECMSISDAP_ENABLED == ON))

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

void UsbDeviceCmsisDap_Init(stc_usbn_t* pstcUsb);
uint32_t UsbDeviceCmsisDap_GetReceivedData(uint8_t* pu8OutBuffer);
boolean_t UsbDeviceCmsisDap_Send(uint8_t* pu8Buffer, en_usbsend_mode_t enMode);
boolean_t UsbDeviceCmsisDap_IsSending(void);
boolean_t UsbDeviceCmsisDap_DataSent(void);
void UsbDeviceCmsisDap_Tick(void);

#endif
#endif /*__USBDEVICECMSISDAP_H__ */
