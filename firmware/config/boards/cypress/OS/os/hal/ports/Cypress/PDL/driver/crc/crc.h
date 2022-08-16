/******************************************************************************
* \file             crc.h
*
* \version          1.20
*
* \brief            Headerfile for Cyclic Redundancy Check functions  
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

#ifndef __CRC_H__
#define __CRC_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "pdl_user.h"
#include "mcu.h"

#if (FM_CRC_AVAILABLE == PDL_OFF)
    #if (defined(PDL_PERIPHERAL_CRC_ACTIVE))
        #error "Don't enable CRC, as it is not available on this device"
    #endif
#endif

#if (defined(PDL_PERIPHERAL_CRC_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
/**
* \defgroup GroupCRC Cyclic Redundancy Check(CRC)
* \{
* \defgroup GroupCRC_Functions Functions 
* \defgroup GroupCRC_DataStructures Data Structures
* \defgroup GroupCRC_Types Enumerated Types
* \}
*/

/**
* \addtogroup GroupCRC
* \{
* Cyclic Redundancy Check(CRC)is an error detection system.  The CRC code is a 
* remainder after dividing an input data string by a pre-defined generator 
* polynomial, assuming the input data string is a high order polynomial. 
* Ordinarily, the CRC code is added to the end of a data string when 
* transmitted. You divide the received data by the generator polynomial. 
* If the received data is dividable, it is judged that the data is correctly 
* received.<br>
* The CRC module supports both CCITT CRC16 and IEEE-802.3 CRC32. The generator 
* polynomial is fixed to the numeric values for those two modes for each mode, 
* and cannot be changed. You cannot generate a CRC value based on other 
* generator polynomials.<br>
* - CCITT CRC16 generator polynomial: 0x1021<br>
* - IEEE-802.3 CRC32 generator polynomial: 0x04C11DB7<br>
* \section SectionCRC_ConfigurationConsideration Configuration Consideration
* To set up the CRC, provide configuration parameters in the stc_crc_config_t  
* structure. For example, specify whether you are working in 16-bit or 32-bit 
* mode, the bit order, and byte order. Then call Crc_Init(). The results of 
* the CRC calculation will vary based on byte and bit order. The code that 
* confirms the CRC checksum must use the same configuration as the code that 
* created the checksum.<br>
* You can use PDL function calls Crc_PushX() to write data, 8-, 16-, or 32-bits
* at a time or use DMA. When done, call Crc_ReadResult()  to get the result of 
* the CRC calculation.<br>
* Be careful with the endian.
* \section SectionCRC_MoreInfo More Information
* For more information on the CRC peripheral, refer to:<br> 
* <a href="http://www.cypress.com/file/223051/download">FM0+ Peripheral Manual Core Subsystem</a><br>
* <a href="http://www.cypress.com/file/222996/download">FM4 Peripheral Manual Core Subsystem</a><br>
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
* \addtogroup GroupCRC_Types
* \{
*/
/******************************************************************************
 * Global type definitions
 ******************************************************************************/
  
/**
 ******************************************************************************
 ** \brief Crc mode
 ** 
 ** To select between CRC16 and CRC32
 ******************************************************************************/
typedef enum en_crc_mode
{
    Crc16  = 0,  ///< CCITT CRC16 standard
    Crc32  = 1   ///< IEEE-802.3 CRC32 Ethernet standard
} en_crc_mode_t;  

/** \}GroupCRC_Types */

/**
* \addtogroup GroupCRC_DataStructures
* \{
*/
/**
 *****************************************************************************
 ** \brief Crc configuration
 *****************************************************************************/
typedef struct stc_crc_config
{
    en_crc_mode_t enMode;               ///< See description of #en_crc_mode_t
    boolean_t     bUseDma;              ///< TRUE: DMA usage, needs DMA driver
    boolean_t     bFinalXor;            ///< TRUE: CRC result as XOR value
    boolean_t     bResultLsbFirst;      ///< CRC result bit order: TRUE: LSB first, FALSE MSB first
    boolean_t     bResultLittleEndian;  ///< CRC result byte order: TRUE: Litte endian, FLASE: big endian
    boolean_t     bDataLsbFirst;        ///< CRC feed data bit order: TRUE: LSB first, FALSE MSB first
    boolean_t     bDataLittleEndian;    ///< CRC feed data byte order: TRUE: Litte endian, FLASE: big endian
    uint32_t      u32CrcInitValue;      ///< Initial value, upper 16 bit ignored using CRC16
} stc_crc_config_t;

/** \} GroupCRC_DataStructures */
/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/
    
/**
* \addtogroup GroupCRC_Functions
* \{
*/ 
/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/

/* Init/DeInit */
extern en_result_t Crc_Init(const stc_crc_config_t* pstcConfig);
extern void Crc_DeInit(void);

/* Calucuration */
extern void Crc_Push8 (uint8_t   u8DataToPush);
extern void Crc_Push16(uint16_t u16DataToPush);
extern void Crc_Push32(uint32_t u32DataToPush);

/* Get result */
extern uint32_t Crc_ReadResult(void);

/** \} GroupCRC_Functions */
/** \} GroupCRC */

#ifdef __cplusplus
}
#endif

#endif /* #if (defined(PDL_PERIPHERAL_CRC_ACTIVE)) */

#endif /* __CRC_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
