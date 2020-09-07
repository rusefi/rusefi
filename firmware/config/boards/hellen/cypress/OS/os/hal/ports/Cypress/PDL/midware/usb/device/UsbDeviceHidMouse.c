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
/** \file UsbDeviceHidMouse.c
 **
 ** CONST USB_ASSISTANT_OFF
 **
 ** A detailed description is available at 
 ** @link UsbDeviceHidMouseGroup USB Device Mouse Module description @endlink
 **
 ** History:
 **   - 2013-10-14  1.1  MSc  PDL support added 
 *****************************************************************************/

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "UsbDeviceHidMouse.h"

#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) && (USBDEVICEHIDMOUSE_ENABLED == ON))

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

static uint8_t DataSentFlags = 0; //Status for sent data
static uint8_t pu8MouseData[4];
static stc_usbn_endpoint_data_t* pstcEndpointIN;
static stc_usbn_endpoint_buffer_t stcEndpointBufferIN;
static stc_usbn_t* pstcUsbHandle = NULL;
static boolean_t bReady = FALSE;






/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

static void ConfCallback(stc_usbn_t* pstcUsb);

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

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

void UsbDeviceHidMouse_Init(stc_usbn_t* pstcUsb)
{
    stc_usbdevice_class_config_t stcUsbClassConfig;
    stc_usbdevice_endpoint_config_t stcEndpointConfig;
    uint8_t* pu8Interface = NULL;
    uint8_t u8InterfaceLength = 0;
    uint8_t u8i = 0;
    uint8_t u8NumEndpoints = 0;
    pstcUsbHandle = pstcUsb;
    
    stcUsbClassConfig.u8InterfaceNumber          = 0xFF;                          // do not use fix interface number, choose by class, subclass, protocol
    stcUsbClassConfig.u8InterfaceClass           = 0x03;                          // HID Class
    stcUsbClassConfig.u8InterfaceSubClass        = 0x01;                          // Custom Sub Class
    stcUsbClassConfig.u8InterfaceProtocoll       = 0x02;                          // Custom Protocol
    stcUsbClassConfig.pfnSetupRequestCallback    = NULL;                          // setup requests handled
    stcUsbClassConfig.pfnConnectionCallback      = NULL;                          // No connection callback handled
    stcUsbClassConfig.pfnDisconnectionCallback   = NULL;                          // No disconnection callback handled
    stcUsbClassConfig.pfnConfCallback            = ConfCallback;                  // Callback for configuration set
    UsbDevice_RegisterVendorClass(pstcUsb,&stcUsbClassConfig,&pu8Interface,&u8InterfaceLength);
        
    for(u8i = 0;u8i < u8InterfaceLength;)
    {
        u8i += pu8Interface[u8i];
        if (USBDESCR_ENDPOINT == pu8Interface[u8i + 1])
        {
            stcEndpointConfig.u8EndpointAddress = pu8Interface[u8i + 2];
            if (((stcEndpointConfig.u8EndpointAddress) & 0x80) == 0)
            {
                /* NOT USED */
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
 ** Is called from main application to send data via endpoint
 **
 ** \param pu8Buffer Buffer to send
 ** \param u32DataSize Buffersize
 ** \param enMode    polled sending, interrupt sending or DMA
 **
 ** \return 1: if succesful, 0: if usb was not ready
 *****************************************************************************/
static boolean_t SendData(uint8_t* pu8Buffer, uint32_t u32DataSize, en_usbsend_mode_t enMode)
{
    if (UsbDevice_GetStatus(pstcUsbHandle) == UsbConfigured)
    {
        DataSentFlags &= ~(1<<1);
		
	    UsbDevice_SendData(pstcUsbHandle, pstcEndpointIN, pu8Buffer, u32DataSize, enMode);
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
 ** Move Mouse
 **
 ** \param i8X Move in X direction 
 **
 ** \param i8Y Move in Y direction 
 *****************************************************************************/
void UsbDeviceHidMouse_Move(int8_t i8X, int8_t i8Y)
{
  pu8MouseData[1] = (int8_t)i8X;
  pu8MouseData[2] = (int8_t)i8Y;
  SendData((uint8_t*)pu8MouseData,4,UsbPOLL);
}


/**
 ******************************************************************************
 ** Mouse Left Click (Up-Down-Up)
 **
 *****************************************************************************/
void UsbDeviceHidMouse_LeftClick(void)
{
  UsbDeviceHidMouse_LeftUp();
  UsbDeviceHidMouse_LeftDown();
  UsbDeviceHidMouse_LeftUp();
}


/**
 ******************************************************************************
 ** Mouse Left Click (Up-Down-Up-Down-Up)
 **
 *****************************************************************************/
void UsbDeviceHidMouse_LeftDoubleClick(void)
{
  UsbDeviceHidMouse_LeftUp();
  UsbDeviceHidMouse_LeftDown();
  UsbDeviceHidMouse_LeftUp();
  UsbDeviceHidMouse_LeftDown();
  UsbDeviceHidMouse_LeftUp();
}

/**
 ******************************************************************************
 ** Mouse Left Down
 **
 *****************************************************************************/
void UsbDeviceHidMouse_LeftDown(void)
{
  pu8MouseData[0] |= 0x01;
  pu8MouseData[1] = 0;
  pu8MouseData[2] = 0;  
  pu8MouseData[3] = 0;
  SendData((uint8_t*)pu8MouseData,4,UsbPOLL);
}

/**
 ******************************************************************************
 ** Mouse Left Up
 **
 *****************************************************************************/
void UsbDeviceHidMouse_LeftUp(void)
{
  pu8MouseData[0] &= ~0x01;
  pu8MouseData[1] = 0;
  pu8MouseData[2] = 0;  
  pu8MouseData[3] = 0;
  SendData((uint8_t*)pu8MouseData,4,UsbPOLL);
}

/**
 ******************************************************************************
 ** Mouse Right Click (Up-Down-Up)
 **
 *****************************************************************************/
void UsbDeviceHidMouse_RightClick(void)
{
  UsbDeviceHidMouse_RightUp();
  UsbDeviceHidMouse_RightDown();
  UsbDeviceHidMouse_RightUp();
}

/**
 ******************************************************************************
 ** Mouse Right Click (Up-Down-Up-Down-Up)
 **
 *****************************************************************************/
void UsbDeviceHidMouse_RightDoubleClick(void)
{
  UsbDeviceHidMouse_RightUp();
  UsbDeviceHidMouse_RightDown();
  UsbDeviceHidMouse_RightUp();
  UsbDeviceHidMouse_RightDown();
  UsbDeviceHidMouse_RightUp();
}

/**
 ******************************************************************************
 ** Mouse Right Down
 **
 *****************************************************************************/
void UsbDeviceHidMouse_RightDown(void)
{
  pu8MouseData[0] |= 0x02;
  pu8MouseData[1] = 0;
  pu8MouseData[2] = 0;  
  pu8MouseData[3] = 0;
  SendData((uint8_t*)pu8MouseData,4,UsbPOLL);
}

/**
 ******************************************************************************
 ** Mouse Right Up
 **
 *****************************************************************************/
void UsbDeviceHidMouse_RightUp(void)
{
  pu8MouseData[0] &= ~0x02;
  pu8MouseData[1] = 0;
  pu8MouseData[2] = 0;  
  pu8MouseData[3] = 0;
  SendData((uint8_t*)pu8MouseData,4,UsbPOLL);  
}

/**
 ******************************************************************************
 ** Scroll Wheel Movement
 **
 ** \param i8Scroll Move 
 **
 *****************************************************************************/
void UsbDeviceHidMouse_Scroll(int8_t i8Scroll)
{
  pu8MouseData[1] = 0;
  pu8MouseData[2] = 0;  
  pu8MouseData[3] = (int8_t)i8Scroll;
  SendData((uint8_t*)pu8MouseData,4,UsbPOLL);  
}

/**
 ******************************************************************************
 ** Return connected state
 **
 ** \return TRUE if ready
 **
 *****************************************************************************/
boolean_t UsbDeviceHidMouse_IsConnected(void)
{
    return bReady;
}
#endif
