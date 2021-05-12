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
/** \file UsbDeviceCmsisDap.c
 **
 ** CMSIS-DAP HID Communication Backend 
 ** for protocol layer visit http://www.arm.com
 **
 ** History:
 **   - 2014-02-28    1.0  MSc  First public version
 *****************************************************************************/
#define __USBDEVICECMSISDAP_C__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/

#include "UsbDeviceCmsisDap.h"

#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICECMSISDAP_ENABLED == ON))

#include "DAP_config.h"
#include "DAP.h"

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/

static void RxTxCallback(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint);
static void ConfCallback(stc_usbn_t* pstcUsb);
extern uint32_t DAP_ProcessVendorCommand(uint8_t *request, uint8_t *response);

/******************************************************************************/
/* Local variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

#if (64 != DAP_PACKET_SIZE)
#error "USB HID Output Report Size must match DAP Packet Size"
#endif
#if (64 != DAP_PACKET_SIZE)
#error "USB HID Input Report Size must match DAP Packet Size"
#endif

static volatile uint8_t  USB_RequestFlag;       // Request  Buffer Usage Flag
static volatile uint32_t USB_RequestIn;         // Request  Buffer In  Index
static volatile uint32_t USB_RequestOut;        // Request  Buffer Out Index

static volatile uint8_t  USB_ResponseIdle;      // Response Buffer Idle  Flag
static volatile uint8_t  USB_ResponseFlag;      // Response Buffer Usage Flag
static volatile uint32_t USB_ResponseIn;        // Response Buffer In  Index
static volatile uint32_t USB_ResponseOut;       // Response Buffer Out Index


//static const uint8_t u8MaxSupportedEndpoints = 2;
static stc_usbn_endpoint_buffer_t stcEndpointBufferIN;
static stc_usbn_endpoint_buffer_t stcEndpointBufferOUT;

static          uint8_t  USB_Request [DAP_PACKET_COUNT][DAP_PACKET_SIZE];  // Request  Buffer
static          uint8_t  USB_Response[DAP_PACKET_COUNT][DAP_PACKET_SIZE];  // Response Buffer
uint8_t pu8BufferOUT[64];
uint8_t pu8BufferIN[64];

static stc_usbn_endpoint_data_t* pstcEndpointIN = NULL;
static stc_usbn_endpoint_data_t* pstcEndpointOUT = NULL;

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
        DAP_Setup();                          // DAP Setup
        bDataReceived = FALSE;
        bDataSent = TRUE;
        USB_RequestFlag   = 0;
        USB_RequestIn     = 0;
        USB_RequestOut    = 0;
        USB_ResponseIdle  = 1;
        USB_ResponseFlag  = 0;
        USB_ResponseIn    = 0;
        USB_ResponseOut   = 0;
        bReady = TRUE;
        LED_CONNECTED_OUT(1);                 // Turn on  Debugger Connected LED
        Delayms(50);                         // Wait for 500ms
        LED_RUNNING_OUT(1);                   // Turn on  Target Running LED
        Delayms(50);                         // Wait for 500ms
        LED_CONNECTED_OUT(0);                 // Turn off Debugger Connected LED
        Delayms(50);                         // Wait for 500ms
        LED_RUNNING_OUT(0);                   // Turn off Target Running LED
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
        
        if (pu8BufferOUT[0] == ID_DAP_TransferAbort) {
            DAP_TransferAbort = 1;
            return;
        }
        if (USB_RequestFlag && (USB_RequestIn == USB_RequestOut)) {
            return;  // Discard packet when buffer is full
        }
        // Store data into request packet buffer
        memcpy(USB_Request[USB_RequestIn], pstcEndpoint->pstcEndpointBuffer->pu8Buffer, 64);
        UsbDevice_ReceiveData(pstcUsb, pstcEndpoint, pstcEndpoint->pstcEndpointBuffer->pu8Buffer, 64, UsbIRQ);
        USB_RequestIn++;
        if (USB_RequestIn == DAP_PACKET_COUNT) {
            USB_RequestIn = 0;
        }
        if (USB_RequestIn == USB_RequestOut)
        {
            USB_RequestFlag = 1;
        }
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
void UsbDeviceCmsisDap_Init(stc_usbn_t* pstcUsb)
{
    stc_usbdevice_class_config_t stcUsbClassConfig;
    stc_usbdevice_endpoint_config_t stcEndpointConfig;
    uint8_t* pu8Interface = NULL;
    uint8_t u8InterfaceLength = 0;
    uint8_t u8i = 0;
    //uint8_t u8NumEndpoints = 0;
    pstcUsbHandle = pstcUsb;
    
    pstcEndpointIN = NULL;
    pstcEndpointOUT = NULL;
        
    USB_RequestFlag   = 0;
    USB_RequestIn     = 0;
    USB_RequestOut    = 0;
    USB_ResponseIdle  = 1;
    USB_ResponseFlag  = 0;
    USB_ResponseIn    = 0;
    USB_ResponseOut   = 0;
  
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
            
            if (((stcEndpointConfig.u8EndpointAddress) & 0x80) == 0)
            {
                stcEndpointConfig.pstcEndpointBuffer = &stcEndpointBufferOUT;
                pstcEndpointOUT = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);
                UsbDevice_ReceiveData(pstcUsb, pstcEndpointOUT, pu8BufferOUT, 64, UsbIRQ);
            }
            else
            {
                stcEndpointConfig.pstcEndpointBuffer = &stcEndpointBufferIN;
                pstcEndpointIN = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);
            }
            if ((pstcEndpointIN != NULL) && (pstcEndpointOUT != NULL))
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
boolean_t UsbDeviceCmsisDap_Send(uint8_t* pu8Buffer, en_usbsend_mode_t enMode)
{
    if ((bReady == FALSE) || (bDataSent == FALSE))
    {
        return FALSE;
    }
    bDataSent = FALSE;
    UsbDevice_SendData(pstcUsbHandle, pstcEndpointIN, pu8Buffer, 64, enMode);
    if ((USB_ResponseOut != USB_ResponseIn) || USB_ResponseFlag) {
        
        USB_ResponseOut++;
        if (USB_ResponseOut == DAP_PACKET_COUNT) {
          USB_ResponseOut = 0;
        }
        if (USB_ResponseOut == USB_ResponseIn) {
          USB_ResponseFlag = 0;
        }
        return TRUE;
    } else {
        USB_ResponseIdle = 1;
    }
    
    if (enMode == UsbPOLL)
    {
        bDataSent = TRUE;
    }
    return TRUE;
}

void UsbDeviceCmsisDap_Tick(void)
{
  uint32_t n;

  // Process pending requests
  if ((USB_RequestOut != USB_RequestIn) || USB_RequestFlag) {

    // Process DAP Command and prepare response
    DAP_ProcessCommand(USB_Request[USB_RequestOut], USB_Response[USB_ResponseIn]);
    DAP_ProcessVendorCommand(USB_Request[USB_RequestOut], USB_Response[USB_ResponseIn]);
    // Update request index and flag
    n = USB_RequestOut + 1;
    if (n == DAP_PACKET_COUNT) {
      n = 0;
    }
    USB_RequestOut = n;
    if (USB_RequestOut == USB_RequestIn) {
      USB_RequestFlag = 0;
    }

    if (USB_ResponseIdle) {
      // Request that data is send back to host
      USB_ResponseIdle = 0;
      #if USB_USES_DMA == ON
      UsbDeviceCmsisDap_Send(USB_Response[USB_ResponseOut],UsbDMA);
      #else
      UsbDeviceCmsisDap_Send(USB_Response[USB_ResponseOut],UsbIRQ);
      #endif
    } else {      
      // Update response index and flag
      n = USB_ResponseIn + 1;
      if (n == DAP_PACKET_COUNT) {
        n = 0;
      }
      USB_ResponseIn = n;
      if (USB_ResponseIn == USB_ResponseOut) {
        USB_ResponseFlag = 1;
      }
    }
  }
}

/**
 ******************************************************************************
 ** \brief Check if data transfer is still in process
 **
 ** \return                TRUE on sending     
 **
 ******************************************************************************/
boolean_t UsbDeviceCmsisDap_IsSending()
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
boolean_t UsbDeviceCmsisDap_DataSent()
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
uint32_t UsbDeviceCmsisDap_GetReceivedData(uint8_t* pu8OutBuffer)
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
