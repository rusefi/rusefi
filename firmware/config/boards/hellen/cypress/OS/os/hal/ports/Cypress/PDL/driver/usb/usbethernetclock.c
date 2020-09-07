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
 **   - 2012-11-02  1.5  CNo  Integration Ethernet driver
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

#include "usbethernetclock.h"

/**
 ******************************************************************************
 ** \ingroup UsbEthernetGroup
 ******************************************************************************/
//@{

/*****************************************************************************/
/* Static variable declarations ('static', definition in C source)           */
/*****************************************************************************/

static boolean_t bUsbEthernetClockEnabled = FALSE;

/*****************************************************************************/
/* Global function  ('extern', definition in C source)                       */
/*****************************************************************************/

/**
 ******************************************************************************
 ** \brief Initalize PLL for USB / Ethernet Clock.
 **
 ******************************************************************************/
void UsbEthernetClock_Init(void)
{
    if (bUsbEthernetClockEnabled)
    {
        return;
    }
    
    /************************************************************************/
    /* Initialize default settings                                          */
    /************************************************************************/
    #if defined(FM_USBCLK)
        /* Clear USB/Ethernet Clock Setting Register */
        FM_USBCLK->UCCR = 0;

        /* Disable USB Clock */
        #if defined(bFM_USBCLK_USBEN_USBEN)
            bFM_USBCLK_USBEN_USBEN = 0;
        #elif defined(bFM_USBCLK_USBEN0_USBEN0)
            bFM_USBCLK_USBEN0_USBEN0 = 0;
        #else
            #error no definition for USBEN
        #endif
        #if defined(bFM_USBCLK_USBEN1_USBEN1)
            bFM_USBCLK_USBEN1_USBEN1 = 0;
        #endif
        #if USBETHERNETCLOCK_USBPLL_AVAILABLE == 1   
           /* PLL Disable */
           FM_USBCLK->UPCR1 = 0x00;
        #endif

    #elif defined(FM_USBETHERCLK)
        /* Clear USB/Ethernet Clock Setting Register */
        FM_USBETHERCLK->UCCR = 0;

        /* Disable Ethernet PLL */
        bFM_USBETHERCLK_UPCR7_EPLLEN = 0;

        /* Disable USB PLL */
        bFM_USBETHERCLK_UPCR1_UPLLEN = 0;

        /* Disable USB Clock 0 */
        #if defined(bFM_USBCLK_USBEN0_USBEN)
          bFM_USBCLK_USBEN0_USBEN = 0;
        #elif defined(bFM_USBETHERCLK_UCCR_UCEN0)
          bFM_USBETHERCLK_UCCR_UCEN0 = 0;
        #else
          #error No bit definition for UCCR -> UCEN0
        #endif

        /* Disable USB Clock 1 */
        #if defined(bFM_USBCLK_USBEN1_USBEN)
          bFM_USBCLK_USBEN1_USBEN = 0;
        #elif defined(bFM_USBETHERCLK_UCCR_UCEN1)
          bFM_USBETHERCLK_UCCR_UCEN0 = 0;
        #else
          #error No bit definition for UCCR -> UCEN1
        #endif


        /* PLL Disable */
        FM_USBETHERCLK->UPCR1 = 0x00;
    #else
        #error Incorrect USB/Ethernet clock module
    #endif
    /* End of Initialize default settings */

        
    /************************************************************************/
    /* Setup PLL                                                            */
    /************************************************************************/
    #if defined(FM_USBETHERCLK)
        /* PLL Disable */
        FM_USBETHERCLK->UPCR1 = 0x00;

        /* wait 1ms */
        FM_USBETHERCLK->UPCR2 = 0x03;

        #ifdef USBETHERNETCLOCK_PLL_K
            FM_USBETHERCLK->UPCR3 = USBETHERNETCLOCK_PLL_K - 1;
        #endif
        #ifdef USBETHERNETCLOCK_PLL_N
            FM_USBETHERCLK->UPCR4 = USBETHERNETCLOCK_PLL_N - 1;
        #endif
        #ifdef USBETHERNETCLOCK_PLL_M
            FM_USBETHERCLK->UPCR5 = USBETHERNETCLOCK_PLL_M - 1;
        #endif
    #elif defined(FM_USBCLK)
        #if USBETHERNETCLOCK_USBPLL_AVAILABLE == 1   
            /* PLL Disable */
            FM_USBCLK->UPCR1 = 0x00;
            /* wait 1ms */
            FM_USBCLK->UPCR2 = 0x03;
            #ifdef USBETHERNETCLOCK_PLL_K
                FM_USBCLK->UPCR3 = USBETHERNETCLOCK_PLL_K - 1;
            #endif
            #ifdef USBETHERNETCLOCK_PLL_K
                FM_USBCLK->UPCR4 = USBETHERNETCLOCK_PLL_N - 1;
            #endif
            #ifdef USBETHERNETCLOCK_PLL_M
                FM_USBCLK->UPCR5 = USBETHERNETCLOCK_PLL_M - 1;
            #endif
        #endif
    #else
        #error Incorrect USB/Ethernet clock module
    #endif
    /* End Setup PLL */

            
    /************************************************************************/
    /* Enable PLL and wait PLL gets ready                                   */
    /************************************************************************/
    #if defined(FM_USBCLK)
        #if USBETHERNETCLOCK_USBPLL_AVAILABLE == 1       
            /* Enable USB Part of PLL */
            bFM_USBCLK_UPCR1_UPLLEN = 1;

            /* Wait PLL gets ready */
            while(bFM_USBCLK_UP_STR_UPRDY == 0);
						
						
            /* Select USB clock source */
            FM_USBCLK->UCCR = 0x01 | (USBETHERNETCLOCK_UCSEL << 1);
        #else //!defined(FM_USBCLK)
            #if __CLKMO == 48000000
                 FM_USBCLK->UPCR6 = 0;
                 FM_USBCLK->UCCR = (0x0 << 1) | 0x01;
            #else		
                 FM_USBCLK->UPCR6 = (0x0F & ((__CLKMO * __PLLN * __PLLM) / __PLLK / 48000000ul) - 1); 
                 FM_USBCLK->UCCR = (0x2 << 1) | 0x01;
            #endif
        #endif
        
        /* Enable USB */
        #if (FM_PERIPHERAL_ENABLE_USB0 == ON)
            #if defined(bFM_USBCLK_USBEN_USBEN)
                bFM_USBCLK_USBEN_USBEN = 1;
            #elif defined(bFM_USBCLK_USBEN0_USBEN0)
                bFM_USBCLK_USBEN0_USBEN0 = 1;
            #else
                #error no definition for USBEN
            #endif
        #endif   
        #if (FM_PERIPHERAL_ENABLE_USB1 == ON)        
            #if defined(bFM_USBCLK_USBEN1_USBEN1)
                bFM_USBCLK_USBEN1_USBEN1 = 1;
            #else
                #error no definition for USBEN1
            #endif    
        #endif
            
    #elif defined(FM_USBETHERCLK)
        #if defined(USBETHERNETCLOCK_UBSR)
            FM_USBETHERCLK->UPCR6 = (USBETHERNETCLOCK_UBSR & 0x0F);
        #endif


        /* Enable USB Part of PLL */
        bFM_USBETHERCLK_UPCR1_UPLLEN = 1;

        /* Enable Ethernet Part of PLL */
        bFM_USBETHERCLK_UPCR7_EPLLEN = 1;


        /* Wait PLL gets ready */
        while(bFM_USBETHERCLK_UP_STR_UPRDY == 0);

        #if ((L3_PERIPHERAL_ENABLE_EMAC0 == L3_ON) || (L3_PERIPHERAL_ENABLE_EMAC1 == L3_ON))
             FM_USBETHERCLK->UCCR |= (USBETHERNETCLOCK_UCSEL << 1) | (USBETHERNETCLOCK_ECSEL << 5);


             bFM_USBETHERCLK_UCCR_ECEN = 1;
        #endif

        #if ((FM_PERIPHERAL_ENABLE_USB0 == ON) && (defined(USB0)))
            FM_USBETHERCLK->UCCR |= (USBETHERNETCLOCK_UCSEL << 1) | (USBETHERNETCLOCK_ECSEL << 5);

            #ifdef bFM_USBETHERCLK_UCCR_UCEN0
            bFM_USBETHERCLK_UCCR_UCEN0 = 1;
            #endif


            #ifdef bFM_USBCLK_USBEN0_USBEN
            bFM_USBCLK_USBEN0_USBEN = 1;
            #endif
            #ifdef bFM_USBETHERCLK_USBEN0_USBEN0
            bFM_USBETHERCLK_USBEN0_USBEN0 = 1;
            #endif
        #endif
        #if ((FM_PERIPHERAL_ENABLE_USB1 == ON) && (defined(USB1)))
            FM_USBETHERCLK->UCCR |= (USBETHERNETCLOCK_UCSEL << 1) | (USBETHERNETCLOCK_ECSEL << 5);

            #ifdef bFM_USBETHERCLK_UCCR_UCEN1
            bFM_USBETHERCLK_UCCR_UCEN1 = 1;
            #endif


            #ifdef bFM_USBCLK_USBEN1_USBEN
            bFM_USBCLK_USBEN1_USBEN = 1;
            #endif
            #ifdef bFM_USBETHERCLK_USBEN1_USBEN1
            bFM_USBETHERCLK_USBEN1_USBEN1 = 1;
            #endif
        #endif
    #else
        #error Incorrect USB/Ethernet clock module
    #endif
    /* End Enable PLL and wait PLL gets ready */

            
            
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();

    bUsbEthernetClockEnabled = TRUE;
}

/**
 ******************************************************************************
 ** \brief Deinitalize PLL for USB / Ethernet Clock.
 **
 ******************************************************************************/
void UsbEthernetClock_Deinit(void)
{
     #if defined(FM_USBCLK)
        FM_USBCLK->UCCR = 0;
        #if defined(bFM_USBCLK_USBEN_USBEN)
            bFM_USBCLK_USBEN_USBEN = 0;
        #elif defined(bFM_USBCLK_USBEN0_USBEN0)
            bFM_USBCLK_USBEN0_USBEN0 = 0;
        #else
            #error no definition for USBEN
        #endif
        #if defined(bFM_USBCLK_USBEN1_USBEN1)
            bFM_USBCLK_USBEN1_USBEN1 = 0;
        #endif
        #if USBETHERNETCLOCK_USBPLL_AVAILABLE == 1   
            FM_USBCLK->UPCR1 = 0x00; /* PLL Disable */
        #endif
     #elif defined(FM_USBETHERCLK)

        /* Disable Ethernet PLL */
        bFM_USBETHERCLK_UPCR7_EPLLEN = 0;

        /* Disable USB PLL */
        bFM_USBETHERCLK_UPCR1_UPLLEN = 0;

        FM_USBETHERCLK->UCCR = 0;
        #ifdef bFM_USBCLK_USBEN0_USBEN
        bFM_USBCLK_USBEN0_USBEN = 0;
        #endif
        #ifdef bFM_USBETHERCLK_UCCR_UCEN0
        bFM_USBETHERCLK_UCCR_UCEN0 = 0;
        #endif

        #ifdef bFM_USBCLK_USBEN1_USBEN
        bFM_USBCLK_USBEN1_USBEN = 0;
        #endif
        #ifdef bFM_USBETHERCLK_UCCR_UCEN1
        bFM_USBETHERCLK_UCCR_UCEN1 = 0;
        #endif
        FM_USBETHERCLK->UPCR1 = 0x00; /* PLL Disable */
    #else
        #error Incorrect USB/Ethernet clock module
    #endif
    #if USBETHERNETCLOCK_USBPLL_AVAILABLE == 1       
        #if  defined(FM_USBETHERCLK)
            FM_USBETHERCLK->UPCR1 = 0x00; /* PLL Disable */
        #elif defined(FM_USBCLK)
            FM_USBCLK->UPCR1 = 0x00; /* PLL Disable */
        #else
            #error Incorrect USB/Ethernet clock module
        #endif
    #endif
    bUsbEthernetClockEnabled = FALSE;
}
//@} // UsbEthernetClockGroup


