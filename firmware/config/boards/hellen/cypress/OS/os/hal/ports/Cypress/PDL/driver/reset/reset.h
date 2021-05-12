/*******************************************************************************
* \file              reset.h
* 
* \version           1.20
*
* \brief             Headerfile for RESET functions.
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

#ifndef __RESET_H__
#define __RESET_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "pdl_user.h"
#include "mcu.h"

#if (defined(PDL_PERIPHERAL_RESET_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \defgroup GroupRESET Reset (RESET)
* \{ 
* \defgroup GroupRESET_Functions Functions
* \defgroup GroupRESET_DataStructures Data Structures
* \}
*/

/**
* \addtogroup GroupRESET
* \{ 
* The Reset peripheral lets you to determine the cause of a device reset:
* - Power-on 
* - INITX pin input
* - External power supply/low-voltage detection 
* - Software watchdog 
* - Hardware watchdog 
* - Clock failure detection 
* - Anomalous frequency detection 
* - Software 
* - Deep standby transition  
*
* \note There are differences in the reset factor registers among the FM3 device 
* types. FM3 types 3 and 7 provide an additional LVDH bit, which indicates a 
* low-voltage detection reset. FM3 device types 3 and 7 provide an additional 
* deep-standby transition reset factor indication besides power-on reset and 
* low-voltage reset. FM0+ and FM4 devices do not provide low-voltage and 
* deep-standby transition reset factors.
*
* \section SectionRESET_ConfigurationConsideration Configuration Consideration
* This peripheral does not require configuration. To use the functions in this peripheral, 
* declare an stc_reset_result_t structure to receive the results.<br>
* Call Reset_GetCause() to get the reset cause. This fills in the stc_reset_result_t structure, 
* and stores the same information for later reference. This function clears the hardware register, 
* so the source of the reset is no longer obtainable directly.<br>
* After getting the reset cause, you can use Reset_GetStoredCause() to get the stored value, 
* without touching the reset cause register.<br>
* \note Do not call Reset_GetCause() before startup code has executed.
* 
* \section SectionRESET_MoreInfo More Information
* For more information on the RESET peripheral, refer to:<br>
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
* \addtogroup GroupRESET_DataStructures
* \{
*/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/
  
/**
 ******************************************************************************
 ** \brief Datatype for Reset Cause return structure
 ******************************************************************************/
typedef struct stc_reset_result
{
  boolean_t    bPowerOn;              ///< TRUE: Power on reset occurred
  boolean_t    bInitx;                ///< TRUE: INITX (external) reset occurred
#if (PDL_MCU_TYPE == PDL_FM3_TYPE3) ||  (PDL_MCU_TYPE == PDL_FM3_TYPE7) 
  boolean_t    bLowVoltageDetection;  ///< TRUE: Low Voltage Detection reset ocucrred (only applicable for FM3 Type3 and 7, always FALSE otherwise)
#endif  
  boolean_t    bSoftwareWatchdog;     ///< TRUE: Software Watchdog reset occurred
  boolean_t    bHardwareWatchdog;     ///< TRUE: Hardware Watchdog reset occurred
  boolean_t    bClockSupervisor;      ///< TRUE: Clock Supervisor reset occurred
  boolean_t    bAnomalousFrequency;   ///< TRUE: Anomalous Frequency reset occurred
  boolean_t    bSoftware;             ///< TRUE: Software reset occurred
} stc_reset_result_t ;

/** \} GroupRESET_DataStructures */

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/

/**
* \addtogroup GroupRESET_Functions
* \{
*/

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/

extern en_result_t Reset_GetCause( stc_reset_result_t* pstcResult ) ;
extern en_result_t Reset_GetStoredCause( stc_reset_result_t* pstcResult );

/** \} GroupRESET_Functions */
/** \} GroupRESET */

#ifdef __cplusplus
}
#endif

#endif // #if (defined(PDL_PERIPHERAL_RESET_ACTIVE))

#endif /* __RESET_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
