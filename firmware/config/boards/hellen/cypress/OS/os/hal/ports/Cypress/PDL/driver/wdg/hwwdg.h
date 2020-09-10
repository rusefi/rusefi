/*******************************************************************************
* \file              hwwdg.h
* 
* \version           1.20
*
* \brief             Headerfile for HWWDG functions.
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

#ifndef __HWWDG_H__
#define __HWWDG_H__

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
* \defgroup GroupHWWDG Hardware Watchdog Timer (HWWDG)
* \{ 
* \defgroup GroupHWWDG_Macros Macros
* \defgroup GroupHWWDG_Functions Functions
* \defgroup GroupHWWDG_DataStructures Data Structures
* \defgroup GroupHWWDG_Types Enumerated Types
* \}
*/

/**
* \addtogroup GroupHWWDG
* \{
* This document describes the hardware watchdog timer (HWWDG). The FM families 
* also support a software watchdog timer. <br>
* A watchdog timer generates an interrupt or a system reset if the main program 
* neglects to periodically “feed” the watchdog.<br>
* The hardware watchdog timer is a 32-bit down counter. If a program does not reload 
* the watchdog timer within the specified interval time, it judges that a user program 
* is out of control. When the timer underflows, it outputs an interrupt request to the CPU. 
* If the interrupt request is not cleared, when the next underflow occurs the timer outputs a 
* reset request.<br>
* The HWWDG timer is clocked by the built-in low-speed CR oscillator (CLKLC). If the clock is 
* stopped, the timer stops. It retains its value and resumes the countdown when the clock restarts 
* after returning from standby mode.<br>
* Unlike the software watchdog, the HWWDG does not support a windowed watchdog.
* \section SectionHWWDG_ConfigurationConsideration Configuration Consideration
* To set up the HWWDG, you provide configuration parameters in the stc_hwwdg_config_t structure, such as the timer value, whether 
* to issue a reset, and the pointer to the interrupt callback function. Then call Hwwdg_Init().<br>
* To start the watchdog timer call Hwwdg_Start(). Use Hwwdg_Stop() if you want to stop the counter.<br>
* When the counter is running, you must call Hwwdg_Feed() or Hwwdg_QuickFeed().For example, you can 
* feed the watchdog in your interrupt callback routine. These two functions perform the same task. 
* However, Hwwdg_QuickFeed() is expanded inline for time-critical polling loops. Each function requires 
* two parameters – an arbitrary number, and its inverse. The hardware circuit requires this pattern to 
* clear the interrupt and reload the timer. <br>
* Hwwdg_DeInit() stops and disables the Hardware Watchdog. You must provide two “magic words” 
* for this function to be successful. Check the documentation for this function call for those values.
* \note 
*    - The Hardware Watchdog shares its interrupt vector with the NMI.
*    - The Hardware Watchdog is also switched off in System_Init() in
*      system_fmx.c. Set the definition for HWWD_DISABLE to 0 in 
*      system_fmx.h to enable the Hardware Watchdog during
*      start-up.
*
* \section SectionHWWDG_MoreInfo More Information
* For more information on the HWWDG peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/223041/download">FM0+ Peripheral Manual - Timer Subsystem TRM.pdf</a><br>
* <a href="http://www.cypress.com/file/222986/download">FM4 Peripheral Manual - Timer Subsystem TRM.pdf</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/**
* \addtogroup GroupHWWDG_Macros
* \{
*/

/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                           */
/*****************************************************************************/
/* for WDG_LCK */
#define HWWDG_REG_UNLOCK_1  (0x1ACCE551u)
#define HWWDG_REG_UNLOCK_2  (0xE5331AAEu)

/** \} GroupHWWDG_Macros */

/**
* \addtogroup GroupHWWDG_DataStructures
* \{
*/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/

/**
 *****************************************************************************
 ** \brief Hardware Watchdog configuration
 *****************************************************************************/
typedef struct stc_hwwdg_config
{
    uint32_t   u32LoadValue;    ///< Timer interval
    boolean_t  bResetEnable;    ///< TRUE:  Enables Hardware watchdog reset
    func_ptr_t pfnHwwdgIrqCb;   ///< Pointer to hardware watchdog interrupt callback function
    
} stc_hwwdg_config_t;

/** \} GroupHWWDG_DataStructures */

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/

/**
* \addtogroup GroupHWWDG_Functions
* \{
*/

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/
/* Interrupt */
void HwwdgIrqHandler(void);

/* Setup(Initialize)/Disable(Un-initialize) */
en_result_t Hwwdg_Init(const stc_hwwdg_config_t* pstcConfig);
en_result_t Hwwdg_DeInit(uint32_t u32MagicWord1,
                         uint32_t u32MagicWord2 );

/* Start/Stop */
en_result_t Hwwdg_Start(void);
void Hwwdg_Stop(void);

/* Write/Read for counter */
void Hwwdg_WriteWdgLoad(uint32_t u32LoadValue);
uint32_t Hwwdg_ReadWdgValue(void);

/* Feed watchdog */
void Hwwdg_Feed(uint8_t u8ClearPattern1, uint8_t u8ClearPattern2);

void Hwwdg_EnableDbgBrkWdgCtl(void);
void Hwwdg_DisableDbgBrkWdgCtl(void);

static void Hwwdg_QuickFeed(uint8_t u8ClearPattern1, uint8_t u8ClearPattern2);

/******************************************************************************/
/* Static inline functions                                                    */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief Quickly feed Hardware Watchdog (inline function)
 **
 ** This function feeds the Hardware Watchdog with the unlock, feed, and lock
 ** sequence as an inline function for quick execution in polling loops.
 ** Take care of the arbitrary values, because there are not checked for
 ** plausibility!
 **
 ** \param [in] u8ClearPattern1   Pattern of arbitrary value
 ** \param [in] u8ClearPattern2   Inverted arbitrary value
 ******************************************************************************/
static __INLINE void Hwwdg_QuickFeed(uint8_t u8ClearPattern1, uint8_t u8ClearPattern2)
{
    FM_HWWDT->WDG_LCK = HWWDG_REG_UNLOCK_1;/* Release Lock */
    FM_HWWDT->WDG_ICL = u8ClearPattern1;   /* Clear possible interrupt and reload value, step 1 */
    FM_HWWDT->WDG_ICL = u8ClearPattern2;   /* Clear possible interrupt and reload value, step 2 */
} /* Hwwdg_QuickFeed */

/** \} GroupHWWDG_Functions */
/** \} GroupHWWDG */

#ifdef __cplusplus
}
#endif

#endif /* #if (defined(PDL_PERIPHERAL_WDG_ACTIVE)) */

#endif /* __HWWDG_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
