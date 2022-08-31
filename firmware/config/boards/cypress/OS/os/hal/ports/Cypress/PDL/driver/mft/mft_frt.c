/*******************************************************************************
* \file             mft_frt.c
* 
* \version          1.20
*
* \brief            This file provides the source code to the API for the 
*                   MFT_FRT driver.
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
#include "mft/mft_frt.h"

#if (defined(PDL_PERIPHERAL_MFT_FRT_ACTIVE)) 

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
stc_mft_frt_instance_data_t m_astcMftFrtInstanceDataLut[] =
{
#if (PDL_PERIPHERAL_ENABLE_MFT0_FRT == PDL_ON)
    {
        &MFT0_FRT,  // pstcInstance
        {0u, 0u, 0u, 0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFT1_FRT == PDL_ON)
    {
        &MFT1_FRT,  // pstcInstance
        {0u, 0u, 0u, 0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_MFT2_FRT == PDL_ON)
    {
        &MFT2_FRT,  // pstcInstance
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
 ** \brief Return the internal data for a certain FRT instance.
 **
 ** \param pstcFrt Pointer to FRT instance
 **
 ** \return Pointer to internal data or NULL if instance is not enabled
 **         (or not known)
 **
 *****************************************************************************/
static stc_mft_frt_intern_data_t* MftGetInternDataPtr(volatile stc_mftn_frt_t* pstcFrt)
{
    uint32_t u32Instance;

    for (u32Instance = 0u; u32Instance < FRT_INSTANCE_COUNT; u32Instance++)
    {
        if (pstcFrt == m_astcMftFrtInstanceDataLut[u32Instance].pstcInstance)
        {
            return &m_astcMftFrtInstanceDataLut[u32Instance].stcInternData;
        }
    }

    return NULL;
}

#if (PDL_INTERRUPT_ENABLE_MFT0_FRT == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_FRT == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_FRT == PDL_ON)
/*!
 ******************************************************************************
 ** \brief  FRT interrupt handler sub function
 **
 ** \param [in] pstcMft Pointer to FRT instance
 ** \arg    the pointer of FRT register structure
 **
 ** \param [in]  pstcMftFrtInternData  structure of frt callback function
 **
 ** \retval void
 **
 *****************************************************************************
*/
void Mft_Frt_IrqHandler( volatile stc_mftn_frt_t*pstcMft,
                         stc_mft_frt_intern_data_t* pstcMftFrtInternData)
{
    func_ptr_t pfnCallBack;
    uint32_t* pu32Temp;
    uint8_t u8Ch;
    volatile stc_mft_frt_tcsa0_field_t* pstcTCSA;

    /* Peak match Interrupt */
    for(u8Ch = 0u; u8Ch < 3u; u8Ch++)
    {
        //Get pointer of current channel frt register address
        pstcTCSA = (volatile stc_mft_frt_tcsa0_field_t*)((volatile uint8_t*)&pstcMft->TCSA0 +u8Ch*MFT_FRT_REG_OFFSET);
        
        if((PdlSet == Mft_Frt_GetIrqFlag(pstcMft, u8Ch, enFrtPeakMatchIrq)) && (1u == pstcTCSA->ICRE))
        {
            /* Clear Interrupt */
            Mft_Frt_ClrIrqFlag(pstcMft, u8Ch, enFrtPeakMatchIrq);

            //get peak match callback address of each channel
            pu32Temp = (uint32_t*)&(pstcMftFrtInternData->pfnFrt0PeakIrqCb);
            pfnCallBack = (func_ptr_t)(*(pu32Temp + 2u*u8Ch));
            
            if(pfnCallBack != NULL)
            {
               pfnCallBack();
            }
        }

        if((PdlSet == Mft_Frt_GetIrqFlag(pstcMft, u8Ch, enFrtZeroMatchIrq)) && (1u == pstcTCSA->IRQZE))
        {
            /* Clear Interrupt */
            Mft_Frt_ClrIrqFlag(pstcMft, u8Ch, enFrtZeroMatchIrq);

            //get Zero match callback address of each channel
            pu32Temp = (uint32_t*)&(pstcMftFrtInternData->pfnFrt0ZeroIrqCb);
            pfnCallBack = (func_ptr_t)(*(pu32Temp +  2u*u8Ch));
            
            if(pfnCallBack != NULL)
            {
               pfnCallBack();
            }
        }
    }
  
}

/**
 ******************************************************************************
 ** \brief Device dependent initialization of interrupts according CMSIS with
 **        level defined in l3.h
 **
 ** \param pstcMft Pointer to FRT instance
 **
 ** \return Ok    Successful initialization
 **
 ******************************************************************************/
static void MftFrtInitIrq( volatile stc_mftn_frt_t* pstcMft)
{
#if (PDL_MCU_CORE == PDL_FM0P_CORE)
    NVIC_ClearPendingIRQ(MFT0_FRT_IRQn);
    NVIC_EnableIRQ(MFT0_FRT_IRQn);
    NVIC_SetPriority(MFT0_FRT_IRQn, PDL_IRQ_LEVEL_MFT_FRT);
#elif (PDL_MCU_CORE == PDL_FM3_CORE)  
    NVIC_ClearPendingIRQ(MFT0_2_FRT_IRQn);
    NVIC_EnableIRQ(MFT0_2_FRT_IRQn);
    NVIC_SetPriority(MFT0_2_FRT_IRQn, PDL_IRQ_LEVEL_MFT_FRT);
#else
#if (PDL_INTERRUPT_ENABLE_MFT0_FRT == PDL_ON)    
    if(&MFT0_FRT == pstcMft)
    {
        NVIC_ClearPendingIRQ(MFT0_FRT_PEAK_IRQn);
        NVIC_EnableIRQ(MFT0_FRT_PEAK_IRQn);
        NVIC_SetPriority(MFT0_FRT_PEAK_IRQn, PDL_IRQ_LEVEL_MFT0_FRT);
        
        NVIC_ClearPendingIRQ(MFT0_FRT_ZERO_IRQn);
        NVIC_EnableIRQ(MFT0_FRT_ZERO_IRQn);
        NVIC_SetPriority(MFT0_FRT_ZERO_IRQn, PDL_IRQ_LEVEL_MFT0_FRT); 
    }
#endif
#if (PDL_INTERRUPT_ENABLE_MFT1_FRT == PDL_ON)       
    if(&MFT1_FRT == pstcMft)
    {
        NVIC_ClearPendingIRQ(MFT1_FRT_PEAK_IRQn);
        NVIC_EnableIRQ(MFT1_FRT_PEAK_IRQn);
        NVIC_SetPriority(MFT1_FRT_PEAK_IRQn, PDL_IRQ_LEVEL_MFT1_FRT);
        
        NVIC_ClearPendingIRQ(MFT1_FRT_ZERO_IRQn);
        NVIC_EnableIRQ(MFT1_FRT_ZERO_IRQn);
        NVIC_SetPriority(MFT1_FRT_ZERO_IRQn, PDL_IRQ_LEVEL_MFT1_FRT); 
    }
#endif
#if (PDL_INTERRUPT_ENABLE_MFT2_FRT == PDL_ON)      
    if(&MFT2_FRT == pstcMft)
    {
        NVIC_ClearPendingIRQ(MFT2_FRT_PEAK_IRQn);
        NVIC_EnableIRQ(MFT2_FRT_PEAK_IRQn);
        NVIC_SetPriority(MFT2_FRT_PEAK_IRQn, PDL_IRQ_LEVEL_MFT2_FRT);
        
        NVIC_ClearPendingIRQ(MFT2_FRT_ZERO_IRQn);
        NVIC_EnableIRQ(MFT2_FRT_ZERO_IRQn);
        NVIC_SetPriority(MFT2_FRT_ZERO_IRQn, PDL_IRQ_LEVEL_MFT2_FRT); 
    }
#endif 
#endif    
    
    return;
}


/**
 ******************************************************************************
 ** \brief Device dependent initialization of interrupts according CMSIS with
 **        level defined in l3.h
 **
 ** \param pstcMft  Pointer to FRT instance
 ** \arg    the pointer of FRT register structure
 **
 ** \return Ok    Successful initialization
 **
 ******************************************************************************/
static void MftFrtDeInitIrq( volatile stc_mftn_frt_t* pstcMft)
{
#if (PDL_MCU_CORE == PDL_FM0P_CORE) 
    NVIC_ClearPendingIRQ(MFT0_FRT_IRQn);
    NVIC_DisableIRQ(MFT0_FRT_IRQn);
    NVIC_SetPriority(MFT0_FRT_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);   
#elif (PDL_MCU_CORE == PDL_FM3_CORE)    
    NVIC_ClearPendingIRQ(MFT0_2_FRT_IRQn);
    NVIC_DisableIRQ(MFT0_2_FRT_IRQn);
    NVIC_SetPriority(MFT0_2_FRT_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);   
#else
#if (PDL_INTERRUPT_ENABLE_MFT0_FRT == PDL_ON)      
    if(&MFT0_FRT == pstcMft)
    {
        NVIC_ClearPendingIRQ(MFT0_FRT_PEAK_IRQn);
        NVIC_DisableIRQ(MFT0_FRT_PEAK_IRQn);
        NVIC_SetPriority(MFT0_FRT_PEAK_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);  
        
        NVIC_ClearPendingIRQ(MFT0_FRT_ZERO_IRQn);
        NVIC_DisableIRQ(MFT0_FRT_ZERO_IRQn);
        NVIC_SetPriority(MFT0_FRT_ZERO_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);  
    }
#endif
#if (PDL_INTERRUPT_ENABLE_MFT1_FRT == PDL_ON)       
    if(&MFT1_FRT == pstcMft)
    {
        NVIC_ClearPendingIRQ(MFT1_FRT_PEAK_IRQn);
        NVIC_DisableIRQ(MFT1_FRT_PEAK_IRQn);
        NVIC_SetPriority(MFT1_FRT_PEAK_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);  
        
        NVIC_ClearPendingIRQ(MFT1_FRT_ZERO_IRQn);
        NVIC_DisableIRQ(MFT1_FRT_ZERO_IRQn);
        NVIC_SetPriority(MFT1_FRT_ZERO_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);  
    }
#endif
#if (PDL_INTERRUPT_ENABLE_MFT2_FRT == PDL_ON)       
    if(&MFT2_FRT == pstcMft)
    {
        NVIC_ClearPendingIRQ(MFT2_FRT_PEAK_IRQn);
        NVIC_DisableIRQ(MFT2_FRT_PEAK_IRQn);
        NVIC_SetPriority(MFT2_FRT_PEAK_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);  
        
        NVIC_ClearPendingIRQ(MFT2_FRT_ZERO_IRQn);
        NVIC_DisableIRQ(MFT2_FRT_ZERO_IRQn);
        NVIC_SetPriority(MFT2_FRT_ZERO_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);  
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
 ** \brief  Init FRT Channel
 **
 ** \param [in] pstcMft Pointer to FRT instance
 ** \arg    the pointer of FRT register structure
 ** \param [in]  u8Ch  channel of Free run timer
 ** \arg    MFT_FRT_CH0 ~ MFT_FRT_CH2
 ** \param [in] pstcFrtConfig configure of FRT
 ** \arg    structure of FRT cofnigure
 **
 ** \retval Ok                    Internal data has been setup
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcMft == NULL
 **                               - pstcFrtConfig == NULL
 **                               - Other invalid configuration
 **
 ******************************************************************************/
en_result_t Mft_Frt_Init(volatile stc_mftn_frt_t *pstcMft, 
                         uint8_t u8Ch,
                         const stc_mft_frt_config_t*  pstcFrtConfig)
{
    // Pointer to internal data
    stc_mft_frt_intern_data_t* pstcFrtInternData ;
    volatile stc_mft_frt_tcsa0_field_t* pstcTCSA;
#if (PDL_MCU_CORE == PDL_FM3_CORE)    
    volatile stc_mft_frt_tcsb0_field_t* pstcTCSB;
#endif    
    uint8_t u8ClkDiv;

    // Check for NULL pointer and configuration parameter
    if ((NULL == pstcMft) || (u8Ch >=  MFT_FRT_MAX_CH))
    {
        return ErrorInvalidParameter ;
    }

    // Get pointer to internal data structure ...
    pstcFrtInternData = MftGetInternDataPtr( pstcMft ) ;
    // ... and check for NULL
    if ( NULL == pstcFrtInternData )
    {
        return ErrorInvalidParameter ;
    }

    //Get pointer of current channel frt register address
    pstcTCSA = (volatile stc_mft_frt_tcsa0_field_t*)((volatile uint8_t*)&pstcMft->TCSA0 + u8Ch*MFT_FRT_REG_OFFSET);
#if (PDL_MCU_CORE == PDL_FM3_CORE) 
    pstcTCSB = (volatile stc_mft_frt_tcsb0_field_t*)((volatile uint8_t*)&pstcMft->TCSB0 + u8Ch*MFT_FRT_REG_OFFSET);
#endif    
    
    //set count clock cycle of FRT counter
    u8ClkDiv = (uint8_t)pstcFrtConfig->enFrtClockDiv;
    switch (pstcFrtConfig->enFrtClockDiv)
    {
        case FrtPclkDiv1:
        case FrtPclkDiv2:
        case FrtPclkDiv4:
        case FrtPclkDiv8:
        case FrtPclkDiv16:
        case FrtPclkDiv32:
        case FrtPclkDiv64:
        case FrtPclkDiv128:
        case FrtPclkDiv256:
    #if (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)        
        case FrtPclkDiv512:
        case FrtPclkDiv1024:
    #endif 
            pstcTCSA->CLK = (u8ClkDiv & 0x0Fu);
            break;
        default:
            return ErrorInvalidParameter ;
    }
    
    //set frt mode
    switch (pstcFrtConfig->enFrtMode)
    {
        case FrtUpCount:
            pstcTCSA->MODE = 0u;
            break;
        case FrtUpDownCount:
            pstcTCSA->MODE = 1u;
            break; 
        default:
            return ErrorInvalidParameter ;            
    }

    //set buffer enable bit
    pstcTCSA->BFE = (pstcFrtConfig->bEnBuffer == TRUE) ? 1u : 0u;
    
    //set external clock enable bit
    pstcTCSA->ECKE = (pstcFrtConfig->bEnExtClock == TRUE) ? 1u : 0u;

#if (PDL_MCU_CORE == PDL_FM3_CORE)     
    // Set ADC trigger
    pstcTCSB->AD0E = ((pstcFrtConfig->bTriggerAdc0 == TRUE) ? 1u: 0u);
    pstcTCSB->AD1E = ((pstcFrtConfig->bTriggerAdc1 == TRUE) ? 1u: 0u);
    pstcTCSB->AD2E = ((pstcFrtConfig->bTriggerAdc2 == TRUE) ? 1u: 0u);
#endif
    
#if (PDL_INTERRUPT_ENABLE_MFT0_FRT == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_FRT == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_FRT == PDL_ON)    
    // Set interrupt enable
    if (NULL != pstcFrtConfig->pstcIrqEn)
    {   
        if(pstcFrtConfig->pstcIrqEn->bFrtZeroMatchIrq == TRUE)
        {
            pstcTCSA->IRQZE = 1u;
        }
        
        if(pstcFrtConfig->pstcIrqEn->bFrtPeakMatchIrq == TRUE)
        {
            pstcTCSA->ICRE = 1u;
        }
    }
    
    // Set interrupt callback function
    if (NULL != pstcFrtConfig->pstcIrqCb)
    {
        switch (u8Ch)
        {
            case 0u:
                pstcFrtInternData->pfnFrt0PeakIrqCb = pstcFrtConfig->pstcIrqCb->pfnFrtPeakIrqCb;
                pstcFrtInternData->pfnFrt0ZeroIrqCb = pstcFrtConfig->pstcIrqCb->pfnFrtZeroIrqCb;
                break;
            case 1u:
                pstcFrtInternData->pfnFrt1PeakIrqCb = pstcFrtConfig->pstcIrqCb->pfnFrtPeakIrqCb;
                pstcFrtInternData->pfnFrt1ZeroIrqCb = pstcFrtConfig->pstcIrqCb->pfnFrtZeroIrqCb;
                break;
            case 2u:  
                pstcFrtInternData->pfnFrt2PeakIrqCb = pstcFrtConfig->pstcIrqCb->pfnFrtPeakIrqCb;
                pstcFrtInternData->pfnFrt2ZeroIrqCb = pstcFrtConfig->pstcIrqCb->pfnFrtZeroIrqCb;
                break;
            default:
                break;   
        }
    }
    
    if(pstcFrtConfig->bTouchNvic == TRUE)
    {
        MftFrtInitIrq(pstcMft);
    }
#endif
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  Init FRT Channel
 **
 ** Clear a FRT channel.
 **
 ** \param [in] pstcMft Pointer to FRT instance
 ** \arg    the pointer of FRT register structure
 ** \param [in] u8Ch  channel of Free run timer
 ** \param [in] bTouchNvic Touch NVIC or not
 **
 ** \retval Ok                    Register been cleared
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcMft == NULL
 **                               - Other invalid configuration
 **
 ******************************************************************************/
en_result_t Mft_Frt_DeInit(volatile stc_mftn_frt_t*pstcMft, 
                           uint8_t u8Ch,
                           boolean_t bTouchNvic)
{
    volatile uint16_t* pu16TCSA;
#if (PDL_MCU_CORE == PDL_FM3_CORE)      
    volatile uint16_t* pu16TCSB;
#endif
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)       
    volatile uint16_t* pu16TCSC;
#endif
    
    volatile uint16_t* pu16TCCP;
    
    // Check for NULL pointer and configuration parameter
    if ((NULL == pstcMft) || (u8Ch >= MFT_FRT_MAX_CH))
    {
        return ErrorInvalidParameter ;
    }
    
    //Get pointer of current channel frt register address
    pu16TCSA = (volatile uint16_t*)((volatile uint8_t*)&pstcMft->TCSA0 + u8Ch*MFT_FRT_REG_OFFSET);
    *pu16TCSA = 0x0040u;
#if (PDL_MCU_CORE == PDL_FM3_CORE)   
    pu16TCSB = (volatile uint16_t*)((volatile uint8_t*)&pstcMft->TCSB0 + u8Ch*MFT_FRT_REG_OFFSET);
    *pu16TCSB = 0x0000u;
#endif
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)   
    pu16TCSC = (volatile uint16_t*)((volatile uint8_t*)&pstcMft->TCSC0 + u8Ch*MFT_FRT_REG_OFFSET);
    *pu16TCSC = 0x0000u;
#endif
   
    pu16TCCP = (volatile uint16_t*)((volatile uint8_t*)&pstcMft->TCCP0 + u8Ch*MFT_FRT_REG_OFFSET);
    *pu16TCCP = 0x0000u;

#if (PDL_INTERRUPT_ENABLE_MFT0_FRT == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_FRT == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_FRT == PDL_ON) 
    if(TRUE == bTouchNvic)
    {
        MftFrtDeInitIrq(pstcMft);
    }
#endif
    
    return Ok;
}


/*!
 ******************************************************************************
 ** \brief  Start FRT
 **
 ** \param [in] pstcMft Pointer to FRT instance
 ** \arg    the pointer of FRT register structure
 ** \param [in]  u8Ch  channel of Free run timer
 ** \arg    MFT_FRT_CH0 ~ MFT_FRT_CH2
 **
 ** \retval Ok                    Internal data has been setup
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcMft == NULL
 **                               - u8Ch out of range
 **                               - Other invalid configuration
 **
 *****************************************************************************
*/
en_result_t Mft_Frt_Start(volatile stc_mftn_frt_t*pstcMft, uint8_t u8Ch)
{
    volatile stc_mft_frt_tcsa0_field_t* pstcTCSA;

    // Check for NULL pointer
    if ( (NULL == pstcMft)
         || (u8Ch >= MFT_FRT_MAX_CH))
    {
        return ErrorInvalidParameter ;
    }

    //Get pointer of current channel frt register address
    pstcTCSA = (volatile stc_mft_frt_tcsa0_field_t*)((volatile uint8_t*)&pstcMft->TCSA0 + u8Ch*MFT_FRT_REG_OFFSET);
    pstcTCSA->STOP = 0u;
    pstcTCSA->SCLR = 0u;
    
    return Ok;
}

/*!
 ******************************************************************************
 ** \brief  set frt stop
 **
 ** \param [in] pstcMft Pointer to FRT instance
 ** \arg    the pointer of FRT register structure
 ** \param [in]  u8Ch  channel of Free run timer
 ** \arg    MFT_FRT_CH0 ~ MFT_FRT_CH2
 **
 ** \retval Ok                    Internal data has been setup
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcMft == NULL
 **                               - u8Ch out of range
 **                               - Other invalid configuration
 **
 *****************************************************************************
*/
en_result_t Mft_Frt_Stop(volatile stc_mftn_frt_t*pstcMft, uint8_t u8Ch)
{
    volatile stc_mft_frt_tcsa0_field_t* pstcTCSA;

    // Check for NULL pointer
    if ( (NULL == pstcMft)
         || (u8Ch >= MFT_FRT_MAX_CH))
    {
        return ErrorInvalidParameter ;
    }

    //Get pointer of current channel frt register address
    pstcTCSA = (volatile stc_mft_frt_tcsa0_field_t*)((volatile uint8_t*)&pstcMft->TCSA0 + u8Ch*MFT_FRT_REG_OFFSET);

    pstcTCSA->STOP = 1u;
    pstcTCSA->SCLR = 1u;
    return Ok;
}

#if (PDL_INTERRUPT_ENABLE_MFT0_FRT == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_FRT == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_FRT == PDL_ON)
/*!
 ******************************************************************************
 ** \brief  enable frt interrupt
 **
 ** \param [in] pstcMft Pointer to FRT instance
 ** \arg    the pointer of FRT register structure
 **
 ** \param [in]  u8Ch  channel of Free run timer
 ** \arg    MFT_FRT_CH0 ~ MFT_FRT_CH2
 **
 ** \param [in]  pstcIrqSel  Pointer to FRT interrupt selection structure
 **
 ** \retval Ok                    Internal data has been setup
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcMft == NULL
 **                               - u8Ch out of range
 **                               - Other invalid configuration
 **
 *****************************************************************************
*/
en_result_t Mft_Frt_EnableIrq(volatile stc_mftn_frt_t*pstcMft,
                              uint8_t u8Ch,
                              stc_frt_irq_sel_t* pstcIrqSel)
{
     volatile stc_mft_frt_tcsa0_field_t* pstcTCSA;

    // Check for NULL pointer
    if ( (NULL == pstcMft)
         || (u8Ch >= MFT_FRT_MAX_CH) )
    {
        return ErrorInvalidParameter ;
    }

    //Get pointer of current channel frt register address
    pstcTCSA = (volatile stc_mft_frt_tcsa0_field_t*)((volatile uint8_t*)&pstcMft->TCSA0 +u8Ch*MFT_FRT_REG_OFFSET);

    if(1u == pstcIrqSel->bFrtZeroMatchIrq) //zero detect interrupt
    {
        pstcTCSA->IRQZE = 1u;
    }
    if(1u == pstcIrqSel->bFrtPeakMatchIrq) //peak detect interrupt
    {
        pstcTCSA->ICRE = 1u;
    }
    
    return Ok;
}

/*!
 ******************************************************************************
 ** \brief  disable frt interrupt
 **
 ** \param [in] pstcMft Pointer to FRT instance
 ** \arg    the pointer of FRT register structure
 **
 ** \param [in]  u8Ch  channel of Free run timer
 ** \arg    MFT_FRT_CH0 ~ MFT_FRT_CH2
 **
 ** \param [in]  pstcIrqSel  Pointer to FRT interrupt selection structure
 **
 ** \retval Ok                    Internal data has been setup
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcMft == NULL
 **                               - u8Ch out of range
 **                               - Other invalid configuration
 **
 *****************************************************************************
*/
en_result_t Mft_Frt_DisableIrq(volatile stc_mftn_frt_t*pstcMft,uint8_t u8Ch,
                               stc_frt_irq_sel_t* pstcIrqSel)
{
    volatile stc_mft_frt_tcsa0_field_t* pstcTCSA;

    // Check for NULL pointer
    if ( (NULL == pstcMft)
         || (u8Ch >= MFT_FRT_MAX_CH))
    {
        return ErrorInvalidParameter ;
    }

    //Get pointer of current channel frt register address
    pstcTCSA = (volatile stc_mft_frt_tcsa0_field_t*)((volatile uint8_t*)&pstcMft->TCSA0 + u8Ch*MFT_FRT_REG_OFFSET);

    if(1u == pstcIrqSel->bFrtZeroMatchIrq) //zero detect interrupt
    {
        pstcTCSA->IRQZE = 0u;
    }
    if(1u == pstcIrqSel->bFrtPeakMatchIrq) //peak detect interrupt
    {
        pstcTCSA->ICRE = 0u;
    }
    
    
    return Ok;
}

#endif

/*!
 ******************************************************************************
 ** \brief  get frt interrupt flag
 **
 ** \param [in] pstcMft Pointer to FRT instance
 ** \arg    the pointer of FRT register structure
   
 ** \param [in]  u8Ch  channel of Free run timer
 ** \arg    MFT_FRT_CH0 ~ MFT_FRT_CH2
   
 ** \param [in]  enIrqType  Interrupt type  
 ** \arg   enFrtZeroMatchIrq  Zero match interrupt of FRT
 ** \arg   enFrtPeakMatchIrq  Peak match interrupt of FRT   
 **
 ** \retval Ok                    Internal data has been setup
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcMft == NULL
 **                               - u8Ch out of range
 **                               - Other invalid configuration
 **
 *****************************************************************************
*/
en_irq_flag_t Mft_Frt_GetIrqFlag(volatile stc_mftn_frt_t*pstcMft,
                                 uint8_t u8Ch,
                                 en_mft_frt_irq_t enIrqType)
{
    volatile stc_mft_frt_tcsa0_field_t* pstcTCSA;
    en_irq_flag_t retval;

    //Get pointer of current channel frt register address
    pstcTCSA = (volatile stc_mft_frt_tcsa0_field_t*)((volatile uint8_t*)&pstcMft->TCSA0 + u8Ch*MFT_FRT_REG_OFFSET);

    if(enFrtZeroMatchIrq == enIrqType) //zero detect interrupt
    {
        (pstcTCSA->IRQZF == 1u) ? (retval = PdlSet) : (retval = PdlClr);
    }
    else //peak detect interrupt
    {
        (pstcTCSA->ICLR == 1u) ? (retval = PdlSet) : (retval = PdlClr);
    }

    return retval;
}

/*!
 ******************************************************************************
 ** \brief  clear frt interrupt falg
 **
 ** \param [in] pstcMft MFT register definition
 ** \arg    the pointer of FRT register structure
 **
 ** \param [in]  u8Ch  channel of Free run timer
 ** \arg    MFT_FRT_CH0 ~ MFT_FRT_CH2
 **
 ** \param [in]  enIrqType  the type of interrupt
 ** \arg   enFrtZeroMatchIrq  Zero match interrupt of FRT
 ** \arg   enFrtPeakMatchIrq  Peak match interrupt of FRT
 **
 ** \retval Ok                    Internal data has been setup
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcMft == NULL
 **                               - u8Ch out of range
 **                               - Other invalid configuration
 **
 *****************************************************************************
*/
en_result_t Mft_Frt_ClrIrqFlag(volatile stc_mftn_frt_t*pstcMft,
                               uint8_t u8Ch,
                               en_mft_frt_irq_t enIrqType)
{
    volatile stc_mft_frt_tcsa0_field_t* pstcTCSA;

    // Check for NULL pointer
    if ( (NULL == pstcMft)
         || (u8Ch >= MFT_FRT_MAX_CH))
    {
        return ErrorInvalidParameter ;
    }

    //Get pointer of current channel frt register address
    pstcTCSA = (volatile stc_mft_frt_tcsa0_field_t*)((volatile uint8_t*)&pstcMft->TCSA0 + u8Ch*MFT_FRT_REG_OFFSET);

    if(enFrtZeroMatchIrq == enIrqType) //zero detect interrupt
    {
        pstcTCSA->IRQZF = 0u;
    }
    else //peak detect interrupt
    {
        pstcTCSA->ICLR = 0u;
    }

    return Ok;
}

/*!
 ******************************************************************************
 ** \brief  set frt cycle value
 **
 ** \param [in] pstcMft Pointer to FRT instance
 ** \arg    the pointer of FRT register structure
 **
 ** \param [in]  u8Ch  channel of Free run timer
 ** \arg    MFT_FRT_CH0 ~ MFT_FRT_CH2
 **
 ** \param [in]  u16Cycle  the cycle value
 ** \arg     number of 16bit
 **
 ** \retval Ok                    Internal data has been setup
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcMft == NULL
 **                               - u8Ch out of range
 **                               - Other invalid configuration
 **
 *****************************************************************************
*/
en_result_t Mft_Frt_SetCountCycle(volatile stc_mftn_frt_t*pstcMft,
                                  uint8_t u8Ch, 
                                  uint16_t u16Cycle)
{
    volatile uint16_t* pu16TCCP;

    // Check for NULL pointer
    if ( (pstcMft == NULL)
         || (u8Ch >= MFT_FRT_MAX_CH))
    {
        return ErrorInvalidParameter ;
    }

    //Get pointer of current channel frt register address
    pu16TCCP = (volatile uint16_t*)( (volatile uint8_t*)&pstcMft->TCCP0 + u8Ch*MFT_FRT_REG_OFFSET);

    *pu16TCCP = u16Cycle;

    return Ok;
}

/*!
 ******************************************************************************
 ** \brief  set frt count value
 **
 ** \param [in] pstcMft Pointer to FRT instance
 ** \arg    the pointer of FRT register structure
 **
 ** \param [in]  u8Ch  channel of Free run timer
 ** \arg    MFT_FRT_CH0 ~ MFT_FRT_CH2
 **
 ** \param [in]  u16Count  the count value
 ** \arg     number of 16bit
 **
 ** \retval Ok                    Internal data has been setup
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcMft == NULL
 **                               - Other invalid configuration
 **
 *****************************************************************************
*/
en_result_t Mft_Frt_SetCountVal(volatile stc_mftn_frt_t*pstcMft,
                                uint8_t u8Ch,
                                uint16_t u16Count)
{
    volatile uint16_t* pu16TCDT;

    // Check for NULL pointer
    if ( (NULL == pstcMft)
         || (u8Ch >= MFT_FRT_MAX_CH))
    {
        return ErrorInvalidParameter ;
    }

    //Get pointer of current channel frt register address
    pu16TCDT = (volatile uint16_t*)( (volatile uint8_t*)&pstcMft->TCDT0 + u8Ch*MFT_FRT_REG_OFFSET);

    *pu16TCDT = u16Count;

    return Ok;
}

/*!
 ******************************************************************************
 ** \brief  get frt current count
 **
 ** \param [in] pstcMft Pointer to FRT instance
 ** \arg    the pointer of FRT register structure
 **
 ** \param [in]  u8Ch  channel of Free run timer
 ** \arg    MFT_FRT_CH0 ~ MFT_FRT_CH2
 **
 ** \retval current count of frt
 **
 *****************************************************************************
*/
uint16_t Mft_Frt_GetCurCount(volatile stc_mftn_frt_t*pstcMft, uint8_t u8Ch)
{
    uint16_t retTCDT;
    volatile  uint16_t* pstcTCDT;

    // Check for NULL pointer
    if ( (NULL == pstcMft)
         || (u8Ch >= MFT_FRT_MAX_CH))
    {
        return ErrorInvalidParameter ;
    }

    //Get pointer of current channel frt register address
    pstcTCDT = (volatile uint16_t*)( (volatile uint8_t*)&pstcMft->TCDT0 + u8Ch*MFT_FRT_REG_OFFSET);

    retTCDT = *pstcTCDT;

    return retTCDT;
}

#if (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)    
/**
 ******************************************************************************
 ** \brief  set mask zero times
 **
 ** \param [in] pstcMft Pointer to FRT instance
 ** \arg    the pointer of FRT register structure
 ** \param [in]  u8Ch  channel of Free run timer 
 ** \arg    MFT_FRT_CH0 ~ MFT_FRT_CH2
 ** \param [in] u8Times mask times
 ** \arg    value of 0~15
 **
 ** \retval Ok                    Internal data has been setup
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcMft == NULL
 **                               - u8Ch out of range
 **                               - Other invalid configuration
 **
 ******************************************************************************/
en_result_t Mft_Frt_SetMaskZeroTimes(volatile stc_mftn_frt_t*pstcMft,
                                     uint8_t u8Ch, 
                                     uint8_t u8Times)
{
      volatile stc_mft_frt_tcsc0_field_t* pstcTCSC;

      // Check for NULL pointer
      if ( (NULL == pstcMft) || 
           (u8Ch >= MFT_FRT_MAX_CH) || 
           (u8Times > 15u) )
      {
          return ErrorInvalidParameter ;
      }

      //Get pointer of current channel frt register address
      pstcTCSC=(volatile stc_mft_frt_tcsc0_field_t*)((volatile uint8_t*)&pstcMft->TCSC0 + u8Ch*MFT_FRT_REG_OFFSET);
      
      pstcTCSC->MSZI = u8Times;
      
      return Ok;
}

/**
 ******************************************************************************
 ** \brief  Get current mask zero  times
 **
 ** \param [in] pstcMft Pointer to FRT instance
 ** \arg    the pointer of FRT register structure
 **
 ** \param [in]  u8Ch  channel of Free run timer 
 ** \arg    MFT_FRT_CH0 ~ MFT_FRT_CH2
 **
 ** \retval Ok                    Internal data has been setup
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcMft == NULL
 **                               - u8Ch out of range
 **                               - Other invalid configuration
 **
 ******************************************************************************/
uint8_t Mft_Frt_GetCurMaskZeroTimes(volatile stc_mftn_frt_t*pstcMft, uint8_t u8Ch)
{
    volatile stc_mft_frt_tcsc0_field_t* pu16TCSC;

    // Check for NULL pointer
    if ( (NULL == pstcMft) 
         || (u8Ch >= MFT_FRT_MAX_CH))
    {
        return 0xFFu ;
    }
		
    //Get pointer of current channel frt register address
    pu16TCSC = (volatile stc_mft_frt_tcsc0_field_t*)((volatile uint8_t*)&pstcMft->TCSC0 + u8Ch*MFT_FRT_REG_OFFSET);

    return pu16TCSC->MSZC;
}
/**
 ******************************************************************************
 ** \brief  set mask peak  times
 **
 ** \param [in] pstcMft Pointer to FRT instance
 ** \arg    the pointer of FRT register structure
 ** \param [in]  u8Ch  channel of Free run timer 
 ** \arg    MFT_FRT_CH0 ~ MFT_FRT_CH2
 ** \param [in] u8Times mask times
 ** \arg    value of 0~15
 **
 ** \retval Ok                    Internal data has been setup
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcMft == NULL
 **                               - u8Ch out of range
 **                               - Other invalid configuration
 **
 ******************************************************************************/
en_result_t Mft_Frt_SetMaskPeakTimes(volatile stc_mftn_frt_t*pstcMft,
                                     uint8_t u8Ch, uint8_t u8Times)
{
    volatile stc_mft_frt_tcsc0_field_t* pstcTCSC;

    // Check for NULL pointer
      if ( (NULL == pstcMft) || 
           (u8Ch >= MFT_FRT_MAX_CH) || 
           (u8Times > 15u) )
      {
          return ErrorInvalidParameter ;
      }
		
    //Get pointer of current channel frt register address
    pstcTCSC = (volatile stc_mft_frt_tcsc0_field_t*)((volatile uint8_t*)&pstcMft->TCSC0 + u8Ch*MFT_FRT_REG_OFFSET);

    pstcTCSC->MSPI = u8Times;;
      
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  get mask peak  times
 **
 ** \param [in] pstcMft Pointer to FRT instance
 ** \arg    the pointer of FRT register structure
 **
 ** \param [in]  u8Ch  channel of Free run timer 
 ** \arg    MFT_FRT_CH0 ~ MFT_FRT_CH2
 **
 ** \retval Current mask peak times
 **
 ******************************************************************************/
uint8_t Mft_Frt_GetCurMaskPeakTimes(volatile stc_mftn_frt_t*pstcMft, uint8_t u8Ch)
{
    volatile stc_mft_frt_tcsc0_field_t* pstcTCSC;

    // Check for NULL pointer
    if ( (NULL == pstcMft) 
         || (u8Ch >= MFT_FRT_MAX_CH))
    {
        return 0xFFu ;
    }
		
    //Get pointer of current channel frt register address
    pstcTCSC = (volatile stc_mft_frt_tcsc0_field_t*)((volatile uint8_t*)&pstcMft->TCSC0 + u8Ch*MFT_FRT_REG_OFFSET);

    return pstcTCSC->MSPC;
}

#endif

/**
 ******************************************************************************
 ** \brief  Configure FRT channel to offest mode or normall mode
 **
 ** The FRT channel 1 or 2, set to offset mode, will be working with FRT channel
 ** 0 with offset mode. The channel with offset mode will follow the behavior 
 ** FRT channel 0, thus it is no need to call Mft_Frt_Init() to initialize this
 ** channel. See peripheral manual about offset mode for the details.
 **
 ** \param [in] pstcMft Pointer to FRT instance
 ** \arg    the pointer of FRT register structure
 ** \param [in] u8Ch    Channel of Free run timer 
 ** \arg    FrtOffsetCh1 FRT channel 1 which can be set to offset mode
 ** \arg    FrtOffsetCh2 FRT channel 2 which can be set to offset mode
 ** \param [in] bOffsetMode 
 ** \arg    TRUE   Set the FRT channel to Offset mode
 ** \arg    FALSE  Set the FRT channel to normal mode
 **
 ** \retval Ok                    Mode has been set normally
 ** \retval ErrorInvalidParameter If oen of following cases match:
 **                               - pstcMft == NULL
 **                               - enCh out of range
 **
 ** \note Only TYPE3 of FM4 or later products has this function.
 **
 ******************************************************************************/
#if (PDL_MCU_TYPE >= PDL_FM4_TYPE3)
en_result_t  Mft_Frt_SetOffsetMode(volatile stc_mftn_frt_t*pstcMft, 
                                   uint8_t u8Ch, 
                                   boolean_t bOffsetMode)
{
    volatile stc_mft_frt_tcsd_field_t* pstcTCSD;

    if(NULL == pstcMft)
    {
        return ErrorInvalidParameter ;
    }
    
    //Get pointer of current channel frt register address
    pstcTCSD = (volatile stc_mft_frt_tcsd_field_t*)((volatile uint8_t*)&pstcMft->TCSD);
    
    switch (u8Ch)
    {
        case 1u:
            pstcTCSD->OFMD1 = (bOffsetMode == TRUE) ? 1u : 0u;
            break;
        case 2u:
            pstcTCSD->OFMD2 = (bOffsetMode == TRUE) ? 1u : 0u;
            break;
        default:
            return ErrorInvalidParameter ;
    }
    
    return Ok;
}
#endif

#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)
/**
 ******************************************************************************
 ** \brief Set the Simultaneous Start register of MFT
 **
 ** \param [in]  u16Stop           Bit0 ----- MFT unit.0 Ch.0
 **                                Bit1 ----- MFT unit.0 Ch.1
 **                                Bit2 ----- MFT unit.0 Ch.2
 **                                Bit3 ----- MFT unit.1 Ch.0
 **                                Bit4 ----- MFT unit.1 Ch.1
 **                                Bit5 ----- MFT unit.1 Ch.2
 **                                Bit6 ----- MFT unit.2 Ch.0
 **                                Bit7 ----- MFT unit.2 Ch.1
 **                                Bit8 ----- MFT unit.2 Ch.2
 ** \arg  0    Start FRT
 ** \arg  1    Stop FRT
 **
 ** \param [in]  u16Clr            Bit0 ----- MFT unit.0 Ch.0
 **                                Bit1 ----- MFT unit.0 Ch.1
 **                                Bit2 ----- MFT unit.0 Ch.2
 **                                Bit3 ----- MFT unit.1 Ch.0
 **                                Bit4 ----- MFT unit.1 Ch.1
 **                                Bit5 ----- MFT unit.1 Ch.2
 **                                Bit6 ----- MFT unit.2 Ch.0
 **                                Bit7 ----- MFT unit.2 Ch.1
 **                                Bit8 ----- MFT unit.2 Ch.2
 ** \arg  0    Do nothing
 ** \arg  1    Issues FRT operation state initialization request. 
 **
 ** \retval Ok  Simultaneous start register is set 
 ******************************************************************************/
en_result_t Mft_Frt_SetSimultaneousStart(uint16_t u16Stop, uint16_t u16Clr)
{
    volatile stc_mftn_frt_t* pstcFrt = &MFT0_FRT;
    
    pstcFrt->TCAL = (u16Stop | (u16Clr << 16u));
    
    return Ok;
}
#endif

#endif // #if (defined(PDL_PERIPHERAL_$$X_ACTIVE))

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
