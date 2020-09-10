/******************************************************************************
* \file     mainflash.h
*
* \version  1.20
*
* \brief    Headerfile for main flash functions
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


#ifndef _MAINFLASH_H_
#define _MAINFLASH_H_

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if defined(PDL_PERIPHERAL_MAIN_FLASH_ACTIVE) 
/**
* \defgroup GroupFLASH Flash Memory (FLASH)
* \{
* \defgroup GroupMFLASH Main Flash Memory (MFLASH)
* \{ 
* \defgroup GroupMFLASH_Macros Macros
* \defgroup GroupMFLASH_Functions Functions 
* \defgroup GroupMFLASH_Types Enumerated Types
* \}
* \}
*/
/**
* \addtogroup GroupMFLASH
* \{
* The PDL Flash peripheral provides support for working with Dual Operation Flash, 
* MainFlash, and WorkFlash. This document discusses MainFlash.<br>
* You can erase the entire MainFlash area or erase individual sectors. 
* You can write data as words (32-bit) or half-words (16 bit). Built-in Error Correction Code (ECC) 
* functionality can correct up to 1 bit of errors in each word. There is no function to detect 
* 2-bit errors. Errors are automatically corrected when memory is read. ECC codes 
* are automatically added upon writing to flash memory. Because there are no read cycle penalties 
* as a result of error correction, it is not necessary to consider the error correction penalties 
* during software development.<br>
* 
* \section SectionMFLASH_ConfigurationConsideration Configuration Consideration
*
* Before using the MainFlash functions, ensure that the code is not in the 
* flash memory you are working on. For example, in the IAR environment, the key word "__ramfunc" 
* puts the code in RAM. <br>
*
* In the Keil environment, right-click the file with your code and in the options for the file, 
* choose a RAM area for the file.<br>
*
* No configuration is required. Simply call the function to erase or write data. You can write 
* data using 16-bit or 32-bit alignment.<br>
* 
* FM4 TYPE3 microcontrollers also support a DualFlash mode. Use MFlash_SetDualMode() 
* to activate this operating mode. DualFlash creates a separate Flash area. In DualFlash mode, 
* the MFlash_xxx() functions have no effect on the DualFlash area. Use MFlash_Dualxxx() 
* functions to erase or write data to the DualFlash area.<br>
*
* \section SectionMFLASH_MoreInfo More Information
* For more information on the Flash peripheral, refer to:<br> 
* <a href="http://www.cypress.com/search/all?f[0]=meta_type%3Atechnical_documents&f[1]=resource_meta_type%3A583&f[2]=field_related_products%3A88896">FM0+ Flash Programming Manuals</a><br>
* <a href="http://www.cypress.com/search/all?f[0]=meta_type%3Atechnical_documents&f[1]=field_related_products%3A88906&f[2]=resource_meta_type%3A583">FM4 Flash Programming Manuals</a><br>
*  The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/
/**
* \addtogroup GroupMFLASH_Macros
* \{
*/     
/******************************************************************************
 * Global type definitions
 ******************************************************************************/
/**
 ******************************************************************************
 ** \brief write command
 ** 
 ** To Select write command sequence by MCU type
 ******************************************************************************/ 
#if (PDL_MCU_TYPE == PDL_FM3_TYPE3) || (PDL_MCU_TYPE == PDL_FM3_TYPE7) ||(PDL_MCU_CORE == PDL_FM4_CORE)
    #define     MFLASH_CODE1                 0x0AA8
    #define     MFLASH_CODE2                 0x0554   
#elif(PDL_MCU_TYPE == PDL_FM3_TYPE10) || (PDL_MCU_TYPE == PDL_FM3_TYPE11)
    #define     MFLASH_CODE1                 0x0554
    #define     MFLASH_CODE2                 0x0AA8  
#elif (PDL_MCU_CORE == PDL_FM0P_CORE)
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE3)
    #define     MFLASH_CODE1                 0x0A98
    #define     MFLASH_CODE2                 0x0544 
#else
    #define     MFLASH_CODE1                 0x0AA8
    #define     MFLASH_CODE2                 0x0554   
#endif
#else
    #define     MFLASH_CODE1                 0x1550
    #define     MFLASH_CODE2                 0x0AA8
#endif
/**
 ******************************************************************************
 ** \brief main flash check bit mask
 ** 
 ** To Set flash check hardware sequence flag bit mask
 ******************************************************************************/       
#define     MFLASH_CHK_DPOL_MASK         (uint16_t)0x0080
#define     MFLASH_CHK_TOGG_MASK         (uint16_t)0x0040
#define     MFLASH_CHK_TLOV_MASK         (uint16_t)0x0020
/**
 ******************************************************************************
 ** \brief CR address
 ** 
 ** To Set CR address
 ******************************************************************************/   
#define     CR_DATA_ADDR                 (0x00100004)

/** \} GroupMFLASH_Macros */

/**
* \addtogroup GroupMFLASH_Types
* \{
*/
/**
 ******************************************************************************
 ** \brief state feedback
 ** 
 ** To feedback the flash operation state 
 ******************************************************************************/ 
typedef enum en_mflash_state
{
       MFLASH_RET_OK,                           ///< OK
       MFLASH_RET_INVALID_PARA,                 ///< parameter error
       MFLASH_RET_ABNORMAL,                     ///< operation fail
       MFLASH_RET_ECCERROR                      ///< flash ECC verify error
}en_mflash_state_t;   
/**
 ******************************************************************************
 ** \brief check toggle state
 ** 
 ** To feedback the flash toggle state 
 ******************************************************************************/ 
typedef enum en_mflash_toggle
{
       MFLASH_CHK_TOGG_NORMAL,                   ///< flash toggle normal
       MFLASH_CHK_TOGG_ABNORMAL                  ///< flash toggle abnormal
}en_mflash_toggle_t; 
/**
 ******************************************************************************
 ** \brief check toggle state
 ** 
 ** To feedback the flash toggle state 
 ******************************************************************************/ 
typedef enum en_mflash_datapoll
{
       MFLASH_CHK_DPOL_NORMAL,                   ///< data poll normal
       MFLASH_CHK_DPOL_ABNORMAL                  ///< data poll abnormal
}en_mflash_datapoll_t; 
/**
 ******************************************************************************
 ** \brief check toggle state
 ** 
 ** To feedback the flash toggle state 
 ******************************************************************************/ 
typedef enum en_mflash_eccbitstat
{
       MFLASH_ECC_NORMAL,                   ///< data poll normal
       MFLASH_ECC_ABNORMAL                  ///< data poll abnormal
}en_mflash_eccbitstat_t; 

/** \}GroupMFLASH_Types */

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/
/**
* \addtogroup GroupMFLASH_Functions
* \{
*/
/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

// [andreika]: todo: add gcc support?
#if defined ( __ICCARM__ )
#define __RAMFUNC __ramfunc 
#elif defined ( __GNUC__ )
#define __RAMFUNC __attribute__( ( section(".data") ) )
#else
#define __RAMFUNC
#endif

// [andreika]: add __RAMFUNC and volatile
__RAMFUNC en_result_t MFlash_ChipErase(boolean_t bCrRemain);
__RAMFUNC en_result_t MFlash_SectorErase(volatile uint16_t* pu16SecAddr);
__RAMFUNC en_result_t MFlash_WriteData32Bit(uint32_t*  pu32WriteAddr, 
                                  uint32_t* pu32WriteData, 
                                  uint32_t u32Size, 
                                  boolean_t bVerifyAndEccCheck);
__RAMFUNC en_result_t MFlash_WriteData16Bit(uint16_t*  pu16WriteAddr, 
                                  uint16_t* pu16WriteData, 
                                  uint32_t u32Size);
__RAMFUNC en_result_t MFlash_WriteData16Bit_Fm0Type3CrSecureArea(uint16_t*  pu16WriteAddr, 
                                  uint16_t* pu16WriteData, 
                                  uint32_t u32Size);
#if (PDL_MCU_TYPE == PDL_FM4_TYPE3)
en_result_t MFlash_SetDualMode(boolean_t bDualMode);
en_result_t MFlash_SetRemapMode(boolean_t bRemapMode);
en_result_t MFlash_DualSectorErase(uint16_t* pu16SecAddr);
en_result_t MFlash_DualWriteData32Bit(uint32_t*  pu32WriteAddr,\
                                      uint32_t* pu32WriteData, \
                                      uint32_t u32Size, \
                                      boolean_t bVerifyAndEccCheck);
en_result_t MFlash_DualWriteData16Bit(uint16_t*  pu16WriteAddr, \
                                      uint16_t* pu16WriteData, \
                                      uint32_t u32Size);
/** \} GroupMFLASH_Functions */
/** \} GroupMFLASH    */
/** \} GroupFLASH    */
#endif
#ifdef __cplusplus
}
#endif


#endif /* PERIPHERAL_AVAILABLE_MAINFLASH */

#endif /* _MAINFLASH_FM_H_ */
