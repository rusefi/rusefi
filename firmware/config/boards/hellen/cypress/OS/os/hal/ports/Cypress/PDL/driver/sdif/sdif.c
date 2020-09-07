/*******************************************************************************
* \file              sdif.c
* 
* \version           1.20
*
* \brief             SD Card Interface driver.
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
#include "sdif/sdif.h"

#if (defined(PDL_PERIPHERAL_SD_ACTIVE))

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/
/// Look-up table for all enabled ADC instances and their internal data
stc_sdif_instance_data_t m_astcSdifInstanceDataLut[SDIF_INSTANCE_COUNT] =
{
#if (PDL_PERIPHERAL_ENABLE_SD0 == PDL_ON)
    { 
        &SDIF0,  // pstcInstance
        {
            {0u, 0u, 0u, 0u, 0u, 0u,0u, 0u, 0u},
            {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}
        } // stcInternData (not initialized yet)
    }
#endif

};

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/
#if (PDL_INTERRUPT_ENABLE_SD0 == PDL_ON)
static void SdifInitNvic(volatile stc_sdifn_t* pstcSdif);
static void SdifDeInitNvic(volatile stc_sdifn_t* pstcSdif);
static stc_sdif_intern_data_t* SdifGetInternDataPtr(volatile stc_sdifn_t* pstcSdif);
#endif

/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/

/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/

/******************************************************************************/
/* Local Functions                                                            */
/******************************************************************************/

#if (PDL_INTERRUPT_ENABLE_SD0 == PDL_ON) 
/**
 ******************************************************************************
 ** \brief SDIF instance interrupt service routine
 **
 ** SDIF instance interrupt service routine, clear interrupt cause and
 **
 ** \param  pstcSdif              Pointer to SDIF instance
 ** \param  pstcSdifInternData    Pointer to SDIF intern data
 **        
 ******************************************************************************/
void SdifIrqHandler( volatile stc_sdifn_t*   pstcSdif,
                     stc_sdif_intern_data_t* pstcSdifInternData)
{
    /* Normal interrupt handler */
    if (1u == pstcSdif->SNINTST_f.CMDCMPLT)   // Command complete
    {
        pstcSdif->SNINTST = 0x0001u; // Clear interrupt flag
        if (NULL != pstcSdifInternData->stcNormalIrqCb.pfnCommandCompleteIrqCb)
        {
            pstcSdifInternData->stcNormalIrqCb.pfnCommandCompleteIrqCb();
        }
    }

    if (1u == pstcSdif->SNINTST_f.TRSFCMPLT)   // Transfer complete
    {
        pstcSdif->SNINTST = 0x0002u; // Clear interrupt flag
        if (NULL != pstcSdifInternData->stcNormalIrqCb.pfnTransferCompleteIrqCb)
        {
            pstcSdifInternData->stcNormalIrqCb.pfnTransferCompleteIrqCb();
        }
    }
    
    if (1u == pstcSdif->SNINTST_f.BLCKGEVNT)   // block gap event
    {
        pstcSdif->SNINTST = 0x0004u; // Clear interrupt flag
        if (NULL != pstcSdifInternData->stcNormalIrqCb.pfnBlockGapIrqCb)
        {
            pstcSdifInternData->stcNormalIrqCb.pfnBlockGapIrqCb();
        }
    }
    
    if (1u == pstcSdif->SNINTST_f.DMAINT)   // DMA interrupt
    {
        pstcSdif->SNINTST = 0x0008u; // Clear interrupt flag
        if (NULL != pstcSdifInternData->stcNormalIrqCb.pfnDmaIrqCb)
        {
            pstcSdifInternData->stcNormalIrqCb.pfnDmaIrqCb();
        }
    }
    
    if (1u == pstcSdif->SNINTST_f.BUFWRRDY)   // Buffer write ready
    {
        pstcSdif->SNINTST = 0x0010u; // Clear interrupt flag
        if (NULL != pstcSdifInternData->stcNormalIrqCb.pfnBufferWriteReadyIrqCb)
        {
            pstcSdifInternData->stcNormalIrqCb.pfnBufferWriteReadyIrqCb();
        }
    }
    
    if (1u == pstcSdif->SNINTST_f.BUFRDRDY)   // Buffer read ready
    {
        pstcSdif->SNINTST = 0x0020u; // Clear interrupt flag
        if (NULL != pstcSdifInternData->stcNormalIrqCb.pfnBufferReadReadyIrqCb)
        {
            pstcSdifInternData->stcNormalIrqCb.pfnBufferReadReadyIrqCb();
        }
    }
    
    if (1u == pstcSdif->SNINTST_f.CARDINS)   // Card insetion
    {
        pstcSdif->SNINTST = 0x0040u; // Clear interrupt flag
        if (NULL != pstcSdifInternData->stcNormalIrqCb.pfnCardInsertIrqCb)
        {
            pstcSdifInternData->stcNormalIrqCb.pfnCardInsertIrqCb();
        }
    }
    
    if (1u == pstcSdif->SNINTST_f.CARDRMV)   // Card removal
    {
        pstcSdif->SNINTST = 0x0080u; // Clear interrupt flag
        if (NULL != pstcSdifInternData->stcNormalIrqCb.pfnCardRemovalIrqCb)
        {
            pstcSdifInternData->stcNormalIrqCb.pfnCardRemovalIrqCb();
        }
    }
    
    if (1u == pstcSdif->SNINTST_f.CARDINT)   // Card interrupt
    {
        pstcSdif->SNINTST = 0x0100u; // Clear interrupt flag
        if (NULL != pstcSdifInternData->stcNormalIrqCb.pfnCardIrqCb)
        {
            pstcSdifInternData->stcNormalIrqCb.pfnCardIrqCb();
        }
    }
    
    /* Error interrupt handler */
    if (1u == pstcSdif->SEINTST_f.CMDTOERR)   // Command timeout error
    {
        pstcSdif->SEINTST = 0x0001u; // Clear interrupt flag
        if (NULL != pstcSdifInternData->stcErrIrqCb.pfnCmdTimeoutErrIrqCb)
        {
            pstcSdifInternData->stcErrIrqCb.pfnCmdTimeoutErrIrqCb();
        }
    }
    
    if (1u == pstcSdif->SEINTST_f.CMDCRCERR)   // Command CRC error
    {
        pstcSdif->SEINTST = 0x0002u; // Clear interrupt flag
        if (NULL != pstcSdifInternData->stcErrIrqCb.pfnCmdCrcErrIrqCb)
        {
            pstcSdifInternData->stcErrIrqCb.pfnCmdCrcErrIrqCb();
        }
    }
    
    if (1u == pstcSdif->SEINTST_f.CMDEBERR)   // Command end bit error
    {
        pstcSdif->SEINTST = 0x0004u; // Clear interrupt flag
        if (NULL != pstcSdifInternData->stcErrIrqCb.pfnCmdEndBitErrIrqCb)
        {
            pstcSdifInternData->stcErrIrqCb.pfnCmdEndBitErrIrqCb();
        }
    }
    
    if (1u == pstcSdif->SEINTST_f.CMDIDXERR)   // Command index error
    {
        pstcSdif->SEINTST = 0x0008u; // Clear interrupt flag
        if (NULL != pstcSdifInternData->stcErrIrqCb.pfnCmdIndexErrIrqCb)
        {
            pstcSdifInternData->stcErrIrqCb.pfnCmdIndexErrIrqCb();
        }
    }
    
    if (1u == pstcSdif->SEINTST_f.DTTOERR)   // Data timeout error
    {
        pstcSdif->SEINTST = 0x0010u; // Clear interrupt flag
        if (NULL != pstcSdifInternData->stcErrIrqCb.pfnDataTimeoutErrIrqCb)
        {
            pstcSdifInternData->stcErrIrqCb.pfnDataTimeoutErrIrqCb();
        }
    }
    
    if (1u == pstcSdif->SEINTST_f.DTEBERR)   // Data end bit error
    {
        pstcSdif->SEINTST = 0x0020u; // Clear interrupt flag
        if (NULL != pstcSdifInternData->stcErrIrqCb.pfnDataEndBitErrIrqCb)
        {
            pstcSdifInternData->stcErrIrqCb.pfnDataEndBitErrIrqCb();
        }
    }
    
    if (1u == pstcSdif->SEINTST_f.DTCRCERR)   // Data CRC error
    {
        pstcSdif->SEINTST = 0x0040u; // Clear interrupt flag
        if (NULL != pstcSdifInternData->stcErrIrqCb.pfnDataCrcErrIrqCb)
        {
            pstcSdifInternData->stcErrIrqCb.pfnDataCrcErrIrqCb();
        }
    }
    
    if (1u == pstcSdif->SEINTST_f.CRTLMTERR)   // Current limitation error
    {
        pstcSdif->SEINTST = 0x0080u; // Clear interrupt flag
        if (NULL != pstcSdifInternData->stcErrIrqCb.pfnCurrentLimitErrIrqCb)
        {
            pstcSdifInternData->stcErrIrqCb.pfnCurrentLimitErrIrqCb();
        }
    }
    
    if (1u == pstcSdif->SEINTST_f.ACMD12ERR)   // Auto CMD12 error
    {
        pstcSdif->SEINTST = 0x0100u; // Clear interrupt flag
        if (NULL != pstcSdifInternData->stcErrIrqCb.pfnAutoCmdErrIrqCb)
        {
            pstcSdifInternData->stcErrIrqCb.pfnAutoCmdErrIrqCb();
        }
    }
    
    if (1u == pstcSdif->SEINTST_f.ADMAERR)   // ADMA error
    {
        pstcSdif->SEINTST = 0x0200u; // Clear interrupt flag
        if (NULL != pstcSdifInternData->stcErrIrqCb.pfnAdmaErrIrqCb)
        {
            pstcSdifInternData->stcErrIrqCb.pfnAdmaErrIrqCb();
        }
    }
    
    if (1u == pstcSdif->SEINTST_f.TUNINGERR)   // Tuning error
    {
        pstcSdif->SEINTST = 0x0400u; // Clear interrupt flag
        if (NULL != pstcSdifInternData->stcErrIrqCb.pfnTuningErrIrqCb)
        {
            pstcSdifInternData->stcErrIrqCb.pfnTuningErrIrqCb();
        }
    }
    
    return;
} // SdifIrqHandler

/**
 ******************************************************************************
 ** \brief Set NVIC Interrupt depending on SDIF instance
 **
 ** \param pstcSdif Pointer to SDIF instance
 **
 ******************************************************************************/
static void SdifInitNvic(volatile stc_sdifn_t* pstcSdif)
{
    NVIC_ClearPendingIRQ(SD_IRQn);
    NVIC_EnableIRQ(SD_IRQn);
    NVIC_SetPriority(SD_IRQn, PDL_IRQ_LEVEL_SD);   
} // SdifInitNvic

/**
 ******************************************************************************
 ** \brief Clear NVIC Interrupt depending on SDIF instance
 **
 ** \param pstcSdif Pointer to SDIF instance
 **
 ******************************************************************************/
static void SdifDeInitNvic(volatile stc_sdifn_t* pstcSdif)
{
    NVIC_ClearPendingIRQ(SD_IRQn);
    NVIC_DisableIRQ(SD_IRQn);
    NVIC_SetPriority(SD_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);   

} // SdifDeInitNvic
#endif // #if (PDL_INTERRUPT_ENABLE_SD0 == PDL_ON)

/**
 ******************************************************************************
 ** \brief Return the internal data for a certain SDIF instance.
 **
 ** \param pstcSdif Pointer to SDIF instance
 **
 ** \return Pointer to internal data or NULL if instance is not enabled (or not known)
 **
 ******************************************************************************/
static stc_sdif_intern_data_t* SdifGetInternDataPtr(volatile stc_sdifn_t* pstcSdif) 
{
    uint8_t u8Instance;
   
    for (u8Instance = 0u; u8Instance < SDIF_INSTANCE_COUNT; u8Instance++)
    {
        if (pstcSdif == m_astcSdifInstanceDataLut[u8Instance].pstcInstance)
        {
            return &m_astcSdifInstanceDataLut[u8Instance].stcInternData;
        }
    }

    return NULL;
}


/******************************************************************************/
/* Global Functions                                                           */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief Initialize SD interface
 **
 ** This function initializes an SDIF module
 **
 ** \param [in]  pstcSdif         SDIF instance
 ** \param [in]  pstcConfig       SDIF module configuration 
 **
 ** \retval Ok                    SDIF initialized normally
 ** \retval ErrorInvalidParameter If one of following cases matchs: 
 **                               - pstcSdif == NULL
 **                               - pstcConfig == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t  Sdif_Init( volatile stc_sdifn_t* pstcSdif,
                        const stc_sdif_config_t*    pstcConfig )
{
    // Pointer to internal data
    stc_sdif_intern_data_t* pstcSdifInternData ; 
    
    // Check for NULL pointer
    if ((NULL == pstcSdif) ||
        (NULL == pstcConfig))
    {
        return ErrorInvalidParameter ;
    } 
    
    // Get pointer to internal data structure ...
    pstcSdifInternData = SdifGetInternDataPtr( pstcSdif ) ;
    
    // Check for instance available or not
    if(NULL == pstcSdifInternData)
    {
        return ErrorInvalidParameter ;
    }
    
    // Select ADMA or not
    pstcSdif->SHCTL1_f.DMASEL = ((TRUE == pstcConfig->bSelAdma) ? 2u : 0u);
    
    // Set speed mode
    pstcSdif->SHCTL1_f.HIGHSPDEN = ((TRUE == pstcConfig->bEnableHighSpeed) ? 1u : 0u);
    
    // Set bit width
    pstcSdif->SHCTL1_f.DATAWIDTH = ((TRUE == pstcConfig->bEnable4BitMode) ? 1u : 0u);
    
    // Enable all status
    pstcSdif->SNINTSTE = 0x1FFFu;
    pstcSdif->SEINTSTE = 0x07FFu;

#if (PDL_INTERRUPT_ENABLE_SD0 == PDL_ON)    
    // Enable normal interrupt signal
    if(NULL != pstcConfig->pstcNormalIrqEn)
    {
        pstcSdif->SNINTSGE = pstcConfig->pstcNormalIrqEn->u16NormalStatus;
    }
    
    // Set normal interrupt callback functions
    if(NULL != pstcConfig->pstcNormalIrqCb)
    {
        pstcSdifInternData->stcNormalIrqCb.pfnCommandCompleteIrqCb      = pstcConfig->pstcNormalIrqCb->pfnCommandCompleteIrqCb;
        pstcSdifInternData->stcNormalIrqCb.pfnTransferCompleteIrqCb     = pstcConfig->pstcNormalIrqCb->pfnTransferCompleteIrqCb;
        pstcSdifInternData->stcNormalIrqCb.pfnBlockGapIrqCb             = pstcConfig->pstcNormalIrqCb->pfnBlockGapIrqCb;
        pstcSdifInternData->stcNormalIrqCb.pfnDmaIrqCb                  = pstcConfig->pstcNormalIrqCb->pfnDmaIrqCb;
        pstcSdifInternData->stcNormalIrqCb.pfnBufferWriteReadyIrqCb     = pstcConfig->pstcNormalIrqCb->pfnBufferWriteReadyIrqCb;
        pstcSdifInternData->stcNormalIrqCb.pfnBufferReadReadyIrqCb      = pstcConfig->pstcNormalIrqCb->pfnBufferReadReadyIrqCb;
        pstcSdifInternData->stcNormalIrqCb.pfnCardInsertIrqCb           = pstcConfig->pstcNormalIrqCb->pfnCardInsertIrqCb;
        pstcSdifInternData->stcNormalIrqCb.pfnCardRemovalIrqCb          = pstcConfig->pstcNormalIrqCb->pfnCardRemovalIrqCb;
        pstcSdifInternData->stcNormalIrqCb.pfnCardIrqCb                 = pstcConfig->pstcNormalIrqCb->pfnCardIrqCb;
    }
    
    // Enable error interrupt signal
    if(NULL != pstcConfig->pstcErrIrqEn)
    {
        pstcSdif->SEINTSGE = pstcConfig->pstcErrIrqEn->u16ErrStatus;
    }
    
    // Set error interrupt callback functions
    if(NULL != pstcConfig->pstcErrIrqCb)
    {
        pstcSdifInternData->stcErrIrqCb.pfnCmdTimeoutErrIrqCb       = pstcConfig->pstcErrIrqCb->pfnCmdTimeoutErrIrqCb;
        pstcSdifInternData->stcErrIrqCb.pfnCmdCrcErrIrqCb           = pstcConfig->pstcErrIrqCb->pfnCmdCrcErrIrqCb;
        pstcSdifInternData->stcErrIrqCb.pfnCmdEndBitErrIrqCb        = pstcConfig->pstcErrIrqCb->pfnCmdEndBitErrIrqCb;
        pstcSdifInternData->stcErrIrqCb.pfnCmdIndexErrIrqCb         = pstcConfig->pstcErrIrqCb->pfnCmdIndexErrIrqCb;
        pstcSdifInternData->stcErrIrqCb.pfnDataTimeoutErrIrqCb      = pstcConfig->pstcErrIrqCb->pfnDataTimeoutErrIrqCb;
        pstcSdifInternData->stcErrIrqCb.pfnDataEndBitErrIrqCb       = pstcConfig->pstcErrIrqCb->pfnDataEndBitErrIrqCb;
        pstcSdifInternData->stcErrIrqCb.pfnDataCrcErrIrqCb          = pstcConfig->pstcErrIrqCb->pfnDataCrcErrIrqCb;
        pstcSdifInternData->stcErrIrqCb.pfnCurrentLimitErrIrqCb     = pstcConfig->pstcErrIrqCb->pfnCurrentLimitErrIrqCb;
        pstcSdifInternData->stcErrIrqCb.pfnAutoCmdErrIrqCb          = pstcConfig->pstcErrIrqCb->pfnAutoCmdErrIrqCb;
        pstcSdifInternData->stcErrIrqCb.pfnAdmaErrIrqCb             = pstcConfig->pstcErrIrqCb->pfnAdmaErrIrqCb;
        pstcSdifInternData->stcErrIrqCb.pfnTuningErrIrqCb           = pstcConfig->pstcErrIrqCb->pfnTuningErrIrqCb;
    }
    
    // Set NVIC
    if(TRUE == pstcConfig->bTouchNvic)
    {
        SdifInitNvic(pstcSdif);
    }

#endif
    
    return Ok;
} // Sdif_Init

/**
 ******************************************************************************
 ** \brief De-Initialize SD interface
 **
 ** This function de-initializes an SDIF module
 **
 ** \param [in]  pstcSdif          SDIF instance
 ** \param [in]  bTouchIrq         Disable IRQ or not
 ** \param [in]  bTouchIrqCb       Reset callback functions in internal data or not
 ** \param [in]  bTouchNvic        Disable NVIC or not
 **
 ** \retval Ok                    SDIF de-initialized normally
 ** \retval ErrorInvalidParameter pstcSdif == NULL
 ******************************************************************************/
en_result_t  Sdif_DeInit( volatile stc_sdifn_t* pstcSdif, 
                          boolean_t bTouchIrq,
                          boolean_t bTouchIrqCb,
                          boolean_t bTouchNvic)
{
#if (PDL_INTERRUPT_ENABLE_SD0 == PDL_ON)      
    // Pointer to internal data
    stc_sdif_intern_data_t* pstcSdifInternData ; 
    
    // Get pointer to internal data structure ...
    pstcSdifInternData = SdifGetInternDataPtr( pstcSdif ) ;
    
    if(NULL == pstcSdifInternData)
    {
        return ErrorInvalidParameter ;
    }
#endif    
    
    // Check for NULL pointer
    if (NULL == pstcSdif)
    {
        return ErrorInvalidParameter ;
    } 
    
    // Clear clock control and power control registers
    pstcSdif->SCLKCTL = 0x0000u;
    pstcSdif->SPWRCTL = 0x00u;

#if (PDL_INTERRUPT_ENABLE_SD0 == PDL_ON)        
    if(TRUE == bTouchIrq)
    {
        pstcSdif->SNINTSGE = 0x0000u;
        pstcSdif->SEINTSGE = 0x0000u;
    }
    
    if(TRUE == bTouchIrqCb)
    {
        pstcSdifInternData->stcNormalIrqCb.pfnCommandCompleteIrqCb      = NULL;
        pstcSdifInternData->stcNormalIrqCb.pfnTransferCompleteIrqCb     = NULL;
        pstcSdifInternData->stcNormalIrqCb.pfnBlockGapIrqCb             = NULL;
        pstcSdifInternData->stcNormalIrqCb.pfnDmaIrqCb                  = NULL;
        pstcSdifInternData->stcNormalIrqCb.pfnBufferWriteReadyIrqCb     = NULL;
        pstcSdifInternData->stcNormalIrqCb.pfnBufferReadReadyIrqCb      = NULL;
        pstcSdifInternData->stcNormalIrqCb.pfnCardInsertIrqCb           = NULL;
        pstcSdifInternData->stcNormalIrqCb.pfnCardRemovalIrqCb          = NULL;
        pstcSdifInternData->stcNormalIrqCb.pfnCardIrqCb                 = NULL;
        pstcSdifInternData->stcErrIrqCb.pfnCmdTimeoutErrIrqCb       = NULL;
        pstcSdifInternData->stcErrIrqCb.pfnCmdCrcErrIrqCb           = NULL;
        pstcSdifInternData->stcErrIrqCb.pfnCmdEndBitErrIrqCb        = NULL;
        pstcSdifInternData->stcErrIrqCb.pfnCmdIndexErrIrqCb         = NULL;
        pstcSdifInternData->stcErrIrqCb.pfnDataTimeoutErrIrqCb      = NULL;
        pstcSdifInternData->stcErrIrqCb.pfnDataEndBitErrIrqCb       = NULL;
        pstcSdifInternData->stcErrIrqCb.pfnDataCrcErrIrqCb          = NULL;
        pstcSdifInternData->stcErrIrqCb.pfnCurrentLimitErrIrqCb     = NULL;
        pstcSdifInternData->stcErrIrqCb.pfnAutoCmdErrIrqCb          = NULL;
        pstcSdifInternData->stcErrIrqCb.pfnAdmaErrIrqCb             = NULL;
        pstcSdifInternData->stcErrIrqCb.pfnTuningErrIrqCb           = NULL;
    }
    
    if(TRUE == bTouchNvic)
    {
        SdifDeInitNvic(pstcSdif);
    }
#endif
    
    return Ok;
}

#if (PDL_INTERRUPT_ENABLE_SD0 == PDL_ON) 
/**
 ******************************************************************************
 ** \brief Enable SDIF normal interrupt
 **
 ** This function enable the SDIF interrupt which is selected.
 **
 ** \param [in]  pstcSdif         SDIF instance
 ** \param [in]  enNormalIrqSel   SDIF normal interrupt selection
 **
 ** \retval Ok                    Enable normal interrupt normally
 ** \retval ErrorInvalidParameter If one of following cases matchs: 
 **                               - pstcSdif == NULL
 **                               - enNormalIrqSel out of range
 ******************************************************************************/      
en_result_t  Sdif_EnableNormalIrq( volatile stc_sdifn_t* pstcSdif,
                                   en_sdif_normal_irq_sel_t enNormalIrqSel)
{
    // Check for NULL pointer
    if (NULL == pstcSdif) 
    {
        return ErrorInvalidParameter ;
    }
    
    switch (enNormalIrqSel)
    {
        case SdifCommandCompleteIrq:
            pstcSdif->SNINTSGE_f.CMDCMPLTG = 1u;
            break;
        case SdifTransferCompleteIrq:
            pstcSdif->SNINTSGE_f.TRSFCMPLTG = 1u;
            break;
        case SdifBlockGapEventIrq:
            pstcSdif->SNINTSGE_f.BLCKGEVNTG = 1u;
            break;
        case SdifDmaIrq:
            pstcSdif->SNINTSGE_f.DMAINTG = 1u;
            break;
        case SdifBufferWriteReadyIrq:
            pstcSdif->SNINTSGE_f.BUFWRRDYG = 1u;
            break;
        case SdifBufferReadReadyIrq:
            pstcSdif->SNINTSGE_f.BUFRDRDYG = 1u;
            break;
        case SdifCardInsertionIrq:
            pstcSdif->SNINTSGE_f.CARDINSG = 1u;
            break;
        case SdifCardRemovalIrq:
            pstcSdif->SNINTSGE_f.CARDRMVG = 1u;
            break;    
        case SdifCardIrq:
            pstcSdif->SNINTSGE_f.CARDINTG = 1u;
            break;  
        default:
            return ErrorInvalidParameter ;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable SDIF normal interrupt
 **
 ** This function disable the SDIF interrupt which is selected.
 **
 ** \param [in]  pstcSdif         SDIF instance
 ** \param [in]  enNormalIrqSel   SDIF normal interrupt selection
 **
 ** \retval Ok                    Disable normal interrupt normally
 ** \retval ErrorInvalidParameter If one of following cases matchs: 
 **                               - pstcSdif == NULL
 **                               - enNormalIrqSel out of range
 ******************************************************************************/      
en_result_t  Sdif_DisableNormalIrq( volatile stc_sdifn_t* pstcSdif,
                                   en_sdif_normal_irq_sel_t enNormalIrqSel)
{
    // Check for NULL pointer
    if (NULL == pstcSdif)
    {
        return ErrorInvalidParameter ;
    }
    
    switch (enNormalIrqSel)
    {
        case SdifCommandCompleteIrq:
            pstcSdif->SNINTSGE_f.CMDCMPLTG = 0u;
            break;
        case SdifTransferCompleteIrq:
            pstcSdif->SNINTSGE_f.TRSFCMPLTG = 0u;
            break;
        case SdifBlockGapEventIrq:
            pstcSdif->SNINTSGE_f.BLCKGEVNTG = 0u;
            break;
        case SdifDmaIrq:
            pstcSdif->SNINTSGE_f.DMAINTG = 0u;
            break;
        case SdifBufferWriteReadyIrq:
            pstcSdif->SNINTSGE_f.BUFWRRDYG = 0u;
            break;
        case SdifBufferReadReadyIrq:
            pstcSdif->SNINTSGE_f.BUFRDRDYG = 0u;
            break;
        case SdifCardInsertionIrq:
            pstcSdif->SNINTSGE_f.CARDINSG = 0u;
            break;
        case SdifCardRemovalIrq:
            pstcSdif->SNINTSGE_f.CARDRMVG = 0u;
            break;    
        case SdifCardIrq:
            pstcSdif->SNINTSGE_f.CARDINTG = 0u;
            break;  
        default:
            return ErrorInvalidParameter ;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Enable SDIF error interrupt
 **
 ** This function enable the SDIF error interrupt which is selected.
 **
 ** \param [in]  pstcSdif         SDIF instance
 ** \param [in]  enErrIrqSel      SDIF error interrupt selection
 **
 ** \retval Ok                    Enable error interrupt normally
 ** \retval ErrorInvalidParameter If one of following cases matchs: 
 **                               - pstcSdif == NULL
 **                               - enErrIrqSel out of range
 ******************************************************************************/      
en_result_t  Sdif_EnableErrIrq( volatile stc_sdifn_t* pstcSdif,
                                en_sdif_err_irq_sel_t enErrIrqSel)
{
    // Check for NULL pointer
    if (NULL == pstcSdif) 
    {
        return ErrorInvalidParameter ;
    }
    
    switch (enErrIrqSel)
    {
        case SdifCmdTimeoutErrIrq:
            pstcSdif->SEINTSGE_f.CMDTOERRG = 1u;
            break;
        case SdifCmdCrcErrIrq:
            pstcSdif->SEINTSGE_f.CMDCRCERRG = 1u;
            break;
        case SdifCmdEndBitErrIrq:
            pstcSdif->SEINTSGE_f.CMDEBERRG = 1u;
            break;
        case SdifCmdIndexErrIrq:
            pstcSdif->SEINTSGE_f.CMDIDXERRG = 1u;
            break;
        case SdifDataTimeoutErrIrq:
            pstcSdif->SEINTSGE_f.DTTOERRG = 1u;
            break;
        case SdifDataCrcErrIrq:
            pstcSdif->SEINTSGE_f.DTCRCERRG = 1u;
            break;
        case SdifDataEndBitErrIrq:
            pstcSdif->SEINTSGE_f.DTEBERRG = 1u;
            break;
        case SdifCurrentLimitErrIrq:
            pstcSdif->SEINTSGE_f.CRTLMTERRG = 1u;
            break;    
        case SdifAutoCmdErrIrq:
            pstcSdif->SEINTSGE_f.ACMD12ERRG = 1u;
            break;  
        case SdifAdmaErrIrq:
            pstcSdif->SEINTSGE_f.ADMAERRG = 1u;
            break;  
        case SdifTuningErrIrq:
            pstcSdif->SEINTSGE_f.TUNINGERRG = 1u;
            break;              
        default:
            return ErrorInvalidParameter ;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable SDIF error interrupt
 **
 ** This function disable the SDIF error interrupt which is selected.
 **
 ** \param [in]  pstcSdif         SDIF instance
 ** \param [in]  enErrIrqSel      SDIF error interrupt selection
 **
 ** \retval Ok                    Disable error interrupt normally
 ** \retval ErrorInvalidParameter If one of following cases matchs: 
 **                               - pstcSdif == NULL
 **                               - enErrIrqSel out of range
 ******************************************************************************/      
en_result_t  Sdif_DisableErrIrq( volatile stc_sdifn_t* pstcSdif,
                                 en_sdif_err_irq_sel_t enErrIrqSel)
{
    // Check for NULL pointer
    if (NULL == pstcSdif) 
    {
        return ErrorInvalidParameter ;
    }
    
    switch (enErrIrqSel)
    {
        case SdifCmdTimeoutErrIrq:
            pstcSdif->SEINTSGE_f.CMDTOERRG = 0u;
            break;
        case SdifCmdCrcErrIrq:
            pstcSdif->SEINTSGE_f.CMDCRCERRG = 0u;
            break;
        case SdifCmdEndBitErrIrq:
            pstcSdif->SEINTSGE_f.CMDEBERRG = 0u;
            break;
        case SdifCmdIndexErrIrq:
            pstcSdif->SEINTSGE_f.CMDIDXERRG = 0u;
            break;
        case SdifDataTimeoutErrIrq:
            pstcSdif->SEINTSGE_f.DTTOERRG = 0u;
            break;
        case SdifDataCrcErrIrq:
            pstcSdif->SEINTSGE_f.DTCRCERRG = 0u;
            break;
        case SdifDataEndBitErrIrq:
            pstcSdif->SEINTSGE_f.DTEBERRG = 0u;
            break;
        case SdifCurrentLimitErrIrq:
            pstcSdif->SEINTSGE_f.CRTLMTERRG = 0u;
            break;    
        case SdifAutoCmdErrIrq:
            pstcSdif->SEINTSGE_f.ACMD12ERRG = 0u;
            break;  
        case SdifAdmaErrIrq:
            pstcSdif->SEINTSGE_f.ADMAERRG = 0u;
            break;  
        case SdifTuningErrIrq:
            pstcSdif->SEINTSGE_f.TUNINGERRG = 0u;
            break;              
        default:
            return ErrorInvalidParameter ;
    }
    
    return Ok;
}

#endif

/**
 ******************************************************************************
 ** \brief Get error status of SDIF
 **
 ** This function gets the SDIF error status.
 **
 ** \param [in]  pstcSdif         SDIF instance
 ** \param [out] pstcStatus       Pointer to error status selection structure
 **
 ** \retval Ok                    Get error status of SDIF normally
 ** \retval ErrorInvalidParameter If one of following cases matchs:  
 **                               - pstcSdif == NULL
 **                               - pstcStatus == NULL
 ******************************************************************************/   
en_result_t Sdif_GetErrStatus( volatile stc_sdifn_t* pstcSdif, 
                               stc_sdif_err_irq_status_t* pstcStatus)
{
    // Check for NULL pointer
    if (NULL == pstcSdif) 
    {
        return ErrorInvalidParameter ;
    }
    
    pstcStatus->u16ErrStatus = pstcSdif->SEINTST;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Clear error status of SDIF
 **
 ** This function clears the SDIF error status which is selected.
 **
 ** \param [in]  pstcSdif         SDIF instance
 ** \param [in]  pstcStatus       Pointer to error status selection structure
 **
 ** \retval Ok                    Clear error status of SDIF normally
 ** \retval ErrorInvalidParameter If one of following cases matchs: 
 **                               - pstcSdif == NULL
 **                               - pstcStatus == NULL
 ******************************************************************************/ 
en_result_t  Sdif_ClrErrStatus( volatile stc_sdifn_t* pstcSdif, 
                                stc_sdif_err_irq_status_t* pstcStatus)
{
    // Check for NULL pointer
    if ((NULL == pstcSdif) || (NULL == pstcStatus))
    {
        return ErrorInvalidParameter ;
    }
    
    pstcSdif->SEINTST = pstcStatus->u16ErrStatus;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get normal status of SDIF
 **
 ** This function gets the SDIF normal status.
 **
 ** \param [in]  pstcSdif         SDIF instance
 ** \param [out] pstcStatus       Pointer to normal status selection structure
 **
 ** \retval Ok                    Get normal status of SDIF normally
 ** \retval ErrorInvalidParameter pstcSdif == NULL
 ******************************************************************************/  
en_result_t Sdif_GetNormalStatus( volatile stc_sdifn_t* pstcSdif, 
                                  stc_sdif_normal_irq_status_t* pstcStatus)
{
    // Check for NULL pointer
    if (NULL == pstcSdif) 
    {
        return ErrorInvalidParameter ;
    }
    
    pstcStatus->u16NormalStatus = pstcSdif->SNINTST;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Clear normal status of SDIF
 **
 ** This function clears the SDIF normal status which is selected.
 **
 ** \param [in]  pstcSdif         SDIF instance
 ** \param [in]  pstcStatus       Pointer to normal status selection structure
 **
 ** \retval Ok                    Clear normal status of SDIF normally
 ** \retval ErrorInvalidParameter If one of following cases matchs: 
 **                               - pstcSdif == NULL
 **                               - pstcStatus == NULL
 ******************************************************************************/ 
en_result_t Sdif_ClrNormalStatus( volatile stc_sdifn_t* pstcSdif, 
                                  stc_sdif_normal_irq_status_t* pstcStatus)
{
    // Check for NULL pointer
    if ((NULL == pstcSdif) || (NULL == pstcStatus))
    {
        return ErrorInvalidParameter ;
    }
    
    pstcSdif->SNINTST = pstcStatus->u16NormalStatus;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get error status of SDIF auto command
 **
 ** This function gets the SDIF error status of auto command
 **
 ** \param [in]  pstcSdif         SDIF instance
 ** \param [in]  pstcStatus       Pointer to normal status selection structure
 **
 ** \retval Ok                    Auto command error status of SDIF gotten normally
 ** \retval ErrorInvalidParameter If one of following cases matchs: 
 **                               - pstcSdif == NULL
 **                               - pstcStatus == NULL
 ******************************************************************************/ 
en_result_t  Sdif_GetAutoCommandErrStatus( volatile stc_sdifn_t* pstcSdif, 
                                           stc_sdif_auto_cmd_err_t* pstcStatus)
{
    // Check for NULL pointer
    if ((NULL == pstcSdif) || (NULL == pstcStatus))
    {
        return ErrorInvalidParameter ;
    }
    
    pstcStatus->u16AutoCmdErr = pstcSdif->SACMDEST;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Enable internal clock of SDIF
 **
 ** This function makes SD host controller start working.
 **
 ** \param [in]  pstcSdif         SDIF instance
 **
 ** \retval Ok                    Internal clock of SDIF enabled normally
 ** \retval ErrorInvalidParameter pstcSdif == NULL
 ******************************************************************************/ 
en_result_t  Sdif_EnableInternalClock( volatile stc_sdifn_t* pstcSdif )
{
    // Check for NULL pointer
    if (NULL == pstcSdif) 
    {
        return ErrorInvalidParameter ;
    }
    
    pstcSdif->SCLKCTL_f.INTLCLCKEN = 1u;
    
    while(1u != pstcSdif->SCLKCTL_f.INTLCLCKST);
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable internal clock of SDIF
 **
 ** This function makes SD host controller stop working.
 **
 ** \param [in]  pstcSdif         SDIF instance
 **
 ** \retval Ok                    Internal clock of SDIF enabled normally
 ** \retval ErrorInvalidParameter pstcSdif == NULL
 ******************************************************************************/ 
en_result_t  Sdif_DisableInternalClock( volatile stc_sdifn_t* pstcSdif )
{
    // Check for NULL pointer
    if (NULL == pstcSdif) 
    {
        return ErrorInvalidParameter ;
    }
    
    pstcSdif->SCLKCTL_f.INTLCLCKEN = 0u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Enable SDCLK output
 **
 ** SD host drives SDCLK line.
 **
 ** \param [in]  pstcSdif         SDIF instance
 **
 ** \retval Ok                    SDCLK output of SDIF enabled normally
 ** \retval ErrorInvalidParameter pstcSdif == NULL
 ******************************************************************************/ 
en_result_t   Sdif_EnableSdclk( volatile stc_sdifn_t* pstcSdif )
{
    // Check for NULL pointer
    if (NULL == pstcSdif) 
    {
        return ErrorInvalidParameter ;
    }
    
    pstcSdif->SCLKCTL_f.SDCLCKEN = 1u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable SDCLK output
 **
 ** SD host don't drive SDCLK line.
 **
 ** \param [in]  pstcSdif         SDIF instance
 **
 ** \retval Ok                    SDCLK output of SDIF disabled normally
 ** \retval ErrorInvalidParameter pstcSdif == NULL
 ******************************************************************************/ 
en_result_t   Sdif_DisableSdclk( volatile stc_sdifn_t* pstcSdif )
{
    // Check for NULL pointer
    if (NULL == pstcSdif) 
    {
        return ErrorInvalidParameter ;
    }
    
    pstcSdif->SCLKCTL_f.SDCLCKEN = 0u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set the clock division of SD clock
 **
 ** This function changes the SD clock division.
 **
 ** \param [in]  pstcSdif         SDIF instance
 ** \param [in]  u16Div           Division value
 **
 ** \retval Ok                    Clock division is changed normally
 ** \retval ErrorInvalidParameter pstcSdif == NULL
 ******************************************************************************/ 
en_result_t Sdif_SetClkDiv( volatile stc_sdifn_t* pstcSdif, uint16_t u16Div)
{
    // Check for NULL pointer
    if (NULL == pstcSdif) 
    {
        return ErrorInvalidParameter ;
    }
    
    // Set clock division (10-bit)
    pstcSdif->SCLKCTL_f.SDCLKSEL = (u16Div & 0xFFu);
    pstcSdif->SCLKCTL_f.UPSDCLKSEL = ((u16Div >> 8u) &  0x03u);
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set the bus width of SD Bus
 **
 ** This function changes the SD bus width.
 **
 ** \param [in]  pstcSdif         SDIF instance
 ** \param [in]  bEnable4BitMode  Enable 4-bit mode or not
 **
 ** \retval Ok                    Bus width is set normally
 ** \retval ErrorInvalidParameter pstcSdif == NULL
 ******************************************************************************/ 
en_result_t Sdif_SetBusWidth( volatile stc_sdifn_t* pstcSdif, 
                              boolean_t bEnable4BitMode )
{
    // Check for NULL pointer
    if (NULL == pstcSdif) 
    {
        return ErrorInvalidParameter ;
    }
    
    // Set bit width
    pstcSdif->SHCTL1_f.DATAWIDTH = ((TRUE == bEnable4BitMode) ? 1u : 0u);
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Set the bus speed mode of SD Bus
 **
 ** This function changes the SD bus width.
 **
 ** \param [in]  pstcSdif              SDIF instance
 ** \param [in]  bEnableHighSpeedMode  Enable high speed mode or not
 **
 ** \retval Ok                    Bus width is set normally
 ** \retval ErrorInvalidParameter pstcSdif == NULL
 ******************************************************************************/ 
en_result_t Sdif_SetBusSpeedMode( volatile stc_sdifn_t* pstcSdif, 
                                  boolean_t bEnableHighSpeedMode )
{
    // Check for NULL pointer
    if (NULL == pstcSdif) 
    {
        return ErrorInvalidParameter ;
    }
    
    // Check for NULL pointer
    if (NULL == pstcSdif) 
    {
        return ErrorInvalidParameter ;
    }
    
    // Set bit width
    pstcSdif->SHCTL1_f.HIGHSPDEN = ((TRUE == bEnableHighSpeedMode) ? 1u : 0u);
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Select bus voltage of SD bus
 **
 ** This function selects the SD bus voltage 
 **
 ** \param [in]  pstcSdif         SDIF instance
 ** \param [in]  enVoltage        Voltage selection
 **
 ** \retval Ok                    Voltage selected normally
 ** \retval ErrorInvalidParameter pstcSdif == NULL or enVoltage out of range
 ******************************************************************************/ 
en_result_t  Sdif_SelBusVoltage( volatile stc_sdifn_t* pstcSdif, 
                                 en_sdif_voltage_sel_t enVoltage)
{
    // Check for NULL pointer
    if (NULL == pstcSdif) 
    {
        return ErrorInvalidParameter ;
    }
    
    switch (enVoltage)
    {
        case SdifVoltage18v:
            pstcSdif->SPWRCTL_f.SDBUSVLSEL = 5u;
            break;
        case SdifVoltage30v:
            pstcSdif->SPWRCTL_f.SDBUSVLSEL = 6u;
            break;
        case SdifVoltage33v:
            pstcSdif->SPWRCTL_f.SDBUSVLSEL = 7u;
            break;
        default:
            return ErrorInvalidParameter ;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Power on SD bus power
 **
 ** This function starts power supply on SD bus
 **
 ** \param [in]  pstcSdif         SDIF instance
 **
 ** \retval Ok                    Power on normally
 ** \retval ErrorInvalidParameter pstcSdif == NULL
 ******************************************************************************/ 
en_result_t  Sdif_BusPowerOn( volatile stc_sdifn_t* pstcSdif)
{
    // Check for NULL pointer
    if (NULL == pstcSdif) 
    {
        return ErrorInvalidParameter ;
    }
    
    pstcSdif->SPWRCTL_f.SDBUSPWR = 1u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Power off SD bus power
 **
 ** This function stops power supply on SD bus
 **
 ** \param [in]  pstcSdif         SDIF instance
 **
 ** \retval Ok                    Power off normally
 ** \retval ErrorInvalidParameter pstcSdif == NULL
 ******************************************************************************/ 
en_result_t  Sdif_BusPowerOff( volatile stc_sdifn_t* pstcSdif)
{
    // Check for NULL pointer
    if (NULL == pstcSdif) 
    {
        return ErrorInvalidParameter ;
    }
    
    pstcSdif->SPWRCTL_f.SDBUSPWR = 0u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Send SD command
 **
 ** This function sends command on CMD line
 **
 ** \param [in]  pstcSdif         SDIF instance
 ** \param [in]  pstcConfig       Pointer to command structure
 **
 ** \retval Ok                    Command sent normally
 ** \retval ErrorInvalidParameter pstcSdif == NULL or pstcConfig == NULL
 ******************************************************************************/ 
en_result_t Sdif_SendCommand( volatile stc_sdifn_t* pstcSdif,
                              stc_sdif_cmd_config_t* pstcConfig )
{
    stc_sdif_scmmd_field_t stcCmd;
    
    // Clear structure
    PDL_ZERO_STRUCT(stcCmd);
  
    // Check for NULL pointer
    if ((NULL == pstcSdif) || (NULL == pstcConfig))
    {
        return ErrorInvalidParameter ;
    }
    
    // Set command type
    switch (pstcConfig->enCmdType)
    {
        case SdifCmdNormal:
            stcCmd.CMDTYPE = 0u;
            break;
        case SdifCmdSuspend:
            stcCmd.CMDTYPE = 1u;
            break;
        case SdifCmdResume:
            stcCmd.CMDTYPE = 2u;
            break;
        case SdifCmdAbort:
            stcCmd.CMDTYPE = 3u;
            break;
        default:
            return ErrorInvalidParameter ;
    }
    
    // Set data present during the command
    stcCmd.DATPRESSEL = ((TRUE == pstcConfig->bDataPresent) ? 1u : 0u);
    
    // Set command index check 
    stcCmd.CMDIDXCHKE = ((TRUE == pstcConfig->bCmdIndexCheck) ? 1u : 0u);
    
    // Set command CRC check
    stcCmd.CMDCRCCHKE = ((TRUE == pstcConfig->bCmdCrcCheck) ? 1u : 0u);
    
    // Set command response type
    switch (pstcConfig->enResponseType)
    {
        case SdifResponseNone:
            stcCmd.RESPTYPE = 0u;
            break;
        case SdifResponse136Bit:
            stcCmd.RESPTYPE = 1u;
            break;
        case SdifResponse48Bit:
            stcCmd.RESPTYPE = 2u;
            break;
        case SdifResponse48BitCheckBusy:
            stcCmd.RESPTYPE = 3u;
            break;
        default:
            return ErrorInvalidParameter ;            
    }
    
    // Set command index
    stcCmd.CMDINDEX = pstcConfig->u8CmdIndex;
    
    // Auto command setting
    switch (pstcConfig->enAutoCmdType)
    {
        case SdifAutoCmdDisable:
            pstcSdif->STRSFMD_f.AUTOCMDEN = 0u;
            break;
        case SdifAutoCmd12Enable:
            pstcSdif->STRSFMD_f.AUTOCMDEN = 1u;
            break;
        case SdifAutoCmd23Enable:
            pstcSdif->STRSFMD_f.AUTOCMDEN = 2u;
            break;
        default:
            return ErrorInvalidParameter ;     
    }
    
    // Set augument derectly to hardware register
    pstcSdif->SSA1 = pstcConfig->u32Argument;
    
    // Update command hardware register (Command is sent)
   //according to the spec, this register should be only write once.
   pstcSdif->SCMMD = *((uint16_t *)&stcCmd);

    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get SD response
 **
 ** This function receives response on CMD line
 **
 ** \param [in]   pstcSdif           SDIF instance
 ** \param [out]  pu16ResponseData   pointer to response data
 ** \param [in]   u8ResponseRegCount Response register count
 **
 ** \retval Ok                    Response received normally
 ** \retval ErrorInvalidParameter pstcSdif == NULL or u8ResponseRegCount > 8u
 ******************************************************************************/ 
en_result_t  Sdif_GetResponse( volatile stc_sdifn_t* pstcSdif,
                               uint16_t* pu16ResponseData, 
                               uint8_t u8ResponseRegCount)
{
    uint16_t* pu16ResponseBaseAddr;
    uint8_t u8i;
  
    // Check for NULL pointer
    if ((NULL == pstcSdif) || (NULL == pu16ResponseData) || (u8ResponseRegCount > 8u))
    {
        return ErrorInvalidParameter ;
    }
    
    pu16ResponseBaseAddr = (uint16_t*)&pstcSdif->SRESP0;
    
    for(u8i=0; u8i<u8ResponseRegCount; u8i++)
    {
        *pu16ResponseData++ = *pu16ResponseBaseAddr++;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Initialize data transfer
 **
 ** This function must be called before data transfer to initialize data 
 ** transfer mode.
 **
 ** \param [in]   pstcSdif           SDIF instance
 ** \param [in]   pstcDataConfig     pointer to data transfer configuration
 **
 ** \retval Ok                    Data is initialized normally
 ** \retval ErrorInvalidParameter pstcSdif == NULL or pstcDataConfig == NULL
 ******************************************************************************/ 
en_result_t  Sdif_InitDataTransfer( volatile stc_sdifn_t* pstcSdif, 
                                    stc_sdif_data_config_t* pstcDataConfig)
{
    // Check for NULL pointer
    if ((NULL == pstcSdif) || (NULL == pstcDataConfig))
    {
        return ErrorInvalidParameter ;
    }
    
    // Set multi-block or single block transfer
    pstcSdif->STRSFMD_f.BLCKCNTSEL = ((TRUE == pstcDataConfig->bMultipleBlock) ? 1u : 0u);
    
    // Set data transfer direction
    pstcSdif->STRSFMD_f.DTTRSFDIR = ((TRUE == pstcDataConfig->bRead) ? 1u : 0u);
    
    // Set block size
    pstcSdif->SBSIZE_f.TRSFBLCKSZ = pstcDataConfig->u16BlockSize;
    
    // Set block count
    pstcSdif->SBLCNT = pstcDataConfig->u16BlockCount;
    
    // Set block count enable
    pstcSdif->STRSFMD_f.BLCKCNTEN = ((TRUE == pstcDataConfig->bBlockCountEnable) ? 1u : 0u);
    
    // Enable DMA or not
    pstcSdif->STRSFMD_f.DMAEN = ((TRUE == pstcDataConfig->bEnableDma) ? 1u : 0u);
    
    // Set descriptor table of ADMA
    pstcSdif->SADSA0 = (uint16_t)pstcDataConfig->u32AdmaDespTableAddress;
    pstcSdif->SADSA1 = (uint16_t)(pstcDataConfig->u32AdmaDespTableAddress >> 16u);
    
    // Set data timeout time
    if(pstcDataConfig->u8DataTimeout > SdifTimeout_BaseClk_2_27)
    {
        return ErrorInvalidParameter ;
    }
    
    pstcSdif->STOCTL_f.DTTMOUTVAL = pstcDataConfig->u8DataTimeout;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Read data from SD data buffer port
 **
 ** This function reads 32-bit data from data buffer
 **
 ** \param [in]   pstcSdif           SDIF instance
 **
 ** \retval Data that is read
 ******************************************************************************/ 
uint32_t  Sdif_ReadData( volatile stc_sdifn_t* pstcSdif )
{
    return pstcSdif->SBUFDP;
}

/**
 ******************************************************************************
 ** \brief Write data to SD data buffer port
 **
 ** This function writes 32-bit data to data buffer
 **
 ** \param [in]   pstcSdif           SDIF instance
 ** \param [in]   u32Data            Data to be written
 **
 ** \retval Ok                    Data is written normally
 ** \retval ErrorInvalidParameter pstcSdif  == NULL
 ******************************************************************************/ 
en_result_t  Sdif_WriteData( volatile stc_sdifn_t* pstcSdif,
                             uint32_t u32Data)
{
    // Check for NULL pointer
    if (NULL == pstcSdif)
    {
        return ErrorInvalidParameter ;
    }
    
    pstcSdif->SBUFDP = u32Data;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Stop data transfer during block gap
 **
 ** This function is used to stop data trasnfer of multi-block transfer
 **
 ** \param [in]   pstcSdif           SDIF instance
 **
 ** \retval Ok                    Data stops tranfer normally
 ** \retval ErrorInvalidParameter pstcSdif  == NULL
 ******************************************************************************/ 
en_result_t  Sdif_StopAtBlockGap( volatile stc_sdifn_t* pstcSdif )
{
    // Check for NULL pointer
    if (NULL == pstcSdif)
    {
        return ErrorInvalidParameter ;
    }
    
    pstcSdif->SBLKGPCTL_f.BLCKGSTPREQ = 1u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Restart data transfer
 **
 ** This function is used to restart data transfer when transfer is pending
 **
 ** \param [in]   pstcSdif           SDIF instance
 **
 ** \retval Ok                    Data restarts tranfer normally
 ** \retval ErrorInvalidParameter pstcSdif  == NULL
 ******************************************************************************/ 
en_result_t  Sdif_RestartTransfer( volatile stc_sdifn_t* pstcSdif )
{
    // Check for NULL pointer
    if (NULL == pstcSdif)
    {
        return ErrorInvalidParameter ;
    }
    
    pstcSdif->SBLKGPCTL_f.CONTREQ = 1u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Poll card insertion status
 **
 ** This function checks card is inserted or not 
 **
 ** \param [in]   pstcSdif           SDIF instance
 **
 ** \retval TRUE                    Card is inserted
 ** \retval FALSE                   Card is removed
 ******************************************************************************/ 
boolean_t Sdif_PollCardInsert( volatile stc_sdifn_t* pstcSdif )
{
    // Wait until card is stable
    while(pstcSdif->SPRSTAT_f.CARDSTB != 1u);
    
    return (boolean_t)pstcSdif->SPRSTAT_f.CARDINS;
}

/**
 ******************************************************************************
 ** \brief Issue software reset to SD card
 **
 ** This function issues software reset all command to SD card
 **
 ** \param [in]   pstcSdif           SDIF instance
 ** \param [in]   enReset            Reset type
 **
 ** \retval Ok                       Software reset is done normally
 ** \retval ErrorInvalidParameter    pstcSdif == NULL
 ******************************************************************************/ 
en_result_t Sdif_SoftwareReset( volatile stc_sdifn_t* pstcSdif, 
                                en_sdif_reset_t enReset)
{
    // Check for NULL pointer
    if (NULL == pstcSdif)
    {
        return ErrorInvalidParameter ;
    }
    
    switch (enReset)
    {
        case SdifResetDataLine:
            pstcSdif->SSRST_f.SWRSTDATLN = 1u;
            while(0u != pstcSdif->SSRST_f.SWRSTDATLN); // Wait until reset finish
            break;
        case SdifResetCmdLine:
            pstcSdif->SSRST_f.SWRSTCMDLN = 1u;
            while(0u != pstcSdif->SSRST_f.SWRSTCMDLN); // Wait until reset finish
            break;
        case SdifResetAll:
            pstcSdif->SSRST_f.SWRSTALL = 1u;
            while(0u != pstcSdif->SSRST_f.SWRSTALL); // Wait until reset finish
            break;
        default:
            return ErrorInvalidParameter ;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Check the status of CMD line
 **
 ** This function checks whether a commmand wiout data can be sent
 **
 ** \param [in]   pstcSdif           SDIF instance
 **
 ** \retval TRUE                     Command is ready to send
 ** \retval FALSE                    Command is inhitted to send
 ******************************************************************************/ 
boolean_t Sdif_CheckCommandInhit( volatile stc_sdifn_t* pstcSdif )
{
    return (boolean_t)pstcSdif->SPRSTAT_f.CMDINH;
}

/**
 ******************************************************************************
 ** \brief Check the status of DAT line
 **
 ** This function checks whether a commmand with data can be sent
 **
 ** \param [in]   pstcSdif           SDIF instance
 **
 ** \retval TRUE                     Command is ready to send
 ** \retval FALSE                    Command is inhitted to send
 ******************************************************************************/ 
boolean_t Sdif_CheckCommandWithDataInhit( volatile stc_sdifn_t* pstcSdif )
{
    return (boolean_t)pstcSdif->SPRSTAT_f.CMDDATINH;
}

/**
 ******************************************************************************
 ** \brief Set the wakeup factor
 **
 ** This function sets SD host wakeup factors.
 **
 ** \param [in]   pstcSdif            SDIF instance
 ** \param [in]   bCardRemovalWakeup  Whether card removal triggers SD host wakeup
 ** \param [in]   bCardInsertWakeup   Whether card insertion triggers SD host wakeup
 ** \param [in]   bCardIrqWakeup      Whether card interrupt triggers SD host wakeup
 **
 ** \retval Ok                       SD host wakeup factor set normally 
 ** \retval FALSE                    Command is inhitted to send
 ******************************************************************************/
en_result_t  Sdif_SetWakeupFactor( volatile stc_sdifn_t* pstcSdif,
                                   boolean_t bCardRemovalWakeup,
                                   boolean_t bCardInsertWakeup, 
                                   boolean_t bCardIrqWakeup)
{
    // Check for NULL pointer
    if (NULL == pstcSdif)
    {
        return ErrorInvalidParameter ;
    }
    
    pstcSdif->SWKUPCTL_f.WKUPEVNTEN2 = ((TRUE == bCardRemovalWakeup) ? 1u : 0u);
    pstcSdif->SWKUPCTL_f.WKUPEVNTEN1 = ((TRUE == bCardInsertWakeup) ? 1u : 0u);
    pstcSdif->SWKUPCTL_f.WKUPEVNTEN0 = ((TRUE == bCardIrqWakeup) ? 1u : 0u);
    
    return Ok;
}

#endif // #if (defined(PDL_PERIPHERAL_SD_ACTIVE))

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
