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
/** \file usbethernetclocklegacy.h
 **
 ** A detailed description is available at 
 ** @link UsbEthernetClockLegacyGroup USB Ethernet Clock legcy support for older libraries of L3, PDL or MCU template description @endlink
 **
 ** History:
 **   - 2015-11-05  V1.0  MSc  First Version
 *****************************************************************************/

#ifndef __USBETHERNETCLOCKLEGACY_H__
#define __USBETHERNETCLOCKLEGACY_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 ******************************************************************************
 ** \defgroup UsbEthernetClockLegacyGroup USB Ethernet Clock legcy support for older libraries of L3, PDL or MCU template
 **
 ******************************************************************************/
//@{
    
    
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "base_types.h"
#include "mcu.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/


#ifndef FM3MCUTYPE_TYPE0
    #define FM3MCUTYPE_TYPE0 0
#endif
#ifndef FM3MCUTYPE_TYPE1
    #define FM3MCUTYPE_TYPE1 1
#endif
#ifndef FM3MCUTYPE_TYPE2
    #define FM3MCUTYPE_TYPE2 2
#endif
#ifndef FM3MCUTYPE_TYPE3
    #define FM3MCUTYPE_TYPE3 3
#endif
#ifndef FM3MCUTYPE_TYPE4
    #define FM3MCUTYPE_TYPE4 4
#endif
#ifndef FM3MCUTYPE_TYPE5
    #define FM3MCUTYPE_TYPE5 5
#endif
#ifndef FM3MCUTYPE_TYPE6
    #define FM3MCUTYPE_TYPE6 6
#endif
#ifndef FM3MCUTYPE_TYPE7
    #define FM3MCUTYPE_TYPE7 7
#endif
#ifndef FM3MCUTYPE_TYPE8
    #define FM3MCUTYPE_TYPE8 8
#endif
#ifndef FM3MCUTYPE_TYPE9
    #define FM3MCUTYPE_TYPE9 9
#endif

#ifndef FM4MCUTYPE_TYPE0
    #define FM4MCUTYPE_TYPE0 0
#endif
#ifndef FM4MCUTYPE_TYPE1
    #define FM4MCUTYPE_TYPE1 1
#endif        
#ifndef FM4MCUTYPE_TYPE2
    #define FM4MCUTYPE_TYPE2 2
#endif
#ifndef FM4MCUTYPE_TYPE3
    #define FM4MCUTYPE_TYPE3 3
#endif
#ifndef FM4MCUTYPE_TYPE4
    #define FM4MCUTYPE_TYPE4 4
#endif
#ifndef FM4MCUTYPE_TYPE5
    #define FM4MCUTYPE_TYPE5 5
#endif
#ifndef FM4MCUTYPE_TYPE6
    #define FM4MCUTYPE_TYPE6 6
#endif

#ifdef FM3_DEVICE_TYPE 
    #ifndef FM3MCUTYPE
         #define FM3MCUTYPE FM3_DEVICE_TYPE
    #endif
#endif

#ifdef FM4_DEVICE_TYPE 
    #ifndef FM4MCUTYPE
         #define FM4MCUTYPE FM4_DEVICE_TYPE
    #endif
#endif

#if (0 == USBETHERNETCLOCK_USE_L3)
  #ifndef FM4MCUTYPE
    #ifdef  _MB9ABXXX_H_
        #define FM4MCUTYPE FM4MCUTYPE_TYPE0
    #endif
    #ifdef  _MB9B560R_H_
        #define FM4MCUTYPE FM4MCUTYPE_TYPE0
    #endif
    #ifdef _S6E2CC_H_
        #define FM4MCUTYPE FM4MCUTYPE_TYPE3
    #endif
  #endif
  #ifndef FM3MCUTYPE
  #ifdef _MB9B120K_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE9
  #endif
  #ifdef _MB9B320K_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE9
  #endif
  #ifdef _MB9B420K_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE9
  #endif
  #ifdef _MB9B520K_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE9
  #endif
  #ifdef _MB9B120L_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE9
  #endif
  #ifdef _MB9B320L_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE9
  #endif
  #ifdef _MB9B420L_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE9
  #endif
  #ifdef _MB9B520L_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE9
  #endif
  #ifdef _MB9B120M_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE9
  #endif
  #ifdef _MB9B320M_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE9
  #endif
  #ifdef _MB9B420M_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE9
  #endif
  #ifdef _MB9B520M_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE9
  #endif
  #ifdef _MB9B500N_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE0
  #endif
  #ifdef _MB9B400N_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE0
  #endif
  #ifdef _MB9B300N_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE0
  #endif
  #ifdef _MB9B100N_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE0
  #endif
  #ifdef _MB9B500R_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE0
  #endif
  #ifdef _MB9B400R_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE0
  #endif
  #ifdef _MB9B300R_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE0
  #endif
  #ifdef _MB9B100R_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE0
  #endif
  #ifdef _MB9BD10S_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE2
  #endif
  #ifdef _MB9B610S_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE2
  #endif
  #ifdef _MB9B510S_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE2
  #endif
  #ifdef _MB9B410S_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE2
  #endif
  #ifdef _MB9B310S_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE2
  #endif
  #ifdef _MB9B210S_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE2
  #endif
  #ifdef _MB9B110S_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE2
  #endif
  #ifdef _MB9BD10T_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE2
  #endif
  #ifdef _MB9B610T_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE2
  #endif
  #ifdef _MB9B210T_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE2
  #endif
  #ifdef _MB9B510T_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE2
  #endif
  #ifdef _MB9B410T_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE2
  #endif
  #ifdef _MB9B310T_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE2
  #endif
  #ifdef _MB9B110T_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE2
  #endif
  #ifdef _MB9B510N_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE4
  #endif
  #ifdef _MB9B410N_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE4
  #endif
  #ifdef _MB9B310N_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE4
  #endif
  #ifdef _MB9B110N_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE4
  #endif
  #ifdef _MB9B510R_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE4
  #endif
  #ifdef _MB9B410R_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE4
  #endif
  #ifdef _MB9B310R_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE4
  #endif
  #ifdef _MB9B110R_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE4
  #endif
  #ifdef _MB9A310K_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE5
  #endif
  #ifdef _MB9A110K_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE5
  #endif
  #ifdef _MB9A310L_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE1
  #endif
  #ifdef _MB9A110L_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE1
  #endif
  #ifdef _MB9A310M_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE1
  #endif
  #ifdef _MB9A110M_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE1
  #endif
  #ifdef _MB9A310N_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE1
  #endif
  #ifdef _MB9A100N_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE0
  #endif
  #ifdef _MB9A110N_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE1
  #endif
  #ifdef _MB9A100R_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE0
  #endif
  #ifdef _MB9AB40L_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE6
  #endif
  #ifdef _MB9AA40L_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE6
  #endif
  #ifdef _MB9A340L_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE6
  #endif
  #ifdef _MB9A140L_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE6
  #endif
  #ifdef _MB9AB40M_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE6
  #endif
  #ifdef _MB9AA40M_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE6
  #endif
  #ifdef _MB9A340M_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE6
  #endif
  #ifdef _MB9A140M_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE6
  #endif
  #ifdef _MB9AB40N_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE6
  #endif
  #ifdef _MB9AA40N_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE6
  #endif
  #ifdef _MB9A340N_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE6
  #endif
  #ifdef _MB9A140N_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE6
  #endif
  #ifdef _MB9A150M_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE8
  #endif
  #ifdef _MB9A150N_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE8
  #endif
  #ifdef _MB9A150R_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE8
  #endif
  #ifdef _MB9A130K_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE3
  #endif
  #ifdef _MB9A130L_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE3
  #endif
  #ifdef _MB9AA30L_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE7
  #endif
  #ifdef _MB9AA30M_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE7
  #endif
  #ifdef _MB9A130M_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE7
  #endif
  #ifdef _MB9AA30N_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE7
  #endif
  #ifdef _MB9A130N_H_
      #define FM3MCUTYPE FM3MCUTYPE_TYPE7
  #endif
  #endif
#else // (0 == USBETHERNETCLOCK_USE_L3)
  #ifndef FM3MCUTYPE
    #if (L3_DEVICE_TYPE == L3_TYPE0)
      #define FM3MCUTYPE FM3MCUTYPE_TYPE0
    #elif (L3_DEVICE_TYPE == L3_TYPE1)
      #define FM3MCUTYPE FM3MCUTYPE_TYPE1
    #elif (L3_DEVICE_TYPE == L3_TYPE2)
      #define FM3MCUTYPE FM3MCUTYPE_TYPE2
    #elif (L3_DEVICE_TYPE == L3_TYPE3)
      #define FM3MCUTYPE FM3MCUTYPE_TYPE3
    #elif (L3_DEVICE_TYPE == L3_TYPE4)
      #define FM3MCUTYPE FM3MCUTYPE_TYPE4
    #elif (L3_DEVICE_TYPE == L3_TYPE5)
      #define FM3MCUTYPE FM3MCUTYPE_TYPE5
    #elif (L3_DEVICE_TYPE == L3_TYPE6)
      #define FM3MCUTYPE FM3MCUTYPE_TYPE6
    #elif (L3_DEVICE_TYPE == L3_TYPE7)
      #define FM3MCUTYPE FM3MCUTYPE_TYPE7
    #elif (L3_DEVICE_TYPE == L3_TYPE9)
      #define FM3MCUTYPE FM3MCUTYPE_TYPE9
    #endif
  #endif
#endif // (0 == USBETHERNETCLOCK_USE_L3)

    /*
#if defined(FM3MCUTYPE) && (FM3MCUTYPE == FM3MCUTYPE_TYPE0)
    #if   MAINCRYSTALFREQ == FREQ_4MHZ
        #define USBETHERNETCLOCK_PLL_K 1
        #define USBETHERNETCLOCK_PLL_N 24
        #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
    #elif MAINCRYSTALFREQ == FREQ_8MHZ
        #define USBETHERNETCLOCK_PLL_K 1
        #define USBETHERNETCLOCK_PLL_N 12
        #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
    #elif MAINCRYSTALFREQ == FREQ_16MHZ
        #define USBETHERNETCLOCK_PLL_K 1
        #define USBETHERNETCLOCK_PLL_N 6
        #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
    #elif MAINCRYSTALFREQ == FREQ_24MHZ
        #define USBETHERNETCLOCK_PLL_K 2
        #define USBETHERNETCLOCK_PLL_N 8
        #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
    #elif MAINCRYSTALFREQ == FREQ_48MHZ
        #define USBETHERNETCLOCK_UCSEL 0x00 // CLKMO
    #else
        #error NOT SUPPORTED CRYSTAL / PLL SETTING FOR USB / ETHERNET CLOCK
    #endif
#elif ((FM3MCUTYPE == FM3MCUTYPE_TYPE1) || ((FM3MCUTYPE == FM3MCUTYPE_TYPE4) || (FM3MCUTYPE == FM3MCUTYPE_TYPE5)))
    #if   MAINCRYSTALFREQ == FREQ_4MHZ
        #define USBETHERNETCLOCK_PLL_K 1
        #define USBETHERNETCLOCK_PLL_N 60
        #define USBETHERNETCLOCK_PLL_M 5
        #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
    #elif MAINCRYSTALFREQ == FREQ_8MHZ
        #define USBETHERNETCLOCK_PLL_K 1
        #define USBETHERNETCLOCK_PLL_N 30
        #define USBETHERNETCLOCK_PLL_M 5
        #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
    #elif MAINCRYSTALFREQ == FREQ_16MHZ
        #define USBETHERNETCLOCK_PLL_K 1
        #define USBETHERNETCLOCK_PLL_N 15
        #define USBETHERNETCLOCK_PLL_M 5
        #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
    #elif MAINCRYSTALFREQ == FREQ_24MHZ
        #define USBETHERNETCLOCK_PLL_K 2
        #define USBETHERNETCLOCK_PLL_N 20
        #define USBETHERNETCLOCK_PLL_M 5
        #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
    #elif MAINCRYSTALFREQ == FREQ_48MHZ
        #define USBETHERNETCLOCK_UCSEL 0x00 // CLKMO
    #else
        #error NOT SUPPORTED CRYSTAL / PLL SETTING FOR USB / ETHERNET CLOCK
    #endif
#elif ((FM3MCUTYPE == FM3MCUTYPE_TYPE6) || (FM3MCUTYPE == FM3MCUTYPE_TYPE9))
    #if   MAINCRYSTALFREQ == FREQ_4MHZ
        #define USBETHERNETCLOCK_PLL_K 1
        #define USBETHERNETCLOCK_PLL_N 24
        #define USBETHERNETCLOCK_PLL_M 2
        #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
    #elif MAINCRYSTALFREQ == FREQ_8MHZ
        #define USBETHERNETCLOCK_PLL_K 1
        #define USBETHERNETCLOCK_PLL_N 12
        #define USBETHERNETCLOCK_PLL_M 2
        #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
    #elif MAINCRYSTALFREQ == FREQ_12MHZ
        #define USBETHERNETCLOCK_PLL_K 1
        #define USBETHERNETCLOCK_PLL_N 20
        #define USBETHERNETCLOCK_PLL_M 5
        #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
    #elif MAINCRYSTALFREQ == FREQ_16MHZ
        #define USBETHERNETCLOCK_PLL_K 1
        #define USBETHERNETCLOCK_PLL_N 6
        #define USBETHERNETCLOCK_PLL_M 2
        #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
    #elif MAINCRYSTALFREQ == FREQ_24MHZ
        #define USBETHERNETCLOCK_PLL_K 2
        #define USBETHERNETCLOCK_PLL_N 8
        #define USBETHERNETCLOCK_PLL_M 2
        #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
    #elif MAINCRYSTALFREQ == FREQ_48MHZ
        #define USBETHERNETCLOCK_UCSEL 0x00 // CLKMO
    #else
        #error NOT SUPPORTED CRYSTAL / PLL SETTING FOR USB / ETHERNET CLOCK
    #endif
#elif (FM3MCUTYPE == FM3MCUTYPE_TYPE2)
    #if ((USBETHERNETCLOCK_USE_USB == 1) && (USBETHERNETCLOCK_USE_ETHERNET == 0))
        #if   MAINCRYSTALFREQ == FREQ_4MHZ
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 60
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
        #elif MAINCRYSTALFREQ == FREQ_8MHZ
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 30
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
        #elif MAINCRYSTALFREQ == FREQ_16MHZ
            #define USBETHERNETCLOCK_PLL_K 1
            #define USBETHERNETCLOCK_PLL_N 15
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
        #elif MAINCRYSTALFREQ == FREQ_24MHZ
            #define USBETHERNETCLOCK_PLL_K 2
            #define USBETHERNETCLOCK_PLL_N 20
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
        #elif MAINCRYSTALFREQ == FREQ_25MHZ
            #define USBETHERNETCLOCK_PLL_K 2
            #define USBETHERNETCLOCK_PLL_N 48
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
        #elif MAINCRYSTALFREQ == FREQ_48MHZ
            #define USBETHERNETCLOCK_UCSEL 0x00 // CLKMO
        #elif MAINCRYSTALFREQ == FREQ_50MHZ
            #define USBETHERNETCLOCK_PLL_K 10
            #define USBETHERNETCLOCK_PLL_N 48
            #define USBETHERNETCLOCK_PLL_M 5
            #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
        #else
            #error NOT SUPPORTED CRYSTAL / PLL SETTING FOR USB / ETHERNET CLOCK
        #endif
    #else
        #if ETHERNETCLOCK_OUTFREQ == FREQ_25MHZ
            #if   MAINCRYSTALFREQ == FREQ_4MHZ
                #define USBETHERNETCLOCK_PLL_K 1
                #define USBETHERNETCLOCK_PLL_N 50
                #define USBETHERNETCLOCK_PLL_M 8
                #define USBETHERNETCLOCK_ECSEL 0x01 // USB/Ethernet-PLL oscillation clock
            #elif MAINCRYSTALFREQ == FREQ_8MHZ
                #define USBETHERNETCLOCK_PLL_K 1
                #define USBETHERNETCLOCK_PLL_N 25
                #define USBETHERNETCLOCK_PLL_M 8
                #define USBETHERNETCLOCK_ECSEL 0x01 // USB/Ethernet-PLL oscillation clock
            #elif MAINCRYSTALFREQ == FREQ_16MHZ
                #define USBETHERNETCLOCK_PLL_K 2
                #define USBETHERNETCLOCK_PLL_N 25
                #define USBETHERNETCLOCK_PLL_M 8
                #define USBETHERNETCLOCK_ECSEL 0x01 // USB/Ethernet-PLL oscillation clock
            #elif MAINCRYSTALFREQ == FREQ_24MHZ
                #define USBETHERNETCLOCK_PLL_K 6
                #define USBETHERNETCLOCK_PLL_N 50
                #define USBETHERNETCLOCK_PLL_M 8
                #define USBETHERNETCLOCK_ECSEL 0x01 // USB/Ethernet-PLL oscillation clock
            #elif MAINCRYSTALFREQ == FREQ_50MHZ
                #define USBETHERNETCLOCK_ECSEL 0x00 // CLKMO
            #elif MAINCRYSTALFREQ == FREQ_48MHZ
                #define USBETHERNETCLOCK_PLL_K 6
                #define USBETHERNETCLOCK_PLL_N 25
                #define USBETHERNETCLOCK_PLL_M 8
                #define USBETHERNETCLOCK_ECSEL 0x01 // USB/Ethernet-PLL oscillation clock
            #elif MAINCRYSTALFREQ == FREQ_50MHZ
                #define USBETHERNETCLOCK_PLL_K 5
                #define USBETHERNETCLOCK_PLL_N 20
                #define USBETHERNETCLOCK_PLL_M 8
                #define USBETHERNETCLOCK_ECSEL 0x01 // USB/Ethernet-PLL oscillation clock
            #else
                #error NOT SUPPORTED CRYSTAL / PLL SETTING FOR USB / ETHERNET CLOCK
            #endif
        #elif ETHERNETCLOCK_OUTFREQ == FREQ_50MHZ
            #if   MAINCRYSTALFREQ == FREQ_4MHZ
                #define USBETHERNETCLOCK_PLL_K 1
                #define USBETHERNETCLOCK_PLL_N 50
                #define USBETHERNETCLOCK_PLL_M 4
                #define USBETHERNETCLOCK_ECSEL 0x01 // USB/Ethernet-PLL oscillation clock
            #elif MAINCRYSTALFREQ == FREQ_8MHZ
                #define USBETHERNETCLOCK_PLL_K 1
                #define USBETHERNETCLOCK_PLL_N 25
                #define USBETHERNETCLOCK_PLL_M 4
                #define USBETHERNETCLOCK_ECSEL 0x01 // USB/Ethernet-PLL oscillation clock
            #elif MAINCRYSTALFREQ == FREQ_16MHZ
                #define USBETHERNETCLOCK_PLL_K 2
                #define USBETHERNETCLOCK_PLL_N 25
                #define USBETHERNETCLOCK_PLL_M 4
                #define USBETHERNETCLOCK_ECSEL 0x01 // USB/Ethernet-PLL oscillation clock
            #elif MAINCRYSTALFREQ == FREQ_24MHZ
                #define USBETHERNETCLOCK_PLL_K 3
                #define USBETHERNETCLOCK_PLL_N 50
                #define USBETHERNETCLOCK_PLL_M 4
                #define USBETHERNETCLOCK_ECSEL 0x01 // USB/Ethernet-PLL oscillation clock
            #elif MAINCRYSTALFREQ == FREQ_25MHZ
                #define USBETHERNETCLOCK_PLL_K 5
                #define USBETHERNETCLOCK_PLL_N 40
                #define USBETHERNETCLOCK_PLL_M 4
                #define USBETHERNETCLOCK_ECSEL 0x01 // USB/Ethernet-PLL oscillation clock
            #elif MAINCRYSTALFREQ == FREQ_48MHZ
                #define USBETHERNETCLOCK_PLL_K 6
                #define USBETHERNETCLOCK_PLL_N 25
                #define USBETHERNETCLOCK_PLL_M 4
                #define USBETHERNETCLOCK_ECSEL 0x01 // USB/Ethernet-PLL oscillation clock
            #else
                #error NOT SUPPORTED CRYSTAL / PLL SETTING FOR USB / ETHERNET CLOCK
            #endif
        #else
            #error ETHERNETCLOCK must be 25MHz or 50MHz
        #endif
 
        #if MAINCRYSTALFREQ == FREQ_48MHZ
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
#elif defined(FM4MCUTYPE) && ((FM4MCUTYPE == FM4MCUTYPE_TYPE0) || (FM4MCUTYPE == FM4MCUTYPE_TYPE3))
     #if   MAINCRYSTALFREQ == FREQ_4MHZ
        #define USBETHERNETCLOCK_PLL_K 1
        #define USBETHERNETCLOCK_PLL_N 60
        #define USBETHERNETCLOCK_PLL_M 5
        #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
    #elif MAINCRYSTALFREQ == FREQ_8MHZ
        #define USBETHERNETCLOCK_PLL_K 1
        #define USBETHERNETCLOCK_PLL_N 30
        #define USBETHERNETCLOCK_PLL_M 5
        #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
    #elif MAINCRYSTALFREQ == FREQ_16MHZ
        #define USBETHERNETCLOCK_PLL_K 1
        #define USBETHERNETCLOCK_PLL_N 15
        #define USBETHERNETCLOCK_PLL_M 5
        #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
    #elif MAINCRYSTALFREQ == FREQ_24MHZ
        #define USBETHERNETCLOCK_PLL_K 2
        #define USBETHERNETCLOCK_PLL_N 20
        #define USBETHERNETCLOCK_PLL_M 5
        #define USBETHERNETCLOCK_UCSEL 0x01 // USB/Ethernet-PLL oscillation clock
    #elif MAINCRYSTALFREQ == FREQ_48MHZ
        #define USBETHERNETCLOCK_UCSEL 0x00 // CLKMO
    #else
        #error NOT SUPPORTED CRYSTAL / PLL SETTING FOR USB / ETHERNET CLOCK
    #endif
#else
    #error FM MCU TYPE UNKNOWN
#endif
*/  
#ifndef L3_PERIPHERAL_ENABLE_USB0
    #define L3_PERIPHERAL_ENABLE_USB0 L3_OFF
#endif
  
#ifndef L3_PERIPHERAL_ENABLE_USB1
    #define L3_PERIPHERAL_ENABLE_USB1 L3_OFF
#endif

#ifndef __IO 
  #define     __IO    volatile             /*!< Defines 'read / write' permissions              */
#endif
  
#ifndef FM3_PERIPH_BASE
  #define FM3_PERIPH_BASE       (0x40000000UL)                 /* Peripheral  Base                       */
#endif
 
 
 
#if (FM3MCUTYPE == FM3MCUTYPE_TYPE2) 
/************************************************************************************************************/
/* FM3 Type 2 Ethernet USB Clock                                                                                     */
/************************************************************************************************************/
  #ifdef FM3_USBCLK
    #undef FM3_USBCLK
    #warning You are using a mcu header file that is not valid for your choosen mcu type! (Operation of this module may work)
  #endif
  #ifdef FM3_USBCLK_BASE
    #undef FM3_USBCLK_BASE
  #endif
  #ifdef bFM3_USBCLK_UPCR1_UPLLEN
    #undef bFM3_USBCLK_UPCR1_UPLLEN
  #endif
  
  #ifndef FM3_USBETHERNETCLK_BASE
    #define FM3_USBETHERNETCLK_BASE (FM3_PERIPH_BASE + 0x36000UL)  /* USB clock registers                    */
  #endif
  #ifdef FM3_USBETHERNETCLK
    #undef FM3_USBETHERNETCLK
    #define FM3_USBETHERNETCLK ((stc_usb_ethernetclkn_t *)FM3_USBETHERNETCLK_BASE)
  #endif
  #ifndef FM3_USBETHERNETCLK
      #define FM3_USBETHERNETCLK ((stc_usb_ethernetclkn_t *)FM3_USBETHERNETCLK_BASE)
  #endif
  
  #ifdef FM3_USBETHERNETCLK
    #define FM_USBETHERCLK      ((stc_usb_ethernetclkn_t *)FM3_USBETHERNETCLK)
  #endif
  /******************************************************************************
   * USB Ethernet CLK
   ******************************************************************************/
  /* USB ETHERNET CLK register bit fields */
  typedef struct stc_usbethernetclkn_uccr_field
  {
    __IO  uint8_t UCEN0      : 1;
    __IO  uint8_t UCSEL0     : 1;
    __IO  uint8_t UCSEL1     : 1;
    __IO  uint8_t UCEN1      : 1;
    __IO  uint8_t ECEN       : 1;
    __IO  uint8_t ECSEL0     : 1;
    __IO  uint8_t ECSEL1     : 1;
  } stc_usbethernetclkn_uccr_field_t;

  typedef struct stc_usbethernetclkn_upcr1_field
  {
    __IO  uint8_t UPLLEN     : 1;
    __IO  uint8_t UPINC      : 1;
  } stc_usbethernetclkn_upcr1_field_t;

  typedef struct stc_usbethernetclkn_upcr2_field
  {
    __IO  uint8_t UPOWT0     : 1;
    __IO  uint8_t UPOWT1     : 1;
    __IO  uint8_t UPOWT2     : 1;
  } stc_usbethernetclkn_upcr2_field_t;

  typedef struct stc_usbethernetclkn_upcr3_field
  {
    __IO  uint8_t UPLLK0     : 1;
    __IO  uint8_t UPLLK1     : 1;
    __IO  uint8_t UPLLK2     : 1;
    __IO  uint8_t UPLLK3     : 1;
    __IO  uint8_t UPLLK4     : 1;
  } stc_usbethernetclkn_upcr3_field_t;

  typedef struct stc_usbethernetclkn_upcr4_field
  {
    __IO  uint8_t UPLLN0     : 1;
    __IO  uint8_t UPLLN1     : 1;
    __IO  uint8_t UPLLN2     : 1;
    __IO  uint8_t UPLLN3     : 1;
    __IO  uint8_t UPLLN4     : 1;
    __IO  uint8_t UPLLN5     : 1;
    __IO  uint8_t UPLLN6     : 1;
  } stc_usbethernetclkn_upcr4_field_t;

  typedef struct stc_usbethernetclkn_up_str_field
  {
    __IO  uint8_t UPRDY      : 1;
  } stc_usbethernetclkn_up_str_field_t;

  typedef struct stc_usbethernetclkn_upint_enr_field
  {
    __IO  uint8_t UPCSE      : 1;
  } stc_usbethernetclkn_upint_enr_field_t;

  typedef struct stc_usbethernetclkn_upint_clr_field
  {
    __IO  uint8_t UPCSC      : 1;
  } stc_usbethernetclkn_upint_clr_field_t;

  typedef struct stc_usbethernetclkn_upint_str_field
  {
    __IO  uint8_t UPCSI      : 1;
  } stc_usbethernetclkn_upint_str_field_t;

  typedef struct stc_usbethernetclkn_upcr5_field
  {
    __IO  uint8_t UPLLM0     : 1;
    __IO  uint8_t UPLLM1     : 1;
    __IO  uint8_t UPLLM2     : 1;
    __IO  uint8_t UPLLM3     : 1;
  } stc_usbethernetclkn_upcr5_field_t;

  typedef struct stc_usbethernetclkn_upcr6_field
  {
    __IO  uint8_t UBSR0      : 1;
    __IO  uint8_t UBSR1      : 1;
    __IO  uint8_t UBSR2      : 1;
    __IO  uint8_t UBSR3      : 1;
  } stc_usbethernetclkn_upcr6_field_t;

  typedef struct stc_usbethernetclkn_upcr7_field
  {
    __IO  uint8_t EPLLEN     : 1;
  } stc_usbethernetclkn_upcr7_field_t;

  typedef struct stc_usbethernetclkn_usben0_field
  {
    __IO  uint8_t USBEN0     : 1;
  } stc_usbethernetclkn_usben0_field_t;

  typedef struct stc_usbethernetclkn_usben1_field
  {
    __IO  uint8_t USBEN1     : 1;
  } stc_usbethernetclkn_usben1_field_t;


  /******************************************************************************
   * USBETHERNETCLK
   ******************************************************************************/
  /* USB Ethernet clock registers */
  typedef struct stc_usb_ethernetclkn
  {
    union {
      __IO  uint8_t UCCR;
      stc_usbethernetclkn_uccr_field_t UCCR_f;
    };
          uint8_t RESERVED0[3];
    union {
      __IO  uint8_t UPCR1;
      stc_usbethernetclkn_upcr1_field_t UPCR1_f;
    };
          uint8_t RESERVED1[3];
    union {
      __IO  uint8_t UPCR2;
      stc_usbethernetclkn_upcr2_field_t UPCR2_f;
    };
          uint8_t RESERVED2[3];
    union {
      __IO  uint8_t UPCR3;
      stc_usbethernetclkn_upcr3_field_t UPCR3_f;
    };
          uint8_t RESERVED3[3];
    union {
      __IO  uint8_t UPCR4;
      stc_usbethernetclkn_upcr4_field_t UPCR4_f;
    };
          uint8_t RESERVED4[3];
    union {
      __IO  uint8_t UP_STR;
      stc_usbethernetclkn_up_str_field_t UP_STR_f;
    };
          uint8_t RESERVED5[3];
    union {
      __IO  uint8_t UPINT_ENR;
      stc_usbethernetclkn_upint_enr_field_t UPINT_ENR_f;
    };
          uint8_t RESERVED6[3];
    union {
      __IO  uint8_t UPINT_CLR;
      stc_usbethernetclkn_upint_clr_field_t UPINT_CLR_f;
    };
          uint8_t RESERVED7[3];
    union {
      __IO  uint8_t UPINT_STR;
      stc_usbethernetclkn_upint_str_field_t UPINT_STR_f;
    };
          uint8_t RESERVED8[3];
    union {
      __IO  uint8_t UPCR5;
      stc_usbethernetclkn_upcr5_field_t UPCR5_f;
    };
          uint8_t RESERVED9[3];
    union {
      __IO  uint8_t UPCR6;
      stc_usbethernetclkn_upcr6_field_t UPCR6_f;
    };
          uint8_t RESERVED10[3];
    union {
      __IO  uint8_t UPCR7;
      stc_usbethernetclkn_upcr7_field_t UPCR7_f;
    };
          uint8_t RESERVED11[3];
    union {
      __IO  uint8_t USBEN0;
      stc_usbethernetclkn_usben0_field_t USBEN0_f;
    };
          uint8_t RESERVED12[3];
    union {
      __IO  uint8_t USBEN1;
      stc_usbethernetclkn_usben1_field_t USBEN1_f;
    };
  } stc_usb_ethernetclkn_t;
  
  /* USB clock registers */
 

  #define bFM_USBETHERCLK_UCCR_UCEN0         *((volatile unsigned int*)(0x426C0000UL))
  #define bFM_USBETHERCLK_UCCR_UCSEL0        *((volatile unsigned int*)(0x426C0004UL))
  #define bFM_USBETHERCLK_UCCR_UCSEL1        *((volatile unsigned int*)(0x426C0008UL))
  #define bFM_USBETHERCLK_UCCR_UCEN1         *((volatile unsigned int*)(0x426C000CUL))
  #define bFM_USBETHERCLK_UCCR_ECEN          *((volatile unsigned int*)(0x426C0010UL)) 
  #define bFM_USBETHERCLK_UCCR_ECSEL0        *((volatile unsigned int*)(0x426C0014UL)) 
  #define bFM_USBETHERCLK_UCCR_ECSEL1        *((volatile unsigned int*)(0x426C0018UL)) 
  #define bFM_USBETHERCLK_UPCR1_UPLLEN       *((volatile unsigned int*)(0x426C0080UL))
  #define bFM_USBETHERCLK_UPCR1_UPINC        *((volatile unsigned int*)(0x426C0084UL)) 
  #define bFM_USBETHERCLK_UPCR2_UPOWT0       *((volatile unsigned int*)(0x426C0100UL)) 
  #define bFM_USBETHERCLK_UPCR2_UPOWT1       *((volatile unsigned int*)(0x426C0104UL))  
  #define bFM_USBETHERCLK_UPCR2_UPOWT2       *((volatile unsigned int*)(0x426C0108UL))  
  #define bFM_USBETHERCLK_UPCR3_UPLLK0       *((volatile unsigned int*)(0x426C0180UL)) 
  #define bFM_USBETHERCLK_UPCR3_UPLLK1       *((volatile unsigned int*)(0x426C0184UL))  
  #define bFM_USBETHERCLK_UPCR3_UPLLK2       *((volatile unsigned int*)(0x426C0188UL))  
  #define bFM_USBETHERCLK_UPCR3_UPLLK3       *((volatile unsigned int*)(0x426C018CUL))
  #define bFM_USBETHERCLK_UPCR3_UPLLK4       *((volatile unsigned int*)(0x426C0190UL)) 
  #define bFM_USBETHERCLK_UPCR4_UPLLN0       *((volatile unsigned int*)(0x426C0200UL)) 
  #define bFM_USBETHERCLK_UPCR4_UPLLN1       *((volatile unsigned int*)(0x426C0204UL))
  #define bFM_USBETHERCLK_UPCR4_UPLLN2       *((volatile unsigned int*)(0x426C0208UL))
  #define bFM_USBETHERCLK_UPCR4_UPLLN3       *((volatile unsigned int*)(0x426C020CUL))
  #define bFM_USBETHERCLK_UPCR4_UPLLN4       *((volatile unsigned int*)(0x426C0210UL))
  #define bFM_USBETHERCLK_UPCR4_UPLLN5       *((volatile unsigned int*)(0x426C0214UL))
  #define bFM_USBETHERCLK_UPCR4_UPLLN6       *((volatile unsigned int*)(0x426C0218UL))
  #define bFM_USBETHERCLK_UP_STR_UPRDY       *((volatile unsigned int*)(0x426C0280UL))
  #define bFM_USBETHERCLK_UPINT_ENR_UPCSE    *((volatile unsigned int*)(0x426C0300UL))
  #define bFM_USBETHERCLK_UPINT_CLR_UPCSC    *((volatile unsigned int*)(0x426C0380UL))
  #define bFM_USBETHERCLK_UPINT_STR_UPCSI    *((volatile unsigned int*)(0x426C0400UL))
  #define bFM_USBETHERCLK_UPCR5_UPLLM0       *((volatile unsigned int*)(0x426C0480UL))
  #define bFM_USBETHERCLK_UPCR5_UPLLM1       *((volatile unsigned int*)(0x426C0484UL))
  #define bFM_USBETHERCLK_UPCR5_UPLLM2       *((volatile unsigned int*)(0x426C0488UL))
  #define bFM_USBETHERCLK_UPCR5_UPLLM3       *((volatile unsigned int*)(0x426C048CUL))
  #define bFM_USBETHERCLK_UPCR6_UBSR0        *((volatile unsigned int*)(0x426C0500UL))
  #define bFM_USBETHERCLK_UPCR6_UBSR1        *((volatile unsigned int*)(0x426C0504UL))
  #define bFM_USBETHERCLK_UPCR6_UBSR2        *((volatile unsigned int*)(0x426C0508UL))
  #define bFM_USBETHERCLK_UPCR6_UBSR3        *((volatile unsigned int*)(0x426C050CUL))
  #define bFM_USBETHERCLK_UPCR7_EPLLEN       *((volatile unsigned int*)(0x426C0580UL))
  #define bFM_USBETHERCLK_USBEN0_USBEN0      *((volatile unsigned int*)(0x426C0600UL))
  #define bFM_USBETHERCLK_USBEN1_USBEN1      *((volatile unsigned int*)(0x426C0680UL))
  
#elif defined(FM3MCUTYPE) && (FM3MCUTYPE == FM3MCUTYPE_TYPE0)
/************************************************************************************************************/
/* FM3 Type 0 USB Clock                                                                                     */
/************************************************************************************************************/
  #ifdef FM3_USBETHERNETCLK
    #undef FM3_USBETHERNETCLK
    #warning You are using a mcu header file that is not valid for your choosen mcu type! (Operation of this module may work)
  #endif
  #ifdef FM3_USBETHERNETCLK_BASE
    #undef FM3_USBETHERNETCLK_BASE
  #endif
      
  #ifndef FM3_USBCLK_BASE
    #define FM3_USBCLK_BASE       (FM3_PERIPH_BASE + 0x36000UL)  /* USB clock registers */
  #endif
  #ifdef FM3_USBCLK
    #define FM_USBCLK      ((stc_usbclkn_t *)FM3_USBCLK_BASE)
  #endif
  /******************************************************************************
   * USBCLK
   ******************************************************************************/
  /* USBCLK register bit fields */
  typedef struct stc_usbclkn_uccr_field
  {
    __IO  uint8_t UCEN       : 1;
    __IO  uint8_t UCSEL      : 1;
  } stc_usbclkn_uccr_field_t;

  typedef struct stc_usbclkn_upcr1_field
  {
    __IO  uint8_t UPLLEN     : 1;
    __IO  uint8_t UPINC      : 1;
  } stc_usbclkn_upcr1_field_t;

  typedef struct stc_usbclkn_upcr2_field
  {
    __IO  uint8_t UPOWT0     : 1;
    __IO  uint8_t UPOWT1     : 1;
    __IO  uint8_t UPOWT2     : 1;
  } stc_usbclkn_upcr2_field_t;

  typedef struct stc_usbclkn_upcr3_field
  {
    __IO  uint8_t UPLLK0     : 1;
    __IO  uint8_t UPLLK1     : 1;
    __IO  uint8_t UPLLK2     : 1;
    __IO  uint8_t UPLLK3     : 1;
    __IO  uint8_t UPLLK4     : 1;
  } stc_usbclkn_upcr3_field_t;

  typedef struct stc_usbclkn_upcr4_field
  {
    __IO  uint8_t UPLLN0     : 1;
    __IO  uint8_t UPLLN1     : 1;
    __IO  uint8_t UPLLN2     : 1;
    __IO  uint8_t UPLLN3     : 1;
    __IO  uint8_t UPLLN4     : 1;
  } stc_usbclkn_upcr4_field_t;

  typedef struct stc_usbclkn_up_str_field
  {
    __IO  uint8_t UPRDY      : 1;
  } stc_usbclkn_up_str_field_t;

  typedef struct stc_usbclkn_upint_enr_field
  {
    __IO  uint8_t UPCSE      : 1;
  } stc_usbclkn_upint_enr_field_t;

  typedef struct stc_usbclkn_upint_clr_field
  {
    __IO  uint8_t UPCSC      : 1;
  } stc_usbclkn_upint_clr_field_t;

  typedef struct stc_usbclkn_upint_str_field
  {
    __IO  uint8_t UPCSI      : 1;
  } stc_usbclkn_upint_str_field_t;

  typedef struct stc_usbclkn_usben_field
  {
    __IO  uint8_t USBEN      : 1;
  } stc_usbclkn_usben_field_t;
  
  
  /******************************************************************************
   * USBCLK
   ******************************************************************************/
  /* USB clock registers */
  typedef struct
  {
    union {
      __IO  uint8_t UCCR;
      stc_usbclkn_uccr_field_t UCCR_f;
    };
          uint8_t RESERVED0[3];
    union {
      __IO  uint8_t UPCR1;
      stc_usbclkn_upcr1_field_t UPCR1_f;
    };
          uint8_t RESERVED1[3];
    union {
      __IO  uint8_t UPCR2;
      stc_usbclkn_upcr2_field_t UPCR2_f;
    };
          uint8_t RESERVED2[3];
    union {
      __IO  uint8_t UPCR3;
      stc_usbclkn_upcr3_field_t UPCR3_f;
    };
          uint8_t RESERVED3[3];
    union {
      __IO  uint8_t UPCR4;
      stc_usbclkn_upcr4_field_t UPCR4_f;
    };
          uint8_t RESERVED4[3];
    union {
      __IO  uint8_t UP_STR;
      stc_usbclkn_up_str_field_t UP_STR_f;
    };
          uint8_t RESERVED5[3];
    union {
      __IO  uint8_t UPINT_ENR;
      stc_usbclkn_upint_enr_field_t UPINT_ENR_f;
    };
          uint8_t RESERVED6[3];
    union {
      __IO  uint8_t UPINT_CLR;
      stc_usbclkn_upint_clr_field_t UPINT_CLR_f;
    };
          uint8_t RESERVED7[3];
    union {
      __IO  uint8_t UPINT_STR;
      stc_usbclkn_upint_str_field_t UPINT_STR_f;
    };
          uint8_t RESERVED8[15];
    union {
      __IO  uint8_t USBEN;
      stc_usbclkn_usben_field_t USBEN_f;
    };
  } stc_usbclkn_t;
  
  #define bFM_USBCLK_UCCR_UCEN                  *((volatile unsigned int*)(0x426C0000UL))
  #define bFM_USBCLK_UCCR_UCSEL                 *((volatile unsigned int*)(0x426C0004UL))
  #define bFM_USBCLK_UPCR1_UPLLEN               *((volatile unsigned int*)(0x426C0080UL))
  #define bFM_USBCLK_UPCR1_UPINC                *((volatile unsigned int*)(0x426C0084UL))
  #define bFM_USBCLK_UPCR2_UPOWT0               *((volatile unsigned int*)(0x426C0100UL)) 
  #define bFM_USBCLK_UPCR2_UPOWT1               *((volatile unsigned int*)(0x426C0104UL))   
  #define bFM_USBCLK_UPCR2_UPOWT2               *((volatile unsigned int*)(0x426C0108UL))   
  #define bFM_USBCLK_UPCR3_UPLLK0               *((volatile unsigned int*)(0x426C0180UL))   
  #define bFM_USBCLK_UPCR3_UPLLK1               *((volatile unsigned int*)(0x426C0184UL))   
  #define bFM_USBCLK_UPCR3_UPLLK2               *((volatile unsigned int*)(0x426C0188UL))   
  #define bFM_USBCLK_UPCR3_UPLLK3               *((volatile unsigned int*)(0x426C018CUL))   
  #define bFM_USBCLK_UPCR3_UPLLK4               *((volatile unsigned int*)(0x426C0190UL))   
  #define bFM_USBCLK_UPCR4_UPLLN0               *((volatile unsigned int*)(0x426C0200UL))  
  #define bFM_USBCLK_UPCR4_UPLLN1               *((volatile unsigned int*)(0x426C0204UL))   
  #define bFM_USBCLK_UPCR4_UPLLN2               *((volatile unsigned int*)(0x426C0208UL)) 
  #define bFM_USBCLK_UPCR4_UPLLN3               *((volatile unsigned int*)(0x426C020CUL))   
  #define bFM_USBCLK_UPCR4_UPLLN4               *((volatile unsigned int*)(0x426C0210UL))   
  #define bFM_USBCLK_UP_STR_UPRDY               *((volatile unsigned int*)(0x426C0280UL))  
  #define bFM_USBCLK_UPINT_ENR_UPCSE            *((volatile unsigned int*)(0x426C0300UL))    
  #define bFM_USBCLK_UPINT_CLR_UPCSC            *((volatile unsigned int*)(0x426C0380UL))   
  #define bFM_USBCLK_UPINT_STR_UPCSI            *((volatile unsigned int*)(0x426C0400UL))  
  #define bFM_USBCLK_USBEN_USBEN                *((volatile unsigned int*)(0x426C0600UL))
  
#elif defined(FM4MCUTYPE) && (FM4MCUTYPE == FM4MCUTYPE_TYPE3) 
/************************************************************************************************************/
/* FM3 Type 2 Ethernet USB Clock                                                                                     */
/************************************************************************************************************/
  #ifdef FM4_USBCLK
    #undef FM4_USBCLK
    #warning You are using a mcu header file that is not valid for your choosen mcu type! (Operation of this module may work)
  #endif
  #ifdef FM4_USBCLK_BASE
    #undef FM4_USBCLK_BASE
  #endif
  #ifdef bFM4_USBCLK_UPCR1_UPLLEN
    #undef bFM4_USBCLK_UPCR1_UPLLEN
  #endif
  #ifndef FM4_USBETHERNETCLK_BASE
    #define FM4_USBETHERNETCLK_BASE (FM4_PERIPH_BASE + 0x36000UL)  /* USB clock registers                    */
  #endif
  #ifdef FM4_USBETHERNETCLK
    #undef FM4_USBETHERNETCLK
    #define FM4_USBETHERNETCLK ((stc_usb_ethernetclkn_t *)FM4_USBETHERNETCLK_BASE)
  #endif
  #ifndef FM4_USBETHERNETCLK
      #define FM4_USBETHERNETCLK ((stc_usb_ethernetclkn_t *)FM4_USBETHERNETCLK_BASE)
  #endif
  
  #ifdef FM4_USBETHERNETCLK
    #define FM_USBETHERCLK      ((stc_usb_ethernetclkn_t *)FM4_USBETHERNETCLK)
  #endif
  /******************************************************************************
   * USB Ethernet CLK
   ******************************************************************************/
  /* USB ETHERNET CLK register bit fields */
  typedef struct stc_usbethernetclkn_uccr_field
  {
    __IO  uint8_t UCEN0      : 1;
    __IO  uint8_t UCSEL0     : 1;
    __IO  uint8_t UCSEL1     : 1;
    __IO  uint8_t UCEN1      : 1;
    __IO  uint8_t ECEN       : 1;
    __IO  uint8_t ECSEL0     : 1;
    __IO  uint8_t ECSEL1     : 1;
  } stc_usbethernetclkn_uccr_field_t;

  typedef struct stc_usbethernetclkn_upcr1_field
  {
    __IO  uint8_t UPLLEN     : 1;
    __IO  uint8_t UPINC      : 1;
  } stc_usbethernetclkn_upcr1_field_t;

  typedef struct stc_usbethernetclkn_upcr2_field
  {
    __IO  uint8_t UPOWT0     : 1;
    __IO  uint8_t UPOWT1     : 1;
    __IO  uint8_t UPOWT2     : 1;
  } stc_usbethernetclkn_upcr2_field_t;

  typedef struct stc_usbethernetclkn_upcr3_field
  {
    __IO  uint8_t UPLLK0     : 1;
    __IO  uint8_t UPLLK1     : 1;
    __IO  uint8_t UPLLK2     : 1;
    __IO  uint8_t UPLLK3     : 1;
    __IO  uint8_t UPLLK4     : 1;
  } stc_usbethernetclkn_upcr3_field_t;

  typedef struct stc_usbethernetclkn_upcr4_field
  {
    __IO  uint8_t UPLLN0     : 1;
    __IO  uint8_t UPLLN1     : 1;
    __IO  uint8_t UPLLN2     : 1;
    __IO  uint8_t UPLLN3     : 1;
    __IO  uint8_t UPLLN4     : 1;
    __IO  uint8_t UPLLN5     : 1;
    __IO  uint8_t UPLLN6     : 1;
  } stc_usbethernetclkn_upcr4_field_t;

  typedef struct stc_usbethernetclkn_up_str_field
  {
    __IO  uint8_t UPRDY      : 1;
  } stc_usbethernetclkn_up_str_field_t;

  typedef struct stc_usbethernetclkn_upint_enr_field
  {
    __IO  uint8_t UPCSE      : 1;
  } stc_usbethernetclkn_upint_enr_field_t;

  typedef struct stc_usbethernetclkn_upint_clr_field
  {
    __IO  uint8_t UPCSC      : 1;
  } stc_usbethernetclkn_upint_clr_field_t;

  typedef struct stc_usbethernetclkn_upint_str_field
  {
    __IO  uint8_t UPCSI      : 1;
  } stc_usbethernetclkn_upint_str_field_t;

  typedef struct stc_usbethernetclkn_upcr5_field
  {
    __IO  uint8_t UPLLM0     : 1;
    __IO  uint8_t UPLLM1     : 1;
    __IO  uint8_t UPLLM2     : 1;
    __IO  uint8_t UPLLM3     : 1;
  } stc_usbethernetclkn_upcr5_field_t;

  typedef struct stc_usbethernetclkn_upcr6_field
  {
    __IO  uint8_t UBSR0      : 1;
    __IO  uint8_t UBSR1      : 1;
    __IO  uint8_t UBSR2      : 1;
    __IO  uint8_t UBSR3      : 1;
  } stc_usbethernetclkn_upcr6_field_t;

  typedef struct stc_usbethernetclkn_upcr7_field
  {
    __IO  uint8_t EPLLEN     : 1;
  } stc_usbethernetclkn_upcr7_field_t;

  typedef struct stc_usbethernetclkn_usben0_field
  {
    __IO  uint8_t USBEN0     : 1;
  } stc_usbethernetclkn_usben0_field_t;

  typedef struct stc_usbethernetclkn_usben1_field
  {
    __IO  uint8_t USBEN1     : 1;
  } stc_usbethernetclkn_usben1_field_t;


  /******************************************************************************
   * USBETHERNETCLK
   ******************************************************************************/
  /* USB Ethernet clock registers */
  typedef struct stc_usb_ethernetclkn
  {
    union {
      __IO  uint8_t UCCR;
      stc_usbethernetclkn_uccr_field_t UCCR_f;
    };
          uint8_t RESERVED0[3];
    union {
      __IO  uint8_t UPCR1;
      stc_usbethernetclkn_upcr1_field_t UPCR1_f;
    };
          uint8_t RESERVED1[3];
    union {
      __IO  uint8_t UPCR2;
      stc_usbethernetclkn_upcr2_field_t UPCR2_f;
    };
          uint8_t RESERVED2[3];
    union {
      __IO  uint8_t UPCR3;
      stc_usbethernetclkn_upcr3_field_t UPCR3_f;
    };
          uint8_t RESERVED3[3];
    union {
      __IO  uint8_t UPCR4;
      stc_usbethernetclkn_upcr4_field_t UPCR4_f;
    };
          uint8_t RESERVED4[3];
    union {
      __IO  uint8_t UP_STR;
      stc_usbethernetclkn_up_str_field_t UP_STR_f;
    };
          uint8_t RESERVED5[3];
    union {
      __IO  uint8_t UPINT_ENR;
      stc_usbethernetclkn_upint_enr_field_t UPINT_ENR_f;
    };
          uint8_t RESERVED6[3];
    union {
      __IO  uint8_t UPINT_CLR;
      stc_usbethernetclkn_upint_clr_field_t UPINT_CLR_f;
    };
          uint8_t RESERVED7[3];
    union {
      __IO  uint8_t UPINT_STR;
      stc_usbethernetclkn_upint_str_field_t UPINT_STR_f;
    };
          uint8_t RESERVED8[3];
    union {
      __IO  uint8_t UPCR5;
      stc_usbethernetclkn_upcr5_field_t UPCR5_f;
    };
          uint8_t RESERVED9[3];
    union {
      __IO  uint8_t UPCR6;
      stc_usbethernetclkn_upcr6_field_t UPCR6_f;
    };
          uint8_t RESERVED10[3];
    union {
      __IO  uint8_t UPCR7;
      stc_usbethernetclkn_upcr7_field_t UPCR7_f;
    };
          uint8_t RESERVED11[3];
    union {
      __IO  uint8_t USBEN0;
      stc_usbethernetclkn_usben0_field_t USBEN0_f;
    };
          uint8_t RESERVED12[3];
    union {
      __IO  uint8_t USBEN1;
      stc_usbethernetclkn_usben1_field_t USBEN1_f;
    };
  } stc_usb_ethernetclkn_t;
  
  /* USB clock registers */
 

  #define bFM_USBETHERCLK_UCCR_UCEN0         *((volatile unsigned int*)(0x426C0000UL))
  #define bFM_USBETHERCLK_UCCR_UCSEL0        *((volatile unsigned int*)(0x426C0004UL))
  #define bFM_USBETHERCLK_UCCR_UCSEL1        *((volatile unsigned int*)(0x426C0008UL))
  #define bFM_USBETHERCLK_UCCR_UCEN1         *((volatile unsigned int*)(0x426C000CUL))
  #define bFM_USBETHERCLK_UCCR_ECEN          *((volatile unsigned int*)(0x426C0010UL)) 
  #define bFM_USBETHERCLK_UCCR_ECSEL0        *((volatile unsigned int*)(0x426C0014UL)) 
  #define bFM_USBETHERCLK_UCCR_ECSEL1        *((volatile unsigned int*)(0x426C0018UL)) 
  #define bFM_USBETHERCLK_UPCR1_UPLLEN       *((volatile unsigned int*)(0x426C0080UL))
  #define bFM_USBETHERCLK_UPCR1_UPINC        *((volatile unsigned int*)(0x426C0084UL)) 
  #define bFM_USBETHERCLK_UPCR2_UPOWT0       *((volatile unsigned int*)(0x426C0100UL)) 
  #define bFM_USBETHERCLK_UPCR2_UPOWT1       *((volatile unsigned int*)(0x426C0104UL))  
  #define bFM_USBETHERCLK_UPCR2_UPOWT2       *((volatile unsigned int*)(0x426C0108UL))  
  #define bFM_USBETHERCLK_UPCR3_UPLLK0       *((volatile unsigned int*)(0x426C0180UL)) 
  #define bFM_USBETHERCLK_UPCR3_UPLLK1       *((volatile unsigned int*)(0x426C0184UL))  
  #define bFM_USBETHERCLK_UPCR3_UPLLK2       *((volatile unsigned int*)(0x426C0188UL))  
  #define bFM_USBETHERCLK_UPCR3_UPLLK3       *((volatile unsigned int*)(0x426C018CUL))
  #define bFM_USBETHERCLK_UPCR3_UPLLK4       *((volatile unsigned int*)(0x426C0190UL)) 
  #define bFM_USBETHERCLK_UPCR4_UPLLN0       *((volatile unsigned int*)(0x426C0200UL)) 
  #define bFM_USBETHERCLK_UPCR4_UPLLN1       *((volatile unsigned int*)(0x426C0204UL))
  #define bFM_USBETHERCLK_UPCR4_UPLLN2       *((volatile unsigned int*)(0x426C0208UL))
  #define bFM_USBETHERCLK_UPCR4_UPLLN3       *((volatile unsigned int*)(0x426C020CUL))
  #define bFM_USBETHERCLK_UPCR4_UPLLN4       *((volatile unsigned int*)(0x426C0210UL))
  #define bFM_USBETHERCLK_UPCR4_UPLLN5       *((volatile unsigned int*)(0x426C0214UL))
  #define bFM_USBETHERCLK_UPCR4_UPLLN6       *((volatile unsigned int*)(0x426C0218UL))
  #define bFM_USBETHERCLK_UP_STR_UPRDY       *((volatile unsigned int*)(0x426C0280UL))
  #define bFM_USBETHERCLK_UPINT_ENR_UPCSE    *((volatile unsigned int*)(0x426C0300UL))
  #define bFM_USBETHERCLK_UPINT_CLR_UPCSC    *((volatile unsigned int*)(0x426C0380UL))
  #define bFM_USBETHERCLK_UPINT_STR_UPCSI    *((volatile unsigned int*)(0x426C0400UL))
  #define bFM_USBETHERCLK_UPCR5_UPLLM0       *((volatile unsigned int*)(0x426C0480UL))
  #define bFM_USBETHERCLK_UPCR5_UPLLM1       *((volatile unsigned int*)(0x426C0484UL))
  #define bFM_USBETHERCLK_UPCR5_UPLLM2       *((volatile unsigned int*)(0x426C0488UL))
  #define bFM_USBETHERCLK_UPCR5_UPLLM3       *((volatile unsigned int*)(0x426C048CUL))
  #define bFM_USBETHERCLK_UPCR6_UBSR0        *((volatile unsigned int*)(0x426C0500UL))
  #define bFM_USBETHERCLK_UPCR6_UBSR1        *((volatile unsigned int*)(0x426C0504UL))
  #define bFM_USBETHERCLK_UPCR6_UBSR2        *((volatile unsigned int*)(0x426C0508UL))
  #define bFM_USBETHERCLK_UPCR6_UBSR3        *((volatile unsigned int*)(0x426C050CUL))
  #define bFM_USBETHERCLK_UPCR7_EPLLEN       *((volatile unsigned int*)(0x426C0580UL))
  #define bFM_USBETHERCLK_USBEN0_USBEN0      *((volatile unsigned int*)(0x426C0600UL))
  #define bFM_USBETHERCLK_USBEN1_USBEN1      *((volatile unsigned int*)(0x426C0680UL))
  
  
#elif defined(FM4MCUTYPE)
/************************************************************************************************************/
/* FM4 General USB Clock                                                                                            */
/************************************************************************************************************/
  #ifdef FM4_USBETHERNETCLK
    #undef FM4_USBETHERNETCLK
    #warning You are using a mcu header file that is not valid for your choosen mcu type! (Operation of this module may work)
  #endif
  #ifdef FM4_USBETHERNETCLK_BASE
    #undef FM4_USBETHERNETCLK_BASE
  #endif
      
  #ifndef FM4_USBCLK_BASE
    #define FM4_USBCLK_BASE       (FM4_PERIPH_BASE + 0x36000UL)  /* USB clock registers                    */
  #endif 
  #ifdef FM4_USBCLK
    #define FM_USBCLK      ((stc_usbclkn_t *)FM4_USBCLK_BASE)
  #endif

  /******************************************************************************
   * USBCLK
   ******************************************************************************/
  /* USBCLK register bit fields */
  typedef struct stc_usbclkn_uccr_field
  {
    __IO  uint8_t UCEN       : 1;
    __IO  uint8_t UCSEL      : 1;
  } stc_usbclkn_uccr_field_t;

  typedef struct stc_usbclkn_upcr1_field
  {
    __IO  uint8_t UPLLEN     : 1;
    __IO  uint8_t UPINC      : 1;
  } stc_usbclkn_upcr1_field_t;

  typedef struct stc_usbclkn_upcr2_field
  {
    __IO  uint8_t UPOWT0     : 1;
    __IO  uint8_t UPOWT1     : 1;
    __IO  uint8_t UPOWT2     : 1;
  } stc_usbclkn_upcr2_field_t;

  typedef struct stc_usbclkn_upcr3_field
  {
    __IO  uint8_t UPLLK0     : 1;
    __IO  uint8_t UPLLK1     : 1;
    __IO  uint8_t UPLLK2     : 1;
    __IO  uint8_t UPLLK3     : 1;
    __IO  uint8_t UPLLK4     : 1;
  } stc_usbclkn_upcr3_field_t;

  typedef struct stc_usbclkn_upcr4_field
  {
    __IO  uint8_t UPLLN0     : 1;
    __IO  uint8_t UPLLN1     : 1;
    __IO  uint8_t UPLLN2     : 1;
    __IO  uint8_t UPLLN3     : 1;
    __IO  uint8_t UPLLN4     : 1;
    __IO  uint8_t UPLLN5     : 1;
    __IO  uint8_t UPLLN6     : 1;
  } stc_usbclkn_upcr4_field_t;

  typedef struct stc_usbclkn_up_str_field
  {
    __IO  uint8_t UPRDY      : 1;
  } stc_usbclkn_up_str_field_t;

  typedef struct stc_usbclkn_upint_enr_field
  {
    __IO  uint8_t UPCSE      : 1;
  } stc_usbclkn_upint_enr_field_t;

  typedef struct stc_usbclkn_upint_clr_field
  {
    __IO  uint8_t UPCSC      : 1;
  } stc_usbclkn_upint_clr_field_t;

  typedef struct stc_usbclkn_upint_str_field
  {
    __IO  uint8_t UPCSI      : 1;
  } stc_usbclkn_upint_str_field_t;

  typedef struct stc_usbclkn_upcr5_field
  {
    __IO  uint8_t UPLLM0     : 1;
    __IO  uint8_t UPLLM1     : 1;
    __IO  uint8_t UPLLM2     : 1;
    __IO  uint8_t UPLLM3     : 1;
  } stc_usbclkn_upcr5_field_t;

  typedef struct stc_usbclkn_usben_field
  {
    __IO  uint8_t USBEN      : 1;
  } stc_usbclkn_usben_field_t;
  /******************************************************************************
   * USBCLK
   ******************************************************************************/
  /* USB clock registers */
  typedef struct
  {
    union {
      __IO  uint8_t UCCR;
      stc_usbclkn_uccr_field_t UCCR_f;
    };
          uint8_t RESERVED0[3];
    union {
      __IO  uint8_t UPCR1;
      stc_usbclkn_upcr1_field_t UPCR1_f;
    };
          uint8_t RESERVED1[3];
    union {
      __IO  uint8_t UPCR2;
      stc_usbclkn_upcr2_field_t UPCR2_f;
    };
          uint8_t RESERVED2[3];
    union {
      __IO  uint8_t UPCR3;
      stc_usbclkn_upcr3_field_t UPCR3_f;
    };
          uint8_t RESERVED3[3];
    union {
      __IO  uint8_t UPCR4;
      stc_usbclkn_upcr4_field_t UPCR4_f;
    };
          uint8_t RESERVED4[3];
    union {
      __IO  uint8_t UP_STR;
      stc_usbclkn_up_str_field_t UP_STR_f;
    };
          uint8_t RESERVED5[3];
    union {
      __IO  uint8_t UPINT_ENR;
      stc_usbclkn_upint_enr_field_t UPINT_ENR_f;
    };
          uint8_t RESERVED6[3];
    union {
      __IO  uint8_t UPINT_CLR;
      stc_usbclkn_upint_clr_field_t UPINT_CLR_f;
    };
          uint8_t RESERVED7[3];
    union {
      __IO  uint8_t UPINT_STR;
      stc_usbclkn_upint_str_field_t UPINT_STR_f;
    };
          uint8_t RESERVED8[3];
    union {
      __IO  uint8_t UPCR5;
      stc_usbclkn_upcr5_field_t UPCR5_f;
    };
          uint8_t RESERVED9[11];
    union {
      __IO  uint8_t USBEN;
      stc_usbclkn_usben_field_t USBEN_f;
    };
  }stc_usbclkn_t;
  /* USB clock registers */
  
  #define bFM_USBCLK_UCCR_UCEN                  *((volatile unsigned int*)(0x426C0000UL))
  #define bFM_USBCLK_UCCR_UCSEL                 *((volatile unsigned int*)(0x426C0004UL))
  #define bFM_USBCLK_UPCR1_UPLLEN               *((volatile unsigned int*)(0x426C0080UL))
  #define bFM_USBCLK_UPCR1_UPINC                *((volatile unsigned int*)(0x426C0084UL))
  #define bFM_USBCLK_UPCR2_UPOWT0               *((volatile unsigned int*)(0x426C0100UL))  
  #define bFM_USBCLK_UPCR2_UPOWT1               *((volatile unsigned int*)(0x426C0104UL))   
  #define bFM_USBCLK_UPCR2_UPOWT2               *((volatile unsigned int*)(0x426C0108UL))  
  #define bFM_USBCLK_UPCR3_UPLLK0               *((volatile unsigned int*)(0x426C0180UL))
  #define bFM_USBCLK_UPCR3_UPLLK1               *((volatile unsigned int*)(0x426C0184UL))  
  #define bFM_USBCLK_UPCR3_UPLLK2               *((volatile unsigned int*)(0x426C0188UL))
  #define bFM_USBCLK_UPCR3_UPLLK3               *((volatile unsigned int*)(0x426C018CUL))  
  #define bFM_USBCLK_UPCR3_UPLLK4               *((volatile unsigned int*)(0x426C0190UL))   
  #define bFM_USBCLK_UPCR4_UPLLN0               *((volatile unsigned int*)(0x426C0200UL))   
  #define bFM_USBCLK_UPCR4_UPLLN1               *((volatile unsigned int*)(0x426C0204UL))  
  #define bFM_USBCLK_UPCR4_UPLLN2               *((volatile unsigned int*)(0x426C0208UL))   
  #define bFM_USBCLK_UPCR4_UPLLN3               *((volatile unsigned int*)(0x426C020CUL))   
  #define bFM_USBCLK_UPCR4_UPLLN4               *((volatile unsigned int*)(0x426C0210UL))   
  #define bFM_USBCLK_UPCR4_UPLLN5               *((volatile unsigned int*)(0x426C0214UL))  
  #define bFM_USBCLK_UPCR4_UPLLN6               *((volatile unsigned int*)(0x426C0218UL))  
  #define bFM_USBCLK_UP_STR_UPRDY               *((volatile unsigned int*)(0x426C0280UL)) 
  #define bFM_USBCLK_UPINT_ENR_UPCSE            *((volatile unsigned int*)(0x426C0300UL))
  #define bFM_USBCLK_UPINT_CLR_UPCSC            *((volatile unsigned int*)(0x426C0380UL))   
  #define bFM_USBCLK_UPINT_STR_UPCSI            *((volatile unsigned int*)(0x426C0400UL))  
  #define bFM_USBCLK_UPCR5_UPLLM0               *((volatile unsigned int*)(0x426C0480UL)) 
  #define bFM_USBCLK_UPCR5_UPLLM1               *((volatile unsigned int*)(0x426C0484UL))   
  #define bFM_USBCLK_UPCR5_UPLLM2               *((volatile unsigned int*)(0x426C0488UL)) 
  #define bFM_USBCLK_UPCR5_UPLLM3               *((volatile unsigned int*)(0x426C048CUL))    
  #define bFM_USBCLK_USBEN_USBEN                *((volatile unsigned int*)(0x426C0600UL))
  
#elif defined(FM3MCUTYPE)
/************************************************************************************************************/
/* FM3 General USB Clock                                                                                    */
/************************************************************************************************************/
  #ifdef FM3_USBETHERNETCLK
    #undef FM3_USBETHERNETCLK
    #warning You are using a mcu header file that is not valid for your choosen mcu type! (Operation of this module may work)
  #endif
  #ifdef FM3_USBETHERNETCLK_BASE
    #undef FM3_USBETHERNETCLK_BASE
  #endif
      
  #ifndef FM3_USBCLK_BASE
    #define FM3_USBCLK_BASE       (FM3_PERIPH_BASE + 0x36000UL)  /* USB clock registers                    */
  #endif 
  #ifdef FM3_USBCLK
    #define FM_USBCLK      ((stc_usbclkn_t *)FM3_USBCLK_BASE)
  #endif

  /******************************************************************************
   * USBCLK
   ******************************************************************************/
  /* USBCLK register bit fields */
  typedef struct stc_usbclkn_uccr_field
  {
    __IO  uint8_t UCEN       : 1;
    __IO  uint8_t UCSEL      : 1;
  } stc_usbclkn_uccr_field_t;

  typedef struct stc_usbclkn_upcr1_field
  {
    __IO  uint8_t UPLLEN     : 1;
    __IO  uint8_t UPINC      : 1;
  } stc_usbclkn_upcr1_field_t;

  typedef struct stc_usbclkn_upcr2_field
  {
    __IO  uint8_t UPOWT0     : 1;
    __IO  uint8_t UPOWT1     : 1;
    __IO  uint8_t UPOWT2     : 1;
  } stc_usbclkn_upcr2_field_t;

  typedef struct stc_usbclkn_upcr3_field
  {
    __IO  uint8_t UPLLK0     : 1;
    __IO  uint8_t UPLLK1     : 1;
    __IO  uint8_t UPLLK2     : 1;
    __IO  uint8_t UPLLK3     : 1;
    __IO  uint8_t UPLLK4     : 1;
  } stc_usbclkn_upcr3_field_t;

  typedef struct stc_usbclkn_upcr4_field
  {
    __IO  uint8_t UPLLN0     : 1;
    __IO  uint8_t UPLLN1     : 1;
    __IO  uint8_t UPLLN2     : 1;
    __IO  uint8_t UPLLN3     : 1;
    __IO  uint8_t UPLLN4     : 1;
    __IO  uint8_t UPLLN5     : 1;
    __IO  uint8_t UPLLN6     : 1;
  } stc_usbclkn_upcr4_field_t;

  typedef struct stc_usbclkn_up_str_field
  {
    __IO  uint8_t UPRDY      : 1;
  } stc_usbclkn_up_str_field_t;

  typedef struct stc_usbclkn_upint_enr_field
  {
    __IO  uint8_t UPCSE      : 1;
  } stc_usbclkn_upint_enr_field_t;

  typedef struct stc_usbclkn_upint_clr_field
  {
    __IO  uint8_t UPCSC      : 1;
  } stc_usbclkn_upint_clr_field_t;

  typedef struct stc_usbclkn_upint_str_field
  {
    __IO  uint8_t UPCSI      : 1;
  } stc_usbclkn_upint_str_field_t;

  typedef struct stc_usbclkn_upcr5_field
  {
    __IO  uint8_t UPLLM0     : 1;
    __IO  uint8_t UPLLM1     : 1;
    __IO  uint8_t UPLLM2     : 1;
    __IO  uint8_t UPLLM3     : 1;
  } stc_usbclkn_upcr5_field_t;

  typedef struct stc_usbclkn_usben_field
  {
    __IO  uint8_t USBEN      : 1;
  } stc_usbclkn_usben_field_t;
  /******************************************************************************
   * USBCLK
   ******************************************************************************/
  /* USB clock registers */
  typedef struct
  {
    union {
      __IO  uint8_t UCCR;
      stc_usbclkn_uccr_field_t UCCR_f;
    };
          uint8_t RESERVED0[3];
    union {
      __IO  uint8_t UPCR1;
      stc_usbclkn_upcr1_field_t UPCR1_f;
    };
          uint8_t RESERVED1[3];
    union {
      __IO  uint8_t UPCR2;
      stc_usbclkn_upcr2_field_t UPCR2_f;
    };
          uint8_t RESERVED2[3];
    union {
      __IO  uint8_t UPCR3;
      stc_usbclkn_upcr3_field_t UPCR3_f;
    };
          uint8_t RESERVED3[3];
    union {
      __IO  uint8_t UPCR4;
      stc_usbclkn_upcr4_field_t UPCR4_f;
    };
          uint8_t RESERVED4[3];
    union {
      __IO  uint8_t UP_STR;
      stc_usbclkn_up_str_field_t UP_STR_f;
    };
          uint8_t RESERVED5[3];
    union {
      __IO  uint8_t UPINT_ENR;
      stc_usbclkn_upint_enr_field_t UPINT_ENR_f;
    };
          uint8_t RESERVED6[3];
    union {
      __IO  uint8_t UPINT_CLR;
      stc_usbclkn_upint_clr_field_t UPINT_CLR_f;
    };
          uint8_t RESERVED7[3];
    union {
      __IO  uint8_t UPINT_STR;
      stc_usbclkn_upint_str_field_t UPINT_STR_f;
    };
          uint8_t RESERVED8[3];
    union {
      __IO  uint8_t UPCR5;
      stc_usbclkn_upcr5_field_t UPCR5_f;
    };
          uint8_t RESERVED9[11];
    union {
      __IO  uint8_t USBEN;
      stc_usbclkn_usben_field_t USBEN_f;
    };
  }stc_usbclkn_t;
  /* USB clock registers */
  
  #define bFM_USBCLK_UCCR_UCEN                  *((volatile unsigned int*)(0x426C0000UL))
  #define bFM_USBCLK_UCCR_UCSEL                 *((volatile unsigned int*)(0x426C0004UL))
  #define bFM_USBCLK_UPCR1_UPLLEN               *((volatile unsigned int*)(0x426C0080UL))
  #define bFM_USBCLK_UPCR1_UPINC                *((volatile unsigned int*)(0x426C0084UL))
  #define bFM_USBCLK_UPCR2_UPOWT0               *((volatile unsigned int*)(0x426C0100UL))
  #define bFM_USBCLK_UPCR2_UPOWT1               *((volatile unsigned int*)(0x426C0104UL))  
  #define bFM_USBCLK_UPCR2_UPOWT2               *((volatile unsigned int*)(0x426C0108UL))   
  #define bFM_USBCLK_UPCR3_UPLLK0               *((volatile unsigned int*)(0x426C0180UL))   
  #define bFM_USBCLK_UPCR3_UPLLK1               *((volatile unsigned int*)(0x426C0184UL))  
  #define bFM_USBCLK_UPCR3_UPLLK2               *((volatile unsigned int*)(0x426C0188UL))    
  #define bFM_USBCLK_UPCR3_UPLLK3               *((volatile unsigned int*)(0x426C018CUL)) 
  #define bFM_USBCLK_UPCR3_UPLLK4               *((volatile unsigned int*)(0x426C0190UL))  
  #define bFM_USBCLK_UPCR4_UPLLN0               *((volatile unsigned int*)(0x426C0200UL))  
  #define bFM_USBCLK_UPCR4_UPLLN1               *((volatile unsigned int*)(0x426C0204UL))  
  #define bFM_USBCLK_UPCR4_UPLLN2               *((volatile unsigned int*)(0x426C0208UL))  
  #define bFM_USBCLK_UPCR4_UPLLN3               *((volatile unsigned int*)(0x426C020CUL))   
  #define bFM_USBCLK_UPCR4_UPLLN4               *((volatile unsigned int*)(0x426C0210UL))  
  #define bFM_USBCLK_UPCR4_UPLLN5               *((volatile unsigned int*)(0x426C0214UL))
  #define bFM_USBCLK_UPCR4_UPLLN6               *((volatile unsigned int*)(0x426C0218UL))
  #define bFM_USBCLK_UP_STR_UPRDY               *((volatile unsigned int*)(0x426C0280UL)) 
  #define bFM_USBCLK_UPINT_ENR_UPCSE            *((volatile unsigned int*)(0x426C0300UL))   
  #define bFM_USBCLK_UPINT_CLR_UPCSC            *((volatile unsigned int*)(0x426C0380UL))   
  #define bFM_USBCLK_UPINT_STR_UPCSI            *((volatile unsigned int*)(0x426C0400UL))   
  #define bFM_USBCLK_UPCR5_UPLLM0               *((volatile unsigned int*)(0x426C0480UL)) 
  #define bFM_USBCLK_UPCR5_UPLLM1               *((volatile unsigned int*)(0x426C0484UL))   
  #define bFM_USBCLK_UPCR5_UPLLM2               *((volatile unsigned int*)(0x426C0488UL))   
  #define bFM_USBCLK_UPCR5_UPLLM3               *((volatile unsigned int*)(0x426C048CUL))   
  #define bFM_USBCLK_USBEN_USBEN                *((volatile unsigned int*)(0x426C0600UL))
#else
    #error MCU type could not be detected  
#endif
/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

#ifdef __cplusplus
}
#endif

//@} // UsbEthernetClockLegacyGroup

#endif /* __USBETHERNETCLOCKLEGACY_H__*/

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

