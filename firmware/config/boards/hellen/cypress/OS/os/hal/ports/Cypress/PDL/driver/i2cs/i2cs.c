/*******************************************************************************
* \file             i2cs.c
* 
* \version          1.20
*
* \brief            This file provides the source code to the API for the I2C 
*                   Slave driver.
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
#include "i2cs/i2cs.h"

#if (defined(PDL_PERIPHERAL_I2CS_ACTIVE))

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/


/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/// Look-up table for all enabled I2CS instances and their internal data
stc_i2cs_instance_data_t m_astcI2csInstanceDataLut[I2CS_INSTANCE_COUNT] =
{
#if (PDL_PERIPHERAL_ENABLE_I2CS0 == PDL_ON)
    {
        &I2CS0,  // pstcInstance
        {
            0u, 0u, 0u, 0u   
        },
    }
#endif
};

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
#if (PDL_INTERRUPT_ENABLE_I2CS0 == PDL_ON)
/**
 ******************************************************************************
 ** \brief Device dependent initialization of transmit interrupts
 **        according CMSIS with level defined in pdl.h
 **
 ** \param pstcI2cs   Pointer to I2CS instance  
 **
 ******************************************************************************/
static void I2csInitNvic(volatile stc_i2csn_t* pstcI2cs)
{
    if (pstcI2cs == (volatile stc_i2csn_t*)(&I2CS0))
    {
        NVIC_ClearPendingIRQ(MFS6_RX_TX_I2CSLAVE_IRQn);
        NVIC_EnableIRQ(MFS6_RX_TX_I2CSLAVE_IRQn);
        NVIC_SetPriority(MFS6_RX_TX_I2CSLAVE_IRQn, PDL_IRQ_LEVEL_MFS6_I2CS0);   
    }
    
} /* I2csInitNvic */

/**
 ******************************************************************************
 ** \brief Device dependent de-initialization of interrupts according CMSIS
 **
 ** \param pstcI2cs   Pointer to I2CS instance      
 **
 ******************************************************************************/
static void I2csDeInitNvic(volatile stc_i2csn_t* pstcI2cs)
{
    if (pstcI2cs == (volatile stc_i2csn_t*)(&I2CS0))
    {
        NVIC_ClearPendingIRQ(MFS6_RX_TX_I2CSLAVE_IRQn);
        NVIC_DisableIRQ(MFS6_RX_TX_I2CSLAVE_IRQn);
        NVIC_SetPriority(MFS6_RX_TX_I2CSLAVE_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);   
    }
    
} /* I2csDeInitNvic */

#endif
/**
 ******************************************************************************
 ** \brief Return the internal data for a certain I2C instance.
 **
 ** \param pstcI2cs Pointer to I2CS instance
 **
 ** \return Pointer to internal data or NULL if instance is not enabled (or not known)
 **
 ******************************************************************************/
static stc_i2cs_intern_data_t* I2csGetInternDataPtr(volatile stc_i2csn_t* pstcI2cs)
{
    stc_i2cs_intern_data_t* pstcInternDataPtr = NULL;
    uint32_t u32Instance;

    if (NULL != pstcI2cs)
    {
        for (u32Instance = 0u; u32Instance < (uint32_t)I2csInstanceIndexMax; u32Instance++)
        {
            if (pstcI2cs == m_astcI2csInstanceDataLut[u32Instance].pstcInstance)
            {
                pstcInternDataPtr = &m_astcI2csInstanceDataLut[u32Instance].stcInternData;
                break;
            }
        }
            
    }

    return (pstcInternDataPtr);
} /* MfsGetInternDataPtr */


#if (PDL_INTERRUPT_ENABLE_I2CS0 == PDL_ON)    
/**
 ******************************************************************************
 ** \brief I2CS receive interrupt service routine.
 ** 
 ** \param pstcI2cs Pointer to I2CS instance   
 ** \param pstcInternData Pointer to I2CS internal data structure       
 ** 
 ** This function is called on I2CS Receive IRQ. 
 **
 ******************************************************************************/
void I2csIrqHandlerRx( volatile stc_i2csn_t*   pstcI2cs,
                       stc_i2cs_intern_data_t* pstcInternData)
{
    if ((NULL != pstcI2cs) && (NULL != pstcInternData))
    {
        if (NULL != pstcInternData->pfnRxIrqCb)
        {
            pstcInternData->pfnRxIrqCb();
        }
    }
} /* MfsIrqHandlerRx */

/**
 ******************************************************************************
 ** \brief I2CS transfer interrupt service routine.
 ** 
 ** \param pstcI2cs          Pointer to I2CS instance   
 ** \param pstcInternData    Pointer to I2CS internal data structure       
 ** 
 ** This function is called on I2CS transfer IRQ. 
 **
 ******************************************************************************/
void I2csIrqHandlerTx( volatile stc_i2csn_t*   pstcI2cs, 
                       stc_i2cs_intern_data_t*  pstcInternData)
{
    if ((NULL != pstcI2cs) && (NULL != pstcInternData))
    {
        if (NULL != pstcInternData->pfnTxIrqCb)
        {
            pstcInternData->pfnTxIrqCb();
        }
    }
    
} /* I2csIrqHandlerTx */
          
/**
 ******************************************************************************
 ** \brief I2CS status interrupt service routine.
 ** 
 ** \param pstcI2cs           Pointer to I2CS instance   
 ** \param pstcInternData Pointer to I2CS internal data structure       
 ** 
 ** This function is called on I2CS status IRQ. 
 **
 ******************************************************************************/
void I2csIrqHandlerStatus( volatile stc_i2csn_t*   pstcI2cs, 
                           stc_i2cs_intern_data_t* pstcInternData)
{
    if ((NULL != pstcI2cs) && (NULL != pstcInternData))
    {
        if(1u == pstcI2cs->IBSSR_f.INT)
        {
            if (NULL != pstcInternData->pfnStatusIrqCb)
            {
                pstcInternData->pfnStatusIrqCb();
            }
            
            pstcI2cs->IBSSCR = 0x0400u;
        }
        
        if(1u == pstcI2cs->IBSSR_f.RSC) 
        {
            if (NULL != pstcInternData->pfnCondIrqCb)
            {
                pstcInternData->pfnCondIrqCb(I2CS_IRQ_COND_RESTART_DETECTION);
            }
            
            pstcI2cs->IBSSCR = 0x0004u;
        }
        
        if(1u == pstcI2cs->IBSSR_f.SPC)
        {
            if (NULL != pstcInternData->pfnCondIrqCb)
            {
                pstcInternData->pfnCondIrqCb(I2CS_IRQ_COND_STOP_DETECTION);
            }
            
            pstcI2cs->IBSSCR = 0x0002u;
        }
    }
}

/**
 ******************************************************************************
 ** \brief Enable I2CS interrupts
 **
 ** \param [in] pstcI2cs   Pointer to I2CS instance   
 ** \param [in] enIrqSel   enumeration of I2CS interrupt selection
 ** \arg I2cTxIrq  TX interrupt of I2CS
 ** \arg I2cRxIrq  RX interrupt of I2CS
 ** \arg I2cStatusIrq status interrupt of I2C
 ** \arg I2cCondIrq   condition interrupt of I2C
 ** 
 ** \retval Ok                    Interrupts has been enabled 
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcI2cs == NULL
 **                               - Other invalid configuration                   
 ** 
 ******************************************************************************/
en_result_t I2cs_EnableIrq(volatile stc_i2csn_t* pstcI2cs, 
                           en_i2cs_irq_sel_t enIrqSel)
{
    if (NULL == pstcI2cs)
    {
        return (ErrorInvalidParameter);
    }
    
    switch (enIrqSel)
    {
        case I2csTxIrq:
            pstcI2cs->IBSCR_f.TIE = 1u;
            break;
        case I2csRxIrq:
            pstcI2cs->IBSCR_f.RIE = 1u;
            break;
        case I2csStatusIrq:
            pstcI2cs->IBSCR_f.INTE = 1u;
            break;
        case I2csCondIrq:
            pstcI2cs->IBSCR_f.CNDE = 1u;
            break;  
        default:
            return (ErrorInvalidParameter);
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable I2CS interrupts
 **
 ** \param [in] pstcI2cs   Pointer to I2C instance   
 ** \param [in] enIrqSel   enumeration of I2C interrupt selection
 ** \arg I2cTxIrq  TX interrupt of I2CS
 ** \arg I2cRxIrq  RX interrupt of I2CS
 ** \arg I2cStatusIrq status interrupt of I2C
 ** \arg I2cCondIrq   condition interrupt of I2C
 ** 
 ** \retval Ok                    Interrupts has been disabled 
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcI2cs == NULL
 **                               - Other invalid configuration                   
 ** 
 ******************************************************************************/
en_result_t I2cs_DisableIrq(volatile stc_i2csn_t* pstcI2cs, 
                            en_i2cs_irq_sel_t enIrqSel)
{
    if (NULL == pstcI2cs)
    {
        return (ErrorInvalidParameter);
    }
    
    switch (enIrqSel)
    {
        case I2csTxIrq:
            pstcI2cs->IBSCR_f.TIE = 0u;
            break;
        case I2csRxIrq:
            pstcI2cs->IBSCR_f.RIE = 0u;
            break;
        case I2csStatusIrq:
            pstcI2cs->IBSCR_f.INTE = 0u;
            break;
        case I2csCondIrq:
            pstcI2cs->IBSCR_f.CNDE = 0u;
            break;  
        default:
            return (ErrorInvalidParameter);
    }
    
    return Ok;
}

#endif   

/**
 ******************************************************************************
 ** \brief Initialization of I2CS module.
 **
 ** Set registers to active I2C Slave.
 **  
 ** \param [in]   pstcI2cs        Pointer to I2CS instance register area
 ** \param [in]   pstcConfig      I2CS configuration
 ** 
 ** \retval Ok                        Process successfully done.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcI2cs == NULL
 **             - pstcConfig == NULL
 **             - pstcMfsInternData == NULL (invalid or disabled MFS unit
 **                                          (PDL_PERIPHERAL_ENABLE_I2CS0)) 
 **
 ******************************************************************************/
en_result_t I2cs_Init(volatile stc_i2csn_t*        pstcI2cs, 
                      const stc_i2cs_config_t* pstcConfig)
{
    stc_i2cs_intern_data_t* pstcInternData;
    
	/* Preset local register variables to zero */
    stc_i2cslave_ibscr_field_t    stcIBSCR;
    stc_i2cslave_ibsadr_field_t   stcIBSADR;
    stc_i2cslave_ibsmskr_field_t  stcIBSMSKR;
    stc_i2cslave_ibsdstupr_field_t stcIBSDSTUPR;
    
    PDL_ZERO_STRUCT(stcIBSCR);
    PDL_ZERO_STRUCT(stcIBSADR);
    PDL_ZERO_STRUCT(stcIBSMSKR);
    PDL_ZERO_STRUCT(stcIBSDSTUPR);
    
    /* Check for valid pointer and get pointer to internal data struct ... */
    pstcInternData = I2csGetInternDataPtr(pstcI2cs);
    
    /* Parameter check and get ptr to internal data struct */
    if ((NULL == pstcInternData)
    ||  (NULL == pstcConfig)
       )
    {
        return (ErrorInvalidParameter);
    }

    /* Ack enable */
    stcIBSCR.ACKE = TRUE;

    /* Set WSEL */
    stcIBSCR.WSEL = ((TRUE == pstcConfig->bWaitSelection) ? 1u : 0u) ;
    
    /* Set noise filter */
    stcIBSCR.NFCNT = ((TRUE == pstcConfig->bEnableNoiseFilter) ? 0u : 3u) ;
    
    /* Set reverved address enable */
    stcIBSCR.RSVEN = ((TRUE == pstcConfig->bEnableReservedAddr) ? 1u : 0u) ;
    
    /* Set slave address */
    stcIBSADR.SA = pstcConfig->u8SlaveAddr;
    
    /* Enable slave address detection */
    stcIBSADR.SAEN = ((TRUE == pstcConfig->bEnableSlaveAddr) ? 1u : 0u);
    
    /* Set slave address bit mask */
    stcIBSMSKR.SM = pstcConfig->u8SlaveMaskAddr;
    
    /* Set setup time */
    stcIBSDSTUPR.SETUP = pstcConfig->u8SetupTime;
    
    /* Enable I2C*/
    stcIBSMSKR.EN = TRUE;

    /* now setup hardware with correct mode first and then go on with */
    /*   bit settings */

    /* I2C disable before other registers are set. */
    pstcI2cs->IBSMSKR_f.EN  = 0u;
    
    /* Set registers value */
    pstcI2cs->IBSDSTUPR_f = stcIBSDSTUPR;
    pstcI2cs->IBSCR_f     = stcIBSCR;
    pstcI2cs->IBSADR_f    = stcIBSADR;
    pstcI2cs->IBSMSKR_f   = stcIBSMSKR;
    
#if (PDL_INTERRUPT_ENABLE_I2CS0 == PDL_ON)     
    /* Configure interrupt */
    if(NULL != pstcConfig->pstcIrqEn)
    {
        if(TRUE == pstcConfig->pstcIrqEn->bTxIrq)
        {
            pstcI2cs->IBSCR_f.TIE = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bRxIrq)
        {
            pstcI2cs->IBSCR_f.RIE = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bStatusIrq)
        {
            pstcI2cs->IBSCR_f.INTE = 1u;
        }
        
        if(TRUE == pstcConfig->pstcIrqEn->bCondIrq)
        {
            pstcI2cs->IBSCR_f.CNDE = 1u;
        }
        
        
    }
    
    /* Configure interrupt callback */
    if(NULL != pstcConfig->pstcIrqCb)
    {
        pstcInternData->pfnTxIrqCb = pstcConfig->pstcIrqCb->pfnTxIrqCb;
        pstcInternData->pfnRxIrqCb = pstcConfig->pstcIrqCb->pfnRxIrqCb;
        pstcInternData->pfnStatusIrqCb = pstcConfig->pstcIrqCb->pfnStatusIrqCb;
        pstcInternData->pfnCondIrqCb = pstcConfig->pstcIrqCb->pfnCondIrqCb;
    }
    
    /* Configure NVIC */
    if(TRUE == pstcConfig->bTouchNvic)
    {
        I2csInitNvic(pstcI2cs);
    }

#endif
    
    return (Ok);
} /* I2cs_Init */

/**
 ******************************************************************************
 ** \brief Deinitialisation of I2C Slave.
 **
 ** All used I2CS register are reset to their default values.
 **  
 ** \param [in]   pstcI2cs        Pointer to I2CS instance register area
 ** \param [in]   bTouchNvic      Touch NVIC or not
 ** 
 ** \retval Ok                        Process successfully done.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **             - pstcI2cs == NULL
 **
 ******************************************************************************/
en_result_t I2cs_DeInit( volatile stc_i2csn_t* pstcI2cs, boolean_t bTouchNvic)
{
    en_result_t            enResult;
    stc_i2cs_intern_data_t* pstcInternData;
    
    /* Check for valid pointer and get pointer to internal data struct ... */
    pstcInternData = I2csGetInternDataPtr(pstcI2cs);
    /* ... and check */
    if (NULL == pstcInternData)
    {
        enResult = ErrorInvalidParameter;
    }
    else
    {
        /* Disable I2CS */
        pstcI2cs->IBSMSKR = 0u;

        /* Reset all other used register to default value */
        pstcI2cs->IBSCR = 0u;
        pstcI2cs->IBSADR  = 0u;
        pstcI2cs->IBSDSTUPR = 0u;
        
#if (PDL_INTERRUPT_ENABLE_I2CS0 == PDL_ON)        
        if(TRUE == bTouchNvic)
        {
            I2csDeInitNvic(pstcI2cs);
        }
#endif
        enResult = Ok;
    }

    return (enResult);
} /* I2cs_DeInit */

/**
 ******************************************************************************
 ** \brief Write I2CS data buffer
 **
 ** \param [in] pstcI2cs    Pointer to I2CS instance   
 ** \param [in] u8Data      Data to be sent
 ** 
 ** \retval Ok                    Data has been successfully sent
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcI2cs == NULL           
 ** 
 ******************************************************************************/
en_result_t I2cs_SendData(volatile stc_i2csn_t*      pstcI2cs, uint8_t u8Data)
{
    if (NULL == pstcI2cs)
    {
        return ErrorInvalidParameter;
    }
    
    pstcI2cs->IBSTDR = u8Data;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Read I2CS data buffer
 **
 ** \param [in] pstcI2cs   Pointer to I2CS instance   
 ** 
 ** \retval Receive data        
 ** 
 ******************************************************************************/
uint8_t I2cs_ReceiveData(volatile stc_i2csn_t*      pstcI2cs)
{
    return pstcI2cs->IBSRDR;
}

/**
 ******************************************************************************
 ** \brief Configure ACK signal sent to master
 **
 ** \param [in] pstcI2cs    Pointer to I2C instance   
 ** \param [in] enAck      ACK to be sent
 ** \arg  I2cAck ACK will be sent as response signal
 ** \arg  I2cNAck NACK will be sent as response signal
 ** 
 ** \retval Ok                    ACK signal has been successfully configured
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcI2cs == NULL           
 ** 
 ** This function is only meaningful when I2C is in slave mode
 **
 ******************************************************************************/
en_result_t I2cs_ConfigAck(volatile stc_i2csn_t* pstcI2cs, en_i2cs_ack_t enAck)
{
    if (NULL == pstcI2cs)
    {
        return ErrorInvalidParameter;
    }
    
    (I2csAck == enAck) ?  (pstcI2cs->IBSCR_f.ACKE = 1u) :  (pstcI2cs->IBSCR_f.ACKE = 0u);
    
    return Ok;
    
}

/**
 ******************************************************************************
 ** \brief Get the ACK signal from slave
 **
 ** \param [in] pstcI2cs    Pointer to I2C instance   
 ** 
 ** \retval I2cAck Receive the ACK  from I2C Slave
 ** \retval I2cNAck Receive the NACK from I2C Slave
 ** 
 ** This function applies in the I2C master mode.
 **
 ******************************************************************************/
en_i2cs_ack_t   I2cs_GetAck(volatile stc_i2csn_t*        pstcI2cs)
{
    en_i2cs_ack_t enRet;
    
    enRet = (0u == pstcI2cs->IBSSR_f.RACK) ? I2csAck : I2csNAck;
    
    return enRet; 
}

/**
 ******************************************************************************
 ** \brief Get status of I2CS according to status type
 **
 ** \param [in] pstcI2cs      Pointer to I2CS instance   
 ** \param [in] enStatus     I2C status type
 ** \arg   I2csStatus               I2CS status interrupt request flag
 ** \arg   I2csRxFull               I2CS RX buffer full interrupt request flag
 ** \arg   I2csTxEmpty              I2CS TX buffer empty interrupt request flag
 ** \arg   I2csFirstByteDetect      I2CS First byte detection
 ** \arg   I2csReservedByteDetect   I2CS reserved byte detection
 ** \arg   I2csActive               I2CS active status
 ** \arg   I2csRestartDetect        I2CS re-start condition detection	
 ** \arg   I2cStopDetect            I2CS stop condition detection
 ** \arg   I2csBusBusy              I2CS bus busy status 
 ** 
 ** \retval FALSE                 If one of following conditions are met:
 **                               - I2CS status interrupt request flag is clear [enStatus = I2csStatus] 
 **                               - I2CS receive buffer is not full [enStatus = I2cRxFull] 
 **                               - I2CS tranfer buffer is not empty [enStatus = I2cTxEmpty]  
 **                               - I2CS first byte is not detected [enStatus = I2cFirstByteDetect] 
 **                               - I2CS reserved address is not detected [enStatus = I2cReservedByteDetect] 
 **                               - I2CS not active [enStatus = I2csActive]
 **                               - I2CS restart is not detected [enStatus = I2csRestart] 
 **                               - I2CS stop is not detected [enStatus = I2cStopDetect] 
 **                               - I2CS bus is idle [enStatus = I2cBusStatus] 
 **
 ** \retval TRUE                 If one of following conditions are met:
 **                               - I2CS status interrupt request flag is set  [enStatus = I2csStatus] 
 **                               - I2CS receive buffer is full [enStatus = I2cRxFull] 
 **                               - I2CS tranfer buffer is empty [enStatus = I2cTxEmpty] 
 **                               - I2CS first byte is detected [enStatus = I2cFirstByteDetect] 
 **                               - I2CS reserved address is detected [enStatus = I2cReservedByteDetect]
 **                               - I2CS is active [enStatus = I2csActive] 
 **                               - I2CS restart is detected [enStatus = I2csRestart] 
 **                               - I2CS stop is detected [enStatus = I2cStopDetect] 
 **                               - I2CS bus is busy [enStatus = I2cBusStatus] 
 ** 
 ******************************************************************************/
boolean_t I2cs_GetStatus(volatile stc_i2csn_t* pstcI2cs, 
                         en_i2cs_status_t enStatus)
{
    boolean_t bRet = FALSE;
    
    switch(enStatus)
    {
        case I2csStatus:
            bRet = (1u == pstcI2cs->IBSSR_f.INT ) ? TRUE : FALSE;
            break;  
        case I2csRxFull:
            bRet = (1u == pstcI2cs->IBSSR_f.RDRF) ? TRUE : FALSE;
            break;  
        case I2csTxEmpty:
            bRet = (1u == pstcI2cs->IBSSR_f.TDRE) ? TRUE : FALSE;
            break;   
        case I2csFirstByteDetect:
            bRet = (1u == pstcI2cs->IBSSR_f.FBT) ? TRUE : FALSE;
            break;  
        case I2csReservedByteDetect:
            bRet = (1u == pstcI2cs->IBSSR_f.RSA ) ? TRUE : FALSE;
            break;
        case I2csActive:
            bRet = (1u == pstcI2cs->IBSSR_f.ACT ) ? TRUE : FALSE;
            break;            
        case I2csRestartDetect:
            bRet = (1u == pstcI2cs->IBSSR_f.SPC) ? TRUE : FALSE;
            break;             
        case I2csStopDetect:
            bRet = (1u == pstcI2cs->IBSSR_f.SPC) ? TRUE : FALSE;
            break;  
        case I2csBusBusy:
            bRet = (1u == pstcI2cs->IBSSR_f.BB) ? TRUE : FALSE;
            break;  
        default:
            break;  
    }
    
    return bRet;
}


/**
 ******************************************************************************
 ** \brief Clear status of I2CS according to status type
 **
 ** \param [in] pstcI2cs      Pointer to I2CS instance   
 ** \param [in] enStatus     I2CS status type
 ** \arg   I2csStatus               I2CS status interrupt request flag
 ** \arg   I2csRxFull               I2CS RX buffer full interrupt request flag
 ** \arg   I2csTxEmpty              I2CS TX buffer empty interrupt request flag
 ** \arg   I2csFirstByteDetect      I2CS First byte detection
 ** \arg   I2csReservedByteDetect   I2CS reserved byte detection
 ** \arg   I2csActive               I2CS active status
 ** \arg   I2csRestartDetect        I2CS re-start condition detection	
 ** \arg   I2cStopDetect            I2CS stop condition detection
 ** \arg   I2csBusBusy              I2CS bus busy status 
 ** 
 ** \retval Ok                    Status has been cleared normally
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcI2cs == NULL
 ** \note  The following status can only be cleared by hardware behavior:
 **        - I2cRxFull
 **        - I2cTxEmpty
 **        - I2cFirstByteDetect
 **        - I2cReservedByteDetect
 **        - I2csRestartDetect
 **        - I2csStopDetect
 **        - I2cBusBusy
 ** 
 ******************************************************************************/
en_result_t I2cs_ClrStatus(volatile stc_i2csn_t* pstcI2cs,
                           en_i2cs_status_t enStatus)
{
    if (NULL == pstcI2cs)
    {
        return ErrorInvalidParameter;
    }
    
    switch(enStatus)
    {
        case I2csStatus:
            pstcI2cs->IBSSCR = 0x0400u;
            break;  
        case I2csRxFull:
        case I2csTxEmpty:
            break;  
        case I2csFirstByteDetect:
            break;  
        case I2csReservedByteDetect:
            break;
        case I2csActive:
            break;     
        case I2csRestartDetect:
            pstcI2cs->IBSSCR = 0x0004u;
            break;            
        case I2csStopDetect:
            pstcI2cs->IBSSCR = 0x0002u;
            break;  
        case I2csBusBusy:
            break;  
        default:
            return ErrorInvalidParameter;  
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get direction of I2CS data in slave mode
 **
 ** \param [in] pstcI2cs      Pointer to I2CS instance   
 ** 
 ** \retval i2c_master_tx_slave_rx  Master will send data to slave 
 ** \retval i2c_slave_tx_master_rx  Master will receive data from slave
 ** 
 ** This function can be called after receiving the device address from master
 ** in the slave mode.
 **
 ******************************************************************************/
en_i2cs_data_dir_t I2cs_GetDataDir(volatile stc_i2csn_t* pstcI2cs)
{
    en_i2cs_data_dir_t enDir;
    
    (0u == pstcI2cs->IBSSR_f.TRX) ? (enDir = I2csDataRx) : (enDir = I2csDataTx);
    
    return enDir;
}

#endif /* #if (defined(PDL_PERIPHERAL_MFS_ACTIVE)) */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
