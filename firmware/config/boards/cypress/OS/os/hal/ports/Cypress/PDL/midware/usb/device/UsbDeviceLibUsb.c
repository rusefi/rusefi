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
/** \file UsbDeviceLibUsb.c
 **
 **
 ** CONST USB_ASSISTANT_OFF
 **
 ** A detailed description is available at 
 ** @link UsbDeviceLibUsbGroup USB Device LibUSB Module description @endlink
 **
 ** History:
 **   - 2013-10-14    1.1  MSc  PDL support added
 *****************************************************************************/

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "UsbDeviceLibUsb.h"

#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICELIBUSB_ENABLED == ON))

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/



/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/



/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/

static uint8_t DataReceivedFlags = 0; //Status for received data
static uint8_t DataSentFlags = 0; //Status for sent data


static uint32_t u32LastReceivedSize1; // last received size EP1
static stc_usbn_endpoint_buffer_t stcEpBuffer1; //struct for custom buffer EP1
static uint8_t pu8BufferEp1[BUFFER_SIZE_ENDPOINT1]; // custom buffer EP1
static stc_usbn_endpoint_data_t* pstcEpOUT1;

static stc_usbn_endpoint_data_t* pstcEpIN2;
stc_usbn_endpoint_buffer_t stcEpBuffer2; //struct for custom buffer EP2v


/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/



static void UsbDeviceLibUsb_RxCallbackEp1(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint);

static void UsbDeviceLibUsb_TxCallbackEp2(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint);


/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 ******************************************************************************
 ** Is called to initialize this class (usally in UsbConfig.c)
 **
 ** \param pstcUsb      USB Handle

 *****************************************************************************/
void UsbDeviceLibUsb_Init(stc_usbn_t* pstcUsb)
{
    stc_usbdevice_class_config_t stcUsbClassConfig;
    stc_usbdevice_endpoint_config_t stcEndpointConfig;
    uint8_t* pu8Interface = NULL;
    uint8_t u8InterfaceLength = 0;

    stcUsbClassConfig.u8InterfaceNumber          = 0;       
    stcUsbClassConfig.u8InterfaceClass           = 0xFF;        
    stcUsbClassConfig.u8InterfaceSubClass        = 0x00;                    
    stcUsbClassConfig.u8InterfaceProtocoll       = 0x00;                    
    stcUsbClassConfig.pfnSetupRequestCallback    = ClassSetupRequest0;                    
    stcUsbClassConfig.pfnConnectionCallback      = NULL;                    
    stcUsbClassConfig.pfnDisconnectionCallback   = NULL;                    
    stcUsbClassConfig.pfnConfCallback            = NULL;  
    UsbDevice_RegisterVendorClass(pstcUsb,&stcUsbClassConfig,&pu8Interface,&u8InterfaceLength);

    stcEndpointConfig.u8EndpointAddress = 1;
    stcEndpointConfig.pfnRxTxCallback = UsbDeviceLibUsb_RxCallbackEp1;
    stcEndpointConfig.pstcEndpointBuffer = &stcEpBuffer1;
    pstcEpOUT1 = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);
    UsbDevice_ReceiveData(pstcUsb, pstcEpOUT1, pu8BufferEp1, BUFFER_SIZE_ENDPOINT1, UsbIRQ);

    stcEndpointConfig.u8EndpointAddress = 0x80 | 2;
    stcEndpointConfig.pfnRxTxCallback = UsbDeviceLibUsb_TxCallbackEp2;
    stcEndpointConfig.pstcEndpointBuffer = &stcEpBuffer2;
    pstcEpIN2 = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);

}

/**
 ******************************************************************************
 ** Class Setup Request handling
 **
 ** \param pstcUsb USB handle
 **
 ** \param pstcSetup Received Setup
 **
 *****************************************************************************/
static void ClassSetupRequest0(stc_usbn_t* pstcUsb, stc_usb_request_t* pstcSetup)
{
    
}

/**
 ******************************************************************************
 ** Is called from UsbDevice.c when a endpoint buffer was received
 **
 ** \param pstcUsb      USB Handle
 ** \param pstcEndpoint Endpoint

 *****************************************************************************/
static void UsbDeviceLibUsb_RxCallbackEp1(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint)
{
    u32LastReceivedSize1 = pstcEndpoint->pstcEndpointBuffer->u32DataSize;;
    DataReceivedFlags |= (1<<1); // setting data received flag

    //Add your code here to process the received buffer
    UsbDevice_ReceiveData(pstcUsb, pstcEpOUT1, pu8BufferEp1, BUFFER_SIZE_ENDPOINT1, UsbIRQ);
}


/**
 ******************************************************************************
 ** Is used to use received data from endpoint
 **
 ** \param ppu8Buffer pointer to buffer
 *****************************************************************************/
uint32_t UsbDeviceLibUsb_GetReceivedDataEndpoint1(uint8_t** ppu8Buffer)
{
    if ((DataReceivedFlags & (1<<1)) == 0)
    {
        return 0; // nothing to receive
    }
    DataReceivedFlags -= (1<<1);
    *ppu8Buffer = pstcEpOUT1->pstcEndpointBuffer->pu8Buffer;
    return u32LastReceivedSize1;
}


/**
 ******************************************************************************
 ** Is called from main application to send data via endpoint
 **
 ** \param pstcUsb      USB Handle
 ** \param pu8Buffer Buffer to send
 ** \param u32DataSize Buffersize
 ** \param enMode    polled sending, interrupt sending or DMA
 **
 ** \return 1: if succesful, 0: if usb was not ready
 *****************************************************************************/
boolean_t UsbDeviceLibUsb_SendDataVia2(stc_usbn_t* pstcUsb, uint8_t* pu8Buffer, uint32_t u32DataSize, en_usbsend_mode_t enMode)
{
    if (UsbDevice_GetStatus(pstcUsb) == UsbConfigured)
    {
        DataSentFlags &= ~(1<<2);
		
	UsbDevice_SendData(pstcUsb, pstcEpIN2, pu8Buffer, u32DataSize, enMode);
        if (enMode == UsbPOLL)
        {
            DataSentFlags |= (1<<2);
        }
        return 1;
    }
    return 0;
}

/**
 ******************************************************************************
 ** Is called from UsbDevice.c when a endpoint buffer was sent
 **
 ** \param pstcUsb      USB Handle
 ** \param pstcEndpoint Endpoint
 *****************************************************************************/
static void UsbDeviceLibUsb_TxCallbackEp2(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint)
{
    DataSentFlags |= (1<<2);
}
/**
 ******************************************************************************
 ** Is used to use get the sent status of endpoint 2
 **
 *****************************************************************************/
uint8_t UsbDeviceLibUsb_DataSent2(void)
{
    if ((DataSentFlags & (1<<2)) > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#endif
