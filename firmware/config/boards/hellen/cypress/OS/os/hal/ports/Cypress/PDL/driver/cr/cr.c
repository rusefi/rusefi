/******************************************************************************
* \file             cr.c
*
* \version          1.20
*
* \brief            High-speed Clock Rate driver
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
#include "cr/cr.h"

#if (defined(PDL_PERIPHERAL_CR_ACTIVE))


/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/
#define CR_REG_UNLOCK_CODE        0x1ACCE554u
#define CR_REG_LOCK_CODE          0u  
   
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

/**
 ******************************************************************************
 ** \brief   Set the frequency division of CR output to Base timer
 **
 ** \param   [in]  enCrDiv            CR division
 ** \arg           CrFreqDivBy4
 ** \arg           CrFreqDivBy8
 ** \arg           CrFreqDivBy16
 ** \arg           CrFreqDivBy32
 ** \arg           CrFreqDivBy64
 ** \arg           CrFreqDivBy128
 ** \arg           CrFreqDivBy256
 ** \arg           CrFreqDivBy512
 **
 ** \retval  Ok                       Set the frequency division successfully
 ** \retval  ErrorInvalidParameter    enCrDiv > CrFreqDivBy512
 **
 ** \note The division values of 64,128,256,512 are only supported by FM3 TYPE3,
 **       TYPE7, FM4 and FM0+ products.
 **                                     
 ******************************************************************************/
en_result_t Cr_SetFreqDiv(en_cr_freq_div_t enCrDiv)
{
    switch (enCrDiv)
    {
        case CrFreqDivBy4:
            FM_CRTRIM->MCR_PSR = 0u;
            break;
        case CrFreqDivBy8:
            FM_CRTRIM->MCR_PSR = 1u;
            break;
        case CrFreqDivBy16:
            FM_CRTRIM->MCR_PSR = 2u;
            break;
        case CrFreqDivBy32:
            FM_CRTRIM->MCR_PSR = 3u;
            break;
#if (PDL_MCU_TYPE == PDL_FM3_TYPE3) || (PDL_MCU_TYPE == PDL_FM3_TYPE7) || \
    (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)            
        case CrFreqDivBy64:
            FM_CRTRIM->MCR_PSR = 4u;
            break;
        case CrFreqDivBy128:
            FM_CRTRIM->MCR_PSR = 5u;
            break;
        case CrFreqDivBy256:
            FM_CRTRIM->MCR_PSR = 6u;
            break;
        case CrFreqDivBy512:
            FM_CRTRIM->MCR_PSR = 7u;
            break;
#endif
        default:
            return ErrorInvalidParameter;
    }
    
    return Ok;
}

#if (PDL_MCU_TYPE == PDL_FM3_TYPE8)  || (PDL_MCU_TYPE == PDL_FM3_TYPE9)  || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE10) || (PDL_MCU_TYPE == PDL_FM3_TYPE11) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE12) || (PDL_MCU_CORE == PDL_FM0P_CORE)  || \
    (PDL_MCU_CORE == PDL_FM4_CORE)  
/**
 ******************************************************************************
 ** \brief   Set CR temperature trimming register
 **
 ** \param   [in]  u8Data             temperature trimming value, only Bit[4:0]
 **                                   is valid.
 **
 ** \retval  Ok                       Set the frequency division successfully
 **                                     
 ******************************************************************************/
en_result_t Cr_SetTempTrimmingData(uint8_t u8Data)
{
    FM_CRTRIM->MCR_RLR = CR_REG_UNLOCK_CODE;
    FM_CRTRIM->MCR_TTRM = u8Data & 0x1Fu;
    FM_CRTRIM->MCR_RLR = CR_REG_LOCK_CODE;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief   Get CR temperature trimming register      
 **
 ** \retval  temperature trimming value, only Bit[4:0] is valid.                       
 **                                     
 ******************************************************************************/
uint8_t Cr_GetTempTrimmingData(void)
{
    uint8_t u8CrTempData;
    u8CrTempData = FM_CRTRIM->MCR_TTRM & 0x1Fu;
    return u8CrTempData;
}

#endif

/**
 ******************************************************************************
 ** \brief   Set CR frequency trimming register
 **
 ** \param   [in]  u16Data             temperature trimming value, only Bit[9:0]
 **                                    is valid.
 **
 ** \retval  Ok                        Set CR frequency trimming successfully
 **
 ** \note The meaning of bit field of CR data is different among devices, see
 **       peripheral manual for the details.
 **                                     
 ******************************************************************************/
en_result_t Cr_SetFreqTrimmingData(uint16_t u16Data)
{
    FM_CRTRIM->MCR_RLR = CR_REG_UNLOCK_CODE;
#if (PDL_MCU_TYPE == PDL_FM3_TYPE1) || (PDL_MCU_TYPE == PDL_FM3_TYPE2) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE4) || (PDL_MCU_TYPE == PDL_FM3_TYPE5)   
    FM_CRTRIM->MCR_FTRM = u16Data & 0xFFu;
#else
    FM_CRTRIM->MCR_FTRM = u16Data & 0x3FFu;
#endif    
    FM_CRTRIM->MCR_RLR = CR_REG_LOCK_CODE;
    return Ok;
}

/**
 ******************************************************************************
 ** \brief   Get frequency trimming register
 **
 ** \retval  temperature trimming value, only Bit[9:0]
 **          is valid.
 **                                     
 ******************************************************************************/
uint16_t Cr_GetFreqTrimmingData(void)
{
    uint16_t u16CrFreqData;
#if (PDL_MCU_TYPE == PDL_FM3_TYPE1) || (PDL_MCU_TYPE == PDL_FM3_TYPE2) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE4) || (PDL_MCU_TYPE == PDL_FM3_TYPE5)       
    u16CrFreqData = FM_CRTRIM->MCR_FTRM & 0xFFu;
#else
    u16CrFreqData = FM_CRTRIM->MCR_FTRM & 0x3FFu;
#endif    
    return u16CrFreqData;
}

#endif /* #if (defined(PDL_PERIPHERAL_CR_ACTIVE)) */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
