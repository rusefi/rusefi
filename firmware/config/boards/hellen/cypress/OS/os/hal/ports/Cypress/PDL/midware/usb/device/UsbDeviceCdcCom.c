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
/** \file UsbDeviceCdcCom.c
 **
 ** CONST USB_ASSISTANT_OFF
 **
 ** A detailed description is available at 
 ** @link UsbDeviceCdcComGroup USB Device Cdc Com Module description @endlink
 **
 ** History:
 **   - 2009-08-31    1.0  MSc  First version  (works with 16FX)
 **   - 2010-04-16    1.1  MSc  new USB library added - API had changed
 **   - 2011-08-30    1.2  MSc  bug fixes while echo data
 **                             added UsbDeviceCdcCom_SetEchoAtSeperatorOnly for
 **                             echo data only after seperator received
 **   - 2012-07-24    1.3  MSc  Version for USB Library >= FM3 
 **   - 2012-11-26    1.4  MSc  Data received routine added 
 **   - 2013-01-30    1.5  MSc  DMA routines added 
 **   - 2013-04-24    1.6  MSc  Data sent handle added
 **   - 2013-10-14    1.7  MSc  PDL support added
 **   - 2014-08-25    1.8  MSc  Polled data changed to IRQ transfer
 **                             USE_DTR_FOR_CONNECT to enable/disable DTR 
 **                             USBDEVICECDCCOM_USE_PRINTF to enable printf
 **   - 2015-09-04    1.9  MSCH Usb_WaitHook() added
 *****************************************************************************/

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "UsbDeviceCdcCom.h"

#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICECDCCOM_ENABLED == ON))

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/



/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

static const char_t pcASCII[] = "0123456789ABCDEF";

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/



/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/

static volatile uint8_t DataReceivedFlags = 0; //Status for received data
static volatile uint8_t DataSentFlags = 0xFF; //Status for sent data
static uint8_t pu8VcommBuffer[USBDEVICECDCCOM_MAX_RECEIVE_BUFFER];
static stc_cdc_linecoding_t stcLineCoding = {9600,USBCLASSCDC_CHARFORMAT_1STOPBIT,USBCLASSCDC_PARITYTYPE_NONE,8};
static uint8_t pu8LineCoding[8];
static boolean_t bEchoMode = FALSE;
static boolean_t bEchoAtSeperatorOnly = FALSE;
static volatile boolean_t bVcommActive = FALSE;
static boolean_t bSeperatorFlag = FALSE;
static char_t cSeperator = '\0';
static volatile uint32_t u32ReceivedData = 0;
static uint8_t pu8IngoingBuffer[USBDEVICECDCCOM_EPOUT_BUFFERSIZE];
static boolean_t bReady = FALSE;
static stc_usbn_t* pstcUsbHandle = NULL;
static uint8_t u8ControlInterface = 0;
static stc_cdc_uart_state_t stcSerialState;
static stc_usbn_endpoint_data_t* pstcEndpointControlIN;
static stc_usbn_endpoint_buffer_t stcEndpointBufferControlIN;
static stc_usbn_endpoint_data_t* pstcEndpointIN;
static stc_usbn_endpoint_data_t* pstcEndpointOUT;
static stc_usbn_endpoint_buffer_t stcEndpointBufferIN;
static stc_usbn_endpoint_buffer_t stcEndpointBufferOUT;
static usbdevicecdccom_linecodingchange_func_ptr_t pfnLineCodingChange;
static cdc_dtr_changed_t pstnDtrChanged = NULL;
static cdc_rts_changed_t pstnRtsChanged = NULL;
#if (USBDEVICECDCCOM_USE_PRINTF == 1)
    char_t tbuf;

    static   long        brk_siz = 0;
    #if    HEAP_SIZE
        typedef  int         _heap_t;
        #define ROUNDUP(s)   (((s)+sizeof(_heap_t)-1)&~(sizeof(_heap_t)-1))
        static   _heap_t     _heap[ROUNDUP(HEAP_SIZE)/sizeof(_heap_t)];
        #define              _heap_size       ROUNDUP(HEAP_SIZE)
    #else
        extern  char         *_heap;
        extern  long         _heap_size;
    #endif
#endif
        
/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/
static void ConfCallback(stc_usbn_t* pstcUsb);
static void RxTxCallback(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint);
static boolean_t SendData(uint8_t* pu8Buffer, uint32_t u32DataSize, en_usbsend_mode_t enMode);
static void DecodeSetupRequest(stc_usbn_t* pstcUsb, stc_usb_request_t* pstcSetup);
static void SetLineCoding(stc_usbn_t* pstcUsb, stc_usbn_endpoint_data_t* pstcEndpoint);
static void SendLineCoding(void);
static uint32_t ASCIItobin(uint8_t k);
static cdc_data_received_t pstnReceivedData;
static cdc_data_sent_t pstnSentData;

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 ******************************************************************************
 ** \brief Configuration change callback, used to see if configuration is set
 **        or cleared
 **
 ** \param pstcUsb Pointer to USB instance
 **  
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
    #if USE_DTR_FOR_CONNECT == 0
        bVcommActive = bReady;
    #endif
}

/**
 ******************************************************************************
 ** \brief Receive or transmitted data callback
 **
 ** \param pstcUsb         Pointer to USB instance
 **
 ** \param pstcEndpoint    Pointer to endpoint instance
 **   
 **
 ******************************************************************************/
static void RxTxCallback(stc_usbn_t* pstcUsb, struct stc_usbn_endpoint_data* pstcEndpoint)
{
    volatile uint32_t u32i;
    uint8_t* pu8Buffer = pstcEndpoint->pstcEndpointBuffer->pu8Buffer;
    uint32_t u32DataSize = pstcEndpoint->pstcEndpointBuffer->u32DataSize;
    
    if ((pstcEndpoint->u8EndpointAddress) & 0x80)
    {
        DataSentFlags |= (1<<1);
        if (pstnSentData != NULL)
        {
            pstnSentData();
        }
    } 
    else
    {
        DataReceivedFlags |= (1<<3); // setting data received flag
        if (pstnReceivedData != NULL)
        {
            pstnReceivedData(pu8Buffer,u32DataSize);
            UsbDevice_ReceiveData(pstcUsb, pstcEndpoint, pstcEndpoint->pstcEndpointBuffer->pu8Buffer, pstcEndpoint->pstcEndpointBuffer->u32BufferSize, UsbIRQ);
            return;
        }
            
        if ((bEchoMode) && (bVcommActive) && (bEchoAtSeperatorOnly == FALSE))
        {
            UsbDevice_SendData(pstcUsbHandle, pstcEndpointIN, pu8Buffer, u32DataSize, UsbPOLL);
        }
        for(u32i=0;u32i<u32DataSize;u32i++)
        {
            if ((cSeperator != '\0') && (*pu8Buffer == cSeperator)) {
                bSeperatorFlag = TRUE;
            }
            if (u32ReceivedData < USBDEVICECDCCOM_MAX_RECEIVE_BUFFER) 
            {
                pu8VcommBuffer[u32ReceivedData++] = *pu8Buffer++; 
            }
            if ((cSeperator != '\0') && (bEchoMode) && (bVcommActive) && (bEchoAtSeperatorOnly) && (bSeperatorFlag))
            {
                UsbDevice_SendData(pstcUsbHandle, pstcEndpointIN, (uint8_t *)pu8VcommBuffer, u32ReceivedData, UsbPOLL);
            }
        }
        UsbDevice_ReceiveData(pstcUsb, pstcEndpoint, pstcEndpoint->pstcEndpointBuffer->pu8Buffer, pstcEndpoint->pstcEndpointBuffer->u32BufferSize, UsbIRQ);
    }
}


/**
 ******************************************************************************
 ** \brief Initialisation callback
 **
 ** \param pstcUsb         Pointer to USB instance
 **    
 **
 ******************************************************************************/
void UsbDeviceCdcCom_Init(stc_usbn_t* pstcUsb)
{
    stc_usbdevice_class_config_t stcUsbClassConfig;
    stc_usbdevice_endpoint_config_t stcEndpointConfig;
    uint8_t* pu8Interface = NULL;
    uint8_t u8InterfaceLength = 0;
    uint8_t u8i = 0;
    uint8_t u8NumEndpoints = 0;
    pstcUsbHandle = pstcUsb;
    
    stcUsbClassConfig.u8InterfaceNumber          = 0xFF;                          // do not use fix interface number, choose by class, subclass, protocol
    stcUsbClassConfig.u8InterfaceClass           = 0x02;                          // CDC Class
    stcUsbClassConfig.u8InterfaceSubClass        = 0x02;                          // Custom Sub Class
    stcUsbClassConfig.u8InterfaceProtocoll       = 0x01;                          // Custom Protocol
    stcUsbClassConfig.pfnSetupRequestCallback    = DecodeSetupRequest;            // setup requests handled
    stcUsbClassConfig.pfnConnectionCallback      = NULL;                          // No connection callback handled
    stcUsbClassConfig.pfnDisconnectionCallback   = NULL;                          // No disconnection callback handled
    stcUsbClassConfig.pfnConfCallback            = ConfCallback;                  // Callback for configuration set
    UsbDevice_RegisterVendorClass(pstcUsb,&stcUsbClassConfig,&pu8Interface,&u8InterfaceLength);

    u8NumEndpoints = 0;
    u8ControlInterface = pu8Interface[2];
    for(u8i = 0;u8i < u8InterfaceLength;)
    {
        u8i += pu8Interface[u8i];
        if (USBDESCR_ENDPOINT == pu8Interface[u8i + 1])
        {
            stcEndpointConfig.u8EndpointAddress = pu8Interface[u8i + 2];
            stcEndpointConfig.pfnRxTxCallback = NULL;
            if (((stcEndpointConfig.u8EndpointAddress) & 0x80) != 0)
            {
                stcEndpointConfig.pstcEndpointBuffer = &stcEndpointBufferControlIN;
                pstcEndpointControlIN = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);
                pstcEndpointControlIN->bAutomaticNullTermination = TRUE;
            }
            u8NumEndpoints++;
        }
    }
    
    stcUsbClassConfig.u8InterfaceNumber          = 0xFF;                          // do not use fix interface number, choose by class, subclass, protocol
    stcUsbClassConfig.u8InterfaceClass           = 0x0A;                          // CDC Class
    stcUsbClassConfig.u8InterfaceSubClass        = 0x00;                          // Custom Sub Class
    stcUsbClassConfig.u8InterfaceProtocoll       = 0x00;                          // Custom Protocol
    stcUsbClassConfig.pfnSetupRequestCallback    = DecodeSetupRequest;            // setup requests handled
    stcUsbClassConfig.pfnConnectionCallback      = NULL;                          // No connection callback handled
    stcUsbClassConfig.pfnDisconnectionCallback   = NULL;                          // No disconnection callback handled
    stcUsbClassConfig.pfnConfCallback            = ConfCallback;                  // Callback for configuration set
    UsbDevice_RegisterVendorClass(pstcUsb,&stcUsbClassConfig,&pu8Interface,&u8InterfaceLength);
    
    u8NumEndpoints = 0;        
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

                UsbDevice_ReceiveData(pstcUsb, pstcEndpointOUT, pu8IngoingBuffer, USBDEVICECDCCOM_EPOUT_BUFFERSIZE, UsbIRQ);
            }
            else
            {
                stcEndpointConfig.pstcEndpointBuffer = &stcEndpointBufferIN;
                pstcEndpointIN = UsbDevice_SetupEndpoint(pstcUsb, &stcEndpointConfig);
            }
            u8NumEndpoints++;
        }
    }
}
    
/**
 ******************************************************************************
 ** Set DTR changed callback
 **
 ** \param pstnCallback callback handle
 **
 *****************************************************************************/
void UsbDeviceCdcCom_SetDtrChangedCallback(cdc_dtr_changed_t pstnCallback)
{
    pstnDtrChanged = pstnCallback;
}

/**
 ******************************************************************************
 ** Set RTS changed callback
 **
 ** \param pstnCallback callback handle
 **
 *****************************************************************************/
void UsbDeviceCdcCom_SetRtsChangedCallback(cdc_rts_changed_t pstnCallback)
{
    pstnRtsChanged = pstnCallback;
}

/**
 ******************************************************************************
 ** Set received callback
 **
 ** \param pstnCallback callback handle
 **
 *****************************************************************************/
void UsbDeviceCdcCom_SetReceivedCallback(cdc_data_received_t pstnCallback)
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
void UsbDeviceCdcCom_SetSentCallback(cdc_data_sent_t pstnCallback)
{
    pstnSentData = pstnCallback;
}

/**
 ******************************************************************************
 ** Is called from main application to send data via endpoint 1
 **
 ** \param pu8Buffer Buffer to send
 ** \param u32DataSize Buffersize
 ** \param enMode Sending mode: UsbPOLL, UsbIRQ or UsbDMA;
 **
 ** \return TRUE: if succesful, FALSE: if usb was not ready
 *****************************************************************************/
static boolean_t SendData(uint8_t* pu8Buffer, uint32_t u32DataSize, en_usbsend_mode_t enMode)
{
    volatile uint32_t u32Timeout = 100000000;
    if (UsbDevice_GetStatus(pstcUsbHandle) == UsbConfigured)
    {
        while(!(DataSentFlags & (1<<1)) && u32Timeout > 0)
        {
            Usb_WaitHook();
            u32Timeout--;
        }
        DataSentFlags &= ~(1<<1);
		
	while(UsbDevice_SendData(pstcUsbHandle, pstcEndpointIN, pu8Buffer, u32DataSize, enMode) != Ok)
        {
            Usb_WaitHook();
        }
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
 ** Check if data was sent
 **
 ** \return TRUE if data was sent
 *****************************************************************************/
boolean_t UsbDeviceCdcCom_DataSent(void)
{
    if ((DataSentFlags & (1<<1)) > 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**
 ******************************************************************************
 ** Set callback function for LineCoding changed event
 **
 ** \param pfnLineCodingChangeCallback  Callback handler
 *****************************************************************************/
void UsbDeviceCdcCom_SetCallbackLineCodingChange(usbdevicecdccom_linecodingchange_func_ptr_t pfnLineCodingChangeCallback)
{
    pfnLineCodingChange = pfnLineCodingChangeCallback;
}

/**
 ******************************************************************************
 ** Decode setup request (class specific) handler
 **
 ** \param pstcUsb USB handle
 **
 ** \param pstcSetup pointer of setup request
 *****************************************************************************/
static void DecodeSetupRequest(stc_usbn_t* pstcUsb, stc_usb_request_t* pstcSetup)
{
    switch (pstcSetup->bRequest)
    {
        case SET_LINE_CODING:
            UsbDevice_ReceiveDataControl(pstcUsb,SetLineCoding);
            UsbDevice_SendDataControl(pstcUsbHandle,(uint8_t *)NULL, 0);
            return;
        case GET_LINE_CODING:
            SendLineCoding();
            return;
        case SET_CONTROL_LINE_STATE:
            if (pstnRtsChanged != NULL)
            {
                pstnRtsChanged((pstcSetup->wValue & 0x02) != 0);
            }
            
            if (pstnDtrChanged != NULL)
            {
                pstnDtrChanged((pstcSetup->wValue & 0x01) != 0);
            }

            #if USE_DTR_FOR_CONNECT == 1
            if (pstcSetup->wValue & 0x01)
            {
                bVcommActive = TRUE;
            } 
            else 
            {
                bVcommActive = FALSE;
            }  
            #endif
            if (pstcSetup->wValue & 0x02) 
            {

            } 
            else
            {

            }
            UsbDevice_SendDataControl(pstcUsbHandle,(uint8_t *)NULL, 0);
            return;
       default:
            UsbDevice_SendDataControl(pstcUsbHandle,(uint8_t *)NULL, 0);
            return;
    }   
}

/**
 ******************************************************************************
 ** Update serial state
 **
 ** \param pstcState Set specified bits
 **
 *****************************************************************************/
void UsbDeviceCdcCom_SerialStateSet(stc_cdc_uart_state_t* pstcState)
{
    uint8_t au8Data[10];
    stcSerialState.u16UartState |= pstcState->u16UartState;
    au8Data[0] = 0xA1;
    au8Data[1] = SERIAL_STATE;
    au8Data[2] = 0;
    au8Data[3] = 0;
    au8Data[4] = u8ControlInterface;
    au8Data[5] = 0;
    au8Data[6] = 2;
    au8Data[7] = 0;
    au8Data[8] = (uint8_t)(stcSerialState.u16UartState & 0xFF);
    au8Data[9] = (uint8_t)(stcSerialState.u16UartState >> 8);
    while(UsbDevice_SendData(pstcUsbHandle, pstcEndpointControlIN, &au8Data[0], 10, UsbPOLL) != Ok)
    {
        Usb_WaitHook();
    }
}

/**
 ******************************************************************************
 ** Update serial state
 **
 ** \param pstcState Clear specified bits
 **
 *****************************************************************************/
void UsbDeviceCdcCom_SerialStateClear(stc_cdc_uart_state_t* pstcState)
{
    uint8_t au8Data[10];
    stcSerialState.u16UartState &= ~(pstcState->u16UartState);
    au8Data[0] = 0xA1;
    au8Data[1] = SERIAL_STATE;
    au8Data[2] = 0;
    au8Data[3] = 0;
    au8Data[4] = u8ControlInterface;
    au8Data[5] = 0;
    au8Data[6] = 2;
    au8Data[7] = 0;
    au8Data[8] = (uint8_t)(stcSerialState.u16UartState & 0xFF);
    au8Data[9] = (uint8_t)(stcSerialState.u16UartState >> 8);
    while(UsbDevice_SendData(pstcUsbHandle, pstcEndpointControlIN, &au8Data[0], 10, UsbPOLL) != Ok)
    {
        Usb_WaitHook();
    }
}

/**
 ******************************************************************************
 ** Update serial state
 **
 ** \param pstcState Set this state
 **
 *****************************************************************************/
void UsbDeviceCdcCom_SerialStateUpdate(stc_cdc_uart_state_t* pstcState)
{
    uint8_t au8Data[10];
    stcSerialState.u16UartState = pstcState->u16UartState;
    au8Data[0] = 0xA1;
    au8Data[1] = SERIAL_STATE;
    au8Data[2] = 0;
    au8Data[3] = 0;
    au8Data[4] = u8ControlInterface;
    au8Data[5] = 0;
    au8Data[6] = 2;
    au8Data[7] = 0;
    au8Data[8] = (uint8_t)(stcSerialState.u16UartState & 0xFF);
    au8Data[9] = (uint8_t)(stcSerialState.u16UartState >> 8);
    while(UsbDevice_SendData(pstcUsbHandle, pstcEndpointControlIN, &au8Data[0], 10, UsbPOLL) != Ok)
    {
        Usb_WaitHook();
    }
}

/**
 ******************************************************************************
 ** Get serial state
 **
 ** \param pstcState returned state
 **
 *****************************************************************************/
void UsbDeviceCdcCom_SerialStateGet(stc_cdc_uart_state_t* pstcState)
{
    pstcState->u16UartState = stcSerialState.u16UartState;
}
/**
 ******************************************************************************
 ** callback for data package of setup request "SET_LINE_CODING"
 **
 ** \param pstcUsb USB handle
 **
 ** \param pstcEndpoint endpoint handle of received data
 **
 *****************************************************************************/
static void SetLineCoding(stc_usbn_t* pstcUsb, stc_usbn_endpoint_data_t* pstcEndpoint)
{
   static uint8_t pu8Buffer[8];
   static volatile uint32_t u32Rate = 0;
   memcpy(pu8Buffer,pstcEndpoint->pstcEndpointBuffer->pu8Buffer,8);
   u32Rate = 0;
   u32Rate += ((uint32_t)pu8Buffer[3]) << 24;
   u32Rate += ((uint32_t)pu8Buffer[2]) << 16;
   u32Rate += ((uint32_t)pu8Buffer[1]) <<  8;
   u32Rate += ((uint32_t)pu8Buffer[0]) <<  0;
   /*switch(u32Rate)
   {
       case 110:
         break;
       case 300:
         break;
       case 1200:
         break;    
       case 2400:
         break;
       case 4800:
         break;  
       case 9600:
         break;
       case 19200:
         break;
       case 38400:
         break;
       case 57600:
         break;
       case 115200:
         break;
       case 230400:
         break;
       case 460800:
         break;
       case 921600:
         break;
       default:
         u32Rate = u32Rate;
         return;
   }*/
   stcLineCoding.dwDTERate = u32Rate;
   stcLineCoding.bCharFormat = pu8Buffer[4];
   stcLineCoding.bParityType = pu8Buffer[5];
   stcLineCoding.bDataBits   = pu8Buffer[6];
   if (pfnLineCodingChange != NULL)
   {
       pfnLineCodingChange(&stcLineCoding);
   }
   
}
/**
 ******************************************************************************
 ** answer for request "GET_LINE_CODING"
 **
 *****************************************************************************/
static void SendLineCoding(void) 
{
    pu8LineCoding[3] = (uint8_t)((stcLineCoding.dwDTERate >> 24) & 0xFF);
    pu8LineCoding[2] = (uint8_t)((stcLineCoding.dwDTERate >> 16) & 0xFF);
    pu8LineCoding[1] = (uint8_t)((stcLineCoding.dwDTERate >>  8) & 0xFF);
    pu8LineCoding[0] = (uint8_t)((stcLineCoding.dwDTERate >>  0) & 0xFF);
    pu8LineCoding[4] = stcLineCoding.bCharFormat;
    pu8LineCoding[5] = stcLineCoding.bParityType;
    pu8LineCoding[6] = stcLineCoding.bDataBits;
    UsbDevice_SendDataControl(pstcUsbHandle,(uint8_t *)pu8LineCoding, 8);
    
}



/**
 ******************************************************************************
 ** getting vcomm connection status
 **
 ** \return TRUE: active / FALSE: inactive
 *****************************************************************************/
boolean_t UsbDeviceCdcCom_IsConnected(void) 
{
    return (bVcommActive && bReady);
}


/**
 ******************************************************************************
 ** sending byte / char_t
 **
 ** \param c byte / char_t to send
 **
 ** \return 1: error vcomm inactive / 0: successful sent
 *****************************************************************************/

uint8_t UsbDeviceCdcCom_SendByte(char_t c) 
{
    if (bVcommActive) 
    {
        SendData((uint8_t*)&c,1,UsbIRQ);
        return 0;
    } 
    return 1;
}


/**
 ******************************************************************************
 ** sending a null terminated string
 **
 ** \param pcBuffer string to send
 **
 ** \return 1: error vcomm inactive / 0: successful sent
 *****************************************************************************/

uint8_t UsbDeviceCdcCom_SendString(char_t *pcBuffer) 
{
    volatile uint32_t u32Size;
    if (bVcommActive) 
    {
        for(u32Size=0;(pcBuffer[u32Size] != '\0');u32Size++); //calculating size
        
        SendData((uint8_t *)pcBuffer,u32Size,UsbIRQ);
        return 0;
    } 
    return 1;
}


/**
 ******************************************************************************
 ** sending a buffer
 **
 ** \param pu8Buffer buffer to send
 ** \param u32Size   buffersize
 **
 ** \return 1: error vcomm inactive / 0: successful sent
 *****************************************************************************/

uint8_t UsbDeviceCdcCom_SendBuffer(uint8_t *pu8Buffer, uint32_t u32Size) 
{
    if (bVcommActive) 
    {
        #if USB_USES_DMA == ON
            SendData((uint8_t *)pu8Buffer,u32Size,UsbDMA);
        #else
            SendData((uint8_t *)pu8Buffer,u32Size,UsbIRQ);
        #endif    
        return 0;
    } 
    return 1;
}


/**
 ******************************************************************************
 ** splits the buffer at a defined character
 **
 ** \param cNewSeperator split buffer every newSeparator
 **        if zerro, ignore splitting (default) 
 **
 ** \return 1: error vcomm inactive / 0: successful sent
 *****************************************************************************/

void UsbDeviceCdcCom_SetSeparator(const char_t cNewSeperator) 
{
    cSeperator = cNewSeperator;
}


/**
 ******************************************************************************
 ** echoes every received char
 **
 ** \param bEchoOnOff switch echo on/off
 **
 *****************************************************************************/

void UsbDeviceCdcCom_SetEchomode(boolean_t bEchoOnOff) 
{
    bEchoMode = bEchoOnOff;
}

/**
 ******************************************************************************
 ** echoes every received char after receiving defined seperator
 **
 ** \param bEchoAtSeperatorOnlyOnOff switch echo on/off
 **
 *****************************************************************************/

void UsbDeviceCdcCom_SetEchoAtSeperatorOnly(boolean_t bEchoAtSeperatorOnlyOnOff) 
{
    bEchoAtSeperatorOnly = bEchoAtSeperatorOnlyOnOff;
}

/**
 ******************************************************************************
 ** returns number of received bytes in buffer
 **
 **
 ** \return number of received bytes in buffer
 *****************************************************************************/

uint32_t UsbDeviceCdcCom_ReceivedLength(void) 
{
    if ((cSeperator == '\0') || (bSeperatorFlag == TRUE))
    {
        return u32ReceivedData;
    }
    return 0;   
}



/**
 ******************************************************************************
 ** transfer receive buffer and clear it
 **
 ** \param pu8Buffer buffer to write in 
 **
 ** \return buffer size
 *****************************************************************************/
 
uint32_t UsbDeviceCdcCom_ReceiveBuffer(uint8_t *pu8Buffer, uint32_t numBytes) /* [andreika]: add numBytes */
{
    uint32_t u32ReturnValue;
    if (u32ReceivedData < numBytes)
    	numBytes = u32ReceivedData;
    for(u32ReturnValue=0;u32ReturnValue<numBytes;u32ReturnValue++) 
    {
        pu8Buffer[u32ReturnValue] = pu8VcommBuffer[u32ReturnValue];
    }
    u32ReceivedData -= numBytes;
    if (u32ReceivedData <= 0)
	    bSeperatorFlag = FALSE;
    return u32ReturnValue;
}


/**
 ******************************************************************************
 ** transfer one byte from the buffer 
 **
 **
 ** \return received byte
 *****************************************************************************/
 
uint8_t UsbDeviceCdcCom_ReceiveByte(void) 
{
    uint32_t i;
    uint8_t c;
    while (u32ReceivedData == 0);
    c = pu8VcommBuffer[0];
    /* shift buffer by 1 byte */
    for(i=0;i<(u32ReceivedData - 1);i++) 
    {    
        pu8VcommBuffer[i] = pu8VcommBuffer[i + 1];    
    }
    u32ReceivedData--;
    return c;
}


/**
 ******************************************************************************
 ** transfer last received byte without touching the buffer 
 **
 **
 ** \return received byte
 *****************************************************************************/
 
uint8_t UsbDeviceCdcCom_ReceiveLastByte(void) 
{
    if (u32ReceivedData > 0)
    {
        return (char_t)pu8VcommBuffer[u32ReceivedData - 1];
    }
    return 0;
}


/**
 ******************************************************************************
 ** sends a x-digit Hex-number (as ASCII charcaters) 
 **
 ** \param n value
 ** \param digits number of hex-digits 
 **
 ** \return 1: error vcomm inactive / 0: successful sent 
 *****************************************************************************/
 
uint8_t UsbDeviceCdcCom_SendHex(uint32_t n, uint8_t digits)
{
    uint8_t i,ch,div=0;

    if (UsbDeviceCdcCom_SendString("0x")) return 1;                /* hex string */
    div=(digits-1) << 2;    /* init shift divisor */

    for (i=0; i<digits; i++) {
      ch = (n >> div) & 0xF;/* get hex-digit value */
      if (UsbDeviceCdcCom_SendByte(pcASCII[ch])) return 1;        /* prompt to terminal as ASCII */
      div-=4;                /* next digit shift */
   }
   return 0;
}


/**
 ******************************************************************************
 ** sends a decimal-digit 
 **
 ** \param x value
 **
 ** \return 1: error vcomm inactive / 0: successful sent 
 *****************************************************************************/
 
uint8_t UsbDeviceCdcCom_SendDec(uint32_t x)
{
    int16_t i;
    char_t buf[9];
    if (x == 0) 
    {
      if (UsbDeviceCdcCom_SendString("       0")) return 1;
      return 0;
    }
    buf[8]='\0';                /* end sign of string */
    
    for (i=8; i>0; i--) 
    {
       buf[i-1] = pcASCII[x % 10];
       x = x/10;

    }

    for (i=0; buf[i]=='0'; i++) // no print16_t of zero 
    {
       buf[i] = ' ';
    }
    if (UsbDeviceCdcCom_SendString(buf)) return 1;                    /* send string */
    return 0;
}


/**
 ******************************************************************************
 ** 4-bit bin to ascii function
 **
 ** \param a value
 ** \param base type: HEX / DEC
 ** \param fill fill char
 **
 *****************************************************************************/
 
void UsbDeviceCdcCom_SendBinAsci4(uint16_t a, uint8_t base, char_t fill)                
{                            
  uint8_t val;
  a &= 0x0F;    

  switch(base)
    {    
        case HEX:    if (a <= 9) UsbDeviceCdcCom_SendByte(a + '0');                        
                  else        UsbDeviceCdcCom_SendByte(a + 'A' - 10);
                break;

        case DEC:    val = a / 10;
                if( val ) UsbDeviceCdcCom_SendByte( a / 10 + '0');
                    else  UsbDeviceCdcCom_SendByte( fill );
                UsbDeviceCdcCom_SendByte( a % 10 + '0');
                break;

        default:    UsbDeviceCdcCom_SendString(" <format not supported> ");
    }
}


/**
 ******************************************************************************
 ** 8-bit bin to ascii function
 **
 ** \param a value
 ** \param base type: HEX / DEC
 ** \param fill fill char
 **
 *****************************************************************************/
 
void UsbDeviceCdcCom_SendBinAsci8(uint16_t a, uint8_t base, char_t fill)
{
  uint16_t rem,val,d;
  uint8_t filling=1;
                        
    switch(base)
    {    
        case HEX:    UsbDeviceCdcCom_SendBinAsci4(a >> 4, HEX, fill);                         
                  UsbDeviceCdcCom_SendBinAsci4(a, HEX, fill);                        
                break;

        case DEC:    rem = a;
                for( d=100; d >= 10; d = d / 10)
                { 
                  val = rem / d; rem = rem % d;
                  if( val ) filling=0; 
                  if( filling ) UsbDeviceCdcCom_SendByte( fill );
                    else UsbDeviceCdcCom_SendByte( val + '0' );
                }
                UsbDeviceCdcCom_SendByte( rem + '0');
                break;

        default:    UsbDeviceCdcCom_SendString(" <format not supported> ");
    }

}


/**
 ******************************************************************************
 ** bin to ascii function
 **
 ** \param a value
 **
 *****************************************************************************/
 
void UsbDeviceCdcCom_SendBin2DotDec99(uint8_t a)
{
  
        uint8_t rem, val;
        UsbDeviceCdcCom_SendByte('.');

        rem = a;

        val = rem / 10; rem = rem % 10;
        UsbDeviceCdcCom_SendByte( val + '0' );
        
        UsbDeviceCdcCom_SendByte( rem + '0');


}


/**
 ******************************************************************************
 ** receive hex value
 **
 ** \param digits number of digits
 **
 ** \return value 
 *****************************************************************************/
 
uint32_t UsbDeviceCdcCom_ReceiveHex(uint8_t digits)
{
   uint32_t number=0;
   signed digit=0;
   uint8_t abort=0,mlt=0,i,key;
   UsbDeviceCdcCom_SetSeparator('\0');
   UsbDeviceCdcCom_SetEchomode(TRUE);
   mlt=(4*(digits-1));  /* 20 for 6 hex-digit numers, 4 for 2 hex-digits */
   for (i=0;i<digits;i++)
   {
     digit = -1;     
     while ((digit==-1) & (!abort))     /* input next valid digit */
     {
       key = UsbDeviceCdcCom_ReceiveByte();             /* wait for next key input (ASCII) */
       if (key == 27) abort=1;    /* ESC aborts */
       digit = ASCIItobin(key);   /* convert to number */ 
       if (digit == -1) UsbDeviceCdcCom_SendByte(8); /* backspace if not valid */
     }
     number+= (digit << mlt);     /* add digit value to number */
     mlt-=4;                      /* next digit shift */
   }
   UsbDeviceCdcCom_SetEchomode(FALSE);
   if (!abort) 
      return number;             /* return input value */
   else
   {
      UsbDeviceCdcCom_SendString("\n\n input cancled \n");
      return 0;                /* return abort indicator */
   }
}

/* Internal */
static uint32_t ASCIItobin(uint8_t k)
{
  char_t d=(char_t) -1;
  if ((k > 47) & (k < 58)) d = k - 48;  /* 0..9 */
  if ((k > 64) & (k < 71)) d = k - 55;  /* A..F */
  if ((k > 96) & (k < 103)) d = k - 87; /* a..f */
  return d;
}

#if (USBDEVICECDCCOM_USE_PRINTF == 1)

/**
 ******************************************************************************
 ** Low-Level function for printf()
 **
 ** \param fileno Filenumber
 **
 ** \param buf    Buffer
 **
 ** \param size   Size
 **
 *****************************************************************************/
int __write(int fileno, char *buf, unsigned int size)
{
    unsigned int cnt = size;
    switch(fileno)
    {
        case 0 :  
            //return(0);                            /* stdin */
        case 1 :  
            UsbDeviceCdcCom_SendBuffer((uint8_t*)buf,size);   
            return(cnt);                          /* successful output */
        case 2 :  
            return(-1);                           /* stderr */
        default:  
            return(-1);                           /* should never happend */
    }
}

/**
 ******************************************************************************
 ** \brief        low level read function, read characters via UART1
 *                carrige return (ASCII:13) is translated into line feed
 *                and carrige return (ASCII: 10 and 13)
 ** \param fileno Filenumber
 **
 ** \param buf    Buffer
 **
 ** \param size   Size
 **
 ** \return       successfull: number of read characters
 **               error: -1
 **
 *****************************************************************************/
int __read( int fileno, char *buf, unsigned int size) {

	unsigned int cnt;
	unsigned char helpchar;
    switch(fileno)
    {
        case 0 :  
            for(cnt = 0;size > cnt;cnt++)         /* stdin */
            {
                
                helpchar = UsbDeviceCdcCom_ReceiveByte();
                if (helpchar == 13) 
                {
                    *buf=10;
		       		return(cnt+1);			/* successful input */
                }
                *buf=helpchar;
                buf++;
            }
            return(cnt);                          /* successful output */
        case 1 :  
            return(0);                            /* stdout */
        case 2 :  
            return(-1);                           /* stderr */
        default:  
            return(-1);                           /* should never happend */
    }
}

/**
 ******************************************************************************
 ** \brief        low-Level function to close specific file
 **
 ** \param fileno Filenumber
 **
 ** \return       successful or error (-1)
 **
 *****************************************************************************/
int __close(int fileno)
{
    if((fileno >= 0) && (fileno <= 2))
    {
        return(0);
    }
    else
    {
        return(-1);
    }
}

extern  char   *sbrk(int size)
{
   if (brk_siz + size > _heap_size || brk_siz + size < 0)
        return((char*)-1);
   brk_siz += size;
   return( (char *)_heap + brk_siz - size);
}


#endif /* (USBDEVICECDCCOM_USE_PRINTF == 1) */

#endif




