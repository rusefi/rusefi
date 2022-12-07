/*******************************************************************************
* \file             qprc.c
* 
* \version          1.20
*
* \brief            This file provides the source code to the API for the QPRC 
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
#include "qprc/qprc.h"

#if (defined(PDL_PERIPHERAL_QPRC_ACTIVE))

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)
static volatile stc_qprc_nfn_t* QprcGetNoisFilterPtr( volatile stc_qprcn_t *pstcQprc );
#endif

#if (PDL_INTERRUPT_ENABLE_QPRC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_QPRC1 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_QPRC2 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_QPRC3 == PDL_ON)   
static stc_qprc_intern_data_t *QprcGetInternDataPtr( volatile stc_qprcn_t *pstcQprc );
#endif
/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/

/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/
/// Look-up table for all enabled ADC instances and their internal data
stc_qprc_instance_data_t m_astcQprcInstanceDataLut[QPRC_INSTANCE_COUNT] =
{
#if (PDL_PERIPHERAL_ENABLE_QPRC0 == PDL_ON)
    {
        &QPRC0,   // pstcInstance QPRC
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
        &QPRCNF0, // pstcInstanceNf QPRC-NF
    #endif    
        {
            // stcInternData (not initialized yet)
            0u, 0u, 0u, 0u, 0u, 0u
        }
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_QPRC1 == PDL_ON)
    {
        &QPRC1,   // pstcInstance QPRC
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
        &QPRCNF1, // pstcInstanceNf QPRC-NF
    #endif    
        {
            // stcInternData (not initialized yet)
            0u, 0u, 0u, 0u, 0u, 0u
        }
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_QPRC2 == PDL_ON)
    {
        &QPRC2,   // pstcInstance QPRC
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
        &QPRCNF2, // pstcInstanceNf QPRC-NF
    #endif    
        {
            // stcInternData (not initialized yet)
            0u, 0u, 0u, 0u, 0u, 0u
        }
    },
#endif

};

#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)
/**
 ******************************************************************************
 ** \brief Get the noise filter for a certain QPRC instance
 **
 ** \param [in]  pstcQprc  Pointer to QPRC instance
 **
 ** \return  Pointer to the noise filter
 **
 ******************************************************************************/
static volatile stc_qprc_nfn_t *QprcGetNoisFilterPtr( volatile stc_qprcn_t *pstcQprc )
{
    uint32_t u32Instance;

    for (u32Instance = 0u; u32Instance < QPRC_INSTANCE_COUNT; u32Instance++)
    {
        if (pstcQprc == m_astcQprcInstanceDataLut[u32Instance].pstcInstance)
        {
            return m_astcQprcInstanceDataLut[u32Instance].pstcInstanceNf;
        }
    }

    return (volatile stc_qprc_nfn_t *)NULL;
}
#endif

/**
 ******************************************************************************
 ** \brief Get the internal data for a certain QPRC instance
 **
 ** \param [in]  pstcQprc  Pointer to QPRC instance
 **
 ** \return  Pointer to internal data
 **
 ******************************************************************************/
static  stc_qprc_intern_data_t *QprcGetInternDataPtr( volatile stc_qprcn_t *pstcQprc )
{
    uint32_t u32Instance;

    for (u32Instance = 0u; u32Instance < QPRC_INSTANCE_COUNT; u32Instance++)
    {
        if (pstcQprc == m_astcQprcInstanceDataLut[u32Instance].pstcInstance)
        {
            return &m_astcQprcInstanceDataLut[u32Instance].stcInternData;
        }
    }

    return NULL;
}    

#if (PDL_INTERRUPT_ENABLE_QPRC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_QPRC1 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_QPRC2 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_QPRC3 == PDL_ON)   
/**
 ******************************************************************************
 ** \brief Set NVIC Interrupt depending on QPRC instance
 **
 ** \param pstcQprc Pointer to QPRC instance
 **
 ******************************************************************************/
static void QprcInitNvic(volatile stc_qprcn_t* pstcQprc)
{
#if (PDL_MCU_CORE == PDL_FM4_CORE)
    #if (PDL_INTERRUPT_ENABLE_QPRC0 == PDL_ON)
    NVIC_ClearPendingIRQ(QPRC0_IRQn);
    NVIC_EnableIRQ(QPRC0_IRQn);
    NVIC_SetPriority(QPRC0_IRQn, PDL_IRQ_LEVEL_QPRC0);
    #endif
    #if (PDL_INTERRUPT_ENABLE_QPRC1 == PDL_ON)
    NVIC_ClearPendingIRQ(QPRC1_IRQn);
    NVIC_EnableIRQ(QPRC1_IRQn);
    NVIC_SetPriority(QPRC1_IRQn, PDL_IRQ_LEVEL_QPRC1);
    #endif
    #if (PDL_INTERRUPT_ENABLE_QPRC2 == PDL_ON)
    NVIC_ClearPendingIRQ(QPRC2_IRQn);
    NVIC_EnableIRQ(QPRC2_IRQn);
    NVIC_SetPriority(QPRC2_IRQn, PDL_IRQ_LEVEL_QPRC2);
    #endif
    #if (PDL_INTERRUPT_ENABLE_QPRC3 == PDL_ON)
    NVIC_ClearPendingIRQ(QPRC3_IRQn);
    NVIC_EnableIRQ(QPRC3_IRQn);
    NVIC_SetPriority(QPRC3_IRQn, PDL_IRQ_LEVEL_QPRC3);
    #endif
#elif (PDL_MCU_CORE == PDL_FM0P_CORE)
    NVIC_ClearPendingIRQ(DT_QPRC_IRQn);
    NVIC_EnableIRQ(DT_QPRC_IRQn);
    NVIC_SetPriority(DT_QPRC_IRQn, PDL_IRQ_LEVEL_DT_QPRC);
#else
    NVIC_ClearPendingIRQ(DT_QPRC0_2_IRQn);
    NVIC_EnableIRQ(DT_QPRC0_2_IRQn);
    NVIC_SetPriority(DT_QPRC0_2_IRQn, PDL_IRQ_LEVEL_DT_QPRC);
#endif
    
}

/**
 ******************************************************************************
 ** \brief Clear NVIC Interrupt depending on QPRC instance
 **
 ** \param [in]  pstcQprc  Pointer to QPRC instance
 **
 ******************************************************************************/
static void QprcDeInitNvic(volatile stc_qprcn_t* pstcQprc)
{   
#if (PDL_MCU_CORE == PDL_FM4_CORE)
    #if (PDL_INTERRUPT_ENABLE_QPRC0 == PDL_ON)
    NVIC_ClearPendingIRQ(QPRC0_IRQn);
    NVIC_DisableIRQ(QPRC0_IRQn);
    NVIC_SetPriority(QPRC0_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    #endif
    #if (PDL_INTERRUPT_ENABLE_QPRC1 == PDL_ON)
    NVIC_ClearPendingIRQ(QPRC1_IRQn);
    NVIC_DisableIRQ(QPRC1_IRQn);
    NVIC_SetPriority(QPRC1_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    #endif
    #if (PDL_INTERRUPT_ENABLE_QPRC2 == PDL_ON)
    NVIC_ClearPendingIRQ(QPRC2_IRQn);
    NVIC_DisableIRQ(QPRC2_IRQn);
    NVIC_SetPriority(QPRC2_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    #endif
    #if (PDL_INTERRUPT_ENABLE_QPRC3 == PDL_ON)
    NVIC_ClearPendingIRQ(QPRC3_IRQn);
    NVIC_DisableIRQ(QPRC3_IRQn);
    NVIC_SetPriority(QPRC3_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    #endif
#elif (PDL_MCU_CORE == PDL_FM0P_CORE)
    NVIC_ClearPendingIRQ(DT_QPRC_IRQn);
    NVIC_DisableIRQ(DT_QPRC_IRQn);
    NVIC_SetPriority(DT_QPRC_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
#else
    NVIC_ClearPendingIRQ(DT_QPRC0_2_IRQn);
    NVIC_DisableIRQ(DT_QPRC0_2_IRQn);
    NVIC_SetPriority(DT_QPRC0_2_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
#endif    
    return;
}

/**
 ******************************************************************************
 ** \brief QPRC instance interrupt service routine
 **
 ** \param [in]  pstcQprc  Pointer to QPRC instance
 ** \param [in]  pstcQprcInternData  Pointer to internal data
 ******************************************************************************/
void Qprc_IrqHandler  ( volatile stc_qprcn_t *pstcQprc,
                        stc_qprc_intern_data_t *pstcQprcInternData )
{
    if ((NULL == pstcQprc) ||
        (NULL == pstcQprcInternData))
    {
        return;
    }

    if (PdlSet == pstcQprc->QICRL_f.QPCMF)    // QPCCR match ?
    {
        pstcQprc->QICRL_f.QPCMF = PdlClr;    // Clear interrupt
        if (NULL != pstcQprcInternData->pfnPcMatchIrqCb)
        {
            pstcQprcInternData->pfnPcMatchIrqCb();
        }
    }

    if (PdlSet == pstcQprc->QICRL_f.QPRCMF)   // QPRCR match ?
    {
        pstcQprc->QICRL_f.QPRCMF = PdlClr;   // Clear interrupt
        if (NULL != pstcQprcInternData->pfnPcRcMatchIrqCb)
        {
            pstcQprcInternData->pfnPcRcMatchIrqCb();
        }
    }

    if (PdlSet == pstcQprc->QICRL_f.OFDF)    //Overflow interrupt ?
    {
        pstcQprc->QICRL_f.OFDF = PdlClr;     // Clear interrupt
        if (NULL != pstcQprcInternData->pfnPcOfUfZeroIrqCb)
        {
            pstcQprcInternData->pfnPcOfUfZeroIrqCb(QPRC_PC_OVERFLOW_INT);
        }
    }

    if (PdlSet == pstcQprc->QICRL_f.UFDF)  //Underflow interrupt ?
    {
        pstcQprc->QICRL_f.UFDF = PdlClr;   // Clear interrupt
        if (NULL != pstcQprcInternData->pfnPcOfUfZeroIrqCb)
        {
            pstcQprcInternData->pfnPcOfUfZeroIrqCb(QPRC_PC_UNDERFLOW_INT);
        }
    }

    if (PdlSet == pstcQprc->QICRL_f.ZIIF)    //Zero interrupt ?
    {
        pstcQprc->QICRL_f.ZIIF = PdlClr;     // Clear interrupt
        if (NULL != pstcQprcInternData->pfnPcOfUfZeroIrqCb)
        {
            pstcQprcInternData->pfnPcOfUfZeroIrqCb(QPRC_PC_ZERO_INDEX_INT);
        }
    }

    if (PdlSet == pstcQprc->QICRH_f.CDCF)    //PC invert match ?
    {
        pstcQprc->QICRH_f.CDCF = PdlClr;     // Clear interrupt
        if (NULL != pstcQprcInternData->pfnPcCountInvertIrqCb)
        {
            pstcQprcInternData->pfnPcCountInvertIrqCb();
        }
    }

    if (PdlSet == pstcQprc->QECR_f.ORNGF)    //RC outrange ?
    {
        pstcQprc->QECR_f.ORNGF = PdlClr;     // Clear interrupt
        if (NULL != pstcQprcInternData->pfnRcOutrangeIrqCb)
        {
            pstcQprcInternData->pfnRcOutrangeIrqCb();
        }
    }

    if (PdlSet == pstcQprc->QICRH_f.QPCNRCMF)     //PcMatch and RcMatch ?
    {
        pstcQprc->QICRH_f.QPCNRCMF = PdlClr;      // Clear interrupt
        if (NULL != pstcQprcInternData->pfnPcMatchRcMatchIrqCb)
        {
            pstcQprcInternData->pfnPcMatchRcMatchIrqCb();
        }
    }

    return;
}

#endif

/**
 ******************************************************************************
 ** \brief Initialize QPRC
 **
 ** This function initializes an QPRC module and sets up the internal
 ** data structures
 **
 ** \param [in]  pstcQprc          Pointer to QPRC instance
 ** \param [in]  pstcConfig        Pointer to QPRC module configuration
 **
 ** \retval Ok                    Internal data has been setup
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcQprc == NULL
 **                               - pstcConfig == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Qprc_Init( volatile stc_qprcn_t* pstcQprc,
                       const stc_qprc_config_t*  pstcConfig )
{
    stc_qprc_intern_data_t* pstcQprcInternData; 
  
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
    volatile stc_qprc_nfn_t *pstcQprcNoiseFilter;
    pstcQprcNoiseFilter = QprcGetNoisFilterPtr(pstcQprc);
    if(NULL == pstcQprcNoiseFilter)
    {
        return ErrorInvalidParameter;
    }
#endif
    if ( (NULL == pstcQprc) ||
         (NULL == pstcConfig)
       )
    {
        return ErrorInvalidParameter;
    }

     pstcQprcInternData = QprcGetInternDataPtr(pstcQprc);

    if (NULL == pstcQprcInternData)
    {
        return ErrorInvalidParameter;
    }

    // Set AIN and BIN swap function
    pstcQprc->QCR_f.SWAP = ((TRUE == pstcConfig->bSwapAinBin) ? 1u : 0u);
    
    // Set the compare mode of Position counter
    switch (pstcConfig->enCompareMode)
    {
        case QprcCompareWithPosition:
            pstcQprc->QCR_f.RSEL = 0u;
            break;
        case QprcCompareWithRevolution:
            pstcQprc->QCR_f.RSEL = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    // Set ZIN active edge
    switch (pstcConfig->enZinEdge)
    {
        case QprcZinDisable: // Disables edge and level detection
            pstcQprc->QCR_f.CGE = 0u;
            break;
        case QprcZinFallingEdge: // ZIN active at falling edge
            pstcQprc->QCR_f.CGSC = 0u;            
            pstcQprc->QCR_f.CGE = 1u;
            break;
        case QprcZinRisingEdge: // ZIN active at rising edge
            pstcQprc->QCR_f.CGSC = 0u;            
            pstcQprc->QCR_f.CGE = 2u;
            break;
        case QprcZinBothEdges: // ZIN active at falling or rising edge
            pstcQprc->QCR_f.CGSC = 0u;   
            pstcQprc->QCR_f.CGE = 3u;
            break;
        case QprcZinLowLevel: // ZIN active at low level detected
            pstcQprc->QCR_f.CGSC = 1u; 
            pstcQprc->QCR_f.CGE = 1u;
            break;
        case QprcZinHighLevel: // ZIN active at high level detected
            pstcQprc->QCR_f.CGSC = 1;  
            pstcQprc->QCR_f.CGE = 2u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    // Set BIN active edge
    switch (pstcConfig->enBinEdge)
    {
        case QprcBinDisable:     // Disables edge detection
            pstcQprc->QCR_f.BES = 0u;
            break;
        case QprcBinFallingEdge: // BIN active at falling edge
            pstcQprc->QCR_f.BES = 1u;
            break;
        case QprcBinRisingEdge:  // BIN active at rising edge
            pstcQprc->QCR_f.BES = 2u;
            break;
        case QprcBinBothEdges:   // BIN active at falling or rising edge
            pstcQprc->QCR_f.BES = 3u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    // Set AIN active edge
    switch (pstcConfig->enAinEdge)
    {
        case QprcBinDisable: // Disables edge detection
            pstcQprc->QCR_f.AES = 0u;
            break;
        case QprcBinFallingEdge: // BIN active at falling edge
            pstcQprc->QCR_f.AES = 1u;
            break;
        case QprcBinRisingEdge: // BIN active at rising edge
            pstcQprc->QCR_f.AES = 2u;
            break;
        case QprcBinBothEdges: // BIN active at falling or rising edge
            pstcQprc->QCR_f.AES = 3u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    // Set PC reset mask count
    switch (pstcConfig->enPcResetMask)
    {
        case QprcResetMaskDisable: // No reset mask
            pstcQprc->QCR_f.PCRM = 0u;
            break;
        case QprcResetMask2Times:
            pstcQprc->QCR_f.PCRM = 1u;
            break;
        case QprcResetMask4Times:
            pstcQprc->QCR_f.PCRM = 2u;
            break;
        case QprcResetMask8Times:
            pstcQprc->QCR_f.PCRM = 3u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    // Set PC compare range
    pstcQprc->QECR_f.ORNGMD = (TRUE == pstcConfig->b8KValue) ? 1u : 0u ; // TRUE: Outrange mode from 0 to 0x7FFF, FALSE: Outrange mode from 0 to 0xFFFF:

#if ((PDL_MCU_TYPE >= PDL_FM4_TYPE3) && (PDL_MCU_TYPE != PDL_FM4_TYPE6))
    // Set phase edge change 
    pstcQprc->QECR_f.PEC = ((TRUE == pstcConfig->bPhaseEdge) ? 1u : 0u);
#endif
    
    /* Set QPRC Noise Filter */
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
    pstcQprcNoiseFilter = QprcGetNoisFilterPtr(pstcQprc);
    pstcQprcNoiseFilter->NFCTLA_f.AINMD = (TRUE == pstcConfig->stcAinFilter.bInputMask) ? 1u : 0u ;
    pstcQprcNoiseFilter->NFCTLA_f.AINLV = (TRUE == pstcConfig->stcAinFilter.bInputInvert) ? 1u : 0u ;
    switch (pstcConfig->stcAinFilter.enWidth)
    {
        case QprcNoFilter:
            pstcQprcNoiseFilter->NFCTLA_f.AINNWS = 0u;
            break;
        case QprcFilterWidth4Pclk:
            pstcQprcNoiseFilter->NFCTLA_f.AINNWS = 1u;
            break;
        case QprcFilterWidth8Pclk:
            pstcQprcNoiseFilter->NFCTLA_f.AINNWS = 2u;
            break;
        case QprcFilterWidth16Pclk:
            pstcQprcNoiseFilter->NFCTLA_f.AINNWS = 3u;
            break;
        case QprcFilterWidth32Pclk:
            pstcQprcNoiseFilter->NFCTLA_f.AINNWS = 4u;
            break;
        case QprcFilterWidth64Pclk:
            pstcQprcNoiseFilter->NFCTLA_f.AINNWS = 5u;
            break;
        case QprcFilterWidth128Pclk:
            pstcQprcNoiseFilter->NFCTLA_f.AINNWS = 6u;
            break;
        case QprcFilterWidth256Pclk:
            pstcQprcNoiseFilter->NFCTLA_f.AINNWS = 7u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    pstcQprcNoiseFilter->NFCTLB_f.BINMD = (TRUE == pstcConfig->stcBinFilter.bInputMask) ? 1u : 0u ;
    pstcQprcNoiseFilter->NFCTLB_f.BINLV = (TRUE == pstcConfig->stcBinFilter.bInputInvert) ? 1u : 0u ;
    switch (pstcConfig->stcBinFilter.enWidth)
    {
        case QprcNoFilter:
            pstcQprcNoiseFilter->NFCTLB_f.BINNWS = 0u;
            break;
        case QprcFilterWidth4Pclk:
            pstcQprcNoiseFilter->NFCTLB_f.BINNWS = 1u;
            break;
        case QprcFilterWidth8Pclk:
            pstcQprcNoiseFilter->NFCTLB_f.BINNWS = 2u;
            break;
        case QprcFilterWidth16Pclk:
            pstcQprcNoiseFilter->NFCTLB_f.BINNWS = 3u;
            break;
        case QprcFilterWidth32Pclk:
            pstcQprcNoiseFilter->NFCTLB_f.BINNWS = 4u;
            break;
        case QprcFilterWidth64Pclk:
            pstcQprcNoiseFilter->NFCTLB_f.BINNWS = 5u;
            break;
        case QprcFilterWidth128Pclk:
            pstcQprcNoiseFilter->NFCTLB_f.BINNWS = 6u;
            break;
        case QprcFilterWidth256Pclk:
            pstcQprcNoiseFilter->NFCTLB_f.BINNWS = 7u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    pstcQprcNoiseFilter->NFCTLZ_f.ZINMD = (TRUE == pstcConfig->stcCinFilter.bInputMask) ? 1u : 0u ;
    pstcQprcNoiseFilter->NFCTLZ_f.ZINLV = (TRUE == pstcConfig->stcCinFilter.bInputInvert) ? 1u : 0u ;
    switch (pstcConfig->stcCinFilter.enWidth)
    {
        case QprcNoFilter:
            pstcQprcNoiseFilter->NFCTLZ_f.ZINNWS = 0u;
            break;
        case QprcFilterWidth4Pclk:
            pstcQprcNoiseFilter->NFCTLZ_f.ZINNWS = 1u;
            break;
        case QprcFilterWidth8Pclk:
            pstcQprcNoiseFilter->NFCTLZ_f.ZINNWS = 2u;
            break;
        case QprcFilterWidth16Pclk:
            pstcQprcNoiseFilter->NFCTLZ_f.ZINNWS = 3u;
            break;
        case QprcFilterWidth32Pclk:
            pstcQprcNoiseFilter->NFCTLZ_f.ZINNWS = 4u;
            break;
        case QprcFilterWidth64Pclk:
            pstcQprcNoiseFilter->NFCTLZ_f.ZINNWS = 5u;
            break;
        case QprcFilterWidth128Pclk:
            pstcQprcNoiseFilter->NFCTLZ_f.ZINNWS = 6u;
            break;
        case QprcFilterWidth256Pclk:
            pstcQprcNoiseFilter->NFCTLZ_f.ZINNWS = 7u;
            break;
        default:
            return ErrorInvalidParameter;
    }
#endif

#if (PDL_INTERRUPT_ENABLE_QPRC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_QPRC1 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_QPRC2 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_QPRC3 == PDL_ON)       
    // Set interrupt enable
    if(pstcConfig->pstcIrqEn != NULL)
    {
        if (TRUE == pstcConfig->pstcIrqEn->bQprcPcMatchIrq)
        {
            pstcQprc->QICRL_f.QPCMIE = 1u;
        }
        
        if (TRUE == pstcConfig->pstcIrqEn->bQprcPcRcMatchIrq)
        {
            pstcQprc->QICRL_f.QPRCMIE = 1u;
        }
        
        if (TRUE == pstcConfig->pstcIrqEn->bQprcPcMatchRcMatchIrq)
        {
            pstcQprc->QICRH_f.QPCNRCMIE = 1u;
        }
        
        if (TRUE == pstcConfig->pstcIrqEn->bQprcPcOfUfZeroIrq)
        {
            pstcQprc->QICRL_f.OUZIE = 1u;
        }
        
        if (TRUE == pstcConfig->pstcIrqEn->bQprcPcCountInvertIrq)
        {
            pstcQprc->QICRH_f.CDCIE = 1u;
        }
        
        if (TRUE == pstcConfig->pstcIrqEn->bQprcRcOutrangeIrq)
        {
            pstcQprc->QECR_f.ORNGIE = 1u;
        }
        
    }
    
    // Set interrupt callback functions
    if(NULL != pstcConfig->pstcIrqCb)
    {
        pstcQprcInternData->pfnPcMatchIrqCb        = pstcConfig->pstcIrqCb->pfnPcMatchIrqCb;
        pstcQprcInternData->pfnPcRcMatchIrqCb      = pstcConfig->pstcIrqCb->pfnPcRcMatchIrqCb;
        pstcQprcInternData->pfnPcMatchRcMatchIrqCb = pstcConfig->pstcIrqCb->pfnPcMatchRcMatchIrqCb;
        pstcQprcInternData->pfnPcOfUfZeroIrqCb     = pstcConfig->pstcIrqCb->pfnPcOfUfZeroIrqCb;
        pstcQprcInternData->pfnPcCountInvertIrqCb  = pstcConfig->pstcIrqCb->pfnPcCountInvertIrqCb;
        pstcQprcInternData->pfnRcOutrangeIrqCb     = pstcConfig->pstcIrqCb->pfnRcOutrangeIrqCb;
    }
    
    // Set NVIC
    if(TRUE == pstcConfig->bTouchNvic)
    {
        QprcInitNvic(pstcQprc);
    }

#endif    
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief De-Initialize QPRC
 **
 ** This function clears an QPRC instance.
 **
 ** \param [in]  pstcQprc          Pointer to QPRC instance
 ** \param [in]  bTouchNvic        Touch NVIC or not
 **
 ** \retval Ok                    QPRC instance cleared
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcQprc == NULL
 ******************************************************************************/
en_result_t Qprc_DeInit( volatile stc_qprcn_t* pstcQprc, boolean_t bTouchNvic )
{
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
    volatile stc_qprc_nfn_t *pstcQprcNoiseFilter;
    pstcQprcNoiseFilter = QprcGetNoisFilterPtr(pstcQprc);
    if(NULL == pstcQprcNoiseFilter)
    {
        return ErrorInvalidParameter;
    }
#endif
    
    if(NULL == pstcQprc)
    {
        return ErrorInvalidParameter;
    }
  
    pstcQprc->QCRL = 0u;
    pstcQprc->QCRH = 0u;
    pstcQprc->QPCR = 0u;
    pstcQprc->QRCR = 0u;
    pstcQprc->QPCCR = 0u;
    pstcQprc->QPRCR = 0u;
    pstcQprc->QICRL = 0u;
    pstcQprc->QICRH = 0u;
    pstcQprc->QMPR = 0xFFFFu;
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE) 
    pstcQprcNoiseFilter->NFCTLA = 0u;
    pstcQprcNoiseFilter->NFCTLB = 0u;
    pstcQprcNoiseFilter->NFCTLZ = 0u;
#endif     

#if (PDL_INTERRUPT_ENABLE_QPRC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_QPRC1 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_QPRC2 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_QPRC3 == PDL_ON)           
    if(TRUE == bTouchNvic)
    {
        QprcDeInitNvic(pstcQprc);
    }
#endif    
   
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Stop Position Counter
 **
 ** \param [in]  pstcQprc  Pointer to QPRC instance
 **
 ** \retval Ok                    Stop Position Counter counting successfully
 ** \retval ErrorInvalidParameter pstcQprc == NULL
 ******************************************************************************/
en_result_t Qprc_StopPcCount(volatile stc_qprcn_t *pstcQprc)
{
    // Check for NULL pointer
    if (NULL == pstcQprc)
    {
        return ErrorInvalidParameter ;
    }

    pstcQprc->QCR_f.PSTP = 1u;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Restart Position Counter from stop status
 **
 ** \param [in]  pstcQprc  Pointer to QPRC instance
 **
 ** \retval Ok                    Restart Position Counter successfully
 ** \retval ErrorInvalidParameter pstcQprc == NULL
 ******************************************************************************/
en_result_t Qprc_RestartPcCount(volatile stc_qprcn_t *pstcQprc)
{
    // Check for NULL pointer
    if (NULL == pstcQprc)
    {
        return ErrorInvalidParameter ;
    }

    pstcQprc->QCR_f.PSTP = 0u;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set count value of Position counter
 **
 ** \param [in]  pstcQprc        Pointer to QPRC instance
 ** \param [in]  u16PcValue      Count value
 **
 ** \retval Ok                    Count value has been setup
 ** \retval ErrorInvalidParameter pstcQprc == NULL
 ******************************************************************************/
en_result_t Qprc_SetPcCount ( volatile stc_qprcn_t *pstcQprc,
                                    uint16_t u16PcValue )
{
    if (NULL == pstcQprc)
    {
        return ErrorInvalidParameter;
    }

    pstcQprc->QPCR = u16PcValue;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get count value of Position counter
 **
 ** \param [in]  pstcQprc  Pointer to QPRC instance
 **
 ** \retval Count value
 **
 ** \note If pstcQprc = NULL, the return value will be 0xFFFF.
 ******************************************************************************/
uint16_t Qprc_GetPcCount ( volatile stc_qprcn_t *pstcQprc )
{
    if (NULL == pstcQprc)
    {
        return 0xFFFFu;
    }

    return (uint16_t)pstcQprc->QPCR;
}

/**
 ******************************************************************************
 ** \brief Set count value of Revolution Counter
 **
 ** \param [in]  pstcQprc        Pointer to QPRC instance
 ** \param [in]  u16RcValue      Count value
 **
 ** \retval Ok                    Count value has been setup
 ** \retval ErrorInvalidParameter pstcQprc == NULL
 ******************************************************************************/
en_result_t Qprc_SetRcCount ( volatile stc_qprcn_t *pstcQprc,
                                    uint16_t u16RcValue )
{
    if (NULL == pstcQprc)
    {
        return ErrorInvalidParameter;
    }

    pstcQprc->QRCR = u16RcValue;
    return Ok;
}


/**
 ******************************************************************************
 ** \brief Get count value of Revolution Counter
 **
 ** \param [in]  pstcQprc  Pointer to QPRC instance
 **
 ** \retval Count value
 **
 ** \note If pstcQprc = NULL, the return value will be 0xFFFF.
 ******************************************************************************/
uint16_t Qprc_GetRcCount ( volatile stc_qprcn_t *pstcQprc )
{
    if (NULL == pstcQprc)
    {
        return 0xFFFFu;
    }

    return (uint16_t)pstcQprc->QPCR;
}

/**
 ******************************************************************************
 ** \brief Set maximum count value of Position Counter
 **
 ** \param [in]  pstcQprc           Pointer to QPRC instance
 ** \param [in]  u16PcMaxValue      Maximum count value
 **
 ** \retval Ok                    Maximum count value has been setup
 ** \retval ErrorInvalidParameter pstcQprc == NULL
 ******************************************************************************/
en_result_t Qprc_SetPcMaxValue( volatile stc_qprcn_t *pstcQprc,
                                        uint16_t u16PcMaxValue )
{
    if (NULL == pstcQprc)
    {
        return ErrorInvalidParameter;
    }

    pstcQprc->QMPR = u16PcMaxValue;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get maximum count value of Position Counter
 **
 ** \param [in]  pstcQprc  Pointer to QPRC instance
 **
 ** \retval  PcMaxValue
 **
 ** \note If pstcQprc = NULL, the return value will be 0xFFFF.
 ******************************************************************************/
uint16_t Qprc_GetPcMaxValue(volatile stc_qprcn_t *pstcQprc)
{
    if (NULL == pstcQprc)
    {
        return 0xFFFFu;
    }

    return (uint16_t)pstcQprc->QMPR;
}

/**
 ******************************************************************************
 ** \brief Set compare value of Position counter
 **
 ** \param [in]  pstcQprc           Pointer to QPRC instance
 ** \param [in]  u16PcValue         Compare value
 **
 ** \retval Ok                    Compare value has been setup
 ** \retval ErrorInvalidParameter pstcQprc == NULL
 ******************************************************************************/

en_result_t Qprc_SetPcCompareValue( volatile stc_qprcn_t *pstcQprc,
                                    uint16_t u16PcValue )
{
    if (NULL == pstcQprc)
    {
        return ErrorInvalidParameter;
    }

    pstcQprc->QPCCR = u16PcValue;

    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get compare value of Position counter
 **
 ** \param [in]  pstcQprc  Pointer to QPRC instance 
 **
 ** \retval Count value
 ******************************************************************************/

uint16_t Qprc_GetPcCompareValue( volatile stc_qprcn_t *pstcQprc)
{
    if (NULL == pstcQprc)
    {
        return ErrorInvalidParameter;
    }

    return (uint16_t)pstcQprc->QPCCR;
}


/**
 ******************************************************************************
 ** \brief Set compare value of Position and Revolution Counter
 **
 ** \param [in]  pstcQprc          Pointer to QPRC instance
 ** \param [in]  u16PcRcValue      Compare value
 **
 ** \retval Ok                     Compare value has been setup
 ** \retval ErrorInvalidParameter  pstcQprc == NULL
 **
 ** \note the object with which this value is compared depends on the compare 
 **       mode setting in the Qprc_Init(), plesae refer to the structure type
 **       #en_qprc_compmode_t.
 ******************************************************************************/
en_result_t Qprc_SetPcRcCompareValue( volatile stc_qprcn_t *pstcQprc,
                                                uint16_t u16PcRcValue )
{
    if (NULL == pstcQprc)
    {
        return ErrorInvalidParameter;
    }

    pstcQprc->QPRCR = u16PcRcValue;

    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get compare value of Position and Revolution Counter
 **
 ** \param [in]  pstcQprc  Pointer to QPRC instance
 **
 ** \retval Count value
 **
 ** If pstcQprc = NULL, the return will be 0xFFFF.
 ******************************************************************************/
uint16_t Qprc_GetPcRcCompareValue(volatile stc_qprcn_t *pstcQprc)
{
    if (NULL == pstcQprc)
    {
        return 0xFFFFu;
    }

    return (uint16_t)pstcQprc->QPRCR;
}

/**
 ******************************************************************************
 ** \brief Set Position Counter mode
 **
 ** \param [in]  pstcQprc               Pointer to QPRC instance
 ** \param [in]  enMode                 Position Counter mode
 **
 ** \retval Ok                          Position Counter mode has been setup
 ** \retval ErrorInvalidParameter       If one of following conditions are met:
 **                                     - pstcQprc == NULL
 **                                     - enMode  > QprcPcMode3
 **                                     - Other invalid configuration
 ******************************************************************************/

en_result_t Qprc_ConfigPcMode( volatile stc_qprcn_t *pstcQprc,
                                        en_qprc_pcmode_t enMode )
{
    if ((NULL == pstcQprc) ||
        (enMode > QprcPcMode3))
    {
        return ErrorInvalidParameter;
    }

    switch(enMode)
    {
        case QprcPcMode0: //Disable position counter
        {
            pstcQprc->QCR_f.PCM = 0u;
        }
        break;

        case QprcPcMode1: //Increments with AIN active edge and decrements with BIN active edge
        {
            pstcQprc->QCR_f.PCM = 1u;
        }
        break;

        case QprcPcMode2: //Phase difference count mode: Counts up if AIN is leading BIN, down if BIN leading.
        {
            pstcQprc->QCR_f.PCM = 2u;
        }
        break;

        case QprcPcMode3: // Directional count mode: Counts up/down with BIN active edge and AIN level
        {
            pstcQprc->QCR_f.PCM = 3u;
        }
        break;

        default:
            return ErrorInvalidParameter;
    }

    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set Revolution Counter mode
 **
 ** \param [in]  pstcQprc         Pointer to QPRC instance
 ** \param [in]  enMode           Revolution Counter mode
 **
 ** \retval Ok                    Revolution Counter mode has been setup
 ** \retval ErrorInvalidParameter If one of following confitions are met:
 **                               - pstcQprc == NULL
 **                               - enMode  > QprcRcMode3
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Qprc_ConfigRcMode( volatile stc_qprcn_t *pstcQprc,
                                        en_qprc_rcmode_t enMode )
{
    if ((NULL == pstcQprc) ||
        (enMode > QprcRcMode3))
    {
        return ErrorInvalidParameter;
    }

    switch(enMode)
    {
        case QprcRcMode0:  // RC_Mode0: Disable revolution counter
        {
            pstcQprc->QCR_f.RCM = 0u;
        }
        break;

        case QprcRcMode1: // Up/down count of RC with ZIN active edge
        {
            pstcQprc->QCR_f.RCM = 1u;
        }
        break;

        case QprcRcMode2: // Up/down count of RC on over or underflow in position count match
        {
            pstcQprc->QCR_f.RCM = 2u;
        }
        break;

        case QprcRcMode3: // Up/down count of RC on over or underflow in position count match and ZIN active edge
        {
            pstcQprc->QCR_f.RCM = 3u;
        }
        break;

        default:
            return ErrorInvalidParameter;
    }

    return Ok;
}

#if (PDL_INTERRUPT_ENABLE_QPRC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_QPRC1 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_QPRC2 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_QPRC3 == PDL_ON)
/**
 ******************************************************************************
 ** \brief Enable QPRC interrupt
 **
 ** \param [in]  pstcQprc  Pointer to QPRC instance
 ** \param [in]  enIrqSel  Interrupt type
 **
 ** \retval Ok                    QPRC interrupts has been enabled
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcQprc invalid
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Qprc_EnableIrq( volatile stc_qprcn_t* pstcQprc, en_qprc_irq_sel_t enIrqSel)
{
    if ( NULL == pstcQprc )
    {
        return ErrorInvalidParameter;
    }

    switch(enIrqSel)
    {
        case QprcPcOfUfZeroIrq:
            pstcQprc->QICRL_f.OUZIE = 1u;
            break;

        case QprcPcMatchIrq:
            pstcQprc->QICRL_f.QPCMIE = 1u;
            break;

        case QprcPcRcMatchIrq:
            pstcQprc->QICRL_f.QPRCMIE = 1u;
            break;

        case QprcPcMatchRcMatchIrq:
            pstcQprc->QICRH_f.QPCNRCMIE = 1u;
            break;

        case QprcPcCountInvertIrq:
            pstcQprc->QICRH_f.CDCIE = 1u;
            break;

        case QprcRcOutrangeIrq:
            pstcQprc->QECR_f.ORNGIE = 1u;
            break;

        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable QPRC interrupt
 **
 ** \param [in]  pstcQprc               Pointer to QPRC instance
 ** \param [in]  enIrqSel               Interrupt type
 **
 ** \retval Ok                    PC match interrupt enable bit has been setup
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcQprc invalid
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Qprc_DisableIrq( volatile stc_qprcn_t* pstcQprc, en_qprc_irq_sel_t enIrqSel)
{
    if ( NULL == pstcQprc )
    {
        return ErrorInvalidParameter;
    }

    switch(enIrqSel)
    {
        case QprcPcOfUfZeroIrq:
            pstcQprc->QICRL_f.OUZIE = 0u;
            break;

        case QprcPcMatchIrq:
            pstcQprc->QICRL_f.QPCMIE = 0u;
            break;

        case QprcPcRcMatchIrq:
            pstcQprc->QICRL_f.QPRCMIE = 0u;
            break;

        case QprcPcMatchRcMatchIrq:
            pstcQprc->QICRH_f.QPCNRCMIE = 0u;
            break;

        case QprcPcCountInvertIrq:
            pstcQprc->QICRH_f.CDCIE = 0u;
            break;

        case QprcRcOutrangeIrq:
            pstcQprc->QECR_f.ORNGIE = 0u;
            break;

        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

#endif

/**
 ******************************************************************************
 ** \brief Get interrupt flag of QPRC
 **
 ** \param [in]  pstcQprc            Pointer to QPRC instance
 ** \param [in]  enIrqSel            Interrupt type
 **
 ** \retval PdlClr                   Interrupt doesn't occur selected by enIntType
 ** \retval PdlSet                   Interrupt occurs selected by enIntType
 ******************************************************************************/
en_irq_flag_t Qprc_GetIrqFlag( volatile stc_qprcn_t *pstcQprc,
                               en_qprc_irq_sel_t enIrqSel)
{
    en_irq_flag_t enIrqFlag = PdlClr;

    if (NULL == pstcQprc)
    {
        return PdlClr;
    }

    switch(enIrqSel)
    {
        case QprcPcOfUfZeroIrq:
            if(PdlSet == pstcQprc->QICRL_f.ZIIF)
            {
                enIrqFlag = PdlSet;
                break;
            }
            
            if(PdlSet == pstcQprc->QICRL_f.OFDF)
            {
                enIrqFlag = PdlSet;
                break;
            }
            
            if(PdlSet == pstcQprc->QICRL_f.UFDF)
            {
                enIrqFlag = PdlSet;
                break;
            }
            break;

        case QprcPcMatchIrq:
            enIrqFlag = (1u == pstcQprc->QICRL_f.QPCMF) ? PdlSet : PdlClr ;
            break;

        case QprcPcRcMatchIrq:
            enIrqFlag = (1u == pstcQprc->QICRL_f.QPRCMF) ? PdlSet : PdlClr ;
            break;

        case QprcPcMatchRcMatchIrq:
            enIrqFlag = (1u == pstcQprc->QICRH_f.QPCNRCMF) ? PdlSet : PdlClr ;
            break;

        case QprcPcCountInvertIrq:
            enIrqFlag = (1u == pstcQprc->QICRH_f.CDCF) ? PdlSet : PdlClr ;
            break;

        case QprcRcOutrangeIrq:
            enIrqFlag = (1u == pstcQprc->QECR_f.ORNGF) ? PdlSet : PdlClr ;
            break;

        default:
            break;
    }

    return enIrqFlag;
}


/**
 ******************************************************************************
 ** \brief Clear interrupt flag of QPRC
 **
 ** \param [in]  pstcQprc            Pointer to QPRC instance
 ** \param [in]  enIrqSel            Interrupt type
 **
 ** \retval Ok                       Clear interrupt flag successfully
 ** \retval ErrorInvalidParameter    If one of following conditions are met:
 **                                  - pstcQprc invalid
 **                                  - enIntType > QprcRcOutrangeInt
 **                                  - Other invalid configuration
 ******************************************************************************/
en_result_t Qprc_ClrIrqFlag( volatile stc_qprcn_t *pstcQprc,
                             en_qprc_irq_sel_t enIrqSel)
{
    if (NULL == pstcQprc)
    {
        return ErrorInvalidParameter;
    }

    switch(enIrqSel)
    {
        case QprcPcOfUfZeroIrq:
            pstcQprc->QICRL_f.ZIIF = PdlClr;
            pstcQprc->QICRL_f.OFDF = PdlClr;
            pstcQprc->QICRL_f.UFDF = PdlClr;
            break;

        case QprcPcMatchIrq:
            pstcQprc->QICRL_f.QPCMF = PdlClr;
            break;

        case QprcPcRcMatchIrq:
            pstcQprc->QICRL_f.QPRCMF = PdlClr;
            break;

        case QprcPcMatchRcMatchIrq:
            pstcQprc->QICRH_f.QPCNRCMF = PdlClr;
            break;

        case QprcPcCountInvertIrq:
            pstcQprc->QICRH_f.CDCF = PdlClr;
            break;

        case QprcRcOutrangeIrq:
            pstcQprc->QECR_f.ORNGF = PdlClr;
            break;

        default:
            break;
    }

    return Ok;

}

/**
 ******************************************************************************
 ** \brief Get last position counter flow direction
 **
 ** \param [in]  pstcQprc             Pointer to QPRC instance
 **
 ** \retval PdlClr                    The position counter was incremented.
 ** \retval PdlSet                    The position counter was decremented.
 ******************************************************************************/
en_stat_flag_t Qprc_GetPcOfUfDir( volatile stc_qprcn_t *pstcQprc )
{
    en_stat_flag_t enStatFlag = PdlClr;

    if (NULL == pstcQprc)
    {
        return PdlClr;
    }

    enStatFlag = (1 == pstcQprc->QICRH_f.DIROU) ? PdlSet : PdlClr ;
    return enStatFlag;
}

/**
 ******************************************************************************
 ** \brief Get last position counter direction
 **
 ** \param [in]  pstcQprc             Pointer to QPRC instance
 **
 ** \retval PdlClr                    The position counter was incremented.
 ** \retval PdlSet                    The position counter was decremented.
 ******************************************************************************/
en_stat_flag_t Qprc_GetPcDir( volatile stc_qprcn_t *pstcQprc )
{
    en_stat_flag_t enStatFlag = PdlClr;

    if (NULL == pstcQprc)
    {
        return PdlClr;
    }

    enStatFlag = (1u == pstcQprc->QICRH_f.DIRPC) ? PdlSet : PdlClr ;

    return enStatFlag;
}

#endif // #if (defined(PDL_PERIPHERAL_QPRC_ACTIVE))

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
