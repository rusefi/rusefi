/*******************************************************************************
* \file             exint.c
* 
* \version          1.30
*                       
* \brief            External Interrupts driver
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
#include "exint/exint.h"

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/
#if (defined(PDL_PERIPHERAL_EXINT_ACTIVE))
stc_exint_intern_data_t stcExintInternData;

const uint8_t cau8ExintReferenceLut[PDL_EXINT_CHANNELS + 1] = 
{
#if (PDL_INTERRUPT_ENABLE_EXINT0 == PDL_ON)
  0u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT1 == PDL_ON)
  1u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT2 == PDL_ON)
  2u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT3 == PDL_ON)
  3u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT4 == PDL_ON)
  4u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT5 == PDL_ON)
  5u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT6 == PDL_ON)
  6u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT7 == PDL_ON)
  7u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT8 == PDL_ON)
  8u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT9 == PDL_ON)
  9u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT10 == PDL_ON)
  10u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT11 == PDL_ON)
  11u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT12 == PDL_ON)
  12u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT13 == PDL_ON)
  13u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT14 == PDL_ON)
  14u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT15 == PDL_ON)
  15u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT16 == PDL_ON)
  16u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT17 == PDL_ON)
  17u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT18 == PDL_ON)
  18u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT19 == PDL_ON)
  19u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT20 == PDL_ON)
  20u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT21 == PDL_ON)
  21u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT22 == PDL_ON)
  22u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT23 == PDL_ON)
  23u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT24 == PDL_ON)
  24u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT25 == PDL_ON)
  25u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT26 == PDL_ON)
  26u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT27 == PDL_ON)
  27u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT28 == PDL_ON)
  28u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT29 == PDL_ON)
  29u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT30 == PDL_ON)
  30u,
#endif
#if (PDL_INTERRUPT_ENABLE_EXINT31 == PDL_ON)
  31u,
#endif
  255u   // End of table
};

#endif

#if (defined(PDL_PERIPHERAL_NMI_ACTIVE))
stc_exint_nmi_intern_data_t stcExintNMIInternData;
#endif

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/

#if (defined(PDL_PERIPHERAL_EXINT_ACTIVE))

/**
 ******************************************************************************
 ** \brief Return the internal reference index of an Exint channel
 **
 ** \param u8Channel   Exint channel number
 **
 ** \return uint8_t    Reference index (255 if not found or u8Channel > 31)
 **
 ******************************************************************************/
static uint8_t ExintGetInternDataIdx(uint8_t u8Channel)
{
  uint8_t u8Index;
  
  if (u8Channel > 31u)
  {
     return 255u;
  }
  
  for (u8Index = 0u; u8Index < PDL_EXINT_CHANNELS; u8Index++)
  {
    if (u8Channel == cau8ExintReferenceLut[u8Index])
    {
      return u8Index;
    }
  }
        
  return 255u;  // Should never happen ...
}

/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/

/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief Interrupt handlers for External interrupts 0
 **
 ** This function clears the cause bit and calls the callback function, if
 ** specified.
 ******************************************************************************/
void Exint_IrqHandler(uint8_t u8Channel)
{
  uint8_t u8Reference;
  
  u8Reference = ExintGetInternDataIdx(u8Channel);
  
  FM_EXTI->EICL &= (0xFFFFFFFFu ^ (1ul << u8Channel));
  
  if (0u != stcExintInternData.apfnExintCallback[u8Reference])
  {
    /* [andreika]: pass channel as parameter */
    stcExintInternData.apfnExintCallback[u8Reference](u8Channel);
  }
} // Exint_IrqHandler

/**
 ******************************************************************************
 ** \brief Init External Interrupts
 **
 ** This function initializes the external interrupts according the given
 ** configuration.
 **
 ** \param [in]  pstcConfig        EXINT configuration parameters
 ** 
 ** \retval Ok                     EXINT initialized
 ** \retval ErrorInvalidParameter  pstcConfig == NULL or Illegal mode
 ******************************************************************************/
en_result_t Exint_Init( const stc_exint_config_t* pstcConfig)
{
  uint8_t  u8Index;
  uint8_t  u8Reference;
  uint32_t u32Elvr;
  uint32_t u32Elvr1;
#if (PDL_MCU_TYPE == PDL_FM4_TYPE5) ||  (PDL_MCU_TYPE == PDL_FM4_TYPE6) || (PDL_MCU_TYPE == PDL_FM0P_TYPE3) 
  uint32_t u32Elvr2;
#endif  
  uint32_t u32Enir;
  
  u32Elvr  = 0u;
  u32Elvr1 = 0u;
#if (PDL_MCU_TYPE == PDL_FM4_TYPE5) ||  (PDL_MCU_TYPE == PDL_FM4_TYPE6) || (PDL_MCU_TYPE == PDL_FM0P_TYPE3)    
  u32Elvr2 = 0u;
#endif  
  u32Enir  = 0u;
  
  if ( NULL == pstcConfig )
  {
    return ErrorInvalidParameter;
  }
  
  for (u8Index = 0u; u8Index < PDL_EXINT_INSTANCE_COUNT; u8Index++)
  {
    u8Reference = cau8ExintReferenceLut[u8Index];
    
    if(1u == pstcConfig->abEnable[u8Index])
    {
        if (u8Reference < 16u)
        {
          switch (pstcConfig->aenLevel[u8Index])
          {
            case ExIntLowLevel:
              break;        // bits remain 2'b00
            case ExIntHighLevel:
              u32Elvr |= 1u << (2u * u8Reference);
              break;
            case ExIntRisingEdge:
              u32Elvr |= 2u << (2u * u8Reference);
              break;
            case ExIntFallingEdge:
              u32Elvr |= 3u << (2u * u8Reference);
              break;
          #if (PDL_MCU_TYPE == PDL_FM4_TYPE5) ||  (PDL_MCU_TYPE == PDL_FM4_TYPE6) || (PDL_MCU_TYPE == PDL_FM0P_TYPE3)    
            case ExIntBothEdge:
              u32Elvr2 |= 1u << (u8Reference);
              break;
          #endif    
            default:
              return ErrorInvalidParameter;
          }
        }
        else
        {
          switch (pstcConfig->aenLevel[u8Index])
          {
            case ExIntLowLevel:
              break;        // bits remain 2'b00
            case ExIntHighLevel:
              u32Elvr1 |= 1u << (2u * (u8Reference - 16u));
              break;
            case ExIntRisingEdge:
              u32Elvr1 |= 2u << (2u * (u8Reference - 16u));
              break;
            case ExIntFallingEdge:
              u32Elvr1 |= 3u << (2u * (u8Reference - 16u));
              break;
          #if (PDL_MCU_TYPE == PDL_FM4_TYPE5) ||  (PDL_MCU_TYPE == PDL_FM4_TYPE6) || (PDL_MCU_TYPE == PDL_FM0P_TYPE3) 
            case ExIntBothEdge:
              u32Elvr2 |= 1u << (u8Reference);
              break;
          #endif    
            default:
              return ErrorInvalidParameter;
          }
        }
        u32Enir |= 1ul << u8Reference;
    }
    else
    {
        u32Enir &= ~(1ul << u8Reference);
    }
    stcExintInternData.apfnExintCallback[u8Index] = pstcConfig->apfnExintCallback[u8Index];
  }
  
  // update hardware
  FM_EXTI->ELVR  = u32Elvr;
  FM_EXTI->ELVR1 = u32Elvr1;
#if (PDL_MCU_TYPE == PDL_FM4_TYPE5) ||  (PDL_MCU_TYPE == PDL_FM4_TYPE6) || (PDL_MCU_TYPE == PDL_FM0P_TYPE3)   
  FM_EXTI->ELVR2 = u32Elvr2;
#endif  
  FM_EXTI->EICL  = 0u;         // Clear all possible interrupts due to level change
  FM_EXTI->ENIR  = u32Enir;
  
  if(TRUE == pstcConfig->bTouchNvic)
  {
#if (PDL_MCU_CORE == PDL_FM0P_CORE)
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE3)
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT0) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT1)
    NVIC_ClearPendingIRQ(EXINT0_1_IRQn); 
    NVIC_SetPriority(EXINT0_1_IRQn, PDL_IRQ_LEVEL_EXINT0_1);
    NVIC_EnableIRQ(EXINT0_1_IRQn);  
  #endif
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT2) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT3)
    NVIC_ClearPendingIRQ(EXINT2_3_IRQn); 
    NVIC_SetPriority(EXINT2_3_IRQn, PDL_IRQ_LEVEL_EXINT2_3);
    NVIC_EnableIRQ(EXINT2_3_IRQn);  
  #endif
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT4) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT5)
    NVIC_ClearPendingIRQ(EXINT4_5_IRQn); 
    NVIC_SetPriority(EXINT4_5_IRQn, PDL_IRQ_LEVEL_EXINT4_5);
    NVIC_EnableIRQ(EXINT4_5_IRQn);  
  #endif
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT6) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT7)
    NVIC_ClearPendingIRQ(EXINT6_7_IRQn); 
    NVIC_SetPriority(EXINT6_7_IRQn, PDL_IRQ_LEVEL_EXINT6_7);
    NVIC_EnableIRQ(EXINT6_7_IRQn);  
  #endif
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT8) 
    NVIC_ClearPendingIRQ(EXINT8_IRQn); 
    NVIC_SetPriority(EXINT8_IRQn, PDL_IRQ_LEVEL_EXINT8);
    NVIC_EnableIRQ(EXINT8_IRQn);  
  #endif
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT12) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT13) 
    NVIC_ClearPendingIRQ(EXINT12_13_IRQn); 
    NVIC_SetPriority(EXINT12_13_IRQn, PDL_IRQ_LEVEL_EXINT12_13);
    NVIC_EnableIRQ(EXINT12_13_IRQn);  
  #endif
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT15)
    NVIC_ClearPendingIRQ(EXINT15_IRQn); 
    NVIC_SetPriority(EXINT15_IRQn, PDL_IRQ_LEVEL_EXINT15);
    NVIC_EnableIRQ(EXINT15_IRQn);  
  #endif  
#else
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT0) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT1) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT2) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT3) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT4) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT5) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT6) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT7)
    NVIC_ClearPendingIRQ(EXINT0_7_IRQn); 
    NVIC_SetPriority(EXINT0_7_IRQn, PDL_IRQ_LEVEL_EXINT0_7);
    NVIC_EnableIRQ(EXINT0_7_IRQn);
  #endif  
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT8)  || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT9)  || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT10) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT11) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT12) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT13) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT14) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT15) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT16) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT17) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT18) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT19) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT20) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT21) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT22) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT23) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT24) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT25) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT26) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT27) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT28) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT29) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT30) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT31) 
    NVIC_ClearPendingIRQ(EXINT8_23_IRQn); 
    NVIC_SetPriority(EXINT8_23_IRQn, PDL_IRQ_LEVEL_EXINT8_23);
    NVIC_EnableIRQ(EXINT8_23_IRQn);     
  #endif
#endif    
#elif (PDL_MCU_CORE == PDL_FM3_CORE)
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT0) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT1) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT2) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT3) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT4) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT5) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT6) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT7)
    NVIC_ClearPendingIRQ(EXINT0_7_IRQn); 
    NVIC_SetPriority(EXINT0_7_IRQn, PDL_IRQ_LEVEL_EXINT0_7);
    NVIC_EnableIRQ(EXINT0_7_IRQn);
  #endif  
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT8)  || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT9)  || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT10) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT11) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT12) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT13) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT14) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT15) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT16) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT17) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT18) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT19) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT20) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT21) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT22) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT23) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT24) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT25) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT26) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT27) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT28) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT29) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT30) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT31) 
    NVIC_ClearPendingIRQ(EXINT8_31_IRQn); 
    NVIC_SetPriority(EXINT8_31_IRQn, PDL_IRQ_LEVEL_EXINT8_31);
    NVIC_EnableIRQ(EXINT8_31_IRQn);          
  #endif        
#else  
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT0)
    NVIC_ClearPendingIRQ(EXINT0_IRQn); 
    NVIC_SetPriority(EXINT0_IRQn, PDL_IRQ_LEVEL_EXINT0);
    NVIC_EnableIRQ(EXINT0_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT1)
    NVIC_ClearPendingIRQ(EXINT1_IRQn); 
    NVIC_SetPriority(EXINT1_IRQn, PDL_IRQ_LEVEL_EXINT1);
    NVIC_EnableIRQ(EXINT1_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT2)
    NVIC_ClearPendingIRQ(EXINT2_IRQn);
    NVIC_SetPriority(EXINT2_IRQn, PDL_IRQ_LEVEL_EXINT2);
    NVIC_EnableIRQ(EXINT2_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT3)
    NVIC_ClearPendingIRQ(EXINT3_IRQn);
    NVIC_SetPriority(EXINT3_IRQn, PDL_IRQ_LEVEL_EXINT3);
    NVIC_EnableIRQ(EXINT3_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT4)
    NVIC_ClearPendingIRQ(EXINT4_IRQn);
    NVIC_SetPriority(EXINT4_IRQn, PDL_IRQ_LEVEL_EXINT4);
    NVIC_EnableIRQ(EXINT4_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT5)
    NVIC_ClearPendingIRQ(EXINT5_IRQn);
    NVIC_SetPriority(EXINT5_IRQn, PDL_IRQ_LEVEL_EXINT5);
    NVIC_EnableIRQ(EXINT5_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT6)
    NVIC_ClearPendingIRQ(EXINT6_IRQn);
    NVIC_SetPriority(EXINT6_IRQn, PDL_IRQ_LEVEL_EXINT6);
    NVIC_EnableIRQ(EXINT6_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT7)
    NVIC_ClearPendingIRQ(EXINT7_IRQn);
    NVIC_SetPriority(EXINT7_IRQn, PDL_IRQ_LEVEL_EXINT7);
    NVIC_EnableIRQ(EXINT7_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT8)
    NVIC_ClearPendingIRQ(EXINT8_IRQn);
    NVIC_SetPriority(EXINT8_IRQn, PDL_IRQ_LEVEL_EXINT8);
    NVIC_EnableIRQ(EXINT8_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT9)
    NVIC_ClearPendingIRQ(EXINT9_IRQn);
    NVIC_SetPriority(EXINT9_IRQn, PDL_IRQ_LEVEL_EXINT9);
    NVIC_EnableIRQ(EXINT9_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT10)
    NVIC_ClearPendingIRQ(EXINT10_IRQn);
    NVIC_SetPriority(EXINT10_IRQn, PDL_IRQ_LEVEL_EXINT10);
    NVIC_EnableIRQ(EXINT10_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT11)
    NVIC_ClearPendingIRQ(EXINT11_IRQn);
    NVIC_SetPriority(EXINT11_IRQn, PDL_IRQ_LEVEL_EXINT11);
    NVIC_EnableIRQ(EXINT11_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT12)
    NVIC_ClearPendingIRQ(EXINT12_IRQn);
    NVIC_SetPriority(EXINT12_IRQn, PDL_IRQ_LEVEL_EXINT12);
    NVIC_EnableIRQ(EXINT12_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT13)
    NVIC_ClearPendingIRQ(EXINT13_IRQn);
    NVIC_SetPriority(EXINT13_IRQn, PDL_IRQ_LEVEL_EXINT13);
    NVIC_EnableIRQ(EXINT13_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT14)
    NVIC_ClearPendingIRQ(EXINT14_IRQn);
    NVIC_SetPriority(EXINT14_IRQn, PDL_IRQ_LEVEL_EXINT14);
    NVIC_EnableIRQ(EXINT14_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT15)
    NVIC_ClearPendingIRQ(EXINT15_IRQn);
    NVIC_SetPriority(EXINT15_IRQn, PDL_IRQ_LEVEL_EXINT15);
    NVIC_EnableIRQ(EXINT15_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT16) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT17) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT18) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT19)
    NVIC_ClearPendingIRQ(EXINT16_19_IRQn);
    NVIC_SetPriority(EXINT16_19_IRQn, PDL_IRQ_LEVEL_EXINT16_17_18_19);
    NVIC_EnableIRQ(EXINT16_19_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT20) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT21) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT22) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT23)
    NVIC_ClearPendingIRQ(EXINT20_23_IRQn);
    NVIC_SetPriority(EXINT20_23_IRQn, PDL_IRQ_LEVEL_EXINT20_21_22_23);
    NVIC_EnableIRQ(EXINT20_23_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT24) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT25) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT26) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT27)
    NVIC_ClearPendingIRQ(EXINT24_27_IRQn);
    NVIC_SetPriority(EXINT24_27_IRQn, PDL_IRQ_LEVEL_EXINT24_25_26_27);
    NVIC_EnableIRQ(EXINT24_27_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT28) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT29) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT30) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT31)
    NVIC_ClearPendingIRQ(EXINT28_31_IRQn);
    NVIC_SetPriority(EXINT28_31_IRQn, PDL_IRQ_LEVEL_EXINT28_29_30_31);
    NVIC_EnableIRQ(EXINT28_31_IRQn);
#endif
#endif
  }
  
  return Ok;
} // Exint_DisableChannel

/**
 ******************************************************************************
 ** \brief Init External Interrupts
 **
 ** This function de-initializes all external interrupts.
 ** 
 ** \retval Ok                     EXINT sucessful disabled
 ******************************************************************************/
en_result_t Exint_DeInit(void)
{
  FM_EXTI->ENIR = 0;   // Disable all channels

#if (PDL_MCU_CORE == PDL_FM0P_CORE)
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE3)
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT0) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT1)
  NVIC_SetPriority(EXINT0_1_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT0_1_IRQn);
  #endif
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT2) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT3)
  NVIC_SetPriority(EXINT2_3_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT2_3_IRQn);
  #endif
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT4) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT5)
  NVIC_SetPriority(EXINT4_5_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT4_5_IRQn);
  #endif
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT6) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT7)
  NVIC_SetPriority(EXINT6_7_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT6_7_IRQn);
  #endif
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT8) 
  NVIC_SetPriority(EXINT8_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT8_IRQn);
  #endif
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT12) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT13) 
  NVIC_SetPriority(EXINT12_13_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT12_13_IRQn);
  #endif
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT15)
  NVIC_SetPriority(EXINT15_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT15_IRQn);
  #endif
#else
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT0) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT1) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT2) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT3) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT4) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT5) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT6) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT7)
  NVIC_SetPriority(EXINT0_7_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT0_7_IRQn);
  #endif
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT8) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT9) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT10) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT11) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT12) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT13) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT14) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT15) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT16) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT17) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT18) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT19) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT20) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT21) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT22) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT23) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT24) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT25) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT26) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT27) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT28) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT29) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT30) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT31)
  NVIC_SetPriority(EXINT8_23_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT8_23_IRQn);      
  #endif          
#endif  
#elif (PDL_MCU_CORE == PDL_FM3_CORE)
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT0) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT1) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT2) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT3) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT4) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT5) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT6) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT7)
  NVIC_SetPriority(EXINT0_7_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT0_7_IRQn);
  #endif
  #if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT8) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT9) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT10) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT11) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT12) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT13) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT14) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT15) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT16) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT17) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT18) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT19) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT20) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT21) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT22) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT23) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT24) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT25) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT26) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT27) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT28) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT29) || \
      (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT30) || (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT31)
  NVIC_SetPriority(EXINT8_31_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT8_31_IRQn);      
  #endif        
#else    
  // Disable interrupts
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT0)
  NVIC_SetPriority(EXINT0_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT0_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT1)
  NVIC_SetPriority(EXINT1_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT1_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT2)
  NVIC_SetPriority(EXINT2_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT2_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT3)
  NVIC_SetPriority(EXINT3_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT3_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT4)
  NVIC_SetPriority(EXINT4_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT4_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT5)
  NVIC_SetPriority(EXINT5_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT5_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT6)
  NVIC_SetPriority(EXINT6_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT6_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT7)
  NVIC_SetPriority(EXINT7_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT7_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT8)
  NVIC_SetPriority(EXINT8_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT8_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT9)
  NVIC_SetPriority(EXINT9_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT9_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT10)
  NVIC_SetPriority(EXINT10_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT10_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT11)
  NVIC_SetPriority(EXINT11_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT11_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT12)
  NVIC_SetPriority(EXINT12_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT12_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT13)
  NVIC_SetPriority(EXINT13_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT13_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT14)
  NVIC_SetPriority(EXINT14_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT14_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT15)
  NVIC_SetPriority(EXINT15_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  NVIC_DisableIRQ(EXINT15_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT16) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT17) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT18) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT19)
  NVIC_SetPriority(EXINT16_19_IRQn, PDL_IRQ_LEVEL_EXINT16_17_18_19);
  NVIC_DisableIRQ(EXINT16_19_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT20) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT21) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT22) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT23)
  NVIC_SetPriority(EXINT20_23_IRQn, PDL_IRQ_LEVEL_EXINT20_21_22_23);
  NVIC_DisableIRQ(EXINT20_23_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT24) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT25) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT26) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT27)
  NVIC_SetPriority(EXINT24_27_IRQn, PDL_IRQ_LEVEL_EXINT24_25_26_27);
  NVIC_DisableIRQ(EXINT24_27_IRQn);
#endif
#if (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT28) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT29) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT30) || \
    (PDL_ON == PDL_INTERRUPT_ENABLE_EXINT31)
  NVIC_SetPriority(EXINT28_31_IRQn, PDL_IRQ_LEVEL_EXINT28_29_30_31);
  NVIC_DisableIRQ(EXINT28_31_IRQn);
#endif  
#endif
  
  return Ok;  
} // Exint_DeInit

/**
 ******************************************************************************
 ** \brief Enable a single External Interrupt
 **
 ** This function enables a single External Interrupt.
 **
 ** \note This function does not set
 **       stc_exint_config_t#abEnable[u8Channel]!
 **
 ** \param [in]  u8Channel         External Interrupt channel number
 ** 
 ** \retval Ok                     Channel enabled
 ** \retval ErrorInvalidParameter  Invalid channel number
 ******************************************************************************/
en_result_t Exint_EnableChannel(uint8_t u8Channel)
{
  if (u8Channel > 32)
  {
    return ErrorInvalidParameter;
  }
  
  FM_EXTI->EICL &= 0xFFFFFFFFu ^ (1u << u8Channel);
  FM_EXTI->ENIR |= 1u << u8Channel;
  
  return Ok;
 
} // Exint_EnableChannel

/**
 ******************************************************************************
 ** \brief Disable a single External Interrupt
 **
 ** This function disables a single External Interrupt.
 **
 ** \note This function does not clear
 **       stc_exint_config_t#abEnable[u8Channel]!
 **
 ** \param [in]  u8Channel         External Interrupt channel number
 ** 
 ** \retval Ok                     Channel enabled
 ** \retval ErrorInvalidParameter  Invalid channel number or channel
 ******************************************************************************/
en_result_t Exint_DisableChannel(uint8_t u8Channel)
{
  if (u8Channel > 32)
  {
    return ErrorInvalidParameter;
  }
    
  FM_EXTI->ENIR &= (1u << u8Channel) ^ 0xFFFFFFFFu;
  
  return Ok;
} // Exint_DisableChannel

/**
 ******************************************************************************
 ** \brief Set external interrupt detect mode
 **
 ** \param u8Channel   0~31
 ** \param enLevel
 ** \arg ExIntLowLevel       Low level is active edge for interrupt detection 
 ** \arg ExIntHighLevel      High level is active edge for interrupt detection 
 ** \arg ExIntRisingEdge     Rising edge is active edge for interrupt detection 
 ** \arg ExIntFallingEdge    Falling edge is active edge for interrupt detection
 **         
 ** \retval Ok                         Interreupt detection edge set normmally
 ** \retval ErrorInvalidParameter      u8Ch > EXINT_MAX_CH_INDEX
 **                                    invalid setting for enLevel
 **
 ******************************************************************************/
en_result_t Exint_SetDetectMode(uint8_t u8Channel, en_exint_level_t enLevel)
{
    if((u8Channel + 1) > PDL_EXINT_CHANNELS)
    {
        return ErrorInvalidParameter;
    }
  
    switch (enLevel)
    {
        case ExIntLowLevel:
        case ExIntHighLevel:
        case ExIntRisingEdge:
        case ExIntFallingEdge:
        #if (PDL_MCU_TYPE == PDL_FM4_TYPE5) ||  (PDL_MCU_TYPE == PDL_FM4_TYPE6) || (PDL_MCU_TYPE == PDL_FM0P_TYPE3)       
            FM_EXTI->ELVR2 &= ~(1ul << u8Channel);
        #endif    
            if (u8Channel < 16)
            {
            FM_EXTI->ELVR &= ~(3ul << (2u * u8Channel));
            FM_EXTI->ELVR |= ((uint8_t)enLevel<<(2u * u8Channel));
            }
            else
            {
                FM_EXTI->ELVR1 &= ~(3ul << (2u * (u8Channel - 16u)));
                FM_EXTI->ELVR1 |= ((uint8_t)enLevel<<(2u * (u8Channel - 16u)));
            }
            break;  
    #if (PDL_MCU_TYPE == PDL_FM4_TYPE5) ||  (PDL_MCU_TYPE == PDL_FM4_TYPE6) || (PDL_MCU_TYPE == PDL_FM0P_TYPE3)        
        case ExIntBothEdge:
            FM_EXTI->ELVR2 |= (1ul << u8Channel);
            break;
    #endif            
        default:    
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Get the external interrupt detection mode 
 **
 ** \param u8Channel   0~31
 **         
 ** \retval ExIntLowLevel       Low level is active edge for interrupt detection 
 ** \retval ExIntHighLevel      High level is active edge for interrupt detection 
 ** \retval ExIntRisingEdge     Rising edge is active edge for interrupt detection 
 ** \retval ExIntFallingEdge    Falling edge is active edge for interrupt detection
 **
 ******************************************************************************/
en_exint_level_t Exint_GetDetectMode(uint8_t u8Channel)
{
    uint8_t u8Level;
    u8Level = (FM_EXTI->ELVR & (3ul<<(2u * u8Channel))) >> (2u * u8Channel);
    return (en_exint_level_t)u8Level;
}

#endif // #if (defined(PDL_PERIPHERAL_EXINT_ACTIVE))

#if (defined(PDL_PERIPHERAL_NMI_ACTIVE))

/**
 ******************************************************************************
 ** \brief Interrupt handler for None-Maskable Interrupt (NMI)
 **
 ** This function clears the cause bit and calls the NMI callback function, if
 ** specified.
 ******************************************************************************/
void Exint_Nmi_IrqHandler(void)
{
  if ((FM_EXTI->NMIRR & 0x01u) == 0x01u)
  {
    FM_EXTI->NMICL = 0u;

    if (0u != stcExintNMIInternData.pfnNmiCallback)
    {
      stcExintNMIInternData.pfnNmiCallback();
    }
  }
}

/**
 ******************************************************************************
 ** \brief Init Non-Maskable Interrupt (NMI)
 **
 ** This function initializes the NMI according to the given
 ** configuration.
 **
 ** \param [in]  pstcConfig        NMI configuration parameters
 ** 
 ** \retval Ok                     NMI initialized
 ** \retval ErrorInvalidParameter  pstcConfig == NULL
 **
 ******************************************************************************/
en_result_t Exint_Nmi_Init(stc_exint_nmi_config_t* pstcConfig)
{
  if ( pstcConfig == NULL )
  {
    return ErrorInvalidParameter ;
  }

#if (PDL_MCU_TYPE == PDL_FM0P_TYPE3)
  FM_EXTI->NMIENR = 0x01;  // Enable NMI 
#endif
  
  // Set internal data
  stcExintNMIInternData.pfnNmiCallback = pstcConfig->pfnNmiCallback;
  
  return Ok; 
} // Exint_Nmi_Init

/**
 ******************************************************************************
 ** \brief De-Init Non-Maskable Interrupt (NMI)
 **
 ** This function de-initializes the NMI according to the given
 ** configuration.
 **
 ** \param [in]  pstcConfig        NMI configuration parameters
 ** 
 ** \retval Ok                     NMI initialized
 ** \retval ErrorInvalidParameter  pstcConfig == NULL
 **
 ******************************************************************************/
en_result_t Exint_Nmi_DeInit(stc_exint_nmi_config_t* pstcConfig)
{
  if ( pstcConfig == NULL )
  {
    return ErrorInvalidParameter ;
  }

#if (PDL_MCU_TYPE == PDL_FM0P_TYPE3)
  FM_EXTI->NMIENR = 0x00;   // Disable NMI 
#endif  
  
  // Set internal data
  stcExintNMIInternData.pfnNmiCallback = NULL;

  return Ok; 
} // Exint_Nmi_DeInit

#endif  // #if (defined(PDL_PERIPHERAL_NMI_ACTIVE))


/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
