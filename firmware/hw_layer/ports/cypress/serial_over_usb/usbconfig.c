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
/************************************************************************/
/** \file usbconfig.c
 **
 ** Part of USB Driver Module
 **
 ** A detailed description is available at 
 ** @link UsbConfigGroup USB Device Cdc Com Module description @endlink
 **
 ** History:
 **   - 2012-08-24    2.0  MSc  New Version for use with M3 L3 USB driver
 **   - 2012-10-02    2.1  MSc  use of external interrupts without L3 implemented
 **   - 2012-11-22    2.2  MSc  minor bug fixes
 **   - 2015-05-05    2.3  MSCH updated to latest version, changed
 **                        !USB_DISBALE_..._FUNCTIONALITY to FM_PERIPHERAL_USB_..._ENABLED
 *****************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/

#include "pdl_header.h"
#include "usb.h"

/**
 ******************************************************************************
 ** \defgroup UsbConfigGroup USB Config
 **
 ** Provided functions of USB module:
 ** 
 ** - UsbConfig_UsbInit()
 ** - UsbConfig_SwitchMode()
 ** - UsbConfig_Device0Vbus()
 ** - UsbConfig_Device1Vbus()
 ** - UsbConfig_Host0Overcurrent()
 ** - UsbConfig_Host1Overcurrent()
 ** - UsbConfig_Host0Vbus()
 ** - UsbConfig_Host1Vbus()
 ** - UsbConfig_Host0PulldownHostEnable()
 ** - UsbConfig_Host1PulldownHostEnable()
 ** - UsbDevice_Usb0ExintCallback()
 ** - UsbDevice_Usb1ExintCallback()
 **
 ** Used to initialize and configure the USB HAL. It gives an example how to initialize
 ** all USB parts. UsbConfig_UsbInit() is used to setup USB host and device mode for USB0 and USB1.
 ** UsbConfig_SwitchMode() is used to detect the Device VBUS and to do the Host / Device switching.
 ** UsbConfig_Device<n>Vbus() <n = 0,1> is used to do GPIO read or write for the Device VBUS detection pin.
 ** UsbConfig_Device<n>Vbus() is defined as callback for the USB stack.
 ** UsbConfig_Host<n>Overcurrent() <n = 0,1> is used to do GPIO read or write for the Host overcurrent detection.
 ** UsbConfig_Host<n>Overcurrent() is defined as callback for the USB stack.
 ** UsbConfig_Host<n>Vbus() <n = 0,1> is used to do GPIO read or write for the Host VBUS enable.
 ** UsbConfig_Host<n>Vbus() is defined as callback for the USB stack.
 ** UsbConfig_Host<n>PulldownHostEnable() <n = 0,1> is used to do GPIO read or write to enable the Host 15K pulldowns (externally).
 ** UsbConfig_Host<n>PulldownHostEnable() is defined as callback for the USB stack.
 ** UsbDevice_Usb<n>ExintCallback() <n = 0,1> is used if the Device VBUS detection is done by IRQ.
 ** UsbDevice_Usb<n>ExintCallback() is called from an external IRQ pin.
 **
 ******************************************************************************/
//@{

/**
 ******************************************************************************    
 ** \page usbconfig_module_includes Required includes in main application
 ** \brief Following includes are required
 ** @code  
 ** #include "usb.h"   
 ** @endcode
 **
 ******************************************************************************/

/**
 ****************************************************************************** 
 ** \page usbconfig_module_init Example: Initialization
 ** \brief Following initialization is required
 **
 ** @code
 ** UsbConfig_UsbInit();   
 ** @endcode
 **
 ******************************************************************************/
    
/**
 ****************************************************************************** 
 ** \page usbconfig_example_main Example: Whole example
 ** @code 
 ** #include "usb.h"    
 **
 **   
 ** int main()
 ** {
 **
 **     // other initializations
 **
 **     UsbConfig_UsbInit();   
 **     
 **     // other initializations
 **
 **     for(;;)
 **     {
 **         UsbConfig_SwitchMode(); //must be called periodically to do VBUS detection
 **                                 //or Host / Device switching
 **         
 **         // application code
 **     }
 ** }
 ** @endcode
 **
 ******************************************************************************/
              
#if ((FM_PERIPHERAL_USB_DEVICE_ENABLED == ON) || (FM_PERIPHERAL_USB_HOST_ENABLED == ON))
     
#if (FM_PERIPHERAL_USB_DEVICE_ENABLED == ON)
  #include "usbdevice.h"
  #if ((USE_USBDEVICEHW_H == 1) || (USB_USE_PDL == 1))
      #include "usbdevicehw.h"
  #endif
  #if ((USE_USBDESCRIPTORS_H == 1) || (USB_USE_PDL == 1))
    #include "usbdescriptors.h"
  #endif
#endif
#if (FM_PERIPHERAL_USB_HOST_ENABLED == ON)
  #include "usbhost.h"
  #if ((USE_USBHOSTHW_H == 1) || (USB_USE_PDL == 1))
      #include "usbhosthw.h"
  #endif
#endif

#if (USBDEVICECDCCOM_ENABLED == ON)
    #include "UsbDeviceCdcCom.h"
#endif
#if (USBDEVICEHIDCOM_ENABLED == ON)
    #include "UsbDeviceHidCom.h"
#endif
#if (USBDEVICEHIDJOYSTICK_ENABLED == ON)
    #include "UsbDeviceHidJoystick.h"
#endif
#if (USBDEVICEHIDKEYBOARD_ENABLED == ON)
    #include "UsbDeviceHidKeyboard.h"         
#endif 
#if (USBDEVICEHIDMOUSE_ENABLED == ON)
    #include "UsbDeviceHidMouse.h"
#endif    
#if (USBDEVICELIBUSB_ENABLED == ON)
    #include "UsbDeviceLibUsb.h"
#endif
#if (USBDEVICEPRINTER_ENABLED == ON)
    #include "UsbDevicePrinter.h"
#endif
#if USBHOSTHIDMOUSE_ENABLED == ON
    #include "UsbHostHidMouse.h"
#endif
#if USBHOSTHIDKEYBOARD_ENABLED == ON
    #include "UsbHostHidKeyboard.h"
#endif     
#if (USBDEVICEMASSSTORAGE_ENABLED == ON)
    #include "UsbDeviceMassStorage.h"
#endif 
/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/

#if FM_PERIPHERAL_ENABLE_USB0_DEVICE == ON
static void UsbConfig_UsbDeviceClassesInitCallback0(stc_usbn_t* pstcUSB);
#endif

#if FM_PERIPHERAL_ENABLE_USB1_DEVICE == ON
static void UsbConfig_UsbDeviceClassesInitCallback1(stc_usbn_t* pstcUSB);
#endif

#ifdef __USBDEVICEHW_H__
    #if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && (((USE_USBDEVICEHW_H == 1) || (USB_USE_PDL == 1))))
        static boolean_t UsbConfig_Device0Vbus(en_usb_extint_param_t enType);
    #endif
    #if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && (((USE_USBDEVICEHW_H == 1) || (USB_USE_PDL == 1))))
        static boolean_t UsbConfig_Device1Vbus(en_usb_extint_param_t enType);
    #endif
#endif
#ifdef __USBHOSTHW_H__
    #if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && (((USE_USBHOSTHW_H == 1) || (USB_USE_PDL == 1))))
        boolean_t UsbConfig_Host0Overcurrent(en_usb_extint_param_t enType);
        boolean_t UsbConfig_Host0Vbus(en_usb_gpio_param_t enType);
        boolean_t UsbConfig_Host0PulldownHostEnable(en_usb_gpio_param_t enType);
    #endif        
    #if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && (((USE_USBHOSTHW_H == 1) || (USB_USE_PDL == 1))))
        boolean_t UsbConfig_Host1Overcurrent(en_usb_extint_param_t enType);
        boolean_t UsbConfig_Host1Vbus(en_usb_gpio_param_t enType);
        boolean_t UsbConfig_Host1PulldownHostEnable(en_usb_gpio_param_t enType);
    #endif   
#endif

#if FM_PERIPHERAL_ENABLE_USB0_DEVICE == ON
static void UsbConfig_UsbDeviceClassesInitCallback0(stc_usbn_t* pstcUSB)
{
    #if (USBDEVICECDCCOM_ENABLED == ON)
        UsbDeviceCdcCom_Init((stc_usbn_t*)&USB0);
    #endif
    #if (USBDEVICEHIDCOM_ENABLED == ON)
        UsbDeviceHidCom_Init((stc_usbn_t*)&USB0);
    #endif
    #if (USBDEVICEHIDJOYSTICK_ENABLED == ON)
        UsbDeviceHidJoystick_Init((stc_usbn_t*)&USB0);
    #endif
    #if (USBDEVICEHIDKEYBOARD_ENABLED == ON)
        UsbDeviceHidKeyboard_Init((stc_usbn_t*)&USB0);        
    #endif 
    #if (USBDEVICEHIDMOUSE_ENABLED == ON)
        UsbDeviceHidMouse_Init((stc_usbn_t*)&USB0);
    #endif    
    #if (USBDEVICELIBUSB_ENABLED == ON)
        UsbDeviceLibUsb_Init((stc_usbn_t*)&USB0);
    #endif
    #if (USBDEVICEPRINTER_ENABLED == ON)
        UsbDevicePrinter_Init((stc_usbn_t*)&USB0);
    #endif     
    #if (USBDEVICEMASSSTORAGE_ENABLED == ON)
        UsbDeviceMassStorage_Init((stc_usbn_t*)&USB0);
    #endif  
    /* USB0 WIZARD DEVICECLASSINIT */
}
#endif

#if FM_PERIPHERAL_ENABLE_USB1_DEVICE == ON
static void UsbConfig_UsbDeviceClassesInitCallback1(stc_usbn_t* pstcUSB)
{
    #if (USBDEVICECDCCOM_ENABLED == ON)
        UsbDeviceCdcCom_Init((stc_usbn_t*)&USB1);
    #endif
    #if (USBDEVICEHIDCOM_ENABLED == ON)
        UsbDeviceHidCom_Init((stc_usbn_t*)&USB1);
    #endif
    #if (USBDEVICEHIDJOYSTICK_ENABLED == ON)
        UsbDeviceHidJoystick_Init((stc_usbn_t*)&USB1);
    #endif
    #if (USBDEVICEHIDKEYBOARD_ENABLED == ON)
        UsbDeviceHidKeyboard_Init((stc_usbn_t*)&USB1);        
    #endif 
    #if (USBDEVICEHIDMOUSE_ENABLED == ON)
        UsbDeviceHidMouse_Init((stc_usbn_t*)&USB1);
    #endif    
    #if (USBDEVICELIBUSB_ENABLED == ON)
        UsbDeviceLibUsb_Init((stc_usbn_t*)&USB1);
    #endif
    #if (USBDEVICEPRINTER_ENABLED == ON)
        UsbDevicePrinter_Init((stc_usbn_t*)&USB1);
    #endif 
    #if (USBDEVICEMASSSTORAGE_ENABLED == ON)
        UsbDeviceMassStorage_Init((stc_usbn_t*)&USB1);
    #endif 
    /* USB1 WIZARD DEVICECLASSINIT */    
}
#endif

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/








/**
 ******************************************************************************
 ** \brief Initialize USB
 **
 ******************************************************************************/
void UsbConfig_UsbInit(void)
{
    stc_usb_config_t       stcUsbConfig;
    #if (!defined(USB_DISBALE_DEVICE_FUNCTIONALITY))
    stc_usbdevice_config_t stcUsbDeviceConfig;
    #endif
    
    Usb_Init();
    
    
    /* Setup USB 0 */
    USB_ZERO_STRUCT(stcUsbConfig);
    
    #if (FM_PERIPHERAL_USB_DEVICE_ENABLED)
        USB_ZERO_STRUCT(stcUsbDeviceConfig);
    #endif /* (FM_PERIPHERAL_USB_DEVICE_ENABLED) */
    
    #if ((FM_PERIPHERAL_ENABLE_USB0_HOST == ON) && (FM_PERIPHERAL_ENABLE_USB0_DEVICE == ON))
        stcUsbConfig.enMode = UsbHostDeviceEnabled;
    #elif ((FM_PERIPHERAL_ENABLE_USB0_HOST == ON) && (FM_PERIPHERAL_ENABLE_USB0_DEVICE == OFF))
        stcUsbConfig.enMode = UsbHostEnabled;
    #elif ((FM_PERIPHERAL_ENABLE_USB0_HOST == OFF) && (FM_PERIPHERAL_ENABLE_USB0_DEVICE == ON))
        stcUsbConfig.enMode = UsbDeviceEnabled;
    #endif
    
    #if (FM_PERIPHERAL_USB_DEVICE_ENABLED)
        #if ((USE_USBDESCRIPTORS_H == 1) || (USB_USE_PDL == 1))
            stcUsbDeviceConfig.pu8DeviceDescriptor = (uint8_t*)au8DeviceDescriptor;
            stcUsbDeviceConfig.pu8ConfigDescriptor = (uint8_t*)au8ConfigDescriptor; 
            stcUsbDeviceConfig.astcReportDescriptors = (stc_usbdevice_reportdescriptor_t*)astcReportDescriptors;
            stcUsbDeviceConfig.pstcStringDescriptors = (stc_usbdevice_stringdescriptor_t*)pstcStringDescriptors;
            stcUsbDeviceConfig.u8StringDescriptorCount = USBDESCRIPTORS_STRINGDESCRIPTOR_COUNT;
        #endif /* ((USE_USBDESCRIPTORS_H == 1) || (USB_USE_PDL == 1)) */
    #endif /* (FM_PERIPHERAL_USB_DEVICE_ENABLED) */
    
    stcUsbConfig.bUseInterrupts = TRUE;
    
    #if (FM_PERIPHERAL_USB_DEVICE_ENABLED)
        #if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && ((USE_USBDEVICEHW_H == 1) || (USB_USE_PDL == 1)))
            stcUsbConfig.pfnDeviceVbus = UsbConfig_Device0Vbus;
        #endif
    #endif /* (FM_PERIPHERAL_USB_DEVICE_ENABLED) */
    
    #if ((FM_PERIPHERAL_USB_HOST_ENABLED))
        #if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && ((USE_USBHOSTHW_H == 1) || (USB_USE_PDL == 1)))
            stcUsbConfig.pfnHostVbus = UsbConfig_Host0Vbus;
            stcUsbConfig.pfnHostPullDownHostEnable = UsbConfig_Host0PulldownHostEnable;
            stcUsbConfig.pfnHostOvercurrent = UsbConfig_Host0Overcurrent;
        #endif
    #endif /* ((FM_PERIPHERAL_USB_HOST_ENABLED)) */
    
    #if FM_PERIPHERAL_ENABLE_USB0_DEVICE == ON
        stcUsbDeviceConfig.pfnInitClassesCallback = UsbConfig_UsbDeviceClassesInitCallback0;
    #endif

    #if FM_PERIPHERAL_ENABLE_USB0_DEVICE == ON
        UsbDevice_Init((stc_usbn_t*)&USB0,&stcUsbConfig,&stcUsbDeviceConfig);    
    #endif
    
    #if FM_PERIPHERAL_ENABLE_USB0 == ON
        #if FM_PERIPHERAL_ENABLE_USB0_HOST == ON
            UsbHost_Init((stc_usbn_t*)&USB0,&stcUsbConfig);
        #endif
        Usb_Configure((stc_usbn_t*)&USB0,&stcUsbConfig);
    #endif
    
    
 
    
    /* Setup USB 1 */
    USB_ZERO_STRUCT(stcUsbConfig);
    
    #if (FM_PERIPHERAL_USB_DEVICE_ENABLED)
        USB_ZERO_STRUCT(stcUsbDeviceConfig);
    #endif
    
    #if ((FM_PERIPHERAL_ENABLE_USB1_HOST == ON) && (FM_PERIPHERAL_ENABLE_USB1_DEVICE == ON))
        stcUsbConfig.enMode = UsbHostDeviceEnabled;
    #elif ((FM_PERIPHERAL_ENABLE_USB1_HOST == ON) && (FM_PERIPHERAL_ENABLE_USB1_DEVICE == OFF))
        stcUsbConfig.enMode = UsbHostEnabled;
    #elif ((FM_PERIPHERAL_ENABLE_USB1_HOST == OFF) && (FM_PERIPHERAL_ENABLE_USB1_DEVICE == ON))
        stcUsbConfig.enMode = UsbDeviceEnabled;
    #endif
        
    #if (FM_PERIPHERAL_USB_DEVICE_ENABLED)
        #if ((USE_USBDESCRIPTORS_H == 1) || (USB_USE_PDL == 1))
            stcUsbDeviceConfig.pu8DeviceDescriptor = (uint8_t*)au8DeviceDescriptor;
            stcUsbDeviceConfig.pu8ConfigDescriptor = (uint8_t*)au8ConfigDescriptor; 
            stcUsbDeviceConfig.astcReportDescriptors = (stc_usbdevice_reportdescriptor_t*)astcReportDescriptors;
            stcUsbDeviceConfig.pstcStringDescriptors = (stc_usbdevice_stringdescriptor_t*)pstcStringDescriptors;
            stcUsbDeviceConfig.u8StringDescriptorCount = USBDESCRIPTORS_STRINGDESCRIPTOR_COUNT;
        #endif
    #endif /* (FM_PERIPHERAL_USB_DEVICE_ENABLED) */
    
    stcUsbConfig.bUseInterrupts = TRUE;
    
    #if (FM_PERIPHERAL_USB_DEVICE_ENABLED)
        #if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && ((USE_USBDEVICEHW_H == 1) || (USB_USE_PDL == 1)))
            stcUsbConfig.pfnDeviceVbus = UsbConfig_Device1Vbus;
        #endif
    #endif /* (FM_PERIPHERAL_USB_DEVICE_ENABLED) */
    
    #if (FM_PERIPHERAL_USB_HOST_ENABLED)
        #if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && ((USE_USBHOSTHW_H == 1) || (USB_USE_PDL == 1)))
            stcUsbConfig.pfnHostVbus = UsbConfig_Host1Vbus;
            stcUsbConfig.pfnHostPullDownHostEnable = UsbConfig_Host1PulldownHostEnable;
            stcUsbConfig.pfnHostOvercurrent = UsbConfig_Host1Overcurrent;
        #endif
    #endif /* (FM_PERIPHERAL_USB_HOST_ENABLED) */
    
    #if FM_PERIPHERAL_ENABLE_USB1_DEVICE == ON
        stcUsbDeviceConfig.pfnInitClassesCallback = UsbConfig_UsbDeviceClassesInitCallback1;
    #endif
    
    #if FM_PERIPHERAL_ENABLE_USB1_DEVICE == ON
        UsbDevice_Init((stc_usbn_t*)&USB1,&stcUsbConfig,&stcUsbDeviceConfig);    
    #endif
    
    #if FM_PERIPHERAL_ENABLE_USB1 == ON
        #if FM_PERIPHERAL_ENABLE_USB1_HOST == ON
            UsbHost_Init((stc_usbn_t*)&USB1,&stcUsbConfig);
        #endif
        Usb_Configure((stc_usbn_t*)&USB1,&stcUsbConfig);
    #endif
}

#ifdef __USBDEVICEHW_H__

#if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && (((USE_USBDEVICEHW_H == 1) || (USB_USE_PDL == 1))))
/**
 ******************************************************************************
 ** \brief Device 0 VBUS GPIO / external interrupt callback
 **
 ** \param enType
 **
 ** \return Depending on enType
 **
 ******************************************************************************/
static boolean_t UsbConfig_Device0Vbus(en_usb_extint_param_t enType)
{
    switch(enType)
    {
      case UsbExtIntDeinit:
        DEVICE0VBUS_DEINIT; 
        break;
      case UsbExtIntInit:
        DEVICE0VBUS_INIT;
        break;  
      case UsbExtIntDisableIsr:
        DEVICE0VBUS_DISABLEISR;
        break; 
      case UsbExtIntEnableIsr:
        DEVICE0VBUS_ENABLEISR;
        break; 
      case UsbExtIntClearIsrFlag:
        DEVICE0VBUS_CLEARISRFLAG;
        break; 
      case UsbExtIntIsSetIsrFlag:
        return DEVICE0VBUS_ISRISSET;
      case UsbExtIntSetLowDetect:
        DEVICE0VBUS_SETLOWDETECT;
        break; 
      case UsbExtIntSetHighDetect:
        DEVICE0VBUS_SETHIGHDETECT;
        break; 
      case UsbExtIntGetLevel:
        return DEVICE0VBUS_HIGHDETECT;
    }
    return FALSE;
}
#endif

#if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && ((USE_USBDEVICEHW_H == 1) || (USB_USE_PDL == 1)))
/**
 ******************************************************************************
 ** \brief Device 1 VBUS GPIO / external interrupt callback
 **
 ** \param enType
 **
 ** \return Depending on enType
 **
 ******************************************************************************/
static boolean_t UsbConfig_Device1Vbus(en_usb_extint_param_t enType)
{
    switch(enType)
    {
      case UsbExtIntDeinit:
        DEVICE1VBUS_DEINIT; 
        break;
      case UsbExtIntInit:
        DEVICE1VBUS_INIT;
        break;  
      case UsbExtIntDisableIsr:
        DEVICE1VBUS_DISABLEISR;
        break; 
      case UsbExtIntEnableIsr:
        DEVICE1VBUS_ENABLEISR;
        break; 
      case UsbExtIntClearIsrFlag:
        DEVICE1VBUS_CLEARISRFLAG;
        break; 
      case UsbExtIntIsSetIsrFlag:
        return DEVICE1VBUS_ISRISSET;
        break; 
      case UsbExtIntSetLowDetect:
        DEVICE1VBUS_SETLOWDETECT;
        break; 
      case UsbExtIntSetHighDetect:
        DEVICE1VBUS_SETHIGHDETECT;
        break; 
      case UsbExtIntGetLevel:
        return DEVICE1VBUS_HIGHDETECT;
        break; 
    }
    return FALSE;
}
#endif

#endif

#ifdef __USBHOSTHW_H__

#if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && (((USE_USBHOSTHW_H == 1) || (USB_USE_PDL == 1))))
/**
 ******************************************************************************
 ** \brief Host 0 Overcurrent GPIO / external interrupt callback
 **
 ** \param enType
 **
 ** \return Depending on enType
 **
 ******************************************************************************/
boolean_t UsbConfig_Host0Overcurrent(en_usb_extint_param_t enType)
{
    switch(enType)
    {
      case UsbExtIntDeinit:
        HOST0OVERCURRENT_DEINIT; 
        break;
      case UsbExtIntInit:
        HOST0OVERCURRENT_INIT;
        break;  
      case UsbExtIntDisableIsr:
        HOST0OVERCURRENT_DISABLEISR;
        break; 
      case UsbExtIntEnableIsr:
        HOST0OVERCURRENT_ENABLEISR;
        break; 
      case UsbExtIntClearIsrFlag:
        HOST0OVERCURRENT_CLEARISRFLAG;
        break; 
      case UsbExtIntIsSetIsrFlag:
        return HOST0OVERCURRENT_ISRISSET;
      case UsbExtIntSetLowDetect:
        HOST0OVERCURRENT_SETLOWDETECT;
        break; 
      case UsbExtIntSetHighDetect:
        HOST0OVERCURRENT_SETHIGHDETECT;
        break; 
      case UsbExtIntGetLevel:
        return HOST0OVERCURRENT_HIGHDETECT;
      default:
        return FALSE;
    }
    return TRUE;
}

/**
 ******************************************************************************
 ** \brief Host 0 VBUS GPIO / external interrupt callback
 **
 ** \param enType
 **
 ** \return Depending on enType
 **
 ******************************************************************************/
boolean_t UsbConfig_Host0Vbus(en_usb_gpio_param_t enType)
{
    switch(enType)
    {
      case UsbGpioDeinit:
        HOST0VBUS_DEINIT; 
        break;
      case UsbGpioInit:
        HOST0VBUS_INIT;
        break;  
      case UsbGpioSet:
        HOST0VBUS_SET;
        break; 
      case UsbGpioClear:
        HOST0VBUS_CLEAR;
        break; 
      default:
        return FALSE;
    }
    return TRUE;
}

/**
 ******************************************************************************
 ** \brief Host 0 pull-down GPIO / external interrupt callback
 **
 ** \param enType
 **
 ** \return Depending on enType
 **
 ******************************************************************************/
boolean_t UsbConfig_Host0PulldownHostEnable(en_usb_gpio_param_t enType)
{
    switch(enType)
    {
      case UsbGpioDeinit:
        HOST0OTGPULLDOWN_DEINIT; 
        break;
      case UsbGpioInit:
        HOST0OTGPULLDOWN_INIT;
        break;  
      case UsbGpioSet:
        HOST0OTGPULLDOWN_SET;
        break; 
      case UsbGpioClear:
        HOST0OTGPULLDOWN_CLEAR;
        break; 
      default:
        return FALSE;
    }
    return TRUE;
}
#endif

#if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && (((USE_USBHOSTHW_H == 1) || (USB_USE_PDL == 1))))
/**
 ******************************************************************************
 ** \brief Host 1 Overcurrent GPIO / external interrupt callback
 **
 ** \param enType
 **
 ** \return Depending on enType
 **
 ******************************************************************************/
boolean_t UsbConfig_Host1Overcurrent(en_usb_extint_param_t enType)
{
    switch(enType)
    {
      case UsbExtIntDeinit:
        HOST1OVERCURRENT_DEINIT; 
        break;
      case UsbExtIntInit:
        HOST1OVERCURRENT_INIT;
        break;  
      case UsbExtIntDisableIsr:
        HOST1OVERCURRENT_DISABLEISR;
        break; 
      case UsbExtIntEnableIsr:
        HOST1OVERCURRENT_ENABLEISR;
        break; 
      case UsbExtIntClearIsrFlag:
        HOST1OVERCURRENT_CLEARISRFLAG;
        break; 
      case UsbExtIntIsSetIsrFlag:
        return HOST1OVERCURRENT_ISRISSET;
      case UsbExtIntSetLowDetect:
        HOST1OVERCURRENT_SETLOWDETECT;
        break; 
      case UsbExtIntSetHighDetect:
        HOST1OVERCURRENT_SETHIGHDETECT;
        break; 
      case UsbExtIntGetLevel:
        return HOST1OVERCURRENT_HIGHDETECT;
      default:
        return FALSE;
    }
    return TRUE;
}

/**
 ******************************************************************************
 ** \brief Host 1 VBUS GPIO / external interrupt callback
 **
 ** \param enType
 **
 ** \return Depending on enType
 **
 ******************************************************************************/
boolean_t UsbConfig_Host1Vbus(en_usb_gpio_param_t enType)
{
    switch(enType)
    {
      case UsbGpioDeinit:
        HOST1VBUS_DEINIT; 
        break;
      case UsbGpioInit:
        HOST1VBUS_INIT;
        break;  
      case UsbGpioSet:
        HOST1VBUS_SET;
        break; 
      case UsbGpioClear:
        HOST1VBUS_CLEAR;
        break; 
      default:
        return FALSE;
    }
    return TRUE;
}

/**
 ******************************************************************************
 ** \brief Host 1 pull-down GPIO / external interrupt callback
 **
 ** \param enType
 **
 ** \return Depending on enType
 **
 ******************************************************************************/
boolean_t UsbConfig_Host1PulldownHostEnable(en_usb_gpio_param_t enType)
{
    switch(enType)
    {
      case UsbGpioDeinit:
        HOST1OTGPULLDOWN_DEINIT; 
        break;
      case UsbGpioInit:
        HOST1OTGPULLDOWN_INIT;
        break;  
      case UsbGpioSet:
        HOST1OTGPULLDOWN_SET;
        break; 
      case UsbGpioClear:
        HOST1OTGPULLDOWN_CLEAR;
        break; 
      default:
        return FALSE;
    }
    return TRUE;
}
#endif

#endif

void UsbConfig_SwitchMode(void)
{
    #if FM_PERIPHERAL_ENABLE_USB0 == ON
        #if ((FM_PERIPHERAL_ENABLE_USB0_HOST == ON) && (FM_PERIPHERAL_ENABLE_USB0_DEVICE == OFF))
            Usb_SwitchUsb((stc_usbn_t*)&USB0,UsbSwitchToHost,0);
        #endif
        #if ((FM_PERIPHERAL_ENABLE_USB0_HOST == OFF) && (FM_PERIPHERAL_ENABLE_USB0_DEVICE == ON))
            Usb_SwitchUsb((stc_usbn_t*)&USB0,UsbSwitchDependingDeviceVbus,0);
        #endif
        #if ((FM_PERIPHERAL_ENABLE_USB0_HOST == ON) && (FM_PERIPHERAL_ENABLE_USB0_DEVICE == ON))
            Usb_SwitchUsb((stc_usbn_t*)&USB0,UsbSwitchDependingDeviceVbus,0);
        #endif
    #endif

    #if FM_PERIPHERAL_ENABLE_USB1 == ON
        #if ((FM_PERIPHERAL_ENABLE_USB1_HOST == ON) && (FM_PERIPHERAL_ENABLE_USB1_DEVICE == OFF))

            Usb_SwitchUsb((stc_usbn_t*)&USB1,UsbSwitchToHost,0);
        #endif
        #if ((FM_PERIPHERAL_ENABLE_USB1_HOST == OFF) && (FM_PERIPHERAL_ENABLE_USB1_DEVICE == ON))
            Usb_SwitchUsb((stc_usbn_t*)&USB1,UsbSwitchDependingDeviceVbus,0);
        #endif
        #if ((FM_PERIPHERAL_ENABLE_USB1_HOST == ON) && (FM_PERIPHERAL_ENABLE_USB1_DEVICE == ON))
            Usb_SwitchUsb((stc_usbn_t*)&USB1,UsbSwitchDependingDeviceVbus,0);
        #endif
    #endif
}


#if (FM_PERIPHERAL_ENABLE_USB0_DEVICE == ON)
void UsbDevice_Usb0ExintCallback(void)
{
    if (Usb_SwitchUsb((stc_usbn_t*)&USB0,UsbSwitchDependingDeviceVbus,0) == Ok)
    {
        if (DEVICE0VBUS_HIGHDETECT)
        {
            DEVICE0VBUS_SETLOWDETECT;
        }
        else
        {
            DEVICE0VBUS_SETHIGHDETECT;
        }
    }
    
}
#endif

#if (FM_PERIPHERAL_ENABLE_USB1_DEVICE == ON)
void UsbDevice_Usb1ExintCallback(void)
{
    if (Usb_SwitchUsb((stc_usbn_t*)&USB1,UsbSwitchDependingDeviceVbus,0) == Ok)
    {
        if (DEVICE1VBUS_HIGHDETECT)
        {
            DEVICE1VBUS_SETLOWDETECT;
        }
        else
        {
            DEVICE1VBUS_SETHIGHDETECT;
        }
    }
}
#endif



#if (USB_USE_L3 == 0)
#if ((USB_USE_L3 == 0) && (USB_USE_EXT_INT == 1))
#if (!defined(USB_DISBALE_DEVICE_FUNCTIONALITY))
void INT8_31_Handler (void)
{
  #if FM_PERIPHERAL_ENABLE_USB0 == ON
  if (DEVICE0VBUS_ISRISSET)
  {
     DEVICE0VBUS_CLEARISRFLAG;
     UsbDevice_Usb0ExintCallback();
  }
  #endif
  #if FM_PERIPHERAL_ENABLE_USB1 == ON
  if (DEVICE1VBUS_ISRISSET)
  {
     DEVICE1VBUS_CLEARISRFLAG;
     UsbDevice_Usb1ExintCallback();
  }
  #endif
}

void INT8_15_Handler (void)
{
  #if FM_PERIPHERAL_ENABLE_USB0 == ON
  if (DEVICE0VBUS_ISRISSET)
  {
     DEVICE0VBUS_CLEARISRFLAG;
     UsbDevice_Usb0ExintCallback();
  }
  #endif
  #if FM_PERIPHERAL_ENABLE_USB1 == ON
  if (DEVICE1VBUS_ISRISSET)
  {
     DEVICE1VBUS_CLEARISRFLAG;
     UsbDevice_Usb1ExintCallback();
  }
  #endif
}

void INT0_7_Handler (void)
{
  #if FM_PERIPHERAL_ENABLE_USB0 == ON
  if (DEVICE0VBUS_ISRISSET)
  {
     DEVICE0VBUS_CLEARISRFLAG;
     UsbDevice_Usb0ExintCallback();
  }
  #endif
  #if FM_PERIPHERAL_ENABLE_USB1 == ON
  if (DEVICE1VBUS_ISRISSET)
  {
     DEVICE1VBUS_CLEARISRFLAG;
     UsbDevice_Usb1ExintCallback();
  }
  #endif
}
#endif
#endif
#endif
#else

/**
 ******************************************************************************
 ** \brief Initialize USB (dummy if USB is disabled)
 **
 ******************************************************************************/
void UsbConfig_UsbInit(void)
{
}

/**
 ******************************************************************************
 ** \brief Switch USB mode (dummy if USB is disabled)
 **
 ******************************************************************************/
void UsbConfig_SwitchMode(void)
{
}
#endif /* ((!defined(USB_DISBALE_DEVICE_FUNCTIONALITY)) || (!defined(USB_DISBALE_HOST_FUNCTIONALITY))) */
//@} // UsbConfigGroup