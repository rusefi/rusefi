/*******************************************************************************
* \file              pcrc.h
* 
* \version           1.20
*
* \brief             Headerfile for Programmable CRC functions.
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

#ifndef __PCRC_H__
#define __PCRC_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "pdl_user.h"
#include "mcu.h"

#if (defined(PDL_PERIPHERAL_PCRC_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \defgroup GroupPCRC Programmable Cyclic Redundancy Check (PCRC)
* \{ 
* \defgroup GroupPCRC_Functions Functions
* \defgroup GroupPCRC_DataStructures Data Structures
* \defgroup GroupPCRC_Types Enumerated Types
* \}
*/

/**
* \addtogroup GroupPCRC
* \{
* Use the Programmable CRC (PCRC) peripheral to compute CRC values using an 
* arbitrary generator polynomial on the input data. By contrast, the CRC 
* peripheral supports defined standards with fixed generator polynomials.<br>
* PCRC is available in FM4 devices types 3 and 4. Other FM devices do not 
* support programmable CRC. Check the datasheet for your device to determine whether 
* PCRC is supported. <br>
* PCRC supports these features:
* * Arbitrary CRC generator polynomial (CRC values length up to 32-bit)
* * Arbitrary seed value for CRC computing
* * Choice of input data width can be 8-, 16-, 24-, or 32-bit
* * Choice of input data format (byte/bit order)
* * Choice of output data format (byte/bit order)
* * Arbitrary value for bitwise XOR
* * Interrupt notification when CRC calculation is complete
* * Data input using hardware DMA via DSTC<br>
* For byte/bit ordering, you have four choices. The bits in a byte can be in least
* significant bit or most significant bit order. The bytes can be ordered as big-endian
* or little-endian. 
* \section SectionPCRC_ConfigurationConsideration Configuration Consideration
* To set up the PCRC you provide configuration parameters in the stc_pcrc_config_t 
* structure. For example you specify the generator polynomial, the data formats, 
* the data size, and so on. Then call Pcrc_Init().<br>
*  At runtime use API function calls to:
* * Set the initial seed value
* * Set the input or output data format
* * Set the input data size
* * Enable or disable interrupts
* * Write data to calculate the CRC
* * Read the result <br>
* To calculate a CRC, call Pcrc_WriteData() repeatedly until you have pushed all the of 
* input data into the calculation. You should check whether CRC is busy before every call 
* to Pcrc_WriteData(). Use Pcrc_GetLockStatus().<br>
* When done, use Pcrc_ReadResult() to get the result. 
* 
* \section SectionPCRC_MoreInfo More Information
* For more information on the PCRC peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/222996/download">FM4 Peripheral Manual - Core Subsystem TRM.pdf</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/******************************************************************************
 * Global definitions
 ******************************************************************************/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/
 
/**
* \addtogroup GroupPCRC_Types
* \{ 
*/

/**
 *****************************************************************************
 ** \brief Programmable Crc data format
 *****************************************************************************/
typedef enum en_crc_data_format
{
    MsbFirstBigEndian    = 0u,   ///< MSB-first / Big endian is selected for output format conversion.
    MsbFirstLittleEndian = 1u,   ///< MSB-first / Little endian is selected for output format conversion.
    LsbFirstBigEndian    = 2u,   ///< LSB-first / Big endian is selected for output format conversion.
    LsbFirstLittleEndian = 3u,   ///< LSB-first / Little endian is selected for output format conversion.
} en_crc_data_format_t;  

/**
 *****************************************************************************
 ** \brief Programmable Crc input data size
 *****************************************************************************/
typedef enum en_crc_input_data_size
{
    InputData8Bit  = 0u,   ///<   8-bit input data size is selected.
    InputData16Bit = 1u,   ///<  16-bit input data size is selected.
    InputData24Bit = 2u,   ///<  24-bit input data size is selected.
    InputData32Bit = 3u,   ///<  32-bit input data size is selected.
} en_crc_input_data_size_t; 

/** \} GroupPCRC_Types */

/**
* \addtogroup GroupPCRC_DataStructures
* \{
*/

/**
 *****************************************************************************
 ** \brief Programmable Crc configuration
 *****************************************************************************/
typedef struct stc_pcrc_config
{
    uint32_t      u32GeneratorPolynomial;     ///< CRC Computing Generator Polynomial
    en_crc_data_format_t enInputFormat;       ///< Input data format    
    en_crc_data_format_t enOutputFormat;      ///< Output data format  
    en_crc_input_data_size_t enInputDataSize; ///< Input data size 
    uint32_t      u32CrcInitValue;      ///< Initial value   
    uint32_t      u32FinalXorValue;     ///< Specifies the value of XOR (exclusive OR) for final results of CRC computing.   

    boolean_t  bUseDstc;          ///< TRUE: the transfer request of input data for DMA.    
#if (PDL_INTERRUPT_ENABLE_PCRC== PDL_ON)
    boolean_t  bIrqEn;            ///< TRUE: Interrupt request enable.
    func_ptr_t pfnIrqCb;  ///<  Interrupt request callback funciton
    boolean_t bTouchNvic;      
#endif        
} stc_pcrc_config_t;

/** \} GroupPCRC_DataStructures */

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/

/**
* \addtogroup GroupPCRC_Functions
* \{
*/

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/

#if (PDL_INTERRUPT_ENABLE_PCRC == PDL_ON)
void PcrcIrqHandler(void);
#endif
en_result_t Pcrc_Init(const stc_pcrc_config_t* pstcConfig);
void Pcrc_DeInit(void); 
void Pcrc_SetInitialValue(uint32_t u32InitialValue);
void Pcrc_SetFinalXorValue(uint32_t u32FinalXorValue);
void Pcrc_SetOutputDataFormat(en_crc_data_format_t enOutputDataFormat); 
void Pcrc_SetInputDataFormat(en_crc_data_format_t enInputDataFormat); 
void Pcrc_SetInputDataSize(en_crc_input_data_size_t enInputDataSize); 
void Pcrc_SetCfgTest(uint8_t u8CfgTest); 
uint8_t Pcrc_GetIntRequestFlag(void); 
void Pcrc_EnableIntRequest(void); 
void Pcrc_DisableIntRequest(void);
void Pcrc_EnableDmaTx(void); 
void Pcrc_DisableDmaTx(void); 
uint8_t Pcrc_GetLockStatus(void); 
void Pcrc_ClrIntRequest(void); 
void Pcrc_WriteData(uint32_t u32WriteData);
uint32_t Pcrc_ReadResult(void);

/** \} GroupPCRC_Functions */
/** \} GroupPCRC */
#ifdef __cplusplus
}
#endif

#endif /* #if (defined(PDL_PERIPHERAL_PCRC_ACTIVE)) */

#endif /* __PCRC_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
