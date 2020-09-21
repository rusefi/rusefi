/******************************************************************************
* \file     dualflash.h
*
* \version  1.20
*
* \brief    Headerfile for dual flash functions
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


#ifndef _DUALFLASH_H_
#define _DUALFLASH_H_

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if defined(PDL_PERIPHERAL_DUAL_FLASH_ACTIVE)
/**
* \addtogroup GroupFLASH
* \{
* \defgroup GroupDFLASH Dual Flash (DFlash)
* \{
* \defgroup GroupDFLASH_Macros Macros
* \defgroup GroupDFLASH_Functions Functions 
* \}
*/
/**
* \addtogroup GroupDFLASH
* \{
* The PDL Flash peripheral provides support for working with Dual Operation 
* Flash, MainFlash, and WorkFlash. This document discusses Dual Operation Flash.<br>
* FM0+ parts support Dual Operation Flash. There are two memory banks, the upper 
* bank and the lower bank. With Dual Operation Flash you can:<br>
* - Read both banks<br>
* - Erase both banks<br>
* - Read one bank while writing or erasing the other bank<br>
* \section SectionDFLASH_ConfigurationConsideration Configuration Consideration
* Before using the Dual Flash functions, make sure the Flash operation code is not in the Flash memory
* you are working on. <br> 
* No configuration is required. Simply call the function to erase or write data. 
* You can write data using 8-, 16-, or 32-bit alignment.<br>
* 
* \section SectionDFLASH_MoreInfo More Information
* For more information about dual operation Flash support, refer to:<br>
* <a href="http://www.cypress.com/search/all?f[0]=meta_type%3Atechnical_documents&f[1]=resource_meta_type%3A583&f[2]=field_related_products%3A88896">
* the Flash Programming Manual for a particular FM0+ microcontroller series</a><br>
*
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/**
* \addtogroup GroupDFLASH_Macros
* \{
*/  
/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

#define     FLASH_LOW_BANK1_START_ADDR              (uint8_t*)(0x00000000ul)
#define     FLASH_LOW_BANK1_END_ADDR                (uint8_t*)(0x00003FFFul)
#define     FLASH_LOW_BANK2_START_ADDR              (uint8_t*)(0x00200000ul)
#define     FLASH_LOW_BANK2_END_ADDR                (uint8_t*)(0x00208000ul)
#define     FLASH_HIGH_BANK_START_ADDR              (uint8_t*)(0x00004000ul)
#define     FLASH_HIGH_BANK_END_ADDR                (uint8_t*)(0x00080000ul)

#define     DFLASH_RET_OK                0u
#define     DFLASH_RET_INVALID_PARA      1u
#define     DFLASH_RET_ABNORMAL          2u

#define     DFLASH_CHK_TOGG_NORMAL       0u
#define     DFLASH_CHK_TOGG_ABNORMAL     1u

#define     DFLASH_CHK_DPOL_NORMAL       0u
#define     DFLASH_CHK_DPOL_ABNORMAL     1u

#define     DFLASH_CHK_DPOL_MASK         (uint8_t)0x80u
#define     DFLASH_CHK_TOGG_MASK         (uint8_t)0x40u
#define     DFLASH_CHK_TLOV_MASK         (uint8_t)0x20u

/** \} GroupDFLASH_Macros */

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/
/**
* \addtogroup GroupDFLASH_Functions
* \{
*/
/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
en_result_t DFlash_ChipErase(void);
en_result_t DFlash_SectorErase(uint16_t* pu16SecAddr);
en_result_t DFlash_WriteData32Bit( uint32_t*  pu32Addr, uint32_t* pu32Data,uint32_t u32Size );
en_result_t DFlash_WriteData16Bit( uint16_t*  pu16Addr, uint16_t* pu16Data, uint32_t u32Size );
en_result_t DFlash_WriteData8Bit( uint8_t*  pu8Addr, uint8_t* pu8Data, uint32_t u32Size );
#ifdef __cplusplus
}
#endif
/** \} GroupDFLASH_Functions */
/** \} GroupDFLASH */
/** \} GroupFLASH */

#endif /* PERIPHERAL_AVAILABLE_DUALFLASH  */

#endif /* _DUALFLASH_H_  */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
