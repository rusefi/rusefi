/*******************************************************************************
* \file              wc.h
* 
* \version           1.20
*
* \brief             Headerfile for WC functions.
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

#ifndef __WC_H__
#define __WC_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_WC_ACTIVE))

/**
* \defgroup GroupWC Watch Counter (WC)
* \{ 
* \defgroup GroupWC_Macros Macros
* \defgroup GroupWC_Functions Functions
* \defgroup GroupWC_DataStructures Data Structures
* \defgroup GroupWC_Types Enumerated Types
* \}
*/

/**
* \addtogroup GroupWC
* \{
* The watch counter is a 6-bit down counter. It counts down from a specified value, 
* and generates an interrupt request at the time that it enters an underflow condition.
* The timer’s period is based on an input clock, an output clock, and a prescalar. 
* The output clock is one of WCCK0 to WCCK3. The input clock can be the main clock, 
* the sub clock, the built-in high-speed CR clock or the built-in low-speed CR clock. 
* The prescaler outputs the division clocks (WCCK0 to WCCK3) based on your prescalar choice. 
* The prescalar choice ranges from 2 to 2^23. <br>   
* The output clock and the watch counter’s value (0-63) determines the timer’s period. For example, 
* if based on your configuration the timer’s period is one second, and you set the watch counter 
* to a value of 60, it will generate an interrupt request once per minute.<br>
* You can handle the watch counter in one of two modes. In interrupt mode, the peripheral sets the 
* interrupt flag and calls the interrupt handler when the interrupt occurs. In polling mode, the 
* peripheral just sets the interrupt flag. You must poll to check the flag status, and handle the 
* interrupt when the flag is set.<br>
*
* The Watch Counter wakes up the microcontroller from low-power mode.
*
* \section SectionWC_ConfigurationConsideration Configuration Consideration
* You configure the clock choices and the counter. You then start the clock, and the counter itself.<br>
*
* For easy setting needed underflow frequency in the WC use the following formula:<br>
* Set values in the stc_wc_pres_clk_t structure to specify the input clock and the prescalar choice. 
* See the documentation for en_output_clk_t to see prescalar choices and how they affect WCCK0 
* through WCCK3. Then call Wc_Pres_SelClk() to initialize these choices.<br>
*
* Set values in the stc_wc_config_t structure to configure the counter. For example, set enCntClk 
* to specify which clock drives the counter (WCCK0-WCCK3). If bIrqEnable is FALSE, you must poll to 
* check for interrupts. If bIrqEnable is TRUE, the interrupt handler is called directly. <br>
* 
* After setting the fields, call Wc_Init() to initialize the counter.<br>
* Call Wc_Pres_EnableDiv() to start the clock supply. <br>
* Call Wc_EnableCount() to start the counter. <br>
* If you are polling for interrupts, use WcGetIrqFlag() to check the interrupt status. If the 
* flag is set, call Wc_IrqHandler() to clear the flag and invoke the handler.
* 
* \section SectionWC_MoreInfo More Information
* For more information on the WC peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/223041/download">FM0+ Peripheral Manual - Timer Subsystem TRM.pdf</a><br>
* <a href="http://www.cypress.com/file/222986/download">FM4 Peripheral Manual - Timer Subsystem TRM.pdf</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/
 
/**
* \addtogroup GroupWC_Macros
* \{
*/

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/
#define stc_wcn_t   FM_WC_TypeDef
#define WC0         (*((stc_wcn_t *) FM_WC_BASE))

/** \} GroupWC_Macros */

/**
* \addtogroup GroupWC_Types
* \{
*/

/**
 ******************************************************************************
 ** \brief Enumeration to define source clock of watch counter precaler
 ******************************************************************************/ 
typedef enum en_input_clk
{
    WcPresInClkSubOsc   = 0u,      ///< Watch counter prescaler source clock: sub oscillator
    WcPresInClkMainOsc  = 1u,      ///< Watch counter prescaler source clock: main oscillator
#if (PDL_MCU_TYPE == PDL_FM3_TYPE12) || (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)    
    WcPresInClkHighCr   = 2u,      ///< Watch counter prescaler source clock: high-speed CR
    WcPresInClkLowCr    = 3u,      ///< Watch counter prescaler source clock: low-speed CR 
#endif
}en_input_clk_t;

/**
 ******************************************************************************
 ** \brief Enumeration to define output clock of watch counter precaler
 ******************************************************************************/ 
typedef enum en_output_clk
{                                ///                                         WCCk3           WCCk2           WCCk1           WCCk0                         
    WcPresOutClkArray0 = 0u,     ///< Watch counter prescaler output array0: 2^15/src clock, 2^14/src clock, 2^13/src clock, 2^12/src clock
    WcPresOutClkArray1 = 1u,     ///< Watch counter prescaler output array0: 2^25/src clock, 2^24/src clock, 2^23/src clock, 2^22/src clock
#if (PDL_MCU_TYPE == PDL_FM3_TYPE12) || (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)        
    WcPresOutClkArray2 = 2u,     ///< Watch counter prescaler output array0: 2^4/src clock,  2^3/src clock,  2^2/src clock,  2/src clock
    WcPresOutClkArray3 = 3u,     ///< Watch counter prescaler output array0: 2^8/src clock,  2^7/src clock,  2^6/src clock,  2^5/src clock
    WcPresOutClkArray4 = 4u,     ///< Watch counter prescaler output array0: 2^12/src clock, 2^11/src clock, 2^10/src clock, 2^9/src clock
    WcPresOutClkArray5 = 5u,     ///< Watch counter prescaler output array0: 2^19/src clock, 2^18/src clock, 2^17/src clock, 2^16/src clock
    WcPresOutClkArray6 = 6u,     ///< Watch counter prescaler output array0: 2^23/src clock, 2^22/src clock, 2^21/src clock, 2^20/src clock
#endif    
}en_output_clk_t;

/**
 ******************************************************************************
 ** \brief Enumeration to set the clock of watch counter
 ******************************************************************************/
typedef enum en_wc_cnt_clk
{
    WcCntClkWcck0 = 0u,       ///< Watch counter source clock: WCCK0
    WcCntClkWcck1 = 1u,       ///< Watch counter source clock: WCCK1
    WcCntClkWcck2 = 2u,       ///< Watch counter source clock: WCCK2
    WcCntClkWcck3 = 3u,       ///< Watch counter source clock: WCCK3
    
}en_wc_cnt_clk_t;

/** \} GroupWC_Types */

/**
* \addtogroup GroupWC_DataStructures
* \{
*/

/**
 ******************************************************************************
 ** \brief Structure to configure watch counter prescaler
 ******************************************************************************/
typedef struct stc_wc_pres_clk
{
    en_input_clk_t      enInputClk;   ///< Watch counter prescaler input clock setting
    en_output_clk_t     enOutputClk;  ///< Watch counter prescaler output clock setting
   
}stc_wc_pres_clk_t;

/**
 ******************************************************************************
 ** \brief Structure to configure the watch counter
 ******************************************************************************/
typedef struct stc_wc_config
{
    en_wc_cnt_clk_t  enCntClk;      ///< Watch counter source clock setting
    uint8_t          u8ReloadValue; ///< Reload value 
#if (PDL_INTERRUPT_ENABLE_WC0 == PDL_ON)    
    boolean_t        bIrqEnable;     ///< TRUE: enable IRQ, FALSE: don't enable IRQ
    func_ptr_t       pfnIrqCallback; ///< Pointer to interrupt callback function
    boolean_t        bTouchNvic;     ///< TRUE: enable NVIC, FALSE: don't enable NVIC
#endif  
}stc_wc_config_t;

/** \} GroupWC_DataStructures */

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/

/**
* \addtogroup GroupWC_Functions
* \{
*/

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/
/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/* 1. WC prescaler setting */
en_result_t Wc_Pres_SelClk(stc_wcn_t* pstcWc, stc_wc_pres_clk_t* pstcWcPresClk);
en_result_t Wc_Pres_EnableDiv(stc_wcn_t* pstcWc);
en_result_t Wc_Pres_DisableDiv(stc_wcn_t* pstcWc);
en_stat_flag_t Wc_Pres_GetDivStat(stc_wcn_t* pstcWc);
/* 2. WC setting */
/* Init/De-Init */
en_result_t Wc_Init(stc_wcn_t* pstcWc, const stc_wc_config_t* pstcWcConfig);
en_result_t Wc_DeInit(stc_wcn_t* pstcWc, boolean_t bTouchNvic);
/* Func/Int enable/disable */
en_result_t Wc_EnableCount(stc_wcn_t* pstcWc);
en_result_t Wc_DisableCount(stc_wcn_t* pstcWc);
#if (PDL_INTERRUPT_ENABLE_WC0 == PDL_ON)
en_result_t Wc_EnableIrq(stc_wcn_t* pstcWc);
en_result_t Wc_DisableIrq(stc_wcn_t* pstcWc);
#endif
/* Count write/read */
en_result_t Wc_WriteReloadVal(stc_wcn_t* pstcWc, uint8_t u8Val);
uint8_t Wc_ReadCurCnt(stc_wcn_t* pstcWc);
/* Status clear */
en_result_t Wc_ClearIrqFlag(stc_wcn_t* pstcWc);
/* Status read */
en_irq_flag_t Wc_GetIrqFlag(stc_wcn_t* pstcWc);
en_stat_flag_t Wc_GetOperationFlag(stc_wcn_t* pstcWc);
/* 3. IRQ handler */
void Wc_IrqHandler(stc_wcn_t* pstcWc);

/** \} GroupWC_Functions */
/** \} GroupWC */

#ifdef __cplusplus
}
#endif

#endif // #if (defined(L3_PERIPHERAL_WC_ACTIVE))

#endif /* __WC_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
