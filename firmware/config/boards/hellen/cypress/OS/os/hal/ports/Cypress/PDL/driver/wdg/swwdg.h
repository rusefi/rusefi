/*******************************************************************************
* \file              swwdg.h
* 
* \version           1.20
*
* \brief             Headerfile for SWWDG functions.
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

#ifndef __SWWDG_H__
#define __SWWDG_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_WDG_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \defgroup GroupSWWDG Sofrware Watchdog Timer (SWWDG)
* \{ 
* \defgroup GroupSWWDG_Macros Macros
* \defgroup GroupSWWDG_Functions Functions
* \defgroup GroupSWWDG_DataStructures Data Structures
* \defgroup GroupSWWDG_Types Enumerated Types
* \}
*/

/**
* \addtogroup GroupSWWDG
* \{
* This document describes the software watchdog timer. 
* The FM families also support a hardware watchdog timer.
* A watchdog timer generates an interrupt or a system reset if the main program neglects to periodically “feed” the watchdog.
* The software watchdog timer (SWWDG) is a 32-bit down counter.  If a program does not reload the 
* watchdog timer within the specified interval time, it judges 
* that a user program is out of control. When the timer underflows, it outputs 
* an interrupt request to CPU.  If the interrupt request is not cleared, when the next underflow occurs the timer outputs a reset request.<br>
* The timer uses a divided APB bus clock. If the clock is stopped, the timer stops. 
* It retains its value and resumes 
* the countdown when the clock restarts after returning from  
* standby mode.<br>
* The software watchdog timer also supports a windowed watchdog. This implements a 
* further restriction upon feeding the watchdog, such that the feed must happen within a 
* certain time window. If the reload happens outside the time window, an interrupt or reset 
* request occurs.<br>
* 
* \section SectionSWWDG_ConfigurationConsideration Configuration Consideration
*
* To set up the SWWDG, you provide configuration parameters in the stc_swwdg_config_t
* structure. Note that you can disable the reset request if you wish. You also provide a 
* function pointer to the routine called by the watchdog interrupt.Then call Swwdg_Init(). <br>
* To start the watchdog timer, call Swwdg_Start(). Use Swwdg_Stop() if you want to stop the counter.<br>
* When the counter is running, you must call Swwdg_Feed() or Swwdg_QuickFeed() regularly to reload the watchdog
* timer.  These perform the same task. However, Swwdg_QuickFeed()
* is expanded inline for time-critical polling loops. For example, you might implement a separate dedicated 
* interrupt callback function that feeds the watchdog at regular intervals.<br>
*
* To implement Window Watchdog Mode set the bWinWdgEnable parameter to TRUE. 
* Specify the timing in u8TimingWindow. In Window Watchdog mode if the counter is 
* reloaded outside the timing window the Software Watchdog timer issues an interrupt or 
* reset. It also issues the interrupt or reset for an underflow. <br>

* \section SectionSWWDG_MoreInfo More Information
* For more information on the SWWDG peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/223041/download">FM0+ Peripheral Manual - Timer Subsystem TRM.pdf</a><br>
* <a href="http://www.cypress.com/file/222986/download">FM4 Peripheral Manual - Timer Subsystem TRM.pdf</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/**
* \addtogroup GroupSWWDG_Macros
* \{
*/

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/
/* for WDOGLOCK */
#define SWWDG_REG_UNLOCK    (0x1ACCE551u)

/** \} GroupSWWDG_Macros */

/**
* \addtogroup GroupSWWDG_Types
* \{
*/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief Software watchdog timing window settings
 ******************************************************************************/
typedef enum en_swwdg_timing_window
{
    en_swwdg_timing_window_100 = 0x00,    ///< Reload can be executed at less than or equal to WDOGLOAD.
    en_swwdg_timing_window_75  = 0x01,    ///< Reload can be executed at less than or equal to 75% of WDOGLOAD.
    en_swwdg_timing_window_50  = 0x02,    ///< Reload can be executed at less than or equal to 50% of WDOGLOAD
    en_swwdg_timing_window_25  = 0x03     ///< Reload can be executed at less than or equal to 25% of WDOGLOAD
} en_swwdg_timing_window_t;

/** \} GroupSWWDG_Types */

/**
* \addtogroup GroupSWWDG_DataStructures
* \{
*/

/**
 *****************************************************************************
 ** \brief Software and Hardware Watchdog configuration
 *****************************************************************************/
typedef struct stc_swwdg_config
{
  uint32_t   u32LoadValue;          ///< Timer interval
  boolean_t  bResetEnable;          ///< TRUE:  Enables SW watchdog reset
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
  boolean_t  bWinWdgEnable;         ///< TRUE:  Enables Window watchdog mode
  boolean_t  bWinWdgResetEnable;    ///< TRUE:  Enables reset when reload without timing window was occured.
                                    ///< FALSE: Enables interrupt when reload without timing window was occured.
  uint8_t    u8TimingWindow;        ///< Timing window setting, see description of #en_swwdg_timing_window_t
#endif    
  func_ptr_t pfnSwwdgIrqCb;         ///< Pointer to interrupt callback funciton
  
} stc_swwdg_config_t;

/** \} GroupSWWDG_DataStructures */

/**
* \addtogroup GroupSWWDG_Functions
* \{
*/

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/
/* Interrupt */
void SwwdgIrqHandler(void);

/* Setup(Initialize)/Disable(Un-initialize) */
en_result_t Swwdg_Init(const stc_swwdg_config_t* pstcConfig);
void Swwdg_DeInit(void);

/* Start/Stop */
en_result_t Swwdg_Start(void);
void Swwdg_Stop(void);

/* Write/Read for counter */
void Swwdg_WriteWdgLoad(uint32_t u32LoadValue);
uint32_t Swwdg_ReadWdgValue(void);

/* Feed watchdog */
void Swwdg_Feed(void) ;
static void Swwdg_QuickFeed(void);

/* Setting Debug Break Watchdog Timer Control */
void Swwdg_EnableDbgBrkWdgCtl(void);
void Swwdg_DisableDbgBrkWdgCtl(void);

/******************************************************************************/
/* Static inline functions                                                    */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief Quickly feed Software Watchdog (inline function)
 **
 ** This function feeds the Software Watchdog with the unlock, feed, and lock
 ** sequence as an inline function for quick execution in polling loops.
 ******************************************************************************/
static __INLINE void Swwdg_QuickFeed(void)
{
    FM_SWWDT->WDOGLOCK   = SWWDG_REG_UNLOCK;    /* Release Lock */
    FM_SWWDT->WDOGINTCLR = 1u;                  /* Clear possible interrupt and reload value */
    FM_SWWDT->WDOGLOCK   = 0u;                  /* Lock again it! */
} /* Swwdg_QuickFeed */

/** \} GroupSWWDG_Functions */
/** \} GroupSWWDG */

#ifdef __cplusplus
}
#endif

#endif /* #if (defined(PDL_PERIPHERAL_WDG_ACTIVE)) */

#endif /* __SWWDG_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
