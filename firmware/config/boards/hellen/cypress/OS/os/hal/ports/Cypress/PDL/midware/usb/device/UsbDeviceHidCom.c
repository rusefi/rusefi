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
/** \file UsbDeviceHidCom.c
 **
 ** CONST USB_ASSISTANT_OFF
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
#define __USBDEVICEHIDCOM_C__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/

#include "UsbDeviceHidCom.h"

#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEHIDCOM_ENABLED == ON))
/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/

static void RxTxCallback(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint);
static void ConfCallback(stc_usbn_t* pstcUsb);

/******************************************************************************/
/* Local variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

static const uint8_t u8MaxSupportedEndpoints = 2;
static stc_usbn_endpoint_data_t* ppstcUsedEndpoints[2];
static stc_usbn_endpoint_buffer_t pstcEndpointBuffers[2];
static uint8_t ppu8Buffers[2][64];
static const uint32_t pu32BufferSizes[2] = {64,64};
static stc_usbn_endpoint_data_t* pstcEndpointIN;
static stc_usbn_endpoint_data_t* pstcEndpointOUT;
static boolean_t bDataReceived = FALSE;
static boolean_t bDataSent = TRUE;
static boolean_t bReady = FALSE;
static stc_usbn_t* pstcUsbHandle = NULL;

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/******************************************************************************/
/* Procedures / Functions                                                     */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief Configuration change callback, used to see if configuration is set
 **        or cleared
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \return none     
 **
 ******************************************************************************/
static void ConfCallback(stc_usbn_t* pstcUsb)
{
    if (UsbDevice_GetStatus(pstcUsb) == UsbConfigured)
    {
        bReady = TRUE;
    } else
    {
        bReady = FALSE;
    }
}

/**
 ******************************************************************************
 ** \brief Receive or transmitted data callback
 **
 ** \param pstcUsb         Pointer to USB instance
 **
 ** \param pstcEndpoint    Pointer to endpoint instance
 **
 ** \return none     
 **
 ******************************************************************************/
static void RxTxCallback(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint)
{
    volatile uint8_t i;
    volatile uint8_t u8EndpointIndex = 0xFF;
    
    if ((pstcEndpoint->u8EndpointAddress) & 0x80)
    {
        bDataSent = TRUE;
    } 
    else
    {
        bDataReceived = TRUE;
    }
}

/**
 ******************************************************************************
 ** \brief Initialisation callback
 **
 ** \param pstcUsb         Pointer to USB instance
 **
 ** \return none     
 **
 ******************************************************************************/
void UsbDeviceHidCom_Init(stc_usbn_t* pstcUsb)
{
    stc_usbdevice_class_config_t stcUsbClassConfig;
    stc_usbdevice_endpoint_config_t stcEndpointConfig;
    uint8_t* pu8Interface = NULL;
    uint8_t u8InterfaceLength = 0;
    uint8_t u8i = 0;
    uint8_t u8NumEndpoints = 0;
    pstcUsbHandle = pstcUsb;
    
    stcUsbClassConfig.u8InterfaceNumber          = 0xFF;                    // do not use fix interface number, choose by class, subclass, protocol
    stcUsbClassConfig.u8InterfaceClass           = 0x03;                    // HID Class
    stcUsbClassConfig.u8InterfaceSubClass        = 0x00;                    // Custom Sub Class
    stcUsbClassConfig.u8InterfaceProtocoll       = 0x00;                    // Custom Protocol
    stcUsbClassConfig.pfnSetupRequestCallback    = NULL;                    // No setup requests handled
    stcUsbClassConfig.pfnConnectionCallback      = NULL;                    // No connection callback handled
    stcUsbClassConfig.pfnDisconnectionCallback   = NULL;                    // No disconnection callback handled
    stcUsbClassConfig.pfnConfCallback            = ConfCallback;  // Callback for configuration set
    UsbDevice_RegisterVendorClass(pstcUsb,&stcUsbClassConfig,&pu8Interface,&u8InterfaceLength);
    
    for(u8i = 0;u8i < u8InterfaceLength;)
    {
        u8i += pu8Interface[u8i];
        if (USBDESCR_ENDPOINT == pu8Interface[u8i + 1])
        {
            stcEndpointConfig.u8EndpointAddress = pu8Interface[u8i + 2];
            stcEndpointConfig.pfnRxTxCallback = RxTxCallback;
            stcEndpointConfig.pstcEndpointBuffer = &(pstcEndpointBuffers[u8NumEndpoints]);
            ppstcUsedEndpoints[u8NumEndpoints] = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);
            if (((stcEndpointConfig.u8EndpointAddress) & 0x80) == 0)
            {
                pstcEndpointOUT = ppstcUsedEndpoints[u8NumEndpoints];
                UsbDevice_ReceiveData(pstcUsb, ppstcUsedEndpoints[u8NumEndpoints], ppu8Buffers[u8NumEndpoints], pu32BufferSizes[u8NumEndpoints], UsbIRQ);
            }
            else
            {
                pstcEndpointIN = ppstcUsedEndpoints[u8NumEndpoints];
            }
            u8NumEndpoints++;
            if (u8NumEndpoints > u8MaxSupportedEndpoints)
            {
                return;
            }
        }
    }
    
}

/**
 ******************************************************************************
 ** \brief Send data
 **
 ** \param pu8Buffer       Pointer to data buffer
 **
 ** \param enMode          Transfer mode (UsbPOLL, UsbDMA, UsbIRQ)
 **
 ** \return                TRUE on success     
 **
 ******************************************************************************/
boolean_t UsbDeviceHidCom_Send(uint8_t* pu8Buffer, en_usbsend_mode_t enMode)
{
    if ((bReady == FALSE) || (bDataSent == FALSE))
    {
        return FALSE;
    }
    bDataSent = FALSE;
    UsbDevice_SendData(pstcUsbHandle, pstcEndpointIN, pu8Buffer, 64, enMode);
    if (enMode == UsbPOLL)
    {
        bDataSent = TRUE;
    }
    return TRUE;
}

/**
 ******************************************************************************
 ** \brief Check if data transfer is still in process
 **
 ** \return                TRUE on sending     
 **
 ******************************************************************************/
boolean_t UsbDeviceHidCom_IsSending()
{
    return !bDataSent;
}

/**
 ******************************************************************************
 ** \brief Check if data transfer was transmitted
 **
 ** \return                TRUE if data was sent     
 **
 ******************************************************************************/
boolean_t UsbDeviceHidCom_DataSent()
{
    return bDataSent;
}

/**
 ******************************************************************************
 ** \brief Send data
 **
 ** \param pu8OutBuffer    Pointer to data buffer where to write in data
 **
 ** \return                number of bytes read     
 **
 ******************************************************************************/
uint32_t UsbDeviceHidCom_GetReceivedData(uint8_t* pu8OutBuffer)
{
    if (bDataReceived == TRUE)
    {
         bDataReceived = FALSE;
         memcpy(pu8OutBuffer,pstcEndpointOUT->pstcEndpointBuffer->pu8Buffer,64);
         UsbDevice_ReceiveData(pstcUsbHandle, pstcEndpointOUT, pstcEndpointOUT->pstcEndpointBuffer->pu8Buffer, pstcEndpointOUT->pstcEndpointBuffer->u32BufferSize, UsbIRQ);
         return 64;
    }
    return 0;
}
#endif
