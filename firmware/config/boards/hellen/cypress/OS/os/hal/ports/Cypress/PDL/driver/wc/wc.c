/*******************************************************************************
* \file             wc.c
* 
* \version          1.20
*
* \brief            This file provides the source code to the API for the WC 
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
#include "wc/wc.h"

#if (defined(PDL_PERIPHERAL_WC_ACTIVE))

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/
#if (PDL_INTERRUPT_ENABLE_WC0 == PDL_ON)
static func_ptr_t  pWcfnIrqCallback = NULL;
#endif

#if (PDL_INTERRUPT_ENABLE_WC0 == PDL_ON)
/******************************************************************************/
/* Local Functions                                                            */
/******************************************************************************/
/**
 ******************************************************************************
 ** \brief Set NVIC Interrupt depending on WC instance
 **
 ** \param [in]  pstcWc       Pointer to WC instance   
 **
 ******************************************************************************/
static void WcInitNvic(stc_wcn_t* pstcWc)
{
#if (PDL_MCU_CORE == PDL_FM4_CORE) 
    NVIC_ClearPendingIRQ(WC_IRQn);
    NVIC_EnableIRQ(WC_IRQn);
    NVIC_SetPriority(WC_IRQn, PDL_IRQ_LEVEL_WC0);
#elif (PDL_MCU_CORE == PDL_FM0P_CORE)
  #if (PDL_MCU_TYPE == PDL_FM0P_TYPE1)
    NVIC_ClearPendingIRQ(TIM_IRQn);
    NVIC_EnableIRQ(TIM_IRQn);
    NVIC_SetPriority(TIM_IRQn, PDL_IRQ_LEVEL_TIM_SMCIF1_HDMICEC1);
  #elif (PDL_MCU_TYPE == PDL_FM0P_TYPE3)
    NVIC_ClearPendingIRQ(TIM_IRQn);
    NVIC_EnableIRQ(TIM_IRQn);
    NVIC_SetPriority(TIM_IRQn, PDL_IRQ_LEVEL_DT_RTC_WC);
  #else  
    NVIC_ClearPendingIRQ(TIM_SMCIF1_HDMICEC1_IRQn);
    NVIC_EnableIRQ(TIM_SMCIF1_HDMICEC1_IRQn);
    NVIC_SetPriority(TIM_SMCIF1_HDMICEC1_IRQn, PDL_IRQ_LEVEL_TIM_SMCIF1_HDMICEC1);
  #endif    
#else  
    NVIC_ClearPendingIRQ(TIM_WC_RTC_IRQn);
    NVIC_EnableIRQ(TIM_WC_RTC_IRQn);
    NVIC_SetPriority(TIM_WC_RTC_IRQn, PDL_IRQ_LEVEL_CLK_WC_RTC);
#endif    
}

/**
 ******************************************************************************
 ** \brief Clear NVIC Interrupt depending on WC instance
 **
 ** \param [in]  pstcWc            Pointer to WC instance   
 ** 
 ******************************************************************************/
static void WcDeInitNvic(stc_wcn_t* pstcWc)
{
#if (PDL_MCU_CORE == PDL_FM4_CORE) 
    NVIC_ClearPendingIRQ(WC_IRQn);
    NVIC_DisableIRQ(WC_IRQn);
    NVIC_SetPriority(WC_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);   
#elif (PDL_MCU_CORE == PDL_FM0P_CORE)
  #if (PDL_MCU_TYPE == PDL_FM0P_TYPE1)
    NVIC_ClearPendingIRQ(TIM_IRQn);
    NVIC_DisableIRQ(TIM_IRQn);
    NVIC_SetPriority(TIM_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  #elif (PDL_MCU_TYPE == PDL_FM0P_TYPE3)
    NVIC_ClearPendingIRQ(TIM_IRQn);
    NVIC_DisableIRQ(TIM_IRQn);
    NVIC_SetPriority(TIM_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  #else  
    NVIC_ClearPendingIRQ(TIM_SMCIF1_HDMICEC1_IRQn);
    NVIC_DisableIRQ(TIM_SMCIF1_HDMICEC1_IRQn);
    NVIC_SetPriority(TIM_SMCIF1_HDMICEC1_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  #endif    
#else  
    NVIC_ClearPendingIRQ(TIM_WC_RTC_IRQn);
    NVIC_DisableIRQ(TIM_WC_RTC_IRQn);
    NVIC_SetPriority(TIM_WC_RTC_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);   
#endif    
}
/*!
 ******************************************************************************
 ** \brief  Watch counter interrupt function
 ** 
 ** \param [in]  pstcWc            Pointer to WC instance   
 ** 
 ******************************************************************************/
void Wc_IrqHandler(stc_wcn_t* pstcWc)
{
    if (PdlSet == Wc_GetIrqFlag(pstcWc))
    {
        Wc_ClearIrqFlag(pstcWc);
        if (NULL != pWcfnIrqCallback)
        {
            pWcfnIrqCallback();
        }
    }
}
#endif

/**
 ******************************************************************************
 ** \brief Select the input  clock an and set the division clock to be output.
 **
 ** The Function can set SEL_OUT, SEL_IN of Watch counter prescaler (hereafter WCP)
 **
 ** \param [in]  pstcWc            Pointer to WC instance   
 ** \param [in]  pstcWcPresClk     WC prescaler clock configuration
 **
 ** \retval Ok                     Write data successfully done or started.
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                                - invalid value of pstcWcPresClk->enInputClk
 **                                - invalid value of pstcWcPresClk->enOutputClk
 **
 ******************************************************************************/
en_result_t Wc_Pres_SelClk(stc_wcn_t* pstcWc, stc_wc_pres_clk_t* pstcWcPresClk)
{
    /* check for non-null pointers */
    if ( (NULL == pstcWc) || (NULL == pstcWcPresClk) )
    {
        return ErrorInvalidParameter;
    }
  
    switch (pstcWcPresClk->enInputClk)
    {
        case WcPresInClkSubOsc:
            pstcWc->CLK_SEL_f.SEL_IN = 0u;
            break;
        case WcPresInClkMainOsc:
            pstcWc->CLK_SEL_f.SEL_IN = 1u;
            break;
        #if (PDL_MCU_TYPE == PDL_FM3_TYPE12) || (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)     
        case WcPresInClkHighCr:
            pstcWc->CLK_SEL_f.SEL_IN = 2u;
            break;
        case WcPresInClkLowCr:
            pstcWc->CLK_SEL_f.SEL_IN = 3u;
            break;
        #endif    
        default:
            return ErrorInvalidParameter;
    }

    switch(pstcWcPresClk->enOutputClk)
    {
        case WcPresOutClkArray0:
            pstcWc->CLK_SEL_f.SEL_OUT = 0u;
            break;
        case WcPresOutClkArray1:
            pstcWc->CLK_SEL_f.SEL_OUT = 1u;
            break;
        #if (PDL_MCU_TYPE == PDL_FM3_TYPE12) || (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)        
        case WcPresOutClkArray2:
            pstcWc->CLK_SEL_f.SEL_OUT = 2u;
            break;
        case WcPresOutClkArray3:
            pstcWc->CLK_SEL_f.SEL_OUT = 3u;
            break;
        case WcPresOutClkArray4:
            pstcWc->CLK_SEL_f.SEL_OUT = 4u;
            break;
        case WcPresOutClkArray5:
            pstcWc->CLK_SEL_f.SEL_OUT = 5u;
            break;
        case WcPresOutClkArray6:
            pstcWc->CLK_SEL_f.SEL_OUT = 6u;
            break;
        #endif    
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Enable oscillation of the division clock.
 **
 ** The Function can set CLK_EN:CLK_EN to 1 of WCP
 **
 ** \param [in]  pstcWc         Pointer to WC instance      
 **
 ** \retval Ok                  Enable oscillation of the division clock normally
 **
 ******************************************************************************/
en_result_t Wc_Pres_EnableDiv(stc_wcn_t* pstcWc)
{
    /* check for non-null pointers */
    if (NULL == pstcWc)
    {
        return ErrorInvalidParameter;
    }
  
    pstcWc->CLK_EN_f.CLK_EN = 1u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable oscillation of the division clock.
 **
 ** The Function can clear CLK_EN:CLK_EN to 0 of WCP
 **
 ** \param [in]  pstcWc    Pointer to WC instance      
 **
 ** \retval Ok             Disable oscillation of the division clock normally
 **
 ******************************************************************************/
en_result_t Wc_Pres_DisableDiv(stc_wcn_t* pstcWc)
{
    /* check for non-null pointers */
    if (NULL == pstcWc)
    {
        return ErrorInvalidParameter;
    }
  
    pstcWc->CLK_EN_f.CLK_EN = 0u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get the operation status of the division counter.
 **
 ** The Function can get CLK_EN:CLK_EN_R to 0 of WCP
 **  
 ** \param [in]  pstcWc            Pointer to WC instance   
 **
 ** \retval PdlClr               CLK_EN_R is 0, oscillation of the division clock is not performed
 ** \retval PdlSet               CLK_EN R is 1, oscillation of the division clock is performed
 **
 ******************************************************************************/
en_stat_flag_t Wc_Pres_GetDivStat(stc_wcn_t* pstcWc)
{
    // check for non-null pointers
    if (NULL == pstcWc)
    {
        return PdlClr;
    }
  
    return ((en_stat_flag_t)(pstcWc->CLK_EN_f.CLK_EN_R));
}

/**
 ******************************************************************************
 ** \brief Select the input clock and set the division clock to be output.
 **
 ** The Function can set SEL_OUT, SEL_IN of Watch counter prescaler (hereafter WCP)
 **
 ** \param [in]  pstcWc            Pointer to WC instance
 ** \param [in]  pstcWcConfig      Pointer to WC configuration
 **
 ** \retval Ok                     The data is written to registers normally.
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                                - invalid value of pstcWcConfig->enCntClk
 **
 ******************************************************************************/
en_result_t Wc_Init(stc_wcn_t* pstcWc, const stc_wc_config_t* pstcWcConfig)
{
    // Check for NULL pointers
    if ( (NULL == pstcWcConfig) || 
         (NULL == pstcWc) )
    {
        return ErrorInvalidParameter;
    }
    
    switch (pstcWcConfig->enCntClk)
    {
        case WcCntClkWcck0:
            pstcWc->WCCR_f.CS = 0u;
            break;
        case WcCntClkWcck1:
            pstcWc->WCCR_f.CS = 1u;
            break;
        case WcCntClkWcck2:
            pstcWc->WCCR_f.CS = 2u;
            break;
        case WcCntClkWcck3:
            pstcWc->WCCR_f.CS = 3u;
            break;
        default:
           return ErrorInvalidParameter;
    }

    pstcWc->WCRL = pstcWcConfig->u8ReloadValue;
    
#if (PDL_INTERRUPT_ENABLE_WC0 == PDL_ON)        
    if(TRUE == pstcWcConfig->bIrqEnable)
    {
        pstcWc->WCCR_f.WCIE = 1u;
    }
    
    pWcfnIrqCallback = pstcWcConfig->pfnIrqCallback;
    
    if(TRUE == pstcWcConfig->bTouchNvic)
    {
        WcInitNvic(pstcWc);
    }
#endif
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief De-Initialize WC
 **
 ** Clear all registers of WC
 **
 ** \param [in]  pstcWc            WC configuration
 ** \param [in]  bTouchNvic        Touch NVIC or not
 **
 ** \retval Ok                     Write data successfully done or started.
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                                - pstcWc == NULL
 **
 ******************************************************************************/
en_result_t Wc_DeInit(stc_wcn_t* pstcWc, boolean_t bTouchNvic)
{
    // check for non-null pointers
    if (NULL == pstcWc)
    {
        return ErrorInvalidParameter;
    }
    
    pstcWc->WCCR = 0u;
    pstcWc->WCRL = 0u;
    
#if (PDL_INTERRUPT_ENABLE_WC0 == PDL_ON)      
    if(TRUE == bTouchNvic)
    {
        WcDeInitNvic(pstcWc);
    }
#endif    
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Enable WC operation.
 **
 ** The Function can set WCCR:WCEN to 1 of WC
 **
 ** \param [in]  pstcWc           Pointer to WC instance
 **
 ** \retval Ok                    Enable WC operation normally
 **
 ******************************************************************************/
en_result_t Wc_EnableCount(stc_wcn_t* pstcWc)
{
    // check for non-null pointers
    if (NULL == pstcWc)
    {
        return ErrorInvalidParameter;
    }
  
    pstcWc->WCCR_f.WCEN = 1u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable WC operation.
 **
 ** The Function can clear WCCR:WCEN to 0 of WC
 **
 ** \param [in]  pstcWc       Pointer to WC instance
 **
 ** \retval Ok                Disable WC operation normally
 **
 ******************************************************************************/
en_result_t Wc_DisableCount(stc_wcn_t* pstcWc)
{
    // check for non-null pointers
    if (NULL == pstcWc)
    {
        return ErrorInvalidParameter;
    }
  
    pstcWc->WCCR_f.WCEN = 0u;
    
    return Ok;
}

#if (PDL_INTERRUPT_ENABLE_WC0 == PDL_ON)
/**
 ******************************************************************************
 ** \brief Enable WC underflow interrupt.
 **
 ** The Function can set WCIE of WC
 **
 ** \param [in]  pstcWc             Pointer to WC instance
 **
 ** \retval Ok                      WC enabled successfully.
 ** \retval ErrorInvalidParameter   If one of following conditions are met:
 **                                 - pstcWc == NULL
 **
 ******************************************************************************/
en_result_t Wc_EnableIrq(stc_wcn_t* pstcWc)
{
    // check for non-null pointers
    if (NULL == pstcWc)
    {
        return ErrorInvalidParameter;
    }
    
    pstcWc->WCCR_f.WCIE = 1u;

    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable WC underflow interrupt.
 **
 ** The Function can clear WCIE of WC
 **
 ** \param [in]  pstcWc             Pointer to WC instance
 **
 **
 ** \retval Ok                      Disable WC underflow interrupt normally
 ** \retval ErrorInvalidParameter   If one of following conditions are met:
 **                                 - pstcWc == NULL
 **
 ******************************************************************************/
en_result_t Wc_DisableIrq(stc_wcn_t* pstcWc)
{
    // check for non-null pointers
    if (NULL == pstcWc)
    {
        return ErrorInvalidParameter;
    }
  
    pstcWc->WCCR_f.WCIE = 0u;

    return Ok;
}
#endif
/**
 ******************************************************************************
 ** \brief Set the counter value of WC
 **
 ** The Function can set value to WCRL of WC
 **
 ** \param [in]  pstcWc             Pointer to WC instance
 ** \param [in]  u8Val              Set the reload count value 
 **
 **
 ** \retval Ok                      Set the counter value of WC normally
 ** \retval ErrorInvalidParameter   If one of following conditions are met:
 **                                 - pstcWc == NULL
 **
 ******************************************************************************/
en_result_t Wc_WriteReloadVal(stc_wcn_t* pstcWc, uint8_t u8Val)
{
    // check for non-null pointers
    if (NULL == pstcWc)
    {
        return ErrorInvalidParameter;
    }
  
    pstcWc->WCRL = u8Val;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Read the value in the 6-bit down counter
 **
 ** The Function can read value of WCRD of WC
 **
 ** \param [in]  pstcWc             Pointer to WC instance
 **
 ** \retval Current count value of WC
 **
 ******************************************************************************/
uint8_t Wc_ReadCurCnt(stc_wcn_t* pstcWc)
{
    // check for non-null pointers
    if (NULL == pstcWc)
    {
        return 0xFFu;
    }
  
    return (pstcWc->WCRD);
}

/**
 ******************************************************************************
 ** \brief Clear WC underflow flag
 **
 ** The Function clear WCIF to 0 of WC
 **
 ** \param [in]  pstcWc             Pointer to WC instance
 **
 ** \retval Ok               Clear WC underflow flag normally
 ** \retval ErrorInvalidParameter   If one of following conditions are met:
 **                                 - pstcWc == NULL
 **
 ******************************************************************************/
en_result_t Wc_ClearIrqFlag(stc_wcn_t* pstcWc)
{
    // check for non-null pointers
    if (NULL == pstcWc)
    {
        return ErrorInvalidParameter;
    }
  
    pstcWc->WCCR_f.WCIF = 0u;
    
    return Ok;    
}

/**
 ******************************************************************************
 ** \brief Get WC underflow flag status
 **
 ** The Function get value WCIF of WC
 **
 ** \param [in]  pstcWc          Pointer to WC instance
 **
 ** \retval PdlClr               WCIF is 0, WC underflow does not occur
 ** \retval PdlSet               WCIF is 1, WC underflow occurs
 **
 ******************************************************************************/
en_irq_flag_t Wc_GetIrqFlag(stc_wcn_t* pstcWc)
{
    // check for non-null pointers
    if (NULL == pstcWc)
    {
        return PdlClr;
    }
  
    return ((en_irq_flag_t)(pstcWc->WCCR_f.WCIF));
}

/**
 ******************************************************************************
 ** \brief Get WC operation state
 **
 ** The Function get value WCOP of WC
 **
 ** \param [in]  pstcWc          Pointer to WC instance
 **
 ** \retval PdlClr               WCOP is 0, The WC is stopped
 ** \retval PdlSet               WCOP is 1, The WC is active
 **
 ******************************************************************************/
en_stat_flag_t Wc_GetOperationFlag(stc_wcn_t* pstcWc)
{
    // check for non-null pointers
    if (NULL == pstcWc)
    {
        return PdlClr;
    }
  
    return ((en_stat_flag_t)(pstcWc->WCCR_f.WCOP));
}


#endif // #if (defined(PDL_PERIPHERAL_WC_ACTIVE))

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
