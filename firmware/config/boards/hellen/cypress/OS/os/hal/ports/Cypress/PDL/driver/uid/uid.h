/*******************************************************************************
* \file              uid.h
* 
* \version           1.20
*
* \brief             Headerfile for UID functions.
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

#ifndef __UID_H__
#define __UID_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_UID_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
  
/**
* \defgroup GroupUID Unique ID Register (UID)
* \{ 
* \defgroup GroupUID_Functions Functions
* \defgroup GroupUID_DataStructures Data Structures
* \}
*/

/**
* \addtogroup GroupUID
* \{
* FM family microcontrollers have two 32-bit registers that store an ID number 
* unique to each individual chip. You can read this value; you cannot set it. 
* You can use this value as a unique ID for any purpose. For example, you might 
* read the number the first time you install your software, and save it as a serial number. 
* You can then check to ensure that the software is running on the same processor on which it was 
* installed.<br>
* Unique ID 0 register uses the upper 28 bits. Unique ID 1 uses the lower 13 bits. All other bits are reserved.<br>
* These values will not change due to reset or power on/off.
* \section SectionUID_ConfigurationConsideration Configuration Consideration
* There is no configuration required. You can read the value of one, the other, or both registers.
* Uid_ReadUniqueId() reads out the Unique ID to a pointered structure of the
* type #stc_unique_id_t. Reserved bits are masked to zero, but the results are not aligned to least significant bit (LSB).<br>
* Uid_ReadUniqueId0() reads out the Unique ID 0
* register and aligns the result to the LSB. <br>
* Uid_ReadUniqueId1() reads out the
* Unique ID 1 register and masks reserved bits to '0'.<br>
* Uid_ReadUniqueId64() reads both registers as a 64-bit value, aligns them
* to LSB, and masks out reserved bits to '0'.
* 
* \section SectionUID_MoreInfo More Information
* For more information on the UID peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/223051/download">FM0+ Peripheral Manual - Core Subsystem TRM.pdf</a><br>
* <a href="http://www.cypress.com/file/222996/download">FM4 Peripheral Manual - Core Subsystem TRM.pdf</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/

/**
* \addtogroup GroupUID_DataStructures
* \{
*/

/******************************************************************************/
/* Global type definitions                                                    */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief PDL structure of Unique ID register set
 ******************************************************************************/  
 typedef struct stc_unique_id
 {
   uint32_t u32Uidr0;
   uint32_t u32Uidr1;
 } stc_unique_id_t;

/** \} GroupUID_DataStructures */
 
/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/ 


/**
* \addtogroup GroupUID_Functions
* \{
*/
 
/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/

en_result_t Uid_ReadUniqueId(stc_unique_id_t *pstcUniqueId);
 
uint32_t Uid_ReadUniqueId0(void);
 
uint32_t Uid_ReadUniqueId1(void);

uint64_t Uid_ReadUniqueId64(void);

/** \} GroupUID_Functions */
/** \} GroupUID */

#ifdef __cplusplus
}
#endif

#endif // #if (defined(PDL_PERIPHERAL_UID_ACTIVE))

#endif /* __UID_H__ */

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
