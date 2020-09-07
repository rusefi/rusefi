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
/** \file usbdevice.c
 **
 **
 ** A detailed description is available at 
 ** @link UsbDeviceGroup USB Device Module description @endlink
 **
 ** History:
 **   - 2012-08-28  2.0  MSc  First version (starting at version 2.0).
 **   - 2012-10-02  2.1  MSc  use of external interrupts without L3 implemented
 **   - 2012-11-13  2.2  MSc  Some problems while switching from Host to Device fixed
 **   - 2013-01-31  2.3  MSc  DMA added
 **   - 2013-05-07  2.4  MSc  Bug with sending wrong package sizes solved
 **   - 2013-05-21  2.5  MSc  Some Windows versions request only 4 bytes of string descriptor
 **                           ->fixed: more than only 4 bytes will be sent
 **   - 2013-06-04  2.6  MSc  FM4 support added
 **   - 2013-10-17  2.7  MSc  Device Configuration change: support for more than one report descriptor
 **   - 2014-02-28  2.8  MSc  Busy wait added to send and receive initiate procedure
 **   - 2015-05-13  2.9  MSCH Better timeout handling added
 **   - 2015-05-29  3.0  MSCH bit access for EPnS register changed for more stable access
 **                           added better data overflow handling
 **   - 2015-07-21  3.1  MSCH check DRQ bit before continue data transfers for safety reasons
 **
 ******************************************************************************/


/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/


#include "usbdevice.h"

#if (FM_PERIPHERAL_USB_DEVICE_ENABLED == ON)



/**
 ******************************************************************************
 ** \ingroup UsbDeviceGroup
 ******************************************************************************/
//@{

#if (USE_USBDEVICEHW_H == 1)
#include "usbdevicehw.h"
#endif


static stc_usbdevice_intern_data_t* UsbDeviceGetInternDataPtr(const stc_usbn_t* pstcUsb) ;
static void EndpointContinueTxCallback(stc_usbn_t* pstcUsb, stc_usbn_endpoint_data_t* pstcEndpoint);
static void EndpointContinueRxCallback(stc_usbn_t* pstcUsb, stc_usbn_endpoint_data_t* pstcEndpoint);
static void ControlRxCallback(stc_usbn_t* pstcUsb, stc_usbn_endpoint_data_t* pstcEndpoint);
static void ControlTxCallback(stc_usbn_t* pstcUsb, stc_usbn_endpoint_data_t* pstcEndpoint);
static void ConfigChangeCallback(stc_usbn_t* pstcUsb);
static void SetupRxCallback(stc_usbn_t* pstcUsb);



/// Macro to return the number of enabled MFS instances
#define USBDEVICE_INSTANCE_COUNT (uint32_t)(sizeof(m_astcUsbDeviceInstanceDataLut) / sizeof(m_astcUsbDeviceInstanceDataLut[0]))

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/// Look-up table for all enabled MFS instances and their internal data
stc_usbdevicen_instance_data_t m_astcUsbDeviceInstanceDataLut[] =
{
    #if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && (defined(USB0)) && (FM_PERIPHERAL_ENABLE_USB0_DEVICE == ON))
    { &USB0,  // pstcInstance
      // [andreika]: gcc fix
      {0}    // stcInternData (not initialized yet)
    },
    #endif
    #if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && (defined(USB1)) && (FM_PERIPHERAL_ENABLE_USB1_DEVICE == ON))
    { &USB1,  // pstcInstance
      // [andreika]: gcc fix
      {0}    // stcInternData (not initialized yet)
    }
    #endif
};

/**
 ******************************************************************************
 ** \brief Return the internal data for a certain USB instance.
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \return Pointer to internal data or NULL if instance is not enabled (or not known)
 **
 ******************************************************************************/
static stc_usbdevice_intern_data_t* UsbDeviceGetInternDataPtr(const stc_usbn_t* pstcUsb) 
{
    volatile uint32_t u32Instance;
   
    for (u32Instance = 0; u32Instance < USBDEVICE_INSTANCE_COUNT; u32Instance++)
    {
        if ((uint32_t)pstcUsb == (uint32_t)(m_astcUsbDeviceInstanceDataLut[u32Instance].pstcInstance))
        {
            return &m_astcUsbDeviceInstanceDataLut[u32Instance].stcInternData;
        }
    }

    return NULL;
}

/**
 ******************************************************************************
 ** \brief Callback function, called after sending via endpoints (IN direction)
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pstcEndpoint Pointer to endpoint instance
 **
 ** \return none
 **
 ******************************************************************************/
static void EndpointContinueTxCallback(stc_usbn_t* pstcUsb, stc_usbn_endpoint_data_t* pstcEndpoint)
{
    uint16_t u16DataSend = 0;
    uint32_t u32TransferredData;
    volatile uint32_t u32Timeout;
    USBDBG(">TxClbk\n");
    
    u32TransferredData = (((uint32_t)pstcEndpoint->pstcEndpointBuffer->pu8BufferPos) - ((uint32_t)pstcEndpoint->pstcEndpointBuffer->pu8Buffer));
    USBDBGVAL16("Trsfrd: ",u32TransferredData);
    USBDBGVAL16("Sze: ",pstcEndpoint->pstcEndpointBuffer->u32DataSize);
    while(pstcEndpoint->bIsActive == TRUE)
    {
        pstcEndpoint->bIsActive = FALSE;
    }
    if (u32TransferredData >= ((uint32_t)pstcEndpoint->pstcEndpointBuffer->u32DataSize))
    {
      USBDBG("EOT\n");
      
      USB_EPNS_DRQIE_CLEAR(pstcEndpoint);
      
      /*if ((pstcEndpoint->u8EndpointAddress == 0x80) && (pstcEndpoint->pstcEndpointBuffer->u32DataSize % 64 == 0) && (pstcEndpoint->pstcEndpointBuffer->u32DataSize != 0))
      {
          USBDBG("NULL");
          Usb_HalSend(pstcEndpoint,pstcEndpoint->pstcEndpointBuffer->pu8Buffer,0,&u16DataSend);
      }*/
      if (pstcEndpoint->bAutomaticNullTermination)
      {
          if (((pstcEndpoint->pstcEndpointBuffer->u32DataSize % pstcEndpoint->u16EndpointSize) == 0) && (pstcEndpoint->pstcEndpointBuffer->u32DataSize != 0))
          {
              SystemCoreClockUpdate();
              u32Timeout = SystemCoreClock / 100;
              while((!USB_EPNS_DRQ_ISSET(pstcEndpoint)) && (u32Timeout > 0)) u32Timeout--;
              Usb_HalSend(pstcEndpoint,pstcEndpoint->pstcEndpointBuffer->pu8Buffer,0,&u16DataSend);
          }
      }
      if (pstcEndpoint->pfnRxTxBlockCallback != NULL)
      {
          pstcEndpoint->pfnRxTxBlockCallback(pstcUsb, pstcEndpoint);
      }
      return;
      //Data Transfer End
    }
    
    
    USB_EPNS_DRQIE_CLEAR(pstcEndpoint);
    SystemCoreClockUpdate();
    u32Timeout = SystemCoreClock / 100;
    while((!USB_EPNS_DRQ_ISSET(pstcEndpoint)) && (u32Timeout > 0)) u32Timeout--;
    Usb_HalSend(pstcEndpoint,pstcEndpoint->pstcEndpointBuffer->pu8BufferPos,pstcEndpoint->pstcEndpointBuffer->u32DataSize - u32TransferredData,&u16DataSend);
    pstcEndpoint->pstcEndpointBuffer->pu8BufferPos += u16DataSend;
    
    USB_EPNS_DRQIE_SET(pstcEndpoint);
    
}

/**
 ******************************************************************************
 ** \brief Callback function, called after receiving via endpoints (OUT direction)
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pstcEndpoint Pointer to endpoint instance
 **
 ** \return none
 **
 ******************************************************************************/
static void EndpointContinueRxCallback(stc_usbn_t* pstcUsb, stc_usbn_endpoint_data_t* pstcEndpoint)
{
    uint16_t u16DataReceived = 0;
    uint32_t u32DataLeft = 0;
    volatile uint32_t u32Timeout;     
    USB_EPNS_DRQIE_CLEAR(pstcEndpoint);

    u32DataLeft = pstcEndpoint->pstcEndpointBuffer->u32BufferSize - (((uint32_t)pstcEndpoint->pstcEndpointBuffer->pu8BufferPos) - ((uint32_t)pstcEndpoint->pstcEndpointBuffer->pu8Buffer));
    if (u32DataLeft < pstcEndpoint->u16EndpointSize)
    {
        //signalizing the low level it is less data available than EP size
        u16DataReceived = 0x8000;
        u16DataReceived |= (uint16_t)(u32DataLeft & 0x03FF);
    }
    
    SystemCoreClockUpdate();
    u32Timeout = SystemCoreClock / 100;
    while((!USB_EPNS_DRQ_ISSET(pstcEndpoint)) && (u32Timeout > 0)) u32Timeout--;
    Usb_HalReceive(pstcEndpoint,pstcEndpoint->pstcEndpointBuffer->pu8BufferPos,&u16DataReceived,FALSE);
    
    USB_EPNS_DRQ_CLEAR(pstcEndpoint);
    
    pstcEndpoint->pstcEndpointBuffer->u32DataSize += u16DataReceived;
    pstcEndpoint->pstcEndpointBuffer->pu8BufferPos += u16DataReceived;
    if (((((uint32_t)pstcEndpoint->pstcEndpointBuffer->pu8BufferPos) - ((uint32_t)pstcEndpoint->pstcEndpointBuffer->pu8Buffer)) >= ((uint32_t)pstcEndpoint->pstcEndpointBuffer->u32BufferSize)) || (u16DataReceived < pstcEndpoint->u16EndpointSize))
    {
      pstcEndpoint->pfnRxTxCallback = NULL;
      if (pstcEndpoint->pfnRxTxBlockCallback != NULL)
      {
          pstcEndpoint->pfnRxTxBlockCallback(pstcUsb, pstcEndpoint);
      }
      return;
      //Data Transfer End
    }
    
    USB_EPNS_DRQIE_SET(pstcEndpoint);
}


/**
 ******************************************************************************
 ** \brief Initiate receiving data (OUT direction)
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pstcEndpoint Pointer to endpoint instance
 **
 ** \param pu8Buffer Pointer to data buffer
 **
 ** \param u32DataSize Size of data to be received
 **
 ** \param enMode Mode how to send data (polled, irq driven or dma driven)
 **
 ** \return none
 **
 ******************************************************************************/
en_result_t UsbDevice_ReceiveData(stc_usbn_t* pstcUsb, stc_usbn_endpoint_data_t* pstcEndpoint, uint8_t* pu8Buffer, uint32_t u32DataSize, en_usbsend_mode_t enMode)
{
    uint16_t u16DataReceived = 0;
    uint32_t u32DataLeft = 0;
    volatile uint32_t u32Timeout;    
    if (pstcEndpoint->pstcEndpointBuffer == NULL)
    {
        return ErrorInvalidParameter;
    }
    pstcEndpoint->pstcEndpointBuffer->pu8Buffer = pu8Buffer;
    pstcEndpoint->pstcEndpointBuffer->pu8BufferPos = pu8Buffer;
    pstcEndpoint->pstcEndpointBuffer->u32BufferSize = u32DataSize;
    pstcEndpoint->pstcEndpointBuffer->u32DataSize = 0;
    
    //Usb_EpCheckWaitBusy(pstcEndpoint);
    
    switch(enMode)
    {
      case UsbPOLL:
        while((((uint32_t)pstcEndpoint->pstcEndpointBuffer->pu8BufferPos) - ((uint32_t)pstcEndpoint->pstcEndpointBuffer->pu8Buffer)) < ((uint32_t)pstcEndpoint->pstcEndpointBuffer->u32BufferSize))
        {
            //Usb_EpCheckWaitBusy(pstcEndpoint);
            
            u32DataLeft = pstcEndpoint->pstcEndpointBuffer->u32BufferSize - (((uint32_t)pstcEndpoint->pstcEndpointBuffer->pu8BufferPos) - ((uint32_t)pstcEndpoint->pstcEndpointBuffer->pu8Buffer));
            if (u32DataLeft < pstcEndpoint->u16EndpointSize)
            {
                //signalizing the low level it is less data available than EP size
                u16DataReceived = 0x8000;
                u16DataReceived |= (uint16_t)(u32DataLeft & 0x03FF);
            }
            SystemCoreClockUpdate();
            u32Timeout = SystemCoreClock / 100;
            while((!USB_EPNS_DRQ_ISSET(pstcEndpoint)) && (u32Timeout > 0)) u32Timeout--;
            Usb_HalReceive(pstcEndpoint,pstcEndpoint->pstcEndpointBuffer->pu8BufferPos,&u16DataReceived,TRUE);
            pstcEndpoint->pstcEndpointBuffer->u32DataSize += u16DataReceived;
            pstcEndpoint->pstcEndpointBuffer->pu8BufferPos += u16DataReceived;
            if (u16DataReceived < pstcEndpoint->u16EndpointSize)
            {
                return Ok;
            }
        }
        break;
      case UsbIRQ:
        pstcEndpoint->pfnRxTxCallback = EndpointContinueRxCallback;
        
        //Usb_EpCheckWaitBusy(pstcEndpoint);
        //BITMASK_SET(*(pstcEndpoint->pstcEpStatusRegister),_EPNS_DRQIE);
        USB_EPNS_DRQIE_SET(pstcEndpoint);

        break;
      case UsbDMA:
        #if USB_USES_DMA == ON
        return Error;
        #else
        return Error;
        #endif
    }
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Initiate sending data (IN direction)
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pstcEndpoint Pointer to endpoint instance
 **
 ** \param pu8Buffer Pointer to data buffer
 **
 ** \param u32DataSize Size of data to be send
 **
 ** \param enMode Mode how to send data (polled, irq driven or dma driven)
 **
 ** \return none
 **
 ******************************************************************************/
en_result_t UsbDevice_SendData(stc_usbn_t* pstcUsb, stc_usbn_endpoint_data_t* pstcEndpoint, uint8_t* pu8Buffer, uint32_t u32DataSize, en_usbsend_mode_t enMode)
{
    uint16_t u16DataSend = 0;
    volatile uint32_t u32Timeout;
    USBDBGVAL8("New ",(uint32_t)pstcEndpoint->u8EndpointAddress);
    USBDBGVAL8("Sze ",(uint32_t)u32DataSize);
    if (pstcEndpoint->pstcEndpointBuffer == NULL)
    {
        USBDBG("\nERROR: Buffer == NULL!");
        return ErrorInvalidParameter;
    }
    if (pstcEndpoint->bIsActive == TRUE)
    {
        return ErrorNotReady;
    }
   
    pstcEndpoint->pstcEndpointBuffer->pu8Buffer = pu8Buffer;
    pstcEndpoint->pstcEndpointBuffer->pu8BufferPos = pu8Buffer; 
    pstcEndpoint->pstcEndpointBuffer->u32BufferSize = u32DataSize;
    pstcEndpoint->pstcEndpointBuffer->u32DataSize = u32DataSize;
    
    //Usb_EpCheckWaitBusy(pstcEndpoint);
    
    switch(enMode)
    {
      case UsbPOLL:
        pstcEndpoint->bIsActive = TRUE;
        while((((uint32_t)pstcEndpoint->pstcEndpointBuffer->pu8BufferPos) - ((uint32_t)pstcEndpoint->pstcEndpointBuffer->pu8Buffer)) < ((uint32_t)pstcEndpoint->pstcEndpointBuffer->u32DataSize))
        {
            //Usb_EpCheckWaitBusy(pstcEndpoint);
            SystemCoreClockUpdate();
            u32Timeout = SystemCoreClock / 100;
            while((!USB_EPNS_DRQ_ISSET(pstcEndpoint)) && (u32Timeout > 0)) u32Timeout--;
            Usb_HalSend(pstcEndpoint,pstcEndpoint->pstcEndpointBuffer->pu8BufferPos,pstcEndpoint->pstcEndpointBuffer->u32DataSize - (uint32_t)(pstcEndpoint->pstcEndpointBuffer->pu8BufferPos - pstcEndpoint->pstcEndpointBuffer->pu8Buffer),&u16DataSend);
            pstcEndpoint->pstcEndpointBuffer->pu8BufferPos += u16DataSend;
        }
        if (pstcEndpoint->bAutomaticNullTermination)
        {
            if (((pstcEndpoint->pstcEndpointBuffer->u32DataSize % pstcEndpoint->u16EndpointSize) == 0) && (pstcEndpoint->pstcEndpointBuffer->u32DataSize != 0))
            {
                 SystemCoreClockUpdate();
                 u32Timeout = SystemCoreClock / 100;
                 while((!USB_EPNS_DRQ_ISSET(pstcEndpoint)) && (u32Timeout > 0)) u32Timeout--;
                 Usb_HalSend(pstcEndpoint,pstcEndpoint->pstcEndpointBuffer->pu8Buffer,0,&u16DataSend);
            }
        }
        while(pstcEndpoint->bIsActive == TRUE)
        {
            pstcEndpoint->bIsActive = FALSE;
        }
        break;
      case UsbIRQ:
        pstcEndpoint->bIsActive = TRUE;
        pstcEndpoint->pfnRxTxCallback = EndpointContinueTxCallback;
        SystemCoreClockUpdate();
        u32Timeout = SystemCoreClock / 100;
        while((!USB_EPNS_DRQ_ISSET(pstcEndpoint)) && (u32Timeout > 0)) u32Timeout--;
        Usb_HalSend(pstcEndpoint,pu8Buffer,u32DataSize,&u16DataSend);
        pstcEndpoint->pstcEndpointBuffer->pu8BufferPos += u16DataSend;
        
        //Usb_EpCheckWaitBusy(pstcEndpoint);
        //BITMASK_SET(*(pstcEndpoint->pstcEpStatusRegister),_EPNS_DRQIE);
        USB_EPNS_DRQIE_SET(pstcEndpoint);
        break;
      case UsbDMA:
        #if USB_USES_DMA == ON
        pstcEndpoint->bIsActive = TRUE;
        pstcEndpoint->pfnRxTxCallback = EndpointContinueTxCallback;
        SystemCoreClockUpdate();
        u32Timeout = SystemCoreClock / 100;
        while((!USB_EPNS_DRQ_ISSET(pstcEndpoint)) && (u32Timeout > 0)) u32Timeout--;
        Usb_HalSendDma(pstcEndpoint,pu8Buffer,u32DataSize,&u16DataSend);
        pstcEndpoint->pstcEndpointBuffer->pu8BufferPos += u16DataSend;
        
        //Usb_EpCheckWaitBusy(pstcEndpoint);
        //BITMASK_SET(*(pstcEndpoint->pstcEpStatusRegister),_EPNS_DRQIE);
        USB_EPNS_DRQIE_SET(pstcEndpoint);
        
        break;
        #else
        while(pstcEndpoint->bIsActive == TRUE)
        {
            pstcEndpoint->bIsActive = FALSE;
        }
        pstcEndpoint->bIsActive = FALSE;
        return Error;
        #endif
    default:
        return Error;
        
    }
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Callback function, called after receiving via control endpoint 0 (OUT direction)
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pstcEndpoint Pointer to endpoint instance
 **
 ** \return none
 **
 ******************************************************************************/
static void ControlRxCallback(stc_usbn_t* pstcUsb, stc_usbn_endpoint_data_t* pstcEndpoint)
{
    static uint8_t pu8Buffer[64];
    uint16_t u16BytesCount;
    volatile uint32_t u32Timeout;    
    stc_usbdevice_intern_data_t* pstcUsbDeviceIntern;
    pstcUsbDeviceIntern = UsbDeviceGetInternDataPtr(pstcUsb);
    pstcUsbDeviceIntern->pu8LastControlTransfer = NULL;
    
    SystemCoreClockUpdate();
    u32Timeout = SystemCoreClock / 100;
    while((!USB_EPNS_DRQ_ISSET(pstcEndpoint)) && (u32Timeout > 0)) u32Timeout--;
    Usb_HalReceive(pstcEndpoint,pu8Buffer,&u16BytesCount,FALSE);
    
    while(pstcEndpoint->bIsActive == TRUE)
    {
        pstcEndpoint->bIsActive = FALSE;
    }
    pstcEndpoint->pstcEndpointBuffer->pu8Buffer = pu8Buffer;
    pstcEndpoint->pstcEndpointBuffer->u32DataSize = u16BytesCount;
    while(Usb_HalClearDrq(pstcUsbDeviceIntern->pstcEp0OUT) != Ok);
    if (u16BytesCount == 0)
    {
        if (pstcUsbDeviceIntern->enControlStage == StatusStage)
        {
            pstcUsbDeviceIntern->enControlStage = ControlStageUnknown;
            //BITMASK_SET(*((uint16_t*)pstcEndpoint->pstcEpStatusRegister),_EPNS_BFINI);
            //BITMASK_CLEAR(*((uint16_t*)pstcEndpoint->pstcEpStatusRegister),_EPNS_BFINI);
        }
        //UsbDevice_SendData(pstcUsb, pstcUsbDeviceIntern->pstcEp0IN, pu8Buffer, 0, UsbIRQ);
    } 
    else
    {
        if (pstcUsbDeviceIntern->pfnControlTransferred != NULL)
        {
            pstcUsbDeviceIntern->pfnControlTransferred(pstcUsb, pstcEndpoint);
            pstcUsbDeviceIntern->pfnControlTransferred = NULL;
        } else
        {
            pstcUsbDeviceIntern->pu8LastControlTransfer = pu8Buffer;
        }
    }
    pstcEndpoint->pstcEndpointBuffer->pu8Buffer = NULL;
    pstcEndpoint->pstcEndpointBuffer->u32DataSize = 0;
}

/**
 ******************************************************************************
 ** \brief Callback function, called after sending via control endpoint 0 (IN direction)
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pstcEndpoint Pointer to endpoint instance
 **
 ** \return none
 **
 ******************************************************************************/
static void ControlTxCallback(stc_usbn_t* pstcUsb, stc_usbn_endpoint_data_t* pstcEndpoint)
{
    stc_usbdevice_intern_data_t* pstcUsbDeviceIntern;
    pstcUsbDeviceIntern = UsbDeviceGetInternDataPtr(pstcUsb);
    pstcUsbDeviceIntern->enControlStage = StatusStage;
    while(pstcEndpoint->bIsActive == TRUE)
    {
        pstcEndpoint->bIsActive = FALSE;
    }
}

/**
 ******************************************************************************
 ** \brief Callback function, called after device was configured
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \return none
 **
 ******************************************************************************/
static void ConfigChangeCallback(stc_usbn_t* pstcUsb)
{
    uint8_t u8i;
    stc_usbdevice_intern_data_t* pstcUsbDeviceIntern;
    pstcUsbDeviceIntern = UsbDeviceGetInternDataPtr(pstcUsb); // get internal i
    
    if (Usb_GetConfiguration(pstcUsb) == 0)            // Check configuration
    {
        pstcUsbDeviceIntern->enStatus = UsbConnected;  // If it is 0, set status connected
    }
    else
    {
        pstcUsbDeviceIntern->enStatus = UsbConfigured; // If it is not 0, set status configured
    }
    for(u8i = 0;u8i < USBDEVICE_MAXCLASSES;u8i++)
    {
        if (pstcUsbDeviceIntern->stcUsbClasses[u8i].pfnConfCallback != NULL)
        {
            pstcUsbDeviceIntern->stcUsbClasses[u8i].pfnConfCallback(pstcUsb);
        }
    }
}

/**
 ******************************************************************************
 ** \brief Callback function, called after setup was received
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \return none
 **
 ******************************************************************************/
static void SetupRxCallback(stc_usbn_t* pstcUsb)
{
    volatile uint32_t u32Timeout;    
    static uint8_t u8DataToSend[255];
    uint16_t u16BytesCount;
    uint16_t u16Length;
    static uint8_t pu8Buffer[8];
    uint8_t u8Size;
    static stc_usb_request_t stcSetup;
	  stc_usbdevice_intern_data_t* pstcUsbDeviceIntern;
	
    USB_ZERO_STRUCT(stcSetup);
    pstcUsbDeviceIntern = UsbDeviceGetInternDataPtr(pstcUsb);

    
    pstcUsb->EP0OS_f.DRQOIE  = 0;
    
    SystemCoreClockUpdate();
    u32Timeout = SystemCoreClock / 100;
    while((!USB_EPNS_DRQ_ISSET(pstcUsbDeviceIntern->pstcEp0OUT)) && (u32Timeout > 0)) u32Timeout--;
    Usb_HalReceive(pstcUsbDeviceIntern->pstcEp0OUT,pu8Buffer,&u16BytesCount,TRUE);
    
    
    pstcUsbDeviceIntern->enControlStage = SetupStage;
    pstcUsbDeviceIntern->pu8LastControlTransfer = NULL;
    
    /* Decode Setup Package */
    stcSetup.bmRequestType = pu8Buffer[0];
    stcSetup.bRequest = pu8Buffer[1];
    stcSetup.wValue = pu8Buffer[3] << 8;
    stcSetup.wValue += pu8Buffer[2];
    stcSetup.wIndex = pu8Buffer[5] << 8;
    stcSetup.wIndex += pu8Buffer[4];    
    stcSetup.wLength = pu8Buffer[7] << 8;
    stcSetup.wLength += pu8Buffer[6];  
    
    if (stcSetup.bmRequestType_f.DataPhaseTransferDirection == 1)
    {
        pstcUsbDeviceIntern->enControlStage = DataStageIN;
    }
    else
    {
        pstcUsbDeviceIntern->enControlStage = DataStageOUT;
    }
    
    //Usb_HalClearDrq(pstcUsbDeviceIntern->pstcEp0OUT);
    if ((pu8Buffer[0] & 0x60) == USB_DEVREQ_CLSTYPE)
    {
        if(pu8Buffer[1] == USB_DEVREQ_SET_IDLE)
        {
            //recive SET_IDLE
            BITMASK_CLEAR(pstcUsb->EP0IS,_EP0IS_DRQI);    // DRQi <- 0
            
            pstcUsbDeviceIntern->enControlStage = StatusStage;
            while(pstcUsbDeviceIntern->pstcEp0IN->bIsActive == TRUE)
            {
                pstcUsbDeviceIntern->pstcEp0IN->bIsActive = FALSE;
            }
    
            pstcUsb->EP0OS_f.DRQOIE  = 1;
            return;
        }
    }
    
    switch (stcSetup.bmRequestType_f.Type)
    {
    case 0: // Device
      switch(stcSetup.bRequest) 
      {
        case GET_DESCRIPTOR:
            switch((stcSetup.wValue) >> 8)
            {
                case 1:    // Device Descriptor
                  u16Length = stcSetup.wLength;
                  USBDBG("Req Dev Desc\n");
                  USBDBGVAL16("Len ",u16Length);
                  if (u16Length > pstcUsbDeviceIntern->pu8DeviceDescriptor[0]) u16Length = pstcUsbDeviceIntern->pu8DeviceDescriptor[0];
                  USBDBGVAL16("Len New ",u16Length);
                  pstcUsbDeviceIntern->pstcEp0IN->pfnRxTxBlockCallback = ControlTxCallback;
                  UsbDevice_SendData(pstcUsb, pstcUsbDeviceIntern->pstcEp0IN, pstcUsbDeviceIntern->pu8DeviceDescriptor, u16Length, UsbPOLL);
                  break;
                case 2:    // Config Descriptor
                  u16Length = stcSetup.wLength;
                  USBDBG("Req Conf Desc\n");
                  USBDBGVAL16("Len ",u16Length);
                  if (u16Length > (pstcUsbDeviceIntern->pu8ConfigDescriptor[2] + pstcUsbDeviceIntern->pu8ConfigDescriptor[3]*256)) u16Length = (pstcUsbDeviceIntern->pu8ConfigDescriptor[2] + pstcUsbDeviceIntern->pu8ConfigDescriptor[3]*256);
                  USBDBGVAL16("Len New ",u16Length);
                  pstcUsbDeviceIntern->pstcEp0IN->pfnRxTxBlockCallback = ControlTxCallback;
                  UsbDevice_SendData(pstcUsb, pstcUsbDeviceIntern->pstcEp0IN, pstcUsbDeviceIntern->pu8ConfigDescriptor, u16Length, UsbPOLL);
                  break;
                case 3:    // String Descriptor 
                  u16Length = stcSetup.wLength;
                  u8DataToSend[0] = 0;
                  switch ((stcSetup.wValue) & 0xFF)
                  {
                      case 0:
                          u8DataToSend[0] = 0x04;
                          u8DataToSend[1] = 0x03;
                          u8DataToSend[2] = 0x09;
                          u8DataToSend[3] = 0x04;
                          break;
                      default:
                          if (((stcSetup.wValue) & 0xFF) <= pstcUsbDeviceIntern->u8StringDescriptorCount)
                          {
                              if (pstcUsbDeviceIntern->pstcStringDescriptors[(stcSetup.wValue) & 0xFF].pu8String != NULL)
                              {
                                  u8DataToSend[0] = 2;
                                  u8DataToSend[1] = 0x03;
                                  for(u8Size=0;(( pstcUsbDeviceIntern->pstcStringDescriptors[((stcSetup.wValue) & 0xFF)-1].pu8String[u8Size] != 0) && (u8Size < sizeof(u8DataToSend) / 2 - 2));u8Size++)
                                  {
                                      u8DataToSend[2 + u8Size * 2] = pstcUsbDeviceIntern->pstcStringDescriptors[((stcSetup.wValue) & 0xFF)-1].pu8String[u8Size];
                                      u8DataToSend[3 + u8Size * 2] = 0;
                                  }
                                  u8DataToSend[0] = u8DataToSend[0] + 2 * u8Size;
                                  break;
                              } else
                              {
                                  /* tbd UNICODE */
                              }
                          }
                          break;
                          
                  }
                  if (u8DataToSend[0] > stcSetup.wLength)
                  {
                      u8DataToSend[0] = stcSetup.wLength;
                  }
                  UsbDevice_SendData(pstcUsb, pstcUsbDeviceIntern->pstcEp0IN, u8DataToSend, u8DataToSend[0], UsbPOLL);
                  break;
                case 0x22: // Report Descriptor
                  
                  u16Length = stcSetup.wLength;
                  USBDBG("Req Rep Desc\n");
                  USBDBGVAL16("Len ",u16Length);
                  if (u16Length > pstcUsbDeviceIntern->astcReportDescriptors[(stcSetup.wIndex & 0x03)].u16Size) u16Length = pstcUsbDeviceIntern->astcReportDescriptors[(stcSetup.wIndex & 0x03)].u16Size;
                  USBDBGVAL16("Len New ",u16Length);
                  pstcUsbDeviceIntern->pstcEp0IN->pfnRxTxBlockCallback = ControlTxCallback;
                  UsbDevice_SendData(pstcUsb, pstcUsbDeviceIntern->pstcEp0IN, pstcUsbDeviceIntern->astcReportDescriptors[(stcSetup.wIndex & 0x03)].pu8Descriptor, u16Length, UsbPOLL);
                  break;
                default:
                  USBDBGVAL8("USB Request Unknown Descriptor: \n",(stcSetup.wValue) >> 8);
                  //while(1);
                  break;
            }
            break;
      default:
        //while(1);
        USBDBGVAL8("USB Request Unknown: ",stcSetup.bRequest);
      }
      break;
    case 1: // Class
      if (stcSetup.wIndex < USBDEVICE_MAXCLASSES)
      {
          if (pstcUsbDeviceIntern->stcUsbClasses[stcSetup.wIndex].pfnSetupRequestCallback != NULL)
          {
              pstcUsbDeviceIntern->stcUsbClasses[stcSetup.wIndex].pfnSetupRequestCallback(pstcUsb, &stcSetup);
          }
          else
          {
              //while(1);
          }
      }
      break;
    case 2: // Vendor
      break;
    }
    pstcUsbDeviceIntern->enControlStage = StatusStage;
    while(pstcUsbDeviceIntern->pstcEp0IN->bIsActive == TRUE)
    {
        pstcUsbDeviceIntern->pstcEp0IN->bIsActive = FALSE;
    }
    //pstcUsbDeviceIntern->enControlStage = ControlStageUnknown;
    /*if ((pu8Buffer[0] & 0x60) == USB_DEVREQ_CLSTYPE)
    {
        if(pu8Buffer[1] == USB_DEVREQ_SET_IDLE)
        {
            //recive SET_IDLE
            BITMASK_CLEAR(pstcUsb->EP0IS,_EP0IS_DRQI);    // DRQi <- 0
            pstcUsb->EP0OS_f.DRQOIE  = 1;
            return;
        }
    }*/
    pstcUsb->EP0OS_f.DRQOIE  = 1;
    u16BytesCount = u16BytesCount;
}

/**
 ******************************************************************************
 ** \brief Send control data
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pu8Buffer Buffer
 **
 ** \param u32DataLength Data length
 **
 ** \return none
 **
 ******************************************************************************/
void UsbDevice_SendDataControl(stc_usbn_t * pstcUsb,uint8_t* pu8Buffer, uint32_t u32DataLength)
{
    stc_usbdevice_intern_data_t* pstcUsbDeviceIntern;
    pstcUsbDeviceIntern = UsbDeviceGetInternDataPtr(pstcUsb);
    pstcUsbDeviceIntern->pstcEp0IN->pfnRxTxBlockCallback = ControlTxCallback;
    pstcUsbDeviceIntern->pstcEp0IN->bIsActive = FALSE;
    UsbDevice_SendData(pstcUsb, pstcUsbDeviceIntern->pstcEp0IN, pu8Buffer, u32DataLength, UsbIRQ);
    
}

/**
 ******************************************************************************
 ** \brief Receive control data
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pfnCompletionCallback Data received callback
 **
 ** \return none
 **
 ******************************************************************************/
void UsbDevice_ReceiveDataControl(stc_usbn_t * pstcUsb, usbdevice_controltranfered_func_ptr_t pfnCompletionCallback)
{
    stc_usbdevice_intern_data_t* pstcUsbDeviceIntern;
    pstcUsbDeviceIntern = UsbDeviceGetInternDataPtr(pstcUsb);
    if (pstcUsbDeviceIntern->pu8LastControlTransfer != NULL)
    {
        pfnCompletionCallback(pstcUsb, pstcUsbDeviceIntern->pstcEp0OUT);
        pstcUsbDeviceIntern->pu8LastControlTransfer = NULL;
    } else
    {
        pstcUsbDeviceIntern->pfnControlTransferred = pfnCompletionCallback;
        /*if (pstcUsb->EP0OS_f.DRQO == 1)
        {
            ControlRxCallback(pstcUsb,pstcUsbDeviceIntern->pstcEp0OUT);
        }  
        pstcUsb->EP0OS_f.DRQOIE  = 1;*/
        //Usb_SetEndpointRxTxCallback(pstcUsbDeviceIntern->pstcEp0OUT,ControlRxCallback);
    }
    
}

/**
 ******************************************************************************
 ** \brief Register USB class
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pstcUsbClassConfig configuration of USB class
 **
 ** \param ppu8Buffer Pointer to buffer where to return configuration buffer pointer
 **
 ** \param pu8InterfaceLength Pointer to where to return length of configuration buffer
 **
 ** \return Ok if registered, Error if error occured
 **
 ******************************************************************************/
en_result_t UsbDevice_RegisterVendorClass(stc_usbn_t* pstcUsb, stc_usbdevice_class_config_t* pstcUsbClassConfig, uint8_t** ppu8Buffer, uint8_t* pu8InterfaceLength)
{
    uint16_t u16i;
    stc_usbdevice_intern_data_t* pstcUsbDeviceIntern;
    pstcUsbDeviceIntern = UsbDeviceGetInternDataPtr(pstcUsb);
    
    for(u16i = 0;u16i < (pstcUsbDeviceIntern->pu8ConfigDescriptor[2] + pstcUsbDeviceIntern->pu8ConfigDescriptor[3]*256);u16i++)
    {
        if (pstcUsbDeviceIntern->pu8ConfigDescriptor[u16i+1] == USBDESCR_INTERFACE)
        {
            if  (((pstcUsbDeviceIntern->pu8ConfigDescriptor[u16i+5] == pstcUsbClassConfig->u8InterfaceClass) &&
                (pstcUsbDeviceIntern->pu8ConfigDescriptor[u16i+6] == pstcUsbClassConfig->u8InterfaceSubClass) &&
                (pstcUsbDeviceIntern->pu8ConfigDescriptor[u16i+7] == pstcUsbClassConfig->u8InterfaceProtocoll)) && 
                ((pstcUsbClassConfig->u8InterfaceNumber == 0xFF) || (pstcUsbClassConfig->u8InterfaceNumber == pstcUsbDeviceIntern->pu8ConfigDescriptor[u16i+2])))
            {
                *ppu8Buffer = (pstcUsbDeviceIntern->pu8ConfigDescriptor + u16i);
                pstcUsbDeviceIntern->stcUsbClasses[pstcUsbDeviceIntern->pu8ConfigDescriptor[u16i+2]].u8InterfaceClass = pstcUsbClassConfig->u8InterfaceClass;
                pstcUsbDeviceIntern->stcUsbClasses[pstcUsbDeviceIntern->pu8ConfigDescriptor[u16i+2]].u8InterfaceSubClass = pstcUsbClassConfig->u8InterfaceSubClass;
                pstcUsbDeviceIntern->stcUsbClasses[pstcUsbDeviceIntern->pu8ConfigDescriptor[u16i+2]].u8InterfaceProtocoll = pstcUsbClassConfig->u8InterfaceProtocoll;
                pstcUsbDeviceIntern->stcUsbClasses[pstcUsbDeviceIntern->pu8ConfigDescriptor[u16i+2]].pfnSetupRequestCallback = pstcUsbClassConfig->pfnSetupRequestCallback;
                pstcUsbDeviceIntern->stcUsbClasses[pstcUsbDeviceIntern->pu8ConfigDescriptor[u16i+2]].pfnConnectionCallback = pstcUsbClassConfig->pfnConnectionCallback;
                pstcUsbDeviceIntern->stcUsbClasses[pstcUsbDeviceIntern->pu8ConfigDescriptor[u16i+2]].pfnDisconnectionCallback = pstcUsbClassConfig->pfnDisconnectionCallback;
                pstcUsbDeviceIntern->stcUsbClasses[pstcUsbDeviceIntern->pu8ConfigDescriptor[u16i+2]].pfnConfCallback = pstcUsbClassConfig->pfnConfCallback;
                while(u16i < (pstcUsbDeviceIntern->pu8ConfigDescriptor[2] + pstcUsbDeviceIntern->pu8ConfigDescriptor[3]*256))
                {
                    u16i += pstcUsbDeviceIntern->pu8ConfigDescriptor[u16i];    // go to next descriptor
                    if (pstcUsbDeviceIntern->pu8ConfigDescriptor[u16i+1] == USBDESCR_INTERFACE) 
                    {
                        break;
                    }
                }
                *pu8InterfaceLength = (uint8_t)((uint32_t)u16i - (uint32_t)(((uint32_t)*ppu8Buffer - (uint32_t)pstcUsbDeviceIntern->pu8ConfigDescriptor)));
                return Ok;
            }
        }
        u16i += pstcUsbDeviceIntern->pu8ConfigDescriptor[u16i] - 1;    // go to next descriptor
    }
    return Error;
}
       
/**
 ******************************************************************************
 ** \brief Get Usb Device status
 **
 ** \param pstcUsb Pointer to USB instance
 **
 **
 ** \return status as en_usb_status_t
 **
 ******************************************************************************/
en_usb_status_t UsbDevice_GetStatus(stc_usbn_t* pstcUsb)
{
    stc_usbdevice_intern_data_t* pstcUsbDeviceIntern;
    pstcUsbDeviceIntern = UsbDeviceGetInternDataPtr(pstcUsb); 
    return pstcUsbDeviceIntern->enStatus; 
}

/**
 ******************************************************************************
 ** \brief Get information about the specified endpoint status
 **
 ** \param pstcEpHandle Endpoint handle 
 **
 ** \return Status
 **
 ******************************************************************************/
/*en_usb_endpoint_status_t UsbDevice_HalEndpointStatus(stc_usbn_endpoint_data_t* pstcEpHandle)
{
    return Usb_HalEndpointStatus(pstcEpHandle);
}*/

/**
 ******************************************************************************
 ** \brief Initialization handled from Usb Lowlevel Driver (usb.c)
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pstcUsbConfig Usb Configuration
 **
 ******************************************************************************/
void UsbDevice_InitCallback(stc_usbn_t* pstcUsb, stc_usb_config_t* pstcUsbConfig)
{
    uint16_t i;
    stc_usbdevice_intern_data_t* pstcUsbDeviceIntern;
    pstcUsbDeviceIntern = UsbDeviceGetInternDataPtr(pstcUsb);
    for(i = 0;i<(pstcUsbDeviceIntern->pu8ConfigDescriptor[2] + pstcUsbDeviceIntern->pu8ConfigDescriptor[3]*256);i++)
    {
        if (pstcUsbDeviceIntern->pu8ConfigDescriptor[i+1] == USBDESCR_ENDPOINT)
        {
            Usb_SetupHalEndpoint(pstcUsb,pstcUsbDeviceIntern->pu8ConfigDescriptor[i+2], pstcUsbDeviceIntern->pu8ConfigDescriptor[i+4] | (pstcUsbDeviceIntern->pu8ConfigDescriptor[i+5] << 8),(en_usb_ep_type_t)(pstcUsbDeviceIntern->pu8ConfigDescriptor[i+3] & 0x03),FALSE);
        }
        i += pstcUsbDeviceIntern->pu8ConfigDescriptor[i] - 1;    // go to next descriptor
    }
    pstcUsbConfig->pfnSetpCallback = &SetupRxCallback;
    pstcUsbConfig->pfnConfCallback = &ConfigChangeCallback;
    
    pstcUsbDeviceIntern->pstcEp0IN = Usb_GetEndpointPtr(pstcUsb,0x80);
    pstcUsbDeviceIntern->pstcEp0OUT = Usb_GetEndpointPtr(pstcUsb,0x00);

    pstcUsbDeviceIntern->pstcEp0IN->pstcEndpointBuffer = &(pstcUsbDeviceIntern->stcEndpointBuffer);
    pstcUsbDeviceIntern->pstcEp0OUT->pstcEndpointBuffer = &(pstcUsbDeviceIntern->stcEndpointBuffer);
        
    pstcUsbDeviceIntern->pstcEp0IN->pfnRxTxCallback = &ControlTxCallback;
    pstcUsbDeviceIntern->pstcEp0IN->bAutomaticNullTermination = TRUE;
    
    pstcUsbDeviceIntern->pstcEp0OUT->pfnRxTxCallback = &ControlRxCallback;
    pstcUsbDeviceIntern->pu8LastControlTransfer = NULL;
    
    if (pstcUsbDeviceIntern->pfnInitClassesCallback != NULL)
    {
        pstcUsbDeviceIntern->pfnInitClassesCallback(pstcUsb);
    }
}

/**
 ******************************************************************************
 ** \brief Initialization handled from Usb Lowlevel Driver (usb.c)
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pstcUsbConfig Usb Configuration To Export Data
 **
 ** \param pstcConfig Usb Device Config
 **
 ******************************************************************************/
void UsbDevice_Init(stc_usbn_t* pstcUsb, stc_usb_config_t* pstcUsbConfig, stc_usbdevice_config_t* pstcConfig)
{
    stc_usbdevice_intern_data_t* pstcUsbDeviceIntern;
    pstcUsbDeviceIntern = UsbDeviceGetInternDataPtr(pstcUsb);
    
    pstcUsbConfig->enMode |= UsbDeviceEnabled;
    pstcUsbConfig->pu8DeviceDescriptor = pstcConfig->pu8DeviceDescriptor;
    pstcUsbConfig->pu8ConfigDescriptor = pstcConfig->pu8ConfigDescriptor; 
    pstcUsbConfig->pfnDeviceInit = UsbDevice_InitCallback;
    //pstcUsbConfig->pstcStringDescriptors = pstcConfig->pstcStringDescriptors;
    //pstcUsbConfig->u8StringDescriptorCount = pstcConfig->u8StringDescriptorCount;
      
    pstcUsbDeviceIntern->pu8DeviceDescriptor = pstcConfig->pu8DeviceDescriptor;
    pstcUsbDeviceIntern->pu8ConfigDescriptor = pstcConfig->pu8ConfigDescriptor;
    pstcUsbDeviceIntern->astcReportDescriptors = pstcConfig->astcReportDescriptors;
    pstcUsbDeviceIntern->pstcStringDescriptors = pstcConfig->pstcStringDescriptors;
    pstcUsbDeviceIntern->u8StringDescriptorCount = pstcConfig->u8StringDescriptorCount;
    
    pstcUsbDeviceIntern->pfnInitClassesCallback = pstcConfig->pfnInitClassesCallback;
    
    
    pstcUsbDeviceIntern->pstcEp0IN = Usb_GetEndpointPtr(pstcUsb,0x80);
    pstcUsbDeviceIntern->pstcEp0OUT = Usb_GetEndpointPtr(pstcUsb,0x00);

    pstcUsbDeviceIntern->pstcEp0IN->pstcEndpointBuffer = &(pstcUsbDeviceIntern->stcEndpointBuffer);
    pstcUsbDeviceIntern->pstcEp0OUT->pstcEndpointBuffer = &(pstcUsbDeviceIntern->stcEndpointBuffer);
        
    pstcUsbDeviceIntern->pstcEp0IN->pfnRxTxCallback = &ControlTxCallback;
    pstcUsbDeviceIntern->pstcEp0OUT->pfnRxTxCallback = &ControlRxCallback;
}


/**
 ******************************************************************************
 ** \brief Setup Endpoint
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pstcConfig Pointer of endpoint configuration
 **
 ** \return pointer of endpoint handle #stc_usbn_endpoint_data_t
 **
 ******************************************************************************/
stc_usbn_endpoint_data_t* UsbDevice_SetupEndpoint(stc_usbn_t* pstcUsb, stc_usbdevice_endpoint_config_t* pstcConfig)
{
    stc_usbn_endpoint_data_t* pstcEndpointHandle;
    pstcEndpointHandle = Usb_GetEndpointPtr(pstcUsb, pstcConfig->u8EndpointAddress);
    pstcEndpointHandle->pfnRxTxBlockCallback = pstcConfig->pfnRxTxCallback;
    pstcEndpointHandle->pstcEndpointBuffer = pstcConfig->pstcEndpointBuffer;
    return pstcEndpointHandle;
}

//@} // UsbDeviceGroup
#endif /* (FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) */
