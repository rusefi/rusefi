/******************************************************************************
* \file     workflash.h
*
* \version  1.20
*
* \brief    Headerfile for work flash functions
*
*************************************************************************************
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

#ifndef _WORK_FLASH_H_
#define _WORK_FLASH_H_

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if defined(PDL_PERIPHERAL_WORK_FLASH_ACTIVE) 
/**
* \addtogroup GroupFLASH
* \{
* \defgroup GroupWFLASH Work Flash (WFlash)
* \{ 
* \defgroup GroupWFLASH_Macros Macros
* \defgroup GroupWFLASH_Functions Functions 
* \}
*/
/**
* \addtogroup GroupWFLASH
*\{
* The PDL Flash peripheral provides support for working with Dual Operation Flash, 
* MainFlash, and WorkFlash. This document discusses WorkFlash.<br>
* You can erase the entire WorkFlash memory or erase individual sectors. 
* You can write data as words (32-bit) or half-words (16 bit).<br>
* \section SectionWFLASH_ConfigurationConsideration Configuration Consideration
* WorkFlash is an independent area, separate from MainFlash. You can run code in 
* MainFlash that operates on WorkFlash.<br>
* No configuration is required. Simply call the function to erase or write data. 
* You can write data using 16-bit or 32-bit alignment.<br>
* \section SectionWFLASH_MoreInfo More Information
* For more information on the Flash peripheral, refer to:<br> 
* <a href="http://www.cypress.com/search/all?f[0]=meta_type%3Atechnical_documents&f[1]=resource_meta_type%3A583&f[2]=field_related_products%3A88896">FM0+ Flash Programming Manuals</a><br>
* <a href="http://www.cypress.com/search/all?f[0]=meta_type%3Atechnical_documents&f[1]=field_related_products%3A88906&f[2]=resource_meta_type%3A583">FM4 Flash Programming Manuals</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/**
* \addtogroup GroupWFLASH_Macros
* \{
*/ 
/******************************************************************************
 * Global type definitions
 ******************************************************************************/ 
#define     WFLASH_CODE1                 0x0AA8u
#define     WFLASH_CODE2                 0x0554u

#define     WFLASH_RET_OK                0u
#define     WFLASH_RET_INVALID_PARA      1u
#define     WFLASH_RET_ABNORMAL          2u

#define     WFLASH_CHK_TOGG_NORMAL       0u
#define     WFLASH_CHK_TOGG_ABNORMAL     1u

#define     WFLASH_CHK_DPOL_NORMAL       0u
#define     WFLASH_CHK_DPOL_ABNORMAL     1u

#define     WFLASH_CHK_DPOL_MASK         (uint16_t)0x0080u
#define     WFLASH_CHK_TOGG_MASK         (uint16_t)0x0040u
#define     WFLASH_CHK_TLOV_MASK         (uint16_t)0x0020u
   
#define     WFLASH_BASE_ADDR             (0x200C0000u)

/** \} GroupWFLASH_Macros */

/**
* \addtogroup GroupWFLASH_Functions
* \{
*/
/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
en_result_t WFlash_ChipErase(void);
en_result_t WFlash_SectorErase(uint16_t* pu16Addr);
en_result_t WFlash_WriteData32Bit(uint32_t*  pu32Addr, 
                                 uint32_t* pu32Data, 
                                 uint32_t u32Size);
en_result_t WFlash_WriteData16Bit(uint16_t*  pu16Addr, 
                                 uint16_t* pu16Data, 
                                 uint32_t u32Size);
                                 
/** \} GroupWFLASH_Functions */
/** \} GroupWFLASH */
/** \} GroupFLASH */
#ifdef __cplusplus
}
#endif 

#endif /* PERIPHERAL_AVAILABLE_WORKFLASH */

#endif /* _WORKFLASH_FM3_H_ */
