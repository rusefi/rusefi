/*******************************************************************************
* Copyright (C) 2013-2015, Cypress Semiconductor Corporation. All rights reserved.
*
* This software, including source code, documentation and related
* materials ( "Software" ), is owned by Cypress Semiconductor
* Corporation ( "Cypress" ) and is protected by and subject to worldwide
* patent protection (United States and foreign), United States copyright
* laws and international treaty provisions. Therefore, you may use this
* Software only as provided in the license agreement accompanying the
* software package from which you obtained this Software ( "EULA" ).
* If no EULA applies, Cypress hereby grants you a personal, nonexclusive,
* non-transferable license to copy, modify, and compile the
* Software source code solely for use in connection with Cypress's
* integrated circuit products. Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO
* WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING,
* BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE. Cypress reserves the right to make
* changes to the Software without notice. Cypress does not assume any
* liability arising out of the application or use of the Software or any
* product or circuit described in the Software. Cypress does not
* authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ( "High Risk Product" ). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*/
/** ******************************************************************************
/ ** \file system_s6e2c5.c
 **
 ** FM series system initialization functions
 ** All adjustments can be done in belonging header file.
 **
 ** History:
 ** 16.12.2015 18:31:27 2.0 MISH Auto created by make sys_xxx.c script
 ******************************************************************************/

#include "mcu.h"

/**
 ******************************************************************************
 ** System Clock Frequency (Core Clock) Variable according CMSIS
 ******************************************************************************/
uint32_t SystemCoreClock = __HCLK;

/**
 ******************************************************************************
 ** \brief  Update the System Core Clock with current core Clock retrieved from
 ** cpu registers.
 ** \param  none
 ** \return none
 ******************************************************************************/
void SystemCoreClockUpdate (void) {
  uint32_t masterClk;
  uint32_t u32RegisterRead; // Workaround variable for MISRA C rule conformance

  switch ((FM_CRG->SCM_CTL >> 5u) & 0x07U) {
    case 0u:                                 /* internal High-speed Cr osc.    */
      masterClk = __CLKHC;
      break;

    case 1u:                                 /* external main osc.             */
      masterClk = __CLKMO;
      break;

    case 2u:                                 /* PLL clock                      */
  // Workaround for preventing MISRA C:1998 Rule 46 (MISRA C:2004 Rule 12.2)
  // violation:
  //   "Unordered accesses to a volatile location"
      u32RegisterRead = (__CLKMO  * (((uint32_t)(FM_CRG->PLL_CTL2) & 0x3F) + 1u));
      masterClk = (u32RegisterRead / ((((uint32_t)(FM_CRG->PLL_CTL1) >> 4ul) & 0x0Fu) + 1u));
      break;

    case 4u:                                 /* internal Low-speed CR osc.     */
      masterClk = __CLKLC;
      break;

    case 5u:                                 /* external Sub osc.              */
      masterClk = __CLKSO;
      break;

    default:
      masterClk = 0ul;
      break;
  }

  switch (FM_CRG->BSC_PSR & 0x07u) {
    case 0u:
      SystemCoreClock = masterClk;
      break;

    case 1u:
      SystemCoreClock = masterClk / 2u;
      break;

    case 2u:
      SystemCoreClock = masterClk / 3u;
      break;

    case 3u:
      SystemCoreClock = masterClk / 4u;
      break;

    case 4u:
      SystemCoreClock = masterClk / 6u;
      break;

    case 5u:
      SystemCoreClock = masterClk /8u;
      break;

    case 6u:
      SystemCoreClock = masterClk /16u;
      break;

    default:
      SystemCoreClock = 0ul;
      break;
  }

}

/**
 ******************************************************************************
 ** \brief  Setup the microcontroller system. Initialize the System and update
 ** the SystemCoreClock variable.
 **
 ** \param  none
 ** \return none
 ******************************************************************************/
void SystemInit (void) {

#if (CLOCK_SETUP == CLOCK_SETTING_CMSIS)
  static uint32_t u32IoRegisterRead;  // Workaround variable for MISRA C rule conformance
#endif
  
#if (HWWD_DISABLE)                                 /* HW Watchdog Disable */
  FM_HWWDT->WDG_LCK = 0x1ACCE551u;                 /* HW Watchdog Unlock */
  FM_HWWDT->WDG_LCK = 0xE5331AAEu;
  FM_HWWDT->WDG_CTL = 0u;                          /* HW Watchdog stop */
#endif

// FPU settings 
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
  SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));     /* set CP10 and CP11 Full Access */
#endif
  
#if (TRACE_BUFFER_ENABLE == 1)
  FM_FLASH_IF->FBFCR = 0x01u;                     /* Trace Buffer enable */
  while(0x02u != (FM_FLASH_IF->FBFCR & 0x02u))    /* Wait for trace Buffer enable */
  {}
#endif

#if (CLOCK_SETUP == CLOCK_SETTING_CMSIS)                     /* Clock Setup */
  FM_CRG->BSC_PSR   = BSC_PSR_Val;                /* set System Clock presacaler */
  FM_CRG->APBC0_PSR = APBC0_PSR_Val;              /* set APB0 presacaler */
  FM_CRG->APBC1_PSR = APBC1_PSR_Val;              /* set APB1 presacaler */
  FM_CRG->APBC2_PSR = APBC2_PSR_Val;              /* set APB2 presacaler */
  FM_CRG->SWC_PSR   = SWC_PSR_Val | (1ul << 7u);  /* set SW Watchdog presacaler */
  FM_CRG->TTC_PSR   = TTC_PSR_Val;                /* set Trace Clock presacaler */
  FM_CRG->CSW_TMR   = CSW_TMR_Val;                /* set oscillation stabilization wait time */
  
  if (SCM_CTL_Val & (1ul << 1u)) {                    /* Main clock oscillator enabled ? */
    FM_CRG->SCM_CTL |= (1ul << 1u);                /* enable main oscillator */
    while (!(FM_CRG->SCM_STR & (1ul << 1u)));      /* wait for Main clock oscillation stable */
  }
  
  if (SCM_CTL_Val & (1UL << 3)) {                    /* Sub clock oscillator enabled ? */
    // Initialize VBAT
    FM_RTC->VDET = 0x00;                           /* Clear the power-on signal */
    FM_RTC->VBPFR = 0x1C;                          /* set P46/X0A and P47/X1A pin to 32kHz oscillation pins. */
    FM_RTC->CCB = 0xCE;                     /* set Oscillation boost current */
    FM_RTC->CCS = 0x08;                     /* set Oscillation sustain current */

    // VB_CLK is less or equal to 1MHz
    FM_RTC->VB_CLKDIV = 0;             /* set transfer clock division */
    FM_RTC->BOOST = 0x03;                 /* set Oscillation boost time */

    // Enable SUB CLK oscilation
    FM_RTC->WTOSCCNT_f.SOSCNTL = 1;                /* set 32kHz oscillation clock control connection enabled */
    FM_RTC->WTOSCCNT_f.SOSCEX  = 0;                /* set 32kHz oscillation enabled */

    // Transmit to VBAT domain
    FM_RTC->WTCR20_f.PWRITE = 1;

    // Wait to complete transmission
    while(0 != FM_RTC->WTCR10_f.TRANS)

    FM_CRG->SCM_CTL |= (1UL << 3);                /* enable sub oscillator */ 
    while (!(FM_CRG->SCM_STR & (1UL << 3)));      /* wait for Sub clock oscillation stable */
  }

  FM_CRG->PSW_TMR   = PSW_TMR_Val;                /* set PLL stabilization wait time */
  FM_CRG->PLL_CTL1  = PLL_CTL1_Val;               /* set PLLM and PLLK */
  FM_CRG->PLL_CTL2  = PLL_CTL2_Val;               /* set PLLN          */
  
  if (SCM_CTL_Val & (1ul << 4u)) {                    /* PLL enabled ? */
    if(((PSW_TMR_Val & (1ul << 4u)) == 0) && ((SCM_CTL_Val & (1ul << 1u)) == 0u)){
        //Main clock is sellected as PLL input, but the oscillator is disabled.
        FM_CRG->PSW_TMR_f.PINC = 1u;               /* use high-speed CR as PLL source clock instead of main clock oscillator. */
    }
    FM_CRG->SCM_CTL  |= (1ul << 4u);               /* enable PLL */ 
    while (!(FM_CRG->SCM_STR & (1ul << 4u)));      /* wait for PLL stable */
  }

  if ((SCM_CTL_Val &  0xE0u)==0x80ul) {    /* Low-speed CR oscillation selected ? */
    u32IoRegisterRead = (FM_LSCRP->LCR_PRSLD & 0x3Fu);         /* Read LCR_PRSLD value */ 
    if(u32IoRegisterRead > 0ul) {
      uint32_t i;
      uint32_t u32WaitValue;
      u32WaitValue = (__CLKHC/__CLKLC)*u32IoRegisterRead;     /* caluculate wait value */
      FM_LSCRP->LCR_PRSLD = 0x00u;                            /* Set "0" to LCR_PRSLD register */
      for(i=0; i<u32WaitValue; i++);                          /* wait for changing clock */
    }
    FM_LSCRP->LCR_PRSLD = LCR_PRSLD_Val;                      /* set the division ratio of low-speed CR */
  }

  FM_CRG->SCM_CTL  |= (SCM_CTL_Val & 0xE0);       /* Set Master Clock switch */
  
  // Workaround for preventing MISRA C:1998 Rule 46 (MISRA C:2004 Rule 12.2)
  // violations:
  //   "Unordered reads and writes to or from same location" and
  //   "Unordered accesses to a volatile location"
  do                                              
  {                                               
    u32IoRegisterRead = (FM_CRG->SCM_CTL & 0xE0u);
  }while ((FM_CRG->SCM_STR & 0xE0u) != u32IoRegisterRead);
  
  
#elif (CLOCK_SETUP == CLOCK_SETTING_NONE)
  
  // user defined clock setting
  
#else
  #error Clock setup type unknown!
#endif
  
#if (CR_TRIM_SETUP)
  /* CR Trimming Data  */
  if( 0x001F03FF != (FM_FLASH_IF->CRTRMM & 0x001F03FF) )
  {
    /* UnLock (MCR_FTRM) */
    FM_CRTRIM->MCR_RLR = 0x1ACCE554u;
    /* Set MCR_TTRM */
    FM_CRTRIM->MCR_TTRM = (FM_FLASH_IF->CRTRMM >> 16);
    /* Set MCR_FTRM */
    FM_CRTRIM->MCR_FTRM = FM_FLASH_IF->CRTRMM;
    /* Lock (MCR_FTRM) */
    FM_CRTRIM->MCR_RLR = 0x00000000u;
  }
#endif // (CR_TRIM_SETUP)
}



