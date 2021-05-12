/*******************************************************************************
* \file              mft_adcmp.h
* 
* \version           1.20
*
* \brief             Headerfile for MFT_ADCMP functions.
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

#ifndef __MFT_ADCMP_H__
#define __MFT_ADCMP_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_MFT_ADCMP_ACTIVE))

/**
* \defgroup GroupMFT_ADCMP ADC Start Compare Unit (MFT ADCMP)
* \{  
* \defgroup GroupMFT_ADCMP_Macros Macros
* \defgroup GroupMFT_ADCMP_Functions Functions
* \defgroup GroupMFT_ADCMP_DataStructures Data Structures
* \defgroup GroupMFT_ADCMP_Types Enumerated Types
* \}
*/

/**
* \addtogroup GroupMFT_ADCMP
* \{
* The ADC Start Compare Unit (ADCMP) is part of the Multi-function Timer (MFT). 
* The MFT is a collection of peripherals used primarily for motor control. There may be 
* up to three MFTs supported on a particular FM microcontroller. One MFT can control a 
* 3-phase motor. A microcontroller that supports multiple MFTs can control multiple 3-phase motors.<br>
* The peripherals in the MFT are:<br>
* - Free-run Timer (FRT)
* - Output Compare Unit (OCU)
* - Waveform Generator (WFG)
* - Input Capture Unit (ICU)
* - ADC Start Compare (ADCMP)<br>
* There are six channels (0-5) available in the ADCMP. You configure each channel independently. 
* The ADCMP triggers the ADC when the counter value of the selected FRT matches the compare value
* for the ADCMP channel. Using the ADCMP you can synchronize analog signal conversions to the motor 
* control timer. This enables precise control over sensor-based input to the motor.<br>
* You can change the compare value at runtime. The ADCMP supports a buffered compare value. 
* If you use the buffer, the compare value updates at a defined moment (for example, at a zero count 
* or peak count). Otherwise it updates immediately.<br>
* The compare value can be used as an offset counter from a match in the OCU. In this mode a match 
* occurs in the selected OCU channel based. The ADCMP compare value is used as an offset from that
* moment. When the offset count is reached, the ADCMP sends the trigger signal to the selected ADC.<br>
* Each channel can trigger any one of three ADCs, starting either a normal or a priority scan. Multiple 
* ADCMP channels can trigger the same ADC, enabling multiple start times for the same ADC in a single 
* FRT cycle.
*
* \section SectionMFT_ADCMP_ConfigurationConsideration Configuration Consideration
* Each channel can trigger any one of three ADCs, starting either a normal or a priority scan. Multiple ADCMP channels can 
* trigger the same ADC, enabling multiple start times for the same ADC in a single FRT cycle.<br>
* To set up the ADCMP, you provide configuration parameters in the stc_mft_adcmp_config_t structure. 
* For example, you specify the FRT to use, the compare buffer mode, which ADC to trigger 
* (use constants defined in en_adcmp_trig_sel_t), normal or offset mode, the compare value, and so on. 
* If you choose to use offset mode, also specify which OCU channel to use for the initial match that 
* starts the offset counter. The constant AdcmpSelOccp0 selects OCU channel 0, 2, or 4 depending on
* which OCU you specify. The constant AdcmpSelOccp1 selects OCU channel 1, 3, or 5.<br>
* After setting up the configuration structure, call Mft_Adcmp_Init().<br>
* Then call Mft_Adcmp_EnableOperation() to start the ADCMP.<br>
* To change the compare value of any of the six ADCMP channels call Mft_Adcmp_WriteAcmp() with the new value. 
* If you are using buffering, the value updates at the defined moment.
* \section SectionMFT_ADCMP_MoreInfo More Information
* For more information on the MFT ADCMP peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/223041/download">FM0+ Peripheral Manual - Timer Subsystem TRM.pdf</a><br>
* <a href="http://www.cypress.com/file/222986/download">FM4 Peripheral Manual - Timer Subsystem TRM.pdf</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/**
* \addtogroup GroupMFT_ADCMP_Macros
* \{
*/

/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/
#define  MFT_ADCMP_CH0             0u
#define  MFT_ADCMP_CH1             1u
#define  MFT_ADCMP_CH2             2u
#define  MFT_ADCMP_CH3             3u
#define  MFT_ADCMP_CH4             4u
#define  MFT_ADCMP_CH5             5u

#define  MFT_ADCMP_CH10            0u
#define  MFT_ADCMP_CH32            1u
#define  MFT_ADCMP_CH54            2u

#define  MFT_ADCMP_CH0_FM3         MFT_ADCMP_CH10  
#define  MFT_ADCMP_CH1_FM3         MFT_ADCMP_CH32
#define  MFT_ADCMP_CH2_FM3         MFT_ADCMP_CH54

#define  MFT_ADCMP_CH_MAX          5u
#define  MFT_ADCMP_CPCH_MAX        2u
/******************************************************************************
 * Global type definitions
 ******************************************************************************/
#define stc_mftn_adcmp_t   FM_MFT_ADCMP_TypeDef

#define MFT0_ADCMP  (*(volatile stc_mftn_adcmp_t *) FM_MFT0_ADCMP_BASE)
#define MFT1_ADCMP  (*(volatile stc_mftn_adcmp_t *) FM_MFT1_ADCMP_BASE)
#define MFT2_ADCMP  (*(volatile stc_mftn_adcmp_t *) FM_MFT2_ADCMP_BASE)

/** \} GroupMFT_ADCMP_Macros */

/**
* \addtogroup GroupMFT_ADCMP_Types
* \{
*/

/**
 ******************************************************************************
 ** \brief Define Frt channel connect to Adcmp
 ******************************************************************************/  
typedef enum en_adcmp_frt
{
    Frt0ToAdcmp    = 0u, ///< connect Frt channel 0 to Adcmp
    Frt1ToAdcmp    = 1u, ///< connect Frt channel 1 to Adcmp
    Frt2ToAdcmp    = 2u, ///< connect Frt channel 2 to Adcmp
    
}en_adcmp_frt_t;

/**
 ******************************************************************************
 ** \brief Define Frt channel with FM3 mode connect to Adcmp
 ******************************************************************************/  
typedef enum en_adcmp_fm3_frt
{
#if (PDL_MCU_CORE == PDL_FM3_CORE)  
    Frt0ToAdcmpFm3    = 0u, ///< connect Frt channel 0 to Adcmp
#endif    
    Frt1ToAdcmpFm3    = 1u, ///< connect Frt channel 1 to Adcmp
    Frt2ToAdcmpFm3    = 2u, ///< connect Frt channel 2 to Adcmp
    
}en_adcmp_frt_fm3_t;

/**
 ******************************************************************************
 ** \brief Define Adcmp buffer type
 ******************************************************************************/ 
typedef enum en_adcmp_buf
{
    AdcmpBufDisable     = 0u, ///< disable Adcmp buffer function
    AdcmpBufFrtZero     = 1u, ///< transfer buffer of ACMP and ACMC when counter value of Frt connected= 0x0000
    AdcmpBufFrtPeak     = 2u, ///< transfer buffer of ACMP and ACMC when counter value of Frt connected= TCCP
    AdcmpBufFrtZeroPeak = 3u, ///< transfer buffer of ACMP and ACMC both when counter value of Frt connected= 0x0000 and TCCP
#if (PDL_MCU_CORE == PDL_FM4_CORE)
    #if (PDL_MCU_TYPE >= PDL_FM4_TYPE3)    
    AdcmpBufFrtZeroMszcZero = 4u,   ///< transfer buffer of ACMP and ACMC when counter value of Frt connected= 0x0000 and zero value detection mask counter value is 0
    AdcmpBufFrtPeakMspcZero = 5u,   ///< transfer buffer of ACMP and ACMC when counter value of Frt connected= peak and peak value detection mask counter value is 0
    AdcmpBufFrtZeroMszcZeroOrFrtPeakMspcZero = 6u,  ///< transfer buffer of ACMP and ACMC when counter value of Frt connected= 0x0000 and zero value detection mask counter value is 0 
                                                    ///< or counter value of Frt connected= peak and peak value detection mask counter value is 0
    #endif
#endif
}en_adcmp_buf_t;

/**
 ******************************************************************************
 ** \brief Define Mft Adcmp output start trig channel
 ******************************************************************************/ 
typedef enum en_adcmp_trig_channel
{
    AdcmpStartTrig0 = 0u, ///< Outputs ADC start trigger 0
    AdcmpStartTrig1 = 1u, ///< Outputs ADC start trigger 1
    AdcmpStartTrig2 = 2u, ///< Outputs ADC start trigger 2
    AdcmpStartTrig3 = 3u, ///< Outputs ADC start trigger 3
    AdcmpStartTrig4 = 4u, ///< Outputs ADC start trigger 4
    AdcmpStartTrig5 = 5u, ///< Outputs ADC start trigger 5
    AdcmpStartTrig6 = 6u, ///< Outputs ADC start trigger 6
    AdcmpStartTrig7 = 7u, ///< Outputs ADC start trigger 7
  
}en_adcmp_start_trig_t;

/**
 ******************************************************************************
 ** \brief Define Mft Adcmp trig mode
 ******************************************************************************/
typedef enum en_adcmp_trig_sel
{
    AdcmpTrigAdc0Scan = 0u, ///< AdcmpStartTrig0
    AdcmpTrigAdc0Prio = 1u, ///< AdcmpStartTrig1
    AdcmpTrigAdc1Scan = 2u, ///< AdcmpStartTrig2
    AdcmpTrigAdc1Prio = 3u, ///< AdcmpStartTrig3
    AdcmpTrigAdc2Scan = 4u, ///< AdcmpStartTrig4
    AdcmpTrigAdc2Prio = 5u, ///< AdcmpStartTrig5
    
}en_adcmp_trig_sel_t;

/**
 ******************************************************************************
 ** \brief Define Adcmp running mode
 ******************************************************************************/ 
typedef enum en_adcmp_mode
{
    AdcmpNormalMode = 0u, ///< select Adcmp Normal mode
    AdcmpOffsetMode = 1u, ///< select Adcmp Offset mode
    
}en_adcmp_mode_t;

/**
 ******************************************************************************
 ** \brief Define Occp channel
 ******************************************************************************/ 
typedef enum en_adcmp_occp_sel
{
    AdcmpSelOccp0 = 0u, ///< select Occp0 channel
    AdcmpSelOccp1 = 1u, ///< select Occp1 channel
  
}en_adcmp_occp_sel_t;

/**
 ******************************************************************************
 ** \brief Define Mft_adcmp compatible fm3 mode
 ******************************************************************************/
typedef enum en_adcmp_mode_fm3
{
    AdcmpAccpUpAccpDownFm3   = 0u, ///< Fm3 compatible mode: Accp Up and Down
    AdcmpAccpUpFm3           = 1u, ///< Fm3 compatible mode: Accp Up
    AdcmpAccpDownFm3         = 2u, ///< Fm3 compatible mode: Accp Down
    AdcmpAccpUpAccpdnDownFm3 = 3u, ///< Fm3 compatible mode: Accp up adn Accpdn Down
    
}en_adcmp_mode_fm3_t;

/** \} GroupMFT_ADCMP_Types */

/**
* \addtogroup GroupMFT_ADCMP_DataStructures
* \{
*/

/**
 ******************************************************************************
 ** \brief Define Mft_adcmp configure parameters
 ******************************************************************************/ 
typedef struct stc_mft_adcmp_config
{
    en_adcmp_frt_t        enFrt;         ///< configure Adcmp Frt channel
    en_adcmp_buf_t        enBuf;         ///< configure Adcmp Buffer transfer type
    en_adcmp_trig_sel_t   enTrigSel;     ///< configure Adcmp Trigger type
    en_adcmp_mode_t       enMode;        ///< configure Adcmp Running mode
    en_adcmp_occp_sel_t   enOccpSel;     ///< select Adcmp Occp channel
#if (PDL_MCU_CORE == PDL_FM4_CORE)
    #if (PDL_MCU_TYPE >= PDL_FM4_TYPE3)   
    boolean_t             bCompareFrtZeroMaskCntVal;  ///< TRUE:  Comparison is performed with the FRT zero interrupt mask counter.
                                                      ///< FALSE: Comparison is not performed with the FRT zero interrupt mask counter.
    boolean_t             bCompareFrtPeakMaskCntVal;  ///< TRUE:  Comparison is performed with the FRT peak interrupt mask counter.
                                                      ///< FALSE: Comparison is not performed with the FRT peak interrupt mask counter.
    uint8_t               u8CompareVal;               ///< Specifies the AD conversion start time by the value to be compared with the FRT interrupt mask counter.
    #endif
#endif 
}stc_mft_adcmp_config_t;

/**
 ******************************************************************************
 ** \brief Define Mft_adcmp functions
 ******************************************************************************/ 
typedef struct stc_mft_adcmp_func
{
    boolean_t   bDownEn;  ///< Enable Adcmp Down function
    boolean_t   bPeakEn;  ///< Enable Adcmp Peak function
    boolean_t   bUpEn;    ///< Enable Adcmp Up function
    boolean_t   bZeroEn;  ///< Enable Adcmp Zero function
  
}stc_mft_adcmp_func_t;

/**
 ******************************************************************************
 ** \brief Define Mft_adcmp compatible fm3 configure parameters
 ******************************************************************************/
typedef struct stc_mft_adcmp_config_fm3
{
    en_adcmp_frt_fm3_t     enFrt;     ///< configure Adcmp Frt channel
    en_adcmp_mode_fm3_t    enMode;    ///< configure compatible Fm3 mode
    en_adcmp_buf_t         enBuf;     ///< configure Adcmp Buffer transfer type
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)      
    en_adcmp_trig_sel_t    enTrigSel; ///< Select trig mode
#else
    boolean_t bAdcScanTriggerSel;     ///< FALSE: selects the start signal of ADCMP ch.x as ADC unitx scan conversion start signal.
                                      ///< TRUE: Selects the logic OR signal of FRT ch.0 to ch.2 start signal as ADC unitx scan conversion start signal.
    boolean_t bAdcPrioTriggerSel;     ///< FALSE: selects the start signal of ADCMP ch.x as ADC unitx priority conversion start signal.
                                      ///< TRUE: Selects the logic OR signal of FRT ch.0 to ch.2 start signal as ADC unitx priority conversion start signal.
#endif    
    
}stc_mft_adcmp_config_fm3_t;

/** \} GroupMFT_ADCMP_DataStructures */

/**
* \addtogroup GroupMFT_ADCMP_Functions
* \{
*/

/**
 ******************************************************************************
 ** \brief Global function prototypes (definition in C source)
 ******************************************************************************/
/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
/* 1. FM4/FM0+ */
/* Init */
en_result_t Mft_Adcmp_Init (volatile stc_mftn_adcmp_t *pstcMftAdcmp,  
                            uint8_t u8Ch, 
                            const stc_mft_adcmp_config_t *pstcConfig);
en_result_t Mft_Adcmp_DeInit(volatile stc_mftn_adcmp_t* pstcMftAdcmp, 
                             uint8_t u8Ch);
/* Func enable/disable */
en_result_t Mft_Adcmp_EnableOperation(volatile stc_mftn_adcmp_t *pstcMftAdcmp,  
                                      uint8_t u8Ch, 
                                      stc_mft_adcmp_func_t* pstcFunc);
en_result_t Mft_Adcmp_DisableOperation(volatile stc_mftn_adcmp_t *pstcMftAdcmp,  
                                       uint8_t u8Ch, 
                                       stc_mft_adcmp_func_t* pstcFunc);
/* Count write/read */
en_result_t Mft_Adcmp_WriteAcmp(volatile stc_mftn_adcmp_t *pstcMftAdcmp, uint8_t u8Ch, 
                                uint16_t u16AcmpVal);
uint16_t    Mft_Adcmp_ReadAcmp(volatile stc_mftn_adcmp_t *pstcMftAdcmp, uint8_t u8Ch);
#endif
/* 2. FM3/FM0+/FM4 */
/* Init/De-Init */
en_result_t Mft_Adcmp_Init_Fm3(volatile stc_mftn_adcmp_t *pstcMftAdcmp, 
                               uint8_t u8FM3Ch,
                               const stc_mft_adcmp_config_fm3_t *pstcConfig);
en_result_t Mft_Adcmp_DeInit_Fm3(volatile stc_mftn_adcmp_t *pstcMftAdcmp, 
                                 uint8_t u8FM3Ch);
/* Mode configuration */
en_result_t Mft_Adcmp_EnableOperation_Fm3(volatile stc_mftn_adcmp_t *pstcMftAdcmp, uint8_t u8FM3Ch);
en_result_t Mft_Adcmp_DisableOperation_Fm3(volatile stc_mftn_adcmp_t *pstcMftAdcmp, uint8_t u8FM3Ch);
/* Count write/read */
en_result_t Mft_Adcmp_WriteAccp_Fm3(volatile stc_mftn_adcmp_t *pstcMftAdcmp, uint8_t u8FM3Ch, uint16_t u16AccpVal);
uint16_t Mft_Adcmp_ReadAccp_Fm3(volatile stc_mftn_adcmp_t *pstcMftAdcmp, uint8_t u8FM3Ch);
en_result_t Mft_Adcmp_WriteAccpdn_Fm3(volatile stc_mftn_adcmp_t *pstcMftAdcmp, uint8_t u8FM3Ch, uint16_t u16AccpdnVal);
uint16_t Mft_Adcmp_ReadAccpdn_Fm3(volatile stc_mftn_adcmp_t *pstcMftAdcmp, uint8_t u8FM3Ch);

/** \} GroupMFT_ADCMP_Functions */
/** \} GroupMFT_ADCMP */

#ifdef __cplusplus
}
#endif

#endif

#endif

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
