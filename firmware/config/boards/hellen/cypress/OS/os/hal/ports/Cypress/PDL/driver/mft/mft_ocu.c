/*******************************************************************************
* \file             mft_ocu.c
* 
* \version          1.20
*
* \brief            This file provides the source code to the API for the 
*                   MFT_OCU driver.
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
#include "mft/mft_ocu.h"

#if (defined(PDL_PERIPHERAL_MFT_OCU_ACTIVE))

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/
#if (PDL_MCU_CORE == PDL_FM3_CORE)
#define MFT_FRT_REG_OFFSET     (0x10u)
#elif (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)
#define MFT_FRT_REG_OFFSET     (0x0Cu)
#endif
/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/
/// Look-up table for all enabled FRT of MFT  instances and their internal data
stc_mft_ocu_instance_data_t m_astcMftOcuInstanceDataLut[OCU_INSTANCE_COUNT] =
{
#if (PDL_PERIPHERAL_ENABLE_MFT0_OCU == PDL_ON)
    {
        &MFT0_OCU,  // pstcInstance
        {0u, 0u, 0u, 0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFT1_OCU == PDL_ON)
    {
        &MFT1_OCU,  // pstcInstance
        {0u, 0u, 0u, 0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFT2_OCU == PDL_ON)
    {
        &MFT2_OCU,  // pstcInstance
        {0u, 0u, 0u, 0u, 0u, 0u}          // stcInternData (not initialized yet)
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
 ** \brief Return the internal data for a certain OCU instance.
 **
 ** \param pstcOcu Pointer to OCU instance
 **
 ** \return Pointer to internal data or NULL if instance is not enabled
 **         (or not known)
 **
 *****************************************************************************/
static stc_mft_ocu_intern_data_t* MftGetInternDataPtr(volatile stc_mftn_ocu_t* pstcOcu)
{
    uint32_t u32Instance;

    for (u32Instance = 0u; u32Instance < OCU_INSTANCE_COUNT; u32Instance++)
    {
        if (pstcOcu == m_astcMftOcuInstanceDataLut[u32Instance].pstcInstance)
        {
            return &m_astcMftOcuInstanceDataLut[u32Instance].stcInternData;
        }
    }

    return NULL;
}

#if (PDL_INTERRUPT_ENABLE_MFT0_OCU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_OCU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_OCU == PDL_ON)  
/*!
 ******************************************************************************
 ** \brief  OCU module interrupt handler
 **
 ** \param [in] pstcOcu Pointer to OCU instance
 ** \param [in]  pstcMftOcuInternData  callback function of OCU
 *****************************************************************************
*/
void Mft_Ocu_IrqHandler( volatile stc_mftn_ocu_t* pstcOcu,
                         stc_mft_ocu_intern_data_t* pstcMftOcuInternData)
{
    func_ptr_t funCallBack;
    uint32_t* ptemp;
    uint8_t Ch;

    //lookup the handler
    for(Ch=0u; Ch<MFT_OCU_MAXCH; Ch++)
    {
        if(PdlSet == Mft_Ocu_GetIrqFlag(pstcOcu,Ch))
        {
            //clear the interrupt flag
            Mft_Ocu_ClrIrqFlag(pstcOcu,Ch);

            //pointer callback function address
            ptemp = (uint32_t*)&(pstcMftOcuInternData->pfnOcu0IrqCallback);
            funCallBack = (func_ptr_t)(*(ptemp + Ch));
            
            if(NULL != funCallBack)
            {
               funCallBack();
            }
        }
    }    
}

/**
 ******************************************************************************
 ** \brief Device dependent initialization of interrupts according CMSIS with
 **        level defined in pdl.h
 **
 ** \param [in] pstcOcu Pointer to OCU instance
 **
 ** \retval Ok    Successful initialization
 **
 ******************************************************************************/
static void MftOcuInitNvic( volatile stc_mftn_ocu_t* pstcOcu )
{
#if (PDL_MCU_CORE == PDL_FM0P_CORE)
    NVIC_ClearPendingIRQ(MFT0_OCU_IRQn);
    NVIC_EnableIRQ(MFT0_OCU_IRQn);
    NVIC_SetPriority(MFT0_OCU_IRQn, PDL_IRQ_LEVEL_MFT_OCU);
#elif (PDL_MCU_CORE == PDL_FM3_CORE) 
    NVIC_ClearPendingIRQ(MFT0_2_OCU_IRQn);
    NVIC_EnableIRQ(MFT0_2_OCU_IRQn);
    NVIC_SetPriority(MFT0_2_OCU_IRQn, PDL_IRQ_LEVEL_MFT_OCU);
#else
#if (PDL_INTERRUPT_ENABLE_MFT0_OCU == PDL_ON)  
    if((volatile stc_mftn_ocu_t*)(&MFT0_OCU) == pstcOcu)
    {
        NVIC_ClearPendingIRQ(MFT0_OCU_IRQn);
        NVIC_EnableIRQ(MFT0_OCU_IRQn);
        NVIC_SetPriority(MFT0_OCU_IRQn, PDL_IRQ_LEVEL_MFT0_OCU);
    }
#endif 
#if (PDL_INTERRUPT_ENABLE_MFT1_OCU == PDL_ON)     
    if((volatile stc_mftn_ocu_t*)(&MFT1_OCU) == pstcOcu)
    {
        NVIC_ClearPendingIRQ(MFT1_OCU_IRQn);
        NVIC_EnableIRQ(MFT1_OCU_IRQn);
        NVIC_SetPriority(MFT1_OCU_IRQn, PDL_IRQ_LEVEL_MFT1_OCU);
    }
#endif
#if (PDL_INTERRUPT_ENABLE_MFT2_OCU == PDL_ON)      
    if((volatile stc_mftn_ocu_t*)(&MFT2_OCU) == pstcOcu)
    {
        NVIC_ClearPendingIRQ(MFT2_OCU_IRQn);
        NVIC_EnableIRQ(MFT2_OCU_IRQn);
        NVIC_SetPriority(MFT2_OCU_IRQn, PDL_IRQ_LEVEL_MFT2_OCU);
    }
#endif
    
#endif
    
    return ;
}

/**
 ******************************************************************************
 ** \brief Device dependent de-initialization of interrupts according CMSIS
 **
 ** \param [in] pstcOcu Pointer to OCU instance
 **
 ** \retval Ok    Successful initialization
 **
 ******************************************************************************/
static void MftOcuDeInitNvic( volatile stc_mftn_ocu_t* pstcOcu )
{ 
#if (PDL_MCU_CORE == PDL_FM0P_CORE)  
    NVIC_ClearPendingIRQ(MFT0_OCU_IRQn);
    NVIC_DisableIRQ(MFT0_OCU_IRQn);
    NVIC_SetPriority(MFT0_OCU_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
#elif (PDL_MCU_CORE == PDL_FM3_CORE)
    NVIC_ClearPendingIRQ(MFT0_2_OCU_IRQn);
    NVIC_DisableIRQ(MFT0_2_OCU_IRQn);
    NVIC_SetPriority(MFT0_2_OCU_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
#else
#if (PDL_INTERRUPT_ENABLE_MFT0_OCU == PDL_ON)    
    if((volatile stc_mftn_ocu_t*)(&MFT0_OCU) == pstcOcu)
    {
        NVIC_ClearPendingIRQ(MFT0_OCU_IRQn);
        NVIC_DisableIRQ(MFT0_OCU_IRQn);
        NVIC_SetPriority(MFT0_OCU_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
#endif
#if (PDL_INTERRUPT_ENABLE_MFT1_OCU == PDL_ON)     
    if((volatile stc_mftn_ocu_t*)(&MFT1_OCU) == pstcOcu)
    {
        NVIC_ClearPendingIRQ(MFT1_OCU_IRQn);
        NVIC_DisableIRQ(MFT1_OCU_IRQn);
        NVIC_SetPriority(MFT1_OCU_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
#endif
#if (PDL_INTERRUPT_ENABLE_MFT2_OCU == PDL_ON)     
    if((volatile stc_mftn_ocu_t*)(&MFT2_OCU) == pstcOcu)
    {
        NVIC_ClearPendingIRQ(MFT2_OCU_IRQn);
        NVIC_DisableIRQ(MFT2_OCU_IRQn);
        NVIC_SetPriority(MFT2_OCU_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
#endif
    
#endif    
  
    return;
}

#endif

/******************************************************************************/
/* Local Functions                                                            */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief  Init OCU module
 **
 ** \param [in] pstcOcu Pointer to OCU instance
 ** \param [in]  u8Ch  channel of OCU
 ** \arg    MFT_OCU_CH0 ~ MFT_OCU_CH5
 ** \param [in] pstcOcuConfig configure of OCU
 ** \arg    structure of OCU cofnigure
 **
 ** \retval Ok                    OCU module initialized
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **                               - pstcOcuConfig == NULL
 **                               - Other invalid configuration
 **
 ******************************************************************************/
en_result_t Mft_Ocu_Init( volatile stc_mftn_ocu_t* pstcOcu, 
                          uint8_t u8Ch, const stc_mft_ocu_config_t*  pstcOcuConfig)
{
    stc_mft_ocu_intern_data_t* pstcOcuInternData;
    volatile stc_mft_ocu_ocfs10_field_t* pstcOCFS;
#if (PDL_MCU_CORE == PDL_FM3_CORE) || \
    (PDL_INTERRUPT_ENABLE_MFT0_OCU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_OCU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_OCU == PDL_ON) 
    volatile stc_mft_ocu_ocsa10_field_t* pstcOCSA;
#endif    
    volatile stc_mft_ocu_ocsb10_field_t* pstcOCSB;
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE) 
    volatile stc_mft_ocu_ocsd10_field_t* pstcOCSD;
#endif
    // Check for NULL pointer and configuration parameter
    if ( (NULL == pstcOcu) || 
         (u8Ch >= MFT_OCU_MAXCH) )
    {
        return ErrorInvalidParameter ;
    }
    
    // Get pointer to internal data structure ...
    pstcOcuInternData = MftGetInternDataPtr( pstcOcu ) ;
    // ... and check for NULL
    if ( NULL == pstcOcuInternData )
    {
    	return ErrorInvalidParameter;
    }

    //Get pointer of current channel OCU register address
#if (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)    
    pstcOCFS = (volatile stc_mft_ocu_ocfs10_field_t*)((volatile uint8_t*)&pstcOcu->OCFS10 + u8Ch/2u);
#else
    if(u8Ch < 4)
    {
        pstcOCFS = (volatile stc_mft_ocu_ocfs10_field_t*)((volatile uint8_t*)&pstcOcu->OCFS10 + u8Ch/2u);
    }
    else
    {
        pstcOCFS = (volatile stc_mft_ocu_ocfs10_field_t*)((volatile uint8_t*)&pstcOcu->OCFS10 + 4u);
    }
#endif    
#if (PDL_MCU_CORE == PDL_FM3_CORE) || \
    (PDL_INTERRUPT_ENABLE_MFT0_OCU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_OCU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_OCU == PDL_ON)      
    pstcOCSA = (volatile stc_mft_ocu_ocsa10_field_t*)((volatile uint8_t*)&pstcOcu->OCSA10 + (u8Ch/2u)*4u);
#endif    
    pstcOCSB = (volatile stc_mft_ocu_ocsb10_field_t*)((volatile uint8_t*)&pstcOcu->OCSB10 + (u8Ch/2u)*4u);
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)     
    pstcOCSD = (volatile stc_mft_ocu_ocsd10_field_t*)((volatile uint8_t*)&pstcOcu->OCSD10 + (u8Ch/2u)*4u);
#endif    

#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)      
    //set OCSB register, config ocu's operation
    pstcOCSB->FM4 = ((TRUE == pstcOcuConfig->bFm4) ? 1u : 0u);
#endif    

    // Set OCSE and OCCP buffer mode
    if(0u == (u8Ch % 2u)) //channel 0, 2, 4
    {
       // OCSE buffer
    #if(PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE) 
        switch (pstcOcuConfig->enOcseBufMode)
        {
            case OcseBufDisable:
                pstcOCSD->OCSE0BUFE = 0u;
                break;
            case OcseBufTrsfByFrtZero:
                pstcOCSD->OCSE0BUFE = 1u;
                break;
            case OcseBufTrsfByFrtPeak:
                pstcOCSD->OCSE0BUFE = 2u;
                break;
            case OcseBufTrsfByFrtZeroPeak:
                pstcOCSD->OCSE0BUFE = 3u;
                break;
        #if(PDL_MCU_TYPE >= PDL_FM4_TYPE3)       
            case OcseBufTrsfByFrtZeroMszcZero:
                pstcOCSD->OPBM0 = 1u;
                pstcOCSD->OCSE0BUFE = 1u;
                break;
            case OcseBufTrsfByFrtPeakMspcZero:
                pstcOCSD->OPBM0 = 1u;
                pstcOCSD->OCSE0BUFE = 2u;
                break;
            case OcseBufTrsfByFrtZeroMszcZeroOrFrtPeakMspcZero:   
                pstcOCSD->OPBM0 = 1u;
                pstcOCSD->OCSE0BUFE = 3u;
                break;
        #endif        
            default:
                return ErrorInvalidParameter ;
        }
    #endif
        
        // OCCP buffer
        switch (pstcOcuConfig->enOccpBufMode)
        {
            case OccpBufDisable:
            #if(PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
                pstcOCSD->OCCP0BUFE = 0u;
            #else
                pstcOCSA->BDIS0 = 1u;
            #endif   
                break;
            case OccpBufTrsfByFrtZero:
            #if(PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
                pstcOCSD->OCCP0BUFE = 1u;
            #else 
                pstcOCSB->BTS0 = 0u;
                pstcOCSA->BDIS0 = 0u;
            #endif
                break;
            case OccpBufTrsfByFrtPeak:
            #if(PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
                pstcOCSD->OCCP0BUFE = 2u;
            #else
                pstcOCSB->BTS0 = 1u;
                pstcOCSA->BDIS0 = 0u;
            #endif    
                break;
       #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)          
            case OccpBufTrsfByFrtZeroPeak:
                pstcOCSD->OCCP0BUFE = 3u;
                break;
       #endif         
       #if(PDL_MCU_CORE == PDL_FM4_CORE)    
            #if (PDL_MCU_TYPE >= PDL_FM4_TYPE3)         
            case OccpBufTrsfByFrtZeroMszcZero:
                pstcOCSD->OEBM0 = 1u;
                pstcOCSD->OCCP0BUFE = 1u;
                break;
            case OccpBufTrsfByFrtPeakMspcZero:
                pstcOCSD->OEBM0 = 1u;
                pstcOCSD->OCCP0BUFE = 2u;
                break;
            case OccpBufTrsfByFrtZeroMszcZeroOrFrtPeakMspcZero:
                pstcOCSD->OEBM0 = 1u;
                pstcOCSD->OCCP0BUFE = 3u;
                break;
            #endif                
        #endif        
            default:
                return ErrorInvalidParameter ;
        }
        
        // set FRT to be connected
        switch (pstcOcuConfig->enFrtConnect)
        {
            case Frt0ToOcu:
                pstcOCFS->FSO0 = 0u;
                break;
            case Frt1ToOcu:
                pstcOCFS->FSO0 = 1u;
                break;
            case Frt2ToOcu:
                pstcOCFS->FSO0 = 2u;
                break;
            default:
                return ErrorInvalidParameter ;
        }
        
        // Set initial RT level
        pstcOCSB->OTD0 = ((pstcOcuConfig->enPinState == RtHighLevel) ? 1u : 0u);
    }
    else//channel 1, 3, 5
    {
        // OCSE buffer
    #if(PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE) 
        switch (pstcOcuConfig->enOcseBufMode)
        {
            case OcseBufDisable:
                pstcOCSD->OCSE1BUFE = 0u;
                break;
            case OcseBufTrsfByFrtZero:
                pstcOCSD->OCSE1BUFE = 1u;
                break;
            case OcseBufTrsfByFrtPeak:
                pstcOCSD->OCSE1BUFE = 2u;
                break;
            case OcseBufTrsfByFrtZeroPeak:
                pstcOCSD->OCSE1BUFE = 3u;
                break;
        #if(PDL_MCU_TYPE >= PDL_FM4_TYPE3)       
            case OcseBufTrsfByFrtZeroMszcZero:
                pstcOCSD->OPBM1 = 1u;
                pstcOCSD->OCSE1BUFE = 1u;
                break;
            case OcseBufTrsfByFrtPeakMspcZero:
                pstcOCSD->OPBM1 = 1u;
                pstcOCSD->OCSE1BUFE = 2u;
                break;
            case OcseBufTrsfByFrtZeroMszcZeroOrFrtPeakMspcZero:   
                pstcOCSD->OPBM1 = 1u;
                pstcOCSD->OCSE1BUFE = 3u;
                break;
        #endif        
            default:
                return ErrorInvalidParameter ;
        }
    #endif
        
        // OCCP buffer
        switch (pstcOcuConfig->enOccpBufMode)
        {
            case OccpBufDisable:
            #if(PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
                pstcOCSD->OCCP1BUFE = 0u;
            #else
                pstcOCSA->BDIS1 = 1u;
            #endif   
                break;
            case OccpBufTrsfByFrtZero:
            #if(PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
                pstcOCSD->OCCP1BUFE = 1u;
            #else 
                pstcOCSB->BTS1 = 0u;
                pstcOCSA->BDIS1 = 0u;
            #endif
                break;
            case OccpBufTrsfByFrtPeak:
            #if(PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
                pstcOCSD->OCCP1BUFE = 2u;
            #else
                pstcOCSB->BTS1 = 1u;
                pstcOCSA->BDIS1 = 0u;
            #endif    
                break;
       #if(PDL_MCU_CORE == PDL_FM4_CORE) ||  (PDL_MCU_CORE == PDL_FM0P_CORE)        
            case OccpBufTrsfByFrtZeroPeak:
                pstcOCSD->OCCP1BUFE = 3u;
                break;
       #endif         
       #if(PDL_MCU_CORE == PDL_FM4_CORE)    
            #if (PDL_MCU_TYPE >= PDL_FM4_TYPE3)         
            case OccpBufTrsfByFrtZeroMszcZero:
                pstcOCSD->OEBM1 = 1u;
                pstcOCSD->OCCP1BUFE = 1u;
                break;
            case OccpBufTrsfByFrtPeakMspcZero:
                pstcOCSD->OEBM1 = 1u;
                pstcOCSD->OCCP1BUFE = 2u;
                break;
            case OccpBufTrsfByFrtZeroMszcZeroOrFrtPeakMspcZero:
                pstcOCSD->OEBM1 = 1u;
                pstcOCSD->OCCP1BUFE = 3u;
                break;
            #endif                
        #endif        
            default:
                return ErrorInvalidParameter ;
        }
        
        // set FRT to be connected
        switch (pstcOcuConfig->enFrtConnect)
        {
            case Frt0ToOcu:
                pstcOCFS->FSO1 = 0u;
                break;
            case Frt1ToOcu:
                pstcOCFS->FSO1 = 1u;
                break;
            case Frt2ToOcu:
                pstcOCFS->FSO1 = 2u;
                break;
            default:
                return ErrorInvalidParameter ;
        }
        
        // Set initial RT level
        pstcOCSB->OTD1 = ((pstcOcuConfig->enPinState == RtHighLevel) ? 1u : 0u);
    }
    
#if (PDL_INTERRUPT_ENABLE_MFT0_OCU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_OCU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_OCU == PDL_ON)    
    // set interrupt enable
    if(TRUE == pstcOcuConfig->bIrqEnable)
    {
        if(0u == (u8Ch%2u)) //channel 0,2,4
        {
            pstcOCSA->IOE0 = 1u;
        }
        else //channel 1,3,5
        {
            pstcOCSA->IOE1 = 1u;
        }
    }
    
    // set interrupt callback funciton
    switch (u8Ch)
    {
        case 0u:
            pstcOcuInternData->pfnOcu0IrqCallback = pstcOcuConfig->pfnIrqCallback;
            break;
        case 1u:
            pstcOcuInternData->pfnOcu1IrqCallback = pstcOcuConfig->pfnIrqCallback;
            break;
        case 2u:
            pstcOcuInternData->pfnOcu2IrqCallback = pstcOcuConfig->pfnIrqCallback;
            break;
        case 3u:
            pstcOcuInternData->pfnOcu3IrqCallback = pstcOcuConfig->pfnIrqCallback;
            break;
        case 4u:
            pstcOcuInternData->pfnOcu4IrqCallback = pstcOcuConfig->pfnIrqCallback;
            break;
        case 5u:
            pstcOcuInternData->pfnOcu5IrqCallback = pstcOcuConfig->pfnIrqCallback;
            break;
        default:
            break;
    }
    
    // set NVIC
    if(TRUE == pstcOcuConfig->bTouchNvic)
    {
        MftOcuInitNvic(pstcOcu);
    }
#endif
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  De-Initialize OCU module
 **
 ** \param [in] pstcOcu Pointer to OCU instance
 ** \param [in]  u8Ch  channel of OCU
 ** \arg    MFT_OCU_CH0 ~ MFT_OCU_CH5
 ** \param [in] bTouchNvic
 ** \arg   FALSE  Don't disable NVIC
 ** \arg   TRUE   Disable NVIC
 **
 ** \retval Ok                    OCU module initialized
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **                               - Other invalid configuration
 **
 ******************************************************************************/
en_result_t Mft_Ocu_DeInit( volatile stc_mftn_ocu_t* pstcOcu, 
                            uint8_t u8Ch,
                            boolean_t bTouchNvic)
{
    volatile uint8_t* pu8OCSA;
    volatile uint8_t* pu8OCSB;
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)      
    volatile uint16_t* pu16OCSD;
#endif    
    volatile uint8_t* pu8OCFS;
    
    pu8OCSA = (volatile uint8_t*)((uint8_t*)&pstcOcu->OCSA10 + (u8Ch/2u)*4u);
    pu8OCSB = (volatile uint8_t*)((uint8_t*)&pstcOcu->OCSB10 + (u8Ch/2u)*4u);
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)     
    pu16OCSD = (volatile uint16_t*)((uint8_t*)&pstcOcu->OCSD10 + (u8Ch/2u)*4u);
#endif    

#if (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)       
    pu8OCFS  = (volatile uint8_t*)((volatile uint8_t*)&pstcOcu->OCFS10 + u8Ch/2u);
#else
    if (u8Ch < 4)
    {
    pu8OCFS  = (volatile uint8_t*)((volatile uint8_t*)&pstcOcu->OCFS10 + u8Ch/2u);
    }
    else
    {
        pu8OCFS  = (volatile uint8_t*)((volatile uint8_t*)&pstcOcu->OCFS10 + 4u);
    }
#endif    
    
    *pu8OCSA = 0u;
    *pu8OCSB = 0u;
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
    *pu16OCSD = 0u;
#endif    
    *pu8OCFS  = 0u;

#if (PDL_INTERRUPT_ENABLE_MFT0_OCU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_OCU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_OCU == PDL_ON)        
    if(TRUE == bTouchNvic)
    {
        MftOcuDeInitNvic(pstcOcu);
    }
#endif    
    
    return Ok;
}

#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)
/**
 ******************************************************************************
 ** \brief  Compare congifuration of even OCU channel
 **
 ** \param [in] pstcOcu Pointer to OCU instance
 ** \param [in]  EvenCh  even channel of OCU
 ** \arg    MFT_OCU_CH0, MFT_OCU_CH2, MFT_OCU_CH4
 ** \param [in] pstcConfig pointer to structure of compare mode
 **
 ** \retval Ok                    Even OCU channel compare mode is set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **                               - EvenCh out of range
 **                               - Other invalid configuration
 **
 ******************************************************************************/
en_result_t Mft_Ocu_SetEvenChCompareMode(volatile stc_mftn_ocu_t* pstcOcu,
                                         uint8_t EvenCh,
                                         stc_ocu_even_compare_config_t* pstcConfig)
{
    volatile uint16_t* p16OCSE0;

    // Check for NULL pointer
    if ( (NULL == pstcOcu)
         || (0u != EvenCh%2) 
         || (NULL == pstcConfig) 
         || (pstcConfig->enFrtZeroEvenMatchEvenChRtStatus > RtOutputReverse)  
         || (pstcConfig->enFrtZeroEvenNotMatchEvenChRtStatus > RtOutputReverse)  
         || (pstcConfig->enFrtUpCntEvenMatchEvenChRtStatus > RtOutputReverse) 
         || (pstcConfig->enFrtPeakEvenMatchEvenChRtStatus > RtOutputReverse) 
         || (pstcConfig->enFrtPeakEvenNotMatchEvenChStatus > RtOutputReverse)  
         || (pstcConfig->enFrtDownCntEvenMatchEvenChRtStatus > RtOutputReverse)  
         || (pstcConfig->enIopFlagWhenFrtZeroEvenMatch > IopFlagSet)  
         || (pstcConfig->enIopFlagWhenFrtUpCntEvenMatch > IopFlagSet) 
         || (pstcConfig->enIopFlagWhenFrtPeakEvenMatch > IopFlagSet)  
         || (pstcConfig->enIopFlagWhenFrtDownCntEvenMatch > IopFlagSet)  )
    {
        return ErrorInvalidParameter ;
    }
    
    p16OCSE0 = (volatile uint16_t*)((volatile uint8_t*)&pstcOcu->OCSE0 + EvenCh*4u);
    *p16OCSE0 = 0u;
    *p16OCSE0 |= (uint16_t)((uint16_t)pstcConfig->enFrtZeroEvenMatchEvenChRtStatus    << 10u);
    *p16OCSE0 |= (uint16_t)((uint16_t)pstcConfig->enFrtZeroEvenNotMatchEvenChRtStatus << 14u); 
    *p16OCSE0 |= (uint16_t)((uint16_t)pstcConfig->enFrtUpCntEvenMatchEvenChRtStatus   << 8u);
    *p16OCSE0 |= (uint16_t)((uint16_t)pstcConfig->enFrtPeakEvenMatchEvenChRtStatus    << 6u);                   
    *p16OCSE0 |= (uint16_t)((uint16_t)pstcConfig->enFrtPeakEvenNotMatchEvenChStatus   << 12u);
    *p16OCSE0 |= (uint16_t)((uint16_t)pstcConfig->enFrtDownCntEvenMatchEvenChRtStatus << 4u);
    *p16OCSE0 |= (uint16_t)((uint16_t)pstcConfig->enIopFlagWhenFrtZeroEvenMatch       << 3u);
    *p16OCSE0 |= (uint16_t)((uint16_t)pstcConfig->enIopFlagWhenFrtUpCntEvenMatch      << 2u);
    *p16OCSE0 |= (uint16_t)((uint16_t)pstcConfig->enIopFlagWhenFrtPeakEvenMatch       << 1u);
    *p16OCSE0 |= (uint16_t)((uint16_t)pstcConfig->enIopFlagWhenFrtDownCntEvenMatch    << 0u);
#if (PDL_MCU_CORE == PDL_FM4_CORE)
    #if(PDL_MCU_TYPE >= PDL_FM4_TYPE3)     
    (TRUE == pstcConfig->bFrtZeroPeakExtendMatchCondition) ? (*p16OCSE0 |= (1ul<<12u)) : (*p16OCSE0 &= ~(1ul<<12u)); 
    #endif
#endif    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  Compare congifuration of odd OCU channel
 **
 ** \param [in] pstcOcu Pointer to OCU instance
 ** \param [in]  OddCh  odd channel of OCU
 ** \arg    MFT_OCU_CH1, MFT_OCU_CH3, MFT_OCU_CH5
 ** \param [in] pstcConfig pointer to structure of compare mode
 **
 ** \retval Ok                    Odd OCU channel compare mode is set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **                               - OddCh out of range
 **                               - Other invalid configuration
 **
 ******************************************************************************/
en_result_t Mft_Ocu_SetOddChCompareMode (volatile stc_mftn_ocu_t*pstcOcu,
                                         uint8_t OddCh,
                                         stc_ocu_odd_compare_config_t* pstcConfig)
{
    volatile uint32_t* pu32OCSE1;
    
    // Check for NULL pointer
    if ( (NULL == pstcOcu)
         || (0u == OddCh%2u) 
         || (pstcConfig->enFrtZeroOddMatchEvenMatchOddChRtStatus > RtOutputReverse)     
         || (pstcConfig->enFrtZeroOddMatchEvenNotMatchOddChRtStatus > RtOutputReverse)     
         || (pstcConfig->enFrtZeroOddNotMatchEvenMatchOddChRtStatus > RtOutputReverse)     
         || (pstcConfig->enFrtZeroOddNotMatchEvenNotMatchOddChRtStatus > RtOutputReverse)       
         || (pstcConfig->enFrtUpCntOddMatchEvenMatchOddChRtStatus  > RtOutputReverse)    
         || (pstcConfig->enFrtUpCntOddMatchEvenNotMatchOddChRtStatus > RtOutputReverse)      
         || (pstcConfig->enFrtUpCntOddNotMatchEvenMatchOddChRtStatus > RtOutputReverse)       
         || (pstcConfig->enFrtPeakOddMatchEvenMatchOddChRtStatus     > RtOutputReverse)     
         || (pstcConfig->enFrtPeakOddMatchEvenNotMatchOddChRtStatus  > RtOutputReverse)      
         || (pstcConfig->enFrtPeakOddNotMatchEvenMatchOddChRtStatus  > RtOutputReverse)     
         || (pstcConfig->enFrtPeakOddNotMatchEvenNotMatchOddChRtStatus > RtOutputReverse)      
         || (pstcConfig->enFrtDownOddMatchEvenMatchOddChRtStatus       > RtOutputReverse)     
         || (pstcConfig->enFrtDownOddMatchEvenNotMatchOddChRtStatus    > RtOutputReverse)     
         || (pstcConfig->enFrtDownOddNotMatchEvenMatchOddChRtStatus    > RtOutputReverse)      
         || (pstcConfig->enIopFlagWhenFrtZeroOddMatch    > IopFlagSet)      
         || (pstcConfig->enIopFlagWhenFrtUpCntOddMatch   > IopFlagSet)      
         || (pstcConfig->enIopFlagWhenFrtPeakOddMatch    > IopFlagSet)     
         || (pstcConfig->enIopFlagWhenFrtDownCntOddMatch > IopFlagSet) )
    {
        return ErrorInvalidParameter ;
    }
    
    pu32OCSE1 = (volatile uint32_t*)((volatile uint8_t*)&pstcOcu->OCSE0 + OddCh*4u);
    *pu32OCSE1 = 0u;
    *pu32OCSE1 |= (uint32_t)((uint32_t)pstcConfig->enFrtZeroOddMatchEvenMatchOddChRtStatus        << 26u);
    *pu32OCSE1 |= (uint32_t)((uint32_t)pstcConfig->enFrtZeroOddMatchEvenNotMatchOddChRtStatus     << 10u);
    *pu32OCSE1 |= (uint32_t)((uint32_t)pstcConfig->enFrtZeroOddNotMatchEvenMatchOddChRtStatus     << 30u);
    *pu32OCSE1 |= (uint32_t)((uint32_t)pstcConfig->enFrtZeroOddNotMatchEvenNotMatchOddChRtStatus  << 14u);
    *pu32OCSE1 |= (uint32_t)((uint32_t)pstcConfig->enFrtUpCntOddMatchEvenMatchOddChRtStatus       << 24u);
    *pu32OCSE1 |= (uint32_t)((uint32_t)pstcConfig->enFrtUpCntOddMatchEvenNotMatchOddChRtStatus    << 8u);
    *pu32OCSE1 |= (uint32_t)((uint32_t)pstcConfig->enFrtUpCntOddNotMatchEvenMatchOddChRtStatus    << 18u);
    *pu32OCSE1 |= (uint32_t)((uint32_t)pstcConfig->enFrtPeakOddMatchEvenMatchOddChRtStatus        << 22u);
    *pu32OCSE1 |= (uint32_t)((uint32_t)pstcConfig->enFrtPeakOddMatchEvenNotMatchOddChRtStatus     << 6u) ;
    *pu32OCSE1 |= (uint32_t)((uint32_t)pstcConfig->enFrtPeakOddNotMatchEvenMatchOddChRtStatus     << 28u);
    *pu32OCSE1 |= (uint32_t)((uint32_t)pstcConfig->enFrtPeakOddNotMatchEvenNotMatchOddChRtStatus  << 12u);
    *pu32OCSE1 |= (uint32_t)((uint32_t)pstcConfig->enFrtDownOddMatchEvenMatchOddChRtStatus        << 20u);
    *pu32OCSE1 |= (uint32_t)((uint32_t)pstcConfig->enFrtDownOddMatchEvenNotMatchOddChRtStatus     << 4u);
    *pu32OCSE1 |= (uint32_t)((uint32_t)pstcConfig->enFrtDownOddNotMatchEvenMatchOddChRtStatus     << 16u); 
    *pu32OCSE1 |= (uint32_t)((uint32_t)pstcConfig->enIopFlagWhenFrtZeroOddMatch                   << 3u);
    *pu32OCSE1 |= (uint32_t)((uint32_t)pstcConfig->enIopFlagWhenFrtUpCntOddMatch                  << 2u);
    *pu32OCSE1 |= (uint32_t)((uint32_t)pstcConfig->enIopFlagWhenFrtPeakOddMatch                   << 1u);
    *pu32OCSE1 |= (uint32_t)((uint32_t)pstcConfig->enIopFlagWhenFrtDownCntOddMatch                << 0u);
#if (PDL_MCU_CORE == PDL_FM4_CORE)
    #if(PDL_MCU_TYPE >= PDL_FM4_TYPE3)    
    (TRUE == pstcConfig->bFrtZeroPeakExtendMatchCondition) ? (*pu32OCSE1 |= (1ul<<13u)) : (*pu32OCSE1 &= ~(1ul<<13u)); 
    #endif
#endif    
    return Ok;
}

#endif

/**
 ******************************************************************************
 ** \brief  Set compare mode for FM3 compatible mode 
 **
 ** \param [in] pstcOcu Pointer to OCU instance
 ** \param [in]  u8CoupleCh  Couple channel of OCU
 ** \arg    MFT_OCU_CH0 ~ MFT_OCU_CH5
 ** \param [in] enCompareMode compare mode of OCU
 ** \arg   See the structure type of en_ocu_compare_mode_t
 **
 ** \retval Ok                    OCU mode been set
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **                               - pstcOcuConfig == NULL
 **                               - Other invalid configuration
 **
 ******************************************************************************/
en_result_t Mft_Ocu_SetCompareMode_Fm3 (volatile stc_mftn_ocu_t* pstcOcu, 
                                        uint8_t u8CoupleCh,
                                        en_ocu_compare_mode_t enCompareMode)
{    
    uint8_t u8Mod;
    
    if (NULL == pstcOcu)
    {
        return ErrorInvalidParameter ;
    }
    
    switch (enCompareMode)
    {
        case OcuOdd1ChangeEven1Change:
        case OcuOdd1ChangeEvenActiveHigh:  
        case OcuOddActiveHighEven1Change:  
        case OcuOddActiveHighEvenActiveHigh:  
            switch (u8CoupleCh)
            {
                case MFT_OCU_CH10:
                    pstcOcu->OCSB10_f.CMOD = 0u;
                    break;
                case MFT_OCU_CH32:
                    pstcOcu->OCSB32_f.CMOD = 0u;
                    break;
                case MFT_OCU_CH54:
                    pstcOcu->OCSB54_f.CMOD = 0u;
                    break;
                default:
                    return ErrorInvalidParameter ;
            }
            break;
        case OcuOdd2ChangeEven1Change:
        case OcuOddActiveLowEven1Change:
        case OcuOddActiveLowEvenActiveLow:
            switch (u8CoupleCh)
            {
                case MFT_OCU_CH10:
                    pstcOcu->OCSB10_f.CMOD = 1u;
                    break;
                case MFT_OCU_CH32:
                    pstcOcu->OCSB32_f.CMOD = 1u;
                    break;
                case MFT_OCU_CH54:
                    pstcOcu->OCSB54_f.CMOD = 1u;
                    break;
                default:
                    return ErrorInvalidParameter ;
            }
            break;
        default:
            return ErrorInvalidParameter ;
      
    }
    
    switch (enCompareMode)
    {
        case OcuOdd1ChangeEven1Change:
        case OcuOdd2ChangeEven1Change:
            u8Mod = 0u;
            break;
        case OcuOdd1ChangeEvenActiveHigh:
            u8Mod = 1u;
            break;
        case OcuOddActiveHighEven1Change:
        case OcuOddActiveLowEven1Change:
            u8Mod = 2u;
            break;
        case OcuOddActiveHighEvenActiveHigh:
        case OcuOddActiveLowEvenActiveLow:
            u8Mod = 3u;
            break;
        default:
            return ErrorInvalidParameter ;
      
    }
    
    pstcOcu->OCSC &= ~(3u << (u8CoupleCh * 2u));
    pstcOcu->OCSC |= ((uint8_t)u8Mod << (u8CoupleCh * 2u));
    
    return Ok;
}

/*!
 ******************************************************************************
 ** \brief  enable ocu operation
 **
 ** \param [in] pstcOcu Pointer to OCU instance
 ** \param [in]  u8Ch  channel of OCU
 ** \arg    MFT_OCU_CH0 ~ MFT_OCU_CH5
 **
 ** \retval Ok                    OCU operation enabled
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **                               - u8Ch out of range
 **                               - Other invalid configuration
 **
 *****************************************************************************
*/
en_result_t Mft_Ocu_EnableOperation(volatile stc_mftn_ocu_t* pstcOcu, uint8_t u8Ch)
{
    volatile stc_mft_ocu_ocsa10_field_t* pstcOCSA;

    // Check for NULL pointer
    if ( (NULL == pstcOcu)
         || (u8Ch >= MFT_OCU_MAXCH))
    {
        return ErrorInvalidParameter ;
    }

    //Get pointer of current channel OCU register address
    pstcOCSA = (volatile stc_mft_ocu_ocsa10_field_t*)((volatile uint8_t*)&pstcOcu->OCSA10 + (u8Ch/2u)*4u);

    //set register
    if(0u == (u8Ch%2)) //channel 0,2,4
    {
    	pstcOCSA->CST0 = 1u;
    }
    else //channel 1,3,5
    {
    	pstcOCSA->CST1 = 1u;
    }

    return Ok;
}

/*!
 ******************************************************************************
 ** \brief  Disable ocu operation
 **
 ** \param [in] pstcOcu Pointer to OCU instance
 ** \param [in]  u8Ch  channel of OCU
 ** \arg    MFT_OCU_CH0 ~ MFT_OCU_CH5
 **
 ** \retval Ok                    OCU operation disabled
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **                               - u8Ch out of range
 **                               - Other invalid configuration
 **
 *****************************************************************************
*/
en_result_t Mft_Ocu_DisableOperation(volatile stc_mftn_ocu_t* pstcOcu, uint8_t u8Ch)
{
    volatile stc_mft_ocu_ocsa10_field_t* pstcOCSA;

    // Check for NULL pointer
    if ( (NULL == pstcOcu)
         || (u8Ch >= MFT_OCU_MAXCH))
    {
        return ErrorInvalidParameter ;
    }

    //Get pointer of current channel OCU register address
    pstcOCSA = (volatile stc_mft_ocu_ocsa10_field_t*)((volatile uint8_t*)&pstcOcu->OCSA10 + ((u8Ch/2u) * 4u));

    //set register
    if(0 == (u8Ch%2)) //channel 0,2,4
    {
    	pstcOCSA->CST0 = 0u;
    }
    else //channel 1,3,5
    {
    	pstcOCSA->CST1 = 0u;
    }

    return Ok;
}

#if (PDL_INTERRUPT_ENABLE_MFT0_OCU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_OCU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_OCU == PDL_ON)
/*!
 ******************************************************************************
 ** \brief  enable ocu interrupt
 **
 ** \param [in] pstcOcu Pointer to OCU instance
 ** \param [in]  u8Ch  channel of OCU
 ** \arg    MFT_OCU_CH0 ~ MFT_OCU_CH5
 ** \param [in]  pfnCallback  callback function
 **
 ** \retval Ok                    OCU interrupt enabled
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **                               - u8Ch out of range
 **                               - Other invalid configuration
 **
 *****************************************************************************
*/
en_result_t Mft_Ocu_EnableIrq(volatile stc_mftn_ocu_t* pstcOcu,
                              uint8_t u8Ch, func_ptr_t  pfnCallback)
{
    volatile stc_mft_ocu_ocsa10_field_t* pstcOCSA;
    // Check for NULL pointer
    if ( (NULL == pstcOcu)
         || (u8Ch >= MFT_OCU_MAXCH))
    {
        return ErrorInvalidParameter ;
    }

    //Get pointer of current channel OCU register address
    pstcOCSA = (volatile stc_mft_ocu_ocsa10_field_t*)((volatile uint8_t*)&pstcOcu->OCSA10 + ((u8Ch/2u) * 4u));

    //set register
    if(0u == (u8Ch%2)) //channel 0,2,4
    {
        pstcOCSA->IOE0 = 1u;
    }
    else //channel 1,3,5
    {
        pstcOCSA->IOE1 = 1u;
    }

    return Ok;
}

/*!
 ******************************************************************************
 ** \brief  Disable ocu interrupt
 **
 ** \param [in] pstcOcu Pointer to OCU instance
 ** \param [in]  u8Ch  channel of OCU
 ** \arg    MFT_OCU_CH0 ~ MFT_OCU_CH5
 **
 ** \retval Ok                    OCU interrupt disabled
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **                               - u8Ch out of range
 **                               - Other invalid configuration
 **
 *****************************************************************************
*/
en_result_t Mft_Ocu_DisableIrq(volatile stc_mftn_ocu_t* pstcOcu,uint8_t u8Ch)
{
    volatile stc_mft_ocu_ocsa10_field_t* pstcOCSA;

    // Check for NULL pointer
    if ( (NULL == pstcOcu)
         || (u8Ch >= MFT_OCU_MAXCH))
    {
        return ErrorInvalidParameter ;
    }

    //Get pointer of current channel OCU register address
    pstcOCSA = (volatile stc_mft_ocu_ocsa10_field_t*)((volatile uint8_t*)&pstcOcu->OCSA10 + (u8Ch/2u)*4u);

    //set register
    if(0u == (u8Ch%2u)) //channel 0,2,4
    {
        pstcOCSA->IOE0 = 0u;
    }
    else //channel 1,3,5
    {
        pstcOCSA->IOE1 = 0u;
    }
    
    return Ok;
}

#endif

/*!
 ******************************************************************************
 ** \brief  get ocu interrupt flag
 **
 ** \param [in] pstcOcu Pointer to OCU instance
 ** \param [in]  u8Ch  channel of OCU
 ** \arg    MFT_OCU_CH0 ~ MFT_OCU_CH5
 **
 ** \retval PdlClr  OCU interrupt flag is clear
 ** \retval PdlSet  OCU interrupt flag is set
 **
 *****************************************************************************
*/
en_irq_flag_t Mft_Ocu_GetIrqFlag(volatile stc_mftn_ocu_t* pstcOcu, uint8_t u8Ch)
{
    volatile stc_mft_ocu_ocsa10_field_t* pstcOCSA;
    en_irq_flag_t enFlag = PdlClr;

    //Get pointer of current channel OCU register address
    pstcOCSA = (volatile stc_mft_ocu_ocsa10_field_t*)((volatile uint8_t*)&pstcOcu->OCSA10 + (u8Ch/2u)*4u);

    //set return value
    if(0u == (u8Ch%2)) //channel 0,2,4
    {   
        (pstcOCSA->IOP0 == 1u) ? (enFlag = PdlSet) : (enFlag = PdlClr);
    }
    else //channel 1,3,5
    {
        (pstcOCSA->IOP1 == 1u) ? (enFlag = PdlSet) : (enFlag = PdlClr);
    }

    return enFlag;
}

/*!
 ******************************************************************************
 ** \brief  clear ocu interrupt flag
 **
 ** \param [in] pstcOcu Pointer to OCU instance
 ** \param [in]  u8Ch  channel of OCU
 ** \arg    MFT_OCU_CH0 ~ MFT_OCU_CH5
 **
 ** \retval Ok                    OCU interrupt flag is clear
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **                               - u8Ch out of range
 **                               - Other invalid configuration
 **
 *****************************************************************************
*/
en_result_t Mft_Ocu_ClrIrqFlag(volatile stc_mftn_ocu_t* pstcOcu, uint8_t u8Ch)
{
    volatile stc_mft_ocu_ocsa10_field_t* pstcOCSA;

    // Check for NULL pointer
    if ( (NULL == pstcOcu)
         || (u8Ch >= MFT_OCU_MAXCH))
    {
        return ErrorInvalidParameter ;
    }

    //Get pointer of current channel OCU register address
    pstcOCSA = (volatile stc_mft_ocu_ocsa10_field_t*)((volatile uint8_t*)&pstcOcu->OCSA10 + (u8Ch/2u)*4u);

    //clear register
    if(0u == (u8Ch%2u)) //channel 0,2,4
    {
        pstcOCSA->IOP0 = 0u;
    }
    else //channel 1,3,5
    {
        pstcOCSA->IOP1 = 0u;
    }

    return Ok;
}

/*!
 ******************************************************************************
 ** \brief  Get RT pin level of OCU
 **
 ** \param [in] pstcOcu Pointer to OCU instance
 ** \param [in]  u8Ch  channel of OCU
 ** \arg    MFT_OCU_CH0 ~ MFT_OCU_CH5
 **
 ** \retval RtLowlevel  RT pin is low level
 ** \retval RtHighlevel RT pin is high level
 **
 *****************************************************************************
*/
en_ocu_rt_out_state_t Mft_Ocu_GetRtPinLevel(volatile stc_mftn_ocu_t* pstcOcu, 
                                            uint8_t u8Ch)
{
    volatile stc_mft_ocu_ocsb10_field_t* pstcOCSB;
    en_ocu_rt_out_state_t enLevel = RtLowLevel;
    
    // Get the OCSB address
    pstcOCSB = (volatile stc_mft_ocu_ocsb10_field_t*)((volatile uint8_t*)&pstcOcu->OCSB10 + (u8Ch/2u)*4u);
    
    if(0u == (u8Ch%2))
    {
        (pstcOCSB->OTD0 == 1u) ? (enLevel = RtHighLevel) : (enLevel = RtLowLevel);
    }
    else
    {
        (pstcOCSB->OTD1 == 1u) ? (enLevel = RtHighLevel) : (enLevel = RtLowLevel);
    }
    
    return enLevel;
}

/*!
 ******************************************************************************
 ** \brief  writ occp register
 **
 ** \param [in] pstcOcu Pointer to OCU instance
 ** \param [in]  u8Ch  channel of OCU
 ** \arg    MFT_OCU_CH0 ~ MFT_OCU_CH5
 ** \param [in]  u16Occp  the value of occp
 ** \arg    16bit value
 **
 ** \retval Ok                    OCCP written
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **                               - pstcMft == NULL
 **                               - u8Ch out of range
 **                               - Other invalid configuration
 **
 *****************************************************************************
*/
en_result_t Mft_Ocu_WriteOccp(volatile stc_mftn_ocu_t* pstcOcu,
                              uint8_t u8Ch, uint16_t u16Occp)
{
	volatile uint16_t* p16OCCP;

    // Check for NULL pointer
    if ( (NULL == pstcOcu)
         || (u8Ch >= MFT_OCU_MAXCH))
    {
        return ErrorInvalidParameter ;
    }
    //Get pointer of current channel OCU register address
    p16OCCP = (volatile uint16_t*)((volatile uint8_t*)&pstcOcu->OCCP0 + u8Ch*4u);

    //set register
    *p16OCCP = u16Occp;

    return Ok;
}

/*!
 ******************************************************************************
 ** \brief  get occp register value
 **
 ** \param [in] pstcOcu Pointer to OCU instance
 ** \param [in]  u8Ch  channel of OCU
 ** \arg    MFT_OCU_CH0 ~ MFT_OCU_CH5
 **
 ** \retval 16bit occp value
 **
 *****************************************************************************
*/
uint16_t Mft_Ocu_ReadOccp(volatile stc_mftn_ocu_t* pstcOcu, uint8_t u8Ch)
{
    volatile uint16_t* p16OCCP;

    // Check for NULL pointer
    if ( (NULL == pstcOcu)
         || (u8Ch >= MFT_OCU_MAXCH))
    {
        return 0u ;
    }
    //Get pointer of current channel OCU register address
    p16OCCP = (volatile uint16_t*)((volatile uint8_t*)&pstcOcu->OCCP0 + u8Ch*4u);

    return (uint16_t)((*p16OCCP) & 0xFFFFu);
}

#endif // #if (defined(PDL_PERIPHERAL_$$X_ACTIVE))

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
