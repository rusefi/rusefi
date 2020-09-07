/*******************************************************************************
* \file             icc.c
* 
* \version          1.20
*
* \brief            This file provides the source code to the API for the ICC 
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
#include "icc/icc.h"

#if (defined(PDL_PERIPHERAL_ICC_ACTIVE))

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/// Look-up table for all enabled ICC instances and their internal data
stc_icc_instance_data_t m_astcIccInstanceDataLut[ICC_INSTANCE_COUNT] =
{
#if (PDL_PERIPHERAL_ENABLE_ICC0 == PDL_ON)
    {
        &ICC0,  // pstcInstance 
        {0u,0u,0u,0u,0u,0u,0u,0u}     // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_ICC1 == PDL_ON)
    {
        &ICC1,  // pstcInstance 
        {0u,0u,0u,0u,0u,0u,0u,0u}     // stcInternData (not initialized yet)
    },
#endif
};

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/
static stc_icc_intern_data_t* IccGetInternDataPtr(volatile stc_iccn_t* pstcIcc);

/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/

/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/
#if (PDL_INTERRUPT_ENABLE_ICC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_ICC1 == PDL_ON)
/**
 ******************************************************************************
 ** \brief Device dependent initialization of interrupts
 **        according CMSIS with level defined in pdl_user.h
 **
 ** \param pstcIcc Pointer to ICC instance 
 **
 ******************************************************************************/
static void IccInitIrq(volatile stc_iccn_t* pstcIcc)
{
#if (PDL_MCU_CORE == PDL_FM0P_CORE)  
  #if (PDL_MCU_TYPE == PDL_FM0P_TYPE3)
    #if (PDL_INTERRUPT_ENABLE_ICC1 == PDL_ON)
    if (&ICC1 == pstcIcc)
    {
        /* Enable the interrupt */
        NVIC_ClearPendingIRQ(SMCIF1_FLASHIF_IRQn);
        NVIC_EnableIRQ(SMCIF1_FLASHIF_IRQn);
        NVIC_SetPriority(SMCIF1_FLASHIF_IRQn, PDL_IRQ_LEVEL_SMCIF1_FLASHIF);
    }
    #endif
  #else  
#if (PDL_INTERRUPT_ENABLE_ICC0 == PDL_ON)
    if (&ICC0 == pstcIcc)
    {
        /* Enable the interrupt */
        NVIC_ClearPendingIRQ(PPG00_02_20_DSTC_SMCIF0_HDMICEC0_IRQn);
        NVIC_EnableIRQ(PPG00_02_20_DSTC_SMCIF0_HDMICEC0_IRQn);
        NVIC_SetPriority(PPG00_02_20_DSTC_SMCIF0_HDMICEC0_IRQn, PDL_IRQ_LEVEL_PPG00_02_20_DSTC_SMCIF0_HDMICEC0);
    }
#endif
#if (PDL_INTERRUPT_ENABLE_ICC1 == PDL_ON)
    if (&ICC1 == pstcIcc)
    {
        /* Enable the interrupt */
        NVIC_ClearPendingIRQ(TIM_SMCIF1_HDMICEC1_IRQn);
        NVIC_EnableIRQ(TIM_SMCIF1_HDMICEC1_IRQn);
        NVIC_SetPriority(TIM_SMCIF1_HDMICEC1_IRQn, PDL_IRQ_LEVEL_TIM_SMCIF1_HDMICEC1);
    }
#endif
#endif
#endif
    
#if (PDL_MCU_CORE == PDL_FM4_CORE)
    #if(PDL_MCU_TYPE == PDL_FM4_TYPE5)
    /* Enable the interrupt */
    NVIC_ClearPendingIRQ(ICC0_1_IRQn);
    NVIC_EnableIRQ(ICC0_1_IRQn);
    NVIC_SetPriority(ICC0_1_IRQn, PDL_IRQ_LEVEL_ICC0_ICC1);
    #endif
#endif    
} /* Icc_InitIrq */

/**
 ******************************************************************************
 ** \brief Device dependent de-initialization of interrupts according CMSIS
 **
 ** \param pstcIcc Pointer to ICC instance 
 **
 ******************************************************************************/
static void IccDeInitIrq(volatile stc_iccn_t* pstcIcc)
{
#if (PDL_MCU_CORE == PDL_FM0P_CORE)    
  #if (PDL_MCU_TYPE == PDL_FM0P_TYPE3)  
    #if (PDL_INTERRUPT_ENABLE_ICC1 == PDL_ON)
    if (&ICC1 == pstcIcc)
    {
        /* Disable the interrupt */
        NVIC_ClearPendingIRQ(SMCIF1_FLASHIF_IRQn);
        NVIC_DisableIRQ(SMCIF1_FLASHIF_IRQn);
        NVIC_SetPriority(SMCIF1_FLASHIF_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
    #endif  
  #else
#if (PDL_INTERRUPT_ENABLE_ICC0 == PDL_ON)
    if (&ICC0 == pstcIcc)
    {
        /* Disable the interrupt */
        NVIC_ClearPendingIRQ(PPG00_02_20_DSTC_SMCIF0_HDMICEC0_IRQn);
        NVIC_DisableIRQ(PPG00_02_20_DSTC_SMCIF0_HDMICEC0_IRQn);
        NVIC_SetPriority(PPG00_02_20_DSTC_SMCIF0_HDMICEC0_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
#endif
#if (PDL_INTERRUPT_ENABLE_ICC1 == PDL_ON)
    if (&ICC1 == pstcIcc)
    {
        /* Disable the interrupt */
        NVIC_ClearPendingIRQ(TIM_SMCIF1_HDMICEC1_IRQn);
        NVIC_DisableIRQ(TIM_SMCIF1_HDMICEC1_IRQn);
        NVIC_SetPriority(TIM_SMCIF1_HDMICEC1_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
#endif
#endif
#endif

#if (PDL_MCU_CORE == PDL_FM4_CORE)
    #if(PDL_MCU_TYPE == PDL_FM4_TYPE5)
    /* Disable the interrupt */
    NVIC_ClearPendingIRQ(ICC0_1_IRQn);
    NVIC_DisableIRQ(ICC0_1_IRQn);
    NVIC_SetPriority(ICC0_1_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    #endif
#endif    
} /* Icc_DeInitIrq */

#endif

/******************************************************************************
 * ICC driver functions
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief Return the internal data for a certain ICC instance.
 **
 ** \param pstcIcc Pointer to ICC instance
 **
 ** \return Pointer to internal data or NULL if instance is not enabled (or not known)
 **
 ******************************************************************************/
static stc_icc_intern_data_t* IccGetInternDataPtr(volatile stc_iccn_t* pstcIcc)
{
    stc_icc_intern_data_t* pstcInternDataPtr = NULL;
    uint32_t u32Instance;

    if (NULL != pstcIcc)
    {
        for (u32Instance = 0u; u32Instance < ICC_INSTANCE_COUNT; u32Instance++)
        {
            if (pstcIcc == m_astcIccInstanceDataLut[u32Instance].pstcInstance)
            {
                pstcInternDataPtr = &m_astcIccInstanceDataLut[u32Instance].stcInternData;
                break;
            }
        }
    }

    return (pstcInternDataPtr);
} /* IccGetInternDataPtr */


#if (PDL_INTERRUPT_ENABLE_ICC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_ICC1 == PDL_ON)      
/**
 ******************************************************************************
 ** \brief ICC interrupt service routine.
 ** 
 ** \param pstcIcc Pointer to ICC instance   
 ** \param pstcIccInternData Pointer to ICC internal data structure       
 ** 
 ** This function is called on Interrupt set by the ICC. 
 **
 ******************************************************************************/
void Icc_IrqHandler( volatile stc_iccn_t*   pstcIcc,
                    stc_icc_intern_data_t* pstcIccInternData)
{
    stc_smcif_irq_status_field_t stcIrqStatus;
    
    // Read IRQ status register
    stcIrqStatus = pstcIcc->IRQ_STATUS_f;
  
    if ((NULL != pstcIcc) && (NULL != pstcIccInternData))
    {
        if (TRUE == stcIrqStatus.RXFULIRQ)  // RX buffer full
        {
            if (NULL != pstcIccInternData->pfnRxFullIrqCb)
            {
                pstcIccInternData->pfnRxFullIrqCb();
            }
        }
        
        if (TRUE == stcIrqStatus.TXEMPIRQ) // TX buffer full
        {
            if (NULL != pstcIccInternData->pfnTxEmptyIrqCb)
            {
                pstcIccInternData->pfnTxEmptyIrqCb();
            }
        }
        
        if (TRUE == stcIrqStatus.RXSTBIIRQ) // RX start bit detection
        {
            if (NULL != pstcIccInternData->pfnRxStartBitIrqCb)
            {
                pstcIccInternData->pfnRxStartBitIrqCb();
            }
        }
        
        if (TRUE == stcIrqStatus.CARDEVENTIRQ) // Card event 
        {
            if (NULL != pstcIccInternData->pfnCardEventIrqCb)
            {
                pstcIccInternData->pfnCardEventIrqCb();
            }
        }
        
        if (TRUE == stcIrqStatus.IDTEXPIRQ) // idle timer 
        {
            if (NULL != pstcIccInternData->pfnIdleTimerIrqCb)
            {
                pstcIccInternData->pfnIdleTimerIrqCb();
            }
        }
        
        if (TRUE == stcIrqStatus.RDFIFOIRQ) // Read FIFO count is more than Read FIFO level + 1
        {
            if (NULL != pstcIccInternData->pfnReadFifoIrqCb)
            {
                pstcIccInternData->pfnReadFifoIrqCb();
            }
        }
        
        if (TRUE == stcIrqStatus.WRFIFOIRQ)  // Write FIFO count is less than Write FIFO level
        {
            if (NULL != pstcIccInternData->pfnWriteFifoIrqCb)
            {
                pstcIccInternData->pfnWriteFifoIrqCb();
            }
        }
        
        if (TRUE == stcIrqStatus.RDFIFOOVRIRQ) // Read FIFO overrun
        {
            if (NULL != pstcIccInternData->pfnReadFifoOverrunIrqCb)
            {
                pstcIccInternData->pfnReadFifoOverrunIrqCb();
            }
        }
    }
} // IccIrqHandler 
            
/**
 ******************************************************************************
 ** \brief Enable ICC interrupts
 **
 ** \param [in] pstcIcc    Pointer to ICC instance   
 ** \param [in] enIrqSel   Interrupt selection
 ** 
 ** \retval Ok                    Interrupts has been enabled and callback 
 **                               fucntions are set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcICC == NULL
 **                               - pstcInt == NULL
 **                               - pstcIntCb == NULL
 **                               - Other invalid configuration                   
 **
 ** The NVIC IRQ is also enabled in this function.
 ** 
 ******************************************************************************/
en_result_t Icc_EnableIrq(volatile stc_iccn_t* pstcIcc, 
                          en_icc_irq_sel_t enIrqSel)
{  
    if (NULL == pstcIcc)
    {
        return (ErrorInvalidParameter);
    }
    
    switch (enIrqSel)
    {
        case IccRxFullIrq:
            pstcIcc->GLOBALCONTROL1_f.MASKRXFUL = 1u;
            break;
        case IccTxEmptyIrq:
            pstcIcc->GLOBALCONTROL1_f.MASKTXEMP = 1u;
            break;
        case IccRxStartBitIrq:
            pstcIcc->GLOBALCONTROL1_f.MASKSTI = 1u;
            break;
        case IccCardEventIrq:
            pstcIcc->GLOBALCONTROL1_f.MASKCAEVENT = 1u;
            break;
        case IccIdleTimerIrq:
            pstcIcc->GLOBALCONTROL1_f.MASKITEXP = 1u;
            break;
        case IccReadFifoIrq:
            pstcIcc->FIFO_MODE_f.RDFIFOIRQEN = 1u;
            break;
        case IccWriteFifoIrq:
            pstcIcc->FIFO_MODE_f.WRFIFOIRQEN = 1u;
            break;
        case IccReadFifoOverrunIrq:
            pstcIcc->FIFO_MODE_f.RDFIFOOVRIRQEN = 1u;
            break;
        default:
            return (ErrorInvalidParameter);
    }
        
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable ICC interrupts
 **
 ** \param [in] pstcIcc   Pointer to ICC instance   
 ** \param [in] enIrqSel  Interrupt selection
 ** 
 ** \retval Ok                    Interrupts has been disabled and callback 
 **                               fucntion pointers are set to NULL 
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcIcc == NULL
 **                               - pstcInt == NULL                
 **
 ** The NVIC IRQ is also disabled in this function if all interrupt sources
 ** are disable.
 ** 
 ******************************************************************************/
en_result_t Icc_DisableIrq(volatile stc_iccn_t* pstcIcc, 
                           en_icc_irq_sel_t enIrqSel)
{
    if (NULL == pstcIcc)
    {
        return (ErrorInvalidParameter);
    }
        
    switch (enIrqSel)
    {
        case IccRxFullIrq:
            pstcIcc->GLOBALCONTROL1_f.MASKRXFUL = 0u;
            break;
        case IccTxEmptyIrq:
            pstcIcc->GLOBALCONTROL1_f.MASKTXEMP = 0u;
            break;
        case IccRxStartBitIrq:
            pstcIcc->GLOBALCONTROL1_f.MASKSTI = 0u;
            break;
        case IccCardEventIrq:
            pstcIcc->GLOBALCONTROL1_f.MASKCAEVENT = 0u;
            break;
        case IccIdleTimerIrq:
            pstcIcc->GLOBALCONTROL1_f.MASKITEXP = 0u;
            break;
        case IccReadFifoIrq:
            pstcIcc->FIFO_MODE_f.RDFIFOIRQEN = 0u;
            break;
        case IccWriteFifoIrq:
            pstcIcc->FIFO_MODE_f.WRFIFOIRQEN = 0u;
            break;
        case IccReadFifoOverrunIrq:
            pstcIcc->FIFO_MODE_f.RDFIFOOVRIRQEN = 0u;
            break;
        default:
            return (ErrorInvalidParameter);
    }

    return Ok;
}

/**
 ******************************************************************************
 ** \brief Read ICC IRQ Status
 **
 ** \param [in] pstcIcc   Pointer to ICC instance   
 ** 
 ** \retval Receive data        
 ** 
 ******************************************************************************/
uint16_t Icc_ReadIrqStatus(volatile stc_iccn_t* pstcIcc)
{
    return (pstcIcc->IRQ_STATUS);
}

#endif     

/**
 ******************************************************************************
 ** \brief Initialization of an ICC module to activate as ICC.
 **
 ** Set registers to active ICC as ICC.
 **
 ** \param [in]   pstcIcc         Pointer to ICC instance register area
 ** \param [in]   pstcConfig      ICC configuration
 **
 ** \retval Ok                        Process successfully done.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcICC == NULL
 **             - pstcConfig == NULL
 **             - pstcIccInternData == NULL (invalid or disabled ICC unit
 **                                          (PDL_PERIPHERAL_ENABLE_ICC0)) 
 **
 ******************************************************************************/
en_result_t Icc_Init(volatile stc_iccn_t*    pstcIcc,
                     const stc_icc_config_t* pstcConfig)
{
    stc_icc_intern_data_t* pstcIccInternData;
    
    // Preset local register variables to zero 
    stc_smcif_globalcontrol1_field_t  stcGC1;
    stc_smcif_globalcontrol2_field_t  stcGC2;
    stc_smcif_portcontrol_field_t     stcPC;
	  stc_smcif_fifo_mode_field_t       stcFFM;
    // Clear structures
    PDL_ZERO_STRUCT(stcGC1);
    PDL_ZERO_STRUCT(stcGC2);
    PDL_ZERO_STRUCT(stcPC);
    PDL_ZERO_STRUCT(stcFFM);
   
    // Check for valid pointer and get pointer to internal data struct 
    pstcIccInternData = IccGetInternDataPtr(pstcIcc);
    
    // Parameter check and get ptr to internal data struct 
    if ((NULL == pstcIccInternData)
    ||  (NULL == pstcConfig)
       )
    {
        return (ErrorInvalidParameter);
    }

    stcGC1 = pstcIcc->GLOBALCONTROL1_f;
    stcGC2 = pstcIcc->GLOBALCONTROL2_f;
    stcFFM = pstcIcc->FIFO_MODE_f;
    
    // Set the clock division 
    pstcIcc->CARDCLOCK = pstcConfig->u16ClkDiv;
    
    // Set the baudrate 
    pstcIcc->BAUDRATE = pstcConfig->u16BaudRate;

    // Set the trasnmit data format 
    switch(pstcConfig->enTxDataFormat)
    {
        case IccTx8Even2:
             stcGC1.MODE8N1 = 0u;
             stcGC1.PARITY = 0u;
             break;
        case IccTx8Odd2:
             stcGC1.MODE8N1 = 0u;
             stcGC1.PARITY = 1u;
             break;
        case IccTx8None2:
             stcGC1.MODE8N1 = 1u;
             break;
        default:
             return (ErrorInvalidParameter);
    }
    
    // Set the receive data format 
    switch(pstcConfig->enRxDataFormat)
    {
        case IccRx8Even1:
             stcGC1.MODE8N1 = 0u;
             stcGC1.FRM0 = 0u;
             stcGC1.PARITY = 0u;
             stcGC2.RX8N1 = 0u;
             break;
        case IccRx8Odd1:
             stcGC1.MODE8N1 = 0u;
             stcGC1.FRM0 = 0u;
             stcGC1.PARITY = 1u;
             stcGC2.RX8N1 = 0u;
             break;
        case IccRx8None2:
             stcGC1.MODE8N1 = 1u;
             stcGC1.FRM0 = 0u;
             stcGC2.RX8N1 = 0u;
             break;
        case IccRx8None1:
             stcGC1.MODE8N1 = 1u;
             stcGC1.FRM0 = 0u;
             stcGC2.RX8N1 = 1u;
             break;
        case IccRx9None1:
             stcGC1.MODE8N1 = 1u;
             stcGC1.FRM0 = 1u;
             stcGC2.RX8N1 = 0u;
             break;
        default:
             return (ErrorInvalidParameter);
    }

    // Set idle timer source clock
    switch (pstcConfig->enIdleTimerSrcClk)
    {
        case IccCardClk:
            stcGC1.IDTSC = 0u;
            break;
        case IccEtuClk:
            stcGC1.IDTSC = 1u;
            break;
    }

    // Set idle timer count
    pstcIcc->IDLETIMER = pstcConfig->u16IdleTimerCnt;

    // Set the guard timer
    stcGC1.GUAEN = ((TRUE == pstcConfig->bUseGuardTimer) ? 1u : 0u);
    pstcIcc->GUARDTIMER = pstcConfig->u8GuardTime;
    
    // Set the bit direction and inversion
    if((IccDataLsbFirst == pstcConfig->enBitDirection) && (FALSE == pstcConfig->bInvertData))
    {
        stcGC1.FRM1 = 0u;
        stcGC2.INVDATAOUT = 0u;
    }
    if((IccDataLsbFirst == pstcConfig->enBitDirection) && (TRUE == pstcConfig->bInvertData))
    {
        stcGC1.FRM1 = 0u;
        stcGC2.INVDATAOUT = 1u;
    }
    if((IccDataMsbFirst == pstcConfig->enBitDirection) && (TRUE == pstcConfig->bInvertData))
    {
        stcGC1.FRM1 = 1u;
        stcGC2.INVDATAOUT = 0u;
    }
    if((IccDataMsbFirst == pstcConfig->enBitDirection) && (FALSE == pstcConfig->bInvertData))
    {
        stcGC1.FRM1 = 1u;
        stcGC2.INVDATAOUT = 1u;
    }    
    
    // Set the data resend 
    stcGC1.RESND = ((TRUE == pstcConfig->bDataResend) ? 1u : 0u);

    // Set Data pin mode
    if(TRUE == pstcConfig->bDataPinMode)
    {
        // Conotrolled by software
        stcGC1.IOMOD = 1u;
        stcPC.TRIMOD = 1u;
    }
    else
    {
        // Controlled by hardware
        stcGC1.IOMOD = 0u;
        stcPC.TRIMOD = 0u;
    }

    // Set Clock pin mode
    stcGC1.CKMOD = ((TRUE == pstcConfig->bClkPinMode) ? 1u : 0u);

    // Set IO output enable
    stcPC.VPENOUTEN = ((TRUE == pstcConfig->bVpenPinOutputEn) ? 1u : 0u);
    stcPC.VCCOUTEN = ((TRUE == pstcConfig->bVccPinOutputEn) ? 1u : 0u);
    stcPC.RSTOUTEN = ((TRUE == pstcConfig->bRstPinOutputEn) ? 1u : 0u);
    stcPC.CLKOUTEN = ((TRUE == pstcConfig->bClkPinOutputEn) ? 1u : 0u);
    stcPC.IO1EN = ((TRUE == pstcConfig->bDataPinOutputEn) ? 1u : 0u);

    // Set IO pin level
    stcPC.VPEN = ((TRUE == pstcConfig->bVpenPinLevel) ? 1u : 0u);
    stcPC.VCCEN = ((TRUE == pstcConfig->bVccPinLevel) ? 1u : 0u);
    stcPC.RST = ((TRUE == pstcConfig->bRstPinLevel) ? 1u : 0u);
    stcPC.CLKPT = ((TRUE == pstcConfig->bClkPinLevel) ? 1u : 0u);
    stcPC.IO1 = ((TRUE == pstcConfig->bDataPinLevel) ? 1u : 0u);

    // FIFO setting 
    if(NULL != pstcConfig->pstcFifoConfig)
    {
        stcFFM.WRFIFOLEVEL = pstcConfig->pstcFifoConfig->TxFifoLevel; // Set Write FIFO level
        stcFFM.RDFIFOLEVEL = pstcConfig->pstcFifoConfig->RxFifoLevel; // Set ReadFIFO level
        stcFFM.FIFOEN = 1u; // Enable FIFO
    }

    // Set registers value 
    pstcIcc->GLOBALCONTROL1_f = stcGC1;
    pstcIcc->GLOBALCONTROL2_f = stcGC2;
    pstcIcc->FIFO_MODE_f = stcFFM;
    pstcIcc->PORTCONTROL_f = stcPC;

#if (PDL_INTERRUPT_ENABLE_ICC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_ICC1 == PDL_ON)
    
    if(NULL != pstcConfig->pstcIrqEn)
    {
        if(TRUE == pstcConfig->pstcIrqEn->bRxFullIrq)
        {
            pstcIcc->GLOBALCONTROL1_f.MASKRXFUL = 1u;
        }
        if(TRUE == pstcConfig->pstcIrqEn->bTxEmptyIrq)
        {
            pstcIcc->GLOBALCONTROL1_f.MASKTXEMP = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bRxStartBitIrq)
        {
            pstcIcc->GLOBALCONTROL1_f.MASKSTI = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bCardEventIrq)
        {
            pstcIcc->GLOBALCONTROL1_f.MASKCAEVENT = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bIdleTimerIrq)
        {
            pstcIcc->GLOBALCONTROL1_f.MASKITEXP = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bReadFifoIrq)
        {
            pstcIcc->FIFO_MODE_f.RDFIFOIRQEN = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bWriteFifoIrq)
        {
            pstcIcc->FIFO_MODE_f.WRFIFOIRQEN = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bReadFifoOverrunIrq)
        {
            pstcIcc->FIFO_MODE_f.RDFIFOOVRIRQEN = 1u;
        }
    }

    if (NULL != pstcConfig->pstcIrqCb)
    {
        pstcIccInternData->pfnRxFullIrqCb = pstcConfig->pstcIrqCb->pfnRxFullIrqCb;
        pstcIccInternData->pfnTxEmptyIrqCb = pstcConfig->pstcIrqCb->pfnTxEmptyIrqCb;
        pstcIccInternData->pfnRxStartBitIrqCb = pstcConfig->pstcIrqCb->pfnRxStartBitIrqCb;
        pstcIccInternData->pfnCardEventIrqCb = pstcConfig->pstcIrqCb->pfnCardEventIrqCb;
        pstcIccInternData->pfnIdleTimerIrqCb = pstcConfig->pstcIrqCb->pfnIdleTimerIrqCb;
        pstcIccInternData->pfnReadFifoIrqCb = pstcConfig->pstcIrqCb->pfnReadFifoIrqCb;
        pstcIccInternData->pfnWriteFifoIrqCb = pstcConfig->pstcIrqCb->pfnWriteFifoIrqCb;
        pstcIccInternData->pfnReadFifoOverrunIrqCb = pstcConfig->pstcIrqCb->pfnReadFifoOverrunIrqCb;
    }

    if (TRUE == pstcConfig->bTouchNvic)
    {
        IccInitIrq(pstcIcc);
    }
    
#endif
    
    return (Ok);
} /* ICC_Init  */

/**
 ******************************************************************************
 ** \brief Deinitialisation of a ICC module activating as ICC.
 **
 ** All used icc register are reset to their default values.
 **  
 ** \param [in]   pstcIcc      Pointer to ICC instance register area
 ** \param [in]   bTouchNvic   Touch NVIC or not
 ** 
 ** \retval Ok                        Process successfully done.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcIcc == NULL
 **             - pstcIccInternData == NULL (invalid or disabled ICC unit
 **                                          (PDL_PERIPHERAL_ENABLE_ICC0)) 
 **
 ******************************************************************************/
en_result_t Icc_DeInit(volatile stc_iccn_t* pstcIcc, boolean_t bTouchNvic)
{
    en_result_t            enResult;
    stc_icc_intern_data_t* pstcIccInternData;

    // Check for valid pointer and get pointer to internal data struct ... 
    pstcIccInternData = IccGetInternDataPtr(pstcIcc);
    
    // ... and check 
    if (NULL == pstcIccInternData)
    {
        enResult = ErrorInvalidParameter;
    }
    else
    {
        pstcIcc->GLOBALCONTROL1 = 0u;
        pstcIcc->GLOBALCONTROL2 = 0u;
        pstcIcc->PORTCONTROL = 0u;
        pstcIcc->CARDCLOCK = 0u;
        pstcIcc->FIFO_MODE = 0u;

    #if (PDL_INTERRUPT_ENABLE_ICC0 == PDL_ON) || \
        (PDL_INTERRUPT_ENABLE_ICC1 == PDL_ON)
        pstcIccInternData->pfnRxFullIrqCb = NULL;
        pstcIccInternData->pfnTxEmptyIrqCb = NULL;
        pstcIccInternData->pfnRxStartBitIrqCb = NULL;
        pstcIccInternData->pfnCardEventIrqCb = NULL;
        pstcIccInternData->pfnIdleTimerIrqCb = NULL;
        pstcIccInternData->pfnReadFifoIrqCb = NULL;
        pstcIccInternData->pfnWriteFifoIrqCb = NULL;
        pstcIccInternData->pfnReadFifoOverrunIrqCb = NULL;

        if (TRUE == bTouchNvic)
        {
            IccDeInitIrq(pstcIcc);
        }
    #endif
        enResult = Ok;
    }

    return (enResult);
} // Icc_DeInit 

/**
 ******************************************************************************
 ** \brief Enable ICC Function
 **
 ** \param [in] pstcIcc    Pointer to ICC instance   
 ** 
 ** \retval Ok                    ICC function is enabled successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcIcc == NULL           
 ** 
 ******************************************************************************/
en_result_t Icc_Enable (volatile stc_iccn_t* pstcIcc)
{
    if (NULL == pstcIcc)
    {
        return ErrorInvalidParameter;
    }

    pstcIcc->GLOBALCONTROL2_f.ICCDISABLE = 0u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable ICC Function
 **
 ** \param [in] pstcIcc    Pointer to ICC instance   
 ** 
 ** \retval Ok                    ICC function is enabled successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcIcc == NULL           
 ** 
 ******************************************************************************/
en_result_t Icc_Disable (volatile stc_iccn_t* pstcIcc)
{
    if (NULL == pstcIcc)
    {
        return ErrorInvalidParameter;
    }

    pstcIcc->GLOBALCONTROL2_f.ICCDISABLE = 1u;
    
    return Ok;
}


/**
 ******************************************************************************
 ** \brief Set the baudrate of ICC
 **
 ** \param [in] pstcIcc    Pointer to ICC instance   
 ** \param [in] u16BaudRate Baudrate value [bps]
 ** 
 ** \retval Ok                    ICC baud rate has been successfully modified
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcIcc == NULL
 ** \note
 ** The ICC baud rate can be initialized in the Icc_Init() and be modified
 ** in the funciton.
 ** 
 ******************************************************************************/
en_result_t Icc_SetBaudRate(volatile stc_iccn_t* pstcIcc,
                                 uint16_t u16BaudRate)
{  
    if (NULL == pstcIcc)
    {
        return ErrorInvalidParameter;
    }
    
    pstcIcc->BAUDRATE = u16BaudRate;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get status of ICC according to status type
 **
 ** \param [in] pstcIcc      Pointer to ICC instance   
 ** \param [in] enStatus     ICC status type
 ** \arg   IccTxEmpty               Transmit buffer empty
 ** \arg   IccRxFull                Receive buffer full
 ** \arg   IccRxAction              Receive is in action
 ** \arg   IccTxAction              Transmit is in action
 ** \arg   IccCardDetect            Card insertion is detected
 ** \arg   IccCardEvent             Card event is detected
 ** \arg   IccReceiveOverflow       Receive buffer overflow
 ** \arg   IccIdleTmrRun            Idle timer running
 ** \arg   IccRdFifoOverrun         Read FIFO overflow
 ** \arg   IccRdFifoFull            Read FIFO full
 ** \arg   IccWrFifoEmpty           Write FIFO empty
 ** \arg   IccTxResend              Trasmit detect resend
 ** \arg   IccRxResend              Receive detect resend
 ** 
 ** \retval FALSE            The event do not happen 
 ** \retval TRUE             The event happen
 ** 
 ******************************************************************************/
boolean_t Icc_GetStatus(volatile stc_iccn_t* pstcIcc, 
                             en_icc_status_t enStatus)
{
    boolean_t bResult = FALSE;
    
    switch(enStatus)
    {
        case IccTxEmpty:
            bResult = (pstcIcc->STATUS_f.TXEMP == 1u) ? TRUE : FALSE;
            break;  
        case IccRxFull:
            bResult = (pstcIcc->STATUS_f.RXFUL == 1u) ? TRUE : FALSE;
            break;  
        case IccRxAction:
            bResult = (pstcIcc->STATUS_f.RXACT == 1u) ? TRUE : FALSE;
            break;  
        case IccTxAction:
            bResult = (pstcIcc->STATUS_f.TXACT == 1u) ? TRUE : FALSE;
            break;  
        case IccCardDetect:
            bResult = (pstcIcc->STATUS_f.CARDDETECT == 1u) ? TRUE : FALSE;
            break;  
        case IccCardEvent:
            bResult = (pstcIcc->STATUS_f.CARDEVENT == 1u) ? TRUE : FALSE;
            break;  
        case IccReceiveOverflow:
            bResult = (pstcIcc->STATUS_f.RECOFL == 1u) ? TRUE : FALSE;
            break;  
        case IccIdleTimerRun:
            bResult = (pstcIcc->STATUS_f.IDTRUN == 1u) ? TRUE : FALSE;
            break;  
        case IccReadFifoOverrun:
            bResult = (pstcIcc->STATUS_f.RDFIFOOVR == 1u) ? TRUE : FALSE;
            break;  
        case IccReadFifoFull:
            bResult = (pstcIcc->STATUS_f.RDFIFOFUL == 1u) ? TRUE : FALSE;
            break;  
        case IccWriteFifoEmpty:
            bResult = (pstcIcc->STATUS_f.WRFIFOEMP == 1u) ? TRUE : FALSE;
            break;  
        case IccRxStartErr:
            bResult = (pstcIcc->STATUS_f.RXSTARTERR == 1u) ? TRUE : FALSE;
            break;  
        case IccTxResend:
            bResult = (pstcIcc->STATUS_f.TXRESEND == 1u) ? TRUE : FALSE;
            break;  
        case IccRxResend:
            bResult = (pstcIcc->STATUS_f.RXRESEND == 1u) ? TRUE : FALSE;
            break;  
        default:
            break;  
    }
    
    return bResult;
}

/**
 ******************************************************************************
 ** \brief Get interrupt status of ICC according to status type
 **
 ** \param [in] pstcIcc      Pointer to ICC instance   
 ** \param [in] enIrqSel     ICC interrupt type
 ** \arg   IccRxFullIrq             RX buffer full interrupt status
 ** \arg   IccTxEmptyIrq            TX buffer empty interrupt status
 ** \arg   IccRxStartBitIrq         RX start bit detect interrupt status 
 ** \arg   IccCardEventIrq          Card event interrupt status 
 ** \arg   IccIdleTimerIrq           Idle timer interrupt status
 ** \arg   IccReadFifoIrq           Read FIFO interrupt status
 ** \arg   IccWriteFifoIrq          Write FIFO interrupt status
 ** \arg   IccReadFifoOverrunIrq Read FIFO overrun interrupt status
 ** 
 ** \retval FALSE               The interrupt status is not set
 ** \retval TRUE                The interrupt status is set
 ** 
 ******************************************************************************/
boolean_t Icc_GetIrqStatus(volatile stc_iccn_t* pstcIcc, en_icc_irq_sel_t enIrqSel)
{
    boolean_t bRet = FALSE;

    switch (enIrqSel)
    {
        case IccRxFullIrq:
            bRet = ((1u == pstcIcc->IRQ_STATUS_f.RXFULIRQ) ? TRUE : FALSE);
            break;
        case IccTxEmptyIrq:
            bRet = ((1u == pstcIcc->IRQ_STATUS_f.TXEMPIRQ) ? TRUE : FALSE);
            break;
        case IccRxStartBitIrq:
            bRet = ((1u == pstcIcc->IRQ_STATUS_f.RXSTBIIRQ) ? TRUE : FALSE);
            break;
        case IccCardEventIrq:
            bRet = ((1u == pstcIcc->IRQ_STATUS_f.CARDEVENTIRQ) ? TRUE : FALSE);
            break;
        case IccIdleTimerIrq:
            bRet = ((1u == pstcIcc->IRQ_STATUS_f.IDTEXPIRQ) ? TRUE : FALSE);
            break;
        case IccReadFifoIrq:
            bRet = ((1u == pstcIcc->IRQ_STATUS_f.RDFIFOIRQ) ? TRUE : FALSE);
            break;
        case IccWriteFifoIrq:
            bRet = ((1u == pstcIcc->IRQ_STATUS_f.WRFIFOIRQ) ? TRUE : FALSE);
            break;
        case IccReadFifoOverrunIrq:
            bRet = ((1u == pstcIcc->IRQ_STATUS_f.RXFULIRQ) ? TRUE : FALSE);
            break;
    }

    return bRet;
}


/**
 ******************************************************************************
 ** \brief Write ICC data buffer
 **
 ** \param [in] pstcIcc    Pointer to ICC instance   
 ** \param [in] u16Data    Send data
 ** 
 ** \retval Ok                    Data has been successfully sent
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcIcc == NULL           
 ** 
 ******************************************************************************/
en_result_t Icc_SendData(volatile stc_iccn_t* pstcIcc, uint16_t u16Data)
{
    if (NULL == pstcIcc)
    {
        return ErrorInvalidParameter;
    }
    
    pstcIcc->DATA = u16Data;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Write ICC FIFO data buffer
 **
 ** \param [in] pstcIcc    Pointer to ICC instance   
 ** \param [in] u16Data    Send data
 ** 
 ** \retval Ok                    Data has been successfully sent
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcIcc == NULL           
 ** 
 ******************************************************************************/
en_result_t Icc_SendFifoData(volatile stc_iccn_t* pstcIcc, uint16_t u16Data)
{
    if (NULL == pstcIcc)
    {
        return ErrorInvalidParameter;
    }
    
    pstcIcc->DATA_FIFO = u16Data;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Read ICC data buffer
 **
 ** \param [in] pstcIcc   Pointer to ICC instance   
 ** 
 ** \retval Receive data        
 ** 
 ******************************************************************************/
uint16_t Icc_ReceiveData(volatile stc_iccn_t* pstcIcc)
{    
    return (pstcIcc->DATA);
}

/**
 ******************************************************************************
 ** \brief Read ICC FIFO data buffer
 **
 ** \param [in] pstcIcc   Pointer to ICC instance   
 ** 
 ** \retval Receive data        
 ** 
 ******************************************************************************/
uint16_t Icc_ReceiveFifoData(volatile stc_iccn_t* pstcIcc)
{    
    return (pstcIcc->DATA_FIFO);
}

/**
 ******************************************************************************
 ** \brief Clear ICC Write FIFO
 **
 ** \param [in] pstcIcc    Pointer to ICC instance
 ** 
 ** \retval Ok                    FIFO has been successfully reset
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcIcc == NULL           
 ** 
 ******************************************************************************/
en_result_t Icc_ClearWriteFifo (volatile stc_iccn_t* pstcIcc)
{
    if (NULL == pstcIcc)
    {
        return ErrorInvalidParameter;
    }
    
    pstcIcc->FIFO_CLEAR_MSB_WRITE = 0u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Clear ICC Read FIFO
 **
 ** \param [in] pstcIcc    Pointer to ICC instance
 ** 
 ** \retval Ok                    FIFO has been successfully reset
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcIcc == NULL           
 ** 
 ******************************************************************************/
en_result_t Icc_ClearReadFifo (volatile stc_iccn_t* pstcIcc)
{
    if (NULL == pstcIcc)
    {
        return ErrorInvalidParameter;
    }
    
    pstcIcc->FIFO_CLEAR_MSB_READ = 0u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set ICC TX FIFO level
 **
 ** \param [in] pstcIcc    Pointer to ICC instance   
 ** \param [in] u8Level    FIFO level
 ** 
 ** \retval Ok                    FIFO level has been successfully set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcIcc == NULL           
 ** 
 ******************************************************************************/
en_result_t Icc_SetWriteFifoLevel(volatile stc_iccn_t* pstcIcc,
                                       uint8_t u8Level)
{
    if ((NULL == pstcIcc) || (15u < u8Level))
    {
        return ErrorInvalidParameter;
    }
    
    pstcIcc->FIFO_MODE_f.WRFIFOLEVEL = u8Level;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set ICC TX FIFO level
 **
 ** \param [in] pstcIcc    Pointer to ICC instance   
 ** \param [in] u8Level    FIFO level
 ** 
 ** \retval Ok                    FIFO level has been successfully set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcIcc == NULL           
 ** 
 ******************************************************************************/
en_result_t Icc_SetReadFifoLevel(volatile stc_iccn_t* pstcIcc,
                                       uint8_t u8Level)

{
    if ((NULL == pstcIcc) || (15u < u8Level))
    {
        return ErrorInvalidParameter;
    }
    
    pstcIcc->FIFO_MODE_f.RDFIFOLEVEL = u8Level;
    
    return Ok;
}


/**
 ******************************************************************************
 ** \brief Get ICC Write FIFO current count
 **
 ** \param [in] pstcIcc   Pointer to ICC instance   
 ** 
 ** \retval FIFO count      
 ** 
 ******************************************************************************/
uint8_t Icc_GetWriteFifoCurCnt(volatile stc_iccn_t* pstcIcc)
{    
    uint8_t u8i, u8Count = 0u;
    uint16_t u16WriteFifo;

    u16WriteFifo = pstcIcc->FIFO_LEVEL_WRITE;

    for(u8i=0u; u8i<16u; u8i++)
    {
        if((u16WriteFifo & (1ul << u8i)) == (1ul << u8i))
        {
            u8Count++;
        }
    }
    
    return (u8Count);
}

/**
 ******************************************************************************
 ** \brief Get ICC Read FIFO current count
 **
 ** \param [in] pstcIcc   Pointer to ICC instance   
 ** 
 ** \retval FIFO count      
 ** 
 ******************************************************************************/
uint8_t Icc_GetReadFifoCurCnt(volatile stc_iccn_t* pstcIcc)
{    
    uint8_t u8i, u8Count = 0u;
    uint16_t u16ReadFifo;

    u16ReadFifo = pstcIcc->FIFO_LEVEL_READ;

    for(u8i=0u; u8i<16u; u8i++)
    {
        if((u16ReadFifo & (1ul << u8i)) == (1ul << u8i))
        {
            u8Count++;
        }
    }
    
    return (u8Count);
}

/**
 ******************************************************************************
 ** \brief Set ICC VPEN pin level
 **
 ** \param [in] pstcIcc    Pointer to ICC instance   
 ** \param [in] bLevel     Pin level
 ** 
 ** \retval Ok                    Pin level has been successfully set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcIcc == NULL           
 ** 
 ******************************************************************************/
en_result_t Icc_SetVpenPinLevel (volatile stc_iccn_t* pstcIcc, 
                                 boolean_t bLevel)

{
    if (NULL == pstcIcc)
    {
        return ErrorInvalidParameter;
    }
        
    pstcIcc->PORTCONTROL_f.VPEN = ((bLevel == TRUE) ? 1u: 0u);

    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set ICC VCC pin level
 **
 ** \param [in] pstcIcc    Pointer to ICC instance   
 ** \param [in] bLevel      Pin level
 ** 
 ** \retval Ok                    Pin level has been successfully set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcIcc == NULL           
 ** 
 ******************************************************************************/
en_result_t Icc_SetVccPinLevel (volatile stc_iccn_t* pstcIcc, 
                                boolean_t bLevel)

{
    if (NULL == pstcIcc)
    {
        return ErrorInvalidParameter;
    }
        
    pstcIcc->PORTCONTROL_f.VCCEN = ((bLevel == TRUE) ? 1u: 0u);

    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set ICC Reset pin level
 **
 ** \param [in] pstcIcc    Pointer to ICC instance   
 ** \param [in] bLevel      Pin level
 ** 
 ** \retval Ok                    Pin level has been successfully set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcIcc == NULL           
 ** 
 ******************************************************************************/
en_result_t Icc_SetRstPinLevel (volatile stc_iccn_t* pstcIcc, 
                                boolean_t bLevel)

{
    if (NULL == pstcIcc)
    {
        return ErrorInvalidParameter;
    }
        
    pstcIcc->PORTCONTROL_f.RST = ((bLevel == TRUE) ? 1u: 0u);

    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set ICC Clock pin level
 **
 ** \param [in] pstcIcc    Pointer to ICC instance   
 ** \param [in] bLevel      Pin level
 ** 
 ** \retval Ok                    Pin level has been successfully set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcIcc == NULL           
 ** 
 ******************************************************************************/
en_result_t Icc_SetClkPinLevel (volatile stc_iccn_t* pstcIcc, 
                                boolean_t bLevel)

{
    if (NULL == pstcIcc)
    {
        return ErrorInvalidParameter;
    }
        
    pstcIcc->PORTCONTROL_f.CLKPT = ((bLevel == TRUE) ? 1u: 0u);

    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set ICC Data pin level
 **
 ** \param [in] pstcIcc    Pointer to ICC instance   
 ** \param [in] bLevel      Pin level
 ** 
 ** \retval Ok                    Pin level has been successfully set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcIcc == NULL           
 ** 
 ******************************************************************************/
en_result_t Icc_SetDataPinLevel (volatile stc_iccn_t* pstcIcc, 
                                 boolean_t bLevel)

{
    if (NULL == pstcIcc)
    {
        return ErrorInvalidParameter;
    }
        
    pstcIcc->PORTCONTROL_f.IO1 = ((bLevel == TRUE) ? 1u: 0u);

    return Ok;
}


/**
 ******************************************************************************
 ** \brief Set count value of ICC Idle timer
 **
 ** \param [in] pstcIcc    Pointer to ICC instance   
 ** \param [in] u16Cnt     Timer count
 ** 
 ** \retval Ok                    Configuration has been successfully set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcIcc == NULL           
 ** 
 ******************************************************************************/
en_result_t Icc_SetIdleTimerCnt(volatile stc_iccn_t* pstcIcc, 
                                uint16_t u16Cnt)

{
    if (NULL == pstcIcc)
    {
        return ErrorInvalidParameter;
    }
        
    pstcIcc->IDLETIMER = u16Cnt;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Start Idle timer
 **
 ** \param [in] pstcIcc    Pointer to ICC instance
 ** 
 ** \retval Ok                    No error
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcIcc == NULL           
 ** 
 ******************************************************************************/
en_result_t Icc_StartIdleTimer(volatile stc_iccn_t* pstcIcc)
{
    if (NULL == pstcIcc)
    {
        return ErrorInvalidParameter;
    }
    
    pstcIcc->GLOBALCONTROL1_f.STIDT = 1u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Stop Idle timer
 **
 ** \param [in] pstcIcc    Pointer to ICC instance
 ** 
 ** \retval Ok                    No error
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcIcc == NULL           
 ** 
 ******************************************************************************/
en_result_t Icc_StopIdleTimer(volatile stc_iccn_t* pstcIcc)
{
    if (NULL == pstcIcc)
    {
        return ErrorInvalidParameter;
    }
    
    pstcIcc->GLOBALCONTROL1_f.STIDT = 0u;
    
    return Ok;
}

#endif /* #if (defined(PDL_PERIPHERAL_ICC_ACTIVE)) */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
