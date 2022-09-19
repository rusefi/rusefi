/******************************************************************************
* \file             bt.c
*
* \version          1.20
*
* \brief            Base Timer (BT) driver  
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
#include "bt/bt.h"

#if (defined(PDL_PERIPHERAL_BT_ACTIVE))
/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/
/// Look-up table for all enabled BT instances and their internal data
stc_bt_instance_data_t m_astcBtInstanceDataLut[BT_INSTANCE_COUNT] =
{
#if (PDL_PERIPHERAL_ENABLE_BT0 == PDL_ON)
    { 
        &BT0,                 // pstcInstance
        {0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_BT1 == PDL_ON)
    { 
        &BT1,                 // pstcInstance
        {0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_BT2 == PDL_ON)
    { 
        &BT2,                 // pstcInstance
        {0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_BT3 == PDL_ON)
    { 
        &BT3,                 // pstcInstance
        {0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_BT4 == PDL_ON)
    { 
        &BT4,                 // pstcInstance
        {0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_BT5 == PDL_ON)
    { 
        &BT5,                 // pstcInstance
        {0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_BT6 == PDL_ON)
    { 
        &BT6,                 // pstcInstance
        {0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_BT7 == PDL_ON)
    { 
        &BT7,                 // pstcInstance
        {0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_BT8 == PDL_ON)
    { 
        &BT8,                 // pstcInstance
        {0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_BT9 == PDL_ON)
    { 
        &BT9,                 // pstcInstance
        {0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_BT10 == PDL_ON)
    { 
        &BT10,                // pstcInstance
        {0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_BT11 == PDL_ON)
    { 
        &BT11,                // pstcInstance
        {0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_BT12 == PDL_ON)
    { 
        &BT12,                // pstcInstance
        {0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_BT13 == PDL_ON)
    { 
        &BT13,                // pstcInstance
        {0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_BT14 == PDL_ON)
    { 
        &BT14,                // pstcInstance
        {0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif
#if (PDL_PERIPHERAL_ENABLE_BT15 == PDL_ON)
    { 
        &BT15,                // pstcInstance
        {0u, 0u, 0u}          // stcInternData (not initialized yet)
    },
#endif   
};

/**
 ******************************************************************************
 ** \brief Return the internal data for a certain BT instance.
 **
 ** \param pstcBt Pointer to BT instance
 **
 ** \return Pointer to internal data or NULL if instance is not enabled (or not known)
 **
 ******************************************************************************/
static stc_bt_intern_data_t* BtGetInternDataPtr(volatile stc_btn_t* pstcBt) 
{
    uint32_t u32Instance;
   
    for (u32Instance = 0u; u32Instance < BT_INSTANCE_COUNT; u32Instance++)
    {
        if (pstcBt == m_astcBtInstanceDataLut[u32Instance].pstcInstance)
        {
            return &m_astcBtInstanceDataLut[u32Instance].stcInternData;
        }
    }

    return NULL;
}

#if defined(PDL_INTERRUPT_BT_ACTIVE)  
/**
 ******************************************************************************
 ** \brief BT interrupt service routine
 **
 ** \param pstcBt BT instance
 ** \param pstcBtInternData Pointer to BT internal data      
 **        
 ******************************************************************************/      
void Bt_IrqHandler( volatile stc_btn_t* pstcBt,
                    stc_bt_intern_data_t* pstcBtInternData) 
{
    uint8_t u8Irq;
    
    // Get Interrupt Request
    u8Irq = pstcBt->STC; 
     /*
      * Trigger Interrupt (PWM,PPG,Reload timer)
      * Measurement completion Interrupt (PWC)
      */
    if ((1ul << 2u) == (u8Irq & (1ul << 2u))) 
    {
        //  Clear Interrupt  
        pstcBt->STC &= ~(1ul << 2u); 

        if (pstcBtInternData->pfnBit2IrqCb != NULL) 
        {
            // Call CallBackIrq 
            pstcBtInternData->pfnBit2IrqCb();
        }
    }
    
    // Duty Match Interrupt (PWM)   
    if ((1ul << 1u) == (u8Irq & (1ul << 1u))) 
    {
        // Clear Interrupt 
        pstcBt->STC &= ~(1ul << 1u); 

        if (pstcBtInternData->pfnBit1IrqCb != NULL) 
        {
            // Call CallBackIrq
            pstcBtInternData->pfnBit1IrqCb();
        }
    }
  
    /* 
     * Underflow Interrupt (PWM,PPG,Reload timer)
     * Overflow Interrupt (PWC) 
     */
    if (1ul == (u8Irq & 1ul)) 
    {
        // Clear Interrupt 
        pstcBt->STC &= ~1ul;

        if (pstcBtInternData->pfnBit0IrqCb != NULL) 
        {
            // Call CallBackIrq
            pstcBtInternData->pfnBit0IrqCb();
        }
    }   

}      
  
/**
 ******************************************************************************
 ** \brief Set NVIC Interrupt depending on BT instance
 **
 ** \param pstcBt Pointer to BT instance
 **
 ******************************************************************************/
static void BtInitNvic(volatile stc_btn_t* pstcBt)
{
#if (PDL_MCU_CORE == PDL_FM0P_CORE)
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE1)
    NVIC_ClearPendingIRQ(BT0_3_FLASHIF_IRQn); 
    NVIC_EnableIRQ(BT0_3_FLASHIF_IRQn); 
    NVIC_SetPriority(BT0_3_FLASHIF_IRQn, PDL_IRQ_LEVEL_BT0_7_FLASHIF);
#elif (PDL_MCU_TYPE == PDL_FM0P_TYPE3)  
  #if (PDL_INTERRUPT_ENABLE_BT0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_BT4 == PDL_ON)   
    if((pstcBt == &BT0) || (pstcBt == &BT4))
    {
        NVIC_ClearPendingIRQ(BT0_4_IRQn); 
        NVIC_EnableIRQ(BT0_4_IRQn); 
        NVIC_SetPriority(BT0_4_IRQn, PDL_IRQ_LEVEL_BT0_4);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_BT5 == PDL_ON)   
    if((pstcBt == &BT1) || (pstcBt == &BT5))
    {
        NVIC_ClearPendingIRQ(BT1_5_IRQn); 
        NVIC_EnableIRQ(BT1_5_IRQn); 
        NVIC_SetPriority(BT1_5_IRQn, PDL_IRQ_LEVEL_BT1_5);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_BT6 == PDL_ON)   
    if((pstcBt == &BT2) || (pstcBt == &BT6))
    {
        NVIC_ClearPendingIRQ(BT2_6_IRQn); 
        NVIC_EnableIRQ(BT2_6_IRQn); 
        NVIC_SetPriority(BT2_6_IRQn, PDL_IRQ_LEVEL_BT2_6);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_BT7 == PDL_ON)   
    if((pstcBt == &BT3) || (pstcBt == &BT7))
    {
        NVIC_ClearPendingIRQ(BT3_7_IRQn); 
        NVIC_EnableIRQ(BT3_7_IRQn); 
        NVIC_SetPriority(BT3_7_IRQn, PDL_IRQ_LEVEL_BT3_7);
    }
  #endif  
#else
    NVIC_ClearPendingIRQ(BT0_7_FLASHIF_IRQn); 
    NVIC_EnableIRQ(BT0_7_FLASHIF_IRQn); 
    NVIC_SetPriority(BT0_7_FLASHIF_IRQn, PDL_IRQ_LEVEL_BT0_7_FLASHIF);
#endif    
#elif (PDL_MCU_CORE == PDL_FM3_CORE)
    NVIC_ClearPendingIRQ(BT0_7_IRQn); 
    NVIC_EnableIRQ(BT0_7_IRQn); 
    NVIC_SetPriority(BT0_7_IRQn, PDL_IRQ_LEVEL_BT0_7);
#elif (PDL_MCU_CORE == PDL_FM4_CORE)    
  #if (PDL_INTERRUPT_ENABLE_BT0 == PDL_ON)  
    if(pstcBt == &BT0)
    {
        NVIC_ClearPendingIRQ(BT0_IRQn); 
        NVIC_EnableIRQ(BT0_IRQn); 
        NVIC_SetPriority(BT0_IRQn, PDL_IRQ_LEVEL_BT0);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT1 == PDL_ON)         
    if(pstcBt == &BT1)
    {
        NVIC_ClearPendingIRQ(BT1_IRQn); 
        NVIC_EnableIRQ(BT1_IRQn); 
        NVIC_SetPriority(BT1_IRQn, PDL_IRQ_LEVEL_BT1);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT2 == PDL_ON)        
    if(pstcBt == &BT2)
    {
        NVIC_ClearPendingIRQ(BT2_IRQn); 
        NVIC_EnableIRQ(BT2_IRQn); 
        NVIC_SetPriority(BT2_IRQn, PDL_IRQ_LEVEL_BT2);
    }
  #endif 
  #if (PDL_INTERRUPT_ENABLE_BT3 == PDL_ON)        
    if(pstcBt == &BT3)
    {
        NVIC_ClearPendingIRQ(BT3_IRQn); 
        NVIC_EnableIRQ(BT3_IRQn); 
        NVIC_SetPriority(BT3_IRQn, PDL_IRQ_LEVEL_BT3);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT4 == PDL_ON)         
    if(pstcBt == &BT4)
    {
        NVIC_ClearPendingIRQ(BT4_IRQn); 
        NVIC_EnableIRQ(BT4_IRQn); 
        NVIC_SetPriority(BT4_IRQn, PDL_IRQ_LEVEL_BT4);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT5 == PDL_ON)          
    if(pstcBt == &BT5)
    {
        NVIC_ClearPendingIRQ(BT5_IRQn); 
        NVIC_EnableIRQ(BT5_IRQn); 
        NVIC_SetPriority(BT5_IRQn, PDL_IRQ_LEVEL_BT5);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT6 == PDL_ON)             
    if(pstcBt == &BT6)
    {
        NVIC_ClearPendingIRQ(BT6_IRQn); 
        NVIC_EnableIRQ(BT6_IRQn); 
        NVIC_SetPriority(BT6_IRQn, PDL_IRQ_LEVEL_BT6);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT7 == PDL_ON)        
    if(pstcBt == &BT7)
    {
        NVIC_ClearPendingIRQ(BT7_IRQn); 
        NVIC_EnableIRQ(BT7_IRQn); 
        NVIC_SetPriority(BT7_IRQn, PDL_IRQ_LEVEL_BT7);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT8 == PDL_ON)        
    if(pstcBt == &BT8)
    {
        NVIC_ClearPendingIRQ(BT8_IRQn); 
        NVIC_EnableIRQ(BT8_IRQn); 
        NVIC_SetPriority(BT8_IRQn, PDL_IRQ_LEVEL_BT8);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT9 == PDL_ON)         
    if(pstcBt == &BT9)
    {
        NVIC_ClearPendingIRQ(BT9_IRQn); 
        NVIC_EnableIRQ(BT9_IRQn); 
        NVIC_SetPriority(BT9_IRQn, PDL_IRQ_LEVEL_BT9);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT10 == PDL_ON)          
    if(pstcBt == &BT10)
    {
        NVIC_ClearPendingIRQ(BT10_IRQn); 
        NVIC_EnableIRQ(BT10_IRQn); 
        NVIC_SetPriority(BT10_IRQn, PDL_IRQ_LEVEL_BT10);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT11 == PDL_ON)            
    if(pstcBt == &BT11)
    {
        NVIC_ClearPendingIRQ(BT11_IRQn); 
        NVIC_EnableIRQ(BT11_IRQn); 
        NVIC_SetPriority(BT11_IRQn, PDL_IRQ_LEVEL_BT11);
    }
  #endif
  #if   (PDL_INTERRUPT_ENABLE_BT12 == PDL_ON) || \
        (PDL_INTERRUPT_ENABLE_BT13 == PDL_ON) || \
        (PDL_INTERRUPT_ENABLE_BT14 == PDL_ON) || \
        (PDL_INTERRUPT_ENABLE_BT15 == PDL_ON)  
    if((pstcBt == &BT12) || (pstcBt == &BT13) || (pstcBt == &BT14) || (pstcBt == &BT15)) 
    {
        NVIC_ClearPendingIRQ(BT12_15_IRQn); 
        NVIC_EnableIRQ(BT12_15_IRQn); 
        NVIC_SetPriority(BT12_15_IRQn, PDL_IRQ_LEVEL_BT12_13_14_15);      
    }
  #endif        

#endif    
} 

/**
 ******************************************************************************
 ** \brief Clear NVIC Interrupt depending on BT instance
 **
 ** \param pstcBt Pointer to BT instance
 **
 ******************************************************************************/
static void BtDeInitNvic(volatile stc_btn_t* pstcBt)
{        
#if (PDL_MCU_CORE == PDL_FM0P_CORE)
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE1)
    NVIC_ClearPendingIRQ(BT0_3_FLASHIF_IRQn); 
    NVIC_DisableIRQ(BT0_3_FLASHIF_IRQn); 
    NVIC_SetPriority(BT0_3_FLASHIF_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
#elif (PDL_MCU_TYPE == PDL_FM0P_TYPE3)
  #if (PDL_INTERRUPT_ENABLE_BT0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_BT4 == PDL_ON)  
    NVIC_ClearPendingIRQ(BT0_4_IRQn); 
    NVIC_DisableIRQ(BT0_4_IRQn); 
    NVIC_SetPriority(BT0_4_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  #endif 
  #if (PDL_INTERRUPT_ENABLE_BT1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_BT5 == PDL_ON)  
    NVIC_ClearPendingIRQ(BT1_5_IRQn); 
    NVIC_DisableIRQ(BT1_5_IRQn); 
    NVIC_SetPriority(BT1_5_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_BT6 == PDL_ON)  
    NVIC_ClearPendingIRQ(BT2_6_IRQn); 
    NVIC_DisableIRQ(BT2_6_IRQn); 
    NVIC_SetPriority(BT2_6_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_BT7 == PDL_ON)  
    NVIC_ClearPendingIRQ(BT3_7_IRQn); 
    NVIC_DisableIRQ(BT3_7_IRQn); 
    NVIC_SetPriority(BT3_7_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  #endif  
#else
    NVIC_ClearPendingIRQ(BT0_7_FLASHIF_IRQn); 
    NVIC_DisableIRQ(BT0_7_FLASHIF_IRQn); 
    NVIC_SetPriority(BT0_7_FLASHIF_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
#endif    
#elif (PDL_MCU_CORE == PDL_FM3_CORE)
    NVIC_ClearPendingIRQ(BT0_7_IRQn); 
    NVIC_DisableIRQ(BT0_7_IRQn); 
    NVIC_SetPriority(BT0_7_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
#elif (PDL_MCU_CORE == PDL_FM4_CORE)    
      #if (PDL_INTERRUPT_ENABLE_BT0 == PDL_ON)  
    if(&BT0 == pstcBt)
    {
        NVIC_ClearPendingIRQ(BT0_IRQn); 
        NVIC_DisableIRQ(BT0_IRQn); 
        NVIC_SetPriority(BT0_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT1 == PDL_ON)         
    if(&BT1 == pstcBt)
    {
        NVIC_ClearPendingIRQ(BT1_IRQn); 
        NVIC_DisableIRQ(BT1_IRQn); 
        NVIC_SetPriority(BT1_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT2 == PDL_ON)        
    if(&BT2 == pstcBt)
    {
        NVIC_ClearPendingIRQ(BT2_IRQn); 
        NVIC_DisableIRQ(BT2_IRQn); 
        NVIC_SetPriority(BT2_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
  #endif 
  #if (PDL_INTERRUPT_ENABLE_BT3 == PDL_ON)        
    if(&BT3 == pstcBt)
    {
        NVIC_ClearPendingIRQ(BT3_IRQn); 
        NVIC_DisableIRQ(BT3_IRQn); 
        NVIC_SetPriority(BT3_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT4 == PDL_ON)         
    if(&BT4 == pstcBt)
    {
        NVIC_ClearPendingIRQ(BT4_IRQn); 
        NVIC_DisableIRQ(BT4_IRQn); 
        NVIC_SetPriority(BT4_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT5 == PDL_ON)          
    if(&BT5 == pstcBt)
    {
        NVIC_ClearPendingIRQ(BT5_IRQn); 
        NVIC_DisableIRQ(BT5_IRQn); 
        NVIC_SetPriority(BT5_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT6 == PDL_ON)             
    if(&BT6 == pstcBt)
    {
        NVIC_ClearPendingIRQ(BT6_IRQn); 
        NVIC_DisableIRQ(BT6_IRQn); 
        NVIC_SetPriority(BT6_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT7 == PDL_ON)        
    if(&BT7 == pstcBt)
    {
        NVIC_ClearPendingIRQ(BT7_IRQn); 
        NVIC_DisableIRQ(BT7_IRQn); 
        NVIC_SetPriority(BT7_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT8 == PDL_ON)        
    if(&BT8 == pstcBt)
    {
        NVIC_ClearPendingIRQ(BT8_IRQn); 
        NVIC_DisableIRQ(BT8_IRQn); 
        NVIC_SetPriority(BT8_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT9 == PDL_ON)         
    if(&BT9 == pstcBt)
    {
        NVIC_ClearPendingIRQ(BT9_IRQn); 
        NVIC_DisableIRQ(BT9_IRQn); 
        NVIC_SetPriority(BT9_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT10 == PDL_ON)          
    if(&BT10 == pstcBt)
    {
        NVIC_ClearPendingIRQ(BT10_IRQn); 
        NVIC_DisableIRQ(BT10_IRQn); 
        NVIC_SetPriority(BT10_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
  #endif
  #if (PDL_INTERRUPT_ENABLE_BT11 == PDL_ON)            
    if(&BT11 == pstcBt)
    {
        NVIC_ClearPendingIRQ(BT11_IRQn); 
        NVIC_DisableIRQ(BT11_IRQn); 
        NVIC_SetPriority(BT11_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
    }
  #endif
  #if   (PDL_INTERRUPT_ENABLE_BT12 == PDL_ON) || \
        (PDL_INTERRUPT_ENABLE_BT13 == PDL_ON) || \
        (PDL_INTERRUPT_ENABLE_BT14 == PDL_ON) || \
        (PDL_INTERRUPT_ENABLE_BT15 == PDL_ON)  
    if ((&BT12 == pstcBt) || (&BT13 == pstcBt) || (&BT14 == pstcBt) || (&BT15 == pstcBt)) 
    {
        NVIC_ClearPendingIRQ(BT12_15_IRQn); 
        NVIC_DisableIRQ(BT12_15_IRQn); 
        NVIC_SetPriority(BT12_15_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);      
    }
  #endif   
#endif    
  
    return;
} 

#endif 

/**
 ******************************************************************************
 ** \brief Configure BT IO mode
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  enIoMode         BT IO mode
 ** \arg         BtIoMode0        BT IO mode 0
 ** \arg         BtIoMode1        BT IO mode 1
 ** \arg         BtIoMode2        BT IO mode 2
 ** \arg         BtIoMode3        BT IO mode 3
 ** \arg         BtIoMode4        BT IO mode 4
 ** \arg         BtIoMode5        BT IO mode 5
 ** \arg         BtIoMode6        BT IO mode 6
 ** \arg         BtIoMode7        BT IO mode 7
 ** \arg         BtIoMode8        BT IO mode 8
 **
 ** \retval Ok                    BT IO mode has been set successfully
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcBt == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Bt_ConfigIOMode(volatile stc_btn_t* pstcBt, en_bt_io_mode_t enIoMode)
{
    // Pointer to internal data
    stc_bt_intern_data_t* pstcBtInternData ;
    // Check for NULL pointer
    if ((NULL == pstcBt) || (enIoMode > BtIoMode8))
    {
        return ErrorInvalidParameter ;
    }  
    
    // Get pointer to internal data structure ...
    pstcBtInternData = BtGetInternDataPtr( pstcBt ) ;
    
    if(NULL == pstcBtInternData)
    {
        return ErrorInvalidParameter ;
    }
    
    if((&BT0 == pstcBt) || (&BT1 == pstcBt))
    {
        FM_BTIOSEL03->BTSEL0123_f.SEL01 = (uint8_t)enIoMode; 
    }
    else if((&BT2 == pstcBt) || (&BT3 == pstcBt))
    {
        FM_BTIOSEL03->BTSEL0123_f.SEL23 = (uint8_t)enIoMode; 
    }
#if (defined(FM_BTIOSEL47_AVAILABLE))
    else if((&BT4 == pstcBt) || (&BT5 == pstcBt))
    {
        FM_BTIOSEL47->BTSEL4567_f.SEL45 = (uint8_t)enIoMode;
    }
    else if((&BT6 == pstcBt) || (&BT7 == pstcBt))
    {
        FM_BTIOSEL47->BTSEL4567_f.SEL67 = (uint8_t)enIoMode;
    }
#endif
    
    return Ok;
}

#if (PDL_PERIPHERAL_ENABLE_BT_PWM_MODE == PDL_ON)
/**
 ******************************************************************************
 ** \brief Initialize PWM function of BT
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  pstcPwmConfig    Pointer to PWM configuration
 **
 ** \retval Ok                    PWM function has been configured successfully
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcBt == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Bt_Pwm_Init(volatile stc_btn_t* pstcBt, const stc_bt_pwm_config_t* pstcPwmConfig)
{
    volatile FM_BT_PWM_TypeDef* pRegBt = (volatile FM_BT_PWM_TypeDef*)pstcBt;
    // Pointer to internal data
    stc_bt_intern_data_t* pstcBtInternData ;
    uint16_t u16Pres;
    // Check for NULL pointer
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    // Get pointer to internal data structure ...
    pstcBtInternData = BtGetInternDataPtr( pstcBt ) ;
    
    if(NULL == pstcBtInternData)
    {
        return ErrorInvalidParameter ;
    }
    
    // Reset Base Timer mode
    pRegBt->TMCR_f.FMD = BtResetMode;
    // Set Base Timer to PWM mode
    pRegBt->TMCR_f.FMD = BtPwmMode;
    
    // Clock prescaler setting
    u16Pres = pstcPwmConfig->enPres;  // Workaround for MISRA-C 2004 rule 12.7
    switch (pstcPwmConfig->enPres)
    {
        case PwmPresNone:
        case PwmPres1Div4:
        case PwmPres1Div16:
        case PwmPres1Div128:
        case PwmPres1Div256:
        case PwmPres1ExtClkRising:
        case PwmPres1ExtClkFalling:
        case PwmPres1ExtClkBoth: 
            pRegBt->TMCR2_f.CKS3 = 0u;
            pRegBt->TMCR_f.CKS = u16Pres;
            break;
        case PwmPres1Div512:
        case PwmPres1Div1024:
        case PwmPres1Div2048:
            pRegBt->TMCR2_f.CKS3 = 1u;
            pRegBt->TMCR_f.CKS = (u16Pres & 0x07u);
            break;
    }
    // Restart enable setting
    switch (pstcPwmConfig->enRestartEn)
    {
        case PwmRestartDisable:
            pRegBt->TMCR_f.RTGEN = 0u;
            break;
        case PwmRestartEnable:
            pRegBt->TMCR_f.RTGEN = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    // Output Mask setting
    switch (pstcPwmConfig->enOutputMask)
    {
        case PwmOutputNormal:
            pRegBt->TMCR_f.PMSK = 0u;
            break;
        case PwmOutputMask:
            pRegBt->TMCR_f.PMSK = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    // Output polarity setting
    switch (pstcPwmConfig->enOutputPolarity)
    {
        case PwmPolarityLow:
            pRegBt->TMCR_f.OSEL = 0u;
            break;
        case PwmPolarityHigh:
            pRegBt->TMCR_f.OSEL = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    // External trigger setting
    switch (pstcPwmConfig->enExtTrig)
    {
        case PwmExtTrigDisable:
            pRegBt->TMCR_f.EGS =  0u;
            break;
        case PwmExtTrigRising:
            pRegBt->TMCR_f.EGS = 1u;
            break;
        case PwmExtTrigFalling:
            pRegBt->TMCR_f.EGS = 2u;
            break;
        case PwmExtTrigBoth:
            pRegBt->TMCR_f.EGS = 3u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    // Mode setting
    switch (pstcPwmConfig->enMode)
    {
        case PwmContinuous:
            pRegBt->TMCR_f.MDSE = 0u;
            break;
        case PwmOneshot:
            pRegBt->TMCR_f.MDSE = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }   
    
#if defined(PDL_INTERRUPT_BT_ACTIVE)      
    // Interrupt enable setting
    if(NULL != pstcPwmConfig->pstcPwmIrqEn)
    {
        if(TRUE == pstcPwmConfig->pstcPwmIrqEn->bPwmTrigIrq)
        { 
            pRegBt->STC_f.TGIE = 1u;
        }
        
        if(TRUE == pstcPwmConfig->pstcPwmIrqEn->bPwmDutyMatchIrq)
        {
            pRegBt->STC_f.DTIE = 1u;
        }
        
        if(TRUE == pstcPwmConfig->pstcPwmIrqEn->bPwmUnderflowIrq)
        {
            pRegBt->STC_f.UDIE = 1u;
        }
    }
    
    // Interrupt callback functions
    if(NULL != pstcPwmConfig->pstcPwmIrqCb)
    {
        pstcBtInternData->pfnBit2IrqCb = pstcPwmConfig->pstcPwmIrqCb->pfnPwmTrigIrqCb;
        pstcBtInternData->pfnBit1IrqCb = pstcPwmConfig->pstcPwmIrqCb->pfnPwmDutyMatchIrqCb;
        pstcBtInternData->pfnBit0IrqCb = pstcPwmConfig->pstcPwmIrqCb->pfnPwmUnderflowIrqCb;
    }
    
    // NVIC setting
    if(TRUE == pstcPwmConfig->bTouchNvic)
    {
        BtInitNvic(pstcBt);
    }
#endif    
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief De-Initialize PWM function of BT
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  bTouchNvic       Touch NVIC or not
 **
 ** \retval Ok                    PWM function has been de-initialized successfully
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcBt == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Bt_Pwm_DeInit(volatile stc_btn_t* pstcBt, boolean_t bTouchNvic)
{
    volatile FM_BT_PWM_TypeDef* pRegBt = (volatile FM_BT_PWM_TypeDef*)pstcBt;
    // Pointer to internal data
    stc_bt_intern_data_t* pstcBtInternData ;
    // Check for NULL pointer
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    // Get pointer to internal data structure ...
    pstcBtInternData = BtGetInternDataPtr( pstcBt ) ;
    
    if(NULL == pstcBtInternData)
    {
        return ErrorInvalidParameter ;
    }
    
    pRegBt->TMCR = 0x0000u;
    pRegBt->TMCR2 = 0x0000u;
    pRegBt->PCSR = 0x0000u;
    pRegBt->STC = 0x0000u;
    pRegBt->PDUT = 0x0000u;
#if defined(PDL_INTERRUPT_BT_ACTIVE)   
    if(TRUE == bTouchNvic)
    {
        BtDeInitNvic(pstcBt);
    }
#endif    
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Enable PWM timer counting
 **
 ** \param [in]  pstcBt           BT instance
 **
 ** \retval Ok                    Enable PWM timer counting successfully
 ** \retval ErrorInvalidParameter pstcBt == NULL
 ******************************************************************************/
en_result_t Bt_Pwm_EnableCount(volatile stc_btn_t* pstcBt)
{
    // Check for NULL pointer
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    ((volatile FM_BT_PWM_TypeDef*)pstcBt)->TMCR_f.CTEN = 1u;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable PWM timer counting
 **
 ** \param [in]  pstcBt           BT instance
 **
 ** \retval Ok                    Disable PWM timer counting successfully
 ** \retval ErrorInvalidParameter pstcBt == NULL
 ******************************************************************************/
en_result_t Bt_Pwm_DisableCount(volatile stc_btn_t* pstcBt)
{
    // Check for NULL pointer
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    ((volatile FM_BT_PWM_TypeDef*)pstcBt)->TMCR_f.CTEN = 0u;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Start PWM timer by software
 **
 ** \param [in]  pstcBt           BT instance
 **
 ** \retval Ok                    Start PWM timer successfully
 ** \retval ErrorInvalidParameter pstcBt == NULL
 ******************************************************************************/
en_result_t Bt_Pwm_EnableSwTrig(volatile stc_btn_t* pstcBt)
{
    // Check for NULL pointer
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    ((volatile FM_BT_PWM_TypeDef*)pstcBt)->TMCR_f.STRG = 1u;
    return Ok;
}

#if defined(PDL_INTERRUPT_BT_ACTIVE)  
/**
 ******************************************************************************
 ** \brief Enable PWM timer interrupt
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  enIrqSel         PWM timer interrupt type  
 ** \arg         PwmTrigIrq       PWM trigger detection interrupt
 ** \arg         PwmDutyMatchIrq  PWM duty match interrupt 
 ** \arg         PwmUnderflowIrq  PWM underflow interrupt   
 **
 ** \retval Ok                    Enable PWM timer interrupt successfully
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcBt == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Bt_Pwm_EnableIrq(volatile stc_btn_t* pstcBt, en_pwm_irq_sel_t enIrqSel)
{
    // Check parameter
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    switch (enIrqSel)
    {
        case PwmTrigIrq:
            ((volatile FM_BT_PWM_TypeDef*)pstcBt)->STC_f.TGIE = 1u;
            break;
        case PwmDutyMatchIrq:
            ((volatile FM_BT_PWM_TypeDef*)pstcBt)->STC_f.DTIE = 1u;
            break;
        case PwmUnderflowIrq:
            ((volatile FM_BT_PWM_TypeDef*)pstcBt)->STC_f.UDIE = 1u;
            break;
        default:
            return ErrorInvalidParameter ;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable PWM timer interrupt
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  enIrqSel         PWM timer interrupt type    
 ** \arg         PwmTrigIrq       PWM trigger detection interrupt
 ** \arg         PwmDutyMatchIrq  PWM duty match interrupt 
 ** \arg         PwmUnderflowIrq  PWM underflow interrupt   
 **
 ** \retval Ok                    Disable PWM timer interrupt successfully
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcBt == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Bt_Pwm_DisableIrq(volatile stc_btn_t* pstcBt, en_pwm_irq_sel_t enIrqSel)
{
    // Check parameter
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    switch (enIrqSel)
    {
        case PwmTrigIrq:
            ((volatile FM_BT_PWM_TypeDef*)pstcBt)->STC_f.TGIE = 0u;
            break;
        case PwmDutyMatchIrq:
            ((volatile FM_BT_PWM_TypeDef*)pstcBt)->STC_f.DTIE = 0u;
            break;
        case PwmUnderflowIrq:
            ((volatile FM_BT_PWM_TypeDef*)pstcBt)->STC_f.UDIE = 0u;
            break;
        default:
            return ErrorInvalidParameter ;
    }

    return Ok;
}

#endif  

/**
 ******************************************************************************
 ** \brief Get interrupt flag of PWM timer 
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  enIrqSel         PWM timer interrupt type
 ** \arg         PwmTrigIrq       PWM trigger detection interrupt
 ** \arg         PwmDutyMatchIrq  PWM duty match interrupt 
 ** \arg         PwmUnderflowIrq  PWM underflow interrupt   
 **
 ** \retval PdlSet                Interrupt flag is set
 ** \retval PdlClr                Interrupt flag is clear
 ******************************************************************************/
en_irq_flag_t Bt_Pwm_GetIrqFlag(volatile stc_btn_t* pstcBt, en_pwm_irq_sel_t enIrqSel)
{
    en_irq_flag_t enFlag = PdlClr; 
    
    switch (enIrqSel)
    {
        case PwmTrigIrq:
            enFlag = ((volatile FM_BT_PWM_TypeDef*)pstcBt)->STC_f.TGIR ? PdlSet : PdlClr;
            break;
        case PwmDutyMatchIrq:
            enFlag = ((volatile FM_BT_PWM_TypeDef*)pstcBt)->STC_f.DTIR ? PdlSet : PdlClr;
            break;
        case PwmUnderflowIrq:
            enFlag = ((volatile FM_BT_PWM_TypeDef*)pstcBt)->STC_f.UDIR ? PdlSet : PdlClr;
            break;
        default:
            break;
    }
    
    return enFlag;
}

/**
 ******************************************************************************
 ** \brief Clear interrupt flag of PWM timer 
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  enIrqSel         PWM timer interrupt type
 ** \arg         PwmTrigIrq       PWM trigger detection interrupt
 ** \arg         PwmDutyMatchIrq  PWM duty match interrupt 
 ** \arg         PwmUnderflowIrq  PWM underflow interrupt   
 **
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcBt == NULL
 **                               - enIntType > PwmUnderflowInt
 ** \retval Ok                    Clear interrupt flag successfully
 ******************************************************************************/
en_result_t Bt_Pwm_ClrIrqFlag(volatile stc_btn_t* pstcBt, en_pwm_irq_sel_t enIrqSel)
{
    if(NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }
  
    switch (enIrqSel)
    {
        case PwmTrigIrq:
            ((volatile FM_BT_PWM_TypeDef*)pstcBt)->STC_f.TGIR = 0u;
            break;
        case PwmDutyMatchIrq:
            ((volatile FM_BT_PWM_TypeDef*)pstcBt)->STC_f.DTIR = 0u;
            break;
        case PwmUnderflowIrq:
            ((volatile FM_BT_PWM_TypeDef*)pstcBt)->STC_f.UDIR = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Write Cycle value of PWM timer 
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  u16Cycle         Cycle value
 **
 ** \retval Ok                    Write Cycle value successfully
 ** \retval ErrorInvalidParameter pstcBt == NULL
 ******************************************************************************/
en_result_t Bt_Pwm_WriteCycleVal(volatile stc_btn_t* pstcBt, uint16_t u16Cycle)
{
    // Check parameter
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    ((volatile FM_BT_PWM_TypeDef*)pstcBt)->PCSR = u16Cycle;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Write duty value of PWM timer 
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  u16Duty          Duty value
 **
 ** \retval Ok                    Write duty value successfully
 ** \retval ErrorInvalidParameter pstcBt == NUL
 ******************************************************************************/
en_result_t Bt_Pwm_WriteDutyVal(volatile stc_btn_t* pstcBt, uint16_t u16Duty)
{
    // Check parameter
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    ((volatile FM_BT_PWM_TypeDef*)pstcBt)->PDUT = u16Duty;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Read current count value of PWM timer 
 **
 ** \param [in]  pstcBt           BT instance
 **
 ** \retval Current count value
 ******************************************************************************/
uint16_t Bt_Pwm_ReadCurCnt(volatile stc_btn_t* pstcBt)
{ 
    return ((volatile FM_BT_PWM_TypeDef*)pstcBt)->TMR;
}
#endif

#if (PDL_PERIPHERAL_ENABLE_BT_PPG_MODE == PDL_ON)
/**
 ******************************************************************************
 ** \brief Initialize PPG function of BT
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  pstcPpgConfig    Pointer to PPG configuration
 **
 ** \retval Ok                    PPG function has been configured successfully
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcBt == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Bt_Ppg_Init(volatile stc_btn_t* pstcBt, const stc_bt_ppg_config_t* pstcPpgConfig)
{
    volatile FM_BT_PPG_TypeDef* pRegBt = (volatile FM_BT_PPG_TypeDef*)pstcBt;
    // Pointer to internal data
    stc_bt_intern_data_t* pstcBtInternData ;
    uint16_t u16Pres;
    // Check for NULL pointer
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    // Get pointer to internal data structure ...
    pstcBtInternData = BtGetInternDataPtr( pstcBt ) ;
    
    if(NULL == pstcBtInternData)
    {
        return ErrorInvalidParameter ;
    }
    
    // Reset Base Timer mode
    pRegBt->TMCR_f.FMD = BtResetMode;
    // Set Base Timer to PPG mode
    pRegBt->TMCR_f.FMD = BtPpgMode;
    
    // Clock prescaler setting
    u16Pres = pstcPpgConfig->enPres; // Workaround for MISRA-C 2004 rule 12.7
    switch (pstcPpgConfig->enPres)
    {
        case PpgPresNone:
        case PpgPres1Div4:
        case PpgPres1Div16:
        case PpgPres1Div128:
        case PpgPres1Div256:
        case PpgPres1ExtClkRising:
        case PpgPres1ExtClkFalling:
        case PpgPres1ExtClkBoth:
            pRegBt->TMCR2_f.CKS3 = 0u;
            pRegBt->TMCR_f.CKS = u16Pres;
            break;  
        case PpgPres1Div512:
        case PpgPres1Div1024:
        case PpgPres1Div2048:
            pRegBt->TMCR2_f.CKS3 = 1u;
            pRegBt->TMCR_f.CKS = (u16Pres & 0x7u);
            break;
        default:
            return ErrorInvalidParameter ;
    }
    // Restart enable setting
    switch (pstcPpgConfig->enRestartEn)
    {
        case PpgRestartDisable:
            pRegBt->TMCR_f.RTGEN = 0u;
            break;
        case PpgRestartEnable:
            pRegBt->TMCR_f.RTGEN = 1u;
            break;
        default:
            return ErrorInvalidParameter ;
    }
    // Output Mask setting
    pRegBt->TMCR_f.PMSK = (uint8_t)pstcPpgConfig->enOutputMask;
    switch (pstcPpgConfig->enOutputMask)
    {
        case PpgOutputNormal:
            pRegBt->TMCR_f.PMSK = 0u;
            break;
        case PpgOutputMask:
            pRegBt->TMCR_f.PMSK = 1u;
            break;
        default:   
            return ErrorInvalidParameter ;
    }       
    // Output polarity setting
    switch (pstcPpgConfig->enOutputPolarity)
    {
        case PpgPolarityLow:
            pRegBt->TMCR_f.OSEL = 0u;
            break;
        case PpgPolarityHigh:
            pRegBt->TMCR_f.OSEL = 1u;
            break;
        default:
            return ErrorInvalidParameter ;
    }
    // External trigger setting    
    switch (pstcPpgConfig->enExtTrig)
    {
        case PpgExtTrigDisable:
            pRegBt->TMCR_f.EGS = 0u;
            break;
        case PpgExtTrigRising:
            pRegBt->TMCR_f.EGS = 1u;
            break;
        case PpgExtTrigFalling:
            pRegBt->TMCR_f.EGS = 2u;
            break;
        case PpgExtTrigBoth:
            pRegBt->TMCR_f.EGS = 3u;
            break;
        default:
            return ErrorInvalidParameter ;
    }
    // Mode setting
    switch (pstcPpgConfig->enMode)
    {
        case PpgContinuous:
            pRegBt->TMCR_f.MDSE = 0u;
            break;  
        case PpgOneshot:
            pRegBt->TMCR_f.MDSE = 1u;
            break;
        default:
            return ErrorInvalidParameter ;
    }
    
#if defined(PDL_INTERRUPT_BT_ACTIVE)      
    // Interrupt enable setting
    if (NULL != pstcPpgConfig->pstcPpgIrqEn)
    {
        if(pstcPpgConfig->pstcPpgIrqEn->bPpgTrigIrq == TRUE)
        {
            pRegBt->STC_f.TGIE = 1;   
        } 
        if(pstcPpgConfig->pstcPpgIrqEn->bPpgUnderflowIrq == TRUE)
        {
            pRegBt->STC_f.UDIE = 1;   
        }
    }
    
    // Interrupt callback functions
    if (NULL != pstcPpgConfig->pstcPpgIrqCb)
    {
        pstcBtInternData->pfnBit2IrqCb = pstcPpgConfig->pstcPpgIrqCb->pfnPpgTrigIrqCb;
        pstcBtInternData->pfnBit0IrqCb = pstcPpgConfig->pstcPpgIrqCb->pfnPpgUnderflowIrqCb;
    }
    // NVIC setting
    if(pstcPpgConfig->bTouchNvic == TRUE)
    {
        BtInitNvic(pstcBt);
    }
#endif     
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief De-Initialize PPG function of BT
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  bTouchNvic       Touch NVIC or not
 **
 ** \retval Ok                    PPG function has been de-initialized successfully
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcBt == NULL
 **                               - Other invalid configuration
 ******************************************************************************/                    
en_result_t Bt_Ppg_DeInit(volatile stc_btn_t* pstcBt, boolean_t bTouchNvic)
{
    volatile FM_BT_PPG_TypeDef* pRegBt = (volatile FM_BT_PPG_TypeDef*)pstcBt;
    // Pointer to internal data
    stc_bt_intern_data_t* pstcBtInternData ;
    // Check for NULL pointer
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    // Get pointer to internal data structure ...
    pstcBtInternData = BtGetInternDataPtr( pstcBt ) ;
    
    if(NULL == pstcBtInternData)
    {
        return ErrorInvalidParameter ;
    }
    
    pRegBt->TMCR = 0x0000u;
    pRegBt->TMCR2 = 0x0000u;
    pRegBt->STC = 0x0000u;
    pRegBt->PRLL = 0x0000u;
    pRegBt->PRLH = 0x0000u;
    
#if defined(PDL_INTERRUPT_BT_ACTIVE)   
    if(TRUE == bTouchNvic)
    {
        BtDeInitNvic(pstcBt);
    }
#endif    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Enable PPG timer counting
 **
 ** \param [in]  pstcBt           BT instance
 **
 ** \retval Ok                    Enable PPG timer counting successfully
 ** \retval ErrorInvalidParameter pstcBt == NULL
 ******************************************************************************/
en_result_t Bt_Ppg_EnableCount(volatile stc_btn_t* pstcBt)
{
    // Check for NULL pointer
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    ((volatile FM_BT_PPG_TypeDef*)pstcBt)->TMCR_f.CTEN = 1u;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable PPG timer counting
 **
 ** \param [in]  pstcBt           BT instance
 **
 ** \retval Ok                    Disable PPG timer counting successfully
 ** \retval ErrorInvalidParameter pstcBt == NULL
 ******************************************************************************/
en_result_t Bt_Ppg_DisableCount(volatile stc_btn_t* pstcBt)
{
    // Check for NULL pointer
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    ((volatile FM_BT_PPG_TypeDef*)pstcBt)->TMCR_f.CTEN = 0u;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Start PPG timer by software
 **
 ** \param [in]  pstcBt           BT instance
 **
 ** \retval Ok                    Start PPG timer successfully
 ** \retval ErrorInvalidParameter pstcBt == NULL
 ******************************************************************************/
en_result_t Bt_Ppg_EnableSwTrig(volatile stc_btn_t* pstcBt)
{
    // Check for NULL pointer
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    ((volatile FM_BT_PPG_TypeDef*)pstcBt)->TMCR_f.STRG = 1u;
    return Ok;
}

#if defined(PDL_INTERRUPT_BT_ACTIVE)   
/**
 ******************************************************************************
 ** \brief Enable PPG timer interrupt
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  enIrqSel         PPG timer interrupt type 
 ** \arg         PpgTrigIrq       PPG trigger detection interrupt
 ** \arg         PpgUnderflowIrq  PPG underflow interrupt
 **
 ** \retval Ok                    Enable PPG timer interrupt successfully
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcBt == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Bt_Ppg_EnableIrq(volatile stc_btn_t* pstcBt, en_ppg_irq_sel_t enIrqSel)
{
    // Pointer to internal data
    stc_bt_intern_data_t* pstcBtInternData ;
    // Check parameter
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    // Get pointer to internal data structure ...
    pstcBtInternData = BtGetInternDataPtr( pstcBt ) ;
    
    if(NULL == pstcBtInternData)
    {
        return ErrorInvalidParameter ;
    }
    
    switch (enIrqSel)
    {
        case PpgTrigIrq:
            ((volatile FM_BT_PPG_TypeDef*)pstcBt)->STC_f.TGIE = 1u;
            break;
        case PpgUnderflowIrq:
            ((volatile FM_BT_PPG_TypeDef*)pstcBt)->STC_f.UDIE = 1u;
            break;
        default:
            return ErrorInvalidParameter ;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable PPG timer interrupt
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  enIrqSel         PPG timer interrupt type 
 ** \arg         PpgTrigIrq       PPG trigger detection interrupt
 ** \arg         PpgUnderflowIrq  PPG underflow interrupt
 **
 ** \retval Ok                    Disable PPG timer interrupt successfully
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcBt == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Bt_Ppg_DisableIrq(volatile stc_btn_t* pstcBt, en_ppg_irq_sel_t enIrqSel)
{
    // Check parameter
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    switch (enIrqSel)
    {
        case PpgTrigIrq:
            ((volatile FM_BT_PPG_TypeDef*)pstcBt)->STC_f.TGIE = 0u;
            break;
        case PpgUnderflowIrq:
            ((volatile FM_BT_PPG_TypeDef*)pstcBt)->STC_f.UDIE = 0u;
            break;
        default:
            return ErrorInvalidParameter ;
    }
     
    return Ok;
}

#endif

/**
 ******************************************************************************
 ** \brief Get interrupt flag of PPG timer 
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  enIrqSel         PPG timer interrupt type
 ** \arg         PpgTrigIrq       PPG trigger detection interrupt
 ** \arg         PpgUnderflowIrq  PPG underflow interrupt
 **
 ** \retval PdlSet                Interrupt flag is set
 ** \retval PdlClr                Interrupt flag is clear
 ******************************************************************************/
en_irq_flag_t Bt_Ppg_GetIrqFlag(volatile stc_btn_t* pstcBt, en_ppg_irq_sel_t enIrqSel)
{
    en_irq_flag_t enFlag = PdlClr; 
    
    switch (enIrqSel)
    {
        case PpgTrigIrq:
            enFlag = ((volatile FM_BT_PPG_TypeDef*)pstcBt)->STC_f.TGIR ? PdlSet : PdlClr;
            break;
        case PpgUnderflowIrq:
            enFlag = ((volatile FM_BT_PPG_TypeDef*)pstcBt)->STC_f.UDIR ? PdlSet : PdlClr;
            break;
        default:
            break;
    }
    
    return enFlag;
}

/**
 ******************************************************************************
 ** \brief Clear interrupt flag of PPG timer 
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  enIrqSel         PPG timer interrupt type
 ** \arg         PpgTrigIrq       PPG trigger detection interrupt
 ** \arg         PpgUnderflowIrq  PPG underflow interrupt
 **
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcBt == NULL
 **                               - enIntType > PwmUnderflowInt
 ** \retval Ok                    Clear interrupt flag successfully
 ******************************************************************************/
en_result_t Bt_Ppg_ClrIrqFlag(volatile stc_btn_t* pstcBt, en_ppg_irq_sel_t enIrqSel)
{
    if(NULL == pstcBt)
    {
        return ErrorInvalidParameter;
    }
  
    switch (enIrqSel)
    {
        case PpgTrigIrq:
            ((volatile FM_BT_PPG_TypeDef*)pstcBt)->STC_f.TGIR = 0u;
            break;
        case PpgUnderflowIrq:
            ((volatile FM_BT_PPG_TypeDef*)pstcBt)->STC_f.UDIR = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}
                    
/**
 ******************************************************************************
 ** \brief Write low width count value of PPG timer 
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  u16Val           Low width count value
 **
 ** \retval Ok                    Write low width count value successfully
 ** \retval ErrorInvalidParameter pstcBt == NULL
 ******************************************************************************/
en_result_t Bt_Ppg_WriteLowWidthVal(volatile stc_btn_t* pstcBt, uint16_t u16Val)
{
    // Check parameter
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    ((volatile FM_BT_PPG_TypeDef*)pstcBt)->PRLL = u16Val;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Write high width count value of PPG timer 
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  u16Val           High width count value
 **
 ** \retval Ok                    Write high width count value successfully
 ** \retval ErrorInvalidParameter pstcBt == NULL
 ******************************************************************************/
en_result_t Bt_Ppg_WriteHighWidthVal(volatile stc_btn_t* pstcBt, uint16_t u16Val)
{
    // Check parameter
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    ((volatile FM_BT_PPG_TypeDef*)pstcBt)->PRLH = u16Val;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Read current count value of PPG timer 
 **
 ** \param [in]  pstcBt           BT instance
 **
 ** \retval Current count value
 ******************************************************************************/
uint16_t Bt_Ppg_ReadCurCnt(volatile stc_btn_t* pstcBt)
{ 
    return ((volatile FM_BT_PPG_TypeDef*)pstcBt)->TMR;
}
#endif

#if (PDL_PERIPHERAL_ENABLE_BT_RT_MODE == PDL_ON)
/**
 ******************************************************************************
 ** \brief Initialize RT function of BT
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  pstcRtConfig     Pointer to RT configuration
 **
 ** \retval Ok                    RT function has been configured successfully
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcBt == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Bt_Rt_Init(volatile stc_btn_t* pstcBt, const stc_bt_rt_config_t* pstcRtConfig)
{
    volatile FM_BT_RT_TypeDef* pRegBt = (volatile FM_BT_RT_TypeDef*) pstcBt;
    // Pointer to internal data
    stc_bt_intern_data_t* pstcBtInternData ;
    uint16_t u16Pres, u16ExtTrig;
    // Check for NULL pointer
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  

    // Get pointer to internal data structure ...
    pstcBtInternData = BtGetInternDataPtr( pstcBt ) ;
    
    if(NULL == pstcBtInternData)
    {
        return ErrorInvalidParameter ;
    }
    
    // Reset Base Timer mode
    pRegBt->TMCR_f.FMD = BtResetMode;
    // Set Base Timer to Reload Timer mode
    pRegBt->TMCR_f.FMD = BtRtMode;
    
    // Clock prescaler setting
    u16Pres = pstcRtConfig->enPres; // Workaround for MISRA-C 2004 rule 12.7
    switch (pstcRtConfig->enPres)
    {
        case RtPresNone:
        case RtPres1Div4:
        case RtPres1Div16:
        case RtPres1Div128:
        case RtPres1Div256:
        case RtPres1ExtClkRising:
        case RtPres1ExtClkFalling:
        case RtPres1ExtClkBoth:
            pRegBt->TMCR2_f.CKS3 = 0u;
            pRegBt->TMCR_f.CKS = u16Pres;
            break;  
        case RtPres1Div512:
        case RtPres1Div1024:
        case RtPres1Div2048:
            pRegBt->TMCR2_f.CKS3 = 1u;
            pRegBt->TMCR_f.CKS = (u16Pres & 0x7u);
            break;
        default:
            return ErrorInvalidParameter ;
    }
    // Timer size setting
    switch (pstcRtConfig->enSize)
    {
        case RtSize16Bit:
            pRegBt->TMCR_f.T32 = 0u;
            break;
        case RtSize32Bit:
            pRegBt->TMCR_f.T32 = 1u;
            break;
        default:
            return ErrorInvalidParameter ;
    }
    // Output polarity setting
    switch (pstcRtConfig->enOutputPolarity)
    {
        case RtPolarityLow:
            pRegBt->TMCR_f.OSEL = 0u;
            break;
        case RtPolarityHigh:
            pRegBt->TMCR_f.OSEL = 1u;
            break;
        default:
            return ErrorInvalidParameter ;  
    }
    
    // External trigger setting
    u16ExtTrig = pstcRtConfig->enExtTrig; // Workaround for MISRA-C 2004 rule 12.7
    switch (pstcRtConfig->enExtTrig)
    {
        case RtExtTiggerDisable:
        case RtExtTiggerRisingEdge:
        case RtExtTiggerFallingEdge:
            pRegBt->TMCR_f.EGS = u16ExtTrig;
            break;
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)             
        case RtExtTiggerBothEdge:
            pRegBt->TMCR2_f.GATE = 0u;        
            pRegBt->TMCR_f.EGS = u16ExtTrig;
            break;
        case RtExtTiggerLowLevel:
            pRegBt->TMCR2_f.GATE = 1u;        
            pRegBt->TMCR_f.EGS = 0u;
            break;
        case RtExtTiggerHighLevel:
            pRegBt->TMCR2_f.GATE = 1u;
            pRegBt->TMCR_f.EGS = 1u; 
            break;
    #endif        
        default:
            return ErrorInvalidParameter;
    }
   
    // Mode setting
    switch (pstcRtConfig->enMode)
    {
        case RtReload:
            pRegBt->TMCR_f.MDSE = 0u;
            break;
        case RtOneshot:
            pRegBt->TMCR_f.MDSE = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
#if defined(PDL_INTERRUPT_BT_ACTIVE)      
    // Interrupt enable setting
    if(NULL != pstcRtConfig->pstcRtIrqEn)
    {
        if(pstcRtConfig->pstcRtIrqEn->bRtTrigIrq == TRUE)
        {
            pRegBt->STC_f.TGIE = 1u;
        }
        
        if(pstcRtConfig->pstcRtIrqEn->bRtUnderflowIrq == TRUE)
        {
            pRegBt->STC_f.UDIE = 1u;
        }
    }
    // Interrupt callback functions setting
    if(NULL != pstcRtConfig->pstcRtIrqCb)
    {
        pstcBtInternData->pfnBit2IrqCb = pstcRtConfig->pstcRtIrqCb->pfnRtTrigIrqCb;
        pstcBtInternData->pfnBit0IrqCb = pstcRtConfig->pstcRtIrqCb->pfnRtUnderflowIrqCb;
    }
    // NVIC setting
    if(TRUE == pstcRtConfig->bTouchNvic)
    {
        BtInitNvic(pstcBt);
    }
#endif 
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief De-Initialize RT function of BT
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  bTouchNvic       Touch NVIC or not
 **
 ** \retval Ok                    RT function has been de-initialized successfully
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcBt == NULL
 **                               - Other invalid configuration
 ******************************************************************************/                    
en_result_t Bt_Rt_DeInit(volatile stc_btn_t* pstcBt, boolean_t bTouchNvic)
{
    volatile FM_BT_RT_TypeDef* pRegBt = (volatile FM_BT_RT_TypeDef*)pstcBt;
    // Pointer to internal data
    stc_bt_intern_data_t* pstcBtInternData ;
    // Check for NULL pointer
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    // Get pointer to internal data structure ...
    pstcBtInternData = BtGetInternDataPtr( pstcBt ) ;
    
    if(NULL == pstcBtInternData)
    {
        return ErrorInvalidParameter ;
    }
    
    pRegBt->TMCR = 0x0000u;
    pRegBt->TMCR2 = 0x0000u;
    pRegBt->STC = 0x0000u;
    pRegBt->PCSR = 0x0000u;
    
#if defined(PDL_INTERRUPT_BT_ACTIVE)  
    if(TRUE == bTouchNvic)
    {
        BtDeInitNvic(pstcBt);
    }
#endif    
    return Ok;
}
        
        
/**
 ******************************************************************************
 ** \brief Enable Reload timer counting
 **
 ** \param [in]  pstcBt           BT instance
 **
 ** \retval Ok                    Enable RT timer counting successfully
 ** \retval ErrorInvalidParameter pstcBt == NULL
 ******************************************************************************/
en_result_t Bt_Rt_EnableCount(volatile stc_btn_t* pstcBt)
{
    // Check for NULL pointer
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    ((volatile FM_BT_RT_TypeDef*)pstcBt)->TMCR_f.CTEN = 1u;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable Reload timer counting
 **
 ** \param [in]  pstcBt           BT instance
 **
 ** \retval Ok                    Disable RT timer counting successfully
 ** \retval ErrorInvalidParameter pstcBt == NULL
 ******************************************************************************/
en_result_t Bt_Rt_DisableCount(volatile stc_btn_t* pstcBt)
{
    // Check for NULL pointer
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    ((volatile FM_BT_RT_TypeDef*)pstcBt)->TMCR_f.CTEN = 0u;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Start Reload timer by software
 **
 ** \param [in]  pstcBt           BT instance
 **
 ** \retval Ok                    Start Reload timer successfully
 ** \retval ErrorInvalidParameter pstcBt == NULL
 ******************************************************************************/
en_result_t Bt_Rt_EnableSwTrig(volatile stc_btn_t* pstcBt)
{
    // Check for NULL pointer
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    ((volatile FM_BT_RT_TypeDef*)pstcBt)->TMCR_f.STRG = 1u;
    return Ok;
}

#if defined(PDL_INTERRUPT_BT_ACTIVE)   
/**
 ******************************************************************************
 ** \brief Enable Reload timer interrupt
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  enIrqSel         Reload Timer interrupt type
 ** \arg         RtTrigIrq        Reload timer trigger detection flag
 ** \arg         RtUnderflowIrq   Reload timer underflow interrupt flag
 **
 ** \retval Ok                    Enable Reload timer interrupt successfully
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcBt == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Bt_Rt_EnableIrq(volatile stc_btn_t* pstcBt, en_rt_irq_sel_t enIrqSel)
{
    // Check parameter
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    } 

    switch (enIrqSel)
    {
        case RtTrigIrq:
            ((volatile FM_BT_RT_TypeDef*)pstcBt)->STC_f.TGIE = 1u;
            break;
        case RtUnderflowIrq:
            ((volatile FM_BT_RT_TypeDef*)pstcBt)->STC_f.UDIE = 1u;
            break;
        default:
            return ErrorInvalidParameter ;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable Reload timer interrupt
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  enIrqSel         Reload Timer interrupt type
 ** \arg         RtTrigIrq        Reload timer trigger detection flag
 ** \arg         RtUnderflowIrq   Reload timer underflow interrupt flag
 **
 ** \retval Ok                    Disable Reload timer interrupt successfully
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcBt == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Bt_Rt_DisableIrq(volatile stc_btn_t* pstcBt, en_rt_irq_sel_t enIrqSel)
{
    // Check parameter
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    } 
    
    switch (enIrqSel)
    {
        case RtTrigIrq:
            ((volatile FM_BT_RT_TypeDef*)pstcBt)->STC_f.TGIE = 0u;
            break;
        case RtUnderflowIrq:
            ((volatile FM_BT_RT_TypeDef*)pstcBt)->STC_f.UDIE = 0u;
            break;
        default:
            return ErrorInvalidParameter ;
    }
    
    return Ok;
}

#endif

/**
 ******************************************************************************
 ** \brief Get interrupt flag of Reload timer 
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  enIrqSel         Reload timer interrupt type
 ** \arg         RtTrigIrq        Reload timer trigger detection flag
 ** \arg         RtUnderflowIrq   Reload timer underflow interrupt flag
 **
 ** \retval PdlSet                Interrupt flag is set
 ** \retval PdlClr                Interrupt flag is clear
 ******************************************************************************/
en_irq_flag_t Bt_Rt_GetIrqFlag(volatile stc_btn_t* pstcBt, en_rt_irq_sel_t enIrqSel)
{
    en_irq_flag_t enFlag = PdlClr; 
    
    switch (enIrqSel)
    {
        case RtTrigIrq:
            enFlag = ((volatile FM_BT_RT_TypeDef*)pstcBt)->STC_f.TGIR ? PdlSet : PdlClr;
            break;
        case RtUnderflowIrq:
            enFlag = ((volatile FM_BT_RT_TypeDef*)pstcBt)->STC_f.UDIR ? PdlSet : PdlClr;
            break;
        default:
            break;
    }
    
    return enFlag;
}

/**
 ******************************************************************************
 ** \brief Clear interrupt flag of Reload timer 
 **
 ** \param [in]  pstcBt             BT instance
 ** \param [in]  enIrqSel           Reload timer interrupt type
 ** \arg         RtTrigIrq          Reload timer trigger detection flag
 ** \arg         RtUnderflowIrq     Reload timer underflow interrupt flag
 **
 ** \retval ErrorInvalidParameter   If one of following cases match: 
 **                                 - pstcBt == NULL
 **                                 - enIntType > RtUnderflowIrq
 ** \retval Ok                      Clear interrupt flag successfully
 ******************************************************************************/
en_result_t Bt_Rt_ClrIrqFlag(volatile stc_btn_t* pstcBt, en_rt_irq_sel_t enIrqSel)
{
    if(NULL == pstcBt)
    {
        return ErrorInvalidParameter;
    }
    
    switch (enIrqSel)
    {
        case RtTrigIrq:
            ((volatile FM_BT_RT_TypeDef*)pstcBt)->STC_f.TGIR = 0u;
            break;
        case RtUnderflowIrq:
            ((volatile FM_BT_RT_TypeDef*)pstcBt)->STC_f.UDIR = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}
/**
 ******************************************************************************
 ** \brief Write count cycle of Reload timer 
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  u16Val           Cycle value
 **
 ** \retval Ok                    Write count cycle successfully
 ** \retval ErrorInvalidParameter pstcBt == NULL
 ******************************************************************************/
en_result_t Bt_Rt_WriteCycleVal(volatile stc_btn_t* pstcBt, uint16_t u16Val)
{
    // Check parameter
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    ((volatile FM_BT_RT_TypeDef*)pstcBt)->PCSR = u16Val;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Read current count value of Reload timer 
 **
 ** \param [in]  pstcBt           BT instance
 **
 ** \retval Current count value
 ******************************************************************************/
uint16_t Bt_Rt_ReadCurCnt(volatile stc_btn_t* pstcBt)
{ 
    return ((volatile FM_BT_RT_TypeDef*)pstcBt)->TMR;
}
#endif

#if (PDL_PERIPHERAL_ENABLE_BT_PWC_MODE == PDL_ON)
/**
 ******************************************************************************
 ** \brief Initialize PWC function of BT
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  pstcPwcConfig    Pointer to PWC configuration
 **
 ** \retval Ok                    PWC function has been configured successfully
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcBt == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Bt_Pwc_Init(volatile stc_btn_t* pstcBt, const stc_bt_pwc_config_t* pstcPwcConfig)
{
    volatile FM_BT_PWC_TypeDef* pRegBt = (volatile FM_BT_PWC_TypeDef*) pstcBt;
    // Pointer to internal data
    stc_bt_intern_data_t* pstcBtInternData ;
    uint16_t u16Pres;
    // Check for NULL pointer
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    // Get pointer to internal data structure ...
    pstcBtInternData = BtGetInternDataPtr( pstcBt ) ;
    
    if(NULL == pstcBtInternData)
    {
        return ErrorInvalidParameter ;
    }
    
    // Reset Base Timer mode
    pRegBt->TMCR_f.FMD = BtResetMode;
    // Set Base Timer to Reload Timer mode
    pRegBt->TMCR_f.FMD = BtPwcMode;
    
    // Clock prescaler setting
    u16Pres = pstcPwcConfig->enPres; // workaround for MISRA-C 2004 rule 12.7
    switch (pstcPwcConfig->enPres)
    {
        case PwcPresNone:
        case PwcPres1Div4:
        case PwcPres1Div16:
        case PwcPres1Div128:
        case PwcPres1Div256:
            pRegBt->TMCR2_f.CKS3 = 0u;
            pRegBt->TMCR_f.CKS = u16Pres;
            break;
        case PwcPres1Div512:
        case PwcPres1Div1024:
        case PwcPres1Div2048:
            pRegBt->TMCR2_f.CKS3 = 1u;
            pRegBt->TMCR_f.CKS = (u16Pres & 0x07u);
            break;
        default:
            return ErrorInvalidParameter ;
    }
    // Timer size setting
    switch (pstcPwcConfig->enSize)
    {
        case PwcSize16Bit:
            pRegBt->TMCR_f.T32 = 0u;
            break;
        case PwcSize32Bit:
            pRegBt->TMCR_f.T32 = 1u;
            break;
        default:
            return ErrorInvalidParameter ;
    }
    // Measurement mode setting
    switch (pstcPwcConfig->enMeasureEdge)
    {
        case PwcMeasureRisingToFalling:
            pRegBt->TMCR_f.EGS = 0u;
            break;  
        case PwcMeasureRisingToRising:
            pRegBt->TMCR_f.EGS = 1u;
            break;
        case PwcMeasureFallingToFalling:
            pRegBt->TMCR_f.EGS = 2u;
            break;
        case PwcMeasureEitherToEither:
            pRegBt->TMCR_f.EGS = 3u;
            break;
        case PwcMeasureFallingToRising:
            pRegBt->TMCR_f.EGS = 4u;
            break;
        default:
            return ErrorInvalidParameter ;
    }
    // Mode setting
    switch (pstcPwcConfig->enMode)
    {
        case PwcContinuous:
            pRegBt->TMCR_f.MDSE = 0u;
            break;
        case PwcOneshot:
            pRegBt->TMCR_f.MDSE = 1u;
            break;
        default:
            return ErrorInvalidParameter ;
    }
    
#if defined(PDL_INTERRUPT_BT_ACTIVE)      
    // Interrupt enable setting
    if(NULL != pstcPwcConfig->pstcPwcIrqEn)
    {
        if(pstcPwcConfig->pstcPwcIrqEn->bPwcMeasureCompleteIrq == TRUE)
        {
            pRegBt->STC_f.EDIE = 1u;
        }
        
        if(pstcPwcConfig->pstcPwcIrqEn->bPwcMeasureOverflowIrq == TRUE)
        {
            pRegBt->STC_f.OVIE = 1u;
        }
    }
    
    // Interrupt callback functions
    if (NULL != pstcPwcConfig->pstcPwcIrqCb)
    {
        pstcBtInternData->pfnBit2IrqCb = pstcPwcConfig->pstcPwcIrqCb->pfnPwcMeasureCompleteIrqCb;
        pstcBtInternData->pfnBit0IrqCb = pstcPwcConfig->pstcPwcIrqCb->pfnPwcMeasureOverflowIrqCb;
    }
    
    // NVIC setting
    if(pstcPwcConfig->bTouchNvic == TRUE)
    {
        BtInitNvic(pstcBt);
    }
#endif     
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief De-Initialize PWC function of BT
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  bTouchNvic       Touch NVIC or not
 **
 ** \retval Ok                    PWC function has been de-initialized successfully
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcBt == NULL
 **                               - Other invalid configuration
 ******************************************************************************/                    
en_result_t Bt_Pwc_DeInit(volatile stc_btn_t* pstcBt, boolean_t bTouchNvic)
{
    volatile FM_BT_PWC_TypeDef* pRegBt = (volatile FM_BT_PWC_TypeDef*)pstcBt;
    // Pointer to internal data
    stc_bt_intern_data_t* pstcBtInternData ;
    // Check for NULL pointer
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    // Get pointer to internal data structure ...
    pstcBtInternData = BtGetInternDataPtr( pstcBt ) ;
    
    if(NULL == pstcBtInternData)
    {
        return ErrorInvalidParameter ;
    }
    
    pRegBt->TMCR = 0x0000u;
    pRegBt->TMCR2 = 0x0000u;
    pRegBt->STC = 0x0000u;
    
#if defined(PDL_INTERRUPT_BT_ACTIVE)  
    if(TRUE == bTouchNvic)
    {
        BtDeInitNvic(pstcBt);
    }
#endif    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Enable PWC timer counting
 **
 ** \param [in]  pstcBt           BT instance
 **
 ** \retval Ok                    Enable PWC timer counting successfully
 ** \retval ErrorInvalidParameter pstcBt == NULL
 ******************************************************************************/
en_result_t Bt_Pwc_EnableCount(volatile stc_btn_t* pstcBt)
{
    // Check for NULL pointer
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    ((volatile FM_BT_PWC_TypeDef*)pstcBt)->TMCR_f.CTEN = 1u;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable PWC timer counting
 **
 ** \param [in]  pstcBt           BT instance
 **
 ** \retval Ok                    Disable PWC timer counting successfully
 ** \retval ErrorInvalidParameter pstcBt == NULL
 ******************************************************************************/
en_result_t Bt_Pwc_DisableCount(volatile stc_btn_t* pstcBt)
{
    // Check for NULL pointer
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    ((volatile FM_BT_PWC_TypeDef*)pstcBt)->TMCR_f.CTEN = 0u;
    return Ok;
}

#if defined(PDL_INTERRUPT_BT_ACTIVE)   
/**
 ******************************************************************************
 ** \brief Enable PWC timer interrupt
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  enIrqSel         PWC timer interrupt type
 ** \arg         PwcMeasureCompleteIrq   PWC measure complete interrupt 
 ** \arg         PwcMeasureOverflowIrq   PWC overflow interrupt
 **
 ** \retval Ok                    Enable PWC timer interrupt successfully
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcBt == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Bt_Pwc_EnableIrq(volatile stc_btn_t* pstcBt, en_pwc_irq_sel_t enIrqSel)
{
    // Check parameter
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  

    switch (enIrqSel)
    {
        case PwcMeasureCompleteIrq:
            ((volatile FM_BT_PWC_TypeDef*)pstcBt)->STC_f.EDIE = 1u;
            break;
        case PwcMeasureOverflowIrq:
            ((volatile FM_BT_PWC_TypeDef*)pstcBt)->STC_f.OVIE = 1u;
            break;
        default:    
            return ErrorInvalidParameter ;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Disable PWC timer interrupt
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  enIrqSel         PWC timer interrupt type
 ** \arg         PwcMeasureCompleteIrq   PWC measure complete interrupt 
 ** \arg         PwcMeasureOverflowIrq   PWC overflow interrupt
 **
 ** \retval Ok                    Disable PWC timer interrupt successfully
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcBt == NULL
 **                               - Other invalid configuration
 ******************************************************************************/
en_result_t Bt_Pwc_DisableIrq(volatile stc_btn_t* pstcBt, en_pwc_irq_sel_t enIrqSel)
{
    // Check parameter
    if (NULL == pstcBt)
    {
        return ErrorInvalidParameter ;
    }  
    
    switch (enIrqSel)
    {
        case PwcMeasureCompleteIrq:
            ((volatile FM_BT_PWC_TypeDef*)pstcBt)->STC_f.EDIE = 0u;
            break;
        case PwcMeasureOverflowIrq:
            ((volatile FM_BT_PWC_TypeDef*)pstcBt)->STC_f.OVIE = 0u;
            break;
        default:    
            return ErrorInvalidParameter ;
    }
    
    return Ok;
}
#endif

/**
 ******************************************************************************
 ** \brief Get interrupt flag of PWC timer 
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  enIrqSel         PWC timer interrupt type
 ** \arg         PwcMeasureCompleteIrq   PWC measure complete interrupt 
 ** \arg         PwcMeasureOverflowIrq   PWC overflow interrupt
 **
 ** \retval PdlSet                Interrupt flag is set
 ** \retval PdlClr                Interrupt flag is clear
 ******************************************************************************/
en_irq_flag_t Bt_Pwc_GetIrqFlag(volatile stc_btn_t* pstcBt, en_pwc_irq_sel_t enIrqSel)
{
    en_irq_flag_t enFlag = PdlClr; 
    
    switch (enIrqSel)
    {
        case PwcMeasureCompleteIrq:
            enFlag = ((volatile FM_BT_PWC_TypeDef*)pstcBt)->STC_f.EDIR ? PdlSet : PdlClr;
            break;
        case PwcMeasureOverflowIrq:
            enFlag = ((volatile FM_BT_PWC_TypeDef*)pstcBt)->STC_f.OVIR ? PdlSet : PdlClr;
            break;
        default:
            break;
    }
    
    return enFlag;
}

/**
 ******************************************************************************
 ** \brief Clear interrupt flag of PWC timer 
 **
 ** \param [in]  pstcBt           BT instance
 ** \param [in]  enIrqSel         PWC timer interrupt type
 ** \arg         PwcMeasureCompleteIrq   PWC measure complete interrupt 
 ** \arg         PwcMeasureOverflowIrq   PWC overflow interrupt
 **
 ** \retval ErrorInvalidParameter If one of following cases match:
 **                               - pstcBt == NULL
 **                               - enIntType > PwcMeasureOverflowIrq
 ** \retval Ok                    Clear interrupt flag successfully
 ******************************************************************************/
en_result_t Bt_Pwc_ClrIrqFlag(volatile stc_btn_t* pstcBt, en_pwc_irq_sel_t enIrqSel)
{
    if(NULL == pstcBt) 
    {
        return ErrorInvalidParameter;
    }
  
    switch (enIrqSel)
    {
        case PwcMeasureCompleteIrq:
            break;
        case PwcMeasureOverflowIrq:
            ((volatile FM_BT_PWC_TypeDef*)pstcBt)->STC_f.OVIR = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get error flag of PWC timer
 **
 ** If the measured data is not read out when the next data is coming,
 ** the error flag will be set.
 **
 ** \param [in]  pstcBt           BT instance
 **
 ** \retval PdlSet                Error flag is set
 ** \retval PdlClr                Error flag is clear
 ******************************************************************************/
en_stat_flag_t Bt_Pwc_GetErrorFlag(volatile stc_btn_t* pstcBt)
{
    en_irq_flag_t enFlag; 
    
    enFlag = ((volatile FM_BT_PWC_TypeDef*)pstcBt)->STC_f.ERR ? PdlSet : PdlClr;
    
    return enFlag;
}

/**
 ******************************************************************************
 ** \brief Get 16 bits measure data of PWC timer
 **
 ** This function reads measure data in 16-bit timer mode.
 **
 ** \param [in]  pstcBt           BT instance
 **
 ** \retval Measure data       
 ******************************************************************************/
uint16_t Bt_Pwc_Get16BitMeasureData(volatile stc_btn_t* pstcBt)
{ 
    uint16_t u16RegDtbf;
    u16RegDtbf = ((volatile FM_BT_PWC_TypeDef*)pstcBt)->DTBF;
    return u16RegDtbf;
}

/**
 ******************************************************************************
 ** \brief Get 32 bits measure data of PWC timer
 **
 ** This function reads measure data in 32-bit timer mode, the input BT instance
 ** should point to register base address of even channel. 
 **
 ** \param [in]  pstcBt           BT instance
 **
 ** \retval Measure data 
 ******************************************************************************/
uint32_t Bt_Pwc_Get32BitMeasureData(volatile stc_btn_t* pstcBt)
{ 
    uint16_t u16RegLow, u16RegHigh;

#if (defined(FM_BTIOSEL47_AVAILABLE))    
    if((&BT1 == pstcBt) || (&BT3 == pstcBt) || (&BT5 == pstcBt) || (&BT7 == pstcBt))
    {
        return 0ul;
    }
#else
    if((&BT1 == pstcBt) || (&BT3 == pstcBt))
    {
        return 0ul;
    }  
#endif
    u16RegLow = ((volatile FM_BT_PWC_TypeDef*)pstcBt)->DTBF;
    u16RegHigh = ((volatile FM_BT_PWC_TypeDef*)((volatile uint8_t*)pstcBt + 0x40u))->DTBF;
    return (((uint32_t)u16RegHigh<<16) | (uint32_t)u16RegLow);
}
#endif

#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)
/**
 ******************************************************************************
 ** \brief Set the Simultaneous Start register of Base timer
 **
 ** This function is only valid in BTIO mode 5 and mode 6, otherwise don't 
 ** use this function!
 **
 ** \param [in]  u16Value           Bit0 ----- Ch.0
 **                                 Bit1 ----- Ch.1
 **                                 ...
 **                                 Bit15 ----- Ch.15
 **
 ** \retval None
 ******************************************************************************/
void Bt_SetSimultaneousStart(uint16_t u16Value)
{
    FM_SBSSR->BTSSSR = u16Value;
}
#endif

#endif
