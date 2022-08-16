/*******************************************************************************
* \file             i2s.c
* 
* \version          1.20
*
* \brief            This file provides the source code to the API for the I2S 
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
#include "i2s/i2s.h"

#if (defined(PDL_PERIPHERAL_I2S_ACTIVE))

/******************************************************************************/
/* Local pre-processor symbols/macros ('define')                              */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/
/// Look-up table for all enabled I2S instances and their internal data
stc_i2s_instance_data_t m_astcI2sInstanceDataLut[I2sInstanceIndexMax] =
{
#if (PDL_PERIPHERAL_ENABLE_I2S0 == PDL_ON)
    {
        &I2S0,   // pstcInstance 
        {0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u}       // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_I2S1 == PDL_ON)
    {
        &I2S1,   // pstcInstance 
        {0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u}       // stcInternData (not initialized yet)
    }
#endif    
};

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 ******************************************************************************
 ** \brief Return the internal data for a certain I2S instance.
 **
 ** \param pstcI2s Pointer to I2S instance
 **
 ** \return Pointer to internal data or NULL if instance is not enabled (or not known)
 **
 ******************************************************************************/
static stc_i2s_intern_data_t* I2sGetInternDataPtr(volatile stc_i2sn_t* pstcI2s) 
{
    uint8_t u8Instance;
   
    for (u8Instance = 0u; u8Instance < I2S_INSTANCE_COUNT; u8Instance++)
    {
        if (pstcI2s == m_astcI2sInstanceDataLut[u8Instance].pstcInstance)
        {
            return &m_astcI2sInstanceDataLut[u8Instance].stcInternData;
        }
    }

    return NULL;
}

#if (PDL_INTERRUPT_ENABLE_I2S0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_I2S1 == PDL_ON)      
/**
 ******************************************************************************
 ** \brief I2S instance interrupt service routine
 **
 ** I2S instance interrupt service routine, clear interrupt cause and
 **
 ** \param  pstcI2s              Pointer to I2S instance
 ** \param  pstcI2sInternData    Pointer to I2S intern data
 **        
 ******************************************************************************/
void I2sIrqHandler( volatile stc_i2sn_t*   pstcI2s,
                    stc_i2s_intern_data_t* pstcI2sInternData)
{
    if (1u == pstcI2s->STATUS_f.TBERR)   // DMA transmit channel block size large than transmit FIFO threshold value of I2S
    {
        pstcI2s->STATUS_f.TBERR = 0u;
      
        if (pstcI2sInternData->pfnTxBlockSizeErrIrqCb != NULL)
        {
            pstcI2sInternData->pfnTxBlockSizeErrIrqCb();
        }
    }

    if (1u == pstcI2s->STATUS_f.RBERR)   // DMA receive channel block size large than receive FIFO threshold value of I2S
    {
        pstcI2s->STATUS_f.RBERR = 0u;
      
        if (pstcI2sInternData->pfnRxBlockSizeErrIrqCb != NULL)
        {
            pstcI2sInternData->pfnRxBlockSizeErrIrqCb();
        }
    }

    if (1u == pstcI2s->STATUS_f.FERR)   // Frame error 
    {
        pstcI2s->STATUS_f.FERR = 0u;
      
        if (pstcI2sInternData->pfnFrameErrIrqCb != NULL)
        {
            pstcI2sInternData->pfnFrameErrIrqCb();
        }
        
    }

    if (1u == pstcI2s->STATUS_f.TXUDR1)   // TX FIFO underflow at the frame start 
    {
        pstcI2s->STATUS_f.TXUDR1 = 0u;
        
        if (pstcI2sInternData->pfnTxFifoUnderflow1IrqCb != NULL)
        {
            pstcI2sInternData->pfnTxFifoUnderflow1IrqCb();
        }
    }

    if (1u == pstcI2s->STATUS_f.TXUDR0)   // TX FIFO underflow during frame transmission 
    {
        pstcI2s->STATUS_f.TXUDR0 = 0u;
      
        if (pstcI2sInternData->pfnTxFifoUnderflow0IrqCb != NULL)
        {
            pstcI2sInternData->pfnTxFifoUnderflow0IrqCb();
        }
    }
    
    if (1u == pstcI2s->STATUS_f.TXOVR)   // TX FIFO overflow 
    {
        pstcI2s->STATUS_f.TXOVR = 0u;
      
        if (pstcI2sInternData->pfnTxFifoOverflowIrqCb != NULL)
        {
            pstcI2sInternData->pfnTxFifoOverflowIrqCb();
        }
    }
    
    if (1u == pstcI2s->STATUS_f.RXUDR)   // RX FIFO underflow 
    {
        pstcI2s->STATUS_f.RXUDR = 0u;
      
        if (pstcI2sInternData->pfnRxFifoUnderflowIrqCb != NULL)
        {
            pstcI2sInternData->pfnRxFifoUnderflowIrqCb();
        }
    }
    
    if (1u == pstcI2s->STATUS_f.RXOVR)   // RX FIFO underflow 
    {
        pstcI2s->STATUS_f.RXOVR = 0u;
      
        if (pstcI2sInternData->pfnRxFifoOverflowIrqCb != NULL)
        {
            pstcI2sInternData->pfnRxFifoOverflowIrqCb();
        }
    }
    
    if (1u == pstcI2s->STATUS_f.EOPI)   // RX FIFO idle  
    {
        pstcI2s->STATUS_f.EOPI = 0u;
      
        if (pstcI2sInternData->pfnRxFifoIdleDetectIrqCb != NULL)
        {
            pstcI2sInternData->pfnRxFifoIdleDetectIrqCb();
        }
    }
    
    if (1u == pstcI2s->STATUS_f.RXFI)   // RX FIFO count match with RX FIFO threshold value  
    {
        if (pstcI2sInternData->pfnRxFifoIrqCb != NULL)
        {
            pstcI2sInternData->pfnRxFifoIrqCb();
        }
    }
    
    if (1u == pstcI2s->STATUS_f.TXFI)   // TX FIFO empty slot match with TX FIFO threshold value    
    {
        if (pstcI2sInternData->pfnTxFifoIrqCb != NULL)
        {
            pstcI2sInternData->pfnTxFifoIrqCb();
        }
    }
    
    return;
} // I2sIrqHandler

/**
 ******************************************************************************
 ** \brief Enable I2S interrupts
 **
 ** \param [in] pstcI2s    Pointer to I2S instance   
 ** \param [in] enIrqSel   I2S interrupt selection
 ** \arg I2sTxFifoUnderflow0Irq    TX FIFO underflow interrupt 0
 ** \arg I2sTxFifoUnderflow1Irq    TX FIFO underflow interrupt 1
 ** \arg I2sTxBlockSizeErrIrq      DMA TX channel block error interrupt
 ** \arg I2sFrameErrIrq            Frame error interrupt
 ** \arg I2sTxFifoOverflowIrq      TX FIFO overflow interrupt
 ** \arg I2sTxFifoIrq              TX FIFO interrupt
 ** \arg I2sRxBlockSizeErrIrq      DMA RX channel block error interrupt
 ** \arg I2sRxFifoIdleDetectIrq    RX FIFO idle interrupt 
 ** \arg I2sRxFifoUnderflowIrq     RX FIFO underflow interrupt
 ** \arg I2sRxFifoOverflowIrq      RX FIFO overflow interrupt
 ** \arg I2sRxFifoIrq              RX FIFO interrupt
 ** 
 ** \retval Ok                    Interrupts has been enabled
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcI2s == NULL  
 ** 
 ******************************************************************************/
en_result_t I2s_EnableIrq(volatile stc_i2sn_t* pstcI2s, 
                          en_i2s_irq_sel_t enIrqSel)
{
    stc_i2s_intern_data_t* pstcI2sInternData;

    /* Check for valid pointer and get pointer to internal data struct ... */
    pstcI2sInternData = I2sGetInternDataPtr(pstcI2s);
  
    if (NULL == pstcI2sInternData)
    {
        return (ErrorInvalidParameter);
    }
    
    switch (enIrqSel)
    {
        case I2sTxFifoUnderflow0Irq:
            pstcI2s->INTCNT_f.TXUD0M = 0u;
            break;
        case I2sTxFifoUnderflow1Irq:
            pstcI2s->INTCNT_f.TXUD1M = 0u;
            break;
        case I2sTxBlockSizeErrIrq:
            pstcI2s->INTCNT_f.TBERM = 0u;
            break;
        case I2sFrameErrIrq:
            pstcI2s->INTCNT_f.FERRM = 0u;
            break;
        case I2sTxFifoOverflowIrq:
            pstcI2s->INTCNT_f.TXOVM = 0u;
            break;
        case I2sTxFifoIrq:
             pstcI2s->INTCNT_f.TXFIM = 0u;
            break;  
        case I2sRxBlockSizeErrIrq:
             pstcI2s->INTCNT_f.RBERM = 0u;
            break; 
        case I2sRxFifoIdleDetectIrq:
             pstcI2s->INTCNT_f.EOPM = 0u;
            break;
        case I2sRxFifoUnderflowIrq:
             pstcI2s->INTCNT_f.RXUDM = 0u;
            break;
        case I2sRxFifoOverflowIrq:
             pstcI2s->INTCNT_f.RXOVM = 0u;
            break;
        case I2sRxFifoIrq:
             pstcI2s->INTCNT_f.RXFIM = 0u;
            break;            
        default:
            return (ErrorInvalidParameter);
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable I2S interrupts
 **
 ** \param [in] pstcI2s    Pointer to I2S instance   
 ** \param [in] enIrqSel   I2S interrupt selection
 ** \arg I2sTxFifoUnderflow0Irq    TX FIFO underflow interrupt 0
 ** \arg I2sTxFifoUnderflow1Irq    TX FIFO underflow interrupt 1
 ** \arg I2sTxBlockSizeErrIrq      DMA TX channel block error interrupt
 ** \arg I2sFrameErrIrq            Frame error interrupt
 ** \arg I2sTxFifoOverflowIrq      TX FIFO overflow interrupt
 ** \arg I2sTxFifoIrq              TX FIFO interrupt
 ** \arg I2sRxBlockSizeErrIrq      DMA RX channel block error interrupt
 ** \arg I2sRxFifoIdleDetectIrq    RX FIFO idle interrupt 
 ** \arg I2sRxFifoUnderflowIrq     RX FIFO underflow interrupt
 ** \arg I2sRxFifoOverflowIrq      RX FIFO overflow interrupt
 ** \arg I2sRxFifoIrq              RX FIFO interrupt
 ** 
 ** \retval Ok                    Interrupts has been disabled
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcI2s == NULL  
 ** 
 ******************************************************************************/
en_result_t I2s_DisableIrq(volatile stc_i2sn_t* pstcI2s, 
                           en_i2s_irq_sel_t enIrqSel)
{
    stc_i2s_intern_data_t* pstcI2sInternData;

    /* Check for valid pointer and get pointer to internal data struct ... */
    pstcI2sInternData = I2sGetInternDataPtr(pstcI2s);
  
    if (NULL == pstcI2sInternData)
    {
        return (ErrorInvalidParameter);
    }
    
    switch (enIrqSel)
    {
        case I2sTxFifoUnderflow0Irq:
            pstcI2s->INTCNT_f.TXUD0M = 1u;
            break;
        case I2sTxFifoUnderflow1Irq:
            pstcI2s->INTCNT_f.TXUD1M = 1u;
            break;
        case I2sTxBlockSizeErrIrq:
            pstcI2s->INTCNT_f.TBERM = 1u;
            break;
        case I2sFrameErrIrq:
            pstcI2s->INTCNT_f.FERRM = 1u;
            break;
        case I2sTxFifoOverflowIrq:
            pstcI2s->INTCNT_f.TXOVM = 1u;
            break;
        case I2sTxFifoIrq:
             pstcI2s->INTCNT_f.TXFIM = 1u;
            break;  
        case I2sRxBlockSizeErrIrq:
             pstcI2s->INTCNT_f.RBERM = 1u;
            break; 
        case I2sRxFifoIdleDetectIrq:
             pstcI2s->INTCNT_f.EOPM = 1u;
            break;
        case I2sRxFifoUnderflowIrq:
             pstcI2s->INTCNT_f.RXUDM = 1u;
            break;
        case I2sRxFifoOverflowIrq:
             pstcI2s->INTCNT_f.RXOVM = 1u;
            break;
        case I2sRxFifoIrq:
             pstcI2s->INTCNT_f.RXFIM = 1u;
            break;            
        default:
            return (ErrorInvalidParameter);
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set NVIC Interrupt depending on I2S instance
 **
 ** \param pstcI2s Pointer to I2S instance
 **
 ******************************************************************************/
static void I2sInitNvic(volatile stc_i2sn_t* pstcI2s)
{
    NVIC_ClearPendingIRQ(PCRC_I2S0_1_IRQn);
    NVIC_EnableIRQ(PCRC_I2S0_1_IRQn);
    NVIC_SetPriority(PCRC_I2S0_1_IRQn, PDL_IRQ_LEVEL_I2S_PCRC);   
} // I2s_InitNvic

/**
 ******************************************************************************
 ** \brief Clear NVIC Interrupt depending on I2S instance
 **
 ** \param pstcI2s Pointer to I2S instance
 **
 ******************************************************************************/
static void I2sDeInitNvic(volatile stc_i2sn_t* pstcI2s)
{
    NVIC_ClearPendingIRQ(PCRC_I2S0_1_IRQn);
    NVIC_DisableIRQ(PCRC_I2S0_1_IRQn);
    NVIC_SetPriority(PCRC_I2S0_1_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);   
} // I2sDeInitNvic
#endif // #if (PDL_INTERRUPT_ENABLE_I2S0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_I2S1 == PDL_ON)

/**
 *****************************************************************************
 ** \brief This function initialises the I2S PLL clock according to the given
 ** parameters.
 **
 ** \attention The clock parameters are only valid for 19.2 MHz main clock
 **
 ** To reset and disable the I2S clock module the function I2s_DeInitClk()
 ** has to be used.
 **
 ** \param [in] pstcConfig      Pointer to I2S clock configuration parameters
 **
 ** \return - Ok                      on successful clock init
 **         - ErrorInvalidParameter   wrong enumerator used
 **         - ErrorTimeout            Clock off or PLL wait too long
 *****************************************************************************/
en_result_t I2s_InitClk(stc_i2s_clk_config_t* pstcConfig)
{
    uint32_t u32TimeOut = SystemCoreClock/1000;

    // Clock off
    FM_I2SPRE->ICCR_f.I2SEN = 0u;

    // Wait for clock off
    while((0u != u32TimeOut) && (1u == FM_I2SPRE->ICCR_f.I2SEN))
    {
        u32TimeOut--;
    }

    // Too long waited?
    if (0u == u32TimeOut)
    {
        return ErrorTimeout;
    }

    // Switch off PLL
    FM_I2SPRE->IPCR1_f.IPLLEN = 0u;

    // Set PLL wait time
    switch (pstcConfig->enI2sPllWaitTime)
    {
        case I2sPllWait26us:      // 26 us
            FM_I2SPRE->IPCR2_f.IPOWT = 0u;
            break;
        case I2sPllWait53us:      // 53 us
            FM_I2SPRE->IPCR2_f.IPOWT = 1u;
            break;
        case I2sPllWait106us:     // 106 us
            FM_I2SPRE->IPCR2_f.IPOWT = 2u;
            break;
        case I2sPllWait213us:     // 213 us
            FM_I2SPRE->IPCR2_f.IPOWT = 3u;
            break;
        case I2sPllWait426us:     // 426 us
            FM_I2SPRE->IPCR2_f.IPOWT = 4u;
            break;
        case I2sPllWait853us:     // 853 us
            FM_I2SPRE->IPCR2_f.IPOWT = 5u;
            break;
        case I2sPllWait1ms70:     // 1.70 ms
            FM_I2SPRE->IPCR2_f.IPOWT = 6u;
            break;
        case I2sPllWait3ms41:     // 3.41 ms
            FM_I2SPRE->IPCR2_f.IPOWT = 7u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    // Set clock frequency 
    FM_I2SPRE->IPCR3_f.IPLLK = pstcConfig->u8Pllk - 1u;
    FM_I2SPRE->IPCR4_f.IPLLN = pstcConfig->u8Plln - 1u;
    FM_I2SPRE->IPCR5_f.IPLLM = pstcConfig->u8Pllm - 1u;

    FM_I2SPRE->IPINT_ENR_f.IPCSE = 0u; // No interrupt used
 
#if (PDL_PERIPHERAL_ENABLE_I2S0)
    // Set I2S ch.0 clock input
    switch (pstcConfig->enI2s0ClkIn)
    {
        case I2s0UsePllClk:
            FM_I2SPRE->ICCR_f.ICSEL = 0u;
            break;
        case I2s0UseI2sMclki:
            FM_I2SPRE->ICCR_f.ICSEL = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
#endif
    
    
#if (PDL_PERIPHERAL_ENABLE_I2S1)
    // Set I2S ch.1 clock input
    switch (pstcConfig->enI2s1ClkIn)
    {
        case I2s1UsePllClk:
            FM_I2SPRE->ICCR1_f.ICSEL1 = 0u;
            break;
        case I2s1UseI2sMclki:
            FM_I2SPRE->ICCR1_f.ICSEL1 = 1u;
            break;
        case I2s1UseI2sMclki1:
            FM_I2SPRE->ICCR1_f.ICSEL1 = 3u;
            break;
        default:
            return ErrorInvalidParameter;
    }
#endif
    
    return Ok;
} // I2s_InitClk()

/**
 *****************************************************************************
 ** \brief This function de-initialises the I2S PLL clock and reset its
 **        registers.
 **
 ** \return  Ok on successful clock de-init
 *****************************************************************************/
en_result_t I2s_DeInitClk(void)
{
  // Clock off
  FM_I2SPRE->ICCR = 0u;
#if(defined(FM_I2SPRE_ICCR1))
  FM_I2SPRE->ICCR_1 = 0u;
#endif
  
  // PLL off
  FM_I2SPRE->IPCR1_f.IPLLEN = 0u;
  
  // Switch off interrupts
  FM_I2SPRE->IPINT_ENR_f.IPCSE = 0u;
  
  // Set PLL registers to default values
  FM_I2SPRE->IPCR3_f.IPLLK = 0x01u;
  FM_I2SPRE->IPCR4_f.IPLLN = 0x1Fu;
  FM_I2SPRE->IPCR5_f.IPLLM = 0x18u;
#if defined(FM_I2SPRE_IPCR5_1) 
  FM_I2SPRE->IPCR5_1_f.IPLLM1 = 0x18u;
#endif
  
  // Clear PLL stabilization wait time register
  FM_I2SPRE->IPCR2_f.IPOWT = 0u;
  
  return Ok;
} // I2s_DeInitClk()

/**
 *****************************************************************************
 ** \brief This function enables I2S PLL
 **
 ** \note This function can be used for polling if the input parameter is true
 **       and returns directly after enable PLL.
 **
 ** \return - Ok             I2S PLL is enabled
 **         - ErrorTimeout   I2S PLL is timeout
 *****************************************************************************/
en_result_t I2s_EnablePll(boolean_t bWaitPolling)
{
    uint32_t u32TimeOut;
    
    u32TimeOut = SystemCoreClock/1000u;
  
    // Enable PLL
    FM_I2SPRE->IPCR1_f.IPLLEN = 1u;
  
    if (bWaitPolling == TRUE)
    {
        // Wait for PLL stabilization
        while((0u != u32TimeOut) && (0u == FM_I2SPRE->IP_STR_f.IPRDY))
        {
            u32TimeOut--;
        }
    
        // Too long waited?
        if (0u == u32TimeOut)
        {
            // Disable PLL
            FM_I2SPRE->IPCR1_f.IPLLEN = 0u;
    
            return ErrorTimeout;
        }
    }
    
    return Ok;
}

/**
 *****************************************************************************
 ** \brief This function disables I2S PLL
 **
 ** \return - Ok             I2S PLL is disable
 *****************************************************************************/
en_result_t I2s_DisablePll(void)
{
    // Disable PLL
    FM_I2SPRE->IPCR1_f.IPLLEN = 0u;
    
    return Ok;
}

/**
 *****************************************************************************
 ** \brief This function checks, if the PLL has stabilized.
 **
 ** \pre I2s_InitClk() has to be called before.
 **
 ** \note This function can be used for polling, if the PLL has stabilized and
 **       in I2s_InitClk() the waiting was disabled
 **
 ** \return - Ok             PLL is stabilized
 **         - ErrorNotReady  PLL is not stabilized
 *****************************************************************************/
en_result_t I2s_CheckPllReady(void)
{
  if (1u == FM_I2SPRE->IP_STR_f.IPRDY)
  {
    return Ok;
  }
  
  return ErrorNotReady;
} // I2s_PllReady()

/**
 *****************************************************************************
 ** \brief This function starts the I2S clock.
 *****************************************************************************/
void I2s_StartClk(volatile stc_i2sn_t* pstcI2s)
{
#if (PDL_PERIPHERAL_ENABLE_I2S0)
  if(&I2S0 == pstcI2s)
    {
        FM_I2SPRE->ICCR_f.I2SEN = 1u;
    }
#endif
  
#if (PDL_PERIPHERAL_ENABLE_I2S1)    
    if(&I2S1 == pstcI2s)
    {
        FM_I2SPRE->ICCR1_f.I2SEN1 = 1u;
    }
#endif
} // I2s_StartClk()

/**
 *****************************************************************************
 ** \brief This function stops the I2S clock.
 *****************************************************************************/
void I2s_StopClk(volatile stc_i2sn_t* pstcI2s)
{
#if (PDL_PERIPHERAL_ENABLE_I2S0)  
    if(&I2S0 == pstcI2s)
    {
        FM_I2SPRE->ICCR_f.I2SEN = 0u;
    }
#endif

#if (PDL_PERIPHERAL_ENABLE_I2S1)     
    else if(&I2S1 == pstcI2s)
    {
        FM_I2SPRE->ICCR1_f.I2SEN1 = 0u;
    }
#endif
} // I2s_StopClk()

/**
 *****************************************************************************
 ** This function initialises one specific I2S module with the parameters
 ** provided in the given configuration structure.
 ** After successful initialization the I2S module is ready to use.
 **
 ** I2S_Init() has to be called with the parameter pstcConfig of type
 ** stc_i2s_config_t, the basic I2S settings automatic retransmission, the I2s
 ** baudrate, and the error and status callback function can be set.
 **
 ** All values in pstcConfig have to be in valid range (see i2s.h for allowed
 ** ranges of dedicated parameters). The error and status change callback
 ** functions can be NULL. In this case no information of error or status
 ** changes will be reported to the user application.
 **
 ** To reset and disable the I2S module the function I2s_DeInit() has to be used.
 **
 ** \param [in] pstcI2s         Pointer to I2S instance
 ** \param [in] pstcConfig      Pointer to I2S configuration parameters
 **
 ** \return - Ok on successful init
 **         - ErrorInvalidParameter if bit rate is 0 or pointers are NULL
 *****************************************************************************/
en_result_t I2s_Init(volatile stc_i2sn_t* pstcI2s, const stc_i2s_config_t* pstcConfig)
{
    stc_i2s_intern_data_t* pstcI2sInterndata;  
    
    if((NULL == pstcI2s) || (NULL == pstcConfig))
    {
        return ErrorInvalidParameter;
    }
    
    // Get the poitner of intern structure
    pstcI2sInterndata = I2sGetInternDataPtr(pstcI2s);
    
    if(NULL == pstcI2sInterndata)
    {
        return ErrorInvalidParameter;
    }

    // Disable I2C channel for configuration
    pstcI2s->OPRREG_f.START = 0u ;     

    // Clock divider configuration
    pstcI2s->CNTREG_f.CKRT  =  pstcConfig->u8ClockDiv ;  
    // Number of overhead bits in corresponding sub frame channel
    pstcI2s->CNTREG_f.OVHD  =  pstcConfig->u16OverheadBits ; 
    // Output data in case of Invalid/Empty Frame
    pstcI2s->CNTREG_f.MSKB  =  pstcConfig->bMaskBit ; 
    // Master - Slave configuration 
    pstcI2s->CNTREG_f.MSMD  =  pstcConfig->bMasterMode ; 
    // Number of Subframe constructions
    pstcI2s->CNTREG_f.SBFN  =  pstcConfig->bSubFrame01 ;
    // Word construction of FIFO (one 32bit word or two 16bit half words
    pstcI2s->CNTREG_f.RHLL  =  pstcConfig->bFifoTwoWords ; 
    // Clock source selector (internal clock CLK_PERI4_PD2 or external clock ECLK)
    pstcI2s->CNTREG_f.ECKM  =  pstcConfig->bBclkDivByMclk ;
    // Bit extension
    pstcI2s->CNTREG_f.BEXT  =  pstcConfig->bBitExtensionHigh ; 
    // Output Mode of Frame Synchronous Signal
    pstcI2s->CNTREG_f.FRUN  =  pstcConfig->bFreeRunMode ; 
    // Shifting order
    pstcI2s->CNTREG_f.MLSB  =  pstcConfig->bLsbFirst ; 
    // Sampling point of data reception
    pstcI2s->CNTREG_f.SMPL  =  pstcConfig->bSampleAtEnd ; 
    // Clock polarity
    pstcI2s->CNTREG_f.CPOL  =  pstcConfig->bClockpolarity ; 
    // Frame Sync Phase
    pstcI2s->CNTREG_f.FSPH  =  pstcConfig->bWordSelectSamePhase ; 
    // Frame Sync Pulse Width
    pstcI2s->CNTREG_f.FSLN  =  pstcConfig->bWordSelectLength ; 
    // Frame Sync Polarity
    pstcI2s->CNTREG_f.FSPL  =  pstcConfig->bWordSelectPolarity ; 

    // Configuration for Sub Frame 0
    pstcI2s->MCR0REG_f.S0CHN  =  pstcConfig->stcSubframe0.u8Snchn ; 
    pstcI2s->MCR0REG_f.S0CHL  =  pstcConfig->stcSubframe0.u8Snchl ; 
    pstcI2s->MCR0REG_f.S0WDL  =  pstcConfig->stcSubframe0.u8Snwdl ; 

    // Configuration for Sub Frame 1
    pstcI2s->MCR0REG_f.S1CHN  =  pstcConfig->stcSubframe1.u8Snchn ; 
    pstcI2s->MCR0REG_f.S1CHL  =  pstcConfig->stcSubframe1.u8Snchl ; 
    pstcI2s->MCR0REG_f.S1WDL  =  pstcConfig->stcSubframe1.u8Snwdl ; 

    // Set active channels for Sub Frame 0   
    pstcI2s->MCR1REG  =  pstcConfig->u32S0ch ; 
    // Set active channels for Sub Frame 1   
    pstcI2s->MCR2REG  =  pstcConfig->u32S1ch ;

    // Set DMA enable
    pstcI2s->INTCNT_f.TXFDM  = (TRUE == pstcConfig->bTxDmaEnable)               ? 0u : 1u ;
    pstcI2s->INTCNT_f.RXFDM  = (TRUE == pstcConfig->bRxDmaEnable)               ? 0u : 1u ;
    
    // FIFO thresholds
    pstcI2s->INTCNT_f.TFTH = 0x0F & (pstcConfig->u8TxFifoThreshold);
    pstcI2s->INTCNT_f.RFTH = 0x0F & (pstcConfig->u8RxFifoThreshold);
    
    // Packet receive completion timer
    switch (pstcConfig->enPacketReceiveCompletionTimer)
    {
        case NoOperation:
            pstcI2s->INTCNT_f.RPTMR = 0x0;
            break;
        case Hclk54000Cycles:
            pstcI2s->INTCNT_f.RPTMR = 0x1;
            break;
        case Hclk108000Cycles:
            pstcI2s->INTCNT_f.RPTMR = 0x2;
            break;
        case Hclk216000Cycles:
            pstcI2s->INTCNT_f.RPTMR = 0x3;
            break;
        default:
            return ErrorInvalidParameter ;
    }

#if (PDL_INTERRUPT_ENABLE_I2S0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_I2S1 == PDL_ON)    
    // Set interrupt enable
    if (pstcConfig->pstcIrqEn != NULL)
    {
        pstcI2s->INTCNT_f.TXUD0M = (TRUE == pstcConfig->pstcIrqEn->bTxFifoUnderflow0Irq) ? 0u : 1u ;
        pstcI2s->INTCNT_f.TXUD1M = (TRUE == pstcConfig->pstcIrqEn->bTxFifoUnderflow1Irq) ? 0u : 1u ;
        pstcI2s->INTCNT_f.TBERM  = (TRUE == pstcConfig->pstcIrqEn->bTxBlockSizeErrIrq)   ? 0u : 1u ;
        pstcI2s->INTCNT_f.FERRM  = (TRUE == pstcConfig->pstcIrqEn->bFrameErrIrq)         ? 0u : 1u ;
        pstcI2s->INTCNT_f.TXOVM  = (TRUE == pstcConfig->pstcIrqEn->bTxFifoOverflowIrq)   ? 0u : 1u ;   
        pstcI2s->INTCNT_f.TXFIM  = (TRUE == pstcConfig->pstcIrqEn->bTxFifoIrq)           ? 0u : 1u ;
        pstcI2s->INTCNT_f.RBERM  = (TRUE == pstcConfig->pstcIrqEn->bRxBlockSizeErrIrq)   ? 0u : 1u ;
        pstcI2s->INTCNT_f.RXUDM  = (TRUE == pstcConfig->pstcIrqEn->bRxFifoUnderflowIrq)  ? 0u : 1u ;
        pstcI2s->INTCNT_f.RXOVM  = (TRUE == pstcConfig->pstcIrqEn->bRxFifoOverflowIrq)   ? 0u : 1u ;
        pstcI2s->INTCNT_f.EOPM   = (TRUE == pstcConfig->pstcIrqEn->bRxFifoIdleDetectIrq) ? 0u : 1u ;
        pstcI2s->INTCNT_f.RXFIM  = (TRUE == pstcConfig->pstcIrqEn->bRxFifoIrq)           ? 0u : 1u ;
    }

    // Set interrupt callback funcitons
    if (pstcConfig->pstcIrqCb != NULL)
    {
        pstcI2sInterndata->pfnTxFifoUnderflow0IrqCb = pstcConfig->pstcIrqCb->pfnTxFifoUnderflow0IrqCb;
        pstcI2sInterndata->pfnTxFifoUnderflow1IrqCb = pstcConfig->pstcIrqCb->pfnTxFifoUnderflow1IrqCb;
        pstcI2sInterndata->pfnTxBlockSizeErrIrqCb = pstcConfig->pstcIrqCb->pfnTxBlockSizeErrIrqCb;
        pstcI2sInterndata->pfnFrameErrIrqCb = pstcConfig->pstcIrqCb->pfnFrameErrIrqCb;
        pstcI2sInterndata->pfnTxFifoOverflowIrqCb = pstcConfig->pstcIrqCb->pfnTxFifoOverflowIrqCb;
        pstcI2sInterndata->pfnTxFifoIrqCb = pstcConfig->pstcIrqCb->pfnTxFifoIrqCb;
        pstcI2sInterndata->pfnRxBlockSizeErrIrqCb = pstcConfig->pstcIrqCb->pfnRxBlockSizeErrIrqCb;
        pstcI2sInterndata->pfnRxFifoUnderflowIrqCb = pstcConfig->pstcIrqCb->pfnRxFifoUnderflowIrqCb;
        pstcI2sInterndata->pfnRxFifoOverflowIrqCb = pstcConfig->pstcIrqCb->pfnRxFifoOverflowIrqCb;
        pstcI2sInterndata->pfnRxFifoIdleDetectIrqCb = pstcConfig->pstcIrqCb->pfnRxFifoIdleDetectIrqCb;
        pstcI2sInterndata->pfnRxFifoIrqCb = pstcConfig->pstcIrqCb->pfnRxFifoIrqCb;
    }
    
    if (pstcConfig->bTouchNvic == TRUE)
    {
        I2sInitNvic(pstcI2s);
    }
#endif    
    
    // Enable I2S Transmitter
    pstcI2s->OPRREG_f.TXENB = (FALSE == pstcConfig->bTxEnable) ? 0u : 1u ;
    pstcI2s->CNTREG_f.TXDIS = (FALSE == pstcConfig->bTxEnable) ? 1u : 0u ;

    // Enable I2S Receiver
    pstcI2s->OPRREG_f.RXENB = (FALSE == pstcConfig->bRxEnable) ? 0u : 1u ;
    pstcI2s->CNTREG_f.RXDIS = (FALSE == pstcConfig->bRxEnable) ? 1u : 0u ;


    return Ok ;    
} // I2S_Init

/**
 *****************************************************************************
 ** \brief Deinitializes the I2S module.
 **
 ** Any pending transmission or reception will be aborted and all I2S related
 ** registers are reset to their default values.
 **
 ** \param [in] pstcI2s            Pointer to I2S instance
 ** \param [in] bTouchNvic         TRUE = De-Init NVIC
 **
 ** \retval Ok                    I2S module has been successfully deinitialized
 ** \retval ErrorInvalidParameter If one of the following conditions are met:
 **   - FM4_I2S0 == NULL
 **   - FM4_I2S0InternData == NULL (invalid or disabled I2S unit 
 *****************************************************************************/
en_result_t I2s_DeInit(volatile stc_i2sn_t* pstcI2s, boolean_t bTouchNvic)
{
    stc_i2s_intern_data_t* pstcI2sInterndata;  
    
    // Get the poitner of intern structure
    pstcI2sInterndata = I2sGetInternDataPtr(pstcI2s);
    
    if(NULL == pstcI2sInterndata)
    {
        return ErrorInvalidParameter ;
    }

    // Disable I2S channel for configuration
    pstcI2s->OPRREG_f.START = 0u ;     

    // Reset CNTREG bitfields to initial values
    pstcI2s->CNTREG = 0u ;

    // Reset MCR0REG bitfields to initial values
    pstcI2s->MCR0REG = 0u;

    // Disable all channels for Sub Frame 0   
    pstcI2s->MCR1REG = 0u ; 
      
    // Disable all channels for Sub Frame 1
    pstcI2s->MCR2REG = 0u ; 
      
    // Finally reset I2S hardware
    pstcI2s->SRST_f.SRST = 1u ;

#if (PDL_INTERRUPT_ENABLE_I2S0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_I2S1 == PDL_ON)     
    pstcI2sInterndata->pfnTxFifoUnderflow0IrqCb = NULL;
    pstcI2sInterndata->pfnTxFifoUnderflow1IrqCb = NULL;
    pstcI2sInterndata->pfnTxBlockSizeErrIrqCb = NULL;
    pstcI2sInterndata->pfnFrameErrIrqCb = NULL;
    pstcI2sInterndata->pfnTxFifoOverflowIrqCb = NULL;
    pstcI2sInterndata->pfnTxFifoIrqCb = NULL;
    pstcI2sInterndata->pfnRxBlockSizeErrIrqCb = NULL;
    pstcI2sInterndata->pfnRxFifoUnderflowIrqCb = NULL;
    pstcI2sInterndata->pfnRxFifoOverflowIrqCb = NULL;
    pstcI2sInterndata->pfnRxFifoIdleDetectIrqCb = NULL;
    pstcI2sInterndata->pfnRxFifoIrqCb = NULL;
    
    if (TRUE == bTouchNvic)
    {
        I2sDeInitNvic(pstcI2s);
    }
#endif    
    
    return Ok ;  
} // I2S_DeInit

/**
 *****************************************************************************
 ** \brief Start I2S
 **
 ** \retval Ok                I2S module has been started
 *****************************************************************************/
en_result_t I2s_Start(volatile stc_i2sn_t* pstcI2s)
{
    pstcI2s->OPRREG_f.START  = 1u ;

    return Ok;
}

/**
 *****************************************************************************
 ** \brief Stop I2S
 **
 ** \retval Ok                I2S module has been stopped
 *****************************************************************************/
en_result_t I2s_Stop(volatile stc_i2sn_t* pstcI2s)
{
    pstcI2s->OPRREG_f.START  = 0u ;

    return Ok;
}

/**
 *****************************************************************************
 ** \brief Reset I2S
 **
 ** \retval Ok                I2S module has been stopped
 *****************************************************************************/
en_result_t I2s_Reset(volatile stc_i2sn_t* pstcI2s)
{
    pstcI2s->SRST_f.SRST = 1u ;

    return Ok;
}

/**
 *****************************************************************************
 ** \brief Enable I2S TX
 **
 ** \retval Ok                I2S module has been stopped
 *****************************************************************************/
en_result_t I2s_EnableTx(volatile stc_i2sn_t* pstcI2s)
{
    // Enable I2S Transmitter
    pstcI2s->OPRREG_f.TXENB = 1u;;
    pstcI2s->CNTREG_f.TXDIS = 0u ;

    return Ok;
}

/**
 *****************************************************************************
 ** \brief Disable I2S TX
 **
 ** \retval Ok                I2S module has been stopped
 *****************************************************************************/
en_result_t I2s_DisableTx(volatile stc_i2sn_t* pstcI2s)
{
    // Enable I2S Transmitter
    pstcI2s->OPRREG_f.TXENB = 0u;;
    pstcI2s->CNTREG_f.TXDIS = 1u ;

    return Ok;
}

/**
 *****************************************************************************
 ** \brief Enable I2S RX
 **
 ** \retval Ok                I2S module has been stopped
 *****************************************************************************/
en_result_t I2s_EnableRx(volatile stc_i2sn_t* pstcI2s)
{
    // Enable I2S Receiver
    pstcI2s->OPRREG_f.RXENB = 1u;
    pstcI2s->CNTREG_f.RXDIS = 0u;
    
    return Ok;
}

/**
 *****************************************************************************
 ** \brief Disable I2S RX
 **
 ** \retval Ok                I2S module has been stopped
 *****************************************************************************/
en_result_t I2s_DisableRx(volatile stc_i2sn_t* pstcI2s)
{
    // Enable I2S Receiver
    pstcI2s->OPRREG_f.RXENB = 0u;
    pstcI2s->CNTREG_f.RXDIS = 1u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get status of I2S according to status type
 **
 ** \param [in] pstcI2s      Pointer to I2S instance   
 ** \param [in] enStatus     I2S status type
 ** \arg   I2sDmaTxChBlockSizeErr  DMA transmit channel block size error
 ** \arg   I2sDmaRxChBlockSizeErr  DMA receive channel block size error
 ** \arg   I2sFrameErr             Frame error
 ** \arg   I2sTxFifoUnderflow1     TX FIFO underflow interrupt flag 1
 ** \arg   I2sTxFifoUnderflow0     TX FIFO underflow interrupt flag 0
 ** \arg   I2sTxFifoOverflow       TX FIFO overflow interrupt flag 
 ** \arg   I2sRxFifoUnderflow      RX FIFO underflow interrupt flag
 ** \arg   I2sRxFifoOverflow       RX FIFO overflow interrupt flag
 ** \arg   I2sRxFifoFifoIdle       RX FIFO idle interrupt flag
 ** \arg   I2sBusy                 I2S transmit busy status
 ** \arg   I2sTxFifoMatchThreshold TX FIFO interrupt flag
 ** \arg   I2sRxFifoMatchThreshold RX FIFO interrupt flag
 ** 
 ** \retval FALSE                 If one of following conditions are met:
 **                               - Block size of the DMA transmit channel is set to a value smaller than the transmit FIFO threshold value [enStatus = I2sDmaTxChBlockSizeErr]
 **                               - Block size of the DMA receive channel is set to a value larger than the receive FIFO threshold value [enStatus = I2sDmaRxChBlockSizeErr]
 **                               - Frame sync signal is received at the frame rate that was set. [enStatus = I2sFrameErr]
 **                               - Transmit FIFO don't underflow at the frame start [enStatus = I2sTxFifoUnderflow1]
 **                               - Transmit FIFO don't underflow during frame transmission [enStatus = I2sTxFifoUnderflow0]
 **                               - Transmit FIFO don't overflow [enStatus = I2sTxFifoOverflow]
 **                               - Receive FIFO don't underflow [enStatus = I2sRxFifoUnderflow]
 **                               - Receive FIFO don't overflow [enStatus = I2sRxFifoOverflow]
 **                               - Receive FIFO is empty when the timer reaches the time that was set [enStatus = I2sRxFifoFifoIdle]
 **                               - Serial transmit control unit is not busy.  [enStatus =I2sBusy]
 **                               - Transmit FIFO empty slot is less than the TX FIFO threshold value  [enStatus = I2sTxFifoMatchThreshold]
 **                               - Receive FIFO data count is less than the RX FIFO threshold value   [enStatus = I2sRxFifoMatchThreshold]
 ** \retval TRUE                 If one of following conditions are met:
 **                               - Block size of the DMA transmit channel is set to a value larger than the transmit FIFO threshold value [enStatus = I2sDmaTxChBlockSizeErr]
 **                               - Block size of the DMA receive channel is set to a value larger than the receive FIFO threshold value [enStatus = I2sDmaRxChBlockSizeErr]
 **                               - Frame sync signal cannot be received at the frame rate that was set. [enStatus = I2sFrameErr]
 **                               - Transmit FIFO underflows at the frame start [enStatus = I2sTxFifoUnderflow1]
 **                               - Transmit FIFO underflows during frame transmission [enStatus = I2sTxFifoUnderflow0]
 **                               - Transmit FIFO overflows [enStatus = I2sTxFifoOverflow]
 **                               - Receive FIFO underflows [enStatus = I2sRxFifoUnderflow]
 **                               - Receive FIFO overflows [enStatus = I2sRxFifoOverflow]
 **                               - Receive FIFO is not empty when the timer reaches the time that was set  [enStatus = I2sRxFifoFifoIdle]
 **                               - Serial transmit control unit is busy.  [enStatus =I2sBusy]
 **                               - Transmit FIFO empty slot meets or exceeds the TX FIFO threshold value  [enStatus = I2sTxFifoMatchThreshold]
 **                               - Receive FIFO data count meets or exceeds the RX FIFO threshold value   [enStatus = I2sRxFifoMatchThreshold]
 ** 
 ******************************************************************************/
boolean_t I2s_GetStatus(volatile stc_i2sn_t* pstcI2s, 
                        en_i2s_status_t enStatus)
{
    boolean_t bRet = FALSE;
      
    switch(enStatus)
    {
        case I2sDmaTxChBlockSizeErr:
            bRet = (1u == pstcI2s->STATUS_f.TBERR) ? TRUE : FALSE;
            break;
        case I2sDmaRxChBlockSizeErr:
            bRet = (1u == pstcI2s->STATUS_f.RBERR) ? TRUE : FALSE;
            break;
        case I2sFrameErr:
            bRet = (1u == pstcI2s->STATUS_f.FERR) ? TRUE : FALSE;
            break;
        case I2sTxFifoUnderflow1:
            bRet = (1u == pstcI2s->STATUS_f.TXUDR1) ? TRUE : FALSE;
            break;
        case I2sTxFifoUnderflow0:
            bRet = (1u == pstcI2s->STATUS_f.TXUDR0) ? TRUE : FALSE;
            break;
        case I2sTxFifoOverflow:
            bRet = (1u == pstcI2s->STATUS_f.TXOVR) ? TRUE : FALSE;
            break;
        case I2sRxFifoUnderflow:
            bRet = (1u == pstcI2s->STATUS_f.RXUDR) ? TRUE : FALSE;
            break; 
        case I2sRxFifoOverflow:
            bRet = (1u == pstcI2s->STATUS_f.RXUDR) ? TRUE : FALSE;
            break;     
        case I2sRxFifoFifoIdle:
            bRet = (1u == pstcI2s->STATUS_f.EOPI) ? TRUE : FALSE;
            break;
        case I2sBusy:
            bRet = (1u == pstcI2s->STATUS_f.BSY) ? TRUE : FALSE;
            break;    
        case I2sTxFifoMatchThreshold:
            bRet = (1u == pstcI2s->STATUS_f.RXUDR) ? TRUE : FALSE;
            break;
        case I2sRxFifoMatchThreshold:
            bRet = (1u == pstcI2s->STATUS_f.RXUDR) ? TRUE : FALSE;
            break;    
        default:
            break;
    }
    
    return bRet;
}

/**
 ******************************************************************************
 ** \brief Clear status of I2S according to status type
 **
 ** \param [in] pstcI2s      Pointer to I2S instance   
 ** \param [in] enStatus     I2S status type
 ** \arg   I2sDmaTxChBlockSizeErr  DMA transmit channel block size error
 ** \arg   I2sDmaRxChBlockSizeErr  DMA receive channel block size error
 ** \arg   I2sFrameErr             Frame error
 ** \arg   I2sTxFifoUnderflow1     TX FIFO underflow interrupt flag 1
 ** \arg   I2sTxFifoUnderflow0     TX FIFO underflow interrupt flag 0
 ** \arg   I2sTxFifoOverflow       TX FIFO overflow interrupt flag 
 ** \arg   I2sRxFifoUnderflow      RX FIFO underflow interrupt flag
 ** \arg   I2sRxFifoOverflow       RX FIFO overflow interrupt flag
 ** \arg   I2sRxFifoFifoIdle       RX FIFO idle interrupt flag
 ** \arg   I2sBusy                 I2S transmit busy status
 ** \arg   I2sTxFifoMatchThreshold TX FIFO interrupt flag
 ** \arg   I2sRxFifoMatchThreshold RX FIFO interrupt flag
 ** 
 ** \retval Ok             The status is cleared
 ** \retval ErrorInvalidParameter pstcI2s == NULL or invalid value of enStatus
 **
 ** \note  The following status is cleared by hardware automatically:
 **        - I2sDmaTxChBlockSizeErr
 **        - I2sDmaRxChBlockSizeErr
 **        - I2sBusy
 **        - I2sTxFifoMatchThreshold
 **        - I2sRxFifoMatchThreshold
 ** 
 ******************************************************************************/
en_result_t I2s_ClrStatus(volatile stc_i2sn_t* pstcI2s, 
                          en_i2s_status_t enStatus)
{
    if(pstcI2s == NULL)
    {
        return ErrorInvalidParameter;
    }
    
    switch(enStatus)
    {
        case I2sDmaTxChBlockSizeErr:
            break;
        case I2sDmaRxChBlockSizeErr:
            break;
        case I2sFrameErr:
            pstcI2s->STATUS_f.FERR = 0u;
            break;
        case I2sTxFifoUnderflow1:
            pstcI2s->STATUS_f.TXUDR1 = 0u;
            break;
        case I2sTxFifoUnderflow0:
            pstcI2s->STATUS_f.TXUDR1 = 0u;
            break;
        case I2sTxFifoOverflow:
            pstcI2s->STATUS_f.TXOVR = 0u;
            break;
        case I2sRxFifoUnderflow:
            pstcI2s->STATUS_f.RXUDR = 0u;
            break; 
        case I2sRxFifoOverflow:
            pstcI2s->STATUS_f.RXUDR = 0u;
            break;     
        case I2sRxFifoFifoIdle:
            pstcI2s->STATUS_f.EOPI = 0u;
            break;
        case I2sBusy:
            break;    
        case I2sTxFifoMatchThreshold:
            break;
        case I2sRxFifoMatchThreshold:
            break;    
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 *****************************************************************************
 ** \brief Write I2S TX FIFO
 **
 ** \param pstcI2s Pointer to I2S instance
 ** \param u32Data  Data to be written
 **
 ** \retval Ok      Data has been written
 ** \retval ErrorInvalidParameter    pstcI2s == NULL
 **
 ** \note The FIFO is always written by word
 *****************************************************************************/
en_result_t I2s_WriteTxFifo(volatile stc_i2sn_t* pstcI2s, uint32_t u32Data)
{
    if (pstcI2s == NULL)
    {
        return ErrorInvalidParameter;
    }
    
    pstcI2s->TXFDAT = u32Data;
    
    return Ok;
}

/**
 *****************************************************************************
 ** \brief Read I2S RX FIFO
 **
 ** \param pstcI2s Pointer to I2S instance
 **
 ** \retval The receive data in FIFO
 **
 ** \note The FIFO is always read by word
 *****************************************************************************/
uint32_t I2s_ReadRxFifo(volatile stc_i2sn_t* pstcI2s)
{
    return pstcI2s->RXFDAT;
}

/**
 *****************************************************************************
 ** \brief Read I2S TX FIFO number
 **
 ** \param pstcI2s Pointer to I2S instance
 **
 ** \retval The transmit data in FIFO
 **
 ** \note It is number of word
 *****************************************************************************/
uint8_t I2s_GetTxFifoNumber(volatile stc_i2sn_t* pstcI2s)
{
    return pstcI2s->STATUS_f.TXNUM;
}

/**
 *****************************************************************************
 ** \brief Read I2S RX FIFO number
 **
 ** \param pstcI2s Pointer to I2S instance
 **
 ** \retval The receive data in FIFO
 **
 ** \note It is number of word
 *****************************************************************************/
uint8_t I2s_GetRxFifoNumber(volatile stc_i2sn_t* pstcI2s)
{
    return pstcI2s->STATUS_f.RXNUM;
}

#endif

/*****************************************************************************/
/* EOF (not truncated)                                                       */
/*****************************************************************************/
