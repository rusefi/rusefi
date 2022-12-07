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
/** \file usbhost.c
 **
 ** A detailed description is available at 
 ** @link UsbHostGroup USB Host Module description @endlink
 **
 ** History:
 **   - 2012-08-28  2.0  MSc  First version (starting at version 2.0).
 **   - 2012-11-13  2.1  MSc  Some problems while switching from Host to Device fixed
 **   - 2013-01-28  2.2  MSc  Better Device Detection
 **   - 2013-06-04  2.3  MSc  FM4 support added
 **   - 2014-02-28  2.4  MSc  Speedup in package handling, fixes in fifo initialization
 **                           ISO transfers added
 **   - 2014-05-06  2.5  MSc  Fixing some variable initializations in disconnect routine
 **                           Added USBHOST_CONNECTION_DELAY to specify a delay in interations 
 **                           for bus-reset / connection delay
 **   - 2014-07-25  2.6  MSc  Updated memory usage EP0
 **   - 2014-09-03  2.7  MSc  Doxygen examples added
 **   - 2015-09-03  2.8  MSCH Fixed disconnection deinitializations
 **
 ******************************************************************************/


/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/


#include "usbhost.h"

#if (FM_PERIPHERAL_USB_HOST_ENABLED == ON)

#include "usbhostclasssupervisor.h"

#if (USE_USBHOSTHW_H == 1)
#include "usbhosthw.h"
#endif

/**
 ******************************************************************************
 ** \addtogroup UsbHostGroup
 ******************************************************************************/
//@{




const stc_usb_request_t stcDeviceDescriptorRequest = {0x80,0x06,0x0100,0x00,18};
const stc_usb_request_t stcShortDeviceDescriptorRequest = {0x80,0x06,0x0100,0x00,8};
const stc_usb_request_t stcClearStallEp0 = {0x02,0x01,0x0000,0x00,0x00};


static stc_usbhost_intern_data_t* UsbHostGetInternDataPtr(stc_usbn_t* pstcUsb) ;
static void EnumerationStateMachine(stc_usbn_t* pstcUsb);
static void ConvertRequestToByteArray(uint8_t* pu8Buffer, stc_usb_request_t* pstcSetup);
static void StallCompletionEp0(stc_usbn_t* pstcUsb, uint8_t* pu8Buffer, uint32_t u32Size);
static void UsbHost_SetupComplete(stc_usbn_t* pstcUsb);



/// Macro to return the number of enabled MFS instances
#define USBHOST_INSTANCE_COUNT (uint32_t)(sizeof(m_astcUsbHostInstanceDataLut) / sizeof(m_astcUsbHostInstanceDataLut[0]))

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/// Look-up table for all enabled MFS instances and their internal data
stc_usbhostn_instance_data_t m_astcUsbHostInstanceDataLut[] =
{
    #if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && (defined(USB0)) && (FM_PERIPHERAL_ENABLE_USB0_HOST == ON))
    { &USB0,  // pstcInstance
      NULL    // stcInternData (not initialized yet)
    },
    #endif
    #if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && (defined(USB1)) && (FM_PERIPHERAL_ENABLE_USB1_HOST == ON))
    { &USB1,  // pstcInstance
      NULL    // stcInternData (not initialized yet)
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
static stc_usbhost_intern_data_t* UsbHostGetInternDataPtr(stc_usbn_t* pstcUsb) 
{
    volatile uint32_t u32Instance;
   
    for (u32Instance = 0; u32Instance < USBHOST_INSTANCE_COUNT; u32Instance++)
    {
        if ((uint32_t)pstcUsb == (uint32_t)(m_astcUsbHostInstanceDataLut[u32Instance].pstcInstance))
        {
            return &m_astcUsbHostInstanceDataLut[u32Instance].stcInternData;
        }
    }

    return NULL;
}


/**
 ******************************************************************************
 ** \brief Add new endpoint
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pstcHandler returns pointer to stc_host_endpoint_t (pointer of pointer)
 **
 ** \param u8EndpointAddress Endpointaddress
 **
 ** \param u16FifoSize Fifosize
 **
 ** \param u8Interval Interval (for interrupt transfers)
 **
 ** \param CompletionHandler Transfer completion callback routine
 **
 ** \return USBHOST_SUCCESS at success
 **
 ******************************************************************************/
uint8_t UsbHost_AddHostEndpoint(stc_usbn_t* pstcUsb, stc_host_endpoint_t** pstcHandler, uint8_t u8EndpointAddress, uint16_t u16FifoSize, uint8_t u8Interval, void(* CompletionHandler)(stc_usbn_t* pstcUsb))
{
   uint8_t i;
   stc_usbhost_intern_data_t* pstcUsbHostIntern;
   volatile boolean_t bOldSchedulerLockState;
   pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
   bOldSchedulerLockState = pstcUsbHostIntern->bSchedulerLock;
   pstcUsbHostIntern->bSchedulerLock = TRUE;
   pstcUsbHostIntern->bSchedulerLock = TRUE;
   pstcUsbHostIntern->bSchedulerLock = TRUE;
   for(i=0;i<MAX_HOST_ENDPOINTS;i++)
   {
      if ((pstcUsbHostIntern->astcHostEndpoints[i].u16FifoSize == 0) || ((&pstcUsbHostIntern->astcHostEndpoints[i] == *pstcHandler) && (*pstcHandler != 0)))
      {
          if ((&pstcUsbHostIntern->astcHostEndpoints[i] == *pstcHandler) && (*pstcHandler != 0))
          {
             USBDBG("    |->Endpoint exists in list...\n");
          }
          USBDBGVAL8("    |->New EP, Address: ",u8EndpointAddress);
          pstcUsbHostIntern->astcHostEndpoints[i].u8Stage = 0;
          pstcUsbHostIntern->astcHostEndpoints[i].u8Status = 0;
          pstcUsbHostIntern->astcHostEndpoints[i].u16FifoSize = u16FifoSize;
          pstcUsbHostIntern->astcHostEndpoints[i].u8Address = u8EndpointAddress;
          pstcUsbHostIntern->astcHostEndpoints[i].u8Interval = u8Interval;
          pstcUsbHostIntern->astcHostEndpoints[i].u8IntervalCount = u8Interval;
          pstcUsbHostIntern->astcHostEndpoints[i].CompletionHandler = CompletionHandler;
          *pstcHandler = &pstcUsbHostIntern->astcHostEndpoints[i];
          USBDBGVAL8("    |->New EP, Index: ",i);
          USBDBGVAL32("   |->New EP, Handler: ",*pstcHandler);
          pstcUsbHostIntern->u8EndpointListEnd = i;
          pstcUsbHostIntern->bSchedulerLock = bOldSchedulerLockState;
          pstcUsbHostIntern->bSchedulerLock = bOldSchedulerLockState;
          pstcUsbHostIntern->bSchedulerLock = bOldSchedulerLockState;
          return USBHOST_SUCCESS;
      }
   }
   pstcUsbHostIntern->bSchedulerLock = bOldSchedulerLockState;
   pstcUsbHostIntern->bSchedulerLock = bOldSchedulerLockState;
   pstcUsbHostIntern->bSchedulerLock = bOldSchedulerLockState;
   return USBHOST_ERROR; 
}

/**
 ******************************************************************************
 ** \brief Get pointzer to host endpoint via endpoint address
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param u8EndpointNumber Endpointaddress
 **
 ** \return Pointer to stc_host_endpoint_t
 **
 ******************************************************************************/
stc_host_endpoint_t* UsbHost_EndpointFromNumber(stc_usbn_t* pstcUsb, uint8_t u8EndpointNumber)
{
   uint8_t i;
   stc_usbhost_intern_data_t* pstcUsbHostIntern;
   pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
   
   for(i=0;i<=pstcUsbHostIntern->u8EndpointListEnd;i++)
   {
       if ((pstcUsbHostIntern->astcHostEndpoints[i].u8Address & 0x0F) == u8EndpointNumber)
       {
           return &pstcUsbHostIntern->astcHostEndpoints[i];
       }
   }
   return NULL;
}

/**
 ******************************************************************************
 ** \brief Force abort current transfer
 **
 ** \param pstcEndpoint Endpoint handle
 **
 ******************************************************************************/
void UsbHost_AbortTransfer(stc_host_endpoint_t* pstcEndpoint)
{
    if (pstcEndpoint == NULL)
    {
        return;
    }
    pstcEndpoint->bAbortTransfer = TRUE;
}


/**
 ******************************************************************************
 ** \brief Force reset data toggle
 **
 ** \param pstcEndpoint Endpoint handle
 **
 ******************************************************************************/
void UsbHost_ResetToggle(stc_host_endpoint_t* pstcEndpoint)
{
    if (pstcEndpoint == NULL)
    {
        return;
    }
    pstcEndpoint->bToggle = FALSE;
}

/**
 ******************************************************************************
 ** \brief Completion callback for ClearEndpoint setup request
 **
 ** \param pstcUsb USB handle
 **
 ** \param pu8Buffer data buffer (not used)
 **
 ** \param u32DataSize data size (not used)
 **
 ******************************************************************************/
void UsbHost_ClearEndpointCompletion(stc_usbn_t* pstcUsb, uint8_t* pu8Buffer, uint32_t u32DataSize)
{
    stc_usbhost_intern_data_t* pstcUsbHostIntern;
    pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
    if (pstcUsbHostIntern->pstcClearEndpoint == NULL)
    {
        return;
    }
    UsbHost_ResetToggle(pstcUsbHostIntern->pstcClearEndpoint);
    pstcUsbHostIntern->pstcClearEndpoint->bAbortTransfer = FALSE;
    pstcUsbHostIntern->pstcClearEndpoint = NULL;
}

/**
 ******************************************************************************
 ** \brief Clear endpoint setup request
 **
 ** \param pstcUsb USB handle
 **
 ** \param pstcEndpoint Endpoint handle
 **
 ******************************************************************************/
void UsbHost_ClearEndpoint(stc_usbn_t* pstcUsb, stc_host_endpoint_t* pstcEndpoint)
{
    stc_usb_request_t setup;
    stc_usbhost_intern_data_t* pstcUsbHostIntern;
    pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
    ZERO_STRUCT(setup);
    setup.bmRequestType=0x02;  
    setup.bRequest=0x01;
    setup.wValue=0x00;  
    setup.wIndex=pstcEndpoint->u8Address;
    setup.wLength=0x00;
    UsbHost_AbortTransfer(pstcEndpoint);
    pstcUsbHostIntern->pstcClearEndpoint = pstcEndpoint;
    UsbHost_SetupRequest(pstcUsb,&setup,UsbHost_ClearEndpointCompletion);
    return;
}
/**
 ******************************************************************************
 ** \brief Clear endpoint setup request (polled)
 **
 ** \param pstcUsb USB handle
 **
 ** \param pstcEndpoint Endpoint handle
 **
 ******************************************************************************/                           
void UsbHost_ClearEndpointPoll(stc_usbn_t* pstcUsb, stc_host_endpoint_t* pstcEndpoint)
{
    volatile uint32_t u32Timeout = 1000000;
    stc_usbhost_intern_data_t* pstcUsbHostIntern;
    pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
    UsbHost_ClearEndpoint(pstcUsb, pstcEndpoint);
    while((pstcUsbHostIntern->pstcClearEndpoint != NULL) && (u32Timeout > 0)) u32Timeout--;
}

/**
 ******************************************************************************
 ** \brief Enumeration Statemachine
 **
 ** \param pstcUsb Pointer to USB instance
 **
 **
 ******************************************************************************/
static void EnumerationStateMachine(stc_usbn_t* pstcUsb)
{
    uint8_t* pu8SetupBuffer;
    uint32_t u32Size = UsbHost_ReceiveSetupData(pstcUsb, &pu8SetupBuffer);
    stc_usbhost_intern_data_t* pstcUsbHostIntern;
    pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
    pstcUsbHostIntern->Ep0IN->CompletionHandler = 0;
	
    USBDBG("ENUM STATEMACH\n");
    if (pu8SetupBuffer != 0)
    {
        if (u32Size > 0)
        {
            if (pu8SetupBuffer[1] == 0x02)
            {
                USBDBG("Received Configuration\n");
            }
            if (pu8SetupBuffer[1] == 0x01)
            {
                USBDBG("Enumeration Process:\n");
                USBDBG("  ->Received Device Descriptor\n");
                USBDBGVAL16("  ->New EP0 buffersize: ",(uint16_t)((uint8_t)pu8SetupBuffer[7]));
                pstcUsbHostIntern->Ep0IN->u16FifoSize = (uint16_t)((uint8_t)pu8SetupBuffer[7]);
                pstcUsbHostIntern->Ep0OUT->u16FifoSize = (uint16_t)((uint8_t)pu8SetupBuffer[7]);
            }
        }
        else
        {
            if (pstcUsbHostIntern->u8DeviceStatus == USBHOST_DEVICE_ADDRESSING)
            {
                   pstcUsbHostIntern->u8EnumerationCounter = 0;
                   pstcUsb->HADR = pstcUsbHostIntern->u8DeviceAddress;
                   USBDBG("Enumeration Process:\n");
                   USBDBGVAL8("  ->Addressed:  ",pstcUsbHostIntern->u8DeviceAddress);
                   pstcUsbHostIntern->u8DeviceStatus = USBHOST_DEVICE_ADDRESSED;
                   //UsbClassSupervisor_Init(pstcUsb);
            }
        }
    }
    else
    {
        if (pstcUsbHostIntern->Ep0IN->u16FifoSize == 20)
        {
            USBDBG("Enumeration Process:\n");
            USBDBG("  -> Endpoint 0 has no Fifo size specified.\n"); 
            USBDBG("     Requesting Device Descriptor.\n");
            UsbHost_SetupRequest(pstcUsb,(stc_usb_request_t *)&stcShortDeviceDescriptorRequest,0);
        }
        else
        {            
            if (pstcUsbHostIntern->u8DeviceStatus == USBHOST_DEVICE_IDLE)
            {
                USBDBG("Enumeration Process:\n");
                pstcUsbHostIntern->Ep0IN->CompletionHandler = EnumerationStateMachine;
                pstcUsbHostIntern->u8DeviceStatus = USBHOST_DEVICE_ADDRESSING;
                UsbHost_AddressDevice(pstcUsb, 2);
                pstcUsbHostIntern->u8EnumerationCounter = 0;
            }
        }
    }
    if (pstcUsbHostIntern->u8DeviceStatus != USBHOST_DEVICE_ADDRESSED)
    {
        pstcUsbHostIntern->u8EnumerationCounter++;
        if (pstcUsbHostIntern->u8EnumerationCounter == 40)
        {
            pstcUsbHostIntern->u32ConnectionDelay -= 1000;
            if (pstcUsbHostIntern->u32ConnectionDelay <= 1000)
            {
                pstcUsbHostIntern->u32ConnectionDelay = USBHOST_CONNECTION_DELAY;
            }
            pstcUsbHostIntern->u8EnumerationCounter = 0;
            //UsbHost_DeInit();
            //UsbHost_Init();
        }
        
        if (pstcUsbHostIntern->u8EnumerationCounter > 50)
        {
            /*stc_usb_request_t stcSetup;
            pstcUsbHostIntern->u8DeviceStatus = USBHOST_DEVICE_IDLE; 
            USBDBG("Enumeration timed out...\n");
            pstcUsbHostIntern->u8EnumerationCounter = 0;
            USBDBG("ENUMERATION FAILED - SYSTEM HALT\n");
            stcSetup.bmRequestType=0x82;  
            stcSetup.bRequest=0x00;
            stcSetup.wValue=0x00;  
            stcSetup.wIndex=0x00;
            stcSetup.wLength=0x02;
            UsbHost_SetupRequest(pstcUsb, (stc_usb_request_t *)&stcShortDeviceDescriptorRequest,EndpointInStatusUpdate);*/
        }
        else
        {
            Usb_AddTimeOut(pstcUsb, EnumerationStateMachine,100);
        }
    }
    else
    {
        UsbClassSupervisor_Init(pstcUsb);
    }
}

/**
 ******************************************************************************
 ** \brief Convert stc_usb_request_t to byte array
 **
 ** \param pu8Buffer pointer to byte array
 **
 ** \param pstcSetup Setup request
 **
 ******************************************************************************/
static void ConvertRequestToByteArray(uint8_t* pu8Buffer, stc_usb_request_t* pstcSetup)
{
    pu8Buffer[0] = pstcSetup->bmRequestType;
    pu8Buffer[1] = pstcSetup->bRequest;
    pu8Buffer[2] = (uint8_t)(pstcSetup->wValue & 0xFF);
    pu8Buffer[3] = (uint8_t)((pstcSetup->wValue >> 8) & 0xFF);
    pu8Buffer[4] = (uint8_t)(pstcSetup->wIndex & 0xFF);
    pu8Buffer[5] = (uint8_t)((pstcSetup->wIndex >> 8) & 0xFF);
    pu8Buffer[6] = (uint8_t)(pstcSetup->wLength & 0xFF);
    pu8Buffer[7] = (uint8_t)((pstcSetup->wLength >> 8) & 0xFF);
}

/**
 ******************************************************************************
 ** \brief Address device
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param u8NewAddress New address
 **
 ******************************************************************************/
void UsbHost_AddressDevice(stc_usbn_t* pstcUsb, uint8_t u8NewAddress)
{
    stc_usb_request_t stcSetup = {0x00,0x05,0x02,0x00,0x00};
    stc_usbhost_intern_data_t* pstcUsbHostIntern;
    pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
    
    USBDBGVAL8("Start Addressing:  ",u8NewAddress);
    stcSetup.wValue = u8NewAddress;
    pstcUsbHostIntern->u8DeviceAddress = u8NewAddress;  // Device Address = NewAddress
    pstcUsbHostIntern->u8DeviceStatus = USBHOST_DEVICE_ADDRESSING;
    UsbHost_SetupRequest(pstcUsb, &stcSetup,NULL);
}

/**
 ******************************************************************************
 ** \brief Receive data of setup request
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pu8Buffer pointer returned
 **
 ** \return length
 **
 ******************************************************************************/
uint32_t UsbHost_ReceiveSetupData(stc_usbn_t* pstcUsb, uint8_t** pu8Buffer)
{
    uint32_t u32Size;
    stc_usbhost_intern_data_t* pstcUsbHostIntern;
    pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
    
    if (pstcUsbHostIntern->Ep0IN->u8Status & USBHOST_ENDPOINTSTATUS_DATA)
    {
        pstcUsbHostIntern->Ep0IN->u8Status &= ~USBHOST_ENDPOINTSTATUS_DATA;
        *pu8Buffer = pstcUsbHostIntern->Ep0IN->pu8Buffer;
        u32Size = (pstcUsbHostIntern->Ep0IN->u32DataSize);
        return u32Size;
    }
    *pu8Buffer = 0;
    return 0;
}

/**
 ******************************************************************************
 ** \brief Add data package to setup package
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pstcSetup setup request
 **
 ** \param pu8Data data to send
 **
 ** \param SetupCompletion completion callback routine
 **
 ******************************************************************************/
void UsbHost_SetupRequestWithData(stc_usbn_t* pstcUsb, stc_usb_request_t* pstcSetup, uint8_t* pu8Data, void(*SetupCompletion)(stc_usbn_t* pstcUsb, uint8_t* pu8Buffer, uint32_t u32DataSize))
{
    stc_usbhost_intern_data_t* pstcUsbHostIntern;
    pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
    pstcUsbHostIntern->pu8SetupData = pu8Data;
    UsbHost_SetupRequest(pstcUsb, pstcSetup,SetupCompletion);
}

/**
 ******************************************************************************
 ** \brief Send setup package
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pstcSetup setup request
 **
 ** \param SetupCompletion completion callback routine
 **
 ******************************************************************************/
void UsbHost_SetupRequest(stc_usbn_t* pstcUsb, stc_usb_request_t* pstcSetup, void(*SetupCompletion)(stc_usbn_t* pstcUsb, uint8_t* pu8Buffer, uint32_t u32DataSize))
{
    stc_usbhost_intern_data_t* pstcUsbHostIntern;
    volatile boolean_t bOldSchedulerLockState;
    pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
    bOldSchedulerLockState = pstcUsbHostIntern->bSchedulerLock;
    if ((pstcUsbHostIntern->Ep0IN->u8Status & USBHOST_ENDPOINTSTATUS_INUSE) || (pstcUsbHostIntern->Ep0OUT->u8Status & USBHOST_ENDPOINTSTATUS_INUSE))
    {
        return;
    }
    pstcUsbHostIntern->bSchedulerLock = TRUE;
    pstcUsbHostIntern->SetupCompletionHandler = SetupCompletion;
    if ((((pstcUsbHostIntern->Ep0IN->u8Status) & USBHOST_ENDPOINTSTATUS_STALL) == 0) && (((pstcUsbHostIntern->Ep0OUT->u8Status) & USBHOST_ENDPOINTSTATUS_STALL) == 0))
    {
        memcpy((uint8_t*)&pstcUsbHostIntern->stcLastSetup,(uint8_t*)pstcSetup,sizeof(pstcUsbHostIntern->stcLastSetup));
    }
    ConvertRequestToByteArray(pstcUsbHostIntern->pu8Setup,pstcSetup);
    USBDBGVAL8("Setup [0]",pstcUsbHostIntern->pu8Setup[0]);
    USBDBGVAL8("[1]",pstcUsbHostIntern->pu8Setup[1]);
    USBDBGVAL8("[2]",pstcUsbHostIntern->pu8Setup[2]);
    USBDBGVAL8("[3]",pstcUsbHostIntern->pu8Setup[3]);
    USBDBGVAL8("[4]",pstcUsbHostIntern->pu8Setup[4]);
    USBDBGVAL8("[5]",pstcUsbHostIntern->pu8Setup[5]);
    USBDBGVAL8("[6]",pstcUsbHostIntern->pu8Setup[6]);
    USBDBGVAL8("[7]",pstcUsbHostIntern->pu8Setup[7]);

    //UsbHost_TransferDataToFifo(pstcUsb, pstcUsbHostIntern->pu8Setup,8,pstcUsbHostIntern->Ep0OUT->u16FifoSize);
    
    pstcUsbHostIntern->Ep0OUT->u8Stage = USBHOST_STAGE_SETUP;
    pstcUsbHostIntern->Ep0IN->u8Stage = USBHOST_STAGE_SETUP;
    
    pstcUsbHostIntern->Ep0IN->u8Status &= ~USBHOST_ENDPOINTSTATUS_INUSE;
    pstcUsbHostIntern->Ep0OUT->u8Status &= ~USBHOST_ENDPOINTSTATUS_INUSE;
    
    pstcUsbHostIntern->Ep0IN->pu8BufferPos = pstcUsbHostIntern->Ep0IN->pu8Buffer;
    pstcUsbHostIntern->Ep0IN->pu8BufferPosNextPackage = pstcUsbHostIntern->Ep0IN->pu8Buffer;
    pstcUsbHostIntern->Ep0IN->u8Status &= ~USBHOST_ENDPOINTSTATUS_DATA;
    pstcUsbHostIntern->Ep0OUT->bToggle = FALSE;
    pstcUsbHostIntern->Ep0OUT->u32DataSize = 8;
    pstcUsbHostIntern->Ep0OUT->pu8Buffer = pstcUsbHostIntern->pu8Setup;
    pstcUsbHostIntern->Ep0OUT->pu8BufferPos = pstcUsbHostIntern->pu8Setup;
    pstcUsbHostIntern->Ep0OUT->u32BufferSize = 8;
    pstcUsbHostIntern->Ep0IN->CompletionHandler = UsbHost_SetupComplete;
    pstcUsbHostIntern->Ep0OUT->CompletionHandler = UsbHost_SetupComplete;
    

    //pstcUsbHostIntern->Ep0OUT->u8Status &= ~USBHOST_ENDPOINTSTATUS_INITIATE;
    //pstcUsbHostIntern->Ep0OUT->u8Status |= USBHOST_ENDPOINTSTATUS_INUSE;
    //pstcUsbHostIntern->Ep0IN->u8Status &= ~USBHOST_ENDPOINTSTATUS_INITIATE;
    pstcUsbHostIntern->Ep0OUT->u8Status |= USBHOST_ENDPOINTSTATUS_INITIATE;
    //pstcUsbHostIntern->u8SetupDelay = 3;
    //pstcUsbHostIntern->ActiveEndpoint=pstcUsbHostIntern->Ep0OUT;
    //UsbHost_TransferData(Ep0OUT,u8Setup,8,SetupComplete);
    
    // synchronous token (sent after next SOF)
    
    //pstcUsbHostIntern->Ep0OUT->u8LastToken = CREATE_TOKEN(pstcUsbHostIntern->Ep0OUT->bToggle,HTOKEN_SETUP,pstcUsbHostIntern->Ep0OUT->u8Address & 0x0F);  
    //pstcUsbHostIntern->ActiveEndpoint = pstcUsbHostIntern->Ep0OUT; 
    //pstcUsbHostIntern->u8LastToken = pstcUsbHostIntern->Ep0OUT->u8LastToken; 
    //pstcUsbHostIntern->u8SyncToken = pstcUsbHostIntern->Ep0OUT->u8LastToken;
                  
    pstcUsbHostIntern->bSchedulerLock = bOldSchedulerLockState;
}

/**
 ******************************************************************************
 ** \brief Stall completion routine
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pu8Buffer data returned
 **
 ** \param u32Size size of data
 **
 ******************************************************************************/
static void StallCompletionEp0(stc_usbn_t* pstcUsb, uint8_t* pu8Buffer, uint32_t u32Size)
{
    stc_usbhost_intern_data_t* pstcUsbHostIntern;
    pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
    USBDBG("Resending last Setup\n");
    UsbHost_SetupRequest(pstcUsb, &pstcUsbHostIntern->stcLastSetupBeforeStall, pstcUsbHostIntern->SetupCompletionHandler);

}

/**
 ******************************************************************************
 ** \brief Stall completion routine
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ******************************************************************************/
static void UsbHost_SetupComplete(stc_usbn_t* pstcUsb)
{
    stc_usbhost_intern_data_t* pstcUsbHostIntern;
    pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
    if (pstcUsbHostIntern->SetupCompletionHandler != 0)
    {
        pstcUsbHostIntern->SetupCompletionHandler(pstcUsb,(pstcUsbHostIntern->Ep0IN->pu8Buffer),(pstcUsbHostIntern->Ep0IN->u32DataSize));
    }
}

/**
 ******************************************************************************
 ** \brief Connection Callback Routine
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ******************************************************************************/
void ConnectionCallback(stc_usbn_t* pstcUsb)
{
    stc_usbhost_intern_data_t* pstcUsbHostIntern;
    pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
    if (pstcUsbHostIntern->u32ConTimeout == 0)
    {
        if ((pstcUsb->HIRQ_f.URIRQ == 0) || (pstcUsbHostIntern->bBusResetSend == FALSE)) 
        {
            pstcUsb->UDCC_f.RST = 1;
            if (pstcUsb->HSTATE_f.TMODE == 1)  /* TMODE : Full-Speed */
            { 
                USBDBG("Full-Speed Device\n");
                pstcUsb->HSTATE_f.ALIVE = 0;
                pstcUsb->HSTATE_f.CLKSEL = 1;
                pstcUsbHostIntern->bFullSpeed = TRUE;
            }
            else /* Low-Speed */
            {       
                USBDBG("Low-Speed Device\n"); 
                pstcUsbHostIntern->bFullSpeed = FALSE;
                pstcUsb->HSTATE_f.ALIVE = 1;
                pstcUsb->HSTATE_f.CLKSEL = 0;
            }
            pstcUsb->UDCC_f.RST = 0;
        }
        USBDBG("\n### Device Connection - wait...\n");
        if (pstcUsb->HSTATE_f.TMODE == 1)  /* TMODE : Full-Speed */
        {
            pstcUsbHostIntern->u32ConTimeout = pstcUsbHostIntern->u32ConnectionDelay;
        }
        else
        {
            pstcUsbHostIntern->u32ConTimeout = pstcUsbHostIntern->u32ConnectionDelay;
        }
        return;
    }
    if (pstcUsbHostIntern->u32ConTimeout > 1)
    {
        pstcUsbHostIntern->u32ConTimeout = pstcUsbHostIntern->u32ConTimeout - 1;
        return;
    }
    USBDBG("\n### Device Connection...\n");
    pstcUsb->HIRQ_f.CNNIRQ = 0;
    if ((pstcUsb->HIRQ_f.URIRQ == 0) || (pstcUsbHostIntern->bBusResetSend == FALSE))
    {
        pstcUsbHostIntern->bBusResetSend = TRUE;          
        if (pstcUsb->HSTATE_f.CSTAT == 1)
        {
            USBDBG(">> Exec Bus Reset...\n");
            
            pstcUsb->HADR = 0;  // Device Address = 0
            
            pstcUsb->EP1S_f.BFINI = 1;
            pstcUsb->EP1S_f.BFINI = 0;
            pstcUsb->EP2S_f.BFINI = 1;
            pstcUsb->EP2S_f.BFINI = 0;
            
            pstcUsb->HCNT_f.URST = 1; // initiate bus reset
        }
    }
    else
    {
        USBDBG("    |->Bus Resetted\n");
        pstcUsb->HIRQ_f.URIRQ = 0;
        pstcUsbHostIntern->u32WaitForCompletionTimeout = 0;
        UsbHost_AddHostEndpoint(pstcUsb,&pstcUsbHostIntern->Ep0IN ,USBHOST_EPADDR0_IN,20,0,0);
        UsbHost_AddHostEndpoint(pstcUsb,&pstcUsbHostIntern->Ep0OUT,USBHOST_EPADDR0_OUT,20,0,0);
        pstcUsbHostIntern->Ep0IN->u32BufferSize = 255;
        pstcUsbHostIntern->Ep0OUT->u32BufferSize = 255;

        pstcUsbHostIntern->Ep0IN->pu8Buffer  = pstcUsbHostIntern->pu8GlobalBuffer;
        pstcUsbHostIntern->Ep0OUT->pu8Buffer = pstcUsbHostIntern->pu8GlobalBuffer;

        USBDBG(">> Starting SOF\n");
        pstcUsb->HFRAME = 0;
        pstcUsb->HEOF = 0x2c9; // Set the time where token are allowed in a frame
    
        pstcUsb->HCNT_f.SOFSTEP = 1; 
    
        pstcUsbHostIntern->Ep0OUT->bToggle = FALSE;
    
        
        pstcUsb->HTOKEN = CREATE_TOKEN(0,HTOKEN_SOF,0);
        /*SET_TOKEN_NOW(,HTOKEN_SOF);*/

        pstcUsbHostIntern->u8EnumerationCounter = 0;
        Usb_AddTimeOut(pstcUsb, EnumerationStateMachine,500);
    }
}

/**
 ******************************************************************************
 ** \brief Clear all endpoints
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ******************************************************************************/
void UsbHost_ClearAllEndpoints(stc_usbn_t* pstcUsb)
{
   uint8_t i;
   stc_usbhost_intern_data_t* pstcUsbHostIntern;
   pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
   pstcUsbHostIntern->u8EndpointListEnd = 0;
   pstcUsbHostIntern->u8CurrentEndpointPosition = 2;
   for(i=2;i<MAX_HOST_ENDPOINTS;i++)
   {
       memset((uint8_t*)&pstcUsbHostIntern->astcHostEndpoints[i],0,sizeof(pstcUsbHostIntern->astcHostEndpoints[i]));
   }
   pstcUsbHostIntern->astcHostEndpoints[0].bToggle = 0;
   pstcUsbHostIntern->astcHostEndpoints[1].bToggle = 0;
}

/**
 ******************************************************************************
 ** \brief Disconnection Callback Routine
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ******************************************************************************/
void UsbHost_DisconnectionCallback(stc_usbn_t* pstcUsb)
{
    stc_usbhost_intern_data_t* pstcUsbHostIntern;
    pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
    USBDBG("\n### Device Disconnection...\n");
    pstcUsbHostIntern->u8DeviceStatus = USBHOST_DEVICE_IDLE; 
    pstcUsbHostIntern->u32BusResetTimeout = 0;
    pstcUsbHostIntern->u32ConnectionDelay = USBHOST_CONNECTION_DELAY;
    pstcUsbHostIntern->u32ConTimeout = 0;
    pstcUsbHostIntern->bBusResetSend = FALSE;
    UsbClassSupervisor_Deinit(pstcUsb);
    UsbHost_ClearAllEndpoints(pstcUsb);
    pstcUsbHostIntern->bSofToken = FALSE;
    pstcUsbHostIntern->bSchedulerLock = FALSE;
    pstcUsbHostIntern->bBusResetSend = FALSE;
    pstcUsbHostIntern->u8DeviceAddress = 0;
    pstcUsbHostIntern->u8EnumerationCounter = 0;
    pstcUsbHostIntern->u32WaitForCompletionTimeout = 0;
    pstcUsbHostIntern->u8LastToken = 0;
    pstcUsbHostIntern->ActiveEndpoint = NULL;
    memset(&pstcUsbHostIntern->stcLastSetup,0,sizeof(pstcUsbHostIntern->stcLastSetup));
    memset(&pstcUsbHostIntern->stcLastSetupBeforeStall,0,sizeof(pstcUsbHostIntern->stcLastSetupBeforeStall));
    memset(pstcUsbHostIntern->pu8SetupData,0,8);
    pstcUsb->HIRQ_f.DIRQ = 0;   // be sure disconnection IRQ is released (should be already done in usb.c)
    pstcUsb->HIRQ_f.CNNIRQ = 0; // wanted here to be sure connection IRQ is not fired
}


/**
 ******************************************************************************
 ** \brief Bus Reset Callback Routine
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ******************************************************************************/
void UsbHost_BusresetCallback(stc_usbn_t* pstcUsb)
{

}

/**
 ******************************************************************************
 ** \brief Host Init Callback Routine
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param  pstcConfig USB Config
 **
 ******************************************************************************/
void UsbHost_InitCallback(stc_usbn_t* pstcUsb,struct stc_usb_config* pstcConfig)
{
    stc_usbhost_intern_data_t* pstcUsbHostIntern;
    pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
    pstcUsbHostIntern->bSofToken = FALSE;
    pstcUsbHostIntern->u32ConTimeout = 0;
    pstcUsbHostIntern->u32BusResetTimeout = 0;
    pstcUsbHostIntern->u8CurrentEndpointPosition = 2;
    pstcUsbHostIntern->u8EndpointListEnd = 0;
    pstcUsbHostIntern->bSchedulerLock = FALSE;
    pstcUsbHostIntern->bBusResetSend = FALSE;
    //pstcUsbHostIntern->SetupCompletionHandler = 0;
    pstcUsbHostIntern->u8DeviceStatus = USBHOST_DEVICE_IDLE;
    pstcUsbHostIntern->u8DeviceAddress = 0;
    pstcUsbHostIntern->u8EnumerationCounter = 0;
    pstcConfig->pfnCnnirqCallback = ConnectionCallback;
    pstcConfig->pfnDirqCallback = UsbHost_DisconnectionCallback;
    pstcConfig->pfnCmpirqCallback = UsbHost_CompletionCallback;
    pstcConfig->pfnSofCallback = UsbHost_SofCallback;
    pstcUsbHostIntern->u32ConnectionDelay = USBHOST_CONNECTION_DELAY;
    pstcUsbHostIntern->pstcUsbHalEndpointIN = Usb_GetEndpointPtr(pstcUsb,0x81);
    pstcUsbHostIntern->pstcUsbHalEndpointOUT = Usb_GetEndpointPtr(pstcUsb,0x02);
    
}

/**
 ******************************************************************************
 ** \brief Transfer OUT token data
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pu8Buffer Source buffer
 **
 ** \param u16Size data size
 **
 ** \param u16FifoSize FIFO data size
 **
 ******************************************************************************/
void UsbHost_TransferDataToFifo(stc_usbn_t* pstcUsb, uint8_t* pu8Buffer, uint16_t u16Size, uint16_t u16FifoSize)
{
    volatile uint32_t u32Timeout;
    boolean_t Odd = u16Size & 1;
    //stc_usbhost_intern_data_t* pstcUsbHostIntern;
    //pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
    USBDBGVAL8("TX: ",u16Size);
    USBDBGVAL32("#",pu8Buffer);
    u32Timeout = 10000;
    while((pstcUsb->EP2S_f.BUSY == 1) && (u32Timeout > 0))
    {
        u32Timeout--;
    }
    pstcUsb->EP2S_f.BFINI=1;
    pstcUsb->EP2S_f.BFINI=0;
    pstcUsb->EP2S = (pstcUsb->EP2S & (~0x1F)) | u16FifoSize;

    u16Size = u16Size / 2; // transfer 16 Bit words;
    while(u16Size--)	
    {
	    //EP2DT = *(uint16_t*)pu8Buffer;
	    pstcUsb->EP2DTL = *pu8Buffer++;
	    pstcUsb->EP2DTH = *pu8Buffer++;
	    //pu8Buffer += 2;
    }
    if(Odd == TRUE)  {
        pstcUsb->EP2DTL= *(uint8_t*)pu8Buffer; // transfer the first or last byte
    }
    pstcUsb->EP2S_f.DRQ = 0;	 // now the OUT FIFO is valid for the next transfer
}

/**
 ******************************************************************************
 ** \brief Transfer IN token data
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pu8Buffer Destination buffer
 **
 ** \param u32MaxBufferSize Maximal data to receive, all other data will be cleared
 **
 ** \return received size
 **
 ******************************************************************************/
uint16_t UsbHost_TransferFifoToBuffer(stc_usbn_t* pstcUsb, uint8_t* pu8Buffer, uint32_t u32MaxBufferSize)
{
    uint16_t u16Size = 0;
    uint16_t u16ReceivedSize;
    volatile uint32_t u32Timeout;
    volatile uint8_t u8Dummy = 0;
    u32Timeout = 10000;
    while((pstcUsb->EP1S_f.BUSY == 1) && (u32Timeout > 0))
    {
        u32Timeout--;
    }
    //stc_usbhost_intern_data_t* pstcUsbHostIntern;
    //pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
    u16Size = (uint16_t)(pstcUsb->EP1S & 0x00FF);
    u16ReceivedSize = u16Size;
    USBDBGVAL16("RX: ",u16Size);
    if (u16ReceivedSize > u32MaxBufferSize)
    {
        USBDBGVAL16("WOW... more data than expected, new data size: ",u32MaxBufferSize);
        u16Size = u32MaxBufferSize;
        u16ReceivedSize = u16Size;
    }
    USBDBGVAL32("#",pu8Buffer);
    u16Size = u16Size / 2; // transfer 16 Bit words;
    while(u16Size--)	
    {
	    //*(uint16_t*)pu8Buffer = EP1DT;
            if (pu8Buffer == 0)
            {
                u8Dummy = pstcUsb->EP1DTL;
                u8Dummy = pstcUsb->EP1DTH;
            }
            else
            {
	        *pu8Buffer++ = pstcUsb->EP1DTL;
	        *pu8Buffer++ = pstcUsb->EP1DTH;
            }
	    //pu8Buffer += 2;
    }
    if(u16ReceivedSize & 1 == TRUE)  {
        if (pu8Buffer == 0)
        {
            u8Dummy = pstcUsb->EP1DTL; // transfer the first or last byte
        }
        else
        {
            *(uint8_t*)pu8Buffer = pstcUsb->EP1DTL; // transfer the first or last byte
        }
    }	
    pstcUsb->EP1S_f.DRQ = 0;	 // now the IN FIFO is valid for the next transfer
    pstcUsb->EP1S_f.BFINI=1;
    pstcUsb->EP1S_f.BFINI=0;
    return u16ReceivedSize;
}

/**
 ******************************************************************************
 ** \brief Transfer data via endpoint
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param Handler Endpoint handle
 **
 ** \param pu8Buffer buffer
 **
 ** \param u32BufferSize size of data to be transferred
 **
 ** \param CompletionHandler callback handle
 **
 ******************************************************************************/
void UsbHost_TransferData(stc_usbn_t* pstcUsb, stc_host_endpoint_t* Handler,uint8_t* pu8Buffer, uint32_t u32BufferSize, void (* CompletionHandler)(stc_usbn_t* pstcUsb))
{
    stc_usbhost_intern_data_t* pstcUsbHostIntern;
    volatile boolean_t bOldSchedulerLockState;
    pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
    bOldSchedulerLockState = pstcUsbHostIntern->bSchedulerLock;
    pstcUsbHostIntern->bSchedulerLock = TRUE;
    DBGPROCENTER("UsbHost_TransferData");

    (Handler->u8Status) &= ~USBHOST_ENDPOINTSTATUS_INITIATE;
    if ((Handler->u8Interval) == 0)
    {
        (Handler->u8Status) |= USBHOST_ENDPOINTSTATUS_INUSE;
    }
    Handler->bAbortTransfer = FALSE;
    USBHOST_RESETENDPOINTBUFFER(Handler,pu8Buffer,u32BufferSize);
    
    (Handler->CompletionHandler) = CompletionHandler;
    (Handler->u8Status) &= ~USBHOST_ENDPOINTSTATUS_INUSE;

    USBDBGVAL32("DATASIZE: ",Handler->u32DataSize);

    if ((Handler->u8Interval) == 0)
    {
        (Handler->u8Status) |= USBHOST_ENDPOINTSTATUS_INITIATE;
    }
    
    DBGPROCRETURN("UsbHost_TransferData");
    pstcUsbHostIntern->bSchedulerLock = bOldSchedulerLockState;

    //UsbHost_Tick();
    //Handler->u8Status |= USBHOST_ENDPOINTSTATUS_DATA;
}

/**
 ******************************************************************************
 ** \brief Write host relevant initializations into pstcUsbConfig
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param pstcUsbConfig Write host relevant initializations into pstcUsbConfig
 **
 ******************************************************************************/
void UsbHost_Init(stc_usbn_t* pstcUsb, stc_usb_config_t* pstcUsbConfig)
{
    //stc_usbhost_intern_data_t* pstcUsbHostIntern;
    //pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
    
    pstcUsbConfig->enMode |= UsbHostEnabled;
    pstcUsbConfig->pfnHostInit = UsbHost_InitCallback;
    

}

/**
 ******************************************************************************
 ** \brief Get status of current enumerated device
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \return USB device status
 **         #USBHOST_DEVICE_IDLE          = 1,
 **         #USBHOST_DEVICE_ADDRESSING    = 2,
 **         #USBHOST_DEVICE_ADDRESSED     = 3,
 **         #USBHOST_DEVICE_ENUMERATED    = 4,
 **         #USBHOST_DEVICE_CONFIGURATING = 5,
 **         #USBHOST_DEVICE_CONFIGURED    = 6
 **
 ******************************************************************************/
uint8_t UsbHost_GetDeviceStatus(stc_usbn_t* pstcUsb)
{
    stc_usbhost_intern_data_t* pstcUsbHostIntern;
    pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
    return pstcUsbHostIntern->u8DeviceStatus;
}


/**
 ******************************************************************************
 ** \brief Set configuration
 **
 ** \param pstcUsb Pointer to USB instance
 **
 ** \param u8Configuration number of configuration
 **
 ******************************************************************************/
void UsbHost_SetConfigurationDescriptor(stc_usbn_t* pstcUsb, uint8_t u8Configuration)
{
    stc_usb_request_t stcSetup = {0x00,0x09,0x00,0x00,0x00};
    stc_usbhost_intern_data_t* pstcUsbHostIntern;
    pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
    stcSetup.wValue |= u8Configuration;
    USBDBGVAL8("Setting Configuration: ",u8Configuration);
    pstcUsbHostIntern->u8DeviceStatus = USBHOST_DEVICE_CONFIGURATING;
    UsbHost_SetupRequest(pstcUsb,&stcSetup,0);
}

/**
 ******************************************************************************
 ** \brief Get endpoint descriptor from buffer of configuration
 **
 ** \param pu8Buffer Buffer
 **
 ** \param u16Size Size of buffer
 **
 ** \param u8InterfaceNumber Interface number
 **
 ** \param u8EndpointNumber Number of endpoint
 **
 ** \return NULL, if endpoint was not found in the configuration, else pointer to endpoint descriptor (as buffer)
 **
 ******************************************************************************/
uint8_t* UsbHost_GetUsbEndpointDescriptor(uint8_t* pu8Buffer, uint16_t u16Size, uint8_t u8InterfaceNumber, uint8_t u8EndpointNumber)
{
    uint16_t u16Position = 0;
    uint8_t u8DescriptorLength = 0;
    uint8_t u8DescriptorType = 0;
    uint8_t u8CurrentInterface = 0xFF;
    uint8_t u8CurrentEndpoint = 0;
    uint16_t u16wTotalLength;
    

    if (pu8Buffer[1] != USB_CONFIGURATION_DESCRIPTOR_TYPE) 
    {
        return 0;
    }
    
    u16wTotalLength = (uint16_t)pu8Buffer[2] + (uint16_t)(pu8Buffer[3] << 8);
    if (u16Size < u16wTotalLength)
    {
       u16wTotalLength = u16Size;
    }
    
    while(u16Position < u16wTotalLength)
    {
       u8DescriptorLength = pu8Buffer[u16Position];
       u8DescriptorType = pu8Buffer[u16Position + 1];
       if (u8DescriptorType == USB_INTERFACE_DESCRIPTOR_TYPE)
       {
           u8CurrentEndpoint = 0;
           u8CurrentInterface = pu8Buffer[u16Position + 2];
       }
       if (u8DescriptorType == USB_ENDPOINT_DESCRIPTOR_TYPE)
       {
           u8CurrentEndpoint = u8CurrentEndpoint + 1;
       }
       if (((u8CurrentEndpoint == u8EndpointNumber) || (u8EndpointNumber == 0)) && (u8InterfaceNumber == u8CurrentInterface))
       {
           return (uint8_t*)(pu8Buffer + u16Position);
       }
       u16Position += u8DescriptorLength;
    }
    return 0;
}


typedef enum en_usbhost_scheduler_event
{
    UsbHostSchedulerEventUnknown = 0,
    UsbHostSchedulerEventSof = 1,
    UsbHostSchedulerEventCompletion = 2,
    UsbHostSchedulerEventTick = 3,
} en_usbhost_scheduler_event_t;

void UsbHost_SchedulerInitiateEndpoint(stc_usbn_t* pstcUsb, stc_usbhost_intern_data_t* pstcUsbHostIntern, stc_host_endpoint_t* pstcEndpoint)
{
    uint32_t u32DataSize = 0;
    uint8_t u8NextToken = 0;
    volatile uint32_t u32Timeout;
    DBGPROCENTER("UsbHost_SchedulerInitiateEndpoint");
    USBDBGVAL8("Init TKN, EP: ", pstcEndpoint->u8Address);
    pstcEndpoint->u8Status &= ~USBHOST_ENDPOINTSTATUS_INITIATE;
    pstcEndpoint->u8Status |= USBHOST_ENDPOINTSTATUS_INUSE;
    pstcEndpoint->u8IntervalCount = (pstcEndpoint->u8Interval);
    pstcUsbHostIntern->u32WaitForCompletionTimeout = 10;
    if (((pstcEndpoint->u8Address) & 0x0F) == 0)
    {
        pstcUsbHostIntern->u32WaitForCompletionTimeout = 10;
    }
    if ((pstcEndpoint->u8Address) & 0x80)
    {
       USBDBG("Type IN\n");
       if (pstcEndpoint->bIsochronous == TRUE)
       {
           u8NextToken = CREATE_TOKEN(FALSE,HTOKEN_ISO_IN,((pstcEndpoint->u8Address) & 0x0F));
       }
       else
       {
           u8NextToken = CREATE_TOKEN(pstcEndpoint->bToggle,HTOKEN_IN,((pstcEndpoint->u8Address) & 0x0F));
       }

    }
    else
    {
        //u32DataSize = (pstcEndpoint->u32DataSize);
        u32DataSize = (pstcEndpoint->u32DataSize);
        USBDBG("Type OUT\n");
        USBDBGVAL32("Data Size:", u32DataSize);
        u32DataSize -= (uint32_t)((pstcEndpoint->pu8BufferPos) - (pstcEndpoint->pu8Buffer));
        USBDBGVAL32("Data Size Left:", u32DataSize);
        if (u32DataSize > (pstcEndpoint->u16FifoSize))
        {
           u32DataSize = pstcEndpoint->u16FifoSize;
           USBDBGVAL32("Resizing new:", u32DataSize);
        }
        if (u32DataSize > 0)
        {
           pstcEndpoint->pu8BufferPosNextPackage += u32DataSize;
           UsbHost_TransferDataToFifo(pstcUsb,pstcEndpoint->pu8BufferPos, (uint16_t)u32DataSize, pstcEndpoint->u16FifoSize);
           u8NextToken = CREATE_TOKEN(pstcEndpoint->bToggle,HTOKEN_OUT,((pstcEndpoint->u8Address) & 0x0F));

        }
        else
        {
           USBDBG("Sending 0 byte\n");
           UsbHost_TransferDataToFifo(pstcUsb,pstcEndpoint->pu8BufferPos, (uint16_t)u32DataSize, pstcEndpoint->u16FifoSize);
           u8NextToken = CREATE_TOKEN(pstcEndpoint->bToggle,HTOKEN_OUT,((pstcEndpoint->u8Address) & 0x0F));
        }
    }
    
    if (pstcEndpoint->u8Stage == USBHOST_STAGE_SETUP)
    {
        u8NextToken = 0x10;
    }
    
    USBDBGVAL8("",u8NextToken);
    USBDBGVAL8("DATA",(u8NextToken & 0x80) > 0);
    pstcUsbHostIntern->bSofToken = FALSE;
    pstcUsbHostIntern->u8LastToken = u8NextToken;
    pstcUsbHostIntern->ActiveEndpoint = pstcEndpoint;
    if ((pstcUsbHostIntern->Ep0OUT == pstcEndpoint) || (pstcUsbHostIntern->Ep0IN == pstcEndpoint))
    {
        pstcUsbHostIntern->Ep0OUT->u8LastToken = u8NextToken;
        pstcUsbHostIntern->Ep0IN->u8LastToken = u8NextToken;
    }
    pstcEndpoint->u8LastToken = u8NextToken;
    pstcUsbHostIntern->u8SchedulerState = USBHOST_SCHEDULER_BUSY;
    pstcEndpoint->u8Status |= USBHOST_ENDPOINTSTATUS_INUSE;
    u32Timeout = 10000;
    while(((pstcUsb->EP1S_f.BUSY == 1) || (pstcUsb->EP2S_f.BUSY == 1)) && (u32Timeout > 0))
    {
        u32Timeout--;
    }
    pstcUsb->HTOKEN = u8NextToken;
    DBGPROCRETURN("UsbHost_SchedulerInitiateEndpoint");
}

void UsbHost_Scheduler(stc_usbn_t* pstcUsb, en_usbhost_scheduler_event_t enEvent)
{
    uint8_t i;
    static volatile boolean_t bSchedulerIsRunning = FALSE;
    stc_usbhost_intern_data_t* pstcUsbHostIntern; 
    if (bSchedulerIsRunning == TRUE) return;
    pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
    
    if (enEvent == UsbHostSchedulerEventCompletion)
    {
    }
    
    if (enEvent == UsbHostSchedulerEventSof)
    {    
        if (pstcUsbHostIntern->u32WaitForCompletionTimeout > 0)
        {
            pstcUsbHostIntern->u32WaitForCompletionTimeout--;
        }

        for(i = 0;i <= pstcUsbHostIntern->u8EndpointListEnd;i++)
        {
            if ((pstcUsbHostIntern->astcHostEndpoints[i].u8Interval > 0) && (pstcUsbHostIntern->astcHostEndpoints[i].u8IntervalCount > 0))
            {
                pstcUsbHostIntern->astcHostEndpoints[i].u8IntervalCount--;
            }
        }
    }

    if (pstcUsbHostIntern->bSchedulerLock == TRUE)
    {
        bSchedulerIsRunning = FALSE;
        return;
    }
    
    if (pstcUsbHostIntern->u32WaitForCompletionTimeout > 0)
    {
        bSchedulerIsRunning = FALSE;
        return;
    }

    if ((pstcUsbHostIntern->Ep0IN->bAbortTransfer == TRUE) || (pstcUsbHostIntern->Ep0OUT->bAbortTransfer == TRUE))
    {
       USBDBG("-> EP0 CANCELED\n");
       pstcUsbHostIntern->Ep0IN->u8Status &= ~USBHOST_ENDPOINTSTATUS_INUSE;
       pstcUsbHostIntern->Ep0OUT->u8Status &= ~USBHOST_ENDPOINTSTATUS_INUSE;
       pstcUsbHostIntern->Ep0IN->bAbortTransfer = FALSE;
       pstcUsbHostIntern->Ep0OUT->bAbortTransfer = FALSE;
       pstcUsbHostIntern->Ep0IN->u8Stage = 0;
       pstcUsbHostIntern->Ep0OUT->u8Stage = 0;
       pstcUsbHostIntern->u32ConTimeout = 0;
       
    }
    
    // Process control transfers
    if (((pstcUsbHostIntern->Ep0IN->u8Status) & USBHOST_ENDPOINTSTATUS_INITIATE) > 0)
    {
        UsbHost_SchedulerInitiateEndpoint(pstcUsb,pstcUsbHostIntern,pstcUsbHostIntern->Ep0IN);
        bSchedulerIsRunning = FALSE;
        return;
    }
    
    if (((pstcUsbHostIntern->Ep0OUT->u8Status) & USBHOST_ENDPOINTSTATUS_INITIATE) > 0)
    {
        UsbHost_SchedulerInitiateEndpoint(pstcUsb,pstcUsbHostIntern,pstcUsbHostIntern->Ep0OUT);
        bSchedulerIsRunning = FALSE;
        return;
    }
    
    if (pstcUsbHostIntern->u8DeviceStatus != USBHOST_DEVICE_CONFIGURED)
    {
        bSchedulerIsRunning = FALSE;
        return;
    }
    
    // Process interrupt transfers
    for(i = 0;i <= pstcUsbHostIntern->u8EndpointListEnd;i++)
    {
        pstcUsbHostIntern->u8CurrentEndpointPosition++;
        if (pstcUsbHostIntern->u8CurrentEndpointPosition > pstcUsbHostIntern->u8EndpointListEnd)
        {
            pstcUsbHostIntern->u8CurrentEndpointPosition = 0;
        }
        if ((pstcUsbHostIntern->astcHostEndpoints[pstcUsbHostIntern->u8CurrentEndpointPosition].u8Interval > 0) && (pstcUsbHostIntern->astcHostEndpoints[pstcUsbHostIntern->u8CurrentEndpointPosition].u8IntervalCount == 0))
        {
            // Init Interrupt Transfer
            pstcUsbHostIntern->astcHostEndpoints[pstcUsbHostIntern->u8CurrentEndpointPosition].pu8BufferPos = (pstcUsbHostIntern->astcHostEndpoints[pstcUsbHostIntern->u8CurrentEndpointPosition].pu8Buffer);
            pstcUsbHostIntern->astcHostEndpoints[pstcUsbHostIntern->u8CurrentEndpointPosition].pu8BufferPosNextPackage = (pstcUsbHostIntern->astcHostEndpoints[pstcUsbHostIntern->u8CurrentEndpointPosition].pu8Buffer);
            UsbHost_SchedulerInitiateEndpoint(pstcUsb,pstcUsbHostIntern,&(pstcUsbHostIntern->astcHostEndpoints[pstcUsbHostIntern->u8CurrentEndpointPosition]));
            bSchedulerIsRunning = FALSE;
            return;
        }
    }
        
    // Process other transfers than interrupt
    for(i = 0;i <= pstcUsbHostIntern->u8EndpointListEnd;i++)
    {
        pstcUsbHostIntern->u8CurrentEndpointPosition++;
        if (pstcUsbHostIntern->u8CurrentEndpointPosition > pstcUsbHostIntern->u8EndpointListEnd)
        {
            pstcUsbHostIntern->u8CurrentEndpointPosition = 0;
        }
        if (((pstcUsbHostIntern->astcHostEndpoints[pstcUsbHostIntern->u8CurrentEndpointPosition].u8Status) & USBHOST_ENDPOINTSTATUS_INITIATE) > 0)
        {
            // Init Transfer
            UsbHost_SchedulerInitiateEndpoint(pstcUsb,pstcUsbHostIntern,&(pstcUsbHostIntern->astcHostEndpoints[pstcUsbHostIntern->u8CurrentEndpointPosition]));
            bSchedulerIsRunning = FALSE;
            return;
        }
    }
}

void UsbHost_CompletionCallback(stc_usbn_t* pstcUsb)
{
    uint32_t tmp;
    uint32_t u32DataSize = 0;
    volatile boolean_t bOldSchedulerLockState;
    stc_host_endpoint_t* pstcEndpointHandle = 0;
    stc_usbhost_intern_data_t* pstcUsbHostIntern;
    pstcUsbHostIntern = UsbHostGetInternDataPtr(pstcUsb);
    pstcEndpointHandle = pstcUsbHostIntern->ActiveEndpoint;
    DBGPROCENTER("UsbHost_CompletionCallback");
    bOldSchedulerLockState = pstcUsbHostIntern->bSchedulerLock;
    pstcUsbHostIntern->bSchedulerLock = TRUE;

    
    USBDBGVAL8("EP: ",pstcEndpointHandle->u8Address);
    
    USBDBGVAL8("EP: ",pstcEndpointHandle->u8Address);
       
    if (pstcEndpointHandle == 0)
    {
      USBDBG("WARNING: pstcEndpointHandle == 0\n");
      USBDBG("\n< UsbHost_CompletionCallback\n");
      pstcUsbHostIntern->bSchedulerLock = bOldSchedulerLockState;
      return;
    }
    pstcEndpointHandle->u8Status &= ~USBHOST_ENDPOINTSTATUS_INITIATE;
    
    if (pstcEndpointHandle->bAbortTransfer == TRUE)
    {
       USBDBG("-> CANCELED\n");
       pstcEndpointHandle->u32DataSize = 0;
       pstcEndpointHandle->u8Status &= ~USBHOST_ENDPOINTSTATUS_INUSE;
       if ((pstcUsbHostIntern->Ep0OUT == pstcEndpointHandle) || (pstcUsbHostIntern->Ep0IN == pstcEndpointHandle))
       {
           pstcUsbHostIntern->Ep0OUT->u8Status &= ~USBHOST_ENDPOINTSTATUS_INUSE;
           pstcUsbHostIntern->Ep0IN->u8Status &= ~USBHOST_ENDPOINTSTATUS_INUSE;
       }
       pstcEndpointHandle->bAbortTransfer = FALSE;
       pstcUsbHostIntern->ActiveEndpoint = 0;
       DBGPROCRETURN("UsbHost_CompletionCallback");
       pstcUsbHostIntern->bSchedulerLock = bOldSchedulerLockState;
       return;
    }
    USBDBGVAL8("HERR: ",pstcUsb->HERR);
    USBDBGVAL8("TKN: ",(pstcEndpointHandle->u8LastToken));
    if ((pstcUsb->HERR & 0x03) == HERR_ACK)
    {
        USBDBG("ACK\n");
        
        if (pstcEndpointHandle->bToggle == TRUE)
        {
            USBDBG("TGL 1 -> 0\n");
            pstcEndpointHandle->bToggle = FALSE;
            if ((pstcUsbHostIntern->Ep0OUT == pstcEndpointHandle) || (pstcUsbHostIntern->Ep0IN == pstcEndpointHandle))
            {
                pstcUsbHostIntern->Ep0OUT->bToggle = FALSE;
                pstcUsbHostIntern->Ep0IN->bToggle = FALSE;
            }
        }
        else
        {
            USBDBG("TGL 0 -> 1\n");
            pstcEndpointHandle->bToggle = TRUE;
            if ((pstcUsbHostIntern->Ep0OUT == pstcEndpointHandle) || (pstcUsbHostIntern->Ep0IN == pstcEndpointHandle))
            {
                pstcUsbHostIntern->Ep0OUT->bToggle = TRUE;
                pstcUsbHostIntern->Ep0IN->bToggle = TRUE;
            }
        }
                    

        if (((pstcEndpointHandle->u8Address) & 0x0F) == 0) // handler == endpoint 0?
        {
           if (((pstcEndpointHandle->u8LastToken) & 0x70) == HTOKEN_SETUP) // type == SETUP token?
           {
               USBDBG("SETUP\n");
               if (((pstcUsbHostIntern->stcLastSetup.bmRequestType) & 0x80) || (pstcUsbHostIntern->stcLastSetup.wLength == 0))
               {
                   USBDBG("TYPE IN\n");
                   pstcEndpointHandle = pstcUsbHostIntern->Ep0IN;
               }
               else
               {
                   USBDBG("TYPE OUT\n");
                   pstcEndpointHandle = pstcUsbHostIntern->Ep0OUT;
               }
               pstcUsbHostIntern->Ep0IN->bToggle = TRUE;
               pstcUsbHostIntern->Ep0OUT->bToggle = TRUE;
               pstcEndpointHandle->pu8BufferPos = (pstcEndpointHandle->pu8Buffer);
               pstcEndpointHandle->u32DataSize = 0;
               if ((pstcUsbHostIntern->stcLastSetup.wLength == 0)) // || ((stcLastSetup.wLength <= (Ep0IN->u16FifoSize)) && (Handler == Ep0IN)))
               {
                   USBDBG("NO DATA -> ENTERING STATUS STAGE\n");
                   pstcUsbHostIntern->Ep0OUT->u8Stage = USBHOST_STAGE_STATUS;
                   pstcUsbHostIntern->Ep0IN->u8Stage = USBHOST_STAGE_STATUS;
               }
               else
               { 
                   USBDBG("DATA -> ENTERING DATA STAGE\n");
                   pstcUsbHostIntern->Ep0OUT->u8Stage = USBHOST_STAGE_DATA;
                   pstcUsbHostIntern->Ep0IN->u8Stage = USBHOST_STAGE_DATA;
               }
               pstcEndpointHandle->u32DataSize = pstcUsbHostIntern->stcLastSetup.wLength;
               if (((pstcUsbHostIntern->stcLastSetup.bmRequestType) & 0x80) || (pstcUsbHostIntern->stcLastSetup.wLength == 0))
               {
                   pstcEndpointHandle->u8Status |= USBHOST_ENDPOINTSTATUS_INITIATE;
               }
               else
               {
                   pstcEndpointHandle->pu8BufferPosNextPackage = pstcUsbHostIntern->pu8SetupData;
                   pstcEndpointHandle->pu8BufferPos = pstcUsbHostIntern->pu8SetupData;
                   pstcEndpointHandle->pu8Buffer = pstcUsbHostIntern->pu8SetupData;
                   pstcEndpointHandle->u32BufferSize = pstcUsbHostIntern->stcLastSetup.wLength;
                   pstcEndpointHandle->u8Status |= USBHOST_ENDPOINTSTATUS_INITIATE;
               }
               DBGPROCRETURN("UsbHost_CompletionCallback");
               pstcUsbHostIntern->bSchedulerLock = bOldSchedulerLockState;
               return;
           } // endif (((pstcEndpointHandle->u8LastToken) & 0x70) == HTOKEN_SETUP) --> type == SETUP token?
           else
           {
               if (pstcUsbHostIntern->u8DeviceStatus == USBHOST_DEVICE_ADDRESSING)
               {
                   pstcUsbHostIntern->u8EnumerationCounter = 0;
                   pstcUsb->HADR = pstcUsbHostIntern->u8DeviceAddress;
                   USBDBG("Enumeration Process:\n");
                   USBDBGVAL8("  ->Addressed:  ",pstcUsbHostIntern->u8DeviceAddress);
                   pstcUsbHostIntern->u8DeviceStatus = USBHOST_DEVICE_ADDRESSED;
                   //UsbClassSupervisor_Init(pstcUsb);
               }
               if (pstcUsbHostIntern->u8DeviceStatus == USBHOST_DEVICE_CONFIGURATING)
               {
                   USBDBG("-->Configured\n");
                   pstcUsbHostIntern->u8DeviceStatus = USBHOST_DEVICE_CONFIGURED;
               }
           }
        } // endif (((pstcEndpointHandle->u8Address) & 0x0F) == 0) --> handler == endpoint 0?

        if ((((pstcEndpointHandle->u8LastToken) & 0x70) != HTOKEN_SETUP))
        {
            USBDBG("NO SETUP\n");
            pstcEndpointHandle->u8Status &= ~USBHOST_ENDPOINTSTATUS_INITIATE;
            if ((((pstcEndpointHandle->u8LastToken) & 0x70) == HTOKEN_IN) || (((pstcEndpointHandle->u8LastToken) & 0x70) == HTOKEN_ISO_IN))
            {
                USBDBG("IN TKN\n");
                tmp = (pstcEndpointHandle->u32DataSize) - ((uint32_t)((uint32_t)(pstcEndpointHandle->pu8BufferPos) - (uint32_t)(pstcEndpointHandle->pu8Buffer)));
                USBDBGVAL32("Max free in buffer: ",(pstcEndpointHandle->u32DataSize));
                u32DataSize = (uint32_t)((uint16_t)UsbHost_TransferFifoToBuffer(pstcUsb,pstcEndpointHandle->pu8BufferPos,tmp));
                (pstcEndpointHandle->pu8BufferPos) = (pstcEndpointHandle->pu8BufferPos) + u32DataSize;
                tmp = ((uint32_t)((uint32_t)(pstcEndpointHandle->pu8BufferPos) - (uint32_t)(pstcEndpointHandle->pu8Buffer)));
                USBDBGVAL8("EP#",(uint32_t)(pstcEndpointHandle->u8Address));
                USBDBGVAL32("#",(uint32_t)(pstcEndpointHandle->pu8Buffer));
                USBDBGVAL32("Received: ", u32DataSize);
                USBDBGVAL32("New Size: ", tmp);
                USBDBGVAL32("Max Size: ",(pstcEndpointHandle->u32DataSize));
                if (((u32DataSize == (pstcEndpointHandle->u16FifoSize)) && ((pstcEndpointHandle->u32DataSize) == 0)) || ((u32DataSize == (pstcEndpointHandle->u16FifoSize) && (pstcEndpointHandle->u32DataSize) > 0) && ((pstcEndpointHandle->u32DataSize) > tmp)))
                {
                    pstcEndpointHandle->u8Status |= USBHOST_ENDPOINTSTATUS_INITIATE;
                }
                else
                { 
                    pstcEndpointHandle->u32DataSize = (uint32_t)((uint32_t)(pstcEndpointHandle->pu8BufferPos) - (uint32_t)(pstcEndpointHandle->pu8Buffer));
                    USBDBGVAL32("RX-Size: ", (pstcEndpointHandle->u32DataSize));
                    pstcEndpointHandle->pu8BufferPos = (pstcEndpointHandle->pu8Buffer);
                    pstcEndpointHandle->u8Status |= USBHOST_ENDPOINTSTATUS_DATA;
                    pstcEndpointHandle->u8Status &= ~USBHOST_ENDPOINTSTATUS_INUSE;
                    if ((pstcUsbHostIntern->Ep0OUT == pstcEndpointHandle) || (pstcUsbHostIntern->Ep0IN == pstcEndpointHandle))
                    {
                        pstcUsbHostIntern->Ep0OUT->u8Status &= ~USBHOST_ENDPOINTSTATUS_INUSE;
                        pstcUsbHostIntern->Ep0IN->u8Status &= ~USBHOST_ENDPOINTSTATUS_INUSE;
                    }
                    if ((((pstcEndpointHandle->u8Address) & 0x0F) == 0) && ((pstcEndpointHandle->u8Stage) == USBHOST_STAGE_DATA))
                    {
                        pstcUsbHostIntern->Ep0IN->u8Stage = USBHOST_STAGE_STATUS;
                        pstcUsbHostIntern->Ep0OUT->u8Stage = USBHOST_STAGE_STATUS;
                        pstcUsbHostIntern->Ep0OUT->bToggle = TRUE;
                        pstcUsbHostIntern->Ep0IN->bToggle = TRUE;
                        pstcUsbHostIntern->Ep0OUT->u32DataSize = 0;
                        pstcUsbHostIntern->Ep0OUT->pu8BufferPos = (pstcUsbHostIntern->Ep0OUT->pu8Buffer);
                        pstcUsbHostIntern->Ep0OUT->pu8BufferPosNextPackage = (pstcUsbHostIntern->Ep0OUT->pu8BufferPos);
                        pstcUsbHostIntern->Ep0OUT->CompletionHandler = pstcUsbHostIntern->Ep0IN->CompletionHandler;
                        pstcUsbHostIntern->Ep0IN->u8Status &= ~USBHOST_ENDPOINTSTATUS_INITIATE;
                        pstcUsbHostIntern->Ep0OUT->u8Status |= USBHOST_ENDPOINTSTATUS_INITIATE;
                    }
                    else
                    {
                        USBDBG("TRANS COMPLETE\n");
                        pstcUsbHostIntern->u32WaitForCompletionTimeout = 0; 
                        pstcEndpointHandle->u8Status &= ~USBHOST_ENDPOINTSTATUS_INITIATE;
                        if ((pstcEndpointHandle->CompletionHandler) != 0)
                        {
                           USBDBG("Exec(IN)\n");
                           pstcEndpointHandle->CompletionHandler(pstcUsb);
                        }
                        pstcUsbHostIntern->ActiveEndpoint = 0;
                    }
                }

            }
            if ((((pstcEndpointHandle->u8LastToken) & 0x70) == HTOKEN_OUT) || (((pstcEndpointHandle->u8LastToken) & 0x70) == HTOKEN_ISO_OUT))
            {
                 pstcEndpointHandle->pu8BufferPos = (pstcEndpointHandle->pu8BufferPosNextPackage);
                 
                USBDBG("OUT TKN\n");
                USBDBGVAL8("EP#",(uint32_t)(pstcEndpointHandle->u8Address));
                USBDBGVAL32("#",(uint32_t)(pstcEndpointHandle->pu8Buffer));
                USBDBGVAL32("Transferred: ",(uint32_t)((pstcEndpointHandle->pu8BufferPos) - (pstcEndpointHandle->pu8Buffer)));
                u32DataSize = (pstcEndpointHandle->u32DataSize);
                
                USBDBGVAL32("Datalen old: ",u32DataSize);
               
                u32DataSize -= (uint32_t)((pstcEndpointHandle->pu8BufferPos) - (pstcEndpointHandle->pu8Buffer));
                
                USBDBGVAL32("Datalen new: ",u32DataSize);
                
                pstcEndpointHandle->u8Status &= ~USBHOST_ENDPOINTSTATUS_INITIATE;
                if (u32DataSize > 0)
                {
                    pstcEndpointHandle->u8Status |= USBHOST_ENDPOINTSTATUS_INITIATE;
                }
                else
                {
                    USBDBG("SIZE == 0\n");
                    pstcEndpointHandle->u8Status &= ~USBHOST_ENDPOINTSTATUS_INUSE;
                    if ((pstcUsbHostIntern->Ep0OUT == pstcEndpointHandle) || (pstcUsbHostIntern->Ep0IN == pstcEndpointHandle))
                    {
                        pstcUsbHostIntern->Ep0OUT->u8Status &= ~USBHOST_ENDPOINTSTATUS_INUSE;
                        pstcUsbHostIntern->Ep0IN->u8Status &= ~USBHOST_ENDPOINTSTATUS_INUSE;
                    }
                    if ((((pstcEndpointHandle->u8Address) & 0x0F) == 0) && ((pstcEndpointHandle->u8Stage) == USBHOST_STAGE_DATA))
                    {
                        USBDBG("DATA STAGE -> STATUS STAGE\n");
                        pstcUsbHostIntern->Ep0IN->u8Stage = USBHOST_STAGE_STATUS;
                        pstcUsbHostIntern->Ep0OUT->u8Stage = USBHOST_STAGE_STATUS;
                        pstcUsbHostIntern->Ep0IN->CompletionHandler = pstcUsbHostIntern->Ep0OUT->CompletionHandler;
                        pstcUsbHostIntern->Ep0IN->bToggle = TRUE;
                        pstcUsbHostIntern->Ep0OUT->bToggle = TRUE;
                        pstcUsbHostIntern->Ep0IN->u32DataSize = 0;
                        pstcUsbHostIntern->Ep0IN->pu8BufferPos = (pstcUsbHostIntern->Ep0IN->pu8Buffer);
                        pstcUsbHostIntern->Ep0IN->pu8BufferPosNextPackage = (pstcUsbHostIntern->Ep0IN->pu8BufferPos);
                        pstcUsbHostIntern->Ep0OUT->u8Status &= ~USBHOST_ENDPOINTSTATUS_INITIATE;
                        pstcUsbHostIntern->Ep0IN->u8Status |= USBHOST_ENDPOINTSTATUS_INITIATE;
                    }
                    else
                    {
                        USBDBG("TRANS COMPLETE\n");
                        pstcUsbHostIntern->u32WaitForCompletionTimeout = 0; 
                        if (pstcEndpointHandle->u8Interval != 0)
                        {
                            pstcEndpointHandle->u32DataSize = 0;
                        }
                        pstcEndpointHandle->u8Status &= ~USBHOST_ENDPOINTSTATUS_INITIATE;
                        if ((pstcEndpointHandle->CompletionHandler) != 0)
                        {
                            pstcEndpointHandle->CompletionHandler(pstcUsb);
                        }
                        
                    }
                    pstcUsbHostIntern->ActiveEndpoint = 0;
                }
            }
            
        } // endif ((((pstcEndpointHandle->u8LastToken) & 0x70) != HTOKEN_SETUP))
        
    } // endif ((pstcUsb->HERR & 0x03) == HERR_ACK)
    
    if ((pstcUsb->HERR & 0x08) > 0)
    {
       USBDBG("Toggle Error\n");
       pstcUsb->HERR_f.TGERR = 0;
       //pstcEndpointHandle->bToggle = ~pstcEndpointHandle->bToggle;
    } // endif ((pstcUsb->HERR & 0x08) > 0)
    
    if ((pstcUsb->HERR & 0x08) > 0)
    {
       USBDBG("Toggle Error\n");
       pstcUsb->HERR_f.TGERR = 0;
       //pstcEndpointHandle->bToggle = ~pstcEndpointHandle->bToggle;
    } // endif ((pstcUsb->HERR & 0x08) > 0)
    
    if ((pstcUsb->HERR & 0x20) > 0)
    {
       USBDBG("NULL\n");
       USBDBGVAL8("HERR: ",pstcUsb->HERR);
       pstcUsb->HERR_f.TOUT = 0;
    } // endif ((pstcUsb->HERR & 0x20) > 0)

    if (((pstcUsb->HERR & 0x03) == HERR_NAK) || ((pstcUsb->HERR & 0x03) == HERR_NULL))
    {
        pstcUsbHostIntern->u32WaitForCompletionTimeout = 0; 
        USBDBG("NAK\n");
        if (((pstcEndpointHandle->u8Interval) > 0) && ((pstcEndpointHandle->pu8Buffer) == (pstcEndpointHandle->pu8BufferPos)))
        {
           USBDBG("Cancel\n");
           pstcEndpointHandle->u8Status &= ~USBHOST_ENDPOINTSTATUS_INUSE;
        }
        else
        {   
           USBDBG("RTY\n");
           //UsbHost_TransferFifoToBuffer(pstcUsb,(uint8_t*)0);
           pstcEndpointHandle->pu8BufferPosNextPackage = pstcEndpointHandle->pu8BufferPos;
           if (pstcEndpointHandle->bAbortTransfer == TRUE)
           {
               USBDBG("-> CANCELED\n");
               pstcEndpointHandle->u32DataSize = 0;
               pstcEndpointHandle->u8Status &= ~USBHOST_ENDPOINTSTATUS_INUSE;
               if ((pstcUsbHostIntern->Ep0OUT == pstcEndpointHandle) || (pstcUsbHostIntern->Ep0IN == pstcEndpointHandle))
               {
                   pstcUsbHostIntern->Ep0OUT->u8Status &= ~USBHOST_ENDPOINTSTATUS_INUSE;
                   pstcUsbHostIntern->Ep0IN->u8Status &= ~USBHOST_ENDPOINTSTATUS_INUSE;
               }
               pstcEndpointHandle->bAbortTransfer = FALSE;
               pstcUsbHostIntern->ActiveEndpoint = 0;
           }
           else
           {
               pstcEndpointHandle->u8Status |= USBHOST_ENDPOINTSTATUS_INITIATE;
           }
        }
    } // endif ((pstcUsb->HERR & 0x03) == HERR_NAK)
    

    
    if ((pstcUsb->HERR & 0x03) == HERR_STALL)
    {
       USBDBG("STALL\n");
       pstcUsbHostIntern->u32WaitForCompletionTimeout = 0; 
       pstcEndpointHandle = pstcUsbHostIntern->ActiveEndpoint;
       pstcEndpointHandle->u8Status |= USBHOST_ENDPOINTSTATUS_STALL;
       if (((pstcEndpointHandle->u8Address) & 0x0F) == 0)
       {
           USBDBG("Clearing STALL EP0\n");
           pstcUsbHostIntern->StallCompletionHandler = pstcEndpointHandle->CompletionHandler;
           memcpy((uint8_t*)&pstcUsbHostIntern->stcLastSetupBeforeStall,(uint8_t*)&pstcUsbHostIntern->stcLastSetup,sizeof(pstcUsbHostIntern->stcLastSetup));
           pstcUsbHostIntern->Ep0IN->u8Status &= ~USBHOST_ENDPOINTSTATUS_INUSE;
           pstcUsbHostIntern->Ep0OUT->u8Status &= ~USBHOST_ENDPOINTSTATUS_INUSE;
           UsbHost_SetupRequest(pstcUsb,(stc_usb_request_t*)&stcClearStallEp0,StallCompletionEp0);
       }
       else
       {
           if ((pstcEndpointHandle->CompletionHandler) != 0)
           {
               pstcEndpointHandle->CompletionHandler(pstcUsb);
           }
       }
    } // endif ((pstcUsb->HERR & 0x03) == HERR_STALL)
    DBGPROCRETURN("UsbHost_CompletionCallback");
    pstcUsbHostIntern->bSchedulerLock = bOldSchedulerLockState;
    pstcUsbHostIntern->u32WaitForCompletionTimeout = 0;
    UsbHost_Scheduler(pstcUsb, UsbHostSchedulerEventCompletion);
}

void UsbHost_SofCallback(stc_usbn_t* pstcUsb)
{
    UsbHost_Scheduler(pstcUsb, UsbHostSchedulerEventSof);
}

void UsbHost_Tick(void)
{
    #if (FM_PERIPHERAL_ENABLE_USB0 == ON) && defined(USB0)
        UsbHost_Scheduler((stc_usbn_t*)&USB0,UsbHostSchedulerEventTick);
    #endif
    #if (FM_PERIPHERAL_ENABLE_USB1 == ON) && defined(USB1)
        UsbHost_Scheduler((stc_usbn_t*)&USB1, UsbHostSchedulerEventTick);
    #endif
}

//@} // UsbHostGroup

#endif
