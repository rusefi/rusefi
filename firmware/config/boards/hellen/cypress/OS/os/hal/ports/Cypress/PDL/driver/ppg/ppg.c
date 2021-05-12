/*******************************************************************************
* \file             ppg.c
* 
* \version          1.20
*
* \brief            This file provides the source code to the API for the PPG 
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
#include "ppg/ppg.h"

#if (defined(PDL_PERIPHERAL_PPG_ACTIVE))


#if (PDL_INTERRUPT_ENABLE_PPG == PDL_ON)
/******************************************************************************/
/* Static variable definitions                                                */
/******************************************************************************/
static func_ptr_t  afnPpgCallback[PPG_INT_MAX_CH];      ///< Callback function pointer of PPG interrupt 

/**
 ******************************************************************************
 ** \brief Set NVIC Interrupt depending on PPG channel
 **
 ** \param u8CoupleCh PPG couple channel
 **
 ******************************************************************************/
static void PpgInitNvic(uint8_t u8CoupleCh)
{
#if (PDL_MCU_CORE == PDL_FM4_CORE)
    if((PPG_COUPLE_CH01 == u8CoupleCh) || (PPG_COUPLE_CH23 == u8CoupleCh) || (PPG_COUPLE_CH45 == u8CoupleCh))
    {
        NVIC_ClearPendingIRQ(PPG00_02_04_IRQn);
        NVIC_EnableIRQ(PPG00_02_04_IRQn);
        NVIC_SetPriority(PPG00_02_04_IRQn, PDL_IRQ_LEVEL_PPG00_02_04);  
    }
    else if ((PPG_COUPLE_CH89 == u8CoupleCh) || (PPG_COUPLE_CH1011 == u8CoupleCh) || (PPG_COUPLE_CH1213 == u8CoupleCh))
    {
        NVIC_ClearPendingIRQ(PPG08_10_12_IRQn);
        NVIC_EnableIRQ(PPG08_10_12_IRQn);
        NVIC_SetPriority(PPG08_10_12_IRQn, PDL_IRQ_LEVEL_PPG08_10_12);  
    }
    else if ((PPG_COUPLE_CH1617 == u8CoupleCh) || (PPG_COUPLE_CH1819 == u8CoupleCh) || (PPG_COUPLE_CH2021 == u8CoupleCh))
    {
        NVIC_ClearPendingIRQ(PPG16_18_20_IRQn);
        NVIC_EnableIRQ(PPG16_18_20_IRQn);
        NVIC_SetPriority(PPG16_18_20_IRQn, PDL_IRQ_LEVEL_PPG16_18_20);
    }
#elif (PDL_MCU_CORE == PDL_FM0P_CORE)
  #if (FM_INTERRUPT_TYPE == PDL_FM0P_INT_TYPE_1_A)
    NVIC_ClearPendingIRQ(PPG00_02_04_08_10_12_16_18_20_IRQn);
    NVIC_EnableIRQ(PPG00_02_04_08_10_12_16_18_20_IRQn);
    NVIC_SetPriority(PPG00_02_04_08_10_12_16_18_20_IRQn, PDL_IRQ_LEVEL_PPG00_02_04_08_10_12_16_18_20);
  #elif (FM_INTERRUPT_TYPE == PDL_FM0P_INT_TYPE_2_A)
    NVIC_ClearPendingIRQ(PPG00_02_20_DSTC_SMCIF0_HDMICEC0_IRQn);
    NVIC_EnableIRQ(PPG00_02_20_DSTC_SMCIF0_HDMICEC0_IRQn);
    NVIC_SetPriority(PPG00_02_20_DSTC_SMCIF0_HDMICEC0_IRQn, PDL_IRQ_LEVEL_PPG00_02_20_DSTC_SMCIF0_HDMICEC0);  
  #endif
#else
    NVIC_ClearPendingIRQ(PPG00_02_04_08_10_12_16_18_20_IRQn);
    NVIC_EnableIRQ(PPG00_02_04_08_10_12_16_18_20_IRQn);
    NVIC_SetPriority(PPG00_02_04_08_10_12_16_18_20_IRQn, PDL_IRQ_LEVEL_PPG);  
#endif    
}

/**
 ******************************************************************************
 ** \brief Clear NVIC Interrupt depending on PPG channel
 **
 ** \param u8CoupleCh PPG couple channel
 **
 ******************************************************************************/
static void PpgDeInitNvic(uint8_t u8CoupleCh)
{
#if (PDL_MCU_CORE == PDL_FM4_CORE)
    if((PPG_COUPLE_CH01 == u8CoupleCh) || (PPG_COUPLE_CH23 == u8CoupleCh) || (PPG_COUPLE_CH45 == u8CoupleCh))
    {
        NVIC_ClearPendingIRQ(PPG00_02_04_IRQn);
        NVIC_DisableIRQ(PPG00_02_04_IRQn);
        NVIC_SetPriority(PPG00_02_04_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);   
    }
    else if ((PPG_COUPLE_CH89 == u8CoupleCh) || (PPG_COUPLE_CH1011 == u8CoupleCh) || (PPG_COUPLE_CH1213 == u8CoupleCh))
    {
        NVIC_ClearPendingIRQ(PPG08_10_12_IRQn);
        NVIC_DisableIRQ(PPG08_10_12_IRQn);
        NVIC_SetPriority(PPG08_10_12_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);   
    }
    else if ((PPG_COUPLE_CH1617 == u8CoupleCh) || (PPG_COUPLE_CH1819 == u8CoupleCh) || (PPG_COUPLE_CH2021 == u8CoupleCh))
    {
        NVIC_ClearPendingIRQ(PPG16_18_20_IRQn);
        NVIC_DisableIRQ(PPG16_18_20_IRQn);
        NVIC_SetPriority(PPG16_18_20_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);   
    }
#elif (PDL_MCU_CORE == PDL_FM0P_CORE)
  #if (FM_INTERRUPT_TYPE == PDL_FM0P_INT_TYPE_1_A)
    NVIC_ClearPendingIRQ(PPG00_02_04_08_10_12_16_18_20_IRQn);
    NVIC_DisableIRQ(PPG00_02_04_08_10_12_16_18_20_IRQn);
    NVIC_SetPriority(PPG00_02_04_08_10_12_16_18_20_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);
  #elif (FM_INTERRUPT_TYPE == PDL_FM0P_INT_TYPE_2_A)
    NVIC_ClearPendingIRQ(PPG00_02_20_DSTC_SMCIF0_HDMICEC0_IRQn);
    NVIC_DisableIRQ(PPG00_02_20_DSTC_SMCIF0_HDMICEC0_IRQn);
    NVIC_SetPriority(PPG00_02_20_DSTC_SMCIF0_HDMICEC0_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);  
  #endif      
#else
    NVIC_ClearPendingIRQ(PPG00_02_04_08_10_12_16_18_20_IRQn);
    NVIC_DisableIRQ(PPG00_02_04_08_10_12_16_18_20_IRQn);
    NVIC_SetPriority(PPG00_02_04_08_10_12_16_18_20_IRQn, PDL_DEFAULT_INTERRUPT_LEVEL);   
#endif
}

/**
 ******************************************************************************
 ** \brief PPG interrupt service routine
 ******************************************************************************/      
void Ppg_IrqHandler(void) 
{     
    if(1u == bFM_MFT_PPG_PPGC0_PUF) // IRQ triggered by ch.0?
    {
        bFM_MFT_PPG_PPGC0_PUF = 0u;
        bFM_MFT_PPG_PPGC1_PUF = 0u;
        afnPpgCallback[0u]();
    }

    if(1u == bFM_MFT_PPG_PPGC2_PUF) // IRQ triggered by ch.2? 
    {
        bFM_MFT_PPG_PPGC2_PUF = 0u;
        bFM_MFT_PPG_PPGC3_PUF = 0u;
        afnPpgCallback[1u]();
    }

    if(1u == bFM_MFT_PPG_PPGC4_PUF) // IRQ triggered by ch.4? 
    {
        bFM_MFT_PPG_PPGC4_PUF = 0u;
        bFM_MFT_PPG_PPGC5_PUF = 0u;
        afnPpgCallback[2u]();
    }

    if(1u == bFM_MFT_PPG_PPGC8_PUF) // IRQ triggered by ch.8?
    {
        bFM_MFT_PPG_PPGC8_PUF = 0u;
        bFM_MFT_PPG_PPGC9_PUF = 0u;
        afnPpgCallback[3u]();
    }
    
    if(1u == bFM_MFT_PPG_PPGC10_PUF) // IRQ triggered by ch.10? 
    {
        bFM_MFT_PPG_PPGC10_PUF = 0u;
        bFM_MFT_PPG_PPGC11_PUF = 0u;
        afnPpgCallback[4u]();
    }
    
    if(1u == bFM_MFT_PPG_PPGC12_PUF) // IRQ triggered by ch.12? 
    {
        bFM_MFT_PPG_PPGC12_PUF = 0u;
        bFM_MFT_PPG_PPGC13_PUF = 0u;
        afnPpgCallback[5u]();
    }

    if(1u == bFM_MFT_PPG_PPGC16_PUF) // IRQ triggered by ch.16?
    {
        bFM_MFT_PPG_PPGC16_PUF = 0u;
        bFM_MFT_PPG_PPGC17_PUF = 0u;
        afnPpgCallback[6u]();
    }
    
    if(1u == bFM_MFT_PPG_PPGC18_PUF) // IRQ triggered by ch.18? 
    {
        bFM_MFT_PPG_PPGC18_PUF = 0u;
        bFM_MFT_PPG_PPGC19_PUF = 0u;
        afnPpgCallback[7u]();
    }
    
    if(1u == bFM_MFT_PPG_PPGC20_PUF) // IRQ triggered by ch.20? 
    {
        bFM_MFT_PPG_PPGC20_PUF = 0u;
        bFM_MFT_PPG_PPGC21_PUF = 0u;
        afnPpgCallback[8u]();
    }
}   

#endif

/**
 ******************************************************************************
 ** \brief  Initialize PPG
 **
 ** \param   [in]  u8CoupleCh         a couple PPG channel
 ** \arg           PPG_COUPLE_CH01 ~  PPG_COUPLE_CH2223
 ** \param   [in]  pstcConfig         pointer to PPG configuration structure
 **
 ** \retval  Ok                       Configure the PPG successfully
 ** \retval  ErrorInvalidParameter    If one of following conditions are met:
 **                                   - u8CoupleCh%2 !=0
 **                                   - u8CoupleCh > PPG_COUPLE_CH2223
 **                                   - Parameter error of PPG configuration structure
 **                                     
 ******************************************************************************/
en_result_t Ppg_Init( uint8_t u8CoupleCh, 
                      const stc_ppg_config_t *pstcConfig)
{
    volatile stc_mft_ppg_ppgc0_field_t *pstcPpgc0;
    volatile stc_mft_ppg_ppgc1_field_t *pstcPpgc1; 
    volatile uint16_t *pu16Revc0, *pu16Revc1;
    volatile uint8_t *pu8Gatec;
    uint8_t u8Offset, u8Gap;
    if( (0u != (u8CoupleCh%2))            ||
        (u8CoupleCh > PPG_COUPLE_CH2223) )
    {
        return ErrorInvalidParameter ;
    }
    u8Offset = u8CoupleCh%4u;
    u8Gap    = u8CoupleCh/4u;
    pstcPpgc1 = (stc_mft_ppg_ppgc1_field_t*)((uint8_t*)&FM_MFT_PPG->PPGC1 + u8Gap*0x40u + u8Offset*2u);
    pstcPpgc0 = (stc_mft_ppg_ppgc0_field_t*)((uint8_t*)&FM_MFT_PPG->PPGC0 + u8Gap*0x40u + u8Offset*2u);  
    pu16Revc0    = (uint16_t*)((uint8_t*)&FM_MFT_PPG->REVC0);
    pu16Revc1    = (uint16_t*)((uint8_t*)&FM_MFT_PPG->REVC1); 
    pu8Gatec     = (uint8_t*)((uint8_t*)&FM_MFT_PPG->GATEC0 + u8Gap*0x40u);
    
    // set mode
    switch (pstcConfig->enMode)
    {
        case Ppg8Bit8Bit:
            pstcPpgc0->MD = 0u;
            break;
        case Ppg8Bit8Pres:
            pstcPpgc0->MD = 1u;
            break;
        case Ppg16Bit:
            pstcPpgc0->MD = 2u;
            break;
        case Ppg16Bit16Pres:
            pstcPpgc0->MD = 3u;
            break;
        default:
            return ErrorInvalidParameter ;
    }
    
    // set even channel clock, level
    switch (pstcConfig->enEvenClock)
    {
        case PpgPclkDiv1:
            pstcPpgc0->PCS = 0u;
            break;
        case PpgPclkDiv4:
            pstcPpgc0->PCS = 1u;
            break;
        case PpgPclkDiv16:
            pstcPpgc0->PCS = 2u;
            break;
        case PpgPclkDiv64:
            pstcPpgc0->PCS = 3u;
            break;
        default:
            return ErrorInvalidParameter ;
    }
    
    if(u8CoupleCh < PPG_CH16)
    {
        switch (pstcConfig->enEvenLevel)
        {
            case PpgNormalLevel:
                *pu16Revc0 &= ~(1ul<<u8CoupleCh);
                break;
            case PpgReverseLevel:
                *pu16Revc0 |= (1ul<<u8CoupleCh);
                break;
            default:
                return ErrorInvalidParameter ;
        }
    }
    else
    {
        switch (pstcConfig->enEvenLevel)
        {
            case PpgNormalLevel:
                *pu16Revc1 &= ~(1ul<<(u8CoupleCh-PPG_CH16));
                break;
            case PpgReverseLevel:
                *pu16Revc1 |= (1ul<<(u8CoupleCh-PPG_CH16));
                break;
            default:
                return ErrorInvalidParameter ;                
        }
    }
    
    // set odd channel clock, level    
    switch (pstcConfig->enOddClock)
    {
        case PpgPclkDiv1:
            pstcPpgc1->PCS = 0u;
            break;
        case PpgPclkDiv4:
            pstcPpgc1->PCS = 1u;
            break;
        case PpgPclkDiv16:
            pstcPpgc1->PCS = 2u;
            break;
        case PpgPclkDiv64:
            pstcPpgc1->PCS = 3u;
            break;
        default:
            return ErrorInvalidParameter ;                
    }
    
    if(u8CoupleCh < PPG_CH16)
    {
        switch (pstcConfig->enOddLevel)
        {
            case PpgNormalLevel:
                *pu16Revc0 &= ~(1ul<<(u8CoupleCh+1));
                break;
            case PpgReverseLevel:
                *pu16Revc0 |= (1ul<<(u8CoupleCh+1));
                break;
            default:
                return ErrorInvalidParameter ;                
        }
    }
    else
    {
        switch (pstcConfig->enOddLevel)
        {
            case PpgNormalLevel:
                *pu16Revc1 &= ~(1ul<<(u8CoupleCh+1-PPG_CH16));
                break;
            case PpgReverseLevel:
                *pu16Revc1 |= (1ul<<(u8CoupleCh+1-PPG_CH16));
                break;
            default:
                return ErrorInvalidParameter ;      
        }
    }
    
    // set PPG mode
    switch (pstcConfig->enTrig)
    {
        case PpgSoftwareTrig:
            pstcPpgc0->TTRG = 0u;
            if((u8CoupleCh%4) == 0u)
            {
                *pu8Gatec &= ~(1ul<<1);
            }
            else
            {
                *pu8Gatec &= ~(1ul<<5);
            }
            break;  
        case PpgMftGateTrig:
            pstcPpgc0->TTRG = 0u;
            if((u8CoupleCh%4) == 0)
            {
                *pu8Gatec |= (1ul<<1);
            }
            else
            {
                *pu8Gatec |= (1ul<<5);
            }
            break; 
        case PpgTimingGenTrig:
            pstcPpgc0->TTRG = 1u;
            break; 
        default:
            return ErrorInvalidParameter ;      
    }
    
    // Set interrupt mode
    switch (pstcConfig->enIrqMode)
    {
        case PpgIrqHighOrLowUnderflow:
            pstcPpgc0->INTM = 0u;
            break;
        case PpgIrqHighUnderflow:
            pstcPpgc0->INTM = 1u;
            break;
        default:
            return ErrorInvalidParameter ;      
    }

#if (PDL_INTERRUPT_ENABLE_PPG == PDL_ON)   
    // Set interrupt enable 
    if(pstcConfig->bIrqEnable == TRUE)
    {
        pstcPpgc0->PIE = 1u;
    }
    
    // Set interrupt callback function
    if((PPG_COUPLE_CH01 == u8CoupleCh) || (PPG_COUPLE_CH23 == u8CoupleCh) || (PPG_COUPLE_CH45 == u8CoupleCh))
    {
        afnPpgCallback[u8CoupleCh/2u] = pstcConfig->pfnIrqCallback;
    }
    else if((PPG_COUPLE_CH89 == u8CoupleCh) || (PPG_COUPLE_CH1011 == u8CoupleCh) || (PPG_COUPLE_CH1213 == u8CoupleCh))
    {
        afnPpgCallback[(u8CoupleCh-2u)/2u] = pstcConfig->pfnIrqCallback;
    }
    else if((PPG_COUPLE_CH1617 == u8CoupleCh) || (PPG_COUPLE_CH1819 == u8CoupleCh) || (PPG_COUPLE_CH2021 == u8CoupleCh))
    {
        afnPpgCallback[(u8CoupleCh-4u)/2u] = pstcConfig->pfnIrqCallback;
    }
    
    // Set NVIC
    if(TRUE == pstcConfig->bTouchNvic)
    {
        PpgInitNvic(u8CoupleCh);
    }
#endif 
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  De-Initialize PPG
 **
 ** \param   [in]  u8CoupleCh         a couple PPG channel
 ** \arg           PPG_COUPLE_CH01 ~  PPG_COUPLE_CH2223
 ** \param   [in]  bTouchNvic         Touch NVIC or not
 **
 ** \retval  Ok                       Configure the PPG successfully
 ** \retval  ErrorInvalidParameter    If one of following conditions are met:
 **                                   - u8CoupleCh%2 !=0
 **                                   - u8CoupleCh > PPG_COUPLE_CH2223
 **                                     
 ******************************************************************************/
en_result_t Ppg_DeInit(uint8_t u8CoupleCh, boolean_t bTouchNvic)
{
    volatile uint8_t *pu8Ppgc0;
    volatile uint8_t *pu8Ppgc1; 
    uint8_t u8Offset, u8Gap;
    if( (0u != (u8CoupleCh%2u))            ||
        (u8CoupleCh > PPG_COUPLE_CH2223) )
    {
        return ErrorInvalidParameter ;
    }
    u8Offset = u8CoupleCh%4u;
    u8Gap    = u8CoupleCh/4u;
    pu8Ppgc1 = (uint8_t*)((uint8_t*)&FM_MFT_PPG->PPGC1 + u8Gap*0x40u + u8Offset*2u);
    pu8Ppgc0 = (uint8_t*)((uint8_t*)&FM_MFT_PPG->PPGC0 + u8Gap*0x40u + u8Offset*2u + 1u);  
    
    *pu8Ppgc0 = 0u;
    *pu8Ppgc1 = 0u;
#if (PDL_INTERRUPT_ENABLE_PPG == PDL_ON)    
    if(TRUE == bTouchNvic)
    {
        PpgDeInitNvic(u8CoupleCh);
    }
#endif    
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  Start PPG by software trigger
 **
 ** \param   [in]  u8Ch               PPG channel
 ** \arg           PPG_CH0~PPG_CH23
 **
 ** \retval  Ok                       Configure the PPG successfully
 ** \retval  ErrorInvalidParameter    If one of following conditions are met:
 **                                   - u8Ch > PPG_CH23
 **                                     
 ******************************************************************************/
en_result_t Ppg_StartSoftwareTrig(uint8_t u8Ch)
{
    if((u8Ch > PPG_CH23))
    {
        return ErrorInvalidParameter ;
    }
    
    if(u8Ch < PPG_CH16)
    {
        FM_MFT_PPG->TRG0 |= (uint16_t)(1ul<<u8Ch);
    }
    else
    {
        FM_MFT_PPG->TRG1 |= (uint16_t)(1ul<<(u8Ch-PPG_CH16));
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief   Stop PPG software trigger
 **
 ** \param   [in]  u8Ch               PPG channel
 ** \arg           PPG_CH0~PPG_CH23
 **
 ** \retval  Ok                       Configure the PPG successfully
 ** \retval  ErrorInvalidParameter    If one of following conditions are met:
 **                                   - u8Ch > PPG_CH23
 **                                     
 ******************************************************************************/
en_result_t Ppg_StopSoftwareTrig(uint8_t u8Ch)
{
    if((u8Ch > PPG_CH23))
    {
        return ErrorInvalidParameter ;
    }
    
    if(u8Ch < PPG_CH16)
    {
        FM_MFT_PPG->TRG0 &= (uint16_t)(~(1ul<<u8Ch));
    }
    else
    {
        FM_MFT_PPG->TRG1 &= (uint16_t)(~(1ul<<(u8Ch-PPG_CH16)));
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief   Set the valid level of GATE signal
 **
 ** \param   [in]  u8EvenCh           an even channel of PPG
 ** \arg           PPG_CH0~PPG_CH22
 ** \param   [in]  enLevel            Valid level
 **
 ** \retval  Ok                       Set the valid level of GATE signal successfully
 ** \retval  ErrorInvalidParameter    If one of following conditions are met:
 **                                   - u8Ch > PPG_CH22
 **                                   - u8EvenCh%2 != 0
 **                                   - enLevel > PpgGateLowActive
 **                                     
 ******************************************************************************/
en_result_t Ppg_SelGateLevel(uint8_t u8EvenCh, en_ppg_gate_level_t enLevel)
{
    volatile uint8_t *pu8Gatec;
    if( (0u != (u8EvenCh%2u))      ||
        (u8EvenCh > PPG_CH22)      ||
        (enLevel > PpgGateLowActive)  )
    {
        return ErrorInvalidParameter ;
    }
    
    pu8Gatec     = (uint8_t*)FM_MFT_PPG_BASE + 0x218u + 0x40u*(u8EvenCh/4);
    
    if(0u == (u8EvenCh%4u))
    {
        *pu8Gatec &= ~(1ul);
        *pu8Gatec |= enLevel;
    }
    else
    {
        *pu8Gatec &= ~(1ul<<4u);
        *pu8Gatec |= (uint8_t)((uint8_t)enLevel<<4u);
    }
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief   Configure the up counter of Timing Generator 0
 **
 ** \param   [in]  pstcConfig         Pointer to up counter 0 configuration structure
 **
 ** \retval  Ok                       Configure the up counter successfully
 ** \retval  ErrorInvalidParameter    If one of following conditions are met:
 **                                   - pstcConfig != NULL
 **                                   - pstcConfig->enClk > PpgUpCntPclkDiv64 
 **                                     
 ******************************************************************************/
en_result_t  Ppg_ConfigUpCnt0(stc_ppg_upcnt0_config_t* pstcConfig)
{
    if((NULL != pstcConfig) || 
       (pstcConfig->enClk > PpgUpCntPclkDiv64 ))
    {
        return ErrorInvalidParameter ;
    }
    
    FM_MFT_PPG->TTCR0 &= ~(3ul<<2u);
    FM_MFT_PPG->TTCR0 |= (uint16_t)((uint16_t)pstcConfig->enClk<<2u);
    FM_MFT_PPG->COMP0 = pstcConfig->u8CmpValue0;
    FM_MFT_PPG->COMP2 = pstcConfig->u8CmpValue2;
    FM_MFT_PPG->COMP4 = pstcConfig->u8CmpValue4;
    FM_MFT_PPG->COMP6 = pstcConfig->u8CmpValue6;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief   Start the up counter of Timing Generator 0                                   
 ******************************************************************************/
void Ppg_StartUpCnt0(void)
{
    bFM_MFT_PPG_TTCR0_STR0 = 1u;
}

/**
 ******************************************************************************
 ** \brief   Get the work status of up counter of Timing Generator 0
 **
 ** \retval  PdlSet                   Up counter is counting
 ** \retval  PdlClr                   Up counter stops
 **                                     
 ******************************************************************************/
en_stat_flag_t  Ppg_GetUpCnt0Status(void)
{
    en_stat_flag_t enRet;
  
    (1u == bFM_MFT_PPG_TTCR0_MONI0) ? (enRet = PdlSet) : (enRet = PdlClr);
  
    return enRet;
}

/**
 ******************************************************************************
 ** \brief   Disable start trigger of Timing Generator 0
 **
 ** \param   pstcTimer0GenCh          Pointer to the structure of selected channels 
 **                                   (ch.0,2,4,6)
 **
 ** \retval  Ok                       Disable start trigger
 ** \retval  ErrorInvalidParameter    If one of following conditions are met:
 **                                   - pstcTimer0GenCh->bPpgChx isn't bool type
 **                                     
 ******************************************************************************/
en_result_t  Ppg_DisableTimerGen0StartTrig(stc_timer0_gen_ch_t* pstcTimer0GenCh)
{
    if((pstcTimer0GenCh->bPpgCh0 > 1u) || 
       (pstcTimer0GenCh->bPpgCh2 > 1u) ||
       (pstcTimer0GenCh->bPpgCh4 > 1u) || 
       (pstcTimer0GenCh->bPpgCh6 > 1u)  )
    {
        return ErrorInvalidParameter;
    }
    
    bFM_MFT_PPG_TTCR0_TRG0O = ((pstcTimer0GenCh->bPpgCh0 == 1u) ? 1u : 0u);
    bFM_MFT_PPG_TTCR0_TRG2O = ((pstcTimer0GenCh->bPpgCh2 == 1u) ? 1u : 0u);
    bFM_MFT_PPG_TTCR0_TRG4O = ((pstcTimer0GenCh->bPpgCh4 == 1u) ? 1u : 0u);
    bFM_MFT_PPG_TTCR0_TRG6O = ((pstcTimer0GenCh->bPpgCh6 == 1u) ? 1u : 0u);
    return Ok;
}

/**
 ******************************************************************************
 ** \brief   Configure the up counter of Timing Generator 1
 **
 ** \param   [in]  pstcConfig         Pointer to up counter 1 configuration structure
 **
 ** \retval  Ok                       Configure the up counter successfully
 ** \retval  ErrorInvalidParameter    If one of following conditions are met:
 **                                   - pstcConfig != NULL
 **                                   - pstcConfig->enClk > PpgUpCntPclkDiv64 
 **                                     
 ******************************************************************************/
en_result_t  Ppg_ConfigUpCnt1(stc_ppg_upcnt1_config_t* pstcConfig)
{
    if(pstcConfig->enClk > PpgUpCntPclkDiv64 )
    {
        return ErrorInvalidParameter ;
    }
    
    FM_MFT_PPG->TTCR1 &= ~(3ul<<2u);
    FM_MFT_PPG->TTCR1 |= (uint16_t)((uint16_t)pstcConfig->enClk<<2u);
    FM_MFT_PPG->COMP1 = pstcConfig->u8CmpValue8;
    FM_MFT_PPG->COMP3 = pstcConfig->u8CmpValue10;
    FM_MFT_PPG->COMP5 = pstcConfig->u8CmpValue12;
    FM_MFT_PPG->COMP7 = pstcConfig->u8CmpValue14;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief   Start the up counter of Timing Generator 1                        
 ******************************************************************************/
void  Ppg_StartUpCnt1(void)
{
    bFM_MFT_PPG_TTCR1_STR1 = 1u;
}

/**
 ******************************************************************************
 ** \brief   Get the work status of up counter of Timing Generator 1
 **
 ** \retval  PdlSet                   Up counter is counting
 ** \retval  PdlClr                   Up counter stops
 **                                     
 ******************************************************************************/
en_stat_flag_t  Ppg_GetUpCnt1Status(void)
{
    en_stat_flag_t enRet;
    
    (bFM_MFT_PPG_TTCR1_MONI1 == 1u) ? (enRet = PdlSet) : (enRet = PdlClr);
  
    return enRet;
}

/**
 ******************************************************************************
 ** \brief   Disable start trigger of Timing Generator 1
 **
 ** \param   pstcTimer1GenCh          Pointer to the structure of selected channels 
 **                                   (ch.8,10,12,14)
 **
 ** \retval  Ok                       Disable start trigger
 ** \retval  ErrorInvalidParameter    If one of following conditions are met:
 **                                   - pstcTimer0GenCh->bPpgChx isn't bool type
 **                                     
 ******************************************************************************/
en_result_t  Ppg_DisableTimerGen1StartTrig(stc_timer1_gen_ch_t* pstcTimer1GenCh)
{
    if((pstcTimer1GenCh->bPpgCh8 > 1u)  || 
       (pstcTimer1GenCh->bPpgCh10 > 1u) ||
       (pstcTimer1GenCh->bPpgCh12 > 1u) || 
       (pstcTimer1GenCh->bPpgCh14 > 1u)  )
    {
        return ErrorInvalidParameter;
    }
    
    bFM_MFT_PPG_TTCR1_TRG1O = ((pstcTimer1GenCh->bPpgCh8 == 1u) ? 1u : 0u);
    bFM_MFT_PPG_TTCR1_TRG3O = ((pstcTimer1GenCh->bPpgCh10 == 1u) ? 1u : 0u);
    bFM_MFT_PPG_TTCR1_TRG5O = ((pstcTimer1GenCh->bPpgCh12 == 1u) ? 1u : 0u);
    bFM_MFT_PPG_TTCR1_TRG7O = ((pstcTimer1GenCh->bPpgCh14 == 1u) ? 1u : 0u);
    return Ok;
}

/**
 ******************************************************************************
 ** \brief   Configure the up counter of Timing Generator 2
 **
 ** \param   [in]  pstcConfig         Pointer to up counter 2 configuration structure
 **
 ** \retval  Ok                       Configure the up counter successfully
 ** \retval  ErrorInvalidParameter    If one of following conditions are met:
 **                                   - pstcConfig != NULL
 **                                   - pstcConfig->enClk > PpgUpCntPclkDiv64 
 **                                     
 ******************************************************************************/
en_result_t  Ppg_ConfigUpCnt2(stc_ppg_upcnt2_config_t* pstcConfig)
{
    if(pstcConfig->enClk > PpgUpCntPclkDiv64 )
    {
        return ErrorInvalidParameter ;
    }
    
    FM_MFT_PPG->TTCR1 &= ~(3ul<<2u);
    FM_MFT_PPG->TTCR1 |= (uint16_t)((uint16_t)pstcConfig->enClk<<2u);
    FM_MFT_PPG->COMP8 = pstcConfig->u8CmpValue16;
    FM_MFT_PPG->COMP10 = pstcConfig->u8CmpValue18;
    FM_MFT_PPG->COMP12 = pstcConfig->u8CmpValue20;
    FM_MFT_PPG->COMP14 = pstcConfig->u8CmpValue22;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief   Start the up counter of Timing Generator 2                                    
 ******************************************************************************/
void  Ppg_StartUpCnt2(void)
{
    bFM_MFT_PPG_TTCR2_STR2 = 1u;
}

/**
 ******************************************************************************
 ** \brief   Get the work status of up counter of Timing Generator 2
 **
 ** \retval  PdlSet                   Up counter is counting
 ** \retval  PdlClr                   Up counter stops
 **                                     
 ******************************************************************************/
en_stat_flag_t  Ppg_GetUpCnt2Status(void)
{
    en_stat_flag_t enRet;
    (bFM_MFT_PPG_TTCR2_MONI2 == 1u) ? (enRet = PdlSet) : (enRet = PdlClr);
    return enRet;
}

/**
 ******************************************************************************
 ** \brief   Disable start trigger of Timing Generator 2
 **
 ** \param   pstcTimer2GenCh          Pointer to the structure of selected channels 
 **                                   (ch.16,18,20,22)
 **
 ** \retval  Ok                       Disable start trigger
 ** \retval  ErrorInvalidParameter    If one of following conditions are met:
 **                                   - pstcTimer0GenCh->bPpgChx isn't bool type
 **                                     
 ******************************************************************************/
en_result_t  Ppg_DisableTimerGen2StartTrig(stc_timer2_gen_ch_t* pstcTimer2GenCh)
{
    if((pstcTimer2GenCh->bPpgCh16 > 1u)  || 
       (pstcTimer2GenCh->bPpgCh18 > 1u) ||
       (pstcTimer2GenCh->bPpgCh20 > 1u) || 
       (pstcTimer2GenCh->bPpgCh22 > 1u)  )
    {
        return ErrorInvalidParameter;
    }
    
    bFM_MFT_PPG_TTCR2_TRG16O = ((pstcTimer2GenCh->bPpgCh16 == 1u) ? 1u : 0u);
    bFM_MFT_PPG_TTCR2_TRG18O = ((pstcTimer2GenCh->bPpgCh18 == 1u) ? 1u : 0u);
    bFM_MFT_PPG_TTCR2_TRG20O = ((pstcTimer2GenCh->bPpgCh20 == 1u) ? 1u : 0u);
    bFM_MFT_PPG_TTCR2_TRG22O = ((pstcTimer2GenCh->bPpgCh22 == 1u) ? 1u : 0u);
    return Ok;
}

#if (PDL_INTERRUPT_ENABLE_PPG == PDL_ON) 
/**
 ******************************************************************************
 ** \brief   Enable PPG interrupt
 **
 ** \param   [in] u8CoupleCh           PPG couple channel
 ** \arg     PPG_CHx, x=0,2,4,8,10,12,16,18,20
 **
 ** \retval  Ok                       Enable PPG interrupt successfully
 ** \retval  ErrorInvalidParameter    If one of following conditions are met:
 **                                   - u8Ch != ch.0,2,4,8,10,12,16,18,20
 **                                     
 ******************************************************************************/
en_result_t  Ppg_EnableIrq(uint8_t u8CoupleCh)
{
    volatile stc_mft_ppg_ppgc0_field_t *pstcPpgc0;
    uint8_t u8Offset, u8Gap;
    if( (0u != (u8CoupleCh%2u))            ||
        (u8CoupleCh > PPG_COUPLE_CH2223) )
    {
        return ErrorInvalidParameter ;
    }
    
    u8Offset = u8CoupleCh%4u;
    u8Gap    = u8CoupleCh/4u;
    pstcPpgc0 = (stc_mft_ppg_ppgc0_field_t*)((uint8_t*)&FM_MFT_PPG->PPGC0 + u8Gap*0x40u + u8Offset*2u);  
    
    pstcPpgc0->PIE = 1u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief   Disable PPG interrupt
 **
 ** \param   [in] u8CoupleCh             PPG couple channel
 ** \arg     PPG_CHx, x=0,2,4,8,10,12,16,18,20
 **
 ** \retval  Ok                       Disable PPG interrupt successfully
 ** \retval  ErrorInvalidParameter    If one of following conditions are met:
 **                                   - u8Ch != ch.0,2,4,8,10,12,16,18,20
 **                                     
 ******************************************************************************/
en_result_t  Ppg_DisableIrq(uint8_t u8CoupleCh)
{
    volatile stc_mft_ppg_ppgc0_field_t *pstcPpgc0;
    uint8_t u8Offset, u8Gap;
    if( (0u != (u8CoupleCh%2u))            ||
        (u8CoupleCh > PPG_COUPLE_CH2223) )
    {
        return ErrorInvalidParameter ;
    }
    
    u8Offset = u8CoupleCh%4u;
    u8Gap    = u8CoupleCh/4u;
    pstcPpgc0 = (stc_mft_ppg_ppgc0_field_t*)((uint8_t*)&FM_MFT_PPG->PPGC0 + u8Gap*0x40u + u8Offset*2u);  
    
    pstcPpgc0->PIE = 0u;
    
    return Ok;
}

#endif

/**
 ******************************************************************************
 ** \brief   Get the interrupt flag of PPG
 **
 ** \param   [in] u8CoupleCh             PPG couple channel
 ** \arg     PPG_CHx, x=0,2,4,8,10,12,16,18,20
 **
 ** \retval  PdlSet                   Interrupt flag set
 ** \retval  PdlClr                   Interrupt flag clear
 **                                     
 ******************************************************************************/
en_irq_flag_t  Ppg_GetIrqFlag(uint8_t u8CoupleCh)
{
    volatile uint8_t *pu8Ppgc0Addr;
    uint8_t u8Offset, u8Gap;
    en_irq_flag_t enFlag;
    // Only Ch.0,2,4,8,10,12,16,18,20 can produce interrupt request
    if( (0u != (u8CoupleCh%2u))            ||
        (u8CoupleCh > PPG_COUPLE_CH2223) )
    {
        return PdlClr ;
    }
    
    u8Offset = u8CoupleCh%4u;
    u8Gap    = u8CoupleCh/4u;
    pu8Ppgc0Addr = (uint8_t*)FM_MFT_PPG_BASE + 0x200u + u8Gap*0x40u + u8Offset*2 + 1u;  
    
    if(0x40u == (*pu8Ppgc0Addr & 0x40u))
    {
        enFlag = PdlSet;
    }
    else
    {
        enFlag = PdlClr;
    }
    
    return enFlag;
}

/**
 ******************************************************************************
 ** \brief   Clear PPG interrupt flag
 **
 ** \param   [in] u8CoupleCh            PPG couple channel
 ** \arg     PPG_CHx, x=0,2,4,8,10,12,16,18,20
 **
 ** \retval  Ok                       Clear PPG interrupt flag successfully
 ** \retval  ErrorInvalidParameter    If one of following conditions are met:
 **                                   - u8Ch != ch.0,2,4,8,10,12,16,18,20
 **                                     
 ******************************************************************************/
en_result_t  Ppg_ClrIrqFlag(uint8_t u8CoupleCh)
{
    volatile uint8_t *pu8Ppgc0Addr, *pu8Ppgc1Addr;
    uint8_t u8Offset, u8Gap;
    // Only Ch.0,2,4,8,10,12,16,18,20 can produce interrupt request
    if( (0u != (u8CoupleCh%2u))            ||
        (u8CoupleCh > PPG_COUPLE_CH2223) )
    {
        return ErrorInvalidParameter ;
    }
    
    u8Offset = u8CoupleCh%4u;
    u8Gap    = u8CoupleCh/4u;
    pu8Ppgc0Addr = (uint8_t*)FM_MFT_PPG_BASE + 0x200u + u8Gap*0x40u + u8Offset*2u + 1u; 
    pu8Ppgc1Addr = (uint8_t*)FM_MFT_PPG_BASE + 0x200u + u8Gap*0x40u + u8Offset*2u ; 
        
    // Clear interrupt flag of both even and odd channel 
    *pu8Ppgc0Addr &= ~(1ul<<6u);
    *pu8Ppgc1Addr &= ~(1ul<<6u);
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief   Set the pulse width of PPG
 **
 ** \param   [in] u8Ch                PPG channel
 ** \arg     PPG_CH0~PPG_CH23
 ** \param   [in] u8LowWidth          Low level width of PPG
 ** \param   [in] u8HighWidth         High level width of PPG
 **
 ** \retval  Ok                       Set the pulse width of PPG successfully
 ** \retval  ErrorInvalidParameter    If one of following conditions are met:
 **                                   - u8Ch > PPG_CH23
 **                                     
 ******************************************************************************/
en_result_t  Ppg_SetLevelWidth(uint8_t u8Ch, uint8_t u8LowWidth, uint8_t u8HighWidth)
{
    volatile uint8_t *pu8Prll0Addr, *pu8Prlh0Addr;
    uint8_t u8Offset, u8Gap;
    
    if(u8Ch > PPG_CH23)
    {
        return ErrorInvalidParameter;
    }
    
    u8Offset = u8Ch%4u;
    u8Gap    = u8Ch/4u;
    pu8Prll0Addr = (uint8_t*)FM_MFT_PPG_BASE + 0x208u + u8Gap*0x40u + u8Offset*4u;
    pu8Prlh0Addr = (uint8_t*)FM_MFT_PPG_BASE + 0x208u + u8Gap*0x40u + u8Offset*4u + 1u;
    *pu8Prll0Addr = u8LowWidth;
    *pu8Prlh0Addr = u8HighWidth;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief  Initialize IGBT mode
 **
 ** \param   [in]  pstcPpgIgbt        pointer to IBGT configuration structure

 ** \retval  ErrorInvalidParameter    If one of following conditions are met:
 **                                   - pstcPpgIgbt == NULL
 **                                   - Other error configuration
 **                                     
 ******************************************************************************/
#if (defined(FM_MFT_PPG_IGBTC))
en_result_t  Ppg_InitIgbt(stc_ppg_igbt_config_t* pstcPpgIgbt)
{

    if((NULL == pstcPpgIgbt) ||
       (pstcPpgIgbt->enMode > IgbtStopProhibitionMode) ||
       (pstcPpgIgbt->enWidth > IgbtFilter32Pclk) ||
       (pstcPpgIgbt->enTrigInputLevel  > IgbtLevelInvert)  ||
       (pstcPpgIgbt->enIgbt0OutputLevel > IgbtLevelInvert) ||
       (pstcPpgIgbt->enIgbt1OutputLevel > IgbtLevelInvert)   )
    {
        return ErrorInvalidParameter;
    }
    // Prohibition mode setting  
    FM_MFT_PPG->IGBTC &= ~(1ul<<7u); 
    FM_MFT_PPG->IGBTC |= (uint8_t)((uint8_t)pstcPpgIgbt->enMode<<7);
    // Filter width setting
    FM_MFT_PPG->IGBTC &= ~(7ul<<4u);
    FM_MFT_PPG->IGBTC |= (uint8_t)((uint8_t)pstcPpgIgbt->enWidth<<4);
    // Trigger input level setting
    FM_MFT_PPG->IGBTC &= ~(1ul<<1u); 
    FM_MFT_PPG->IGBTC |= (uint8_t)((uint8_t)pstcPpgIgbt->enTrigInputLevel<<1); 
    // IGBT output level setting
    FM_MFT_PPG->IGBTC &= ~(1ul<<2u); 
    FM_MFT_PPG->IGBTC |= (uint8_t)((uint8_t)pstcPpgIgbt->enIgbt0OutputLevel<<2); 
    FM_MFT_PPG->IGBTC &= ~(1ul<<3u); 
    FM_MFT_PPG->IGBTC |= (uint8_t)((uint8_t)pstcPpgIgbt->enIgbt0OutputLevel<<3); 
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief   Enable IGBT mode 
 ** \note    Only PPG0 and PP4 supports IGBT mode. 
 **
 ******************************************************************************/
void Ppg_EnableIgbtMode(void)
{
    FM_MFT_PPG->IGBTC |= 1ul;
}

/**
 ******************************************************************************
 ** \brief   Disable IGBT mode 
 ******************************************************************************/
void Ppg_DisableIgbtMode(void)
{
    FM_MFT_PPG->IGBTC &= ~1ul;
}
#endif          

#endif
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
