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
/** \file UsbDevicePrinter.h
 **
 ** CONST USB_ASSISTANT_OFF
 **
 **
 ** A detailed description is available at 
 ** @link UsbDevicePrinterGroup USB Device Printer Module description @endlink
 **
 ** History:
 **   - 2013-08-12    1.0  MSc  First version
 **   - 2013-10-14    1.1  MSc  PDL support added
 *****************************************************************************/

#ifndef __USBDEVICEPRINTER_H__
#define __USBDEVICEPRINTER_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "usb.h"

#ifndef USBDEVICEPRINTER_ENABLED
    #define USBDEVICEPRINTER_ENABLED OFF
#endif
     
#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEPRINTER_ENABLED == ON))

#include "usbdevice.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup UsbDevicePrinterGroup USB Device Middleware: Printer Class
 **
 ** Provided functions of USB Device Printer module:
 ** 
 ** - void UsbDevicePrinter_Init()
 ** - UsbDevicePrinter_GetReceivedData()
 ** - UsbDevicePrinter_SendData()
 ** - UsbDevicePrinter_DataSent(void)
 ** - UsbDevicePrinter_SetReceivedCallback()
 ** - UsbDevicePrinter_SetSentCallback()
 **
 ** Used to enumerate printe class
 **
 ******************************************************************************/
//@{
    
/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/



#define BUFFER_SIZE_ENDPOINTOUT 512



/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/


typedef void (*usbdeviceprinter_datareceived_func_ptr_t)(uint8_t* pu8DataBuffer, uint32_t u32DataLength);
typedef void (*usbdeviceprinter_datasent_func_ptr_t)(void);


/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/



/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

void UsbDevicePrinter_Init(stc_usbn_t* pstcUsb);



uint32_t UsbDevicePrinter_GetReceivedData(uint8_t** ppu8Buffer);

boolean_t UsbDevicePrinter_SendData(stc_usbn_t* pstcUsb, uint8_t* pu8Buffer, uint32_t u32DataSize, en_usbsend_mode_t enMode);
uint8_t UsbDevicePrinter_DataSent(void);
void UsbDevicePrinter_SetReceivedCallback(usbdeviceprinter_datareceived_func_ptr_t pstnCallback);
void UsbDevicePrinter_SetSentCallback(usbdeviceprinter_datasent_func_ptr_t pstnCallback);
#ifdef __cplusplus
}
#endif

//@} // UsbDevicePrinterGroup

#else
    #define UsbDevicePrinter_Init(x) ;
#endif

#endif /* __USBCLASS_H__*/
