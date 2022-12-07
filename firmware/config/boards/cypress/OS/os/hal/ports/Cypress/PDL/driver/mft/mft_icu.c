/*******************************************************************************
* \file             mft_icu.c
* 
* \version          1.20
*
* \brief            This file provides the source code to the API for the 
*                   MFT_ICU driver.
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
#include "mft/mft_icu.h"

#if (defined(PDL_PERIPHERAL_MFT_ICU_ACTIVE))

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/
stc_mft_icu_instance_data_t m_astcMftIcuInstanceDataLut[MFT_ICU_INSTANCE_COUNT] =
{
#if (PDL_PERIPHERAL_ENABLE_MFT0_ICU == PDL_ON)
    { 
        &MFT0_ICU,    // pstcInstance
        {0u, 0u, 0u, 0u}            // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFT1_ICU == PDL_ON)
    { 
        &MFT1_ICU,    // pstcInstance
        {0u, 0u, 0u, 0u}            // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFT2_ICU == PDL_ON)
    { 
        &MFT2_ICU,    // pstcInstance
        {0u, 0u, 0u, 0u}            // stcInternData (not initialized yet)
    },
#endif
};

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/
#if (PDL_INTERRUPT_ENABLE_MFT0_ICU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_ICU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_ICU == PDL_ON)
static stc_mft_icu_intern_data_t* MftIcuGetInternDataPtr(volatile stc_mftn_icu_t *pstcMftIcu);
static void Mft_Icu_InitIrq( volatile stc_mftn_icu_t* pstcMftIcu );
static void Mft_Icu_DeInitIrq( volatile stc_mftn_icu_t* pstcMftIcu );
#endif
/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/

/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/

#if (PDL_INTERRUPT_ENABLE_MFT0_ICU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_ICU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_ICU == PDL_ON)
/**
 *****************************************************************************
 ** \brief Return the internal data for a certain MFT_ICU instance.
 **
 ** \param pstcMftIcu Pointer to MftIcu instance
 **
 ** \return Pointer to internal data or NULL if instance is not enabled (or not known)
 **
 *****************************************************************************/
static stc_mft_icu_intern_data_t* MftIcuGetInternDataPtr(volatile stc_mftn_icu_t *pstcMftIcu) 
{
    uint8_t u8Instance;
   
    for (u8Instance = 0u; u8Instance < MFT_ICU_INSTANCE_COUNT; u8Instance++)
    {
        if (pstcMftIcu == m_astcMftIcuInstanceDataLut[u8Instance].pstcInstance)
        {
            return &m_astcMftIcuInstanceDataLut[u8Instance].stcInternData;
        }
    }

    return NULL;
}		
		
/**
 ******************************************************************************
 ** \brief Device Interrupt handler
 **
 ** \param [in] pstcMftIcu            Pointer to ICU instance
 ** \param [in] pstcMftIcuInternData  Pointer to intern data
 **
 ** \return none
 ******************************************************************************/
void Mft_Icu_IrqHandler( volatile stc_mftn_icu_t* pstcMftIcu, 
                         stc_mft_icu_intern_data_t* pstcMftIcuInternData)
{
    // ICU interrupt 0
    if(TRUE == pstcMftIcu->ICSA10_f.ICP0)
    {
        pstcMftIcu->ICSA10_f.ICP0 = 0u;
        if (pstcMftIcuInternData->pfnIcu0IrqCb != NULL)
        {
             pstcMftIcuInternData->pfnIcu0IrqCb();
        }
    }
    // ICU interrupt 1
    if(TRUE == pstcMftIcu->ICSA10_f.ICP1)
    {
        pstcMftIcu->ICSA10_f.ICP1 = 0u;
        if (pstcMftIcuInternData->pfnIcu1IrqCb != NULL)
        {
             pstcMftIcuInternData->pfnIcu1IrqCb();
        }
    }
    // ICU interrupt 2
    if(TRUE == pstcMftIcu->ICSA32_f.ICP2)
    {
        pstcMftIcu->ICSA32_f.ICP2 = 0u;
        if (pstcMftIcuInternData->pfnIcu2IrqCb != NULL)
        {
             pstcMftIcuInternData->pfnIcu2IrqCb();
        }
    }
    // ICU interrupt 3
    if(TRUE == pstcMftIcu->ICSA32_f.ICP3)
    {
        pstcMftIcu->ICSA32_f.ICP3 = 0u;
        if (pstcMftIcuInternData->pfnIcu3IrqCb != NULL)
        {
             pstcMftIcuInternData->pfnIcu3IrqCb();
        }
    }
}

/**
 ******************************************************************************
 ** \brief Device dependent initialization of interrupts according CMSIS with
 **        level defined in pdl.h
 **
 ** \param  pstcMftIcu Pointer to ICU instance
 ******************************************************************************/
static void Mft_Icu_InitIrq( volatile stc_mftn_icu_t* pstcMftIcu ) 
{
#if (PDL_MCU_CORE == PDL_FM0P_CORE) 
  #if (FM_INTERRUPT_TYPE == PDL_FM0P_INT_TYPE_1_A)
    NVIC_ClearPendingIRQ(MFT0_ICU_IRQn);
    NVIC_EnableIRQ(MFT0_ICU_IRQn);
    NVIC_SetPriority(MFT0_ICU_IRQn, PDL_IRQ_LEVEL_MFT_ICU);
  #elif (FM_INTERRUPT_TYPE == PDL_FM0P_INT_TYPE_2_A)  
    NVIC_ClearPendingIRQ(USB0_F_MFT0_ICU_IRQn);
    NVIC_EnableIRQ(USB0_F_MFT0_ICU_IRQn);
    NVIC_SetPriority(USB0_F_MFT0_ICU_IRQn, PDL_IRQ_LEVEL_MFT_ICU);
  #endif
#elif (PDL_MCU_CORE == PDL_FM3_CORE)   
    NVIC_ClearPendingIRQ(MFT0_2_ICU_IRQn);
    NVIC_EnableIRQ(MFT0_2_ICU_IRQn);
    NVIC_SetPriority(MFT0_2_ICU_IRQn, PDL_IRQ_LEVEL_MFT_ICU);   
#else
    if((volatile stc_mftn_icu_t*)(&MFT0_ICU) == pstcMftIcu)
    {
        NVIC_ClearPendingIRQ(MFT0_ICU_IRQn);
        NVIC_EnableIRQ(MFT0_ICU_IRQn);
        NVIC_SetPriority(MFT0_ICU_IRQn, PDL_IRQ_LEVEL_MFT0_ICU);
    }
  #if(defined(FM_MFT1))    
    if((volatile stc_mftn_icu_t*)(&MFT1_ICU) == pstcMftIcu)
    {
        NVIC_ClearPendingIRQ(MFT1_ICU_IRQn);
        NVIC_EnableIRQ(MFT1_ICU_IRQn);
        NVIC_SetPriority(MFT1_ICU_IRQn, PDL_IRQ_LEVEL_MFT1_ICU);
    }
  #endif  
  #if(defined(FM_MFT2))  
    if((volatile stc_mftn_icu_t*)(&MFT2_ICU) == pstcMftIcu)
    {
        NVIC_ClearPendingIRQ(MFT2_ICU_IRQn);
        NVIC_EnableIRQ(MFT2_ICU_IRQn);
        NVIC_SetPriority(MFT2_ICU_IRQn, PDL_IRQ_LEVEL_MFT2_ICU);
    }
  #endif
#endif

    return;
}

/**
 ******************************************************************************
 ** \brief Device dependent de-initialization of interrupts according CMSIS 
 **
 ** \param [in] pstcMftIcu Pointer to ICU instance
 ******************************************************************************/
static void Mft_Icu_DeInitIrq( volatile stc_mftn_icu_t* pstcMftIcu ) 
{
#if (PDL_MCU_CORE == PDL_FM0P_CORE) 
  #if (FM_INTERRUPT_TYPE == PDL_FM0P_INT_TYPE_1_A)
    NVIC_ClearPendingIRQ(MFT0_ICU_IRQn);
    NVIC_DisableIRQ(MFT0_ICU_IRQn);
    NVIC_SetPriority(MFT0_ICU_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  #elif (FM_INTERRUPT_TYPE == PDL_FM0P_INT_TYPE_2_A)  
    NVIC_ClearPendingIRQ(USB0_F_MFT0_ICU_IRQn);
    NVIC_DisableIRQ(USB0_F_MFT0_ICU_IRQn);
    NVIC_SetPriority(USB0_F_MFT0_ICU_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  #endif
#elif (PDL_MCU_CORE == PDL_FM3_CORE)
    NVIC_ClearPendingIRQ(MFT0_2_ICU_IRQn);
    NVIC_DisableIRQ(MFT0_2_ICU_IRQn);
    NVIC_SetPriority(MFT0_2_ICU_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
#else
    if((volatile stc_mftn_icu_t*)(&MFT0_ICU) == pstcMftIcu)
    {
        NVIC_ClearPendingIRQ(MFT0_ICU_IRQn);
        NVIC_DisableIRQ(MFT0_ICU_IRQn);
        NVIC_SetPriority(MFT0_ICU_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
  #if(defined(FM_MFT1)) 
    if((volatile stc_mftn_icu_t*)(&MFT1_ICU) == pstcMftIcu)
    {
        NVIC_ClearPendingIRQ(MFT1_ICU_IRQn);
        NVIC_DisableIRQ(MFT1_ICU_IRQn);
        NVIC_SetPriority(MFT1_ICU_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
  #endif
  #if(defined(FM_MFT2)) 
    if((volatile stc_mftn_icu_t*)(&MFT2_ICU) == pstcMftIcu)
    {
        NVIC_ClearPendingIRQ(MFT2_ICU_IRQn);
        NVIC_DisableIRQ(MFT2_ICU_IRQn);
        NVIC_SetPriority(MFT2_ICU_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
#endif
#endif
    
    return;
}

#endif

/**
 ******************************************************************************
 ** \brief  Select FRTx channel to connect to ICUx
 **
 ** \param [in] pstcMftIcu    Pointer to ICU instance
 ** \param [in] u8Ch       Mft Icu channel
 ** \arg        MFT_ICU_CH0 ~ MFT_ICU_CH3
 ** \param [in] enFrt      Frt channel number
 ** \arg        Frt0ToIcu,
 **             Frt1ToIcu,
 **             Frt2ToIcu,
 **             IcuFrtToExt0,
 **             IcuFrtToExt1
 ** \retval Ok                     FRT channel selected 
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                                - NULL == pstcMftIcu
 **                                - u8Ch > MFT_ICU_CHx_MAX
 **                                - enFrt > IcuFrtToExt1
 ******************************************************************************/
en_result_t Mft_Icu_SelFrt( volatile stc_mftn_icu_t* pstcMftIcu, uint8_t u8Ch, 
                            en_mft_icu_frt_t enFrt)
{
    volatile stc_mft_icu_icfs10_field_t* stcICFS10;
    volatile stc_mft_icu_icfs32_field_t* stcICFS32;

    // Check for NULL pointer and channel parameter
    boolean_t bAvoidSideEffects  = ((NULL == pstcMftIcu) ? 1u : 0u);
              bAvoidSideEffects |= ((u8Ch > MFT_ICU_CHx_MAX) ? 1u : 0u);
    if (TRUE == bAvoidSideEffects)
    {
        return ErrorInvalidParameter ;
    }

    // Get actual address of register list of current channel
    stcICFS10 = (volatile stc_mft_icu_icfs10_field_t*)(&(pstcMftIcu->ICFS10));
    stcICFS32 = (volatile stc_mft_icu_icfs32_field_t*)(&(pstcMftIcu->ICFS32));
    
    // FRT channel select: FRTx -> ICUx
    if(enFrt > IcuFrtToExt1)
    {
        return ErrorInvalidParameter;
    }
    // configure the Frt channel to connect to Icu
    switch(u8Ch)
    {
        // ICU channel 0
        case MFT_ICU_CH0: 
            stcICFS10->FSI0 = (uint8_t)enFrt;           
            break;
        // ICU channel 1
        case MFT_ICU_CH1:            
            stcICFS10->FSI1 = (uint8_t)enFrt;
            break;
        // ICU channel 2
        case MFT_ICU_CH2:           
            stcICFS32->FSI2 = (uint8_t)enFrt;
            break;
        // ICU channel 3
        case MFT_ICU_CH3:
            stcICFS32->FSI3 = (uint8_t)enFrt;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  Configure ICU module detection mode(different edge)
 **
 ** \param [in] pstcMftIcu    Pointer to ICU instance
 ** \param  [in] u8Ch       Mft Icu channel
 ** \arg        MFT_ICU_CH0 ~ MFT_ICU_CH3
 ** \param  [in] enMode     Icu detect mode
 ** \arg        IcuDisable,
 **             IcuRisingDetect,
 **             IcuFallingDetect,
 **             IcuBothDetect
 ** \retval Ok                     ICU detection mode selected
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                                - NULL == pstcMftIcu
 **                                - u8Ch > MFT_ICU_CHx_MAX
 **                                - enMode > IcuBothDetect
 ******************************************************************************/
en_result_t Mft_Icu_ConfigDetectMode( volatile stc_mftn_icu_t* pstcMftIcu, uint8_t u8Ch, 
                                      en_mft_icu_mode_t enMode)
{
    // Check for NULL pointer and channel parameter
    boolean_t bAvoidSideEffects  = ((NULL == pstcMftIcu) ? 1u : 0u);
    bAvoidSideEffects |= ((u8Ch > MFT_ICU_CHx_MAX) ? 1u : 0u);
    if (bAvoidSideEffects)
    {
        return ErrorInvalidParameter ;
    }

    // Check configuration of Mft Icu mode
    if(enMode > IcuBothDetect)
    {
        return ErrorInvalidParameter;
    }
    // Configure Icu detection mode
    switch (u8Ch)
    {
        // ICU channel 0
        case MFT_ICU_CH0:            
            pstcMftIcu->ICSA10_f.EG0 = (uint8_t)enMode;
            break;
        // ICU channel 1
        case MFT_ICU_CH1:
            pstcMftIcu->ICSA10_f.EG1 = (uint8_t)enMode;
            break;
        // ICU channel 2
        case MFT_ICU_CH2:
            pstcMftIcu->ICSA32_f.EG2 = (uint8_t)enMode;
            break;
        // ICU channel 3
        case MFT_ICU_CH3:
            pstcMftIcu->ICSA32_f.EG3 = (uint8_t)enMode;
            break;
        // ICU channel number error
        default:
            return ErrorInvalidParameter;
    }
    return Ok;
}

#if (PDL_INTERRUPT_ENABLE_MFT0_ICU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_ICU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_ICU == PDL_ON)
/**
 ******************************************************************************
 ** \brief  Enable Mft Icu interrupt
 **
 ** \param [in] pstcMftIcu    Pointer to ICU instance
 ** \param  [in] u8Ch         Mft Icu channel
 ** \arg        MFT_ICU_CH0 ~ MFT_ICU_CH3
 ** \param  [in] pfnIrqCb  register callback function
 ** \arg        user interrupt application function
 ** \param  [in] bTouchNvic   Touch NVIC or not
 ** \retval Ok                     ICU interrupt are enabled
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                                - NULL == pstcMftIcu
 **                                - u8Ch > MFT_ICU_CHx_MAX
 **                                - Other invalid configuration
 ******************************************************************************/
en_result_t Mft_Icu_EnableIrq( volatile stc_mftn_icu_t*pstcMftIcu, uint8_t u8Ch, 
                               func_ptr_t pfnIrqCb, boolean_t bTouchNvic)
{
    // Pointer to internal data
    stc_mft_icu_intern_data_t* pstcMftIcuInternData ;  
    volatile stc_mftn_icu_t* pstcOrgMftIcu = pstcMftIcu;

    // Check for NULL pointer and channel parameter
    boolean_t bAvoidSideEffects  = ((NULL == pstcMftIcu) ? 1u : 0u);
    bAvoidSideEffects |= ((u8Ch > MFT_ICU_CHx_MAX) ? 1u : 0u);
    bAvoidSideEffects |= ((NULL == pfnIrqCb) ? 1u : 0u);
    if (bAvoidSideEffects)
    {
        return ErrorInvalidParameter ;
    }

    // Get pointer to internal data structure
    pstcMftIcuInternData = MftIcuGetInternDataPtr( pstcOrgMftIcu );
    
    switch (u8Ch)
    {
        // ICU channel 0
        case MFT_ICU_CH0:
            pstcMftIcuInternData->pfnIcu0IrqCb = pfnIrqCb;
            pstcMftIcu->ICSA10_f.ICE0 = 1u;
            break;
        // ICU channel 1
        case MFT_ICU_CH1:
            pstcMftIcuInternData->pfnIcu1IrqCb = pfnIrqCb;
            pstcMftIcu->ICSA10_f.ICE1 = 1u;
            break;
        // ICU channel 2
        case MFT_ICU_CH2:
            pstcMftIcuInternData->pfnIcu2IrqCb = pfnIrqCb;
            pstcMftIcu->ICSA32_f.ICE2 = 1u;
            break;
        // ICU channel 3
        case MFT_ICU_CH3:
            pstcMftIcuInternData->pfnIcu3IrqCb = pfnIrqCb;
            pstcMftIcu->ICSA32_f.ICE3 = 1u;
            break;
        // ICU channel number error
        default:
            return ErrorInvalidParameter;
    }

    if(TRUE == bTouchNvic)
    {
        // initialize interrupt
        Mft_Icu_InitIrq(pstcOrgMftIcu);
    }
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  Disable Mft Icu interrupt and release callback function
 **
 ** \param  [in] pstcMftIcu    Pointer to ICU instance
 ** \param  [in] u8Ch          Mft Icu channel
 ** \arg         MFT_ICU_CH0 ~ MFT_ICU_CH3
 ** \param  [in] bTouchNvic    Touch NVIC or not
 **
 ** \retval Ok                     ICU interrupt are disabled
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                                - NULL == pstcMftIcu
 **                                - u8Ch > MFT_ICU_CHx_MAX
 **                                - Other invalid configuration
 ******************************************************************************/

en_result_t Mft_Icu_DisableIrq( volatile stc_mftn_icu_t*pstcMftIcu, 
                                uint8_t u8Ch, 
                                boolean_t bTouchNvic)
{
    // Pointer to internal data
    stc_mft_icu_intern_data_t* pstcMftIcuInternData ;  
    volatile stc_mftn_icu_t* pstcOrgMftIcu = pstcMftIcu;

    // Check for NULL pointer and channel parameter
    boolean_t bAvoidSideEffects  = ((NULL == pstcMftIcu) ? 1u : 0u);
              bAvoidSideEffects |= ((u8Ch > MFT_ICU_CHx_MAX) ? 1u : 0u);
    if (bAvoidSideEffects)
    {
        return ErrorInvalidParameter ;
    }

    // Get pointer to internal data structure
    pstcMftIcuInternData = MftIcuGetInternDataPtr( pstcOrgMftIcu );
    // Dis-register callback function and disable interrupt operation
    switch (u8Ch)
    {
        // ICU channel 0
        case MFT_ICU_CH0:
            pstcMftIcuInternData->pfnIcu0IrqCb = NULL;
            pstcMftIcu->ICSA10_f.ICE0 = 0u;
            break;
        // ICU channel 1
        case MFT_ICU_CH1:
            pstcMftIcuInternData->pfnIcu1IrqCb = NULL;
            pstcMftIcu->ICSA10_f.ICE1 = 0u;
            break;
        // ICU channel 2
        case MFT_ICU_CH2:
            pstcMftIcuInternData->pfnIcu2IrqCb = NULL;
            pstcMftIcu->ICSA32_f.ICE2 = 0u;
            break;
        // ICU channel 3
        case MFT_ICU_CH3:
            pstcMftIcuInternData->pfnIcu3IrqCb = NULL;
            pstcMftIcu->ICSA32_f.ICE3 = 0u;
            break;
        // ICU channel number error
        default:
            return ErrorInvalidParameter;
    }
   
    if(TRUE == bTouchNvic)
    {
        Mft_Icu_DeInitIrq(pstcOrgMftIcu);
    }
    
    return Ok;
}

#endif

/**
 ******************************************************************************
 ** \brief  Get interrupt flag
 **
 ** \param [in] pstcMftIcu    Pointer to ICU instance
 ** \param  [in] u8Ch          Mft Icu channel
 ** \arg        MFT_ICU_CH0 ~ MFT_ICU_CH3
 **
 ** \retval  FALSE ICU active edge is not detected
 ** \retval  TRUE  ICU active edge is detected
 ******************************************************************************/

en_irq_flag_t Mft_Icu_GetIrqFlag(volatile stc_mftn_icu_t *pstcMftIcu, uint8_t u8Ch)
{
    en_irq_flag_t enIrqFlagBuf = PdlClr;

    // Read interrupt flag
    switch (u8Ch)
    {
        // ICU channel 0
        case MFT_ICU_CH0:
            (pstcMftIcu->ICSA10_f.ICP0 == 1u) ? (enIrqFlagBuf = PdlSet) : (enIrqFlagBuf = PdlClr);
            break;
        // ICU channel 1
        case MFT_ICU_CH1:
            (pstcMftIcu->ICSA10_f.ICP1 == 1u) ? (enIrqFlagBuf = PdlSet) : (enIrqFlagBuf = PdlClr);
            break;
        // ICU channel 2
        case MFT_ICU_CH2:         
            (pstcMftIcu->ICSA32_f.ICP2 == 1u) ? (enIrqFlagBuf = PdlSet) : (enIrqFlagBuf = PdlClr);
            break;
        // ICU channel 3
        case MFT_ICU_CH3:          
            (pstcMftIcu->ICSA32_f.ICP3 == 1u) ? (enIrqFlagBuf = PdlSet) : (enIrqFlagBuf = PdlClr);
            break;
        // ICU channel number error
        default:
            break;
    }
    // return interrupt flag
    return enIrqFlagBuf;
}
/**
 ******************************************************************************
 ** \brief  Clear interrupt flag
 **
 ** \param [in] pstcMftIcu    Pointer to ICU instance
 ** \param  [in] u8Ch          Mft Icu channel
 ** \arg        MFT_ICU_CH0 ~ MFT_ICU_CH3
 **
 ** \retval Ok                     Interrupt flag is clear
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                                - NULL == pstcMftIcu
 **                                - u8Ch > MFT_ICU_CHx_MAX
 **                                - Other invalid configuration
 ******************************************************************************/
en_result_t Mft_Icu_ClrIrqFlag( volatile stc_mftn_icu_t *pstcMftIcu, uint8_t u8Ch)
{
    // Check for NULL pointer and channel parameter
    boolean_t bAvoidSideEffects  = ((NULL == pstcMftIcu) ? 1u : 0u);
              bAvoidSideEffects |= ((u8Ch > MFT_ICU_CHx_MAX) ? 1u : 0u);
    if (bAvoidSideEffects)
    {
        return ErrorInvalidParameter ;
    }

	// Clear interrupt flag
    switch (u8Ch)
    {
        // ICU channel 0
        case MFT_ICU_CH0:
            pstcMftIcu->ICSA10_f.ICP0 = 0u;
            break;
        // ICU channel 1
        case MFT_ICU_CH1:
            pstcMftIcu->ICSA10_f.ICP1 = 0u;
            break;
        // ICU channel 2
        case MFT_ICU_CH2:
            pstcMftIcu->ICSA32_f.ICP2 = 0u;
            break;
        // ICU channel 3
        case MFT_ICU_CH3:
            pstcMftIcu->ICSA32_f.ICP3 = 0u;
            break;
        // ICU channel number error
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}
/**
 ******************************************************************************
 ** \brief  Get the latest captured edge type
 **
 ** \param  [in] pstcMftIcu    Pointer to ICU instance
 ** \param  [in] u8Ch          Mft Icu channel
 ** \arg        MFT_ICU_CH0 ~ MFT_ICU_CH3
 **
 ** \return  Detected edge type
 ******************************************************************************/
en_mft_icu_edge_t Mft_Icu_GetLastEdge( volatile stc_mftn_icu_t *pstcMftIcu, uint8_t u8Ch)
{
    en_mft_icu_edge_t enEdgeTypeBuf = IcuFallingEdge;

    // Read last Edge type
    switch (u8Ch)
    {
        // ICU channel 0
        case MFT_ICU_CH0:
            (1u == pstcMftIcu->ICSB10_f.IEI0 ) ? (enEdgeTypeBuf = IcuRisingEdge) : (enEdgeTypeBuf = IcuFallingEdge);
            break;
        // ICU channel 1
        case MFT_ICU_CH1:
            (1u == pstcMftIcu->ICSB10_f.IEI1) ? (enEdgeTypeBuf = IcuRisingEdge) : (enEdgeTypeBuf = IcuFallingEdge);
            break;
        // ICU channel 2
        case MFT_ICU_CH2:
            (1u == pstcMftIcu->ICSB32_f.IEI2) ? (enEdgeTypeBuf = IcuRisingEdge) : (enEdgeTypeBuf = IcuFallingEdge);
            break;
        // ICU channel 3
        case MFT_ICU_CH3:           
            (1u == pstcMftIcu->ICSB32_f.IEI3) ? (enEdgeTypeBuf = IcuRisingEdge) : (enEdgeTypeBuf = IcuFallingEdge);      
            break;
        // ICU channel number error
        default:
            break;
    }
    // return last edge type
    return enEdgeTypeBuf;
}
/**
 ******************************************************************************
 ** \brief  Readout captured data value
 **
 ** \param  [in] pstcMftIcu    Pointer to ICU instance
 ** \param  [in] u8Ch          Mft Icu channel
 ** \arg        MFT_ICU_CH0 ~ MFT_ICU_CH3
 **
 ** \return  captured data value
 ******************************************************************************/
uint16_t Mft_Icu_GetCaptureData(volatile stc_mftn_icu_t *pstcMftIcu, uint8_t u8Ch)
{
    uint16_t u16CaptureVal = 0u;

    // Read captured value
    switch (u8Ch)
    {
        // ICU channel 0
        case MFT_ICU_CH0:
            u16CaptureVal = (uint16_t)(pstcMftIcu->ICCP0);
            break;
        // ICU channel 1
        case MFT_ICU_CH1:
            u16CaptureVal = (uint16_t)(pstcMftIcu->ICCP1);
            break;
        // ICU channel 2
        case MFT_ICU_CH2:
            u16CaptureVal = (uint16_t)(pstcMftIcu->ICCP2);
            break;
        // ICU channel 3
        case MFT_ICU_CH3:
            u16CaptureVal = (uint16_t)(pstcMftIcu->ICCP3);
            break;
        // ICU channel number error
        default:
            break;
    }
    
    return (uint16_t)u16CaptureVal;
}

#endif // #if (defined(PDL_PERIPHERAL_MFT_ICU_ACTIVE))
