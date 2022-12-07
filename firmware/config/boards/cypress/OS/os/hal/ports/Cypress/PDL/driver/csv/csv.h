/******************************************************************************
* \file             csv.h
*
* \version          1.20
*
* \brief            Headerfile for Clock Supervisor functions
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

#ifndef _CSV_H_
#define _CSV_H_

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_CSV_ACTIVE))
/**
* \defgroup GroupCSV Clock Supervisor(CSV)
* \{
* \defgroup GroupCSV_Functions Functions 
* \defgroup GroupCSV_DataStructures Data Structures
* \defgroup GroupCSV_Types Enumerated Types
* \}
*/

/**
* \addtogroup GroupCSV
* \{
* The Clock Supervisor peripheral monitors particular clocks for failure or 
* anomalous frequency. <br>
* <b>Clock Failure Detection</b> <br>
* The clock failure detection function monitors the main and sub clocks 
* independently. If a rising edge of the monitored clock is not detected within
* the specified period, this function determines that the oscillator has 
* failed, and outputs a system reset request.<br>
* - it stops monitoring when the main and sub oscillators stop oscillating<br>
* - it stops monitoring while waiting for oscillation stabilization wait 
* time<br>
* - is automatically enabled when the wait time ends<br>
*
* <b>Anomalous Frequency Detection by Clock (FCS)</b><br>
* The anomalous frequency detection function monitors the main clock. Within 
* the specified period between a rising edge and the next rising edge of the 
* divided high-speed CR clock, this function counts up the internal counter 
* using the main clock. If the count value is outside of a set range, the 
* function determines that the main clock frequency is anomalous, 
* and outputs an interrupt request or a system reset request to the CPU.<br>
* - it monitors frequency of the main clock only<br>
* - it stops monitoring when the main oscillator stops oscillating<br>
* - it stops monitoring while waiting for oscillation stabilization wait 
* time<br>
* - is not automatically enabled<br>
* 
* \section SectionCSV_ConfigurationConsideration Configuration Consideration
* There is no configuration required. Use PDL function calls to enable, 
* disable, and get status for the clock failure detection.
* To get CSV status, declare a structure of type stc_csv_status_t, and call 
* Csv_GetCsvFailCause(). <br>
* Similarly, use function calls to manage the FCS features. Note that FCS is 
* not automatically enabled. Call Csv_EnableFcs()  to 
* start this feature. You set the FCS range with Csv_SetFcsDetectRange().<br>  
* You can enable or disable the FCS interrupt or reset request. If you enable 
* the FCS interrupt, you provide a function pointer to a callback routine to 
* handle the interrupt.<br>
* 
* \section SectionCSV_MoreInfo More Information
* For more information on the CSV peripheral, refer to:<br> 
* <a href="http://www.cypress.com/file/223051/download">FM0+ Peripheral Manual - Core Subsystem TRM.pdf</a><br>
* <a href="http://www.cypress.com/file/222996/download">FM4 Peripheral Manual - Core Subsystem TRM.pdf</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/**
* \addtogroup GroupCSV_Types
* \{
*/
/******************************************************************************
 * Global type definitions
 ******************************************************************************/
/**
 ******************************************************************************
 ** \brief Enumeration to define an index for each BT instance
 ******************************************************************************/     
typedef enum en_fcs_cr_div
{
    FcsCrDiv256 = 5u,        ///< 1/256 frequency of high-speed CR oscillation   
    FcsCrDiv512 = 6u,        ///< 1/512 frequency of high-speed CR oscillation   
    FcsCrDiv1024 = 7u,       ///< 1/1024 frequency of high-speed CR oscillation
    
}en_fcs_cr_div_t;

/** \}GroupCSV_Types */

/**
* \addtogroup GroupCSV_DataStructures
* \{
*/

/**
 ******************************************************************************
 ** \brief Structure of CSV status
 ******************************************************************************/
typedef struct stc_csv_status
{
    boolean_t bCsvMainClockStatus;
    boolean_t bCsvSubClockStatus;
    
}stc_csv_status_t;

/** \} GroupCSV_DataStructures */

/**
* \addtogroup GroupCSV_Functions
* \{
*/
 
/**
 ******************************************************************************
 ** \brief FCS interrupt callback function type
 ******************************************************************************/
typedef void fn_fcs_int_callback(void);

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/
/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif
/* CSV */  
void Csv_EnableMainCsv(void);
void Csv_DisableMainCsv(void);
void Csv_EnableSubCsv(void);
void Csv_DisableSubCsv(void);
uint8_t Csv_GetCsvFailCause(stc_csv_status_t* pstcCsvStatus);
/* FCS */  
void Csv_EnableFcs(void);
void Csv_DisableFcs(void);
void Csv_EnableFcsReset(void);
void Csv_DisableFcsReset(void);
#if (PDL_INTERRUPT_ENABLE_CSV == PDL_ON) 
en_result_t Csv_EnableFcsIrq(fn_fcs_int_callback* pfnIrqCb);
void Csv_DisableFcsIrq(void);
#endif
void Csv_ClrFcsIrqFlag(void);
uint8_t Csv_GetFcsIrqFlag(void);
en_result_t Csv_SetFcsCrDiv(en_fcs_cr_div_t enDiv) ;
void Csv_SetFcsDetectRange(uint16_t u16LowerVal, uint16_t u16UpperVal);
uint16_t Csv_GetFcsDetectCount(void);
/* IRQ */
void Csv_IrqHandler(void);

/** \} GroupCSV_Functions */
/** \} GroupCSV */

#ifdef __cplusplus
}
#endif


#endif

#endif /* _CLOCK_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */
