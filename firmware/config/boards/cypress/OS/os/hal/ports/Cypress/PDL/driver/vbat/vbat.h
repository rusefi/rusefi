/*******************************************************************************
* \file              vbat.h
* 
* \version           1.20
*
* \brief             Headerfile for VBAT functions.
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

#ifndef __VBAT_H__
#define __VBAT_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if defined (FM4_RTC_TYPE_A)
    #define PDL_PERIPHERAL_VBAT_TYPE_A      1
#elif defined (FM4_RTC_TYPE_B) || defined (FM0P_RTC_TYPE_B)
    #define PDL_PERIPHERAL_VBAT_TYPE_B      1
#endif

#if defined (PDL_PERIPHERAL_VBAT_TYPE_A) || defined (PDL_PERIPHERAL_VBAT_TYPE_B)
    #define PDL_PERIPHERAL_VBAT_AVAILABLE   PDL_ON
#else
    #define PDL_PERIPHERAL_VBAT_AVAILABLE   PDL_OFF
#endif

#if (defined(PDL_PERIPHERAL_VBAT_ACTIVE))

#if (PDL_PERIPHERAL_VBAT_AVAILABLE == PDL_ON)

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
 
/**
* \defgroup GroupVBAT VBAT Domain (VBAT)
* \{ 
* \defgroup GroupVBAT_Functions Functions
* \defgroup GroupVBAT_DataStructures Data Structures
* \defgroup GroupVBAT_Types Enumerated Types
* \}
*/

/**
* \addtogroup GroupVBAT
* \{
* Use this peripheral block to manage battery power. Cypress FM microcontrollers have three power domains:<br>
* - CPU domain
* - Always-on domain
* - VBAT domain
* If the system power supply to the VCC pin is cut off, power to the CPU Domain and Always-on Domain is stopped.
* The VBAT domain gets power from a battery. The VBAT domain includes these circuits:<br>
* * Real time clock (RTC)
* * 32 kHz oscillation circuit
* * Power-on circuit
* * Back up registers: 32 bytes
* * Port circuit
* The VBAT power supply provides power to four I/O port pins, P46, P47, P48, and P49. This peripheral enables control of those pins.<br>
* 
* \note The VBAT domain is available only in FM4 devices (except type 5)
* and FM0+ type 2 devices.
*
* \section SectionVBAT_ConfigurationConsideration Configuration Consideration
* To set up a VBAT, you provide configuration parameters in the stc_vbat_config_t
* structure. Then call Vbat_Init(). This call clears the VBAT power-on flag and sets the transfer 
* clock division, sub oscillator configuration, and boost time current.<br>
* To set pin function, call Vbat_SetPinFunc_X() where X = VREGCTL, VWAKEUP,
* or X0A_X1A.<br>
* To initialize a VBAT GPIO pin for output, call Vbat_InitGpioOutput() with the pin, level, and drain. 
* To initialize a pin for input, use Vbat_InitGpioOutput with the pin and pullup.<br>
* You can set or get the level of any of the four pins with Vbat_PutPinX() or Vbat_GetPinX(), where X = P46, P47, P48, or P49.<br>
* Use other function calls start hibernation, read the power flag (which is set when VBAT is power on), and get or clear the wakeup flag.<br> 
*
* \section SectionVBAT_MoreInfo More Information
* For more information on the VBAT peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/223051/download">FM0+ Peripheral Manual - Core Subsystem TRM.pdf</a><br>
* <a href="http://www.cypress.com/file/222996/download">FM4 Peripheral Manual - Core Subsystem TRM.pdf</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')      
 ******************************************************************************/

#ifdef PDL_PERIPHERAL_VBAT_TYPE_A   

/**
* \addtogroup GroupVBAT_Types
* \{
*/
    
/**
 ******************************************************************************
 ** \brief Sustain and Boost Current
 ******************************************************************************/
typedef enum en_clk_current
{
    Clk0nA          = 0u,  ///< 0 nA sustain/boost, not allowed, if subclock is enabled
    Clk385nA        = 1u,  ///< 385 nA
    Clk445nA        = 2u,  ///< 445 nA, initial value for current sustain
    Clk510nA        = 3u,  ///< 510 nA, initial value for current boost
    ClkErrorCurrent = 4u,  ///< Errornous setting (for read-out function)
} en_clk_current_t;
#else
typedef enum en_clk_current
{
    ClkLowPower     = 0u, ///< Low power mode. Allows for a smaller type of crystal oscillator
    ClkStandard     = 1u  ///< Standard power mode (initial value)
} en_clk_current_t;
#endif

/**
 ******************************************************************************
 ** \brief Sustain and Boost Time
 ******************************************************************************/
typedef enum en_clk_boost_time
{
    ClkBoost50ms  = 0u,    ///< Boost time 50 ms (initial value)
    ClkBoost63ms  = 1u,    ///< Boost time 62.5 ms
    ClkBoost125ms = 2u,    ///< Boost time 125 ms
    ClkBoost250ms = 3u     ///< Boost time 250 ms
} en_clk_boost_time_t;

/**
 ******************************************************************************
 ** \brief VBAT GPIO selection
 *******************************************************************************/
typedef enum en_vbat_gpio
{
    VbatGpioP46 = 0u,  ///< P46 pin
    VbatGpioP47 = 1u,  ///< P47 pin
    VbatGpioP48 = 2u,  ///< P48 pin
    VbatGpioP49 = 3u,  ///< P49 pin
    
}en_vbat_gpio_t;

/** \} GroupVBAT_Types */

/**
* \addtogroup GroupVBAT_DataStructures
* \{
*/

/**
 ******************************************************************************
 ** \brief VBAT configuration
 *******************************************************************************/
typedef struct stc_vbat_config
{
    uint8_t    u8ClkDiv;     ///< Transfer clock division of PREAD, PWRITE, BREAD, BWRITE
    boolean_t  bLinkSubClk;  ///< FALSE: The 32 kHz oscillation circuit operates independently as VBAT Domain
                             ///< TRUE:  The 32 kHz oscillation circuit is linked with the clock control circuit.
    boolean_t  bVbatClkEn;   ///< FALSE: Stops the oscillation of VBAT  
                             ///< TRUE: Starts the oscillation of VBAT
    en_clk_current_t enSustainCurrent;  ///< Sustain current of sub clock 
    en_clk_current_t enBoostCurrent;    ///< Boost current of sub clock 
    en_clk_boost_time_t enClkBoostTime; ///< Boost time of sub clock 
    
}stc_vbat_config_t;


/** \} GroupVBAT_DataStructures */

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/

/**
* \addtogroup GroupVBAT_Functions
* \{
*/

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/
/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
/* VBAT init */
en_result_t Vbat_Init(const stc_vbat_config_t* pstcConfig);
/* Function pin setting */
void Vbat_SetPinFunc_VREGCTL(void);
void Vbat_SetPinFunc_VWAKEUP(void);
void Vbat_SetPinFunc_X0A_X1A(void);
/* GPIO setting */
en_result_t Vbat_InitGpioOutput(en_vbat_gpio_t enGpio, 
                                boolean_t bInitLevel, 
                                boolean_t bOpenDrain);
en_result_t Vbat_InitGpioInput(en_vbat_gpio_t enGpio, boolean_t bPullup);
void Vbat_PutPinP46(boolean_t bLevel);
void Vbat_PutPinP47(boolean_t bLevel);
void Vbat_PutPinP48(boolean_t bLevel);
void Vbat_PutPinP49(boolean_t bLevel);
boolean_t Vbat_GetPinP46(void);
boolean_t Vbat_GetPinP47(void);
boolean_t Vbat_GetPinP48(void);
boolean_t Vbat_GetPinP49(void);
/* Start hibernation function */
void Vbat_StartHibernation(void);
/* Get VBAT power on flag */
boolean_t Vbat_GetPowerOnFlag(void);
/* Get/clear hibernation wakeup flag */
boolean_t Vbat_GetWakeupFlag(void);
void Vbat_ClrWakeupFlag(void);

/** \} GroupVBAT_Functions */
/** \} GroupVBAT */

#ifdef __cplusplus
}
#endif

#endif

#endif // #if (defined(PDL_PERIPHERAL_VBAT_ACTIVE))

#endif /* __LPM_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
