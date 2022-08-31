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
/** \file usbethernetclock.h
 **
 ** Headerfile for USB / Ethernet functions
 ** @link UsbEthernetGroup USB/Ethernet Clock Module description @endlink
 **
 ** History:
 **   - 2012-08-10  1.0  MSc  First version.
 **   - 2012-08-23  1.1  MSc  GNU compiler compatible
 **   - 2012-08-30  1.2  MSc  Correction in USB 0/1 clock enable
 **   - 2012-09-19  1.3  MSc  PLL settings for type 6 devices corrected
 **   - 2012-11-02  1.4  MSc  PLL settings for ethernet corrected
 **   - 2012-11-02  1.5  CNo  Ethernet MAC driver integration
 **   - 2012-11-22  1.6  MSc  typedefinitions for ethernetclock added 
 **                           (for use without header file)
 **   - 2013-01-14  1.7  MSc  type 6 12MHz setting added 
 **   - 2013-01-16  1.8  MSc  type 9 setting added 
 **   - 2013-06-04  1.9  MSc  FM4 support added
 **   - 2014-02-28  1.9  MSc  Updates in FM4 defines
 **   - 2014-09-04  2.0  MSc  FM4 type 3 added
 **   - 2015-02-04  2.1  MSc  GCC compatibility added
 **   - 2015-05-12  2.2  MSCH Updated to be compatible with new MCU templates
 **                           added more frequencies for PLL input crystals
 **   - 2015-05-29  2.3  MSCH Updated for USBCLK with 2 USB interfaces
 **   - 2015-06-29  2.4  MSCH Updated to be compatible with all FM3 series again
 **   - 2015-09-01  2.5  MSCH Change of FM_USBETHERNETCLK -> FM_USBETHERCLK
 **                           Added devices with no USB PLL
 **
 ******************************************************************************/

#ifndef __USBETHERNETCLOCK_H__
#define __USBETHERNETCLOCK_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/

#include "mcu.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
  
/**
 ******************************************************************************
 ** \defgroup UsbEthernetGroup USB/Ethernet Clock Module description
 **
 ** Provided functions of USB Ethernet Clock module:
 ** 
 ** - UsbEthernetClock_Init()
 ** - UsbEthernetClock_Deinit()
 **
 ** Used to initialize the USB Ethernet Clock PLL, devider and source selector
 **
 ******************************************************************************/
//@{

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/

   
   
#define USBETHERNETCLOCK_USE_USB      1
#define USBETHERNETCLOCK_USE_ETHERNET 0
#define USBETHERNETCLOCK_USE_L3                           0
#define USBETHERNETCLOCK_USE_PDL                          0

   
#if (USBETHERNETCLOCK_USE_L3 == 0) && (USBETHERNETCLOCK_USE_PDL == 0)
    #define USBETHERNETCLOCK_USE_STANDALONE 1
#else
    #define USBETHERNETCLOCK_USE_STANDALONE 0
#endif

#if !defined(USBETHERNETCLOCK_USE_LEGACYSUPPORT)
    #if (!defined(FM_USB_AVAILABLE)) && (!defined(FM_USB0)) && (!defined(FM_USB1))
        #define USBETHERNETCLOCK_USE_LEGACYSUPPORT        1U  
        #if defined(__USB_C__)
            #warning USB legacy support turned on automatically, but it is better to update to the latest MCU template!
        #endif
    #else
        #define USBETHERNETCLOCK_USE_LEGACYSUPPORT        0U
    #endif
#endif 

#if USBETHERNETCLOCK_USE_LEGACYSUPPORT == 1u
    #include "usbethernetclocklegacy.h"
#else
    #if (!defined(FM_USB_AVAILABLE)) && (!defined(FM_USB0)) && (!defined(FM_USB1))
        #error For this configuration set USBETHERNETCLOCK_USE_LEGACYSUPPORT to 1
    #endif
    #define stc_usbclkn_t FM_USBCLK_TypeDef
    #define stc_usb_ethernetclkn_t FM_USBETHERCLK_TypeDef
    
#endif
    
    
#if USBETHERNETCLOCK_USE_USB == 1
  #include "usb.h" /* if error happens, set USBETHERNETCLOCK_USE_USB to 0 */
#endif

#if USBETHERNETCLOCK_USE_ETHERNET == 1
  #include "emac_user.h" /* if error happens, set USBETHERNETCLOCK_USE_ETHERNET to 0 */

  #if EMAC_PHYINTERFACE_RMII == L3_ON
    #define ETHERNETCLOCK_OUTFREQ FREQ_50MHZ  //< Set required source clock frequency for RMII mode (50MHZ)
  #else
    #define ETHERNETCLOCK_OUTFREQ FREQ_25MHZ  //< Set required source clock frequency for MII mode (25MHZ)
  #endif
  #if EMAC_ECOUT == L3_OFF
    #undef USBETHERNETCLOCK_USE_ETHERNET
    #define USBETHERNETCLOCK_USE_ETHERNET 0
  #endif
#endif

#if (FM0P_DEVICE_TYPE == 3)
    #define USBETHERNETCLOCK_USBPLL_AVAILABLE 0
#else
    #define USBETHERNETCLOCK_USBPLL_AVAILABLE 1    
#endif
    
#if (USBETHERNETCLOCK_USE_USB == 1) && (USBETHERNETCLOCK_USE_STANDALONE == 1)
    #if defined(FM4_USB0)
        #include "base_types.h"
    #endif
    #if defined(FM3_USB0)
        #include "base_types.h"
    #endif   
    #if !defined(FM_PERIPHERAL_ENABLE_USB0_HOST)
        #define FM_PERIPHERAL_ENABLE_USB0_HOST   USB0_HOST_ENABLED
    #endif
    #if !defined(FM_PERIPHERAL_ENABLE_USB1_HOST)
        #define FM_PERIPHERAL_ENABLE_USB1_HOST   USB1_HOST_ENABLED
    #endif
    #if  !defined(FM_PERIPHERAL_ENABLE_USB0_DEVICE)
        #define FM_PERIPHERAL_ENABLE_USB0_DEVICE USB0_DEVICE_ENABLED    
    #endif
    #if  !defined(FM_PERIPHERAL_ENABLE_USB1_DEVICE)
        #define FM_PERIPHERAL_ENABLE_USB1_DEVICE USB1_DEVICE_ENABLED 
    #endif
#elif (USBETHERNETCLOCK_USE_L3 == 1)
    #include "l3.h" /* if error happens, set USBETHERNETCLOCK_USE_L3 to 0 */
    #include "base_types.h"
    #if !defined(FM_PERIPHERAL_ENABLE_USB0_HOST)
        #define FM_PERIPHERAL_ENABLE_USB0_HOST     L3_PERIPHERAL_ENABLE_USB0_HOST 
    #endif
    #if  !defined(FM_PERIPHERAL_ENABLE_USB0_DEVICE)
        #define FM_PERIPHERAL_ENABLE_USB0_DEVICE   L3_PERIPHERAL_ENABLE_USB0_DEVICE
    #endif
    #if !defined(FM_PERIPHERAL_ENABLE_USB1_HOST)
        #define FM_PERIPHERAL_ENABLE_USB1_HOST     L3_PERIPHERAL_ENABLE_USB1_HOST 
    #endif
    #if  !defined(FM_PERIPHERAL_ENABLE_USB1_DEVICE)
        #define FM_PERIPHERAL_ENABLE_USB1_DEVICE   L3_PERIPHERAL_ENABLE_USB1_DEVICE
    #endif
#elif (USBETHERNETCLOCK_USE_PDL == 1)
    #include "pdl.h" /* if error happens, set USBETHERNETCLOCK_USE_PDL to 0 */
    #include "base_types.h"
    #if !defined(FM_PERIPHERAL_ENABLE_USB0_HOST)
        #define FM_PERIPHERAL_ENABLE_USB0_HOST     PDL_PERIPHERAL_ENABLE_USB0_HOST 
    #endif
    #if  !defined(FM_PERIPHERAL_ENABLE_USB0_DEVICE)
        #define FM_PERIPHERAL_ENABLE_USB0_DEVICE   PDL_PERIPHERAL_ENABLE_USB0_DEVICE 
    #endif
    #if !defined(FM_PERIPHERAL_ENABLE_USB1_HOST)    
        #define FM_PERIPHERAL_ENABLE_USB1_HOST     PDL_PERIPHERAL_ENABLE_USB0_HOST 
    #endif
    #if  !defined(FM_PERIPHERAL_ENABLE_USB1_DEVICE)    
        #define FM_PERIPHERAL_ENABLE_USB1_DEVICE   PDL_PERIPHERAL_ENABLE_USB0_DEVICE
    #endif
#else
    #error Not supported configuration
#endif

#ifndef OFF
    #define OFF 0
#endif

#ifndef ON
    #define ON 1
#endif
   
#ifndef L3_OFF
    #define L3_OFF 0
#endif

#ifndef L3_ON
    #define L3_ON 1
#endif
  
#ifndef ETHERNETCLOCK_OUTFREQ 
  #define ETHERNETCLOCK_OUTFREQ FREQ_25MHZ
#endif
  
#ifndef FREQ_4MHZ
    #define FREQ_4MHZ  ( 4000000UL)
#endif

#ifndef FREQ_8MHZ
    #define FREQ_8MHZ  ( 8000000UL)
#endif
#ifndef FREQ_16MHZ
    #define FREQ_16MHZ (16000000UL)
#endif
#ifndef FREQ_24MHZ  
    #define FREQ_24MHZ (24000000UL)
#endif

#ifndef FREQ_48MHZ  
    #define FREQ_48MHZ (48000000UL)
#endif

#ifndef FREQ_25MHZ  
    #define FREQ_25MHZ (25000000UL)
#endif

#ifndef FREQ_50MHZ  
    #define FREQ_50MHZ (50000000UL)
#endif

#ifndef FREQ_96MHZ  
    #define FREQ_96MHZ (96000000UL)
#endif

#ifndef FREQ_144MHZ  
    #define FREQ_144MHZ (144000000UL)
#endif
  
//#define MAINCRYSTALFREQ 4MHZ   // <<< External   4MHz Crystal

#if ((!defined(__CLKMO)) && (!defined(MAINCRYSTALFREQ)))
    #error __CLKMO does not exist, please specify crystal freqency in MAINCRYSTALFREQ
#endif

#if !defined(MAINCRYSTALFREQ)
    #define MAINCRYSTALFREQ __CLKMO
#endif

    

/* USBCLK (FM3 type 0) */
#if (((defined(FM3_DEVICE_TYPE) && (FM3_DEVICE_TYPE == 0))) && (defined(FM3_USBCLK_BASE) || defined(FM_USBCLK_BASE)))
    #if !defined(__CLKMO)
        #error Please define __CLKMO in your system header file
    #elif __CLKMO == (4000000UL)
        #define USBETHERNETCLOCK_PLL_K 1
        #define USBETHERNETCLOCK_PLL_N 24
        #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
    #elif __CLKMO == (4800000UL)
        #define USBETHERNETCLOCK_PLL_K 1
        #define USBETHERNETCLOCK_PLL_N 20
        #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
    #elif __CLKMO == (6000000UL)
        #define USBETHERNETCLOCK_PLL_K 1
        #define USBETHERNETCLOCK_PLL_N 16
        #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
    #elif __CLKMO == (6400000UL)
        #define USBETHERNETCLOCK_PLL_K 1
        #define USBETHERNETCLOCK_PLL_N 15
        #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
    #elif __CLKMO == (8000000UL)
        #define USBETHERNETCLOCK_PLL_K 1
        #define USBETHERNETCLOCK_PLL_N 12
        #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
    #elif __CLKMO == (9600000UL)
        #define USBETHERNETCLOCK_PLL_K 1
        #define USBETHERNETCLOCK_PLL_N 10
        #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
    #elif __CLKMO == (12000000UL)
        #define USBETHERNETCLOCK_PLL_K 1
        #define USBETHERNETCLOCK_PLL_N 8
        #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
    #elif __CLKMO == (48000000UL)
        #define USBETHERNETCLOCK_UCSEL 0x00 // no PLL needed for 48MHz crystal, using directly CLKMO
    #else
        #error The external crystal defined in __CLKMO is not supported for USB PLL
    #endif /* __CLKMO == ... */
#endif /* USBCLK (FM3 type 0) */

/* if Ethernet is not using the USBETHERNETCLOCK */
#if (USBETHERNETCLOCK_USE_ETHERNET == 0)


    /* USB only: USBCLK/USBETHERNELCLK (all except FM3 type 0) */
    /* USBCLK/USBETHERNELCLK (FM3 type 1,4,5) */
    #if (!((defined(FM3_DEVICE_TYPE) && (FM3_DEVICE_TYPE == 0))) && \
       ((FM3_DEVICE_TYPE == 1) || (FM3_DEVICE_TYPE == 4) || (FM3_DEVICE_TYPE == 4)) && \
        (defined(FM3_USBCLK_BASE) || defined(FM4_USBCLK_BASE) \
        || defined(FM0P_USBCLK_BASE) || defined(FM_USBCLK_BASE) \
        || defined(FM3_USBETHERCLK_BASE) || defined(FM4_USBETHERCLK_BASE)  \
        || defined(FM0P_USBETHERCLK_BASE) || defined(FM_USBETHERCLK_BASE)))
        #if !defined(__CLKMO)
            #error Please define __CLKMO in your system header file
        #elif __CLKMO == (4000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 60
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (4500000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 64
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (4800000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 50
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 48
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5106383UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 47
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5760000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 50
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (6000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 40
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (6400000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 45
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (7200000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 40
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (7500000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 32
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (8000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 30
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (8888889UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 27
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (9000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 32
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (9600000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 25
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (10000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 24
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (10909091UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 22
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (11520000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 25
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (12000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 20
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (12631579UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 19
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (14400000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 20
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (15000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 16
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (16000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 15
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (18000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 16
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (19200000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 15
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #else
            #error The external crystal defined in __CLKMO is not supported for USB PLL
        #endif /* __CLKMO == ... */
    /* USBCLK/USBETHERNELCLK (FM3 type 6,9,12) */
    #elif (!((defined(FM3_DEVICE_TYPE) && (FM3_DEVICE_TYPE == 0))) && \
       ((FM3_DEVICE_TYPE == 6) || (FM3_DEVICE_TYPE == 9) || (FM3_DEVICE_TYPE == 12)) && \
        (defined(FM3_USBCLK_BASE) || defined(FM4_USBCLK_BASE) \
        || defined(FM0P_USBCLK_BASE) || defined(FM_USBCLK_BASE) \
        || defined(FM3_USBETHERCLK_BASE) || defined(FM4_USBETHERCLK_BASE)  \
        || defined(FM0P_USBETHERCLK_BASE) || defined(FM_USBETHERCLK_BASE)))
        #if !defined(__CLKMO)
            #error Please define __CLKMO in your system header file
        #elif __CLKMO == (4000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 24
            #define USBETHERNETCLOCK_PLL_M 2
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (4500000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 32
            #define USBETHERNETCLOCK_PLL_M 3
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (4800000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 20
            #define USBETHERNETCLOCK_PLL_M 2
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5760000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 25
            #define USBETHERNETCLOCK_PLL_M 3
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (6000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 16
            #define USBETHERNETCLOCK_PLL_M 2
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (6400000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 15
            #define USBETHERNETCLOCK_PLL_M 2
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (7200000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 20
            #define USBETHERNETCLOCK_PLL_M 3
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (8000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 12
            #define USBETHERNETCLOCK_PLL_M 2
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (9000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 16
            #define USBETHERNETCLOCK_PLL_M 3
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (9600000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 10
            #define USBETHERNETCLOCK_PLL_M 2
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (12000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 8
            #define USBETHERNETCLOCK_PLL_M 2
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (14400000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 10
            #define USBETHERNETCLOCK_PLL_M 3
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (16000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 6
            #define USBETHERNETCLOCK_PLL_M 2
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (18000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 8
            #define USBETHERNETCLOCK_PLL_M 3
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (19200000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 5
            #define USBETHERNETCLOCK_PLL_M 2
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (24000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 6
            #define USBETHERNETCLOCK_PLL_M 3
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (28800000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 5
            #define USBETHERNETCLOCK_PLL_M 3
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #else
            #error The external crystal defined in __CLKMO is not supported for USB PLL
        #endif /* __CLKMO == ... */
    /* USB only: USBCLK/USBETHERNELCLK (all except FM3 type 0,1,4,6,5,9,12) */
    #elif (!((defined(FM3_DEVICE_TYPE) && (FM3_DEVICE_TYPE == 0))) && \
       (defined(FM3_USBCLK_BASE) || defined(FM4_USBCLK_BASE) \
        || defined(FM0P_USBCLK_BASE) || defined(FM_USBCLK_BASE) \
        || defined(FM3_USBETHERCLK_BASE) || defined(FM4_USBETHERCLK_BASE)  \
        || defined(FM0P_USBETHERCLK_BASE) || defined(FM_USBETHERCLK_BASE)))
        #if !defined(__CLKMO)
            #error Please define __CLKMO in your system header file
        #elif ((__CLKMO == (4000000UL)) && (FM0P_DEVICE_TYPE == 2))
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 24
            #define USBETHERNETCLOCK_PLL_M 2
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (4000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 60
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (4500000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 64
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (4800000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 50
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (4881356UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 59
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 48
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5106383UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 47
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5760000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 50
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (6000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 40
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (6400000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 45
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (7200000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 40
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (7500000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 32
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (8000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 30
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (8888889UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 27
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (9000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 32
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (9600000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 25
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (10000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 24
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (10909091UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 22
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (11520000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 25
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (12000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 20
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (12631579UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 19
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (14400000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 20
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (15000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 16
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (15157895UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 19
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (16000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 15
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (18000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 16
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (19200000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 15
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_UCSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (48000000UL)
            #define USBETHERNETCLOCK_UCSEL 0x00 // no PLL needed for 48MHz crystal, using directly CLKMO
        #else
            #error The external crystal defined in __CLKMO is not supported for USB PLL
        #endif /* __CLKMO == ... */
    #endif /* USB only: USBCLK/USBETHERNELCLK (all except FM3 type 0) */

/* if Ethernet is using the USBETHERNETCLOCK */
#elif (USBETHERNETCLOCK_USE_ETHERNET == 1)


    /* Ethernet 25MHz: USBCLK/USBETHERNELCLK (all except FM3 type 0) */
    #if (ETHERNETCLOCK_OUTFREQ == FREQ_25MHZ) && (!((defined(FM3_DEVICE_TYPE) && (FM3_DEVICE_TYPE == 0))) && \
       (defined(FM3_USBCLK_BASE) || defined(FM4_USBCLK_BASE) \
        || defined(FM0P_USBCLK_BASE) || defined(FM_USBCLK_BASE) \
        || defined(FM3_USBETHERCLK_BASE) || defined(FM4_USBETHERCLK_BASE)  \
        || defined(FM0P_USBETHERCLK_BASE) || defined(FM_USBETHERCLK_BASE)))
        #if !defined(__CLKMO)
            #error Please define __CLKMO in your system header file
        #elif __CLKMO == (4000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 50
            #define USBETHERNETCLOCK_PLL_M 8
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (4296875UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 64
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (4385965UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 57
            #define USBETHERNETCLOCK_PLL_M 10
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (4435484UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 62
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (4500000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 50
            #define USBETHERNETCLOCK_PLL_M 9
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (4661017UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 59
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (4687500UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 48
            #define USBETHERNETCLOCK_PLL_M 9
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 40
            #define USBETHERNETCLOCK_PLL_M 8
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5102041UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 49
            #define USBETHERNETCLOCK_PLL_M 10
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5263158UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 38
            #define USBETHERNETCLOCK_PLL_M 8
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5319149UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 47
            #define USBETHERNETCLOCK_PLL_M 10
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5357143UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 42
            #define USBETHERNETCLOCK_PLL_M 9
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5392157UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 51
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5487805UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 41
            #define USBETHERNETCLOCK_PLL_M 9
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5500000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 50
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5612245UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 49
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5625000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 40
            #define USBETHERNETCLOCK_PLL_M 9
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5851064UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 47
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5882353UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 34
            #define USBETHERNETCLOCK_PLL_M 8
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5952381UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 42
            #define USBETHERNETCLOCK_PLL_M 10
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5978261UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 46
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (6000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 50
            #define USBETHERNETCLOCK_PLL_M 12
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (6097561UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 41
            #define USBETHERNETCLOCK_PLL_M 10
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (6250000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 32
            #define USBETHERNETCLOCK_PLL_M 8
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (6395349UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 43
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (6451613UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 31
            #define USBETHERNETCLOCK_PLL_M 8
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (6756757UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 37
            #define USBETHERNETCLOCK_PLL_M 10
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (6818182UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 33
            #define USBETHERNETCLOCK_PLL_M 9
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (6875000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 40
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (7031250UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 32
            #define USBETHERNETCLOCK_PLL_M 9
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (7500000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 30
            #define USBETHERNETCLOCK_PLL_M 9
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (7638889UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 36
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (7812500UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 32
            #define USBETHERNETCLOCK_PLL_M 10
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (7857143UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 35
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (8000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 25
            #define USBETHERNETCLOCK_PLL_M 8
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (8593750UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 32
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (8870968UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 31
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (9000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 25
            #define USBETHERNETCLOCK_PLL_M 9
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (9166667UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 30
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (9375000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 24
            #define USBETHERNETCLOCK_PLL_M 9
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (9782609UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 23
            #define USBETHERNETCLOCK_PLL_M 9
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (10000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 20
            #define USBETHERNETCLOCK_PLL_M 8
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (10227273UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 22
            #define USBETHERNETCLOCK_PLL_M 9
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (10416667UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 24
            #define USBETHERNETCLOCK_PLL_M 10
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (10526316UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 19
            #define USBETHERNETCLOCK_PLL_M 8
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (10714286UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 21
            #define USBETHERNETCLOCK_PLL_M 9
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (11000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 25
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (11250000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 20
            #define USBETHERNETCLOCK_PLL_M 9
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (11764706UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 17
            #define USBETHERNETCLOCK_PLL_M 8
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (11904762UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 21
            #define USBETHERNETCLOCK_PLL_M 10
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (11956522UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 23
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (12000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 25
            #define USBETHERNETCLOCK_PLL_M 12
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (12500000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 16
            #define USBETHERNETCLOCK_PLL_M 8
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (13157895UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 19
            #define USBETHERNETCLOCK_PLL_M 10
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (13750000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 20
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (13888889UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 18
            #define USBETHERNETCLOCK_PLL_M 10
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (14062500UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 16
            #define USBETHERNETCLOCK_PLL_M 9
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (15000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 15
            #define USBETHERNETCLOCK_PLL_M 9
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (15277778UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 18
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (15625000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 16
            #define USBETHERNETCLOCK_PLL_M 10
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (16071429UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 14
            #define USBETHERNETCLOCK_PLL_M 9
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (16176471UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 17
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (16666667UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 15
            #define USBETHERNETCLOCK_PLL_M 10
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (17187500UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 16
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (17857143UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 14
            #define USBETHERNETCLOCK_PLL_M 10
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (18333334UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 15
            #define USBETHERNETCLOCK_PLL_M 11
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (18750000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 16
            #define USBETHERNETCLOCK_PLL_M 12
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (20000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 15
            #define USBETHERNETCLOCK_PLL_M 12
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #else
            #error The external crystal defined in __CLKMO is not supported for Ethernet PLL
        #endif /* __CLKMO == ... */
    /* Ethernet 50MHz: USBCLK/USBETHERNELCLK (all except FM3 type 0) */
    #elif (ETHERNETCLOCK_OUTFREQ == FREQ_50MHZ) && (!((defined(FM3_DEVICE_TYPE) && (FM3_DEVICE_TYPE == 0))) && \
       (defined(FM3_USBCLK_BASE) || defined(FM4_USBCLK_BASE) \
        || defined(FM0P_USBCLK_BASE) || defined(FM_USBCLK_BASE) \
        || defined(FM3_USBETHERCLK_BASE) || defined(FM4_USBETHERCLK_BASE)  \
        || defined(FM0P_USBETHERCLK_BASE) || defined(FM_USBETHERCLK_BASE)))
        #if !defined(__CLKMO)
            #error Please define __CLKMO in your system header file
        #elif __CLKMO == (4000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 50
            #define USBETHERNETCLOCK_PLL_M 4
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (4687500UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 64
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 40
            #define USBETHERNETCLOCK_PLL_M 4
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5319149UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 47
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5882353UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 34
            #define USBETHERNETCLOCK_PLL_M 4
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (5952381UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 42
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (6000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 50
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (6097561UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 41
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (6250000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 32
            #define USBETHERNETCLOCK_PLL_M 4
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (6451613UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 31
            #define USBETHERNETCLOCK_PLL_M 4
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (7500000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 40
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (7812500UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 32
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (8000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 25
            #define USBETHERNETCLOCK_PLL_M 4
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (9375000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 32
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (10000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 20
            #define USBETHERNETCLOCK_PLL_M 4
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (11764706UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 17
            #define USBETHERNETCLOCK_PLL_M 4
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (11904762UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 21
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (12000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 25
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (12500000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 16
            #define USBETHERNETCLOCK_PLL_M 4
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (13888889UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 18
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (15000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 20
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (15625000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 16
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (17857143UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 14
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (18750000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 16
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #elif __CLKMO == (20000000UL)
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 15
            #define USBETHERNETCLOCK_PLL_M 6
            #define USBETHERNETCLOCK_ECSEL 0x01 // use USB/Ethernet-PLL oscillation clock
        #else
            #error The external crystal defined in __CLKMO is not supported for Ethernet PLL
        #endif /* __CLKMO == ... */
    #else
        #error ETHERNETCLOCK must be 25MHz or 50MHz
    #endif /* Ethernet 25/50MHz: USBCLK/USBETHERNELCLK (all except FM3 type 0) */


    #if (USBETHERNETCLOCK_USE_USB == 1)
        #if __CLKMO == FREQ_48MHZ
            #define USBETHERNETCLOCK_UCSEL 0x00 // CLKMO
        #elif __PLLCLK == FREQ_144MHZ
            #define USBETHERNETCLOCK_UCSEL 0x02    // CLKPLL division clock
            #define USBETHERNETCLOCK_UBSR  (3 - 1) // Clock divided by 3
        #elif __PLLCLK == FREQ_96MHz
            #define USBETHERNETCLOCK_UCSEL 0x02    // CLKPLL division clock
            #define USBETHERNETCLOCK_UBSR  (2 - 1) // Clock divided by 3
        #else
            #if (USBETHERNETCLOCK_USE_USB == 1)
                #error USB Clock Setting not supported
            #endif
        #endif
    #endif


#endif


/* Check Clock Settings */
#if USBETHERNETCLOCK_PLL_K > 63
    #error USB ETHERNET CLOCK: (K - 1) must be < 64
#endif

#if ((FM3MCUTYPE == FM3MCUTYPE_TYPE6) || (FM3MCUTYPE == FM3MCUTYPE_TYPE0))
    #if USBETHERNETCLOCK_PLL_N == 0
        #error USB ETHERNET CLOCK: (N - 1) cant be 0
    #endif
    #if USBETHERNETCLOCK_PLL_N > 63
        #error USB ETHERNET CLOCK: (N - 1) cant be > 63
    #endif
#endif
#if ((FM3MCUTYPE == FM3MCUTYPE_TYPE1) || (FM3MCUTYPE == FM3MCUTYPE_TYPE4) || (FM3MCUTYPE == FM3MCUTYPE_TYPE5))
    #if USBETHERNETCLOCK_PLL_N < 13
        #error USB ETHERNET CLOCK: (N - 1) must be > 12
    #endif
    #if USBETHERNETCLOCK_PLL_N > 99
        #error USB ETHERNET CLOCK: (N - 1) cant be > 99
    #endif
#endif

#if ((FM3MCUTYPE == FM3MCUTYPE_TYPE1) || (FM3MCUTYPE == FM3MCUTYPE_TYPE4) || (FM3MCUTYPE == FM3MCUTYPE_TYPE5))
    #if USBETHERNETCLOCK_PLL_M > 15
        #error USB ETHERNET CLOCK: (M - 1) cant be > 15
    #endif
#endif

#if ((FM3MCUTYPE == FM3MCUTYPE_TYPE6))
    #if USBETHERNETCLOCK_PLL_M > 15
        #error USB ETHERNET CLOCK: (M - 1) cant be > 15
    #endif
#endif
/* End Check Clock Settings */

#ifndef USBETHERNETCLOCK_UCSEL
    #define USBETHERNETCLOCK_UCSEL 0x00
#endif

#ifndef USBETHERNETCLOCK_ECSEL
    #define USBETHERNETCLOCK_ECSEL 0x00
#endif

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

void UsbEthernetClock_Init(void);
void UsbEthernetClock_Deinit(void);
#ifdef __cplusplus
}
#endif

//@} // UsbEthernetClockGroup

#endif /* __USBETHERNETCLOCK_H__ */

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/



