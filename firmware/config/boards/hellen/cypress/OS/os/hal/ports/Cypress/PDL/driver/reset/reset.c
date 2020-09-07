/*******************************************************************************
* \file             reset.c
* 
* \version          1.20
*
* \brief            This file provides the source code to the API for the RESET 
*                   driver.
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

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "reset/reset.h"
#include <string.h>

#if (defined(PDL_PERIPHERAL_RESET_ACTIVE))

/******************************************************************************/
/* Local pre-processor symbols/macros ('#define')                             */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions (declared in header file with 'extern')        */
/******************************************************************************/
stc_reset_result_t stcStoredResetCause; ///< Global reset cause register
  

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Local function prototypes ('static')                                       */
/******************************************************************************/

/******************************************************************************/
/* Local variable definitions ('static')                                      */
/******************************************************************************/

/******************************************************************************/
/* Function implementation - global ('extern') and local ('static')           */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief Get current Reset Cause Register contents
 **
 ** This function reads the Reset Cause Register and stores the cause bits
 ** in the result structure pointer.
 **
 ** \attention Calling this function clears all bits in the Reset Cause Register
 **            RST_STR! Reset_GetCause() should only be called after Start-up
 **            code!
 **
 ** \param [out]  pstcResult     Reset Cause result structure
 **
 ** \retval Ok                   Function done sucessfully
 **
 ******************************************************************************/
en_result_t Reset_GetCause(stc_reset_result_t* pstcResult)
{
  stc_crg_rst_str_field_t stcReadResetCause;
  uint16_t u16ResetCause;
  
  u16ResetCause = FM_CRG->RST_STR;
  stcReadResetCause = *(stc_crg_rst_str_field_t*)&u16ResetCause;

  // Check Power-on bit
  if (1u == stcReadResetCause.PONR)
  {
    pstcResult->bPowerOn = TRUE;
  }
  else
  {
    pstcResult->bPowerOn = FALSE;
  }
  
  // Check INITX/External Reset bit
  if (1u == stcReadResetCause.INITX)
  {
    pstcResult->bInitx = TRUE;
  }
  else
  {
    pstcResult->bInitx = FALSE;
  }

#if (PDL_MCU_TYPE == PDL_FM3_TYPE3) ||  (PDL_MCU_TYPE == PDL_FM3_TYPE7)   
  // Check Low Voltage Detection bit
  if (1u == stcReadResetCause.LVDH)
  {
    pstcResult->bLowVoltageDetection = TRUE;
  }
  else
  {
    pstcResult->bLowVoltageDetection = FALSE;
  }
#endif  
  
  // Check Software Watchdog bit
  if (1u == stcReadResetCause.SWDT)
  {
    pstcResult->bSoftwareWatchdog = TRUE;
  }
  else
  {
    pstcResult->bSoftwareWatchdog = FALSE;
  }
  
  // Check Hardware Watchdog bit
  if (1u == stcReadResetCause.HWDT)
  {
    pstcResult->bHardwareWatchdog = TRUE;
  }
  else
  {
    pstcResult->bHardwareWatchdog = FALSE;
  }
  
  // Check Clock Supervisor bit
  if (1u == stcReadResetCause.CSVR)
  {
    pstcResult->bClockSupervisor = TRUE;
  }
  else
  {
    pstcResult->bClockSupervisor = FALSE;
  }
  
  // Check Anomalous Frequency bit
  if (1u == stcReadResetCause.FCSR)
  {
    pstcResult->bAnomalousFrequency = TRUE;
  }
  else
  {
    pstcResult->bAnomalousFrequency = FALSE;
  }
  
  // Check Software Reset bit
  if (1u == stcReadResetCause.SRST)
  {
    pstcResult->bSoftware = TRUE;
  }
  else
  {
    pstcResult->bSoftware = FALSE;
  }

  // Copy reset cause structure argument contents to global reset cause structure
  memcpy(&stcStoredResetCause, pstcResult, sizeof(stcStoredResetCause));
  
  return Ok;
} // Reset_GetCause

/**
 ******************************************************************************
 ** \brief Get stored Reset Cause
 **
 ** This function reads the gloabal Reset Cause Variable
 **
 ** \pre Reset_GetCause() must be called before.
 **
 ** \param [out]  pstcResult     Reset Cause result structure
 **
 ** \retval Ok                   Function done sucessfully
 **
 ******************************************************************************/
en_result_t Reset_GetStoredCause( stc_reset_result_t* pstcResult )
{
  // Copy global reset cause structure contents to reset cause structure argument 
  memcpy(pstcResult, &stcStoredResetCause, sizeof(stcStoredResetCause));
  
  return Ok;
}

#endif // #if (defined(PDL_PERIPHERAL_RESET_ACTIVE))

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
