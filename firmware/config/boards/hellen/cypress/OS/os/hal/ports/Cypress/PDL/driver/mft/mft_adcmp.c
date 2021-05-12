/*******************************************************************************
* \file             mft_adcmp.c
* 
* \version          1.20
*
* \brief            This file provides the source code to the API for the 
*                   MFT_ADCMP driver.
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
#include "mft/mft_adcmp.h"

#if (defined(PDL_PERIPHERAL_MFT_ADCMP_ACTIVE))

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/

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
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
/**
 ******************************************************************************
 ** \brief Device dependent initialization of Mft adcmp module
 **       
 ** \param [in] pstcMftAdcmp    Pointer to Mft instance
 ** \param [in] u8Ch            Mft adcmp channel
 ** \arg        MFT_ADCMP_CH0 ADCMP channel 0
 ** \arg        MFT_ADCMP_CH1 ADCMP channel 1
 ** \arg        MFT_ADCMP_CH2 ADCMP channel 2
 ** \arg        MFT_ADCMP_CH3 ADCMP channel 3
 ** \arg        MFT_ADCMP_CH4 ADCMP channel 4
 ** \arg        MFT_ADCMP_CH5 ADCMP channel 5
 ** \param [in] pstcConfig      Pointer to Mft adcmp config
 ** \arg        structure of Mft Adcmp configuration
 **
 ** \retval Ok                        Adcmp initialized 
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **                                   - pstcMftAdcmp == NULL
 **                                   - pstcConfig == NULL
 **                                   - u8Ch > MFT_ADCMP_CH_MAX
 ******************************************************************************/
en_result_t Mft_Adcmp_Init(volatile stc_mftn_adcmp_t* pstcMftAdcmp, 
                            uint8_t u8Ch, 
                            const stc_mft_adcmp_config_t* pstcConfig)
{
    volatile stc_mft_adcmp_acfs10_field_t* pstcACFS10 ;
    volatile stc_mft_adcmp_acfs32_field_t* pstcACFS32 ;
    volatile stc_mft_adcmp_acfs54_field_t* pstcACFS54 ;
    
    volatile stc_mft_adcmp_acsc0_field_t*   pstcACSCx ;
    volatile stc_mft_adcmp_acsd0_field_t*   pstcACSDx ;
#if (PDL_MCU_TYPE >= PDL_FM4_TYPE3)      
    volatile stc_mft_adcmp_acmc0_field_t*   pstcACMCx ;
#endif     
    if ((NULL == pstcMftAdcmp)||(NULL == pstcConfig)||(u8Ch > MFT_ADCMP_CH_MAX))
    {
        return ErrorInvalidParameter ;
    }

    // Get actual address of register list of current channel
    pstcACFS10 = (volatile stc_mft_adcmp_acfs10_field_t*)(&pstcMftAdcmp->ACFS10);
    pstcACFS32 = (volatile stc_mft_adcmp_acfs32_field_t*)(&pstcMftAdcmp->ACFS32);
    pstcACFS54 = (volatile stc_mft_adcmp_acfs54_field_t*)(&pstcMftAdcmp->ACFS54);
    
    pstcACSCx = (volatile stc_mft_adcmp_acsc0_field_t*)(&pstcMftAdcmp->ACSC0 + 0x4*u8Ch);
    pstcACSDx = (volatile stc_mft_adcmp_acsd0_field_t*)(&pstcMftAdcmp->ACSD0 + 0x4*u8Ch);
#if (PDL_MCU_TYPE >= PDL_FM4_TYPE3)    
    pstcACMCx = (volatile stc_mft_adcmp_acmc0_field_t*)(&pstcMftAdcmp->ACMC0 + 0x4*u8Ch);
#endif
    // Configure parameter
    pstcACSDx->DE = 0u;
    pstcACSDx->PE = 0u;
    pstcACSDx->UE = 0u;
    pstcACSDx->ZE = 0u;
    
    pstcMftAdcmp->ACSA &= 0x0000u;
	
    // Select FRT channel : FRTx -> ADCMPx
    switch(u8Ch)
    {
        case 0:
            pstcACFS10->FSA0 = (uint8_t)pstcConfig->enFrt;
            break;
        case 1:
            pstcACFS10->FSA1 = (uint8_t)pstcConfig->enFrt;
            break;
        case 2:
            pstcACFS32->FSA2 = (uint8_t)pstcConfig->enFrt;
            break;
        case 3:
            pstcACFS32->FSA3 = (uint8_t)pstcConfig->enFrt;
            break;
        case 4:
            pstcACFS54->FSA4 = (uint8_t)pstcConfig->enFrt;
            break;
        case 5:
            pstcACFS54->FSA5 = (uint8_t)pstcConfig->enFrt;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    switch (pstcConfig->enBuf)
    {
        case AdcmpBufDisable:
            pstcACSCx->BUFE = 0u;
            break;
        case AdcmpBufFrtZero:
            pstcACSCx->BUFE = 1u;
            break;
        case AdcmpBufFrtPeak:
            pstcACSCx->BUFE = 2u;
            break;
        case AdcmpBufFrtZeroPeak:
            pstcACSCx->BUFE = 3u;
            break;
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)   
        #if(PDL_MCU_TYPE >= PDL_FM4_TYPE3)    
        case AdcmpBufFrtZeroMszcZero:
            pstcACSCx->APBM = 1u;
            pstcACSCx->BUFE = 1u;
            break;
        case AdcmpBufFrtPeakMspcZero:
            pstcACSCx->APBM = 1u;
            pstcACSCx->BUFE = 2u;
            break;
        case AdcmpBufFrtZeroMszcZeroOrFrtPeakMspcZero:
            pstcACSCx->APBM = 1u;
            pstcACSCx->BUFE = 3u;
            break;
        #endif    
        #endif
        default:
            return ErrorInvalidParameter;
    }
	
    // configure start trigger output channel number
    switch (pstcConfig->enTrigSel)
    {
        case AdcmpTrigAdc0Scan:
            pstcACSCx->ADSEL = 0u;
            break;
        case AdcmpTrigAdc0Prio:
            pstcACSCx->ADSEL = 1u;
            break;
        case AdcmpTrigAdc1Scan:
            pstcACSCx->ADSEL = 2u;
            break;
        case AdcmpTrigAdc1Prio:
            pstcACSCx->ADSEL = 3u;
            break;
        case AdcmpTrigAdc2Scan:
            pstcACSCx->ADSEL = 4u;
            break;
        case AdcmpTrigAdc2Prio:
            pstcACSCx->ADSEL = 5u;
            break;
        default:
            return ErrorInvalidParameter;
    }
	
    // Select ADCMP running mode
    switch (pstcConfig->enMode)
    {
        case AdcmpNormalMode:
            pstcACSDx->AMOD = 0u;
            break;
        case AdcmpOffsetMode:
            pstcACSDx->AMOD = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
	
    // select OCU OCCP register: OCCP(x)    
    switch (pstcConfig->enOccpSel)
    {
        case AdcmpSelOccp0:
            pstcACSDx->OCUS = 0u;
            break;
        case AdcmpSelOccp1:
            pstcACSDx->OCUS = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
#if (PDL_MCU_TYPE >= PDL_FM4_TYPE3)
    // Set the comparison with FRT interrupt mask counter
    pstcACMCx->MZCE = ((pstcConfig->bCompareFrtZeroMaskCntVal == TRUE) ? 1u : 0u);
    pstcACMCx->MPCE = ((pstcConfig->bCompareFrtPeakMaskCntVal == TRUE) ? 1u : 0u);  
    
    if(pstcConfig->u8CompareVal > 15u)
    {
        return ErrorInvalidParameter;
    }
    
    pstcACMCx->AMC = pstcConfig->u8CompareVal;
#endif      
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Device dependent De-initialization of Mft adcmp module
 **       
 ** \param [in] pstcMftAdcmp    Pointer to Mft instance
 ** \param [in] u8Ch            Mft adcmp channel
 ** \arg        MFT_ADCMP_CH0 ADCMP channel 0
 ** \arg        MFT_ADCMP_CH1 ADCMP channel 1
 ** \arg        MFT_ADCMP_CH2 ADCMP channel 2
 ** \arg        MFT_ADCMP_CH3 ADCMP channel 3
 ** \arg        MFT_ADCMP_CH4 ADCMP channel 4
 ** \arg        MFT_ADCMP_CH5 ADCMP channel 5
 **
 ** \retval Ok                        Adcmp De-initialized 
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **                                   - pstcMftAdcmp == NULL
 **                                   - u8Ch > MFT_ADCMP_CH_MAX
 ******************************************************************************/
en_result_t Mft_Adcmp_DeInit(volatile stc_mftn_adcmp_t* pstcMftAdcmp, 
                             uint8_t u8Ch)
{
    volatile stc_mft_adcmp_acfs10_field_t* pstcACFS10 ;
    volatile stc_mft_adcmp_acfs32_field_t* pstcACFS32 ;
    volatile stc_mft_adcmp_acfs54_field_t* pstcACFS54 ;
    
    volatile uint8_t*   pu8ACSCx ;
    volatile uint8_t*   pu8ACSDx ;
    
    if ((NULL == pstcMftAdcmp) || (u8Ch > MFT_ADCMP_CH_MAX))
    {
        return ErrorInvalidParameter ;
    }

    // Get actual address of register list of current channel
    pstcACFS10 = (volatile stc_mft_adcmp_acfs10_field_t*)(&pstcMftAdcmp->ACFS10);
    pstcACFS32 = (volatile stc_mft_adcmp_acfs32_field_t*)(&pstcMftAdcmp->ACFS32);
    pstcACFS54 = (volatile stc_mft_adcmp_acfs54_field_t*)(&pstcMftAdcmp->ACFS54);
    
    pu8ACSCx = (volatile uint8_t*)(&pstcMftAdcmp->ACSC0 + 0x4*u8Ch);
    pu8ACSDx = (volatile uint8_t*)(&pstcMftAdcmp->ACSD0 + 0x4*u8Ch);
    
    // Select FRT channel : FRTx -> ADCMPx
    switch(u8Ch)
    {
        case 0u:
            pstcACFS10->FSA0 = 0u;
            break;
        case 1u:
            pstcACFS10->FSA1 = 0u;
            break;
        case 2u:
            pstcACFS32->FSA2 = 0u;
            break;
        case 3u:
            pstcACFS32->FSA3 = 0u;
            break;
        case 4u:
            pstcACFS54->FSA4 = 0u;
            break;
        case 5u:
            pstcACFS54->FSA5 = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    *pu8ACSCx = 0u;
    *pu8ACSDx = 0u;
    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Enable Mft Adcmp operations
 **
 ** \param [in] pstcMftAdcmp  Pointer to Mft instance
 ** \param [in] u8Ch          Mft Adcmp channel
 ** \arg        MFT_ADCMP_CH0 ADCMP channel 0
 ** \arg        MFT_ADCMP_CH1 ADCMP channel 1
 ** \arg        MFT_ADCMP_CH2 ADCMP channel 2
 ** \arg        MFT_ADCMP_CH3 ADCMP channel 3
 ** \arg        MFT_ADCMP_CH4 ADCMP channel 4
 ** \arg        MFT_ADCMP_CH5 ADCMP channel 5
 ** \param [in] pstcFunc      Pointer to Mft Adcmp function structure
 **          
 ** \retval Ok                     Functions enabled 
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                                - pstcMftAdcmp == NULL
 **                                - pstcFunc == NULL
 **                                - u8Ch > MFT_ADCMP_CH_MAX
 ******************************************************************************/
en_result_t Mft_Adcmp_EnableOperation(volatile stc_mftn_adcmp_t *pstcMftAdcmp, 
                                      uint8_t u8Ch, 
                                      stc_mft_adcmp_func_t* pstcFunc)
{
    volatile stc_mft_adcmp_acsd0_field_t*   pstcACSDx ;
	
    // Check for NULL pointer and channel parameter
    if ((NULL == pstcMftAdcmp)||(NULL == pstcFunc)||(u8Ch > MFT_ADCMP_CH_MAX))
    {
        return ErrorInvalidParameter ;
    }  
    // Get actual address of register list of current channel
    pstcACSDx = (volatile stc_mft_adcmp_acsd0_field_t*)(&pstcMftAdcmp->ACSD0 + 0x4u*u8Ch);
	
    pstcACSDx->PE = pstcFunc->bPeakEn;
    pstcACSDx->ZE = pstcFunc->bZeroEn;
    pstcACSDx->UE = pstcFunc->bUpEn;
    pstcACSDx->DE = pstcFunc->bDownEn;
	
    return Ok;
}
/**
 ******************************************************************************
 ** \brief Disable Mft Adcmp operations
 **
 ** \param [in] pstcMftAdcmp  Pointer to Mft instance
 ** \param [in] u8Ch          Mft Adcmp channel
 ** \arg        MFT_ADCMP_CH0 ADCMP channel 0
 ** \arg        MFT_ADCMP_CH1 ADCMP channel 1
 ** \arg        MFT_ADCMP_CH2 ADCMP channel 2
 ** \arg        MFT_ADCMP_CH3 ADCMP channel 3
 ** \arg        MFT_ADCMP_CH4 ADCMP channel 4
 ** \arg        MFT_ADCMP_CH5 ADCMP channel 5
 ** \param [in] pstcFunc      Pointer to Mft Adcmp function structure
 ** \retval Ok                     Mft Adcmp operations disabled
 ** \retval ErrorInvalidParameter  If one of following conditions are met:
 **                                - pstcMftAdcmp == NULL
 **                                - pstcFunc == NULL
 **                                - u8Ch > MFT_ADCMP_CH_MAX
 ******************************************************************************/
en_result_t Mft_Adcmp_DisableOperation(volatile stc_mftn_adcmp_t *pstcMftAdcmp, 
                                       uint8_t u8Ch, 
                                       stc_mft_adcmp_func_t* pstcFunc)
{
    volatile stc_mft_adcmp_acsd0_field_t*   pstcACSDx;
	
    // Check for NULL pointer and channel parameter
    if ((NULL == pstcMftAdcmp)||(NULL == pstcFunc)||(u8Ch > MFT_ADCMP_CH_MAX))
    {
        return ErrorInvalidParameter ;
    }  
    // Get actual address of register list of current channel
    pstcACSDx = (volatile stc_mft_adcmp_acsd0_field_t*)(&pstcMftAdcmp->ACSD0 + 0x4u*u8Ch);
	
    pstcACSDx->PE = pstcFunc->bPeakEn;
    pstcACSDx->ZE = pstcFunc->bZeroEn;
    pstcACSDx->UE = pstcFunc->bUpEn;
    pstcACSDx->DE = pstcFunc->bDownEn;
	
    return Ok;
}
/**
 ******************************************************************************
 ** \brief Write compare or offset value to ADCMP
 **
 ** \param [in] pstcMftAdcmp  Pointer to Mft instance
 ** \param [in] u8Ch          Mft Adcmp channel
 ** \arg        MFT_ADCMP_CH0 ADCMP channel 0
 ** \arg        MFT_ADCMP_CH1 ADCMP channel 1
 ** \arg        MFT_ADCMP_CH2 ADCMP channel 2
 ** \arg        MFT_ADCMP_CH3 ADCMP channel 3
 ** \arg        MFT_ADCMP_CH4 ADCMP channel 4
 ** \arg        MFT_ADCMP_CH5 ADCMP channel 5
 ** \param [in] u16AdcmpVal ADCMP compare value
 **
 ** \retval Ok                        Compare or offset value to ADCMP is set
 ** \retval ErrorInvalidParameter     If one of following condiitons are met:
 **                                   - pstcMftAdcmp == NULL
 **                                   - u8Ch > MFT_ADCMP_CH_MAX
 ******************************************************************************/
en_result_t Mft_Adcmp_WriteAcmp(volatile stc_mftn_adcmp_t *pstcMftAdcmp, uint8_t u8Ch, 
                                uint16_t u16AdcmpVal)
{
    volatile uint16_t*   pu16AdcmpAcpmReg;
	
    // Check for NULL pointer and channel parameter
    if ((NULL == pstcMftAdcmp)||(u8Ch > MFT_ADCMP_CH_MAX))
    {
        return ErrorInvalidParameter ;
    }
    pu16AdcmpAcpmReg = (volatile uint16_t*)(&pstcMftAdcmp->ACMP0 + 0x2u*u8Ch);
    *pu16AdcmpAcpmReg =  u16AdcmpVal;
	
    return Ok;
}
/**
 ******************************************************************************
 ** \brief Read compare and offset value of ACMP
 **
 ** \param [in] pstcMftAdcmp Pointer to Mft instance
 ** \param [in] u8Ch         Mft Adcmp channel
 ** \arg        MFT_ADCMP_CH0 ADCMP channel 0
 ** \arg        MFT_ADCMP_CH1 ADCMP channel 1
 ** \arg        MFT_ADCMP_CH2 ADCMP channel 2
 ** \arg        MFT_ADCMP_CH3 ADCMP channel 3
 ** \arg        MFT_ADCMP_CH4 ADCMP channel 4
 ** \arg        MFT_ADCMP_CH5 ADCMP channel 5
 **
 ** \return  Value of register ACMP
 **
 ******************************************************************************/
uint16_t Mft_Adcmp_ReadAcmp(volatile stc_mftn_adcmp_t *pstcMftAdcmp, uint8_t u8Ch)
{
    volatile uint16_t*   pu16AdcmpAcpmReg ;
    uint16_t u16Data;
	
    // Check for NULL pointer and channel parameter
    if ((NULL == pstcMftAdcmp)||(u8Ch > MFT_ADCMP_CH_MAX))
    {
        return ErrorInvalidParameter ;
    }  
    pu16AdcmpAcpmReg =  (volatile uint16_t*)(&pstcMftAdcmp->ACMP0 + 0x2u*u8Ch);
    u16Data = *pu16AdcmpAcpmReg;
    
    // return data value of ACMP register
    return u16Data;
}
#endif
/**
 ******************************************************************************
 ** \brief Mft Adcmp fm3 compatible initialization
 **
 ** \param [in] pstcMftAdcmp Pointer to Mft instance
 ** \param [in] u8FM3Ch   Mft Adcmp couple channel
 ** \arg        MFT_ADCMP_CH10  The couple channel of ch.0 and ch.1
 ** \arg        MFT_ADCMP_CH32  The couple channel of ch.2 and ch.3
 ** \arg        MFT_ADCMP_CH54  The couple channel of ch.4 and ch.5
 ** \param [in] pstcConfig   Mft Adcmp configuration parameter
 **
 ** \retval Ok                      Mft Adcmp fm3 compatible mode initialized
 ** \retval ErrorInvalidParameter   Invalid setting of pstcConfig elements
 ******************************************************************************/
en_result_t Mft_Adcmp_Init_Fm3(volatile stc_mftn_adcmp_t *pstcMftAdcmp, 
                               uint8_t u8FM3Ch,
                               const stc_mft_adcmp_config_fm3_t *pstcConfig)
{
    volatile stc_mft_adcmp_acsa_field_t*   pstcACSA ;
#if (PDL_MCU_CORE == PDL_FM3_CORE)     
    volatile uint8_t*   pu8ACSB ;
    volatile stc_mft_adcmp_atsa_field_t*   pstcATSA ;
#endif    
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
    volatile stc_mft_adcmp_acsc0_field_t*   pstcACSC0 ;
    volatile stc_mft_adcmp_acsc0_field_t*   pstcACSC1 ;
    volatile stc_mft_adcmp_acfs10_field_t* pstcACFS10 ;
    volatile stc_mft_adcmp_acfs32_field_t* pstcACFS32 ;
    volatile stc_mft_adcmp_acfs54_field_t* pstcACFS54 ;

#endif    
    // Get actual address of register list of current channel
    pstcACSA = (volatile stc_mft_adcmp_acsa_field_t*)(&pstcMftAdcmp->ACSA);
#if (PDL_MCU_CORE == PDL_FM3_CORE)   
    pu8ACSB  = (volatile uint8_t*)(&pstcMftAdcmp->ACSB);     
    pstcATSA = (volatile stc_mft_adcmp_atsa_field_t*)(&pstcMftAdcmp->ATSA); 
#endif    
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
    pstcACSC0 = (volatile stc_mft_adcmp_acsc0_field_t*)(&pstcMftAdcmp->ACSC0 + 0x8u*u8FM3Ch);
    pstcACSC1 = (volatile stc_mft_adcmp_acsc0_field_t*)(&pstcMftAdcmp->ACSC0 + 0x8u*u8FM3Ch + 0x04u);
    pstcACFS10 = (volatile stc_mft_adcmp_acfs10_field_t*)(&pstcMftAdcmp->ACFS10);
    pstcACFS32 = (volatile stc_mft_adcmp_acfs32_field_t*)(&pstcMftAdcmp->ACFS32);
    pstcACFS54 = (volatile stc_mft_adcmp_acfs54_field_t*)(&pstcMftAdcmp->ACFS54);
#endif    
    
    switch(u8FM3Ch)
    {
        case MFT_ADCMP_CH10:     
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE) 
            pstcACFS10->FSA0 = (uint8_t)pstcConfig->enFrt;
            pstcACFS10->FSA1 = (uint8_t)pstcConfig->enFrt;
        #else
            pstcACSA->CE0 = (uint8_t)pstcConfig->enFrt + 1u;
        #endif
            break;
        case MFT_ADCMP_CH32:
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)   
            pstcACFS32->FSA2 = (uint8_t)pstcConfig->enFrt;
            pstcACFS32->FSA3 = (uint8_t)pstcConfig->enFrt;
        #else
            pstcACSA->CE1 = (uint8_t)pstcConfig->enFrt + 1u;
        #endif
            break;
        case MFT_ADCMP_CH54:
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)   
            pstcACFS54->FSA4 = (uint8_t)pstcConfig->enFrt;
            pstcACFS54->FSA5 = (uint8_t)pstcConfig->enFrt;
        #else
            pstcACSA->CE2 = (uint8_t)pstcConfig->enFrt + 1u;
        #endif    
            break;
        default:
            return ErrorInvalidParameter;
    }

    // configure buffer transfer type    
    switch (pstcConfig->enBuf)
    {
        case AdcmpBufDisable:
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)     
            pstcACSC0->BUFE = 0u;
            pstcACSC1->BUFE = 0u;
        #else
            *pu8ACSB |= (1u << u8FM3Ch);
        #endif    
            break;
        case AdcmpBufFrtZero:
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)   
            pstcACSC0->BUFE = 1u;
            pstcACSC1->BUFE = 1u;
        #else
            *pu8ACSB &= ~(1u << (4u + u8FM3Ch));
            *pu8ACSB &= ~(1u << u8FM3Ch);    
        #endif
            break;
        case AdcmpBufFrtPeak:
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)   
            pstcACSC0->BUFE = 2u;
            pstcACSC1->BUFE = 2u;
        #else
            *pu8ACSB |= (1u << (4u + u8FM3Ch));
            *pu8ACSB &= ~(1u << u8FM3Ch);   
        #endif    
            break;
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
        case AdcmpBufFrtZeroPeak:
            pstcACSC0->BUFE = 3u;
            pstcACSC1->BUFE = 3u;
            break;
        #endif    
        default:
            return ErrorInvalidParameter;
    }   
    // configure start trigger output channel number
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)       
    switch (pstcConfig->enTrigSel)
    {
        case AdcmpTrigAdc0Scan:
            pstcACSC0->ADSEL = 0u;
            pstcACSC1->ADSEL = 0u;
            break;
        case AdcmpTrigAdc0Prio:
            pstcACSC0->ADSEL = 1u;
            pstcACSC1->ADSEL = 1u;
            break;
        case AdcmpTrigAdc1Scan:
            pstcACSC0->ADSEL = 2u;
            pstcACSC1->ADSEL = 2u;
            break;
        case AdcmpTrigAdc1Prio:
            pstcACSC0->ADSEL = 3u;
            pstcACSC1->ADSEL = 3u;
            break;
        case AdcmpTrigAdc2Scan:
            pstcACSC0->ADSEL = 4u;
            pstcACSC1->ADSEL = 4u;
            break;
        case AdcmpTrigAdc2Prio:
            pstcACSC0->ADSEL = 5u;
            pstcACSC1->ADSEL = 5u;
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    // configure Adcmp Fm3 mode
    switch(u8FM3Ch)
    {
        case MFT_ADCMP_CH10:            
            pstcACSA->SEL10 = (uint8_t)pstcConfig->enMode;
            break;
        case MFT_ADCMP_CH32:
            pstcACSA->SEL32 = (uint8_t)pstcConfig->enMode;
            break;
        case MFT_ADCMP_CH54:
            pstcACSA->SEL54 = (uint8_t)pstcConfig->enMode;
            break;
        default:
            return ErrorInvalidParameter;
    }
#else
    switch(u8FM3Ch)
    {
        case MFT_ADCMP_CH10:
            pstcATSA->AD0S = ((pstcConfig->bAdcScanTriggerSel == TRUE) ? 1u : 0u);
            pstcATSA->AD0P = ((pstcConfig->bAdcPrioTriggerSel == TRUE) ? 1u : 0u);
            break;
        case MFT_ADCMP_CH32:
            pstcATSA->AD1S = ((pstcConfig->bAdcScanTriggerSel == TRUE) ? 1u : 0u);
            pstcATSA->AD1P = ((pstcConfig->bAdcPrioTriggerSel == TRUE) ? 1u : 0u);
            break;
        case MFT_ADCMP_CH54:
            pstcATSA->AD2S = ((pstcConfig->bAdcScanTriggerSel == TRUE) ? 1u : 0u);
            pstcATSA->AD2P = ((pstcConfig->bAdcPrioTriggerSel == TRUE) ? 1u : 0u);
            break;
        default:
            return ErrorInvalidParameter;
    }

    // configure Adcmp Fm3 mode
    switch(u8FM3Ch)
    {
        case MFT_ADCMP_CH10:            
            pstcACSA->SEL0 = (uint8_t)pstcConfig->enMode;
            break;
        case MFT_ADCMP_CH32:
            pstcACSA->SEL1 = (uint8_t)pstcConfig->enMode;
            break;
        case MFT_ADCMP_CH54:
            pstcACSA->SEL2 = (uint8_t)pstcConfig->enMode;
            break;
        default:
            return ErrorInvalidParameter;
    }
#endif    
    return Ok;
}

/**
 ******************************************************************************
 ** \brief Mft Adcmp fm3 compatible de-initialization
 **
 ** \param [in] pstcMftAdcmp Pointer to Mft instance
 ** \param [in] u8FM3Ch   Mft Adcmp couple channel
 ** \arg        MFT_ADCMP_CH10  The couple channel of ch.0 and ch.1
 ** \arg        MFT_ADCMP_CH32  The couple channel of ch.2 and ch.3
 ** \arg        MFT_ADCMP_CH54  The couple channel of ch.4 and ch.5
 **
 ** \retval Ok                      Mft Adcmp fm3 compatible mode de-initialized
 ** \retval ErrorInvalidParameter   Invalid setting of u8FM3Ch
 ******************************************************************************/
en_result_t Mft_Adcmp_DeInit_Fm3(volatile stc_mftn_adcmp_t *pstcMftAdcmp, 
                                 uint8_t u8FM3Ch)
{
    volatile stc_mft_adcmp_acsa_field_t*   pstcACSA ;
#if (PDL_MCU_CORE == PDL_FM3_CORE)     
    volatile stc_mft_adcmp_acsb_field_t*   pstcACSB ;
#endif    
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
    volatile uint8_t*   pu8ACSC0 ;
    volatile uint8_t*   pu8ACSC1 ;
    volatile uint8_t* pu8ACFS10 ;
    volatile uint8_t* pu8ACFS32 ;
    volatile uint8_t* pu8ACFS54 ;
#endif    
    
    // Get actual address of register list of current channel
    pstcACSA = (volatile stc_mft_adcmp_acsa_field_t*)(&pstcMftAdcmp->ACSA);
#if (PDL_MCU_CORE == PDL_FM3_CORE)   
    pstcACSB  = (volatile stc_mft_adcmp_acsb_field_t*)(&pstcMftAdcmp->ACSB);     
#endif    
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
    pu8ACSC0 = (volatile uint8_t*)(&pstcMftAdcmp->ACSC0 + 0x8u*u8FM3Ch);
    pu8ACSC1 = (volatile uint8_t*)(&pstcMftAdcmp->ACSC0 + 0x8u*u8FM3Ch + 4u);
    pu8ACFS10 = (volatile uint8_t*)(&pstcMftAdcmp->ACFS10);
    pu8ACFS32 = (volatile uint8_t*)(&pstcMftAdcmp->ACFS32);
    pu8ACFS54 = (volatile uint8_t*)(&pstcMftAdcmp->ACFS54);
#endif 

#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
    *pu8ACSC0  = 0u;
    *pu8ACSC1  = 0u;
#endif    
    
    switch(u8FM3Ch)
    {
        case MFT_ADCMP_CH10:
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
            pstcACSA->CE10 = 0u;
            pstcACSA->SEL10 = 0u;
            *pu8ACFS10 = 0u;
        #else
            pstcACSA->CE0 = 0u;
            pstcACSA->SEL0 = 0u;
            pstcACSB->BDIS0 = 0u;
            pstcACSB->BTS0 = 0u;
        #endif        
            break;
        case MFT_ADCMP_CH32:
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)
            pstcACSA->CE32 = 0u;
            pstcACSA->SEL32 = 0u;
            *pu8ACFS32 = 0u;
        #else
            pstcACSA->CE1 = 0u;
            pstcACSA->SEL1 = 0u;
            pstcACSB->BDIS1 = 0u;
            pstcACSB->BTS1 = 0u;
        #endif     
            break;
        case MFT_ADCMP_CH54:
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)
            pstcACSA->CE54 = 0u;
            pstcACSA->SEL54 = 0u;  
            *pu8ACFS54 = 0u;
        #else
            pstcACSA->CE2 = 0u;
            pstcACSA->SEL2 = 0u;
            pstcACSB->BDIS2 = 0u;
            pstcACSB->BTS2 = 0u;
        #endif    
            break;
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
    
}

/**
 ******************************************************************************
 ** \brief Mft Adcmp fm3 compatible mode configuration and enable operation
 **
 ** \param [in] pstcMftAdcmp Pointer to Mft instance
 ** \param [in] u8FM3Ch   Mft Adcmp channel
 ** \arg        MFT_ADCMP_CH10  The couple channel of ch.0 and ch.1
 ** \arg        MFT_ADCMP_CH32  The couple channel of ch.2 and ch.3
 ** \arg        MFT_ADCMP_CH54  The couple channel of ch.4 and ch.5
 ** \retval Ok                      ADCMP FM3 mode funcitons enabled
 ** \retval ErrorInvalidParameter   If one of following conditions are met:
 **                                 - pstcMftAdcmp == NULL
 **                                 - u8CoupleCh > MFT_ADCMP_CPCH_MAX
 ******************************************************************************/
en_result_t 
Mft_Adcmp_EnableOperation_Fm3(volatile stc_mftn_adcmp_t *pstcMftAdcmp, 
                              uint8_t u8FM3Ch)
{
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)   
    volatile stc_mft_adcmp_acsa_field_t*   pstcACSA ;
    // Get actual address of register list of current channel
    pstcACSA = (volatile stc_mft_adcmp_acsa_field_t*)(&pstcMftAdcmp->ACSA);
#endif
    
    // Check for NULL pointer and channel parameter
    if ((NULL == pstcMftAdcmp)||(u8FM3Ch > MFT_ADCMP_CPCH_MAX))
    {
        return ErrorInvalidParameter ;
    }
   
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)       
    switch(u8FM3Ch)
    {
        case MFT_ADCMP_CH10:
            pstcACSA->CE10 = 1u;
            break;
        case MFT_ADCMP_CH32:
            pstcACSA->CE32 = 1u;
            break;
        case MFT_ADCMP_CH54:
            pstcACSA->CE54 = 1u;
            break;
        default:
            return ErrorInvalidParameter;
    }
#endif    
    return Ok;
}
/**
 ******************************************************************************
 ** \brief De-configurate Mft Adcmp fm3 compatible function and disable operation
 **
 ** \param [in] pstcMftAdcmp Pointer to Mft instance
 ** \param [in] u8FM3Ch   Mft Adcmp channel
 ** \arg        MFT_ADCMP_CH10  The couple channel of ch.0 and ch.1
 ** \arg        MFT_ADCMP_CH32  The couple channel of ch.2 and ch.3
 ** \arg        MFT_ADCMP_CH54  The couple channel of ch.4 and ch.5
 **
 ** \retval Ok                      ADCMP FM3 operation disabled
 ** \retval ErrorInvalidParameter   If one of followings conditions are met:
 **                                 - pstcMftAdcmp == NULL
 **                                 - u8CoupleCh > MFT_ADCMP_CPCH_MAX
 **
 ******************************************************************************/
en_result_t 
Mft_Adcmp_DisableOperation_Fm3(volatile stc_mftn_adcmp_t *pstcMftAdcmp, 
                               uint8_t u8FM3Ch)
{
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)      
    volatile stc_mft_adcmp_acsa_field_t*   pstcACSA;
    // Get actual address of register list of current channel
    pstcACSA = (volatile stc_mft_adcmp_acsa_field_t*)(&pstcMftAdcmp->ACSA);
#endif    

    // Check for NULL pointer and channel parameter
    if ((NULL == pstcMftAdcmp)||(u8FM3Ch > MFT_ADCMP_CPCH_MAX))
    {
        return ErrorInvalidParameter ;
    }
        
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)      
    switch(u8FM3Ch)
    {
        case MFT_ADCMP_CH10:
            pstcACSA->CE10 = 0u;
            break;
        case MFT_ADCMP_CH32:
            pstcACSA->CE32 = 0u;
            break;
        case MFT_ADCMP_CH54:
            pstcACSA->CE54 = 0u;
            break;
        default:
            return ErrorInvalidParameter;
    }
#endif    
    
    return Ok;
}
/**
 ******************************************************************************
 ** \brief Mft Adcmp fm3 compatible mode, write Accp register
 **
 ** \param [in] pstcMftAdcmp Pointer to Mft instance
 ** \param [in] u8FM3Ch   Mft Adcmp channel
 ** \arg        MFT_ADCMP_CH10  The couple channel of ch.0 and ch.1
 ** \arg        MFT_ADCMP_CH32  The couple channel of ch.2 and ch.3
 ** \arg        MFT_ADCMP_CH54  The couple channel of ch.4 and ch.5
 ** \param [in] u16AccpVal      Mft Adcmp configuration parameter
 **
 ** \retval  Ok                      Accp is written
 ** \retval  ErrorInvalidParameter   If one of following conditions are met:
 **                                  - pstcMftAdcmp == NULL
 **                                  - u8CoupleCh > MFT_ADCMP_CPCH_MAX
 ******************************************************************************/
en_result_t Mft_Adcmp_WriteAccp_Fm3(volatile stc_mftn_adcmp_t *pstcMftAdcmp, 
                                    uint8_t u8FM3Ch, uint16_t u16AccpVal)
{
    // Check for NULL pointer and channel parameter
    if ((NULL == pstcMftAdcmp) || (u8FM3Ch > MFT_ADCMP_CPCH_MAX))
    {
        return ErrorInvalidParameter ;
    }
    
    switch (u8FM3Ch)
    {
        case MFT_ADCMP_CH10:
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE) 
            pstcMftAdcmp->ACMP0 = u16AccpVal;
        #else
            pstcMftAdcmp->ACCP0 = u16AccpVal;
        #endif
            break;
        case MFT_ADCMP_CH32:
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE) 
            pstcMftAdcmp->ACMP2 = u16AccpVal;
        #else
            pstcMftAdcmp->ACCP1 = u16AccpVal;
        #endif  
            break;
        case MFT_ADCMP_CH54:
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE) 
            pstcMftAdcmp->ACMP4 = u16AccpVal;
        #else
            pstcMftAdcmp->ACCP2 = u16AccpVal;
        #endif  
            break;
        default:
            return ErrorInvalidParameter ;
    }

    return Ok;
}
/**
 ******************************************************************************
 ** \brief Mft Adcmp fm3 compatible mode, read Accp register stored value
 **
 ** \param [in] pstcMftAdcmp Pointer to Mft instance
 ** \arg        structure of Mft_Adcmp
 ** \param [in] u8FM3Ch   Mft Adcmp channel
 ** \arg        MFT_ADCMP_CH10  The couple channel of ch.0 and ch.1
 ** \arg        MFT_ADCMP_CH32  The couple channel of ch.2 and ch.3
 ** \arg        MFT_ADCMP_CH54  The couple channel of ch.4 and ch.5
 ** \return     Data stored in Accp register
 **
 ******************************************************************************/
uint16_t Mft_Adcmp_ReadAccp_Fm3(volatile stc_mftn_adcmp_t *pstcMftAdcmp, 
                                uint8_t u8FM3Ch)
{
    uint16_t u16Data;

    // Check for NULL pointer and channel parameter
    if ((NULL == pstcMftAdcmp)||(u8FM3Ch > MFT_ADCMP_CPCH_MAX))
    {
        return ErrorInvalidParameter ;
    }

    switch (u8FM3Ch)
    {
        case MFT_ADCMP_CH10:
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)             
            u16Data = pstcMftAdcmp->ACMP0;
        #else
            u16Data = pstcMftAdcmp->ACCP0;
        #endif
            break;
        case MFT_ADCMP_CH32:
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)             
            u16Data = pstcMftAdcmp->ACMP2;
        #else
            u16Data = pstcMftAdcmp->ACCP1;
        #endif  
            break;
        case MFT_ADCMP_CH54:
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)             
            u16Data = pstcMftAdcmp->ACMP4;
        #else
            u16Data = pstcMftAdcmp->ACCP2;
        #endif    
            break;  
        default:
            return ErrorInvalidParameter ;
    }
    
    // return data value of ACMP register
    return u16Data;
}
/**
 ******************************************************************************
 ** \brief Mft Adcmp fm3 compatible mode, write Accpdn register
 **
 ** \param [in] pstcMftAdcmp Pointer to Mft instance
 ** \arg        structure of Mft_Adcmp
 ** \param [in] u8FM3Ch   Mft Adcmp channel
 ** \arg        MFT_ADCMP_CH10  The couple channel of ch.0 and ch.1
 ** \arg        MFT_ADCMP_CH32  The couple channel of ch.2 and ch.3
 ** \arg        MFT_ADCMP_CH54  The couple channel of ch.4 and ch.5
 ** \param [in] u16AccpdnVal    data value of Accpdn
 **
 ** \retval  Ok                      Accpdn is written
 ** \retval  ErrorInvalidParameter   If one of following conditions are met:
 **                                  - pstcMftAdcmp == NULL
 **                                  - u8CoupleCh > MFT_ADCMP_CPCH_MAX
 ******************************************************************************/
en_result_t Mft_Adcmp_WriteAccpdn_Fm3(volatile stc_mftn_adcmp_t *pstcMftAdcmp, 
                                      uint8_t u8FM3Ch, 
                                      uint16_t u16AccpdnVal)
{
    // Check for NULL pointer and channel parameter
    if ((NULL == pstcMftAdcmp)||(u8FM3Ch > MFT_ADCMP_CPCH_MAX))
    {
        return ErrorInvalidParameter ;
    }
    
    switch (u8FM3Ch)
    {
        case MFT_ADCMP_CH10:
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE) 
            pstcMftAdcmp->ACMP1 = u16AccpdnVal;
        #else
            pstcMftAdcmp->ACCPDN0 = u16AccpdnVal;
        #endif
            break;
        case MFT_ADCMP_CH32:
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE) 
            pstcMftAdcmp->ACMP3 = u16AccpdnVal;
        #else
            pstcMftAdcmp->ACCPDN1 = u16AccpdnVal;
        #endif  
            break;
        case MFT_ADCMP_CH54:
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE) 
            pstcMftAdcmp->ACMP5 = u16AccpdnVal;
        #else
            pstcMftAdcmp->ACCPDN2 = u16AccpdnVal;
        #endif  
            break;
        default:
            return ErrorInvalidParameter ;
    }

    return Ok;
}
/**
 ******************************************************************************
 ** \brief Mft Adcmp fm3 compatible mode, read Accpdn register stored value
 **
 ** \param [in] pstcMftAdcmp Pointer to Mft instance
 ** \arg        structure of Mft_Adcmp
 ** \param [in] u8FM3Ch   Mft Adcmp channel
 ** \arg        MFT_ADCMP_CH10  The couple channel of ch.0 and ch.1
 ** \arg        MFT_ADCMP_CH32  The couple channel of ch.2 and ch.3
 ** \arg        MFT_ADCMP_CH54  The couple channel of ch.4 and ch.5
 ** \return    Data stored in Accpdn register
 **
 ******************************************************************************/
uint16_t Mft_Adcmp_ReadAccpdn_Fm3(volatile stc_mftn_adcmp_t *pstcMftAdcmp, 
                                  uint8_t u8FM3Ch)
{
    uint16_t u16Data;

    // Check for NULL pointer and channel parameter
    if ((NULL == pstcMftAdcmp)||(u8FM3Ch > MFT_ADCMP_CPCH_MAX))
    {
        return ErrorInvalidParameter ;
    }
    
    switch (u8FM3Ch)
    {
        case MFT_ADCMP_CH10:
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)             
            u16Data = pstcMftAdcmp->ACMP1;
        #else
            u16Data = pstcMftAdcmp->ACCPDN0;
        #endif
            break;
        case MFT_ADCMP_CH32:
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)             
            u16Data = pstcMftAdcmp->ACMP3;
        #else
            u16Data = pstcMftAdcmp->ACCPDN1;
        #endif  
            break;
        case MFT_ADCMP_CH54:
        #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)             
            u16Data = pstcMftAdcmp->ACMP5;
        #else
            u16Data = pstcMftAdcmp->ACCPDN2;
        #endif    
            break;  
        default:
            return ErrorInvalidParameter ;
    }
    
    // return data value of ACMP register
    return u16Data;
}

#endif

