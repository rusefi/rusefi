/******************************************************************************
* \file             cr.h
*
* \version          1.20
*
* \brief            Headerfile for Clock Rate functions  
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

#ifndef __CR_H__
#define __CR_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_CR_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \defgroup GroupCR High Speed Clock Rate Trimming (CR)
* \{
* \defgroup GroupCR_Functions Functions
* \defgroup GroupCR_Types Enumerated Types
* \}
*/

/**
* \addtogroup GroupCR
* \{
* The high-speed Clock Rate(CR) oscillators privides frequency trimming 
* and oscillation temperature trimming.<br>
* CR oscillators may fluctuate because of process variation or temperature changes. 
* You can reduce the impact of that fluctuation by using trimming functions. 
* The High-Speed Clock Rate Trimming (CR) functions enable trimming for both frequency and temperature. <br>
* \section SectionCR_ConfigurationConsideration Configuration Consideration
* Use the functions in this peripheral to set or get either the frequency or temperature trimming values.<br>
* Trimming values are stored in flash memory. Erasing the flash memory also erases the CR trimming area inside the flash memory. 
* If you use a value in the CR trimming area, save the data to other area (such as RAM) before erasing the flash memory, 
* or only erase sectors other than in the CR trimming area. For the address of the CR trimming area, see the 
* Flash Programming Manual for the product.<br>
*
* Cr_SetFreqDiv() is used to divde CR input signal, which outputs to base 
* timer from dividor output.<br>
*
* Cr_SetTempTrimmingData() sets the CR temperature trimming data, and 
* Cr_GetTempTrimmingData() gets the CR temperature trimming data.
* Not all devices has the temperature trimming register.<br>
*
* Cr_SetFreqTrimmingData() sets the CR frequency trimming data, and 
* Cr_GetFreqTrimmingData() gets the CR frequency trimming data.  
* \section SectionCR_MoreInfo More Information
* For more information on the high speed CR trimming peripheral, refer to:<br> 
* <a href="http://www.cypress.com/file/223051/download">FM0+ PERIPHERAL MANUAL Core Subsystem</a><br>
* <a href="http://www.cypress.com/file/222996/download">FM4 PERIPHERAL MANUAL Core Subsystem</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/******************************************************************************
 * Global definitions
 ******************************************************************************/

 /**
* \addtogroup GroupCR_Types
* \{
*/
/**
 ******************************************************************************
 ** \brief CR frequency division values
 **
 ** \note The division signal is output to internal Base Timer or Multi-Function
 **       Timer. The division values of 64,128,256,512 are only supported by 
 **       FM3 TYPE3, TYPE7, FM4 and FM0+ products.
 ******************************************************************************/
typedef enum en_cr_freq_div
{  
    CrFreqDivBy4    = 0u,       ///< CR output (to Base Timer) prescaler: 4
    CrFreqDivBy8    = 1u,       ///< CR output (to Base Timer) prescaler: 8
    CrFreqDivBy16   = 2u,       ///< CR output (to Base Timer) prescaler: 16
    CrFreqDivBy32   = 3u,       ///< CR output (to Base Timer) prescaler: 32
#if (PDL_MCU_TYPE == PDL_FM3_TYPE3) || (PDL_MCU_TYPE == PDL_FM3_TYPE7) || \
    (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)
    CrFreqDivBy64   = 4u,       ///< CR output (to Base Timer) prescaler: 64
    CrFreqDivBy128  = 5u,       ///< CR output (to Base Timer) prescaler: 128
    CrFreqDivBy256  = 6u,       ///< CR output (to Base Timer) prescaler: 256
    CrFreqDivBy512  = 7u,       ///< CR output (to Base Timer) prescaler: 512
#endif  
}en_cr_freq_div_t;

/** \}GroupCR_Types */  

/**
* \addtogroup GroupCR_Functions
* \{
*/ 
/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/
/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/
en_result_t Cr_SetFreqDiv(en_cr_freq_div_t enCrDiv);
#if (PDL_MCU_TYPE == PDL_FM3_TYPE8)  || (PDL_MCU_TYPE == PDL_FM3_TYPE9)  || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE10) || (PDL_MCU_TYPE == PDL_FM3_TYPE11) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE12) || (PDL_MCU_CORE == PDL_FM0P_CORE)  || \
    (PDL_MCU_CORE == PDL_FM4_CORE)  
en_result_t Cr_SetTempTrimmingData(uint8_t u8Data);
uint8_t Cr_GetTempTrimmingData(void);
#endif
en_result_t Cr_SetFreqTrimmingData(uint16_t u16Data);
uint16_t Cr_GetFreqTrimmingData(void);

/** \} GroupCR_Functions */
/** \} GroupCR */

#ifdef __cplusplus
}
#endif

#endif /* #if (defined(PDL_PERIPHERAL_CR_ACTIVE)) */

#endif /* __CR_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
