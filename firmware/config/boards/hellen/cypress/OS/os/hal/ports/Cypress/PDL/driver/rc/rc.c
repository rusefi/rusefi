/*******************************************************************************
* \file              rc.c
* 
* \version           1.20
*
* \brief             Remote Control driver
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
#include "rc/rc.h"

#if (defined(PDL_PERIPHERAL_RC_ACTIVE))


/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/
/// Look-up table for all enabled RC instances and their internal data
stc_rc_instance_data_t m_astcRcInstanceDataLut[RC_INSTANCE_COUNT] =
{
#if (PDL_PERIPHERAL_ENABLE_RC0 == PDL_ON)
    { 
        &RC0,  // pstcInstance
        {
            0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u
        }           
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_RC1 == PDL_ON)
    {  
        &RC1,  // pstcInstance
        {
            0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u
        }        
    },
#endif
};
   
/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/
#if (PDL_INTERRUPT_ENABLE_RC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_RC1 == PDL_ON)
/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/
static void RcInitNvic(void);
static void RcDeInitNvic(void);

/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/
/**
 ******************************************************************************
 ** \brief Initialize NVIC of Remote Control
 ******************************************************************************/
static void RcInitNvic(void)
{    
#if (PDL_MCU_CORE  ==  PDL_FM0P_CORE)
  #if (PDL_MCU_TYPE == PDL_FM0P_TYPE3)
    NVIC_ClearPendingIRQ(HDMICEC0_1_IRQn);
    NVIC_EnableIRQ(HDMICEC0_1_IRQn);
    NVIC_SetPriority(HDMICEC0_1_IRQn, PDL_IRQ_LEVEL_HDMICEC0_1);
  #else
    #if (PDL_INTERRUPT_ENABLE_RC0 == PDL_ON)
    NVIC_ClearPendingIRQ(PPG00_02_20_DSTC_SMCIF0_HDMICEC0_IRQn);
    NVIC_EnableIRQ(PPG00_02_20_DSTC_SMCIF0_HDMICEC0_IRQn);
    NVIC_SetPriority(PPG00_02_20_DSTC_SMCIF0_HDMICEC0_IRQn, PDL_IRQ_LEVEL_PPG00_02_20_DSTC_SMCIF0_HDMICEC0);
    #endif
    #if (PDL_INTERRUPT_ENABLE_RC1 == PDL_ON)
    NVIC_ClearPendingIRQ(TIM_SMCIF1_HDMICEC1_IRQn);
    NVIC_EnableIRQ(TIM_SMCIF1_HDMICEC1_IRQn);
    NVIC_SetPriority(TIM_SMCIF1_HDMICEC1_IRQn, PDL_IRQ_LEVEL_TIM_SMCIF1_HDMICEC1);
    #endif
  #endif
#elif (PDL_MCU_CORE  ==  PDL_FM3_CORE)  
    #if (PDL_INTERRUPT_ENABLE_RC0 == PDL_ON)
    NVIC_ClearPendingIRQ(USB1F_HDMICEC0_IRQn);
    NVIC_EnableIRQ(USB1F_HDMICEC0_IRQn);
    NVIC_SetPriority(USB1F_HDMICEC0_IRQn, PDL_IRQ_LEVEL_USB1F_HDMICEC0_IRQn);
    #endif
    #if (PDL_INTERRUPT_ENABLE_RC1 == PDL_ON)
    NVIC_ClearPendingIRQ(USB1F_USB1H_HDMICEC1_IRQn);
    NVIC_EnableIRQ(USB1F_USB1H_HDMICEC1_IRQn);
    NVIC_SetPriority(USB1F_USB1H_HDMICEC1_IRQn, \
    PDL_IRQ_LEVEL_USB1F_USB1H_HDMICEC1_IRQn);
    #endif
#elif (PDL_MCU_CORE  ==  PDL_FM4_CORE) 
#endif 
} /* RcInitNvic */

/**
 ******************************************************************************
 ** \brief De-Initialize NVIC of Remote Control
 ******************************************************************************/
static void RcDeInitNvic(void)
{
#if (PDL_MCU_CORE  ==  PDL_FM0P_CORE)
  #if (PDL_MCU_TYPE == PDL_FM0P_TYPE3)
    NVIC_ClearPendingIRQ(HDMICEC0_1_IRQn);
    NVIC_DisableIRQ(HDMICEC0_1_IRQn);
    NVIC_SetPriority(HDMICEC0_1_IRQn, PDL_IRQ_LEVEL_HDMICEC0_1);
  #else
    #if (PDL_INTERRUPT_ENABLE_RC0 == PDL_ON)
    NVIC_ClearPendingIRQ(PPG00_02_20_DSTC_SMCIF0_HDMICEC0_IRQn);
    NVIC_DisableIRQ(PPG00_02_20_DSTC_SMCIF0_HDMICEC0_IRQn);
    NVIC_SetPriority(PPG00_02_20_DSTC_SMCIF0_HDMICEC0_IRQn, PDL_IRQ_LEVEL_PPG00_02_20_DSTC_SMCIF0_HDMICEC0);
    #endif
    #if (PDL_INTERRUPT_ENABLE_RC1 == PDL_ON)
    NVIC_ClearPendingIRQ(TIM_SMCIF1_HDMICEC1_IRQn);
    NVIC_DisableIRQ(TIM_SMCIF1_HDMICEC1_IRQn);
    NVIC_SetPriority(TIM_SMCIF1_HDMICEC1_IRQn, PDL_IRQ_LEVEL_TIM_SMCIF1_HDMICEC1); 
    #endif
  #endif    
#elif (PDL_MCU_CORE  ==  PDL_FM3_CORE) 
    #if (PDL_INTERRUPT_ENABLE_RC0 == PDL_ON)
    NVIC_ClearPendingIRQ(USB1F_HDMICEC0_IRQn);
    NVIC_DisableIRQ(USB1F_HDMICEC0_IRQn);
    NVIC_SetPriority(USB1F_HDMICEC0_IRQn, PDL_IRQ_LEVEL_USB1F_HDMICEC0_IRQn);
    #endif
    #if (PDL_INTERRUPT_ENABLE_RC1 == PDL_ON)
    NVIC_ClearPendingIRQ(USB1F_USB1H_HDMICEC1_IRQn);
    NVIC_DisableIRQ(USB1F_USB1H_HDMICEC1_IRQn);
    NVIC_SetPriority(USB1F_USB1H_HDMICEC1_IRQn, PDL_IRQ_LEVEL_USB1F_USB1H_HDMICEC1_IRQn); 
    #endif
#elif (PDL_MCU_CORE  ==  PDL_FM4_CORE) 
#endif    
} /* RcDeInitNvic */

/**
 ******************************************************************************
 ** \brief RC instance interrupt service routine
 **
 ** RC instance interrupt service routine, clear interrupt cause and
 ** implement interrupt callback function.
 **
 ** \param  pstcRc              Pointer to RC instance
 ** \param  pstcRcInternData    Pointer to RC intern data
 **        
 ******************************************************************************/
void RcIrqHandler(volatile stc_rcn_t *pstcRc, 
                  stc_rc_intern_data_t* pstcRcInternData)
{
    if (TRUE == pstcRc->RCST_f.ST)  // Start bit detection interrupt? 
    {
        if (NULL != pstcRcInternData->pfnRcRxStartIrqCb)
        {
            pstcRcInternData->pfnRcRxStartIrqCb();
        }
        pstcRc->RCST_f.ST = 0u;   
    }
    
    if (TRUE == pstcRc->RCST_f.ACK)  // ACK interrupt? 
    {
        if (NULL != pstcRcInternData->pfnRcRxAckIrqCb)
        {
            pstcRcInternData->pfnRcRxAckIrqCb();
        }
        pstcRc->RCST_f.ACK = 0u;      
    }
    
    if (TRUE == pstcRc->RCST_f.EOM)  // EOM interrupt? 
    { 
        if (NULL != pstcRcInternData->pfnRcRxEomCb)
        {
            pstcRcInternData->pfnRcRxEomCb();
        }
        pstcRc->RCST_f.EOM = 0u;    
    }
    
    if (TRUE == pstcRc->RCST_f.OVF)  // Counter over detection interrupt? 
    {      
        if (NULL != pstcRcInternData->pfnRcRxCntOvfIrqCb)
        {
            pstcRcInternData->pfnRcRxCntOvfIrqCb();
        }
        pstcRc->RCST_f.OVF = 0u;     
    }

    if (TRUE == pstcRc->RCRC_f.RC)  // Peapeat code interrupt? 
    {      
        if (NULL != pstcRcInternData->pfnRcRxRepeatCodeIrqCb)
        {
            pstcRcInternData->pfnRcRxRepeatCodeIrqCb();
        }
        pstcRc->RCRC_f.RC = 0u;    
    }

    if (TRUE == pstcRc->RCLE_f.LEL)  // Maximun data bit width violation detection interrupt? 
    {   
        if (NULL != pstcRcInternData->pfnRcRxMaxDataIrqCb)
        {
            pstcRcInternData->pfnRcRxMaxDataIrqCb();
        }
        pstcRc->RCLE_f.LEL = 0u;   
    }
    
    if (TRUE == pstcRc->RCLE_f.LES)  // Minimun data bit width violation detection interrupt? 
    {
        if (NULL != pstcRcInternData->pfnRcRxMinDataIrqCb)
        {
            pstcRcInternData->pfnRcRxMinDataIrqCb();
        }
        pstcRc->RCLE_f.LES = 0u;    
    }
    
    if (TRUE == pstcRc->TXSTS_f.IBR)  // Bus error detection violation detection interrupt? 
    {      
        if (NULL != pstcRcInternData->pfnRcTxIrqBusErrorCb)
        {
            pstcRcInternData->pfnRcTxIrqBusErrorCb();
        }
        pstcRc->TXSTS_f.IBR = 0u;     
    }
    
    if (TRUE == pstcRc->TXSTS_f.ITST)  // Transmission status violation detection interrupt? 
    {   
        if (NULL != pstcRcInternData->pfnRcTxIrqStatusCb)
        {
            pstcRcInternData->pfnRcTxIrqStatusCb();
        }
        pstcRc->TXSTS_f.ITST = 0u;    
    }
} /* RcIrqHandler */

#endif

#if (PDL_PERIPHERAL_ENABLE_RCRX_SIRCS_MODE == PDL_ON || \
     PDL_PERIPHERAL_ENABLE_RCRX_NEC_MODE == PDL_ON || \
     PDL_PERIPHERAL_ENABLE_RCRX_CEC_MODE== PDL_ON)
/**
 ******************************************************************************
 ** \brief Return the internal data for a certain RC instance.
 **
 ** \param pstcRc Pointer to RC instance
 **
 ** \return Pointer to internal data or NULL if instance is not enabled (or not known)
 **
 ******************************************************************************/
static stc_rc_intern_data_t* RcGetInternDataPtr(volatile stc_rcn_t* pstcRc) 
{
    uint8_t u8Instance;
   
    for (u8Instance = 0u; u8Instance < RC_INSTANCE_COUNT; u8Instance++)
    {
        if (pstcRc == m_astcRcInstanceDataLut[u8Instance].pstcInstance)
        {
            return &m_astcRcInstanceDataLut[u8Instance].stcInternData;
        }
    }

    return NULL;
}
#endif

/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/

#if (PDL_PERIPHERAL_ENABLE_RCRX_SIRCS_MODE == PDL_ON) 
/**
 ******************************************************************************
 ** \brief Initialize SIRCS mode of Remote Control Reception
 **
 ** This function initializes SIRCS mode of an Remote Control Reception
 **
 ** \param [in]  pstcRc              RC instance
 ** \param [in]  pstcRcSircsConfig   Pointer to SIRCS mode configuration 
 **
 ** \retval Ok                    SIRCS mode initialized normally
 ** \retval ErrorInvalidParameter pstcRc == NULL
 **                               pstcRcSircsConfig == NULL
 **                               Other invalid configuration
 ******************************************************************************/
en_result_t Rc_Rx_Sircs_Init(volatile stc_rcn_t *pstcRc,
                             const stc_rc_rx_sircs_config_t *pstcRcSircsConfig)
{
    // Pointer to internal data
    stc_rc_intern_data_t* pstcRcInternData ; 
  
    // Check for NULL pointers
    if ( (NULL == pstcRcSircsConfig) || 
         (NULL == pstcRc) )
    {
        return ErrorInvalidParameter;
    }
    
    // Get pointer to internal data structure ...
    pstcRcInternData = RcGetInternDataPtr( pstcRc ) ;
    
    // Check for instance available or not
    if(pstcRcInternData == NULL)
    {
        return ErrorInvalidParameter ;
    }
    
    // Set mode
    pstcRc->RCCR_f.MOD = RcSircsMode;   
    
    // Set clock division value
    pstcRc->RCCKD = pstcRcSircsConfig->u16DivVal; 
    
    // Select source clock
    switch (pstcRcSircsConfig->enSrcClk)               
    {
        case RcPeripheralClk:
            pstcRc->RCCKD_f.CKSEL = RcPeripheralClk;    
            break;
        case RcSubClk:
            pstcRc->RCCKD_f.CKSEL = RcSubClk;          
            break;
        default:
           return ErrorInvalidParameter;
    }
    
    // Select threshold type
    switch (pstcRcSircsConfig->enThresholdType)      
    {
        case RcThresholdType0:
            pstcRc->RCCR_f.THSEL = 0u;
            break;
        case RcThresholdType1:
            pstcRc->RCCR_f.THSEL = 1u;
            break;
        default:
           return ErrorInvalidParameter;
    }
    
    // Select overflow cycle
    switch (pstcRcSircsConfig->enOverflowCycle)       
    {
        case RcOverflow128Cycle:
            pstcRc->RCST_f.OVFSEL = 0u;
            break;
        case RcOverflow256Cycle:
            pstcRc->RCST_f.OVFSEL = 1u;
            break;
        default:
           return ErrorInvalidParameter;
    }
    
    // Set address compare enable
    pstcRc->RCCR_f.ADRCE = ((TRUE == pstcRcSircsConfig->bAddrCmpEn) ? 1u : 0u);
    
    // Set start bit, minimum pluse, threshold width 
    pstcRc->RCSHW = pstcRcSircsConfig->u8StartBitWidth;     
    pstcRc->RCDAHW = pstcRcSircsConfig->u8MinPulseWidth;    
    pstcRc->RCDBHW = pstcRcSircsConfig->u8ThresholdWidth;   
    
    // Set compare address which is used to compare with the received device address
    pstcRc->RCADR1 = pstcRcSircsConfig->stcAddr.u8Addr1;    
    pstcRc->RCADR2 = pstcRcSircsConfig->stcAddr.u8Addr2;   
    
#if (PDL_INTERRUPT_ENABLE_RC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_RC1 == PDL_ON)       
    // Set interrupt enable
    if(NULL != pstcRcSircsConfig->pstcIrqEn)
    {
        if(TRUE == pstcRcSircsConfig->pstcIrqEn->bRcRxSircsStartIrq)
        {
            pstcRc->RCST_f.STIE = 1u;
        }   
        if(TRUE == pstcRcSircsConfig->pstcIrqEn->bRcRxSircsAckIrq)
        {
            pstcRc->RCST_f.ACKIE = 1u;
        }
        if(TRUE == pstcRcSircsConfig->pstcIrqEn->bRcRxSircsCntOvfIrq)
        {
            pstcRc->RCST_f.OVFIE = 1u;
        }
    }
    
    // Set interrupt callback functions
    if(NULL != pstcRcSircsConfig->pstcIrqCb)
    {
        pstcRcInternData->pfnRcRxStartIrqCb = pstcRcSircsConfig->pstcIrqCb->pfnRcRxSircsStartIrqCb;
        pstcRcInternData->pfnRcRxAckIrqCb = pstcRcSircsConfig->pstcIrqCb->pfnRcRxSircsAckIrqCb;
        pstcRcInternData->pfnRcRxCntOvfIrqCb = pstcRcSircsConfig->pstcIrqCb->pfnRcRxSircsCntOvfIrqCb;
    }
    
    // Set NVIC
    if(TRUE == pstcRcSircsConfig->bTouchNvic)
    {
        RcInitNvic();
    }
#endif
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief De-Initialize SIRCS mode of Remote Control Reception
 **
 ** This function deinitializes an SRemote Control Reception with IRCS mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  bTouchNvic       Touch NVIC or not
 **
 ** \retval Ok                    RC instance fully disabled and reset
 ** \retval ErrorInvalidParameter pstcRc == NULL
 ******************************************************************************/
en_result_t Rc_Rx_Sircs_DeInit(volatile stc_rcn_t *pstcRc, boolean_t bTouchNvic)
{
    // Pointer to internal data
    stc_rc_intern_data_t* pstcRcInternData ;      
 
    // Get pointer to internal data structure ...
    pstcRcInternData = RcGetInternDataPtr( pstcRc ) ;
    
    // Check for instance available or not
    if(NULL == pstcRcInternData)
    {
        return ErrorInvalidParameter ;
    }
    
    // Clear all registers 
    pstcRc->RCCR = 0u;                           
    pstcRc->RCST = 0u;
    pstcRc->RCSHW = 0u;
    pstcRc->RCDAHW = 0u;
    pstcRc->RCDBHW = 0u;
    pstcRc->RCADR1 = 0u;
    pstcRc->RCADR2 = 0u;
    pstcRc->RCDTHH = 0u;
    pstcRc->RCDTHL = 0u;
    pstcRc->RCDTLH = 0u;
    pstcRc->RCDTLL = 0u;
    pstcRc->RCCKD = 0u;
    pstcRc->RCRC = 0u;
    pstcRc->RCRHW = 0u;
    pstcRc->RCLE = 0u;
    pstcRc->RCLESW = 0u;
    pstcRc->RCLELW = 0u;

#if (PDL_INTERRUPT_ENABLE_RC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_RC1 == PDL_ON)  
    // Clear callback functions  
    pstcRcInternData->pfnRcRxAckIrqCb    = NULL;
    pstcRcInternData->pfnRcRxCntOvfIrqCb = NULL;
    pstcRcInternData->pfnRcRxStartIrqCb  = NULL;
    
    // Disable NVIC
    if(TRUE == bTouchNvic)
    {
        RcDeInitNvic();
    }
#endif        

    return Ok;
}

/**
 ******************************************************************************
 ** \brief Enable SIRCS mode reception of Remote Control Reception
 **
 ** This function enables reception funciton of SIRCS mode
 **
 ** \param [in]  pstcRc           RC instance
 **
 ** \retval Ok                    SIRCS mode reception enabled normally
 ** \retval ErrorInvalidParameter pstcRc == NULL
 ******************************************************************************/
en_result_t Rc_Rx_Sircs_EnableRx(volatile stc_rcn_t *pstcRc)
{
    // Check for NULL pointers
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
    
    pstcRc->RCCR_f.EN = 1u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable SIRCS mode reception of Remote Control Reception
 **
 ** This function disables reception funciton of SIRCS mode
 **
 ** \param [in]  pstcRc           RC instance
 **
 ** \retval Ok                    SIRCS mode reception disabled normally
 ** \retval ErrorInvalidParameter pstcRc == NULL
 ******************************************************************************/
en_result_t Rc_Rx_Sircs_DisableRx(volatile stc_rcn_t *pstcRc)
{
    // Check for NULL pointers
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
    
    pstcRc->RCCR_f.EN = 0u;
    
    return Ok;
}

#if (PDL_INTERRUPT_ENABLE_RC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_RC1 == PDL_ON) 
/**
 ******************************************************************************
 ** \brief Enable interrupt of Remote Control Reception with SIRCS mode
 **
 ** This function enable the interrupt selected of Remote Control Reception 
 ** SIRCS mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  enIrqSel         RC Reception SIRCS mode interrupt types
 **
 ** \retval Ok                    Interrupt enabled normally
 ** \retval ErrorInvalidParameter If one of following cases matchs:
 **                               - pstcRc == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Rc_Rx_Sircs_EnableIrq(volatile stc_rcn_t *pstcRc, 
                                  en_rc_rx_sircs_irq_sel_t enIrqSel)
{
    // Check for NULL pointers
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
    
    switch (enIrqSel)
    {
        case RcRxSircsStartIrq:
            pstcRc->RCST_f.STIE = 1u;
            break;
        case RcRxSircsAckIrq:
            pstcRc->RCST_f.ACKIE = 1u;
            break;
        case RcRxSircsCntOvfIrq:
            pstcRc->RCST_f.OVFIE = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable interrupt of Remote Control Reception with SIRCS mode
 **
 ** This function disable the interrupt selected of Remote Control Reception 
 ** SIRCS mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  enIrqSel         RC Reception SIRCS mode interrupt types
 **
 ** \retval Ok                    Interrupt disabled normally
 ** \retval ErrorInvalidParameter If one of following cases matchs:
 **                               - pstcRc == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Rc_Rx_Sircs_DisableIrq(volatile stc_rcn_t *pstcRc, 
                                   en_rc_rx_sircs_irq_sel_t enIrqSel)
{
    // Check for NULL pointers
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
    
    switch (enIrqSel)
    {
        case RcRxSircsStartIrq:
            pstcRc->RCST_f.STIE = 0u;
            break;
        case RcRxSircsAckIrq:
            pstcRc->RCST_f.ACKIE = 0u;
            break;
        case RcRxSircsCntOvfIrq:
            pstcRc->RCST_f.OVFIE = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

#endif

/**
 ******************************************************************************
 ** \brief Get interrupt flag of Remote Control Reception with SIRCS mode
 **
 ** This function gets the interrupt flag status of SIRCS mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  enIrqSel         RC Reception SIRCS mode interrupt types
 **
 ** \retval TRUE                  The interrupt flag selected is set
 ** \retval ErrorInvalidParameter The interrupt flag selected is clear
 ******************************************************************************/
boolean_t Rc_Rx_Sircs_GetIrqFlag(volatile stc_rcn_t *pstcRc, 
                                 en_rc_rx_sircs_irq_sel_t enIrqSel)
{
    boolean_t bRet = FALSE;
    
    switch (enIrqSel)
    {
        case RcRxSircsStartIrq:
            bRet = ((1u == pstcRc->RCST_f.ST) ? TRUE : FALSE);
            break;
        case RcRxSircsAckIrq:
            bRet = ((1u == pstcRc->RCST_f.ACK) ? TRUE : FALSE);
            break;
        case RcRxSircsCntOvfIrq:
            bRet = ((1u == pstcRc->RCST_f.OVF) ? TRUE : FALSE);
            break;
    }
    
    return bRet;
}

/**
 ******************************************************************************
 ** \brief Clear interrupt flag of Remote Control Reception with SIRCS mode
 **
 ** This function clears the interrupt flag status of SIRCS mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  enIrqSel         RC Reception SIRCS mode interrupt types
 **
 ** \retval Ok                    The interrupt flag selected is clear normally
 ** \retval ErrorInvalidParameter If one of following cases matchs:
 **                               - NULL == pstcRc
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Rc_Rx_Sircs_ClrIrqFlag(volatile stc_rcn_t *pstcRc, 
                                   en_rc_rx_sircs_irq_sel_t enIrqSel)
{
    // Check for NULL pointers
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
    
    switch (enIrqSel)
    {
        case RcRxSircsStartIrq:
            pstcRc->RCST_f.ST = 0u;
            break;
        case RcRxSircsAckIrq:
            pstcRc->RCST_f.ACK = 0u;
            break;
        case RcRxSircsCntOvfIrq:
            pstcRc->RCST_f.OVF = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get data of Remote Control Reception with SIRCS mode
 **
 ** This function reads the data from data buffer in SIRCS mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  pstcData         Poitner to SIRCS mode data structure
 **
 ** \retval Ok                    Data is read normally
 ** \retval ErrorInvalidParameter NULL == pstcRc
 ******************************************************************************/
en_result_t Rc_Rx_Sircs_ReadData(volatile stc_rcn_t *pstcRc, 
                                 rc_rx_sircs_data_t* pstcData)
{
    // Check for NULL pointers
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
        
    pstcData->u8Command = pstcRc->RCDTHH; 
    pstcData->u8DeviceAddress = pstcRc->RCDTHL; 
    pstcData->u8Data0 =  pstcRc->RCDTLH;
    pstcData->u8Data1 =  pstcRc->RCDTLL;
    
    return Ok;
}
#endif //#if (PDL_PERIPHERAL_ENABLE_RCRX_SIRCS_MODE == PDL_ON) 

#if (PDL_PERIPHERAL_ENABLE_RCRX_NEC_MODE == PDL_ON) 
/**
 ******************************************************************************
 ** \brief Initialize NEC mode of Remote Control Reception
 **
 ** This function initializes NEC mode of an Remote Control Reception
 **
 ** \param [in]  pstcRc              RC instance
 ** \param [in]  pstcRcNecConfig     Pointer to NEC mode configuration 
 **
 ** \retval Ok                    NEC mode initialized normally
 ** \retval ErrorInvalidParameter If one of following cases matchs: 
 **                               - pstcRc == NULL
 **                               - pstcRcNecConfig == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Rc_Rx_Nec_Init(volatile stc_rcn_t *pstcRc, 
                           const stc_rc_rx_nec_config_t *pstcRcNecConfig)
{
    // Pointer to internal data
    stc_rc_intern_data_t* pstcRcInternData ; 
  
    // Check for NULL pointers
    if ( (NULL == pstcRcNecConfig) || 
         (NULL == pstcRc) )
    {
        return ErrorInvalidParameter;
    }
    
    // Get pointer to internal data structure ...
    pstcRcInternData = RcGetInternDataPtr( pstcRc ) ;
    
    // Check for instance available or not
    if(NULL == pstcRcInternData)
    {
        return ErrorInvalidParameter ;
    }
    
    // Set mode
    pstcRc->RCCR_f.MOD = RcNecMode;                  
    
    // Set clock division
    pstcRc->RCCKD = pstcRcNecConfig->u16DivVal;    
    
    // Set source clock
    switch (pstcRcNecConfig->enSrcClk)               
    {
        case RcPeripheralClk:
            pstcRc->RCCKD_f.CKSEL = RcPeripheralClk;    
            break;
        case RcSubClk:
            pstcRc->RCCKD_f.CKSEL = RcSubClk;          
            break;
        default:
           return ErrorInvalidParameter;
    }
    
    // Set threshold type
    switch (pstcRcNecConfig->enThresholdType)        
    {
        case RcThresholdType0:
            pstcRc->RCCR_f.THSEL = 0u;
            break;
        case RcThresholdType1:
            pstcRc->RCCR_f.THSEL = 1u;
            break;
        default:
           return ErrorInvalidParameter;
    }
    
    // Set overflow type
    switch (pstcRcNecConfig->enOverflowCycle)      
    {
        case RcOverflow128Cycle:
            pstcRc->RCST_f.OVFSEL = 0u;
            break;
        case RcOverflow256Cycle:
            pstcRc->RCST_f.OVFSEL = 1u;
            break;
        default:
           return ErrorInvalidParameter;
    }
    
    // Set address compare enable
    pstcRc->RCCR_f.ADRCE = ((TRUE == pstcRcNecConfig->bAddrCmpEn) ? 1u : 0u);
    
    // Set start bit, minimum pulse, threshold width
    pstcRc->RCSHW = pstcRcNecConfig->u8StartBitWidth;       
    pstcRc->RCDAHW = pstcRcNecConfig->u8MinPulseWidth;      
    pstcRc->RCDBHW = pstcRcNecConfig->u8ThresholdWidth;
    
    // Set compare address
    pstcRc->RCADR1 = pstcRcNecConfig->stcAddr.u8Addr1;     
    pstcRc->RCADR2 = pstcRcNecConfig->stcAddr.u8Addr2;    
    
#if (PDL_INTERRUPT_ENABLE_RC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_RC1 == PDL_ON)   
    // Set interrupt enable
    if(NULL != pstcRcNecConfig->pstcIrqEn)
    {
        if(TRUE == pstcRcNecConfig->pstcIrqEn->bRcRxNecStartIrq)
        {
            pstcRc->RCST_f.STIE = 1u;
        }   
        if(TRUE == pstcRcNecConfig->pstcIrqEn->bRcRxNecAckIrq)
        {
            pstcRc->RCST_f.ACKIE = 1u;
        }
        if(TRUE == pstcRcNecConfig->pstcIrqEn->bRcRxNecCntOvfIrq)
        {
            pstcRc->RCST_f.OVFIE = 1u;
        }
        if(TRUE == pstcRcNecConfig->pstcIrqEn->bRcRxNecRepeatCodeIrq)
        {
            pstcRc->RCRC_f.RCIE = 1u;
        }
    }
    
    // Set interrupt callback functions
    if(NULL != pstcRcNecConfig->pstcIrqEn)
    {
        pstcRcInternData->pfnRcRxStartIrqCb = pstcRcNecConfig->pstcIrqCb->pfnRcRxNecStartIrqCb;
        pstcRcInternData->pfnRcRxAckIrqCb = pstcRcNecConfig->pstcIrqCb->pfnRcRxNecAckIrqCb;
        pstcRcInternData->pfnRcRxCntOvfIrqCb = pstcRcNecConfig->pstcIrqCb->pfnRcRxNecCntOvfIrqCb;
        pstcRcInternData->pfnRcRxRepeatCodeIrqCb = pstcRcNecConfig->pstcIrqCb->pfnRcRxNecRepeatCodeIrqCb;
    }
    
    // Set NVIC enable
    if(TRUE == pstcRcNecConfig->bTouchNvic)
    {
        RcInitNvic();
    }
#endif
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief De-Initialize NEC mode of Remote Control Reception
 **
 ** This function deinitializes an Remote Control Reception with NEC mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  bTouchNvic       Touch NVIC or not
 **
 ** \retval Ok                    RC instance with NEC mode de-initialized 
 ** \retval ErrorInvalidParameter pstcRc == NULL
 ******************************************************************************/
en_result_t Rc_Rx_Nec_DeInit(volatile stc_rcn_t *pstcRc, boolean_t bTouchNvic)
{
    // Pointer to internal data
    stc_rc_intern_data_t* pstcRcInternData ; 
  
    // Check for NULL pointers
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
    
    // Get pointer to internal data structure ...
    pstcRcInternData = RcGetInternDataPtr( pstcRc ) ;
    
    // Check for instance available or not
    if(NULL == pstcRcInternData)
    {
        return ErrorInvalidParameter ;
    }

    // Clear all registers 
    pstcRc->RCCR = 0u;                           
    pstcRc->RCST = 0u;
    pstcRc->RCSHW = 0u;
    pstcRc->RCDAHW = 0u;
    pstcRc->RCDBHW = 0u;
    pstcRc->RCADR1 = 0u;
    pstcRc->RCADR2 = 0u;
    pstcRc->RCDTHH = 0u;
    pstcRc->RCDTHL = 0u;
    pstcRc->RCDTLH = 0u;
    pstcRc->RCDTLL = 0u;
    pstcRc->RCCKD = 0u;
    pstcRc->RCRC = 0u;
    pstcRc->RCRHW = 0u;
    pstcRc->RCLE = 0u;
    pstcRc->RCLESW = 0u;
    pstcRc->RCLELW = 0u;

#if (PDL_INTERRUPT_ENABLE_RC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_RC1 == PDL_ON)  
    // Clear callback functions  
    pstcRcInternData->pfnRcRxAckIrqCb = NULL;
    pstcRcInternData->pfnRcRxCntOvfIrqCb = NULL;
    pstcRcInternData->pfnRcRxStartIrqCb = NULL;
    pstcRcInternData->pfnRcRxRepeatCodeIrqCb = NULL;
    
    // Set NVIC disable
    if(TRUE == bTouchNvic)
    {
        RcDeInitNvic();
    }
#endif        

    return Ok;
}

/**
 ******************************************************************************
 ** \brief Enable NEC mode reception of Remote Control Reception
 **
 ** This function enables reception funciton of NEC mode
 **
 ** \param [in]  pstcRc           RC instance
 **
 ** \retval Ok                    NEC mode reception enabled normally
 ** \retval ErrorInvalidParameter pstcRc == NULL
 ******************************************************************************/
en_result_t Rc_Rx_Nec_EnableRx(volatile stc_rcn_t *pstcRc)
{
    // Check for NULL pointers
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
    
    pstcRc->RCCR_f.EN = 1u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable NEC mode reception of Remote Control Reception
 **
 ** This function disables reception funciton of NEC mode
 **
 ** \param [in]  pstcRc           RC instance
 **
 ** \retval Ok                    NEC mode reception disabled normally
 ** \retval ErrorInvalidParameter pstcRc == NULL
 ******************************************************************************/
en_result_t Rc_Rx_Nec_DisableRx(volatile stc_rcn_t *pstcRc)
{
    // Check for NULL pointers
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
    
    pstcRc->RCCR_f.EN = 0u;
    
    return Ok;
}

#if (PDL_INTERRUPT_ENABLE_RC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_RC1 == PDL_ON) 
/**
 ******************************************************************************
 ** \brief Enable interrupt of Remote Control Reception with NEC mode
 **
 ** This function enable the interrupt selected of Remote Control Reception 
 ** NEC mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  enIrqSel         RC Reception NEC mode interrupt types
 **
 ** \retval Ok                    Interrupt enabled normally
 ** \retval ErrorInvalidParameter If one of following cases matchs: 
 **                               - pstcRc == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Rc_Rx_Nec_EnableIrq(volatile stc_rcn_t *pstcRc, 
                                en_rc_rx_nec_irq_sel_t enIrqSel)
{
    // Check for NULL pointers
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
    
    switch (enIrqSel)
    {
        case RcRxNecStartIrq:
            pstcRc->RCST_f.STIE = 1u;
            break;
        case RcRxNecAckIrq:
            pstcRc->RCST_f.ACKIE = 1u;
            break;
        case RcRxNecCntOvfIrq:
            pstcRc->RCST_f.OVFIE = 1u;
            break;
        case RcRxNecRepeatCodeIrq:
            pstcRc->RCRC_f.RCIE = 1u;
            break;    
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable interrupt of Remote Control Reception with NEC mode
 **
 ** This function disable the interrupt selected of Remote Control Reception 
 ** NEC mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  enIrqSel         RC Reception NEC mode interrupt types
 **
 ** \retval Ok                    Interrupt disabled normally
 ** \retval ErrorInvalidParameter If one of following cases matchs: 
 **                               - pstcRc == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Rc_Rx_Nec_DisableIrq(volatile stc_rcn_t *pstcRc, 
                                 en_rc_rx_nec_irq_sel_t enIrqSel)
{
    // Check for NULL pointers
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
    
    switch (enIrqSel)
    {
        case RcRxNecStartIrq:
            pstcRc->RCST_f.STIE = 0u;
            break;
        case RcRxNecAckIrq:
            pstcRc->RCST_f.ACKIE = 0u;
            break;
        case RcRxNecCntOvfIrq:
            pstcRc->RCST_f.OVFIE = 0u;
            break;
        case RcRxNecRepeatCodeIrq:
            pstcRc->RCRC_f.RCIE = 0u;
            break;      
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

#endif

/**
 ******************************************************************************
 ** \brief Get interrupt flag of Remote Control Reception with NEC mode
 **
 ** This function gets the interrupt flag status of NEC mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  enIrqSel         RC Reception NEC mode interrupt types
 **
 ** \retval TRUE                  The interrupt flag selected is set
 ** \retval ErrorInvalidParameter The interrupt flag selected is clear
 ******************************************************************************/
boolean_t Rc_Rx_Nec_GetIrqFlag(volatile stc_rcn_t *pstcRc, 
                               en_rc_rx_nec_irq_sel_t enIrqSel)
{
    boolean_t bRet = FALSE;
    
    switch (enIrqSel)
    {
        case RcRxNecStartIrq:
            bRet = ((1u == pstcRc->RCST_f.ST) ? TRUE : FALSE);
            break;
        case RcRxNecAckIrq:
            bRet = ((1u == pstcRc->RCST_f.ACK) ? TRUE : FALSE);
            break;
        case RcRxNecCntOvfIrq:
            bRet = ((1u == pstcRc->RCST_f.OVF) ? TRUE : FALSE);
            break;
        case RcRxNecRepeatCodeIrq:
            bRet = ((1u == pstcRc->RCRC_f.RC) ? TRUE : FALSE);
            break;    
    }
    
    return bRet;
}

/**
 ******************************************************************************
 ** \brief Clear interrupt flag of Remote Control Reception with NEC mode
 **
 ** This function clears the interrupt flag status of NEC mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  enIrqSel         RC Reception SIRCS mode interrupt types
 **
 ** \retval Ok                    The interrupt flag selected is clear normally
 ** \retval ErrorInvalidParameter If one of following cases matchs: 
 **                               - NULL == pstcRc
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Rc_Rx_Nec_ClrIrqFlag(volatile stc_rcn_t *pstcRc, 
                                 en_rc_rx_nec_irq_sel_t enIrqSel)
{
    // Check for NULL pointers
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
    
    switch (enIrqSel)
    {
        case RcRxNecStartIrq:
            pstcRc->RCST_f.ST = 0u;
            break;
        case RcRxNecAckIrq:
            pstcRc->RCST_f.ACK = 0u;
            break;
        case RcRxNecCntOvfIrq:
            pstcRc->RCST_f.OVF = 0u;
            break;
        case RcRxNecRepeatCodeIrq:
            pstcRc->RCRC_f.RC = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get data of Remote Control Reception with NEC mode
 **
 ** This function reads the data from data buffer in NEC mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  pstcData         Poitner to NEC mode data structure
 **
 ** \retval Ok                    Data is read normally
 ** \retval ErrorInvalidParameter NULL == pstcRc
 ******************************************************************************/
en_result_t Rc_Rx_Nec_ReadData(volatile stc_rcn_t *pstcRc, 
                               rc_rx_nec_data_t* pstcData)
{
  
    // Check for NULL pointers
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
        
    pstcData->u16CustomCode = pstcRc->RCDTHH ; 
    pstcData->u16CustomCode |= (pstcRc->RCDTHL << 8u); 
    pstcData->u8Data0 = pstcRc->RCDTLH; 
    pstcData->u8Data1 = pstcRc->RCDTLL;
    
    return Ok;
}
#endif  //#if (PDL_PERIPHERAL_ENABLE_RCRX_NEC_MODE == PDL_ON)

#if (PDL_PERIPHERAL_ENABLE_RCRX_CEC_MODE == PDL_ON) 
/**
 ******************************************************************************
 ** \brief Initialize CEC mode of Remote Control Reception
 **
 ** This function initializes CEC mode of an Remote Control Reception
 **
 ** \param [in]  pstcRc              RC instance
 ** \param [in]  pstcRcCecConfig     Pointer to CEC mode configuration 
 **
 ** \retval Ok                    CEC mode initialized normally
 ** \retval ErrorInvalidParameter If one of following cases matchs: 
 **                               - pstcRc == NULL
 **                               - pstcRcCecConfig == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Rc_Rx_Cec_Init(volatile stc_rcn_t *pstcRc, 
                           const stc_rc_rx_cec_config_t *pstcRcCecConfig)
{
    // Pointer to internal data
    stc_rc_intern_data_t* pstcRcInternData ; 
  
    // Check for NULL pointers
    if ( (NULL == pstcRcCecConfig) || 
         (NULL == pstcRc) )
    {
        return ErrorInvalidParameter;
    }
    
    // Get pointer to internal data structure ...
    pstcRcInternData = RcGetInternDataPtr( pstcRc ) ;
    
    // Check for instance available or not
    if(NULL == pstcRcInternData)
    {
        return ErrorInvalidParameter ;
    }
    
    // Set mode
    pstcRc->RCCR_f.MOD = RcCecMode;                 
    
    // Set clock division
    pstcRc->RCCKD = pstcRcCecConfig->u16DivVal;       
    
    // Set source clock
    switch (pstcRcCecConfig->enSrcClk)               
    {
        case RcPeripheralClk:
            pstcRc->RCCKD_f.CKSEL = RcPeripheralClk;   
            break;
        case RcSubClk:
            pstcRc->RCCKD_f.CKSEL = RcSubClk;         
            break;
        default:
           return ErrorInvalidParameter;
    }
    
    // Set threshold type
    switch (pstcRcCecConfig->enThresholdType)       
    {
        case RcThresholdType0:
            pstcRc->RCCR_f.THSEL = 0u;
            break;
        case RcThresholdType1:
            pstcRc->RCCR_f.THSEL = 1u;
            break;
        default:
           return ErrorInvalidParameter;
    }
    
    // Set overflow type
    switch (pstcRcCecConfig->enOverflowCycle)        
    {
        case RcOverflow128Cycle:
            pstcRc->RCST_f.OVFSEL = 0u;
            break;
        case RcOverflow256Cycle:
            pstcRc->RCST_f.OVFSEL = 1u;
            break;
        default:
           return ErrorInvalidParameter;
    }

    
    // Set address compare enable
    pstcRc->RCCR_f.ADRCE = ((TRUE == pstcRcCecConfig->bAddrCmpEn) ? 1u : 0u);

    // Set maximum bit violation detection
    pstcRc->RCLE_f.LELE = ((TRUE == pstcRcCecConfig->u8MaxDataWidth) ? 1u : 0u);
    
    // Set minimum bit violation detection
    pstcRc->RCLE_f.LESE = ((TRUE == pstcRcCecConfig->u8MinDataWidth) ? 1u : 0u);
    
    // Set error pulse output violation detection
    pstcRc->RCLE_f.EPE = ((pstcRcCecConfig->bBusErrorPulseOutput) ? 1u : 0u); 
    
    // Set address
    pstcRc->RCADR1 = pstcRcCecConfig->stcAddr.u8Addr1;
    pstcRc->RCADR2 = pstcRcCecConfig->stcAddr.u8Addr2;
    
    // Set start bit, minimum pulse, threshold width
    pstcRc->RCSHW = pstcRcCecConfig->u8StartBitWidth; 
    pstcRc->RCDAHW = pstcRcCecConfig->u8MinPulseWidth;
    pstcRc->RCDBHW = pstcRcCecConfig->u8ThresholdWidth;
    pstcRc->RCLELW = pstcRcCecConfig->u8MaxDataWidth;
    pstcRc->RCLESW = pstcRcCecConfig->u8MinDataWidth;
    
#if (PDL_INTERRUPT_ENABLE_RC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_RC1 == PDL_ON)   
    // Set interrupt enable
    if (NULL != pstcRcCecConfig->pstcIrqEn)
    {
        if(TRUE == pstcRcCecConfig->pstcIrqEn->bRcRxCecStartIrq)
        {
            pstcRc->RCST_f.STIE = 1u;
        }
        
        if(TRUE == pstcRcCecConfig->pstcIrqEn->bRcRxCecAckIrq)
        {
            pstcRc->RCST_f.ACKIE = 1u;
        }
        
        if(TRUE == pstcRcCecConfig->pstcIrqEn->bRcRxCecCntOvfIrq)
        {
            pstcRc->RCST_f.OVFIE = 1u;
        }
        
        if(TRUE == pstcRcCecConfig->pstcIrqEn->bRcRxCecMaxDataIrq)
        {
            pstcRc->RCLE_f.LELIE = 1u;
        }
        
        if(TRUE == pstcRcCecConfig->pstcIrqEn->bRcRxCecMinDataIrq)
        {
            pstcRc->RCLE_f.LESIE = 1u;
        }
    }
    
    // Set interrupt callback function
    if (NULL != pstcRcCecConfig->pstcIrqCb)
    {
        pstcRcInternData->pfnRcRxStartIrqCb = pstcRcCecConfig->pstcIrqCb->pfnRcRxCecStartIrqCb;
        pstcRcInternData->pfnRcRxAckIrqCb = pstcRcCecConfig->pstcIrqCb->pfnRcRxCecAckIrqCb;
        pstcRcInternData->pfnRcRxCntOvfIrqCb = pstcRcCecConfig->pstcIrqCb->pfnRcRxCecCntOvfIrqCb;
        pstcRcInternData->pfnRcRxMaxDataIrqCb = pstcRcCecConfig->pstcIrqCb->pfnRcRxCecMaxDataIrqCb;
        pstcRcInternData->pfnRcRxMinDataIrqCb = pstcRcCecConfig->pstcIrqCb->pfnRcRxCecMinDataIrqCb;
    }
    
    // Set NVIC enable
    if(TRUE == pstcRcCecConfig->bTouchNvic)
    {
        RcInitNvic();
    }
#endif
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief De-Initialize CEC mode of Remote Control Reception
 **
 ** This function deinitializes an Remote Control Reception with CEC mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  bTouchNvic       Touch NVIC or not
 **
 ** \retval Ok                    RC instance with CEC mode de-initialized 
 ** \retval ErrorInvalidParameter pstcRc == NULL
 ******************************************************************************/
en_result_t Rc_Rx_Cec_DeInit(volatile stc_rcn_t *pstcRc, boolean_t bTouchNvic)
{
    // Pointer to internal data
    stc_rc_intern_data_t* pstcRcInternData ; 
  
    // Check for NULL pointers
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
    
    // Get pointer to internal data structure ...
    pstcRcInternData = RcGetInternDataPtr( pstcRc ) ;
    
    // Check for instance available or not
    if(pstcRcInternData == NULL)
    {
        return ErrorInvalidParameter ;
    }

    /* Clear all registers */
    pstcRc->RCCR = 0u;                           
    pstcRc->RCST = 0u;
    pstcRc->RCSHW = 0u;
    pstcRc->RCDAHW = 0u;
    pstcRc->RCDBHW = 0u;
    pstcRc->RCADR1 = 0u;
    pstcRc->RCADR2 = 0u;
    pstcRc->RCDTHH = 0u;
    pstcRc->RCDTHL = 0u;
    pstcRc->RCDTLH = 0u;
    pstcRc->RCDTLL = 0u;
    pstcRc->RCCKD = 0u;
    pstcRc->RCRC = 0u;
    pstcRc->RCRHW = 0u;
    pstcRc->RCLE = 0u;
    pstcRc->RCLESW = 0u;
    pstcRc->RCLELW = 0u;

#if (PDL_INTERRUPT_ENABLE_RC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_RC1 == PDL_ON)  
    // Clear callback 
    pstcRcInternData->pfnRcRxStartIrqCb = NULL;  
    pstcRcInternData->pfnRcRxAckIrqCb = NULL;
    pstcRcInternData->pfnRcRxCntOvfIrqCb = NULL;
    pstcRcInternData->pfnRcRxMaxDataIrqCb = NULL;
    pstcRcInternData->pfnRcRxMinDataIrqCb = NULL;
    
    // Set NVIC disable
    if(TRUE == bTouchNvic)
    {
        RcDeInitNvic();
    }
#endif        

    return Ok;  
}

/**
 ******************************************************************************
 ** \brief Enable CEC mode reception of Remote Control Reception
 **
 ** This function enables reception funciton of CEC mode
 **
 ** \param [in]  pstcRc           RC instance
 **
 ** \retval Ok                    CEC mode reception enabled normally
 ** \retval ErrorInvalidParameter pstcRc == NULL
 ******************************************************************************/
en_result_t Rc_Rx_Cec_EnableRx(volatile stc_rcn_t *pstcRc)
{
    // Check for NULL pointers
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
    
    pstcRc->RCCR_f.EN = 1u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable CEC mode reception of Remote Control Reception
 **
 ** This function disables reception funciton of CEC mode
 **
 ** \param [in]  pstcRc           RC instance
 **
 ** \retval Ok                    CEC mode reception disabled normally
 ** \retval ErrorInvalidParameter pstcRc == NULL
 ******************************************************************************/
en_result_t Rc_Rx_Cec_DisableRx(volatile stc_rcn_t *pstcRc)
{
    // Check for NULL pointers
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
    
    pstcRc->RCCR_f.EN = 0u;
    
    return Ok;
}

#if (PDL_INTERRUPT_ENABLE_RC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_RC1 == PDL_ON) 
/**
 ******************************************************************************
 ** \brief Enable interrupt of Remote Control Reception with CEC mode
 **
 ** This function enable the interrupt selected of Remote Control Reception 
 ** CEC mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  enIrqSel         RC Reception CEC mode interrupt types
 **
 ** \retval Ok                    Interrupt enabled normally
 ** \retval ErrorInvalidParameter If one of following cases matchs: 
 **                               - pstcRc == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Rc_Rx_Cec_EnableIrq(volatile stc_rcn_t *pstcRc, 
                                en_rc_rx_cec_irq_sel_t enIrqSel)
{
    // Check for NULL pointers
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
    
    switch (enIrqSel)
    {
        case RcRxCecStartIrq:
            pstcRc->RCST_f.STIE = 1u;
            break;
        case RcRxCecAckIrq:
            pstcRc->RCST_f.ACKIE = 1u;
            break;
        case RcRxCecCntOvfIrq:
            pstcRc->RCST_f.OVFIE = 1u;
            break;
        case RcRxCecMaxDataIrq:
            pstcRc->RCLE_f.LELIE = 1u;
            break;
        case RcRxCecMinDataIrq:
            pstcRc->RCLE_f.LESIE = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable interrupt of Remote Control Reception with CEC mode
 **
 ** This function disable the interrupt selected of Remote Control Reception 
 ** CEC mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  enIrqSel         RC Reception CEC mode interrupt types
 **
 ** \retval Ok                    Interrupt disabled normally
 ** \retval ErrorInvalidParameter If one of following cases matchs: 
 **                               - pstcRc == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Rc_Rx_Cec_DisableIrq(volatile stc_rcn_t *pstcRc, 
                                 en_rc_rx_cec_irq_sel_t enIrqSel)
{
    // Check for NULL pointers
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
    
    switch (enIrqSel)
    {
        case RcRxCecStartIrq:
            pstcRc->RCST_f.STIE = 0u;
            break;
        case RcRxCecAckIrq:
            pstcRc->RCST_f.ACKIE = 0u;
            break;
        case RcRxCecCntOvfIrq:
            pstcRc->RCST_f.OVFIE = 0u;
            break;
        case RcRxCecMaxDataIrq:
            pstcRc->RCLE_f.LELIE = 0u;
            break;
        case RcRxCecMinDataIrq:
            pstcRc->RCLE_f.LESIE = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

#endif

/**
 ******************************************************************************
 ** \brief Get interrupt flag of Remote Control Reception with CEC mode
 **
 ** This function gets the interrupt flag status of CEC mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  enIrqSel         RC Reception CEC mode interrupt types
 **
 ** \retval TRUE                  The interrupt flag selected is set
 ** \retval ErrorInvalidParameter The interrupt flag selected is clear
 ******************************************************************************/
boolean_t Rc_Rx_Cec_GetIrqFlag(volatile stc_rcn_t *pstcRc, 
                               en_rc_rx_cec_irq_sel_t enIrqSel)
{
    boolean_t bRet = FALSE;
    
    switch (enIrqSel)
    {
        case RcRxCecStartIrq:
            bRet = ((1u == pstcRc->RCST_f.ST) ? TRUE : FALSE);
            break;
        case RcRxCecAckIrq:
            bRet = ((1u == pstcRc->RCST_f.ACK) ? TRUE : FALSE);
            break;
        case RcRxCecCntOvfIrq:
            bRet = ((1u == pstcRc->RCST_f.OVF) ? TRUE : FALSE);
            break;
        case RcRxCecMaxDataIrq:
            bRet = ((1u == pstcRc->RCLE_f.LEL) ? TRUE : FALSE);
            break;
        case RcRxCecMinDataIrq:
            bRet = ((1u == pstcRc->RCLE_f.LES) ? TRUE : FALSE);
            break;    
    }
    
    return bRet;
}

/**
 ******************************************************************************
 ** \brief Clear interrupt flag of Remote Control Reception with CEC mode
 **
 ** This function clears the interrupt flag status of CEC mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  enIrqSel         RC Reception CEC mode interrupt types
 **
 ** \retval Ok                    The interrupt flag selected is clear normally
 ** \retval ErrorInvalidParameter If one of following cases matchs: 
 **                               - NULL == pstcRc
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Rc_Rx_Cec_ClrIrqFlag(volatile stc_rcn_t *pstcRc, 
                                 en_rc_rx_cec_irq_sel_t enIrqSel)
{
    // Check for NULL pointers
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
    
    switch (enIrqSel)
    {
        case RcRxCecStartIrq:
            pstcRc->RCST_f.ST = 0u;
            break;
        case RcRxCecAckIrq:
            pstcRc->RCST_f.ACK = 0u;
            break;
        case RcRxCecCntOvfIrq:
            pstcRc->RCST_f.OVF = 0u;
            break;
        case RcRxCecMaxDataIrq:
            pstcRc->RCLE_f.LEL = 0u;
            break;
        case RcRxCecMinDataIrq:
            pstcRc->RCLE_f.LES = 0u;
            break;    
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get data of Remote Control Reception with CEC mode
 **
 ** This function reads the data from data buffer in CEC mode
 **
 ** \param [in]  pstcRc           RC instance
 **
 ** \retval CEC data              CEC data
 ******************************************************************************/
uint8_t Rc_Rx_Cec_ReadData(volatile stc_rcn_t *pstcRc)
{
    // Check for NULL pointers
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
        
    return pstcRc->RCDTHH;
}

/**
 ******************************************************************************
 ** \brief Get EOM state of Remote Control Reception with CEC mode
 **
 ** This function reads the data from data buffer in CEC mode
 **
 ** \param [in]  pstcRc           RC instance
 **
 ** \retval True                    that's end of data
 ** \retval Flase                   that isn't end of data
 ******************************************************************************/
boolean_t Rc_Rx_Cec_GetEomState(volatile stc_rcn_t *pstcRc)
{
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
    return pstcRc->RCST_f.EOM;
    
}

/**
 ******************************************************************************
 ** \brief clear EOM state of Remote Control Reception with CEC mode
 **
 ** This function reads the data from data buffer in CEC mode
 **
 ** \param [in]  pstcRc           RC instance
 **
 ** \retval True                    that's end of data
 ** \retval Flase                   that isn't end of data
 ******************************************************************************/
en_result_t Rc_Rx_Cec_ClrEomState(volatile stc_rcn_t *pstcRc)
{
    if (NULL == pstcRc)
    {
        return ErrorInvalidParameter;
    }
    pstcRc->RCST_f.EOM = 0u;
    return Ok;
}
#endif

#if (PDL_PERIPHERAL_ENABLE_RCTX_CEC_MODE == PDL_ON)
/**
 ******************************************************************************
 ** \brief Initialize CEC mode of Remote Control transmission
 **
 ** This function initializes CEC mode of an Remote Control transmission
 **
 ** \param [in]  pstcRc              RC instance
 ** \param [in]  pstcRcTxConfig      Pointer to TX CEC mode configuration 
 **
 ** \retval Ok                    CEC mode initialized normally
 ** \retval ErrorInvalidParameter If one of following cases matchs: 
 **                               - pstcRc == NULL
 **                               - pstcRcTxConfig == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Rc_Tx_Cec_Init(volatile stc_rcn_t *pstcRc, 
                           const stc_rc_tx_cec_config_t *pstcRcTxConfig)
{
    // Pointer to internal data
    stc_rc_intern_data_t* pstcRcInternData ; 
  
    // Get pointer to internal data structure ...
    pstcRcInternData = RcGetInternDataPtr( pstcRc ) ;
    
    // Check for instance available or not
    if(NULL == pstcRcInternData)
    {
        return ErrorInvalidParameter ;
    }

    // Check for NULL pointers
    if ( (NULL == pstcRcTxConfig) || 
         (NULL == pstcRc) )
    {
        return ErrorInvalidParameter;
    }
    
    // Set clock division
    pstcRc->RCCKD = pstcRcTxConfig->u16DivVal;       
    
    // Set source clock
    switch (pstcRcTxConfig->enSrcClk)               
    {
        case RcPeripheralClk:
            pstcRc->RCCKD_f.CKSEL = RcPeripheralClk;   
            break;
        case RcSubClk:
            pstcRc->RCCKD_f.CKSEL = RcSubClk;         
            break;
        default:
           return ErrorInvalidParameter;
    }
    
    
    if (pstcRcTxConfig->u8FreeCycle > 15u)
    {
        return ErrorInvalidParameter ;
    }
    
    pstcRc->SFREE = pstcRcTxConfig->u8FreeCycle;
   
#if (PDL_INTERRUPT_ENABLE_RC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_RC1 == PDL_ON)   
    // Set interrupt enable  
    if (NULL != pstcRcTxConfig->pstcIrqEn)
    {
        if(TRUE == pstcRcTxConfig->pstcIrqEn->bRcTxCecBusErrorIrq)
        {
            pstcRc->TXCTRL_f.IBREN = 1u;
        }   
        if(TRUE == pstcRcTxConfig->pstcIrqEn->bRcTxCecStatusIrq)
        {
            pstcRc->TXCTRL_f.ITSTEN = 1u;
        }
    }
    
    // Set interrupt callback function  
    if (NULL != pstcRcTxConfig->pstcIrqCb)
    {
        pstcRcInternData->pfnRcTxIrqBusErrorCb = pstcRcTxConfig->pstcIrqCb->pfnRcTxIrqBusErrorCb;
        pstcRcInternData->pfnRcTxIrqStatusCb = pstcRcTxConfig->pstcIrqCb->pfnRcTxIrqTxStatusCb;
    }
    
    // Set NVIC enable
    if(TRUE == pstcRcTxConfig->bTouchNvic)
    {
        RcInitNvic();
    }
    
#endif
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief De-Initialize CEC mode of Remote Control transmission
 **
 ** This function deinitializes an Remote Control transmission with CEC mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  bTouchNvic       Touch NVIC or not
 **
 ** \retval Ok                    RC instance with CEC mode de-initialized 
 ** \retval ErrorInvalidParameter pstcRc == NULL
 ******************************************************************************/
en_result_t Rc_Tx_Cec_DeInit(volatile stc_rcn_t *pstcRc, boolean_t bTouchNvic)
{
    // Pointer to internal data
    stc_rc_intern_data_t* pstcRcInternData ; 
  
    // Get pointer to internal data structure ...
    pstcRcInternData = RcGetInternDataPtr( pstcRc ) ;
    
    // Check for instance available or not
    if(NULL == pstcRcInternData)
    {
        return ErrorInvalidParameter ;
    }
    
    // Check for NULL pointers
    if ( NULL == pstcRc )
    {
        return ErrorInvalidParameter;
    }

    /* Clear all registers */                          
    pstcRc->TXCTRL = 0u;
    pstcRc->TXDATA = 0u;
    pstcRc->TXSTS = 0u;
    pstcRc->SFREE = 0u;
    
#if (PDL_INTERRUPT_ENABLE_RC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_RC1 == PDL_ON)     
    // Clear callback 
    pstcRcInternData->pfnRcTxIrqBusErrorCb = NULL;
    pstcRcInternData->pfnRcTxIrqStatusCb = NULL;
    
    // Set NVIC disable
    if(TRUE == bTouchNvic)
    {
        RcDeInitNvic();
    }
#endif        

    return Ok;  
}

/**
 ******************************************************************************
 ** \brief Enable CEC mode transfer of Remote Control transmision
 **
 ** This function enables transfer funciton of CEC mode
 **
 ** \param [in]  pstcRc           RC instance
 **
 ** \retval Ok                    CEC mode transmision enabled normally
 ** \retval ErrorInvalidParameter pstcRc == NULL
 ******************************************************************************/
en_result_t Rc_Tx_Cec_EnableTx(volatile stc_rcn_t *pstcRc)
{
    // Check for NULL pointers
    if ( NULL == pstcRc )
    {
        return ErrorInvalidParameter;
    }
    
    pstcRc->TXCTRL_f.TXEN = 1u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable CEC mode transfer of Remote Control transmision
 **
 ** This function disables transfer funciton of CEC mode
 **
 ** \param [in]  pstcRc           RC instance
 **
 ** \retval Ok                    CEC mode transmision disabled normally
 ** \retval ErrorInvalidParameter pstcRc == NULL
 ******************************************************************************/
en_result_t Rc_Tx_Cec_DisableTx(volatile stc_rcn_t *pstcRc)
{
    // Check for NULL pointers
    if ( NULL == pstcRc )
    {
        return ErrorInvalidParameter;
    }
    
    pstcRc->TXCTRL_f.TXEN = 0u;
    
    return Ok;
}

#if (PDL_INTERRUPT_ENABLE_RC0 == PDL_ON) ||\
    (PDL_INTERRUPT_ENABLE_RC1 == PDL_ON)
/**
 ******************************************************************************
 ** \brief Enable CEC mode interrupt of Remote Control transmision
 **
 ** This function enables transfer interrupt of CEC mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  enIrqSel         Interrupt type      
 **
 ** \retval Ok                    CEC mode transmision interrupt enabled normally
 ** \retval ErrorInvalidParameter pstcRc == NULL
 ******************************************************************************/
en_result_t Rc_Tx_Cec_EnableIrq(volatile stc_rcn_t *pstcRc, 
                                en_rc_tx_cec_irq_sel_t enIrqSel)
{
    // Check for NULL pointers
    if ( NULL == pstcRc )
    {
        return ErrorInvalidParameter;
    }
    
    switch (enIrqSel)
    {
        case RcTxCecBusErrorIrq:
            pstcRc->TXCTRL_f.IBREN = 1u;
            break;
        case RcTxCecTransStausIrq:
            pstcRc->TXCTRL_f.ITSTEN = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable CEC mode interrupt of Remote Control transmision
 **
 ** This function disables transfer interrupt of CEC mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  enIrqSel         Interrupt type
 **
 ** \retval Ok                    CEC mode transmision interrupt disabled normally
 ** \retval ErrorInvalidParameter pstcRc == NULL
 ******************************************************************************/
en_result_t Rc_Tx_Cec_DisableIrq(volatile stc_rcn_t *pstcRc, 
                                 en_rc_tx_cec_irq_sel_t enIrqSel)
{
    // Check for NULL pointers
    if ( NULL == pstcRc )
    {
        return ErrorInvalidParameter;
    }
    
    switch (enIrqSel)
    {
        case RcTxCecBusErrorIrq:
            pstcRc->TXCTRL_f.IBREN = 0u;
            break;
        case RcTxCecTransStausIrq:
            pstcRc->TXCTRL_f.ITSTEN = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}
#endif

/**
 ******************************************************************************
 ** \brief Get interrupt flag of Remote Control transmission with CEC mode
 **
 ** This function gets the interrupt flag status of CEC mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  enIrqSel         RC Reception CEC mode interrupt types
 **
 ** \retval TRUE                  The interrupt flag selected is set
 ** \retval ErrorInvalidParameter The interrupt flag selected is clear
 ******************************************************************************/
boolean_t Rc_Tx_Cec_GetIrqFlag(volatile stc_rcn_t *pstcRc, 
                               en_rc_tx_cec_irq_sel_t enIrqSel)
{
    boolean_t bRet;
    
    switch (enIrqSel)
    {
        case RcTxCecBusErrorIrq:
            bRet = ((TRUE == pstcRc->TXSTS_f.IBR) ? 1u : 0u);
            break;
        case RcTxCecTransStausIrq:
            bRet = ((TRUE == pstcRc->TXSTS_f.ITST) ? 1u : 0u);
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return bRet;
}

/**
 ******************************************************************************
 ** \brief Clear interrupt flag of Remote Control Transmission with CEC mode
 **
 ** This function clears the interrupt flag status of CEC mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  enIrqSel         RC Reception CEC mode interrupt types
 **
 ** \retval Ok                    The interrupt flag selected is clear normally
 ** \retval ErrorInvalidParameter If one of following cases matchs: 
 **                               - NULL == pstcRc
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Rc_Tx_Cec_ClrIrqFlag(volatile stc_rcn_t *pstcRc, 
                                 en_rc_tx_cec_irq_sel_t enIrqSel)
{
    // Check for NULL pointers
    if ( NULL == pstcRc )
    {
        return ErrorInvalidParameter;
    }
    
    switch (enIrqSel)
    {
        case RcTxCecBusErrorIrq:
            pstcRc->TXSTS_f.IBR = 0u;
            break;
        case RcTxCecTransStausIrq:
            pstcRc->TXSTS_f.ITST = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get ACK cycle value of Remote Control transmission with CEC mode
 **
 ** This function gets the ACK cycle value of CEC mode
 **
 ** \param [in]  pstcRc           RC instance
 **
 ** \retval TRUE                  ACK cycle value is set
 ** \retval ErrorInvalidParameter ACK cycle value is clear
 ******************************************************************************/
boolean_t Rc_Tx_GetAckCycleValue(volatile stc_rcn_t *pstcRc)
{
    return pstcRc->TXSTS_f.ACKSV;
}

/**
 ******************************************************************************
 ** \brief Write data into data specified with data type for CEC TX mode
 **
 ** This function writes the data buffer with data specified
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  u8Data           Data to be written
 ** \param [in]  enType           Remote Control TX data type
 **
 ** \retval Ok                     Data is written
 ** \retval ErrorInvalidParameter  pstcRc == NULL or other invalid configuration
 ******************************************************************************/
en_result_t Rc_Tx_Cec_WriteData(volatile stc_rcn_t *pstcRc,
                                uint8_t u8Data,
                                en_rc_txdata_type_t enType)
{
    // Check for NULL pointers
    if ( NULL == pstcRc )
    {
        return ErrorInvalidParameter;
    }
    
    switch (enType)
    {
        case RcTxCecHeader:
            pstcRc->TXCTRL_f.START = 1u;
            pstcRc->TXCTRL_f.EOM   = 0u;
            break;
        case RcTxCecData:
            pstcRc->TXCTRL_f.START = 0u;
            pstcRc->TXCTRL_f.EOM   = 0u;
            break;
        case RcTxCecPolling:
            pstcRc->TXCTRL_f.START = 1u;
            pstcRc->TXCTRL_f.EOM   = 1u;
            break;
        case RcTxCecFinal:
            pstcRc->TXCTRL_f.START = 0u;
            pstcRc->TXCTRL_f.EOM   = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    pstcRc->TXDATA = u8Data;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Write CEC data package with polling mode for CEC TX mode
 **
 ** This function writes a CEC data frame with polling mode
 **
 ** \param [in]  pstcRc           RC instance
 ** \param [in]  u8Header         Data header
 ** \param [in]  pu8Data          Pointer to data to be written
 ** \param [in]  u32Size          Data size
 **
 ** \retval Ok                     Data is written
 ** \retval ErrorInvalidParameter  pstcRc == NULL or other invalid configuration
 ******************************************************************************/
en_result_t Rc_Tx_Cec_SendDataPolling( volatile stc_rcn_t *pstcRc,
                                       uint8_t u8Header,
                                       uint8_t* pu8Data,
                                       uint32_t u32Size)
{
    uint32_t u32Timeout, u32i;
  
    // Check for NULL pointers
    if ( NULL == pstcRc )
    {
        return ErrorInvalidParameter;
    }
    
    // Send header 
    pstcRc->TXCTRL_f.START = 1u;
    pstcRc->TXCTRL_f.EOM   = 0u;
    pstcRc->TXDATA = u8Header;
    
    u32Timeout = 0x100000u;
    while(1)
    {
        u32Timeout--;
        if(TRUE == pstcRc->TXSTS_f.ITST)
        {
            pstcRc->TXSTS_f.ITST = 0u;
            break; // Data sent
        }
        
        if(u32Timeout == 0u)
        {
            return ErrorTimeout;
        }
    }
    
    if (pstcRc->TXSTS_f.ACKSV == 1u)
    {
        return Error; // ACK error
    }
    
    // Send data
    pstcRc->TXCTRL_f.START = 0u;
    pstcRc->TXCTRL_f.EOM   = 0u;
   
    for(u32i=0; u32i<(u32Size-1u); u32i++)
    {
        pstcRc->TXDATA = *pu8Data++;
        
        u32Timeout = 0x100000u;
        while(1)
        {
            u32Timeout--;
            if(TRUE == pstcRc->TXSTS_f.ITST)
            {
                pstcRc->TXSTS_f.ITST = 0u;
                break; // Data sent
            }
            
            if(u32Timeout == 0u)
            {
                return ErrorTimeout;
            }
        }
        
        if (pstcRc->TXSTS_f.ACKSV == 1u)
        {
            return Error; // ACK error
        }
    }
    
    // Send final data
    pstcRc->TXCTRL_f.START = 0u;
    pstcRc->TXCTRL_f.EOM   = 1u;
    
    pstcRc->TXDATA = *pu8Data;
    
    u32Timeout = 0x100000u;
    while(1)
    {
        u32Timeout--;
        if(TRUE == pstcRc->TXSTS_f.ITST)
        {
            pstcRc->TXSTS_f.ITST = 0u;
            break; // Data sent
        }
        
        if(0u == u32Timeout)
        {
            return ErrorTimeout;
        }
    }
    
    if (1u == pstcRc->TXSTS_f.ACKSV)
    {
        return Error; // ACK error
    }
    
    return Ok;
}

#endif

#endif // #if (defined(PDL_PERIPHERAL_RC_ACTIVE))

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
