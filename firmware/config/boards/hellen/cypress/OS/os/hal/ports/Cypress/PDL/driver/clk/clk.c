/******************************************************************************
* \file             clk.c
*
* \version          1.20
*
* \brief            Clock driver
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
#include "clk/clk.h"

#if (defined(PDL_PERIPHERAL_CLK_ACTIVE))

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/
#if (PDL_INTERRUPT_ENABLE_CLK == PDL_ON)
stc_clk_intern_data_t stcClkInternData;
#endif

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
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

/**
 ******************************************************************************
 ** \brief Clock Stabilization Interrupt Handler
 ******************************************************************************/
#if (PDL_INTERRUPT_ENABLE_CLK == PDL_ON)
void Clk_IrqHandler(void)
{
  uint8_t u8IntStrReadOut;
  
  u8IntStrReadOut = FM_CRG->INT_STR;
  
  // PLL stabilization ready?
  if (0u != (u8IntStrReadOut & FM_INT_STR_PCSI_BITPOS))
  {
    FM_CRG->INT_CLR |= FM_INT_CLR_PCSC_BITPOS;  // Clear Irq
    
    // PLL ready callback if defined
    if (NULL != stcClkInternData.pfnPllStabCb)
    {
      stcClkInternData.pfnPllStabCb();
    }
  }

  // Sub Clock stabilization ready?  
  if (0u != (u8IntStrReadOut & FM_INT_STR_SCSI_BITPOS))
  {
    FM_CRG->INT_CLR |= FM_INT_CLR_SCSC_BITPOS;  // Clear Irq
    
    // Sub Clock ready callback if defined
    if (NULL != stcClkInternData.pfnScoStabCb)
    {
      stcClkInternData.pfnScoStabCb();
    }
  }

  // Main Clock stabilization ready?  
  if (0u != (u8IntStrReadOut & FM_INT_STR_MCSI_BITPOS))
  {
    FM_CRG->INT_CLR |= FM_INT_CLR_MCSC_BITPOS;  // Clear Irq
    
    // Main Clock ready callback if defined
    if (NULL != stcClkInternData.pfnMcoStabCb)
    {
      stcClkInternData.pfnMcoStabCb();
    }
  }
}
#endif

/**
 ******************************************************************************
 ** \brief Initialize system clock according to user configuration
 **
 ** Set the definition CLOCK_SETUP to "CLOCK_SETTING_NONE" when using this
 ** function to initialize system clock.
 **
 ** \param [in]  pstcClk         Pointer to clock configuration structure
 **
 ** \retval Ok    Clock initialized normally
 ** \retval ErrorInvalidParameter    The paramter is set to error range    
 ******************************************************************************/
en_result_t Clk_Init(const stc_clk_config_t* pstcClk) 
{
    if(pstcClk == NULL)
    {
        return ErrorInvalidParameter;
    }
    
    /* Set base clock dividor */
    switch(pstcClk->enBaseClkDiv)
    {
        case BaseClkDiv1:
            FM_CRG->BSC_PSR_f.BSR = 0u;
            break;
        case BaseClkDiv2:
            FM_CRG->BSC_PSR_f.BSR = 1u;
            break;    
        case BaseClkDiv3:
            FM_CRG->BSC_PSR_f.BSR = 2u;
            break;
        case BaseClkDiv4:
            FM_CRG->BSC_PSR_f.BSR = 3u;
            break;
        case BaseClkDiv6:
            FM_CRG->BSC_PSR_f.BSR = 4u;
            break;
        case BaseClkDiv8:
            FM_CRG->BSC_PSR_f.BSR = 5u;
            break;
        case BaseClkDiv16:
            FM_CRG->BSC_PSR_f.BSR = 6u;
            break;    
        default:
            return ErrorInvalidParameter;
            
    }
    
    /* Set APB0 bus clock dividor */
    switch(pstcClk->enAPB0Div)
    {
        case Apb0Div1:
            FM_CRG->APBC0_PSR_f.APBC0 = 0u;
            break;
        case Apb0Div2:
            FM_CRG->APBC0_PSR_f.APBC0 = 1u;
            break;
        case Apb0Div4:
            FM_CRG->APBC0_PSR_f.APBC0 = 2u;
            break;
        case Apb0Div8:
            FM_CRG->APBC0_PSR_f.APBC0 = 3u;
            break;    
        default:
            return ErrorInvalidParameter;
    }
    
    /* Set APB1 bus clock dividor */
    switch(pstcClk->enAPB1Div)
    {
        case Apb1Div1:
            FM_CRG->APBC1_PSR_f.APBC1 = 0u;
            break;
        case Apb1Div2:
            FM_CRG->APBC1_PSR_f.APBC1 = 1u;
            break;
        case Apb1Div4:
            FM_CRG->APBC1_PSR_f.APBC1 = 2u;
            break;
        case Apb1Div8:
            FM_CRG->APBC1_PSR_f.APBC1 = 3u;
            break;    
        default:
            return ErrorInvalidParameter;
    }
    
    if(TRUE == pstcClk->bAPB1Disable)
    {
        FM_CRG->APBC1_PSR_f.APBC1EN = 0;
    }
    
    /* Configure stability wait time */
    FM_CRG->CSW_TMR_f.MOWT = pstcClk->enMCOWaitTime;
    FM_CRG->CSW_TMR_f.SOWT = pstcClk->enSCOWaitTime;
    FM_CRG->PSW_TMR_f.POWT = pstcClk->enPLLOWaitTime;
    
#if (PDL_INTERRUPT_ENABLE_CLK == PDL_ON)    
    /* Configure interrupt */
    if(TRUE == pstcClk->bMcoIrq)
    {
        if(NULL == pstcClk->pfnMcoStabCb)
        {
            return ErrorInvalidParameter;
        }
        
        FM_CRG->INT_ENR_f.MCSE = 1;
        stcClkInternData.pfnMcoStabCb = pstcClk->pfnMcoStabCb;
    }
    
    if(TRUE == pstcClk->bScoIrq)
    {
        if(NULL == pstcClk->pfnScoStabCb)
        {
            return ErrorInvalidParameter;
        }
        
        FM_CRG->INT_ENR_f.SCSE = 1;
        stcClkInternData.pfnScoStabCb = pstcClk->pfnScoStabCb;
    }
    
    if(TRUE == pstcClk->bPllIrq)
    {
        if(NULL == pstcClk->pfnPllStabCb)
        {
            return ErrorInvalidParameter;
        }
        
        FM_CRG->INT_ENR_f.PCSE = 1;
        stcClkInternData.pfnPllStabCb = pstcClk->pfnPllStabCb;
    }
    
    #if (PDL_MCU_CORE == PDL_FM3_CORE)
    NVIC_ClearPendingIRQ(TIM_WC_RTC_IRQn);
    NVIC_EnableIRQ(TIM_WC_RTC_IRQn);
    NVIC_SetPriority(TIM_WC_RTC_IRQn, PDL_IRQ_LEVEL_CLK_WC_RTC);
    #elif (PDL_MCU_CORE == PDL_FM4_CORE)
    NVIC_ClearPendingIRQ(TIM_IRQn);
    NVIC_EnableIRQ(TIM_IRQn);
    NVIC_SetPriority(TIM_IRQn, PDL_IRQ_LEVEL_WC0);
    #else
    #if (PDL_MCU_TYPE == PDL_FM0P_TYPE3)
    NVIC_ClearPendingIRQ(TIM_IRQn);
    NVIC_EnableIRQ(TIM_IRQn);
    NVIC_SetPriority(TIM_IRQn, PDL_IRQ_LEVEL_CLK);
    #elif (PDL_MCU_TYPE == PDL_FM0P_TYPE2)    
    NVIC_ClearPendingIRQ(TIM_SMCIF1_HDMICEC1_IRQn);
    NVIC_EnableIRQ(TIM_SMCIF1_HDMICEC1_IRQn);
    NVIC_SetPriority(TIM_SMCIF1_HDMICEC1_IRQn, PDL_IRQ_LEVEL_TIM_SMCIF1_HDMICEC1);
    #else
    NVIC_ClearPendingIRQ(TIM_IRQn);
    NVIC_EnableIRQ(TIM_IRQn);
    NVIC_SetPriority(TIM_IRQn, PDL_IRQ_LEVEL_TIM_SMCIF1_HDMICEC1);
    #endif
    #endif    
    
#endif    
    
    /* Set PLL K, M, N */
    FM_CRG->PLL_CTL1_f.PLLK = pstcClk->u8PllK - 1u;
    FM_CRG->PLL_CTL1_f.PLLM = pstcClk->u8PllM - 1u;
    FM_CRG->PLL_CTL2 = pstcClk->u8PllN - 1u;
    
    return Ok;
}

#if (PDL_MCU_CORE == PDL_FM0P_CORE)   
/**
 ******************************************************************************
 ** \brief Enable high speed CR
 **
 ** This function easily enables the high speed CR. No configuration is needed.
 **
 ** \param bBlock    Wait until CR stability or not
 ** \arg   FALSE     Return immediately after enable high speed CR
 ** \arg   TRUE      Wait until CR stability after enable high speed CR
 **
 ** \retval Ok                high speed CR enabled
 ******************************************************************************/
en_result_t Clk_EnableHscr(boolean_t bBlock)
{
  FM_CRG->SCM_CTL_f.HCRE = 1u;
  
  if(TRUE == bBlock)
  {
    while(FM_CRG->SCM_STR_f.HCRDY != 1u);
  }

  return Ok;
} // Clk_EnableMainClock

/**
 ******************************************************************************
 ** \brief Disable Main Clock
 **
 ** This function easily disables the Main Clock. No configuration is needed.
 **
 ** \retval Ok                Main Clock disabled
 ******************************************************************************/
en_result_t Clk_DisableHscr(void)
{
  FM_CRG->SCM_CTL_f.HCRE = 0u;

  return Ok;
} // Clk_DisableMainClock
#endif

/**
 ******************************************************************************
 ** \brief Enable Main Clock and wait until it is stable
 **
 ** This function easily enables the Main Clock. No configuration is needed.
 ** 
 ** \param bBlock    Wait until Main Clock stability or not
 ** \arg   FALSE     Return immediately after enable Main Clock
 ** \arg   TRUE      Wait until Main Clock stability after enable Main Clock
 **
 ** \retval Ok                Main Clock enabled
 ******************************************************************************/
en_result_t Clk_EnableMainClock(boolean_t bBlock)
{
  FM_CRG->SCM_CTL_f.MOSCE = 1u;
  
  if(TRUE == bBlock)
  {
    while(1u != FM_CRG->SCM_STR_f.MORDY);
  }

  return Ok;
} // Clk_EnableMainClock

/**
 ******************************************************************************
 ** \brief Disable Main Clock
 **
 ** This function easily disables the Main Clock. No configuration is needed.
 **
 ** \retval Ok                Main Clock disabled
 ******************************************************************************/
en_result_t Clk_DisableMainClock(void)
{
  FM_CRG->SCM_CTL_f.MOSCE = 0u;

  return Ok;
} // Clk_DisableMainClock


/**
 ******************************************************************************
 ** \brief Enable Sub Clock
 **
 ** This function easily enables the Sub Clock. No configuration is needed.
 **
 ** \param bBlock    Wait until Sub Clock stability or not
 ** \arg   FALSE     Return immediately after enable Sub Clock
 ** \arg   TRUE      Wait until Sub Clock stability after enable Sub Clock
 **
 ** \retval Ok                Sub Clock enabled
 ******************************************************************************/
en_result_t Clk_EnableSubClock(boolean_t bBlock)
{
  FM_CRG->SCM_CTL_f.SOSCE = 1u;
  
  if(TRUE == bBlock)
  {
    while(1u != FM_CRG->SCM_STR_f.SORDY);
  }

  return Ok;
} // Clk_EnableSubClock

/**
 ******************************************************************************
 ** \brief Disable Sub Clock
 **
 ** This function easily disables the Sub Clock. No configuration is needed.
 **
 ** \retval Ok                Sub Clock disabled
 ******************************************************************************/
en_result_t Clk_DisableSubClock(void)
{
  FM_CRG->SCM_CTL_f.SOSCE = 0u;

  return Ok;
} // Clk_DisableSubClock


/**
 ******************************************************************************
 ** \brief Enable PLL Clock
 **
 ** This function easily enables the PLL Clock. No configuration is needed.
 **
 ** \param bBlock    Wait until PLL Clock stability or not
 ** \arg   FALSE     Return immediately after enable PLL Clock
 ** \arg   TRUE      Wait until PLL Clock stability after enable PLL Clock
 **
 ** \retval Ok                PLL Clock enabled
 ******************************************************************************/
en_result_t Clk_EnablePllClock(boolean_t bBlock)
{
  FM_CRG->SCM_CTL_f.PLLE = 1u;
  
  if(bBlock == TRUE)
  {
    while(1u != FM_CRG->SCM_STR_f.PLRDY);
  }

  return Ok;
} // Clk_EnablePllClock

/**
 ******************************************************************************
 ** \brief Disable PLL Clock
 **
 ** This function easily disables the PLL Clock. No configuration is needed.
 **
 ** \retval Ok                PLL Clock disabled
 ******************************************************************************/
en_result_t Clk_DisablePllClock(void)
{
  FM_CRG->SCM_CTL_f.PLLE = 0u;

  return Ok;
} // Clk_DisableSubClock

/**
 ******************************************************************************
 ** \brief Set Clock Source
 **
 ** This function sets the clock source and performs transition, if wanted.
 **
 ** \param [in]  enSource        System source clock
 ** \arg         ClkMain         Set Main Clock as system source clock
 ** \arg         ClkSub          Set Sub Clock as system source clock
 ** \arg         ClkHsCr         Set High-speed CR as system source clock
 ** \arg         ClkLsCr         Set Low-speed CR as system source clock
 ** \arg         ClkPll          Set Main PLL clock as system source clock
 ** \arg         ClkHsCrPll      Set High-speed CR PLL clock as system source clock
 ** 
 ** \retval Ok                     Clock source set
 ** \retval ErrorInvalidParameter  pstcConfig == NULL or Illegal mode
 ** \retval ErrorInvalidMode       Clock setting not possible
 ******************************************************************************/
en_result_t Clk_SetSource(en_clk_source_t enSource)
{  
  uint8_t u8Rcs, u8Rcm;
  switch(enSource)
  {
    case ClkMain:
      if ((TRUE != FM_CRG->SCM_CTL_f.MOSCE) ||  // Main Oscillator ready?
          (TRUE != FM_CRG->SCM_STR_f.MORDY))
      {
        return ErrorInvalidMode ;
      }
      FM_CRG->SCM_CTL_f.RCS = 0x1u;
      break;
    case ClkSub:
      if ((TRUE != FM_CRG->SCM_CTL_f.SOSCE) ||  // Sub Oscillator ready?
          (TRUE != FM_CRG->SCM_STR_f.SORDY))
      {
        return ErrorInvalidMode ;
      }
      FM_CRG->SCM_CTL_f.RCS = 0x5u;
      break;
    case ClkHsCr:                                // Always possible
      FM_CRG->SCM_CTL_f.RCS = 0x0u;
      break;     
    case ClkLsCr:                                // Always possible
      FM_CRG->SCM_CTL_f.RCS = 0x4u;
      break;
    case ClkHsCrPll:  
      FM_CRG->PSW_TMR_f.PINC = 1u;
      FM_CRG->SCM_CTL_f.RCS = 0x2u;
      break;
    case ClkPll:                           
      if ((TRUE != FM_CRG->SCM_STR_f.MORDY) ||  // PLL ready?
          (TRUE != FM_CRG->SCM_STR_f.PLRDY))
      {
        return ErrorInvalidMode ;
      }
      
      FM_CRG->PSW_TMR_f.PINC = 0u;
      FM_CRG->SCM_CTL_f.RCS = 0x2u;
      break;
    default:
      return ErrorInvalidParameter ;
  }

  /* Wait until switch stable */
  while(1)
  {
    u8Rcs = FM_CRG->SCM_CTL_f.RCS;
    u8Rcm = FM_CRG->SCM_STR_f.RCM;
    if(u8Rcs == u8Rcm)
    {
        break;
    }
  }

  return Ok;
} // Clk_SetSource

#if (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)
/**
 ******************************************************************************
 ** \brief Enables the clock gate of a peripheral
 **
 ** This function sets the corresponding bit in the CKENn register to enable
 ** the clock of a peripheral.
 **
 ** \param  enPeripheral           Enumerator of a peripheral, see
 **                                #en_clk_gate_peripheral_t for details
 **
 ** \retval Ok                     Peripheral clock enabled
 ** \retval ErrorInvalidParameter  Peripheral enumerator does not exist
 ******************************************************************************/
en_result_t Clk_PeripheralClockEnable(en_clk_gate_peripheral_t enPeripheral)
{
  switch (enPeripheral)
  {
    case ClkGateGpio:
#if defined(bFM_CLK_GATING_CKEN0_GIOCK)
      bFM_CLK_GATING_CKEN0_GIOCK = 1u;
#endif      
      break;     
    case ClkGateDma:
#if defined(bFM_CLK_GATING_CKEN0_DMACK)
      bFM_CLK_GATING_CKEN0_DMACK = 1u;
#endif      
      break;
    case ClkGateAdc0:
#if defined(bFM_CLK_GATING_CKEN0_ADCCK0)
      bFM_CLK_GATING_CKEN0_ADCCK0 = 1u;
#endif      
      break;
    case ClkGateAdc1:     
#if defined(bFM_CLK_GATING_CKEN0_ADCCK1)
      bFM_CLK_GATING_CKEN0_ADCCK1 = 1u;
#endif       
      break;
    case ClkGateAdc2:
#if defined(bFM_CLK_GATING_CKEN0_ADCCK2)
      bFM_CLK_GATING_CKEN0_ADCCK2 = 1u;
#endif      
      break;
    case ClkGateAdc3:
#if defined(bFM_CLK_GATING_CKEN0_ADCCK3)
      bFM_CLK_GATING_CKEN0_ADCCK3 = 1u;
#endif       
      break;
    case ClkGateMfs0:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK0)
      bFM_CLK_GATING_CKEN0_MFSCK0 = 1u;
#endif       
      break;
    case ClkGateMfs1:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK1)
      bFM_CLK_GATING_CKEN0_MFSCK1 = 1u;
#endif      
      break;
    case ClkGateMfs2:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK2)
      bFM_CLK_GATING_CKEN0_MFSCK2 = 1u;
#endif       
      break;
    case ClkGateMfs3:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK3)
      bFM_CLK_GATING_CKEN0_MFSCK3 = 1u;
#endif       
      break;
    case ClkGateMfs4:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK4)
      bFM_CLK_GATING_CKEN0_MFSCK4 = 1u;
#endif       
      break;
    case ClkGateMfs5:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK5)
      bFM_CLK_GATING_CKEN0_MFSCK5 = 1u;
#endif      
      break;
    case ClkGateMfs6:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK6)
      bFM_CLK_GATING_CKEN0_MFSCK6 = 1u;
#endif      
      break;
    case ClkGateMfs7:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK7)
      bFM_CLK_GATING_CKEN0_MFSCK7 = 1u;
#endif
      break;
    case ClkGateMfs8:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK8)
      bFM_CLK_GATING_CKEN0_MFSCK8 = 1u;
#endif      
      break;
    case ClkGateMfs9:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK9)
      bFM_CLK_GATING_CKEN0_MFSCK9 = 1u;
#endif       
      break;
    case ClkGateMfs10:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK10)
      bFM_CLK_GATING_CKEN0_MFSCK10 = 1u;
#endif       
      break;
    case ClkGateMfs11:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK11)
      bFM_CLK_GATING_CKEN0_MFSCK11 = 1u;
#endif      
      break;
    case ClkGateMfs12:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK12)
      bFM_CLK_GATING_CKEN0_MFSCK12 = 1u;
#endif       
      break;
    case ClkGateMfs13:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK13)
      bFM_CLK_GATING_CKEN0_MFSCK13 = 1u;
#endif       
      break;
    case ClkGateMfs14:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK14)
      bFM_CLK_GATING_CKEN0_MFSCK14 = 1u;
#endif      
      break;
    case ClkGateMfs15:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK15)
      bFM_CLK_GATING_CKEN0_MFSCK15 = 1u;
#endif       
      break;
    case ClkGateQprc0:
#if defined(bFM_CLK_GATING_CKEN1_QDUCK0)
      bFM_CLK_GATING_CKEN1_QDUCK0 = 1u;
#endif       
      break;
    case ClkGateQprc1:
#if defined(bFM_CLK_GATING_CKEN1_QDUCK1)
      bFM_CLK_GATING_CKEN1_QDUCK1 = 1u;
#endif      
      break;
    case ClkGateQprc2:
#if defined(bFM_CLK_GATING_CKEN1_QDUCK2)
      bFM_CLK_GATING_CKEN1_QDUCK2 = 1u;
#endif       
      break;
    case ClkGateQprc3:
#if defined(bFM_CLK_GATING_CKEN1_QDUCK3)
      bFM_CLK_GATING_CKEN1_QDUCK3 = 1u;
#endif       
      break;
    case ClkGateMft0:
#if defined(bFM_CLK_GATING_CKEN1_MFTCK0)
      bFM_CLK_GATING_CKEN1_MFTCK0 = 1u;
#endif       
      break;
    case ClkGateMft1:
#if defined(bFM_CLK_GATING_CKEN1_MFTCK1)
      bFM_CLK_GATING_CKEN1_MFTCK1 = 1u;
#endif      
      break;
    case ClkGateMft2:
#if defined(bFM_CLK_GATING_CKEN1_MFTCK2)
      bFM_CLK_GATING_CKEN1_MFTCK2 = 1u;
#endif        
      break;
    case ClkGateMft3:
#if defined(bFM_CLK_GATING_CKEN1_MFTCK3)
      bFM_CLK_GATING_CKEN1_MFTCK3 = 1u;
#endif       
      break;
    case ClkGateBt0123:
#if defined(bFM_CLK_GATING_CKEN1_BTMCK0)
      bFM_CLK_GATING_CKEN1_BTMCK0 = 1u;
#endif        
      break;
    case ClkGateBt4567:
#if defined(bFM_CLK_GATING_CKEN1_BTMCK1)
      bFM_CLK_GATING_CKEN1_BTMCK1 = 1u;
#endif       
      break;
    case ClkGateBt891011:
#if defined(bFM_CLK_GATING_CKEN1_BTMCK2)
      bFM_CLK_GATING_CKEN1_BTMCK2 = 1u;
#endif      
      break;
    case ClkGateBt12131415:
#if defined(bFM_CLK_GATING_CKEN1_BTMCK3)
      bFM_CLK_GATING_CKEN1_BTMCK3 = 1u;
#endif       
      break;
    case ClkGateExtif:
#if defined(bFM_CLK_GATING_CKEN0_EXBCK)
      bFM_CLK_GATING_CKEN0_EXBCK = 1u;
#endif        
      break;      
    case ClkGateUsb0:
#if defined(bFM_CLK_GATING_CKEN2_USBCK0 )
      bFM_CLK_GATING_CKEN2_USBCK0  = 1u;
#endif       
      break;
    case ClkGateUsb1:
#if defined(bFM_CLK_GATING_CKEN2_USBCK1 )
      bFM_CLK_GATING_CKEN2_USBCK1  = 1u;
#endif       
      break;    
    case ClkGateCan0:
#if defined(bFM_CLK_GATING_CKEN2_CANCK0 )
      bFM_CLK_GATING_CKEN2_CANCK0  = 1u;
#endif        
      break;
    case ClkGateCan1:
#if defined(bFM_CLK_GATING_CKEN2_CANCK1 )
      bFM_CLK_GATING_CKEN2_CANCK1  = 1u;
#endif      
      break;      
    case ClkGateCan2:
#if defined(bFM_CLK_GATING_CKEN2_CANCK2 )
      bFM_CLK_GATING_CKEN2_CANCK2  = 1u;
#endif      
      break;
    case ClkGateSd:
#if defined(bFM_CLK_GATING_CKEN2_SDCCK )
      bFM_CLK_GATING_CKEN2_SDCCK  = 1u;
#endif        
      break; 
    case ClkGateI2s0:
#if defined(bFM_CLK_GATING_CKEN2_I2SCK0 )
      bFM_CLK_GATING_CKEN2_I2SCK0  = 1u;
#endif      
      break;
    case ClkGateI2s1:
#if defined(bFM_CLK_GATING_CKEN2_I2SCK1 )
      bFM_CLK_GATING_CKEN2_I2SCK1  = 1u;
#endif       
      break;      
    case ClkGatePcrc:
#if defined(bFM_CLK_GATING_CKEN2_PCRCCK )
      bFM_CLK_GATING_CKEN2_PCRCCK  = 1u;
#endif        
      break;
    case ClkGateQspi:
#if defined(bFM_CLK_GATING_CKEN2_QSPICK )
      bFM_CLK_GATING_CKEN2_QSPICK  = 1u;
#endif         
      break; 
#if (PDL_MCU_CORE == PDL_FM0P_CORE)  
    case ClkGateCec0:
    case ClkGateCec1:
#if defined(bFM_CLK_GATING_CKEN2_CECCK )      
      bFM_CLK_GATING_CKEN2_CECCK = 1u;
#endif      
      break;
#else
    case ClkGateCec0:
#if defined(bFM_CLK_GATING_CKEN2_CECCK0)      
        bFM_CLK_GATING_CKEN2_CECCK0 = 1u;
#endif          
        break;
    case ClkGateCec1:
#if defined(bFM_CLK_GATING_CKEN2_CECCK1)      
        bFM_CLK_GATING_CKEN2_CECCK1 = 1u;
#endif       
        break;    
#endif
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
    case ClkGateIcc0:
#if defined(bFM_CLK_GATING_CKEN2_ICCCK0) 
      bFM_CLK_GATING_CKEN2_ICCCK0 = 1u;
#endif      
      break;
    case ClkGateIcc1:
#if defined(bFM_CLK_GATING_CKEN2_ICCCK1) 
      bFM_CLK_GATING_CKEN2_ICCCK1 = 1u;
#endif        
      break;
    case ClkGateI2sl0:
#if defined(bFM_CLK_GATING_CKEN2_IISCCK0) 
      bFM_CLK_GATING_CKEN2_IISCCK0 = 1u;
#endif      
      break;
    case ClkGateI2sl1:
#if defined(bFM_CLK_GATING_CKEN2_IISCCK1) 
      bFM_CLK_GATING_CKEN2_IISCCK1 = 1u;
#endif      
      break;      
#endif      
    default:
      return ErrorInvalidParameter;
  }
  
  return Ok;
} // Clk_PeripheralClockDisable
  
/**
 ******************************************************************************
 ** \brief Read the clock gate state of a peripheral
 **
 ** This function reads out the corresponding bit in the CKENn register.
 **
 ** \param  enPeripheral           Enumerator of a peripheral, see
 **                                #en_clk_gate_peripheral_t for details
 **
 ** \retval TRUE                   Peripheral clock enabled
 ** \retval FALSE                  Peripheral clock not enabled, peripheral
 **                                not existing
 ******************************************************************************/
boolean_t Clk_PeripheralGetClockState(en_clk_gate_peripheral_t enPeripheral)
{
  switch (enPeripheral)
  {
    case ClkGateGpio:
#if defined(bFM_CLK_GATING_CKEN0_GIOCK)
      return ((1u == bFM_CLK_GATING_CKEN0_GIOCK) ? TRUE : FALSE);
#else
      break;
#endif        
    case ClkGateDma:
#if defined(bFM_CLK_GATING_CKEN0_DMACK)
      return ((1u == bFM_CLK_GATING_CKEN0_DMACK) ? TRUE : FALSE);
#else
      break;
#endif      
    case ClkGateAdc0:
#if defined(bFM_CLK_GATING_CKEN0_ADCCK0)
      return ((1u == bFM_CLK_GATING_CKEN0_ADCCK0) ? TRUE: FALSE);
#else
      break;
#endif      
    case ClkGateAdc1:     
#if defined(bFM_CLK_GATING_CKEN0_ADCCK1)
      return ((1u == bFM_CLK_GATING_CKEN0_ADCCK1) ? TRUE : FALSE);
#else
      break;
#endif       
    case ClkGateAdc2:
#if defined(bFM_CLK_GATING_CKEN0_ADCCK2)
      return ((1u == bFM_CLK_GATING_CKEN0_ADCCK2) ? TRUE : FALSE);
#else
      break;
#endif      
    case ClkGateAdc3:
#if defined(bFM_CLK_GATING_CKEN0_ADCCK3)
      return ((1u == bFM_CLK_GATING_CKEN0_ADCCK3) ? TRUE : FALSE);
#else
      break;
#endif       
    case ClkGateMfs0:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK0)
      return ((1u == bFM_CLK_GATING_CKEN0_MFSCK0) ? TRUE : FALSE);
#else
      break;
#endif       
    case ClkGateMfs1:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK1)
      return ((1u == bFM_CLK_GATING_CKEN0_MFSCK1) ? TRUE : FALSE);
#else
      break;
#endif      
    case ClkGateMfs2:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK2)
      return (1u == bFM_CLK_GATING_CKEN0_MFSCK2) ? TRUE : FALSE;
#else
      break;
#endif       
    case ClkGateMfs3:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK3)
      return ((1u == bFM_CLK_GATING_CKEN0_MFSCK3) ? TRUE : FALSE);
#else
      break;
#endif       
    case ClkGateMfs4:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK4)
      return (1u == bFM_CLK_GATING_CKEN0_MFSCK4) ? TRUE : FALSE;
#else
      break;
#endif       
    case ClkGateMfs5:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK5)
      return ((1u == bFM_CLK_GATING_CKEN0_MFSCK5) ? TRUE : FALSE);
#else
      break;
#endif      
    case ClkGateMfs6:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK6)
      return ((1u == bFM_CLK_GATING_CKEN0_MFSCK6) ? TRUE : FALSE);
#else
      break;
#endif      
    case ClkGateMfs7:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK7)
      return ((1u == bFM_CLK_GATING_CKEN0_MFSCK7) ? TRUE : FALSE);
#else
      break;
#endif
    case ClkGateMfs8:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK8)
      return ((1u == bFM_CLK_GATING_CKEN0_MFSCK8) ? TRUE : FALSE);
#else
      break;
#endif      
    case ClkGateMfs9:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK9)
      return ((1u == bFM_CLK_GATING_CKEN0_MFSCK9) ? TRUE : FALSE);
#else
      break;
#endif       
    case ClkGateMfs10:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK10)
      return ((1u == bFM_CLK_GATING_CKEN0_MFSCK10) ? TRUE: FALSE);
#else
      break;
#endif       
    case ClkGateMfs11:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK11)
      return ((1u == bFM_CLK_GATING_CKEN0_MFSCK11) ? TRUE : FALSE);
#else
      break;
#endif
    case ClkGateMfs12:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK12)
      return ((1u == bFM_CLK_GATING_CKEN0_MFSCK12) ? TRUE : FALSE);
#else
      break;
#endif       
    case ClkGateMfs13:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK13)
      return ((1u == bFM_CLK_GATING_CKEN0_MFSCK13) ? TRUE : FALSE);
#else
      break;
#endif       
    case ClkGateMfs14:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK14)
      return ((1u == bFM_CLK_GATING_CKEN0_MFSCK14) ? TRUE : FALSE);
#else
      break;
#endif      
    case ClkGateMfs15:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK15)
      return ((1u == bFM_CLK_GATING_CKEN0_MFSCK15) ? TRUE : FALSE);
#else
      break;
#endif       
    case ClkGateQprc0:
#if defined(bFM_CLK_GATING_CKEN1_QDUCK0)
      return ((1u == bFM_CLK_GATING_CKEN1_QDUCK0) ? TRUE : FALSE);
#else
      break;
#endif       
    case ClkGateQprc1:
#if defined(bFM_CLK_GATING_CKEN1_QDUCK1)
      return ((1u == bFM_CLK_GATING_CKEN1_QDUCK1) ? TRUE : FALSE);
#else
      break;
#endif      
    case ClkGateQprc2:
#if defined(bFM_CLK_GATING_CKEN1_QDUCK2)
      return ((1u == bFM_CLK_GATING_CKEN1_QDUCK2) ? TRUE : FALSE);
#else
      break;
#endif       
    case ClkGateQprc3:
#if defined(bFM_CLK_GATING_CKEN1_QDUCK3)
      return ((1u == bFM_CLK_GATING_CKEN1_QDUCK3) ? TRUE : FALSE);
#else
      break;
#endif       
    case ClkGateMft0:
#if defined(bFM_CLK_GATING_CKEN1_MFTCK0)
      return ((1u == bFM_CLK_GATING_CKEN1_MFTCK0) ? TRUE : FALSE);
#else
      break;
#endif       
    case ClkGateMft1:
#if defined(bFM_CLK_GATING_CKEN1_MFTCK1)
      return ((1u == bFM_CLK_GATING_CKEN1_MFTCK1) ? TRUE : FALSE);
#else
      break;
#endif      
    case ClkGateMft2:
#if defined(bFM_CLK_GATING_CKEN1_MFTCK2)
      return ((1u == bFM_CLK_GATING_CKEN1_MFTCK2) ? TRUE : FALSE);
#else
      break;
#endif        
    case ClkGateMft3:
#if defined(bFM_CLK_GATING_CKEN1_MFTCK3)
      return ((1u == bFM_CLK_GATING_CKEN1_MFTCK3) ? TRUE : FALSE);
#else
      break;
#endif       
    case ClkGateBt0123:
#if defined(bFM_CLK_GATING_CKEN1_BTMCK0)
      return ((1u == bFM_CLK_GATING_CKEN1_BTMCK0) ? TRUE : FALSE);
#else
      break;
#endif        
    case ClkGateBt4567:
#if defined(bFM_CLK_GATING_CKEN1_BTMCK1)
      return ((1u == bFM_CLK_GATING_CKEN1_BTMCK1) ? TRUE : FALSE);
#else
      break;
#endif       
    case ClkGateBt891011:
#if defined(bFM_CLK_GATING_CKEN1_BTMCK2)
      return ((1u == bFM_CLK_GATING_CKEN1_BTMCK2) ? TRUE : FALSE);
#else
      break;
#endif      
    case ClkGateBt12131415:
#if defined(bFM_CLK_GATING_CKEN1_BTMCK3)
      return ((1u == bFM_CLK_GATING_CKEN1_BTMCK3) ? TRUE : FALSE);
#else
      break;
#endif       
    case ClkGateExtif:
#if defined(bFM_CLK_GATING_CKEN0_EXBCK)
      return ((1u == bFM_CLK_GATING_CKEN0_EXBCK) ? TRUE : FALSE);
#else
      break;  
#endif         
    case ClkGateUsb0:
#if defined(bFM_CLK_GATING_CKEN2_USBCK0 )
      return ((1u == bFM_CLK_GATING_CKEN2_USBCK0) ? TRUE : FALSE);
#else
      break;
#endif       
    case ClkGateUsb1:
#if defined(bFM_CLK_GATING_CKEN2_USBCK1 )
      return ((1u == bFM_CLK_GATING_CKEN2_USBCK1) ? TRUE : FALSE);
#else
      break;   
#endif       
    case ClkGateCan0:
#if defined(bFM_CLK_GATING_CKEN2_CANCK0 )
      return ((1u == bFM_CLK_GATING_CKEN2_CANCK0) ? TRUE : FALSE);
#else
      break;
#endif        
    case ClkGateCan1:
#if defined(bFM_CLK_GATING_CKEN2_CANCK1 )
      return ((1u == bFM_CLK_GATING_CKEN2_CANCK1) ? TRUE : FALSE);
#else    
      break;    
#endif      
    case ClkGateCan2:
#if defined(bFM_CLK_GATING_CKEN2_CANCK2 )
      return ((1u == bFM_CLK_GATING_CKEN2_CANCK2) ? TRUE : FALSE);
#else
      break;
#endif      
    case ClkGateSd:
#if defined(bFM_CLK_GATING_CKEN2_SDCCK )
      return (1u == bFM_CLK_GATING_CKEN2_SDCCK);
#else
      break; 
#endif        
    case ClkGateI2s0:
#if defined(bFM_CLK_GATING_CKEN2_I2SCK0 )
      return ((1u == bFM_CLK_GATING_CKEN2_I2SCK0) ? TRUE : FALSE);
#else
      break;
#endif      
    case ClkGateI2s1:
#if defined(bFM_CLK_GATING_CKEN2_I2SCK1 )
      return ((1u == bFM_CLK_GATING_CKEN2_I2SCK1) ? TRUE : FALSE);
#else
      break;    
#endif       
    case ClkGatePcrc:
#if defined(bFM_CLK_GATING_CKEN2_PCRCCK )
      return ((1u == bFM_CLK_GATING_CKEN2_PCRCCK) ? TRUE : FALSE);
#else
      break;
#endif        
    case ClkGateQspi:
#if defined(bFM_CLK_GATING_CKEN2_QSPICK )
      return ((1u == bFM_CLK_GATING_CKEN2_QSPICK) ? TRUE : FALSE);
#else
      break; 
#endif         
#if (PDL_MCU_CORE == PDL_FM0P_CORE)  
    case ClkGateCec0:
    case ClkGateCec1:
#if defined(bFM_CLK_GATING_CKEN2_CECCK )      
      return ((1u == bFM_CLK_GATING_CKEN2_CECCK) ? TRUE : FALSE);
#else
      break;
#endif      
#else
    case ClkGateCec0:
#if defined(bFM_CLK_GATING_CKEN2_CECCK0)      
      return ((1u == bFM_CLK_GATING_CKEN2_CECCK0) ? TRUE : FALSE);
#else
      break;
#endif          
    case ClkGateCec1:
#if defined(bFM_CLK_GATING_CKEN2_CECCK1)      
      return ((1u == bFM_CLK_GATING_CKEN2_CECCK1) ? TRUE : FALSE);
#else
      break;
#endif       
#endif
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)
    case ClkGateIcc0:
#if defined(bFM_CLK_GATING_CKEN2_ICCCK0) 
      return ((1u == bFM_CLK_GATING_CKEN2_ICCCK0) ? TRUE : FALSE);
#else
      break;
#endif      
    case ClkGateIcc1:
#if defined(bFM_CLK_GATING_CKEN2_ICCCK1) 
      return ((1u == bFM_CLK_GATING_CKEN2_ICCCK1) ? TRUE : FALSE);
#else
      break;
#endif        
    case ClkGateI2sl0:
#if defined(bFM_CLK_GATING_CKEN2_IISCCK0) 
      return ((1u == bFM_CLK_GATING_CKEN2_IISCCK0) ? TRUE : FALSE);
#else
      break;
#endif      
    case ClkGateI2sl1:
#if defined(bFM_CLK_GATING_CKEN2_IISCCK1) 
      return ((1u == bFM_CLK_GATING_CKEN2_IISCCK1) ? TRUE : FALSE);
#else      
      break;      
#endif
#endif      
    default:
      break;
  }
  
  return FALSE; // Peripheral not found -> always FALSE
} // Clk_PeripheralGetClockState

/**
 ******************************************************************************
 ** \brief Disables the clock gate of a peripheral
 **
 ** This function clears the corresponding bit in the CKENn register to enable
 ** the clock of a peripheral.
 **
 ** \param  enPeripheral           Enumerator of a peripheral, see
 **                                #en_clk_gate_peripheral_t for details
 **
 ** \retval Ok                     Peripheral clock disabled
 ** \retval ErrorInvalidParameter  Peripheral enumerator does not exist
 ******************************************************************************/
en_result_t Clk_PeripheralClockDisable(en_clk_gate_peripheral_t enPeripheral)
{
  switch (enPeripheral)
  {
    case ClkGateGpio:
#if defined(bFM_CLK_GATING_CKEN0_GIOCK)
      bFM_CLK_GATING_CKEN0_GIOCK = 0u;
#endif      
      break;     
    case ClkGateDma:
#if defined(bFM_CLK_GATING_CKEN0_DMACK)
      bFM_CLK_GATING_CKEN0_DMACK = 0u;
#endif      
      break;
    case ClkGateAdc0:
#if defined(bFM_CLK_GATING_CKEN0_ADCCK0)
      bFM_CLK_GATING_CKEN0_ADCCK0 = 0u;
#endif      
      break;
    case ClkGateAdc1:     
#if defined(bFM_CLK_GATING_CKEN0_ADCCK1)
      bFM_CLK_GATING_CKEN0_ADCCK1 = 0u;
#endif       
      break;
    case ClkGateAdc2:
#if defined(bFM_CLK_GATING_CKEN0_ADCCK2)
      bFM_CLK_GATING_CKEN0_ADCCK2 = 0u;
#endif      
      break;
    case ClkGateAdc3:
#if defined(bFM_CLK_GATING_CKEN0_ADCCK3)
      bFM_CLK_GATING_CKEN0_ADCCK3 = 0u;
#endif       
      break;
    case ClkGateMfs0:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK0)
      bFM_CLK_GATING_CKEN0_MFSCK0 = 0u;
#endif       
      break;
    case ClkGateMfs1:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK1)
      bFM_CLK_GATING_CKEN0_MFSCK1 = 0u;
#endif      
      break;
    case ClkGateMfs2:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK2)
      bFM_CLK_GATING_CKEN0_MFSCK2 = 0u;
#endif       
      break;
    case ClkGateMfs3:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK3)
      bFM_CLK_GATING_CKEN0_MFSCK3 = 0u;
#endif       
      break;
    case ClkGateMfs4:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK4)
      bFM_CLK_GATING_CKEN0_MFSCK4 = 0u;
#endif       
      break;
    case ClkGateMfs5:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK5)
      bFM_CLK_GATING_CKEN0_MFSCK5 = 0u;
#endif      
      break;
    case ClkGateMfs6:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK6)
      bFM_CLK_GATING_CKEN0_MFSCK6 = 0u;
#endif      
      break;
    case ClkGateMfs7:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK7)
      bFM_CLK_GATING_CKEN0_MFSCK7 = 0u;
#endif
      break;
    case ClkGateMfs8:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK8)
      bFM_CLK_GATING_CKEN0_MFSCK8 = 0u;
#endif      
      break;
    case ClkGateMfs9:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK9)
      bFM_CLK_GATING_CKEN0_MFSCK9 = 0u;
#endif       
      break;
    case ClkGateMfs10:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK10)
      bFM_CLK_GATING_CKEN0_MFSCK10 = 0u;
#endif       
      break;
    case ClkGateMfs11:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK11)
      bFM_CLK_GATING_CKEN0_MFSCK11 = 0u;
#endif      
      break;
    case ClkGateMfs12:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK12)
      bFM_CLK_GATING_CKEN0_MFSCK12 = 0u;
#endif       
      break;
    case ClkGateMfs13:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK13)
      bFM_CLK_GATING_CKEN0_MFSCK13 = 0u;
#endif       
      break;
    case ClkGateMfs14:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK14)
      bFM_CLK_GATING_CKEN0_MFSCK14 = 0u;
#endif      
      break;
    case ClkGateMfs15:
#if defined(bFM_CLK_GATING_CKEN0_MFSCK15)
      bFM_CLK_GATING_CKEN0_MFSCK15 = 0u;
#endif       
      break;
    case ClkGateQprc0:
#if defined(bFM_CLK_GATING_CKEN1_QDUCK0)
      bFM_CLK_GATING_CKEN1_QDUCK0 = 0u;
#endif       
      break;
    case ClkGateQprc1:
#if defined(bFM_CLK_GATING_CKEN1_QDUCK1)
      bFM_CLK_GATING_CKEN1_QDUCK1 = 0u;
#endif      
      break;
    case ClkGateQprc2:
#if defined(bFM_CLK_GATING_CKEN1_QDUCK2)
      bFM_CLK_GATING_CKEN1_QDUCK2 = 0u;
#endif       
      break;
    case ClkGateQprc3:
#if defined(bFM_CLK_GATING_CKEN1_QDUCK3)
      bFM_CLK_GATING_CKEN1_QDUCK3 = 0u;
#endif       
      break;
    case ClkGateMft0:
#if defined(bFM_CLK_GATING_CKEN1_MFTCK0)
      bFM_CLK_GATING_CKEN1_MFTCK0 = 0u;
#endif       
      break;
    case ClkGateMft1:
#if defined(bFM_CLK_GATING_CKEN1_MFTCK1)
      bFM_CLK_GATING_CKEN1_MFTCK1 = 0u;
#endif      
      break;
    case ClkGateMft2:
#if defined(bFM_CLK_GATING_CKEN1_MFTCK2)
      bFM_CLK_GATING_CKEN1_MFTCK2 = 0u;
#endif        
      break;
    case ClkGateMft3:
#if defined(bFM_CLK_GATING_CKEN1_MFTCK3)
      bFM_CLK_GATING_CKEN1_MFTCK3 = 0u;
#endif       
      break;
    case ClkGateBt0123:
#if defined(bFM_CLK_GATING_CKEN1_BTMCK0)
      bFM_CLK_GATING_CKEN1_BTMCK0 = 0u;
#endif        
      break;
    case ClkGateBt4567:
#if defined(bFM_CLK_GATING_CKEN1_BTMCK1)
      bFM_CLK_GATING_CKEN1_BTMCK1 = 0u;
#endif       
      break;
    case ClkGateBt891011:
#if defined(bFM_CLK_GATING_CKEN1_BTMCK2)
      bFM_CLK_GATING_CKEN1_BTMCK2 = 0u;
#endif      
      break;
    case ClkGateBt12131415:
#if defined(bFM_CLK_GATING_CKEN1_BTMCK3)
      bFM_CLK_GATING_CKEN1_BTMCK3 = 0u;
#endif       
      break;
    case ClkGateExtif:
#if defined(bFM_CLK_GATING_CKEN0_EXBCK)
      bFM_CLK_GATING_CKEN0_EXBCK = 0u;
#endif        
      break;      
    case ClkGateUsb0:
#if defined(bFM_CLK_GATING_CKEN2_USBCK0 )
      bFM_CLK_GATING_CKEN2_USBCK0  = 0u;
#endif       
      break;
    case ClkGateUsb1:
#if defined(bFM_CLK_GATING_CKEN2_USBCK1 )
      bFM_CLK_GATING_CKEN2_USBCK1  = 0u;
#endif       
      break;    
    case ClkGateCan0:
#if defined(bFM_CLK_GATING_CKEN2_CANCK0 )
      bFM_CLK_GATING_CKEN2_CANCK0  = 0u;
#endif        
      break;
    case ClkGateCan1:
#if defined(bFM_CLK_GATING_CKEN2_CANCK1 )
      bFM_CLK_GATING_CKEN2_CANCK1  = 0u;
#endif      
      break;      
    case ClkGateCan2:
#if defined(bFM_CLK_GATING_CKEN2_CANCK2 )
      bFM_CLK_GATING_CKEN2_CANCK2  = 0u;
#endif      
      break;
    case ClkGateSd:
#if defined(bFM_CLK_GATING_CKEN2_SDCCK )
      bFM_CLK_GATING_CKEN2_SDCCK  = 0u;
#endif        
      break; 
    case ClkGateI2s0:
#if defined(bFM_CLK_GATING_CKEN2_I2SCK0 )
      bFM_CLK_GATING_CKEN2_I2SCK0  = 0u;
#endif      
      break;
    case ClkGateI2s1:
#if defined(bFM_CLK_GATING_CKEN2_I2SCK1 )
      bFM_CLK_GATING_CKEN2_I2SCK1  = 0u;
#endif       
      break;      
    case ClkGatePcrc:
#if defined(bFM_CLK_GATING_CKEN2_PCRCCK )
      bFM_CLK_GATING_CKEN2_PCRCCK  = 0u;
#endif        
      break;
    case ClkGateQspi:
#if defined(bFM_CLK_GATING_CKEN2_QSPICK )
      bFM_CLK_GATING_CKEN2_QSPICK  = 0u;
#endif         
      break; 
#if (PDL_MCU_CORE == PDL_FM0P_CORE)  
    case ClkGateCec0:
    case ClkGateCec1:
#if defined(bFM_CLK_GATING_CKEN2_CECCK )      
      bFM_CLK_GATING_CKEN2_CECCK = 0u;
#endif      
      break;
#else
    case ClkGateCec0:
#if defined(bFM_CLK_GATING_CKEN2_CECCK0)      
        bFM_CLK_GATING_CKEN2_CECCK0 = 0u;
#endif          
        break;
    case ClkGateCec1:
#if defined(bFM_CLK_GATING_CKEN2_CECCK1)      
        bFM_CLK_GATING_CKEN2_CECCK1 = 0u;
#endif       
        break;    
#endif
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
    case ClkGateIcc0:
#if defined(bFM_CLK_GATING_CKEN2_ICCCK0) 
      bFM_CLK_GATING_CKEN2_ICCCK0 = 0u;
#endif      
      break;
    case ClkGateIcc1:
#if defined(bFM_CLK_GATING_CKEN2_ICCCK1) 
      bFM_CLK_GATING_CKEN2_ICCCK1 = 0u;
#endif        
      break;
    case ClkGateI2sl0:
#if defined(bFM_CLK_GATING_CKEN2_IISCCK0) 
      bFM_CLK_GATING_CKEN2_IISCCK0 = 0u;
#endif      
      break;
    case ClkGateI2sl1:
#if defined(bFM_CLK_GATING_CKEN2_IISCCK1) 
      bFM_CLK_GATING_CKEN2_IISCCK1 = 0u;
#endif      
      break;      
#endif        
    default:
      return ErrorInvalidParameter;
  }
  
  return Ok;
} // Clk_PeripheralClockDisable

/**
 ******************************************************************************
 ** \brief Enable the clock gate of all peripherals
 **
 ** This function enable the clock of all peripherals.
 **
 ** \retval Ok                     All peripheral clock enabled
 ******************************************************************************/
en_result_t Clk_PeripheralClockEnableAll(void) 
{
    FM_CLK_GATING->CKEN0 = 0xFFFFFFFFu;
    FM_CLK_GATING->CKEN1 = 0xFFFFFFFFu;
    FM_CLK_GATING->CKEN2 = 0xFFFFFFFFu;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable the clock gate of all peripherals
 **
 ** This function disables the clock of all peripherals.
 **
 ** \retval Ok                     All peripheral clock disabled
 ******************************************************************************/
en_result_t Clk_PeripheralClockDisableAll(void) 
{
    FM_CLK_GATING->CKEN0 = 0u;
    FM_CLK_GATING->CKEN1 = 0u;
    FM_CLK_GATING->CKEN2 = 0u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set reset bit a peripheral
 **
 ** This function sets the corresponding bit in the MRSTn register to set
 ** a peripheral in reset state.
 **
 ** \param  enPeripheral           Enumerator of a peripheral, see
 **                                #en_clk_reset_peripheral_t for details
 **
 ** \retval Ok                     Peripheral clock enabled
 ** \retval ErrorInvalidParameter  Peripheral enumerator does not exist
 ******************************************************************************/
en_result_t Clk_PeripheralSetReset(en_clk_reset_peripheral_t enPeripheral)
{
  switch (enPeripheral)
  {
    case ClkResetDma:
#if defined(bFM_CLK_GATING_MRST0_DMARST)      
      bFM_CLK_GATING_MRST0_DMARST = 1u;
#endif      
      break; 
    case ClkResetAdc0:
#if defined(bFM_CLK_GATING_MRST0_ADCRST0)           
      bFM_CLK_GATING_MRST0_ADCRST0 = 1u;
#endif      
      break;
    case ClkResetAdc1:
#if defined(bFM_CLK_GATING_MRST0_ADCRST1)      
      bFM_CLK_GATING_MRST0_ADCRST1 = 1u;
#endif      
      break;
    case ClkResetAdc2:
#if defined(bFM_CLK_GATING_MRST0_ADCRST2)      
      bFM_CLK_GATING_MRST0_ADCRST2 = 1u;
#endif      
      break;
    case ClkResetAdc3:
#if defined(bFM_CLK_GATING_MRST0_ADCRST3)      
      bFM_CLK_GATING_MRST0_ADCRST3 = 1u;
#endif      
      break;
    case ClkResetMfs0:
#if defined(bFM_CLK_GATING_MRST0_MFSRST0)      
      bFM_CLK_GATING_MRST0_MFSRST0 = 1u;
#endif      
      break;
    case ClkResetMfs1:
#if defined(bFM_CLK_GATING_MRST0_MFSRST1)       
      bFM_CLK_GATING_MRST0_MFSRST1 = 1u;
#endif      
      break;
    case ClkResetMfs2:
#if defined(bFM_CLK_GATING_MRST0_MFSRST2)       
      bFM_CLK_GATING_MRST0_MFSRST2 = 1u;
#endif      
      break;
    case ClkResetMfs3:
#if defined(bFM_CLK_GATING_MRST0_MFSRST3)      
      bFM_CLK_GATING_MRST0_MFSRST3 = 1u;
#endif      
      break;
    case ClkResetMfs4:
#if defined(bFM_CLK_GATING_MRST0_MFSRST4)      
      bFM_CLK_GATING_MRST0_MFSRST4 = 1u;
#endif  
      break;
    case ClkResetMfs5:
#if defined(bFM_CLK_GATING_MRST0_MFSRST5)      
      bFM_CLK_GATING_MRST0_MFSRST5 = 1u;
#endif 
      break;
    case ClkResetMfs6:
#if defined(bFM_CLK_GATING_MRST0_MFSRST6)      
      bFM_CLK_GATING_MRST0_MFSRST6 = 1u;
#endif
      break;
    case ClkResetMfs7:
#if defined(bFM_CLK_GATING_MRST0_MFSRST7)      
      bFM_CLK_GATING_MRST0_MFSRST7 = 1u;
#endif
      break;
    case ClkResetMfs8:
#if defined(bFM_CLK_GATING_MRST0_MFSRST8)      
      bFM_CLK_GATING_MRST0_MFSRST8 = 1u;
#endif
      break;
    case ClkResetMfs9:
#if defined(bFM_CLK_GATING_MRST0_MFSRST9)      
      bFM_CLK_GATING_MRST0_MFSRST9 = 1u;
#endif
      break;
    case ClkResetMfs10:
#if defined(bFM_CLK_GATING_MRST0_MFSRST10)      
      bFM_CLK_GATING_MRST0_MFSRST10 = 1u;
#endif
      break;
    case ClkResetMfs11:
#if defined(bFM_CLK_GATING_MRST0_MFSRST11)      
      bFM_CLK_GATING_MRST0_MFSRST11 = 1u;
#endif
      break;
    case ClkResetMfs12:
#if defined(bFM_CLK_GATING_MRST0_MFSRST12)      
      bFM_CLK_GATING_MRST0_MFSRST12 = 1u;
#endif
      break;
    case ClkResetMfs13:
#if defined(bFM_CLK_GATING_MRST0_MFSRST13)      
      bFM_CLK_GATING_MRST0_MFSRST13 = 1u;
#endif
      break;
    case ClkResetMfs14:
#if defined(bFM_CLK_GATING_MRST0_MFSRST14)      
      bFM_CLK_GATING_MRST0_MFSRST14 = 1u;
#endif
      break;
    case ClkResetMfs15:
#if defined(bFM_CLK_GATING_MRST0_MFSRST15)      
      bFM_CLK_GATING_MRST0_MFSRST15 = 1u;
#endif
      break;
    case ClkResetQprc0:
#if defined(bFM_CLK_GATING_MRST1_QDURST0)      
      bFM_CLK_GATING_MRST1_QDURST0 = 1u;
#endif      
      break;
    case ClkResetQprc1:
#if defined(bFM_CLK_GATING_MRST1_QDURST1)      
      bFM_CLK_GATING_MRST1_QDURST1 = 1u;
#endif 
      break;
    case ClkResetQprc2:
#if defined(bFM_CLK_GATING_MRST1_QDURST2)      
      bFM_CLK_GATING_MRST1_QDURST2 = 1u;
#endif
      break;
    case ClkResetQprc3:
#if defined(bFM_CLK_GATING_MRST1_QDURST3)      
      bFM_CLK_GATING_MRST1_QDURST3 = 1u;
#endif
      break;
    case ClkResetMft0:
#if defined(bFM_CLK_GATING_MRST1_MFTRST0)      
      bFM_CLK_GATING_MRST1_MFTRST0 = 1u;
#endif      
      break;
    case ClkResetMft1:
#if defined(bFM_CLK_GATING_MRST1_MFTRST1)      
      bFM_CLK_GATING_MRST1_MFTRST1 = 1u;
#endif
      break;
    case ClkResetMft2:
#if defined(bFM_CLK_GATING_MRST1_MFTRST2)      
      bFM_CLK_GATING_MRST1_MFTRST2 = 1u;
#endif
      break;
    case ClkResetMft3:
#if defined(bFM_CLK_GATING_MRST1_MFTRST3)      
      bFM_CLK_GATING_MRST1_MFTRST3 = 1u;
#endif
      break;
    case ClkResetBt0123:
#if defined(bFM_CLK_GATING_MRST1_BTMRST0)      
      bFM_CLK_GATING_MRST1_BTMRST0 = 1u;
#endif      
      break;
    case ClkResetBt4567:
#if defined(bFM_CLK_GATING_MRST1_BTMRST1)      
      bFM_CLK_GATING_MRST1_BTMRST1 = 1u;
#endif 
      break;
    case ClkResetBt891011:
#if defined(bFM_CLK_GATING_MRST1_BTMRST2)      
      bFM_CLK_GATING_MRST1_BTMRST2 = 1u;
#endif 
      break;
    case ClkResetBt12131415:
#if defined(bFM_CLK_GATING_MRST1_BTMRST3)      
      bFM_CLK_GATING_MRST1_BTMRST3 = 1u;
#endif 
      break;
    case ClkResetExtif:
#if defined(bFM_CLK_GATING_MRST0_EXBRST)      
      FM_CLK_GATING->MRST0_f.EXBRST = 1u;  
#endif      
      break;    
    case ClkResetUsb0:
#if defined(bFM_CLK_GATING_MRST2_USBRST0)      
      bFM_CLK_GATING_MRST2_USBRST0 = 1u;
#endif      
      break;
    case ClkResetUsb1:
#if defined(bFM_CLK_GATING_MRST2_USBRST1)      
      bFM_CLK_GATING_MRST2_USBRST1 = 1u;
#endif 
      break;    
    case ClkResetSd:
#if defined(bFM_CLK_GATING_MRST2_SDCRST)      
      bFM_CLK_GATING_MRST2_SDCRST = 1u;
#endif      
      break;
    case ClkResetI2s0:
#if defined(bFM_CLK_GATING_MRST2_I2SRST0)      
      bFM_CLK_GATING_MRST2_I2SRST0 = 1u;
#endif      
      break;
    case ClkResetI2s1:
#if defined(bFM_CLK_GATING_MRST2_I2SRST1)      
      bFM_CLK_GATING_MRST2_I2SRST1 = 1u;
#endif 
      break;
    case ClkResetPcrc:
#if defined(bFM_CLK_GATING_MRST2_PCRCRST)        
      bFM_CLK_GATING_MRST2_PCRCRST = 1u;
#endif      
      break;
    case ClkResetQspi:
#if defined(bFM_CLK_GATING_MRST2_HSSPIRST)      
      bFM_CLK_GATING_MRST2_HSSPIRST = 1u;
#endif      
      break;
    case ClkResetCan0:
#if defined(bFM_CLK_GATING_MRST2_CANRST0)        
      bFM_CLK_GATING_MRST2_CANRST0 = 1u;
#endif      
      break;
    case ClkResetCan1:
#if defined(bFM_CLK_GATING_MRST2_CANRST1)        
      bFM_CLK_GATING_MRST2_CANRST1 = 1u;
#endif 
      break;
    case ClkResetCan2:
#if defined(bFM_CLK_GATING_MRST2_CANRST2)        
      bFM_CLK_GATING_MRST2_CANRST2 = 1u;
#endif 
      break;
#if defined(bFM_CLK_GATING_MRST2_CECRST)      
    case ClkResetCec0:
    case ClkResetCec1:
      bFM_CLK_GATING_MRST2_CECRST = 1u;
      break;
#else
    case ClkResetCec0:
#if defined(bFM_CLK_GATING_MRST2_CECRST0)      
      bFM_CLK_GATING_MRST2_CECRST0 = 1u;
#endif      
      break;
    case ClkResetCec1:
#if defined(bFM_CLK_GATING_MRST2_CECRST1)          
      bFM_CLK_GATING_MRST2_CECRST1 = 1u;
#endif      
      break;  
#endif      
    case ClkResetIcc0:
#if defined(bFM_CLK_GATING_MRST2_ICCRST0)      
      bFM_CLK_GATING_MRST2_ICCRST0 = 1u;
#endif      
      break;
    case ClkResetIcc1:
#if defined(bFM_CLK_GATING_MRST2_ICCRST1)       
      bFM_CLK_GATING_MRST2_ICCRST1 = 1u;
#endif      
      break;
    case ClkResetI2sl0:
#if defined(bFM_CLK_GATING_MRST2_IISCRST0)      
      bFM_CLK_GATING_MRST2_IISCRST0 = 1u;
#endif      
      break;
    case ClkResetI2sl1:
#if defined(bFM_CLK_GATING_MRST2_IISCRST1)      
      FM_CLK_GATING->MRST2_f.IISCRST1 = 1u;
#endif      
      break;         
    default:
      return ErrorInvalidParameter;
  }
  
  return Ok;
} // Clk_PeripheralSetReset

/**
 ******************************************************************************
 ** \brief Clear reset bit a peripheral
 **
 ** This function clears the corresponding bit in the MRSTn register to release
 ** a peripheral from reset state.
 **
 ** \param  enPeripheral           Enumerator of a peripheral, see
 **                                #en_clk_reset_peripheral_t for details
 **
 ** \retval Ok                     Peripheral clock enabled
 ** \retval ErrorInvalidParameter  Peripheral enumerator does not exist
 ******************************************************************************/
en_result_t Clk_PeripheralClearReset(en_clk_reset_peripheral_t enPeripheral)
{
  switch (enPeripheral)
  {
    case ClkResetDma:
#if defined(bFM_CLK_GATING_MRST0_DMARST)      
      bFM_CLK_GATING_MRST0_DMARST = 0u;
#endif      
      break; 
    case ClkResetAdc0:
#if defined(bFM_CLK_GATING_MRST0_ADCRST0)           
      bFM_CLK_GATING_MRST0_ADCRST0 = 0u;
#endif      
      break;
    case ClkResetAdc1:
#if defined(bFM_CLK_GATING_MRST0_ADCRST1)      
      bFM_CLK_GATING_MRST0_ADCRST1 = 0u;
#endif      
      break;
    case ClkResetAdc2:
#if defined(bFM_CLK_GATING_MRST0_ADCRST2)      
      bFM_CLK_GATING_MRST0_ADCRST2 = 0u;
#endif      
      break;
    case ClkResetAdc3:
#if defined(bFM_CLK_GATING_MRST0_ADCRST3)      
      bFM_CLK_GATING_MRST0_ADCRST3 = 0u;
#endif      
      break;
    case ClkResetMfs0:
#if defined(bFM_CLK_GATING_MRST0_MFSRST0)      
      bFM_CLK_GATING_MRST0_MFSRST0 = 0u;
#endif      
      break;
    case ClkResetMfs1:
#if defined(bFM_CLK_GATING_MRST0_MFSRST1)       
      bFM_CLK_GATING_MRST0_MFSRST1 = 0u;
#endif      
      break;
    case ClkResetMfs2:
#if defined(bFM_CLK_GATING_MRST0_MFSRST2)       
      bFM_CLK_GATING_MRST0_MFSRST2 = 0u;
#endif      
      break;
    case ClkResetMfs3:
#if defined(bFM_CLK_GATING_MRST0_MFSRST3)      
      bFM_CLK_GATING_MRST0_MFSRST3 = 0u;
#endif      
      break;
    case ClkResetMfs4:
#if defined(bFM_CLK_GATING_MRST0_MFSRST4)      
      bFM_CLK_GATING_MRST0_MFSRST4 = 0u;
#endif  
      break;
    case ClkResetMfs5:
#if defined(bFM_CLK_GATING_MRST0_MFSRST5)      
      bFM_CLK_GATING_MRST0_MFSRST5 = 0u;
#endif 
      break;
    case ClkResetMfs6:
#if defined(bFM_CLK_GATING_MRST0_MFSRST6)      
      bFM_CLK_GATING_MRST0_MFSRST6 = 0u;
#endif
      break;
    case ClkResetMfs7:
#if defined(bFM_CLK_GATING_MRST0_MFSRST7)      
      bFM_CLK_GATING_MRST0_MFSRST7 = 0u;
#endif
      break;
    case ClkResetMfs8:
#if defined(bFM_CLK_GATING_MRST0_MFSRST8)      
      bFM_CLK_GATING_MRST0_MFSRST8 = 0u;
#endif
      break;
    case ClkResetMfs9:
#if defined(bFM_CLK_GATING_MRST0_MFSRST9)      
      bFM_CLK_GATING_MRST0_MFSRST9 = 0u;
#endif
      break;
    case ClkResetMfs10:
#if defined(bFM_CLK_GATING_MRST0_MFSRST10)      
      bFM_CLK_GATING_MRST0_MFSRST10 = 0u;
#endif
      break;
    case ClkResetMfs11:
#if defined(bFM_CLK_GATING_MRST0_MFSRST11)      
      bFM_CLK_GATING_MRST0_MFSRST11 = 0u;
#endif
      break;
    case ClkResetMfs12:
#if defined(bFM_CLK_GATING_MRST0_MFSRST12)      
      bFM_CLK_GATING_MRST0_MFSRST12 = 0u;
#endif
      break;
    case ClkResetMfs13:
#if defined(bFM_CLK_GATING_MRST0_MFSRST13)      
      bFM_CLK_GATING_MRST0_MFSRST13 = 0u;
#endif
      break;
    case ClkResetMfs14:
#if defined(bFM_CLK_GATING_MRST0_MFSRST14)      
      bFM_CLK_GATING_MRST0_MFSRST14 = 0u;
#endif
      break;
    case ClkResetMfs15:
#if defined(bFM_CLK_GATING_MRST0_MFSRST15)      
      bFM_CLK_GATING_MRST0_MFSRST15 = 0u;
#endif
      break;
    case ClkResetQprc0:
#if defined(bFM_CLK_GATING_MRST1_QDURST0)      
      bFM_CLK_GATING_MRST1_QDURST0 = 0u;
#endif      
      break;
    case ClkResetQprc1:
#if defined(bFM_CLK_GATING_MRST1_QDURST1)      
      bFM_CLK_GATING_MRST1_QDURST1 = 0u;
#endif 
      break;
    case ClkResetQprc2:
#if defined(bFM_CLK_GATING_MRST1_QDURST2)      
      bFM_CLK_GATING_MRST1_QDURST2 = 0u;
#endif
      break;
    case ClkResetQprc3:
#if defined(bFM_CLK_GATING_MRST1_QDURST3)      
      bFM_CLK_GATING_MRST1_QDURST3 = 0u;
#endif
      break;
    case ClkResetMft0:
#if defined(bFM_CLK_GATING_MRST1_MFTRST0)      
      bFM_CLK_GATING_MRST1_MFTRST0 = 0u;
#endif      
      break;
    case ClkResetMft1:
#if defined(bFM_CLK_GATING_MRST1_MFTRST1)      
      bFM_CLK_GATING_MRST1_MFTRST1 = 0u;
#endif
      break;
    case ClkResetMft2:
#if defined(bFM_CLK_GATING_MRST1_MFTRST2)      
      bFM_CLK_GATING_MRST1_MFTRST2 = 0u;
#endif
      break;
    case ClkResetMft3:
#if defined(bFM_CLK_GATING_MRST1_MFTRST3)      
      bFM_CLK_GATING_MRST1_MFTRST3 = 0u;
#endif
      break;
    case ClkResetBt0123:
#if defined(bFM_CLK_GATING_MRST1_BTMRST0)      
      bFM_CLK_GATING_MRST1_BTMRST0 = 0u;
#endif      
      break;
    case ClkResetBt4567:
#if defined(bFM_CLK_GATING_MRST1_BTMRST1)      
      bFM_CLK_GATING_MRST1_BTMRST1 = 0u;
#endif 
      break;
    case ClkResetBt891011:
#if defined(bFM_CLK_GATING_MRST1_BTMRST2)      
      bFM_CLK_GATING_MRST1_BTMRST2 = 0u;
#endif 
      break;
    case ClkResetBt12131415:
#if defined(bFM_CLK_GATING_MRST1_BTMRST3)      
      bFM_CLK_GATING_MRST1_BTMRST3 = 0u;
#endif 
      break;
    case ClkResetExtif:
#if defined(bFM_CLK_GATING_MRST0_EXBRST)      
      FM_CLK_GATING->MRST0_f.EXBRST = 0u;  
#endif      
      break;    
    case ClkResetUsb0:
#if defined(bFM_CLK_GATING_MRST2_USBRST0)      
      bFM_CLK_GATING_MRST2_USBRST0 = 0u;
#endif      
      break;
    case ClkResetUsb1:
#if defined(bFM_CLK_GATING_MRST2_USBRST1)      
      bFM_CLK_GATING_MRST2_USBRST1 = 0u;
#endif 
      break;    
    case ClkResetSd:
#if defined(bFM_CLK_GATING_MRST2_SDCRST)      
      bFM_CLK_GATING_MRST2_SDCRST = 0u;
#endif      
      break;
    case ClkResetI2s0:
#if defined(bFM_CLK_GATING_MRST2_I2SRST0)      
      bFM_CLK_GATING_MRST2_I2SRST0 = 0u;
#endif      
      break;
    case ClkResetI2s1:
#if defined(bFM_CLK_GATING_MRST2_I2SRST1)      
      bFM_CLK_GATING_MRST2_I2SRST1 = 0u;
#endif 
      break;
    case ClkResetPcrc:
#if defined(bFM_CLK_GATING_MRST2_PCRCRST)        
      bFM_CLK_GATING_MRST2_PCRCRST = 0u;
#endif      
      break;
    case ClkResetQspi:
#if defined(bFM_CLK_GATING_MRST2_HSSPIRST)      
      bFM_CLK_GATING_MRST2_HSSPIRST = 0u;
#endif      
      break;
    case ClkResetCan0:
#if defined(bFM_CLK_GATING_MRST2_CANRST0)        
      bFM_CLK_GATING_MRST2_CANRST0 = 0u;
#endif      
      break;
    case ClkResetCan1:
#if defined(bFM_CLK_GATING_MRST2_CANRST1)        
      bFM_CLK_GATING_MRST2_CANRST1 = 0u;
#endif 
      break;
    case ClkResetCan2:
#if defined(bFM_CLK_GATING_MRST2_CANRST2)        
      bFM_CLK_GATING_MRST2_CANRST2 = 0u;
#endif 
      break;
#if defined(bFM_CLK_GATING_MRST2_CECRST)      
    case ClkResetCec0:
    case ClkResetCec1:
      bFM_CLK_GATING_MRST2_CECRST = 0u;
      break;
#else
    case ClkResetCec0:
#if defined(bFM_CLK_GATING_MRST2_CECRST0)      
      bFM_CLK_GATING_MRST2_CECRST0 = 0u;
#endif      
      break;
    case ClkResetCec1:
#if defined(bFM_CLK_GATING_MRST2_CECRST1)          
      bFM_CLK_GATING_MRST2_CECRST1 = 0u;
#endif      
      break;  
#endif      
    case ClkResetIcc0:
#if defined(bFM_CLK_GATING_MRST2_ICCRST0)      
      bFM_CLK_GATING_MRST2_ICCRST0 = 0u;
#endif      
      break;
    case ClkResetIcc1:
#if defined(bFM_CLK_GATING_MRST2_ICCRST1)       
      bFM_CLK_GATING_MRST2_ICCRST1 = 0u;
#endif      
      break;
    case ClkResetI2sl0:
#if defined(bFM_CLK_GATING_MRST2_IISCRST0)      
      bFM_CLK_GATING_MRST2_IISCRST0 = 0u;
#endif      
      break;
    case ClkResetI2sl1:
#if defined(bFM_CLK_GATING_MRST2_IISCRST1)      
      FM_CLK_GATING->MRST2_f.IISCRST1 = 0u;
#endif      
      break;         
    default:
      return ErrorInvalidParameter;
  }
  
  return Ok;
} // Clk_PeripheralClearReset

#endif

#endif // #if (defined(PDL_PERIPHERAL_ENABLE_CLK))

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
