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
/** \file UsbDevicePrinter.c
 **
 ** CONST USB_ASSISTANT_OFF
 **
 ** A detailed description is available at 
 ** @link UsbDevicePrinterGroup USB Device Printer Module description @endlink
 **
 ** History:
 **   - 2013-08-12    1.0  MSc  First version
 **   - 2013-10-14    1.1  MSc  PDL support added
 *****************************************************************************/


/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "UsbDevicePrinter.h"

#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEPRINTER_ENABLED == ON))


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


static uint32_t u32LastReceivedSize; // last received size
static stc_usbn_endpoint_buffer_t stcEndpointBufferOUT; //struct for custom buffer 
static uint8_t pu8IngoingBuffer[BUFFER_SIZE_ENDPOINTOUT]; // custom buffer 
static stc_usbn_endpoint_data_t* pstcEndpointOUT;

static stc_usbn_endpoint_buffer_t stcEndpointBufferIN;
stc_usbn_endpoint_buffer_t stcEpBufferIN; //struct for custom buffer
static stc_usbn_endpoint_data_t* pstcEndpointIN;
static stc_usbn_t* pstcUsbHandle = NULL;

static usbdeviceprinter_datareceived_func_ptr_t pstnReceivedData;
static usbdeviceprinter_datasent_func_ptr_t pstnSentData;


/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

static void ClassSetupRequest(stc_usbn_t* pstcUsb, stc_usb_request_t* pstcSetup);

static void UsbDevicePrinter_RxCallback(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint);

static void UsbDevicePrinter_TxCallback(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint);

 
/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 ******************************************************************************
 ** Is called to initialize this class (usally in UsbConfig.c)
 **
 ** \param pstcUsb      USB Handle

 *****************************************************************************/
void UsbDevicePrinter_Init(stc_usbn_t* pstcUsb)
{
    stc_usbdevice_class_config_t stcUsbClassConfig;
    stc_usbdevice_endpoint_config_t stcEndpointConfig;
    uint8_t* pu8Interface = NULL;
    uint8_t u8InterfaceLength = 0;
    uint8_t u8i = 0;
    uint8_t u8NumEndpoints = 0;
    pstcUsbHandle = pstcUsb;

    stcUsbClassConfig.u8InterfaceNumber          = 0xFF;       
    stcUsbClassConfig.u8InterfaceClass           = 0x07;        
    stcUsbClassConfig.u8InterfaceSubClass        = 0x01;                    
    stcUsbClassConfig.u8InterfaceProtocoll       = 0x02;                    
    stcUsbClassConfig.pfnSetupRequestCallback    = ClassSetupRequest;                    
    stcUsbClassConfig.pfnConnectionCallback      = NULL;                    
    stcUsbClassConfig.pfnDisconnectionCallback   = NULL;                    
    stcUsbClassConfig.pfnConfCallback            = NULL;  
    UsbDevice_RegisterVendorClass(pstcUsb,&stcUsbClassConfig,&pu8Interface,&u8InterfaceLength);

    for(u8i = 0;u8i < u8InterfaceLength;)
    {
        u8i += pu8Interface[u8i];
        if (USBDESCR_ENDPOINT == pu8Interface[u8i + 1])
        {
            stcEndpointConfig.u8EndpointAddress = pu8Interface[u8i + 2];
            if (((stcEndpointConfig.u8EndpointAddress) & 0x80) == 0)
            {
                stcEndpointConfig.pfnRxTxCallback = UsbDevicePrinter_RxCallback;
                stcEndpointConfig.pstcEndpointBuffer = &stcEndpointBufferOUT;
                pstcEndpointOUT = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);

                UsbDevice_ReceiveData(pstcUsb, pstcEndpointOUT, pu8IngoingBuffer, BUFFER_SIZE_ENDPOINTOUT, UsbIRQ);
            }
            else
            {
                stcEndpointConfig.pfnRxTxCallback = UsbDevicePrinter_TxCallback;
                stcEndpointConfig.pstcEndpointBuffer = &stcEndpointBufferIN;
                pstcEndpointIN = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);
            }
            u8NumEndpoints++;
        }
    }
}

/**
 ******************************************************************************
 ** Set received callback
 **
 ** \param pstnCallback callback handle
 **
 *****************************************************************************/
void UsbDevicePrinter_SetReceivedCallback(usbdeviceprinter_datareceived_func_ptr_t pstnCallback)
{
    pstnReceivedData = pstnCallback;
}

/**
 ******************************************************************************
 ** Set sent callback
 **
 ** \param pstnCallback callback handle
 **
 *****************************************************************************/
void UsbDevicePrinter_SetSentCallback(usbdeviceprinter_datasent_func_ptr_t pstnCallback)
{
    pstnSentData = pstnCallback;
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
static void ClassSetupRequest(stc_usbn_t* pstcUsb, stc_usb_request_t* pstcSetup)
{
    
}

/**
 ******************************************************************************
 ** Is called from UsbDevice.c when a endpoint buffer was received
 **
 ** \param pstcUsb      USB Handle
 ** \param pstcEndpoint Endpoint

 *****************************************************************************/
static void UsbDevicePrinter_RxCallback(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint)
{
    uint8_t* pu8Buffer = pstcEndpoint->pstcEndpointBuffer->pu8Buffer;
    uint32_t u32DataSize = pstcEndpoint->pstcEndpointBuffer->u32DataSize;
    u32LastReceivedSize = u32DataSize;
    DataReceivedFlags |= (1<<2); // setting data received flag
    if (pstnReceivedData != NULL)
    {
        pstnReceivedData(pu8Buffer,u32DataSize);
    }
    //Add your code here to process the received buffer
    UsbDevice_ReceiveData(pstcUsb, pstcEndpointOUT, pu8IngoingBuffer, BUFFER_SIZE_ENDPOINTOUT, UsbIRQ);
}


/**
 ******************************************************************************
 ** Is used to use received data from endpoint
 **
 ** \param ppu8Buffer pointer to buffer
 *****************************************************************************/
uint32_t UsbDevicePrinter_GetReceivedData(uint8_t** ppu8Buffer)
{
    if ((DataReceivedFlags & (1<<2)) == 0)
    {
        return 0; // nothing to receive
    }
    DataReceivedFlags -= (1<<2);
    *ppu8Buffer = pstcEndpointOUT->pstcEndpointBuffer->pu8Buffer;
    return u32LastReceivedSize;
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
boolean_t UsbDevicePrinter_SendData(stc_usbn_t* pstcUsb, uint8_t* pu8Buffer, uint32_t u32DataSize, en_usbsend_mode_t enMode)
{
    if (UsbDevice_GetStatus(pstcUsb) == UsbConfigured)
    {
        DataSentFlags &= ~(1<<1);
        
        UsbDevice_SendData(pstcUsb, pstcEndpointIN, pu8Buffer, u32DataSize, enMode);
        if (enMode == UsbPOLL)
        {
            DataSentFlags |= (1<<1);
        }
        return TRUE;
    }
    return FALSE;
}

/**
 ******************************************************************************
 ** Is called from UsbDevice.c when a endpoint buffer was sent
 **
 ** \param pstcUsb      USB Handle
 ** \param pstcEndpoint Endpoint
 *****************************************************************************/
static void UsbDevicePrinter_TxCallback(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint)
{
    DataSentFlags |= (1<<1);
    if (pstnSentData != NULL)
    {
        pstnSentData();
    }
}
/**
 ******************************************************************************
 ** Is used to use get the sent status of endpoint
 **
 *****************************************************************************/
uint8_t UsbDevicePrinter_DataSent(void)
{
    if ((DataSentFlags & (1<<1)) > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

#endif
