/*******************************************************************************
* \file             mft_wfg.c
* 
* \version          1.20
*
* \brief            This file provides the source code to the API for the 
*                   MFT_WFG driver.
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
#include "mft/mft_wfg.h"


#if (defined(PDL_PERIPHERAL_MFT_WFG_ACTIVE))

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/
/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/
/// Look-up table for all enabled WFG  instances and their internal data
stc_mft_wfg_instance_data_t m_astcMftWfgInstanceDataLut[WFG_INSTANCE_COUNT] =
{
#if (PDL_PERIPHERAL_ENABLE_MFT0_WFG == PDL_ON)
    { 
        &MFT0_WFG,  // pstcInstance
        {0u, 0u, 0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFT1_WFG == PDL_ON)
    { 
        &MFT1_WFG,  // pstcInstance
        {0u, 0u, 0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFT2_WFG == PDL_ON)
    { 
        &MFT2_WFG,  // pstcInstance
        {0u, 0u, 0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
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
/**
 *****************************************************************************
 ** \brief Return the internal data for a certain WFG instance.
 **
 ** \param pstcWfg Pointer to WFG instance
 **
 ** \return Pointer to internal data or NULL if instance is not enabled 
 **         (or not known)
 **
 *****************************************************************************/
static stc_mft_wfg_intern_data_t* MftGetInternDataPtr(volatile stc_mftn_wfg_t* pstcWfg)
{
    uint32_t u32Instance;
   
    for (u32Instance = 0u; u32Instance < WFG_INSTANCE_COUNT; u32Instance++)
    {
        if (pstcWfg == m_astcMftWfgInstanceDataLut[u32Instance].pstcInstance)
        {
            return &m_astcMftWfgInstanceDataLut[u32Instance].stcInternData;
        }
    }

    return NULL;
}

#if (PDL_INTERRUPT_ENABLE_MFT0_WFG == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_WFG == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_WFG == PDL_ON)   
/*!
 ******************************************************************************
 ** \brief  WFG interrupt handler sub function
 **
 ** \param [in] pstcWfg Pointer to WFG instance
 **
 ** \param [in]  pstcMftWfgInternData  structure of WFG callback function
 ** \arg    structure of stc_mft_intern_data
 *****************************************************************************
*/
void Mft_Wfg_IrqHandler( volatile  stc_mftn_wfg_t* pstcWfg,
		         stc_mft_wfg_intern_data_t* pstcMftWfgInternData)
{
    func_ptr_t funCallBack;
    uint32_t* ptemp;
    uint8_t ch;

    /* timer interrupt*/
    for(ch=0u; ch<3u; ch++)
    {
        if(PdlSet == Mft_Wfg_GetTimerIrqFlag(pstcWfg,ch))
        {
            /* Clear Interrupt */
            Mft_Wfg_ClrTimerIrqFlag(pstcWfg,ch);

            //get peak timer interrupt callback address of each channel
            ptemp = (uint32_t*)&(pstcMftWfgInternData->pfnWfg10TimerIrqCallback);
            funCallBack = (func_ptr_t)(*(ptemp + ch));

            if(NULL != funCallBack)
            {
               funCallBack();
            }
        }
    }

    /* DTIF Interrupt */
    //Digital filter interrupt
    if(PdlSet == Mft_Wfg_Nzcl_GetDigitalFilterIrqFlag(pstcWfg))
    {
        // Mft_Wfg_Nzcl_ClrDigitalFilterIntFlag(pstcMft);
        funCallBack = pstcMftWfgInternData->pfnDtifDigtalFilterIrqCallback;
        if(NULL != funCallBack)
        {
           funCallBack();
        }
    }

#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)      
    //analog filter interrupt
    if(PdlSet == Mft_Wfg_Nzcl_GetAnalogFilterIrqFlag(pstcWfg))
    {
        // Mft_Wfg_Nzcl_ClrAnalogFilterIntFlag(pstcMft);
        funCallBack = pstcMftWfgInternData->pfnDtifAnalogFilterIrqCallback;
        if(NULL != funCallBack)
        {
           funCallBack();
        }
    }
#endif    
}

/**
 ******************************************************************************
 ** \brief Device dependent initialization of interrupts according CMSIS with
 **        level defined in pdl.h
 **
 ** \param [in] pstcWfg Pointer to WFG instance
 **
 ** \retval Ok    Successful initialization
 **
 ******************************************************************************/
static void MftWfgInitNvic( volatile stc_mftn_wfg_t* pstcWfg )
{
#if (PDL_MCU_CORE == PDL_FM0P_CORE)  
    NVIC_ClearPendingIRQ(MFT0_WFG_DTIF_IRQn);
    NVIC_EnableIRQ(MFT0_WFG_DTIF_IRQn);
    NVIC_SetPriority(MFT0_WFG_DTIF_IRQn, PDL_IRQ_LEVEL_MFT_WFG);
#elif (PDL_MCU_CORE == PDL_FM3_CORE)
    NVIC_ClearPendingIRQ(WFG_DTIF_IRQn);
    NVIC_EnableIRQ(WFG_DTIF_IRQn);
    NVIC_SetPriority(WFG_DTIF_IRQn, PDL_IRQ_LEVEL_MFT_WFG);
#else
    #if (PDL_INTERRUPT_ENABLE_MFT0_WFG == PDL_ON)       
    if(pstcWfg == (volatile stc_mftn_wfg_t*)(&MFT0_WFG))
    {
        NVIC_ClearPendingIRQ(MFT0_WFG_DTIF_IRQn);
        NVIC_EnableIRQ(MFT0_WFG_DTIF_IRQn);
        NVIC_SetPriority(MFT0_WFG_DTIF_IRQn, PDL_IRQ_LEVEL_MFT0_WFG);
    }
    #endif
    #if (PDL_INTERRUPT_ENABLE_MFT1_WFG == PDL_ON)       
    if(pstcWfg == (volatile stc_mftn_wfg_t*)(&MFT1_WFG))
    {
        NVIC_ClearPendingIRQ(MFT1_WFG_DTIF_IRQn);
        NVIC_EnableIRQ(MFT1_WFG_DTIF_IRQn);
        NVIC_SetPriority(MFT1_WFG_DTIF_IRQn, PDL_IRQ_LEVEL_MFT1_WFG);
    }
    #endif
    #if (PDL_INTERRUPT_ENABLE_MFT2_WFG == PDL_ON) 
    if(pstcWfg == (volatile stc_mftn_wfg_t*)(&MFT2_WFG))
    {
        NVIC_ClearPendingIRQ(MFT2_WFG_DTIF_IRQn);
        NVIC_EnableIRQ(MFT2_WFG_DTIF_IRQn);
        NVIC_SetPriority(MFT2_WFG_DTIF_IRQn, PDL_IRQ_LEVEL_MFT2_WFG);
    }
    #endif
   
#endif    
    return;
}

/**
 ******************************************************************************
 ** \brief Device dependent de-initialization of interrupts according CMSIS
 **
 ** \param [in] pstcWfg Pointer to WFG instance
 **
 ** \retval Ok    Successful initialization
 **
 ******************************************************************************/
static void MftWfgDeInitNvic( volatile stc_mftn_wfg_t* pstcWfg )
{
#if (PDL_MCU_CORE == PDL_FM0P_CORE)  
    NVIC_ClearPendingIRQ(MFT0_WFG_DTIF_IRQn);
    NVIC_DisableIRQ(MFT0_WFG_DTIF_IRQn);
    NVIC_SetPriority(MFT0_WFG_DTIF_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
#elif (PDL_MCU_CORE == PDL_FM3_CORE)
    NVIC_ClearPendingIRQ(WFG_DTIF_IRQn);
    NVIC_DisableIRQ(WFG_DTIF_IRQn);
    NVIC_SetPriority(WFG_DTIF_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
#else
    #if (PDL_INTERRUPT_ENABLE_MFT0_WFG == PDL_ON)
    if((volatile stc_mftn_wfg_t*)(&MFT0_WFG) == pstcWfg)
    {
        NVIC_ClearPendingIRQ(MFT0_WFG_DTIF_IRQn);
        NVIC_DisableIRQ(MFT0_WFG_DTIF_IRQn);
        NVIC_SetPriority(MFT0_WFG_DTIF_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
    #endif
    #if (PDL_INTERRUPT_ENABLE_MFT1_WFG == PDL_ON)
    if((volatile stc_mftn_wfg_t*)(&MFT1_WFG) == pstcWfg)
    {
        NVIC_ClearPendingIRQ(MFT1_WFG_DTIF_IRQn);
        NVIC_DisableIRQ(MFT1_WFG_DTIF_IRQn);
        NVIC_SetPriority(MFT1_WFG_DTIF_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
    #endif
    #if (PDL_INTERRUPT_ENABLE_MFT2_WFG == PDL_ON)
    if((volatile stc_mftn_wfg_t*)(&MFT2_WFG) == pstcWfg)
    {
        NVIC_ClearPendingIRQ(MFT2_WFG_DTIF_IRQn);
        NVIC_DisableIRQ(MFT2_WFG_DTIF_IRQn);
        NVIC_SetPriority(MFT2_WFG_DTIF_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
    #endif
    
#endif
    
    return;
}

#endif

/**
 ******************************************************************************
 ** \brief  Initialize a couple WFG channels 
 **
 ** \param [in] pstcWfg Pointer to WFG instance
 ** \param [in]  u8CoupleCh  channel of WFG couple
 ** \arg    MFT_WFG_CH10 ~ MFT_WFG_CH54
 ** \param [in] pstcConfig Pointer to WFG configuration
 ** \arg    see the struct stc_wfg_config_t
 **
 ** \retval Ok                    WFG mode is configured
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **                               - u8CoupleCh out of range
 **                               - Other invalid configuration
 **
 ******************************************************************************/
en_result_t Mft_Wfg_Init(volatile stc_mftn_wfg_t* pstcWfg,
		                 uint8_t u8CoupleCh, 
                         const stc_wfg_config_t* pstcConfig)
{
    volatile stc_mft_wfg_wfsa10_field_t* pstcWFSA;
    stc_mft_wfg_intern_data_t* pstcWfgInternData;
    
    // Check for NULL pointer
    if ( (NULL == pstcWfg)
         || (u8CoupleCh >= MFT_WFG_MAXCH))
    {
        return ErrorInvalidParameter ;
    }
    // Get pointer to internal data structure ...
    pstcWfgInternData = MftGetInternDataPtr( pstcWfg ) ;
    
    // ... and check for NULL
    if ( NULL == pstcWfgInternData )
    {
        return ErrorInvalidParameter;
    }

    // Get pointer of current channel WFG register address
    pstcWFSA = (volatile stc_mft_wfg_wfsa10_field_t*)((volatile uint8_t*)&pstcWfg->WFSA10 + u8CoupleCh*4u);
    
    // Configure WFG mode
    switch(pstcConfig->enMode)
    {
        case WfgThroughMode:
            pstcWFSA->TMD = 0u;
            break;
        case WfgRtPpgMode:
            pstcWFSA->TMD = 1u;
            break;
        case WfgTimerPpgMode:
            pstcWFSA->TMD = 2u;
            break;
        case WfgRtDeadTimerMode:
            pstcWFSA->TMD = 4u;
            break;
        case WfgRtDeadTimerFilterMode:
            pstcWFSA->TMD = 5u;
            break;
        case WfgPpgDeadTimerFilterMode:
            pstcWFSA->TMD = 6u;
            break;
        case WfgPpgDeadTimerMode:
            pstcWFSA->TMD = 7u;
            break;
        default:
            return ErrorInvalidParameter;
    }

    // Set control bits
    switch (pstcConfig->enDmodBits)
    {
        case DmodBits00B:
            pstcWFSA->DMOD = 0u;
            break;
        case DmodBits01B:
            pstcWFSA->DMOD = 1u;
            break;
        #if(PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)            
        case DmodBits10B:
            pstcWFSA->DMOD = 2u;
            break;
        case DmodBits11B:
            pstcWFSA->DMOD = 3u;
            break;
        #endif    
        default: 
            return ErrorInvalidParameter;
    }
      
    switch (pstcConfig->enPgenBits)
    {
        case PgenBits00B:
            pstcWFSA->PGEN = 0u;
            break;
        case PgenBits01B:
            pstcWFSA->PGEN = 1u;
            break;
        case PgenBits10B:
            pstcWFSA->PGEN = 2u;
            break;
        case PgenBits11B:
            pstcWFSA->PGEN = 3u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    switch (pstcConfig->enPselBits)
    {
        case PselBits00B:
            pstcWFSA->PSEL = 0u;
            break;
        case PselBits01B:
            pstcWFSA->PSEL = 1u;
            break;
        case PselBits10B:
            pstcWFSA->PSEL = 2u;
            break;
        case PselBits11B: 
            pstcWFSA->PSEL = 3u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    switch (pstcConfig->enGtenBits)
    {
        case GtenBits00B:
            pstcWFSA->GTEN = 0u;
            break;
        case GtenBits01B:
            pstcWFSA->GTEN = 1u;
            break;
        case GtenBits10B:
            pstcWFSA->GTEN = 2u;
            break;
        case GtenBits11B:
            pstcWFSA->GTEN = 3u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    // Set timer clock division
    switch (pstcConfig->enClk)
    {
        case WfgPlckDiv1:
            pstcWFSA->DCK = 0u;
            break;
        case WfgPlckDiv2:
            pstcWFSA->DCK = 1u;
            break;
        case WfgPlckDiv4:
            pstcWFSA->DCK = 2u;
            break;
        case WfgPlckDiv8:
            pstcWFSA->DCK = 3u;    
            break;
        case WfgPlckDiv16:
            pstcWFSA->DCK = 4u;
            break;
        case WfgPlckDiv32: 
            pstcWFSA->DCK = 5u;
            break;
        case WfgPlckDiv64:
            pstcWFSA->DCK = 6u;
            break;
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)       
        case WfgPlckDiv128: 
            pstcWFSA->DCK = 7u;
            break;
        #endif    
        default:
            return ErrorInvalidParameter;
    }

#if (PDL_INTERRUPT_ENABLE_MFT0_WFG == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_WFG == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_WFG == PDL_ON)  
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)        
    // Set interrupt mask
    if(pstcConfig->bWfgimerIrqMask == TRUE)
    {
        switch (u8CoupleCh)
        {
            case MFT_WFG_CH10:
                pstcWfg->NZCL_f.WIM10 = 1u;
                break;
            case MFT_WFG_CH32:
                pstcWfg->NZCL_f.WIM32 = 1u;
                break;
            case MFT_WFG_CH54:
                pstcWfg->NZCL_f.WIM54 = 1u;
                break;
            default:    
                break;
        }
    }
#endif    
    // Set interrupt callback functions
    switch (u8CoupleCh)
    {
        case MFT_WFG_CH10:
            pstcWfgInternData->pfnWfg10TimerIrqCallback = pstcConfig->pfnWfgTimerIrqCallback;
            break;
        case MFT_WFG_CH32:
            pstcWfgInternData->pfnWfg32TimerIrqCallback = pstcConfig->pfnWfgTimerIrqCallback;
            break;
        case MFT_WFG_CH54:
            pstcWfgInternData->pfnWfg54TimerIrqCallback = pstcConfig->pfnWfgTimerIrqCallback;
            break;
        default:
            break;
    }
    
    if(TRUE == pstcConfig->bTouchNvic)
    {
        MftWfgInitNvic(pstcWfg);
    }
#endif
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  De-Initialize a couple WFG channels
 **
 ** \param [in] pstcWfg Pointer to WFG instance
 ** \param [in] u8CoupleCh  Couple channel of WFG couple
 ** \arg    MFT_WFG_CH10 ~ MFT_WFG_CH54
 ** \param [in] bTouchNvic Touch NVIC or not
 **
 ** \retval Ok                    WFG mode is configured
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcWfg == NULL
 **                               - u8CoupleCh out of range
 **                               - Other invalid configuration
 **
 ******************************************************************************/
en_result_t Mft_Wfg_DeInit(volatile stc_mftn_wfg_t* pstcWfg, 
                           uint8_t u8CoupleCh,
                           boolean_t bTouchNvic)
{
    volatile uint16_t* pstcWFSA;
    stc_mft_wfg_intern_data_t* pstcWfgInternData;
    
    if(NULL == pstcWfg)
    {
        return ErrorInvalidParameter;
    }
    
    // Get pointer to internal data structure ...
    pstcWfgInternData = MftGetInternDataPtr( pstcWfg ) ;
    
    // ... and check for NULL
    if ( NULL == pstcWfgInternData )
    {
        return ErrorInvalidParameter;
    }
    
    // Get pointer of current channel WFG register address
    pstcWFSA = (volatile uint16_t*)((volatile uint8_t*)&pstcWfg->WFSA10 + u8CoupleCh*4u);    
    *pstcWFSA = 0;
    
#if (PDL_INTERRUPT_ENABLE_MFT0_WFG == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_WFG == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_WFG == PDL_ON)   
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)        
    switch (u8CoupleCh)
    {
        case MFT_WFG_CH10:
            pstcWfgInternData->pfnWfg10TimerIrqCallback = NULL;
            pstcWfg->NZCL_f.WIM10 = 0u;
            break;
        case MFT_WFG_CH32:
            pstcWfgInternData->pfnWfg32TimerIrqCallback = NULL;
            pstcWfg->NZCL_f.WIM32 = 0u;
            break;
        case MFT_WFG_CH54:
            pstcWfgInternData->pfnWfg54TimerIrqCallback = NULL;
            pstcWfg->NZCL_f.WIM54 = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }  
#endif
    if(TRUE == bTouchNvic)
    {
        MftWfgDeInitNvic(pstcWfg);
    }
   
#endif      
    
    return Ok;
    
}

/*!
 ******************************************************************************
 ** \brief  Start WFG timer
 **
 ** \param [in] pstcWfg Pointer to WFG instance
 ** \param [in]  u8CoupleCh  channel of WFG couple
 ** \arg    MFT_WFG_CH10 ~ MFT_WFG_CH54
 **
 ** \retval Ok                    Start timer successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **                               - u8CoupleCh out of range
 **
 *****************************************************************************
*/          
en_result_t Mft_Wfg_StartTimer(volatile stc_mftn_wfg_t* pstcWfg, uint8_t u8CoupleCh)
{
    if ( (NULL == pstcWfg)
         || (u8CoupleCh >= MFT_WFG_MAXCH))
    {
        return ErrorInvalidParameter ;
    }
    
    switch(u8CoupleCh)
    {
        case MFT_WFG_CH10:
            pstcWfg->WFIR_f.TMIE10 = 1u;
            break;
        case MFT_WFG_CH32:
            pstcWfg->WFIR_f.TMIE32 = 1u;
            break;
        case MFT_WFG_CH54:
            pstcWfg->WFIR_f.TMIE54 = 1u;
            break;
        default:
            break;
    }
    
    return Ok;
}

/*!
 ******************************************************************************
 ** \brief  Stop WFG timer
 **
 ** \param [in] pstcWfg Pointer to WFG instance
 ** \param [in]  u8CoupleCh  channel of WFG couple
 ** \arg    MFT_WFG_CH10 ~ MFT_WFG_CH54
 **
 ** \retval Ok                    Stop timer successfully
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **                               - u8CoupleCh out of range
 **
 *****************************************************************************
*/  
en_result_t Mft_Wfg_StopTimer(volatile stc_mftn_wfg_t* pstcWfg, uint8_t u8CoupleCh)
{
    if ( (NULL == pstcWfg)
         || (u8CoupleCh >= MFT_WFG_MAXCH))
    {
        return ErrorInvalidParameter ;
    }
    
    switch(u8CoupleCh)
    {
        case MFT_WFG_CH10:
            pstcWfg->WFIR_f.TMIS10 = 1u;
            break;
        case MFT_WFG_CH32:
            pstcWfg->WFIR_f.TMIS32 = 1u;
            break;
        case MFT_WFG_CH54:
            pstcWfg->WFIR_f.TMIS54 = 1u;
            break;
        default:
            break;
    }
    
    return Ok;
}


/*!
 ******************************************************************************
 ** \brief  get WFG timer interrupt flag
 **
 ** \param [in] pstcWfg Pointer to WFG instance
 ** \param [in]  u8CoupleCh  channel of WFG couple
 ** \arg    MFT_WFG_CH10 ~ MFT_WFG_CH54
 **
 ** \retval PdlSet WFG Interrupt flag is set 
 ** \retval PdlClr WFG Interrupt flag is clear
 **
 *****************************************************************************
*/
en_irq_flag_t Mft_Wfg_GetTimerIrqFlag( volatile stc_mftn_wfg_t* pstcWfg,
                                       uint8_t u8CoupleCh)
{
    en_irq_flag_t retval = PdlClr;

    switch(u8CoupleCh)
    {
        case MFT_WFG_CH10:     
            (pstcWfg->WFIR_f.TMIF10 == 1u) ? (retval = PdlSet) : (retval = PdlClr);
            break;
        case MFT_WFG_CH32: 
            (pstcWfg->WFIR_f.TMIF32 == 1u) ? (retval = PdlSet) : (retval = PdlClr);
            break;
        case MFT_WFG_CH54:     
            (pstcWfg->WFIR_f.TMIF54 == 1u) ? (retval = PdlSet) : (retval = PdlClr); 
            break;
        default:
            break;
    }

    return retval;
}

/*!
 ******************************************************************************
 ** \brief  clear WFG timer interrupt flag
 **
 ** \param [in] pstcWfg Pointer to WFG instance
 ** \param [in]  u8CoupleCh  channel of WFG couple
 ** \arg    MFT_WFG_CH10 ~ MFT_WFG_CH54
 **
 ** \retval Ok                    WFG timer interrupt flag is clear
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **                               - u8CoupleCh out of range
 **
 *****************************************************************************
*/
en_result_t Mft_Wfg_ClrTimerIrqFlag( volatile stc_mftn_wfg_t* pstcWfg, uint8_t u8CoupleCh)
{
    // Check for NULL pointer
    if ( (NULL == pstcWfg)
         || (u8CoupleCh >= MFT_WFG_MAXCH))
    {
        return ErrorInvalidParameter ;
    }

    //set register value
    switch(u8CoupleCh)
    {
        case MFT_WFG_CH10:
            pstcWfg->WFIR_f.TMIC10 = 1u;
            break;
        case MFT_WFG_CH32:
            pstcWfg->WFIR_f.TMIC32 = 1u;
            break;
        case MFT_WFG_CH54:
            pstcWfg->WFIR_f.TMIC54 = 1u;
            break;
        default:
            break;
    }

    return Ok;
}

/*!
 ******************************************************************************
 ** \brief  write timer count cycle
 **
 ** \param [in] pstcWfg Pointer to WFG instance
 ** \param [in]  u8CoupleCh  channel of WFG couple
 ** \arg    MFT_WFG_CH10 ~ MFT_WFG_CH54
 ** \param [in] u16CycleA WFTA value
 ** \arg    0~65535
 ** \param [in]  u16CycleB  WFTB value
 ** \arg    0~65535
 **
 ** \retval Ok                    Timer count cycle is written
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **                               - u8CoupleCh out of range
 **                               - u16CycleA != u16CycleB (FM3 products)
 **
 **
 *****************************************************************************
*/
en_result_t Mft_Wfg_WriteTimerCountCycle( volatile stc_mftn_wfg_t* pstcWfg,
                                         uint8_t u8CoupleCh,
                                         uint16_t u16CycleA,
                                         uint16_t u16CycleB)
{
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)     
    volatile uint16_t* pu16WFTA;
    volatile uint16_t* pu16WFTB;
#else
    volatile uint16_t* pu16WFTM;
#endif    

    // Check for NULL pointer
    if ( (NULL == pstcWfg)
         || (u8CoupleCh >= MFT_WFG_MAXCH))
    {
        return ErrorInvalidParameter ;
    }

#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)     
    //Get pointer of current channel WFG register address
    pu16WFTA = (volatile uint16_t*)((volatile uint8_t*)&pstcWfg->WFTA10 + u8CoupleCh*8);
    pu16WFTB = (volatile uint16_t*)((volatile uint8_t*)&pstcWfg->WFTB10 + u8CoupleCh*8);

    //set the register
    *pu16WFTA = u16CycleA;
    *pu16WFTB = u16CycleB;
#else
    if(u16CycleA != u16CycleB)
    {
        return ErrorInvalidParameter;
    }
    
    pu16WFTM = (volatile uint16_t*)((volatile uint8_t*)&pstcWfg->WFTM10 + u8CoupleCh*4u);
    *pu16WFTM = u16CycleA;
    
#endif
    
    return Ok;

}

#if(PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE) 
/**
 ******************************************************************************
 ** \brief  set cycle of WFG timer
 **
 ** \param [in] pstcWfg Pointer to WFG instance
 ** \param [in]  u8CoupleCh  channel of WFG couple
 ** \arg    MFT_WFG_CH10 ~ MFT_WFG_CH54
 ** \param [in]  u16Count  wfg pulse counter value
 ** \arg    0~65535
 **
 ** \retval Ok                    cycle of WFG timer is set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **                               - u8CoupleCh out of range
 **
 ******************************************************************************/
en_result_t Mft_Wfg_SetFilterCountValue( volatile stc_mftn_wfg_t* pstcWfg,
                                         uint8_t u8CoupleCh, uint16_t u16Count)
{
    volatile uint16_t* pWFTF;

    //Get pointer of current channel WFG register address
    pWFTF = (volatile uint16_t*)((volatile uint8_t*)&pstcWfg->WFTF10 + u8CoupleCh*8u);

    *pWFTF =u16Count;
    return Ok;
}
#endif

/**
 ******************************************************************************
 ** \brief  Initialize NZCL of waveform generator
 **
 ** \param [in] pstcWfg Pointer to WFG instance
 ** \param [in] pstcNzclConfig  Configure of NZCL
 ** \param [in] bTouchNvic      Touch NVIC or not
 **
 ** \retval Ok                    NZCL is configured
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **
 ******************************************************************************/
en_result_t Mft_Wfg_Nzcl_Init ( volatile stc_mftn_wfg_t* pstcWfg,
                                stc_wfg_nzcl_config_t* pstcNzclConfig,
                                boolean_t bTouchNvic )
{
    stc_mft_wfg_intern_data_t* pstcWfgInternData;
    
    // Check for NULL pointer
    if ( NULL == pstcWfg )
    {
        return ErrorInvalidParameter ;
    }
   
    // Get pointer to internal data structure ...
    pstcWfgInternData = MftGetInternDataPtr( pstcWfg ) ;
    
    if(NULL == pstcWfgInternData)
    {
        return ErrorInvalidParameter ;
    }
    
    // Set digital path for DTTIX pin
    pstcWfg->NZCL_f.DTIEA = ((pstcNzclConfig->bEnDigitalFilter == TRUE) ? 1u : 0u);
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)
    // Set analog path for DTTIX pin    
    pstcWfg->NZCL_f.DTIEB = ((pstcNzclConfig->bEnAnalogFilter == TRUE) ? 1u : 0u);
    #if (PDL_MCU_TYPE >= PDL_FM4_TYPE3)
    // Hold RTO pin when DTIF interrupt occurs or not
    pstcWfg->NZCL_f.DHOLD = ((pstcNzclConfig->bHoldRto == TRUE) ? 1u : 0u);
    #endif
#endif
    
    // Set filter width of digital path 
    switch (pstcNzclConfig->enDigitalFilterWidth)
    {
        case NzlcNoFilter:
            pstcWfg->NZCL_f.NWS = 0u;
            break;
        case NzlcWidth4Cycle:
            pstcWfg->NZCL_f.NWS = 1u;
            break;
        case NzlcWidth8Cycle:
            pstcWfg->NZCL_f.NWS = 2u;
            break;
        case NzlcWidth16Cycle:
            pstcWfg->NZCL_f.NWS = 3u;
            break;
        case NzlcWidth32Cycle:
            pstcWfg->NZCL_f.NWS = 4u;
            break;
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)         
        case NzlcWidth64Cycle:
            pstcWfg->NZCL_f.NWS = 5u;
            break;
        case NzlcWidth128Cycle:
            pstcWfg->NZCL_f.NWS = 6u;
            break;
        #if (PDL_MCU_TYPE >= PDL_FM4_TYPE3)    
        case NzlcWidth256Cycle:
            pstcWfg->NZCL_f.NWS = 7u;
            break;
        #endif    
    #endif        
        default:
            return ErrorInvalidParameter ;
    }  
    
    if(&MFT0_WFG == pstcWfg)
    {
        bFM_GPIO_EPFR01_DTTI0C = pstcNzclConfig->bSwitchToGpio;
    }
  #if(defined(FM_MFT1)) 
    else if (&MFT1_WFG == pstcWfg)
    {
        bFM_GPIO_EPFR02_DTTI1C = pstcNzclConfig->bSwitchToGpio;
    }
  #endif
  #if(defined(FM_MFT2)) 
    else if(&MFT2_WFG == pstcWfg)
    {
        bFM_GPIO_EPFR03_DTTI2C = pstcNzclConfig->bSwitchToGpio;
    }
  #endif
    
#if (PDL_INTERRUPT_ENABLE_MFT0_WFG == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_WFG == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_WFG == PDL_ON)         
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
    pstcWfg->NZCL_f.DIMA = ((pstcNzclConfig->bDtifDigitalFilterIrqMask == TRUE) ? 1u : 0u);   
    pstcWfg->NZCL_f.DIMB = ((pstcNzclConfig->bDtifAnalogFilterIrqMask == TRUE) ? 1u : 0u);  
    #endif    
    
    pstcWfgInternData->pfnDtifDigtalFilterIrqCallback = pstcNzclConfig->pfnDtifDigtalFilterIrqCallback;
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)     
    pstcWfgInternData->pfnDtifAnalogFilterIrqCallback = pstcNzclConfig->pfnDtifAnalogFilterIrqCallback;
    #endif 
    
    if(TRUE == bTouchNvic)
    {
        MftWfgInitNvic(pstcWfg);
    }
#endif    
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  De-Initialize NZCL of waveform generator
 **
 ** \param [in] pstcWfg Pointer to WFG instance
 ** \param [in] bTouchNvic  Disable NVIC or not
 ** \arg TRUE Disable NVIC
 ** \arg FALSE Don't disable NVIC
 **
 ** \retval Ok                    NZCL is cleared
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **
 ******************************************************************************/
en_result_t Mft_Wfg_Nzcl_DeInit(volatile stc_mftn_wfg_t* pstcWfg, boolean_t bTouchNvic)
{
    if(NULL == pstcWfg)
    {
        return ErrorInvalidParameter;
    }
  
    pstcWfg->NZCL = 0u;

#if (PDL_INTERRUPT_ENABLE_MFT0_WFG == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_WFG == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_WFG == PDL_ON)     
    if(TRUE == bTouchNvic)
    {
        MftWfgDeInitNvic(pstcWfg);
    }
#endif    
    
    return Ok;
}

/*!
 ******************************************************************************
 ** \brief  set software trigger DTIF
 **
 ** \param [in] pstcWfg Pointer to WFG instance
 **
 ** \retval Ok                    DTIF is triggered by software
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **                               - u8CoupleCh out of range
 **
 *****************************************************************************
*/
en_result_t Mft_Wfg_Nzcl_SwTriggerDtif( volatile stc_mftn_wfg_t* pstcWfg)
{
    // Check for NULL pointer
    if (NULL == pstcWfg)
    {
        return ErrorInvalidParameter ;
    }

    pstcWfg->NZCL_f.SDTI = 1u;
    return Ok;
}

/*!
 ******************************************************************************
 ** \brief  get Digital filter interrupt flag
 **
 ** \param [in] pstcWfg Pointer to WFG instance
 **
 ** \retval PdlClr Digital Filter interrupt flag is clear
 ** \retval PdlSet Digital Filter interrupt flag is set
 **
 *****************************************************************************
*/
en_irq_flag_t Mft_Wfg_Nzcl_GetDigitalFilterIrqFlag( volatile stc_mftn_wfg_t* pstcWfg)
{
    en_irq_flag_t enFlag = PdlClr;

    //get interrupt flag via digital noise-canceler   
    (pstcWfg->WFIR_f.DTIFA == 1u) ? (enFlag = PdlSet) : (enFlag = PdlClr);
    
    return enFlag;
}

/*!
 ******************************************************************************
 ** \brief  clear Digital filter interrupt flag
 **
 ** \param [in] pstcWfg Pointer to WFG instance
 **
 ** \retval Ok                    Digital filter interrupt flag is clear
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **                               - u8CoupleCh out of range
 **
 *****************************************************************************
*/
en_result_t Mft_Wfg_Nzcl_ClrDigitalFilterIrqFlag( volatile stc_mftn_wfg_t* pstcWfg)
{
    // Check for NULL pointer
    if ( NULL == pstcWfg )
    {
        return ErrorInvalidParameter ;
    }

    pstcWfg->WFIR_f.DTICA = 1u;
    
    return Ok;
}

#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
/*!
 ******************************************************************************
 ** \brief  get Analog Filter interrupt flag
 **
 ** \param [in] pstcWfg Pointer to WFG instance
 **
 ** \retval PdlClr Analog Filter interrupt flag is clear
 ** \retval PdlSet Analog Filter interrupt flag is set
 **
 *****************************************************************************
*/
en_irq_flag_t Mft_Wfg_Nzcl_GetAnalogFilterIrqFlag( volatile stc_mftn_wfg_t* pstcWfg)
{
    en_irq_flag_t enFlag;

    //get interrupt flag via analog noise filter
    (pstcWfg->WFIR_f.DTIFB == 1u) ? (enFlag = PdlSet) : (enFlag = PdlClr);
    
    return enFlag;
}
/*!
 ******************************************************************************
 ** \brief  clear Analog Filter interrupt flag
 **
 ** \param [in] pstcWfg Pointer to WFG instance
 **
 ** \retval Ok                    Analog Filter interrupt flag is clear
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **                               - u8CoupleCh out of range
 **
 *****************************************************************************
*/
en_result_t Mft_Wfg_Nzcl_ClrAnalogFilterIrqFlag( volatile stc_mftn_wfg_t* pstcWfg)
{
    // Check for NULL pointer
    if ( NULL == pstcWfg )
    {
        return ErrorInvalidParameter ;
    }

    pstcWfg->WFIR_f.DTICB = 1u;
    
    return Ok;
}

#endif

#endif // #if (defined(PDL_PERIPHERAL_$$X_ACTIVE))

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
