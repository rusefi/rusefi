/******************************************************************************
* \file         hbif.h
*
* \version      1.20
*
* \brief        Headerfile for Hyper Bus Interface  functions
*
******************************************************************************* 
\copyright
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

#ifndef __HBIF_H__
#define __HBIF_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_HBIF_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif



/**
* \defgroup GroupHBIF Hyper Bus Interface (HBIF)
* \{ 
* \defgroup GroupHBIF_Functions Functions
* \defgroup GroupHBIF_DataStructures Data Structures
* \defgroup GroupHBIF_Types Enumerated Types
* \}
*/
/**
* \addtogroup GroupHBIF
* \{
* The HyperBus interface(HBIF) enables your code to access
* the HyperBus memory devices (both Flash and RAM). The HyperBus achieves high speed 
* read/write throughput by using a double data rate interface.<br>
* Features of the HyperBus Interface include:<br>
* - Supports operational frequency up to 166 MHz<br>
* - Achieves maximum 333 MB/s data throughput <br>
* - Supports the double data rate interface<br>
* - Supports maximum 4 GByte address space<br>
* - Supports two slave devices<br>
* - Supports XiP operation by dynamic wrapped burst request<br>
* - Has a low pin count<br>
*
* \section SectionHBIF_ConfigurationConsideration Configuration Consideration
* You specify general configuration settings in the stc_hbif_config_t structure. 
* This includes the address of two memory configurations, for memory space 0 and space 1. 
* You specify memory configuration in the stc_hbif_mem_config_t 
* structure and provide the pointers to these in the general configuration settings. <br>
* After specifying settings, call:<br>
* - Hbif_Init() to initialize HBIF configuration. <br>
*
* - Hbif_GetStatus() to get the status of HyberBus during data transaction.<br>
*
* - Hbif_GetInterruptFlag() to get the interrupt flag of HyberBus.<br>
*
* - Hbif_SetWriteProtection() to set write protection to external Hyper Flash.<br>
*
* - Hbif_SetGpo0Level() to set the level of GPO0 pin and Hbif_SetGpo1Level() sets
* the level of GPO1 pin.<br>

* \section SectionHBIF_MoreInfo More Information
* For more information on the HBIF peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/222976/download">FM4 Peripheral Manual Communication Subsystem</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/*****************************************************************************/
/* Global pre-processor symbols/macros ('define')                            */
/*****************************************************************************/
  
/******************************************************************************
 * Global definitions
 ******************************************************************************/

 /**
* \addtogroup GroupHBIF_Types
* \{
*/

/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/
/**
 *****************************************************************************
 ** \brief HyperBus interface status types
 *****************************************************************************/
typedef enum en_hbif_status
{
    HbifRstoDuringWrite           = 0u,  ///< RSTO Error in Write Transaction
    HbifTransactionErrDuringWrite = 1u,  ///< Transaction Error in Write Transaction
    HbifDecodeErrDuringWrite      = 2u,  ///< Decode Error in Write Transaction
    HbifWriteActive               = 3u,  ///< Write Transaction Active
    HbifRdsStallDuringRead        = 4u,  ///< RDS Stall Error in Read Transaction
    HbifRstoDuringRead            = 5u,  ///< RSTO Error in Read Transaction
    HbifTransactionErrDuringRead  = 6u,  ///< Transaction Error in Read Transaction
    HbifDecodeErrDuringRead       = 7u,  ///< Decode Error in Read Transaction
    HbifReadActive                = 8u,  ///< Read Transaction Active
    
}en_hbif_status_t;
   
/**
 *****************************************************************************
 ** \brief Wrap size types
 *****************************************************************************/
typedef enum en_hbif_wrap_size
{
    HbifWrap64Bytes = 1u, ///< Wrap bust length: 64 bytes
    HbifWrap16Bytes = 2u, ///< Wrap bust length: 16 bytes
    HbifWrap32Bytes = 3u, ///< Wrap bust length: 32 bytes
    
}en_hbif_wrap_size_t;
  
/**
 *****************************************************************************
 ** \brief Chip Select High Cycle Between Operations
 *****************************************************************************/
typedef enum en_hbif_cshi_cycle_t
{
    HbifCshi15Clk = 0u,   ///< Insert 1.5 clock to the chip select high period 
    HbifCshi25Clk = 1u,   ///< Insert 2.5 clock to the chip select high period 
    HbifCshi35Clk = 2u,   ///< Insert 3.5 clock to the chip select high period 
    HbifCshi45Clk = 3u,   ///< Insert 4.5 clock to the chip select high period 
    HbifCshi55Clk = 4u,   ///< Insert 5.5 clock to the chip select high period 
    HbifCshi65Clk = 5u,   ///< Insert 6.5 clock to the chip select high period
    HbifCshi75Clk = 6u,   ///< Insert 7.5 clock to the chip select high period
    HbifCshi85Clk = 7u,   ///< Insert 8.5 clock to the chip select high period
    HbifCshi95Clk = 8u,   ///< Insert 9.5 clock to the chip select high period
    HbifCshi105Clk = 9u,  ///< Insert 10.5 clock to the chip select high period
    HbifCshi115Clk = 10u, ///< Insert 11.5 clock to the chip select high period
    HbifCshi125Clk = 11u, ///< Insert 12.5 clock to the chip select high period
    HbifCshi135Clk = 12u, ///< Insert 13.5 clock to the chip select high period
    HbifCshi145Clk = 13u, ///< Insert 14.5 clock to the chip select high period
    HbifCshi155Clk = 14u, ///< Insert 15.5 clock to the chip select high period
    HbifCshi165Clk = 15u, ///< Insert 16.5 clock to the chip select high period
    
}en_hbif_rcshi_cycle_t, en_hbif_wcshi_cycle_t;

/**
 *****************************************************************************
 ** \brief  Chip Select Setup To Next CK Rising Edge
 *****************************************************************************/
typedef enum en_hbif_css_cycle_t
{  
    HbifCcs1Clk = 0u,    ///<  inserts the 1 cycles, between the falling edge of chip select and the rising edge of first CK.
    HbifCcs2Clk = 1u,    ///<  inserts the 2 cycles, between the falling edge of chip select and the rising edge of first CK.
    HbifCcs3Clk = 2u,    ///<  inserts the 3 cycles, between the falling edge of chip select and the rising edge of first CK.
    HbifCcs4Clk = 3u,    ///<  inserts the 4 cycles, between the falling edge of chip select and the rising edge of first CK.
    HbifCcs5Clk = 4u,    ///<  inserts the 5 cycles, between the falling edge of chip select and the rising edge of first CK.
    HbifCcs6Clk = 5u,    ///<  inserts the 6 cycles, between the falling edge of chip select and the rising edge of first CK.
    HbifCcs7Clk = 6u,    ///<  inserts the 7 cycles, between the falling edge of chip select and the rising edge of first CK.
    HbifCcs8Clk = 7u,    ///<  inserts the 8 cycles, between the falling edge of chip select and the rising edge of first CK.
    HbifCcs9Clk = 8u,    ///<  inserts the 9 cycles, between the falling edge of chip select and the rising edge of first CK.
    HbifCcs10Clk = 9u,   ///<  inserts the 10 cycles, between the falling edge of chip select and the rising edge of first CK.
    HbifCcs11Clk = 10u,  ///<  inserts the 11 cycles, between the falling edge of chip select and the rising edge of first CK.
    HbifCcs12Clk = 11u,  ///<  inserts the 12 cycles, between the falling edge of chip select and the rising edge of first CK.
    HbifCcs13Clk = 12u,  ///<  inserts the 13 cycles, between the falling edge of chip select and the rising edge of first CK.
    HbifCcs14Clk = 13u,  ///<  inserts the 14 cycles, between the falling edge of chip select and the rising edge of first CK.
    HbifCcs15Clk = 14u,  ///<  inserts the 15 cycles, between the falling edge of chip select and the rising edge of first CK.
    HbifCcs16Clk = 15u,  ///<  inserts the 16 cycles, between the falling edge of chip select and the rising edge of first CK.
    
}en_hbif_rcss_cycle_t, en_hbif_wcss_cycle_t;

/**
 *****************************************************************************
 ** \brief  Chip Select Setup To Next CK Rising Edge
 *****************************************************************************/
typedef enum en_hbif_csh_cycle_t
{
    HbifCsh1Clk = 0u,    ///<  inserts the 1 cycles, between the falling edge of last CK and the rising edge of chip select.
    HbifCsh2Clk = 1u,    ///<  inserts the 2 cycles, between the falling edge of last CK and the rising edge of chip select.
    HbifCsh3Clk = 2u,    ///<  inserts the 3 cycles, between the falling edge of last CK and the rising edge of chip select.
    HbifCsh4Clk = 3u,    ///<  inserts the 4 cycles, between the falling edge of last CK and the rising edge of chip select.
    HbifCsh5Clk = 4u,    ///<  inserts the 5 cycles, between the falling edge of last CK and the rising edge of chip select.
    HbifCsh6Clk = 5u,    ///<  inserts the 6 cycles, between the falling edge of last CK and the rising edge of chip select.
    HbifCsh7Clk = 6u,    ///<  inserts the 7 cycles, between the falling edge of last CK and the rising edge of chip select.
    HbifCsh8Clk = 7u,    ///<  inserts the 8 cycles, between the falling edge of last CK and the rising edge of chip select.
    HbifCsh9Clk = 8u,    ///<  inserts the 9 cycles, between the falling edge of last CK and the rising edge of chip select.
    HbifCsh10Clk = 9u,   ///<  inserts the 10 cycles, between the falling edge of last CK and the rising edge of chip select.
    HbifCsh11Clk = 10u,  ///<  inserts the 11 cycles, between the falling edge of last CK and the rising edge of chip select.
    HbifCsh12Clk = 11u,  ///<  inserts the 12 cycles, between the falling edge of last CK and the rising edge of chip select.
    HbifCsh13Clk = 12u,  ///<  inserts the 13 cycles, between the falling edge of last CK and the rising edge of chip select.
    HbifCsh14Clk = 13u,  ///<  inserts the 14 cycles, between the falling edge of last CK and the rising edge of chip select.
    HbifCsh15Clk = 14u,  ///<  inserts the 15 cycles, between the falling edge of last CK and the rising edge of chip select.
    HbifCsh16Clk = 15u,  ///<  inserts the 16 cycles, between the falling edge of last CK and the rising edge of chip select.
    
}en_hbif_rcsh_cycle_t, en_hbif_wcsh_cycle_t;

/**
 *****************************************************************************
 ** \brief   Latency Cycle for HyperRAM mode
 *****************************************************************************/
typedef enum en_hbif_ltcy_cycle
{
    HbRamLatency4Clk = 0u,
    HbRamLatency5Clk = 1u,
    HbRamLatency6Clk = 2u,
    
}en_hbif_ltcy_cycle_t;

/** \}GroupHBIF_Types */
/**
* \addtogroup GroupHBIF_DataStructures
* \{
*/

/**
 *****************************************************************************
 ** \brief Hyber Flash memory conguration structure
 *****************************************************************************/
typedef struct stc_hbif_mem_config
{
    uint32_t                    u32MemBaseAddress;               ///< The base address for memory space 
    boolean_t                   bMergeContinuousRead;            ///< TRUE: wrap transaction and subsequent continuous transaction can be merged
                                                                 ///< FALSE: wrap transaction and subsequent continuous transaction can't be merged 
    boolean_t                   bSupportAsymmetryCache;          ///< TRUE: asymmetry cache system support
                                                                 ///< FALSE: no asymmetry cache system support 
    boolean_t                   bRegisterSpace;                  ///< TRUE: access register space, FALSE: access memory space
    boolean_t                   bHyperRamDevice;                 ///< TRUE: connect with HyperRAM memory, FALSE: connect with HyperFlash memory
    en_hbif_wrap_size_t         enWrapBustsize;                  ///< Wrapped burst size type 
    en_hbif_rcshi_cycle_t       enReadCsHighCycle;               ///< Before the read access, this setting inserts the CK cycles to the chip select high period.
    en_hbif_wcshi_cycle_t       enWriteCsHighCycle;              ///< Before the write access, this setting inserts the CK cycles to the chip select high period.
    en_hbif_rcss_cycle_t        enReadCsNextClkCycle;            ///< In the read access, this setting inserts the CK cycles, between the falling edge of chip select and the rising edge of first CK.
    en_hbif_wcss_cycle_t        enWriteCsNextClkCycle;           ///< In the write access, this setting inserts the CK cycles, between the falling edge of chip select and the rising edge of first CK.
    en_hbif_rcsh_cycle_t        enReadCsHoldCycle;               ///< In the read access, this setting inserts the CK cycles, between the falling edge of last CK and the rising edge of chip select.
    en_hbif_wcsh_cycle_t        enWriteCsHoldCycle;              ///< In the write access, this setting inserts the CK cycles, between the falling edge of last CK and the rising edge of chip select
    en_hbif_ltcy_cycle_t        enHyperRamLatencyCycle;          ///< Latency Cycle for HyperRAM mode, ignored when the connected device is HyperFlash
    boolean_t                   bGpoOutputLevel;                 ///< TRUE: output high from GPO; FALSE: output low from GPO
    
}stc_hbif_mem_config_t;

/**
 *****************************************************************************
 ** \brief I2S configuration
 *****************************************************************************/
typedef struct stc_hbif_config
{
    stc_hbif_mem_config_t       *pstcMem0Config;                  ///< Pointer to memory space 0 configuration
    stc_hbif_mem_config_t       *pstcMem1Config;                  ///< Pointer to memory space 1 configuration
        
#if (PDL_INTERRUPT_ENABLE_HBIF == PDL_ON) 
    boolean_t                   bEnableInterrupt;                 ///< TRUE: enable hyper bus interrupt, FALSE: disable interrupt
    boolean_t                   bInterruptPolarity;               ///< TRUE: IENOn signal is active high, FALSE: IENOn signal is active low
    func_ptr_t                  pfnIrqCb;                         ///< Pointer to interrupt callback structure
    boolean_t                   bTouchNvic;                       ///< TRUE: Enable NVIC, FALSE: disable NVIC
#endif    
    
} stc_hbif_config_t;

/** \} GroupHBIF_DataStructures */

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/
/**
* \addtogroup GroupHBIF_Functions
* \{
*/ 
/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/
en_result_t Hbif_Init(const stc_hbif_config_t* pstcConfig) ;
en_result_t Hbif_DeInit(boolean_t bTouchNvic) ;
boolean_t Hbif_GetStatus(en_hbif_status_t enStatus);
boolean_t Hbif_GetInterruptFlag(void);
en_result_t Hbif_SetWriteProtection(boolean_t bEnableWp);
en_result_t Hbif_SetGpo0Level(boolean_t bLevel);
en_result_t Hbif_SetGpo1Level(boolean_t bLevel);


/** \} GroupHBIF_Functions */
/** \} GroupHBIF */

#ifdef __cplusplus
}
#endif

#endif

#endif /* __I2S_H__ */
/*****************************************************************************/
/* EOF (not truncated)                                                       */
/*****************************************************************************/
