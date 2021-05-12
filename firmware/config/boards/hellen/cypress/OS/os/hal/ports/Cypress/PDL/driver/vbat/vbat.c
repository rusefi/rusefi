/*******************************************************************************
* \file             vbat.c
* 
* \version          1.20
*
* \brief            This file provides the source code to the API for the VBAT 
*                   driver.
*
********************************************************************************
* \copyright
* Copyright 2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
* CYPRESS PROVIDES THIS SOFTWARE "AS IS" AND MAKES NO WARRANTY
* OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS SOFTWARE,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
* PURPOSE.
*******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "vbat/vbat.h"

#if (defined(PDL_PERIPHERAL_VBAT_ACTIVE))

#if (PDL_PERIPHERAL_VBAT_AVAILABLE == PDL_ON)

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/* This is a workaround for Cypress ID 245800. The CCB register definition is
* not present in the device header file for FM0P_TYPE2 MCU.
*/
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)
#define FM_RTC_CCB      *((volatile  uint8_t*)(0x4003B180UL))
#endif /* PDL_MCU_TYPE == PDL_FM0P_TYPE2 */

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/
/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/
/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/
/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/
/******************************************************************************/
/* Local Functions                                                            */
/******************************************************************************/
/******************************************************************************/
/* Global Functions                                                           */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief Initialize VBAT
 **
 ** This function initializes an VBAT module
 **
 ** \param [in]  pstcConfig       VBAT module configuration 
 **
 ** \retval Ok                    VBAT initialized normally
 ** \retval ErrorInvalidParameter If one of following cases matchs: 
 **                               - pstcConfig == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Vbat_Init(const stc_vbat_config_t* pstcConfig)
{
    // Check parameter
    if (NULL == pstcConfig)
    {
        return ErrorInvalidParameter;
    }
    
    //  Clear VBAT power on flag
    FM_RTC->VDET = 0u;
    
    // Configure transfer clock
    if ((0u == pstcConfig->u8ClkDiv) ||  (0xFFu == pstcConfig->u8ClkDiv))
    {
        return ErrorInvalidParameter;
    }  
    FM_RTC->VB_CLKDIV = pstcConfig->u8ClkDiv;
    
    // Configure sustain/boost current 
#ifdef PDL_PERIPHERAL_VBAT_TYPE_A      
    switch (pstcConfig->enSustainCurrent)
    {
        case Clk0nA:
            FM_RTC->CCS = 0x00u;
            break;
        case Clk385nA:
            FM_RTC->CCS = 0x04u;
            break;
        case Clk445nA:
            FM_RTC->CCS = 0x08u;
            break;
        case Clk510nA:
            FM_RTC->CCS = 0x10u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    switch (pstcConfig->enBoostCurrent)
    {
        case Clk0nA:
            FM_RTC->CCB = 0x00u;
            break;
        case Clk385nA:
            FM_RTC->CCB = 0x04u;
            break;
        case Clk445nA:
            FM_RTC->CCB = 0x08u;
            break;
        case Clk510nA:
            FM_RTC->CCB = 0x10u;
            break;
        default:
            return ErrorInvalidParameter;
    }
#else
    switch (pstcConfig->enSustainCurrent)
    {
        case ClkLowPower:
            FM_RTC->CCS = 0x04u;
            break;
        case ClkStandard:
            FM_RTC->CCS = 0xCEu;
            break;
        default:
            return ErrorInvalidParameter;
    }

/* Workaround for Cypress ID 245800 */
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)
    switch (pstcConfig->enBoostCurrent)
    {
        case ClkLowPower:
            FM_RTC_CCB = 0x04u;
            break;
        case ClkStandard:
            FM_RTC_CCB = 0xCEu;
            break;
        default:
            return ErrorInvalidParameter;
    }
#else
    switch (pstcConfig->enBoostCurrent)
    {
        case ClkLowPower:
            FM_RTC->CCB = 0x04u;
            break;
        case ClkStandard:
            FM_RTC->CCB = 0xCEu;
            break;
        default:
            return ErrorInvalidParameter;
    }
#endif /* (PDL_MCU_TYPE == PDL_FM0P_TYPE2) */
#endif /* PDL_PERIPHERAL_VBAT_TYPE_A */
    
    // Set boost time
    switch (pstcConfig->enClkBoostTime)
    {
        case ClkBoost50ms:
            FM_RTC->BOOST = 0u;
            break;
        case ClkBoost63ms:
            FM_RTC->BOOST = 1u;
            break;
        case ClkBoost125ms:
            FM_RTC->BOOST = 2u;
            break;
        case ClkBoost250ms:
            FM_RTC->BOOST = 3u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    // Configure whether to link clock to clock control module
    FM_RTC->WTOSCCNT_f.SOSCNTL = ((pstcConfig->bLinkSubClk == TRUE) ? 1u : 0u);
    
    // Configure whether to enable sub clock in the VBAT domain
    FM_RTC->WTOSCCNT_f.SOSCEX = ((pstcConfig->bVbatClkEn == TRUE) ? 0u : 1u);
    
    // Transmit to VBAT domain 
    FM_RTC->WTCR20_f.PWRITE = 1u;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Configure pin funciton of P48/VREGCTL to VREGCTL
 ******************************************************************************/
void Vbat_SetPinFunc_VREGCTL(void)
{
    // Recall from VBAT domain 
    FM_RTC->WTCR20_f.PREAD = 1u;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
    
    // Set function register
    FM_RTC->VBPFR_f.VPFR0 = 1u;
    
    // Trasnmit to VBAT domain 
    FM_RTC->WTCR20_f.PWRITE = 1u;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
    
    return;
}

/**
 ******************************************************************************
 ** \brief Configure pin funciton of P49/VWAKEUP to VWAKEUP
 ******************************************************************************/
void Vbat_SetPinFunc_VWAKEUP(void)
{
    // Recall from VBAT domain 
    FM_RTC->WTCR20_f.PREAD = 1;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
    
    // Set function register
    FM_RTC->VBPFR_f.VPFR1 = 1u;
    
    // Trasnmit to VBAT domain 
    FM_RTC->WTCR20_f.PWRITE = 1;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
    
    return;
}

/**
 ******************************************************************************
 ** \brief Configure pin funciton of P46/X0A, P47/X1A to X0A, X1A
 ******************************************************************************/
void Vbat_SetPinFunc_X0A_X1A(void)
{
    // Recall from VBAT domain 
    FM_RTC->WTCR20_f.PREAD = 1;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
    
    // Set function register
    FM_RTC->VBPFR_f.SPSR0 = 1u;
    FM_RTC->VBPFR_f.SPSR1 = 0u;
    FM_RTC->VBPFR_f.VPFR2 = 1u;
    FM_RTC->VBPFR_f.VPFR3 = 1u;
    
    // Trasnmit to VBAT domain 
    FM_RTC->WTCR20_f.PWRITE = 1;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
    
    return;
}

/**
 ******************************************************************************
 ** \brief Initialize VBAT GPIO output
 **
 ** This function initializes an VBAT GPIO output
 **
 ** \param [in]  enGpio       VBAT GPIO list
 ** \param [in]  bInitLevel   GPIO output initial level
 ** \param [in]  bOpenDrain   GPIO open drain or not
 **
 ** \retval Ok                    VBAT initialized normally
 ** \retval ErrorInvalidParameter enGpio out of range
 ** 
 ** \note Open drain funciton is only available for P48 and P49.
 **
 ******************************************************************************/
en_result_t Vbat_InitGpioOutput(en_vbat_gpio_t enGpio, 
                                boolean_t bInitLevel,
                                boolean_t bOpenDrain)
{
    // Recall from VBAT domain 
    FM_RTC->WTCR20_f.PREAD = 1;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
  
    // Set GPIO registers
    switch (enGpio)
    {
        case VbatGpioP46:
            FM_RTC->VBPFR_f.SPSR0 = 0u;
            FM_RTC->VBPFR_f.SPSR1 = 0u;
            FM_RTC->VBPFR_f.VPFR3 = 0u;
            FM_RTC->VBDDR_f.VDDR3 = 1u;
            FM_RTC->VBDOR_f.VDOR3 = ((1u == bInitLevel) ? 1u : 0u);
            break;
        case VbatGpioP47:
            FM_RTC->VBPFR_f.SPSR0 = 0u;
            FM_RTC->VBPFR_f.SPSR1 = 0u;
            FM_RTC->VBPFR_f.VPFR2 = 0u;
            FM_RTC->VBDDR_f.VDDR2 = 1u;     
            FM_RTC->VBDOR_f.VDOR2 = ((1u == bInitLevel) ? 1u : 0u);
            break;
        case VbatGpioP48:
            FM_RTC->VBPFR_f.VPFR0 = 0u;
            FM_RTC->VBDDR_f.VDDR0 = 1u;
            FM_RTC->VBDOR_f.VDOR0 = ((1u == bInitLevel) ? 1u : 0u);
            FM_RTC->VBPZR_f.VPZR0 = ((1u == bOpenDrain) ? 1u : 0u);
            break;
        case VbatGpioP49:
            FM_RTC->VBPFR_f.VPFR1 = 0u;
            FM_RTC->VBDDR_f.VDDR1 = 1u;
            FM_RTC->VBDOR_f.VDOR1 = ((1u == bInitLevel) ? 1u : 0u);
            FM_RTC->VBPZR_f.VPZR1 = ((1u == bOpenDrain) ? 1u : 0u);
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    // Trasnmit to VBAT domain 
    FM_RTC->WTCR20_f.PWRITE = 1;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Initialize VBAT GPIO input
 **
 ** This function initializes an VBAT GPIO input
 **
 ** \param [in]  enGpio       VBAT GPIO list
 ** \param [in]  bPullup      GPIO pullup register connected or not
 **
 ** \retval Ok                    VBAT initialized normally
 ** \retval ErrorInvalidParameter enGpio out of range
 **
 ******************************************************************************/
en_result_t Vbat_InitGpioInput(en_vbat_gpio_t enGpio, boolean_t bPullup)
{
    // Recall from VBAT domain 
    FM_RTC->WTCR20_f.PREAD = 1;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
  
    // Set GPIO registers
    switch (enGpio)
    {
        case VbatGpioP46:
            FM_RTC->VBPFR_f.SPSR0 = 0u;
            FM_RTC->VBPFR_f.SPSR1 = 0u;
            FM_RTC->VBPFR_f.VPFR3 = 0u;
            FM_RTC->VBDDR_f.VDDR3 = 0u;
            FM_RTC->VBPCR_f.VPCR3 = ((1u == bPullup) ? 1u : 0u);
            break;
        case VbatGpioP47:
            FM_RTC->VBPFR_f.SPSR0 = 0u;
            FM_RTC->VBPFR_f.SPSR1 = 0u;
            FM_RTC->VBPFR_f.VPFR2 = 0u;
            FM_RTC->VBDDR_f.VDDR2 = 0u;     
            FM_RTC->VBPCR_f.VPCR2 = ((1u == bPullup) ? 1u : 0u);
            break;
        case VbatGpioP48:
            FM_RTC->VBPFR_f.VPFR0 = 0u;
            FM_RTC->VBDDR_f.VDDR0 = 0u;
            FM_RTC->VBPCR_f.VPCR0 = ((1u == bPullup) ? 1u : 0u);
            break;
        case VbatGpioP49:
            FM_RTC->VBPFR_f.VPFR1 = 0u;
            FM_RTC->VBDDR_f.VDDR1 = 0u;
            FM_RTC->VBPCR_f.VPCR1 = ((1u == bPullup) ? 1u : 0u);
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    // Trasnmit to VBAT domain 
    FM_RTC->WTCR20_f.PWRITE = 1;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set the output level of P46
 **
 ** \param [in]  bLevel       Output level
 **
 ******************************************************************************/
void Vbat_PutPinP46(boolean_t bLevel)
{
    // Recall from VBAT domain 
    FM_RTC->WTCR20_f.PREAD = 1;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
    
    // Set output register
    FM_RTC->VBDOR_f.VDOR3 = ((1u == bLevel) ? 1u : 0u);
    
    // Trasnmit to VBAT domain 
    FM_RTC->WTCR20_f.PWRITE = 1;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
    
    return;
}

/**
 ******************************************************************************
 ** \brief Set the output level of P47
 **
 ** \param [in]  bLevel       Output level
 **
 ******************************************************************************/
void Vbat_PutPinP47(boolean_t bLevel)
{
    // Recall from VBAT domain 
    FM_RTC->WTCR20_f.PREAD = 1;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
    
    // Set output register
    FM_RTC->VBDOR_f.VDOR2 = ((1u == bLevel) ? 1u : 0u);
    
    // Trasnmit to VBAT domain 
    FM_RTC->WTCR20_f.PWRITE = 1;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
    
    return;
}

/**
 ******************************************************************************
 ** \brief Set the output level of P48
 **
 ** \param [in]  bLevel       Output level
 **
 ******************************************************************************/
void Vbat_PutPinP48(boolean_t bLevel)
{
    // Recall from VBAT domain 
    FM_RTC->WTCR20_f.PREAD = 1;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
    
    // Set output register
    FM_RTC->VBDOR_f.VDOR0 = ((1u == bLevel) ? 1u : 0u);
    
    // Trasnmit to VBAT domain 
    FM_RTC->WTCR20_f.PWRITE = 1;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
    
    return;
}

/**
 ******************************************************************************
 ** \brief Set the output level of P49
 **
 ** \param [in]  bLevel       Output level
 **
 ******************************************************************************/
void Vbat_PutPinP49(boolean_t bLevel)
{
    // Recall from VBAT domain 
    FM_RTC->WTCR20_f.PREAD = 1;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
    
    // Set output register
    FM_RTC->VBDOR_f.VDOR1 = ((1u == bLevel) ? 1u : 0u);
    
    // Trasnmit to VBAT domain 
    FM_RTC->WTCR20_f.PWRITE = 1;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
    
    return;
}

/**
 ******************************************************************************
 ** \brief Get the input level of P46
 **
 ** \retval TRUE              High level
 ** \retval FALSE             Low level
 **
 ******************************************************************************/
boolean_t Vbat_GetPinP46(void)
{
    boolean_t bRet;
  
    // Recall from VBAT domain 
    FM_RTC->WTCR20_f.PREAD = 1u;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
    
    bRet = ((1u == FM_RTC->VBDIR_f.VDIR3) ? 1u : 0u) ;
    
    return bRet;
}

/**
 ******************************************************************************
 ** \brief Get the input level of P47
 **
 ** \retval TRUE              High level
 ** \retval FALSE             Low level
 **
 ******************************************************************************/
boolean_t Vbat_GetPinP47(void)
{
    boolean_t bRet;
  
    // Recall from VBAT domain 
    FM_RTC->WTCR20_f.PREAD = 1u;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
    
    bRet = ((1u == FM_RTC->VBDIR_f.VDIR2) ? 1u : 0u) ;
    
    return bRet;
}

/**
 ******************************************************************************
 ** \brief Get the input level of P48
 **
 ** \retval TRUE              High level
 ** \retval FALSE             Low level
 **
 ******************************************************************************/
boolean_t Vbat_GetPinP48(void)
{
    boolean_t bRet;
  
    // Recall from VBAT domain 
    FM_RTC->WTCR20_f.PREAD = 1u;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
    
    bRet = ((1u == FM_RTC->VBDIR_f.VDIR0) ? 1u : 0u) ;
    
    return bRet;
}

/**
 ******************************************************************************
 ** \brief Get the input level of P49
 **
 ** \retval TRUE              High level
 ** \retval FALSE             Low level
 **
 ******************************************************************************/
boolean_t Vbat_GetPinP49(void)
{
    boolean_t bRet;
  
    // Recall from VBAT domain 
    FM_RTC->WTCR20_f.PREAD = 1u;

    // Wait to complete transmission
    while(0u != FM_RTC->WTCR10_f.TRANS)
    {}
    
    bRet = ((1u == FM_RTC->VBDIR_f.VDIR1) ? 1u : 0u) ;
    
    return bRet;
}

/**
 ******************************************************************************
 ** \brief Start hibernation function of VBAT
 ******************************************************************************/
void Vbat_StartHibernation(void)
{
    FM_RTC->HIBRST = 1u;
    
    return;
}

/**
 ******************************************************************************
 ** \brief Get power on flag of VBAT
 **
 ** \retval TRUE Power on flag set
 ** \retval FALSE Power on flag clear
 ******************************************************************************/
boolean_t Vbat_GetPowerOnFlag(void)
{
    boolean_t bRet;
    
    bRet = (1u == FM_RTC->VDET_f.PON) ? 1u : 0u;
    
    return bRet;
}

/**
 ******************************************************************************
 ** \brief Get wakeup flag of VBAT
 **
 ** \retval TRUE wakeup flag set
 ** \retval FALSE wakeup flag clear
 ******************************************************************************/
boolean_t Vbat_GetWakeupFlag(void)
{
    boolean_t bRet;
    
    bRet = (1u == FM_RTC->EWKUP_f.WUP0) ? 1u : 0u;
    
    return bRet;
}

/**
 ******************************************************************************
 ** \brief Clear wakeup flag of VBAT
 ******************************************************************************/
void Vbat_ClrWakeupFlag(void)
{
    FM_RTC->EWKUP_f.WUP0 = 0u;
}

#endif

#endif // #if (defined(PDL_PERIPHERAL_VBAT_ACTIVE))

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
