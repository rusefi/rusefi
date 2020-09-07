/*******************************************************************************
* \file              lvd.h
* 
* \version           1.20
*
* \brief             Headerfile for LVD functions.
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

#ifndef __LVD_H__
#define __LVD_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_LVD_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \defgroup GroupLVD Low Voltage Detection (LVD)
* \{ 
* \defgroup GroupLVD_Macros Macros
* \defgroup GroupLVD_Functions Functions
* \defgroup GroupLVD_DataStructures Data Structures
* \defgroup GroupLVD_Types Enumerated Types
* \}
*/

/**
* \addtogroup GroupLVD
* \{
* The Low-voltage Detection circuit (LVD) monitors the power supply voltage 
* and generates a reset when the power supply voltage falls below a specified
* voltage.<br>
* LVD can also generate an interrupt, rather than a reset, if the power 
* supply voltage falls below the detection threshold. You can set two 
* threshold levels, and generate an interrupt for each.<br>
* Support for LVD varies among FM products. Refer to the Peripheral Manual - 
* Core Subsytem for details for your part. For example, the voltage range 
* differs among device types. The threshold for generating an interrupt 
* and/or reset will vary.
* \section SectionLVD_ConfigurationConsideration Configuration Consideration
* To set up the LVD, you provide configuration parameters in the 
* stc_lvd_config_t structure. For example, you set the voltage threshold 
* for a reset. You can set two thresholds for interrupt generation. 
* Use enumerated constants for the various thresholds and set the values 
* in enLvd0IrqDetectVoltage and enLvd1IrqDetectVoltage. You also 
* specify the interrupt callback function for each interrupt in 
* pfnIrqCallback.<br>
* You can also set the enLvd0IrqReleaseVoltage, and enLvd1IrqReleaseVoltage, 
* but this applies only to FM0+ Type 2 parts.<br>
* Then call Lvd_Init() to initialize the voltage thresholds for the interrupt and 
* reset circuits.<br>
* If you do not provide a callback function, use Lvd_GetIrqStatus() to poll for 
* the interrupt, and clear the interrupt with Lvd_ClearIrqStatus().<br>
* At runtime you can use API function calls to:<br>
* * Enable, disable, or get the status of either low voltage interrupt
* * Enable, disable, or get the status of the low voltage reset
*
* \section SectionLVD_MoreInfo More Information
* For more information on the LVD peripheral, refer to:<br>
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
 
/******************************************************************************
 * Global type definitions
 ******************************************************************************/

/**
* \addtogroup GroupLVD_Types
* \{
*/

/**
 ******************************************************************************
 ** \brief LVD ch.0 Interruption voltage settings for Low Voltage Detection
 ******************************************************************************/  
typedef enum en_lvd0_irq_detect_voltage
{
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE1) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE9)  || (PDL_MCU_TYPE == PDL_FM3_TYPE10) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE11) || (PDL_MCU_TYPE == PDL_FM3_TYPE12)
    Lvd0IrqDetectVoltage280 = 3u,   ///< Interrupt when voltage is vicinity of 2.80 volts
    Lvd0IrqDetectVoltage300 = 4u,   ///< Interrupt when voltage is vicinity of 3.00 volts
    Lvd0IrqDetectVoltage320 = 5u,   ///< Interrupt when voltage is vicinity of 3.20 volts
    Lvd0IrqDetectVoltage360 = 6u,   ///< Interrupt when voltage is vicinity of 3.60 volts
    Lvd0IrqDetectVoltage370 = 7u,   ///< Interrupt when voltage is vicinity of 3.70 volts
    Lvd0IrqDetectVoltage400 = 8u,   ///< Interrupt when voltage is vicinity of 4.00 volts
    Lvd0IrqDetectVoltage410 = 9u,   ///< Interrupt when voltage is vicinity of 4.10 volts
    Lvd0IrqDetectVoltage420 = 10u,  ///< Interrupt when voltage is vicinity of 4.20 volts
    Lvd0IrqDetectVoltageMax,        ///< Max index of LVD interrupt voltage
#elif (PDL_MCU_TYPE == PDL_FM4_TYPE1) || (PDL_MCU_TYPE == PDL_FM4_TYPE2) || (PDL_MCU_TYPE == PDL_FM4_TYPE6)
    Lvd0IrqDetectVoltage280 = 7u,   ///< Interrupt when voltage is vicinity of 2.80 volts
    Lvd0IrqDetectVoltage300 = 4u,   ///< Interrupt when voltage is vicinity of 3.00 volts
    Lvd0IrqDetectVoltage320 = 12u,  ///< Interrupt when voltage is vicinity of 3.20 volts
    Lvd0IrqDetectVoltage360 = 15u,  ///< Interrupt when voltage is vicinity of 3.60 volts
    Lvd0IrqDetectVoltage370 = 14u,  ///< Interrupt when voltage is vicinity of 3.70 volts
    Lvd0IrqDetectVoltage400 = 9u,   ///< Interrupt when voltage is vicinity of 4.00 volts
    Lvd0IrqDetectVoltage410 = 8u,   ///< Interrupt when voltage is vicinity of 4.10 volts
    Lvd0IrqDetectVoltage420 = 24u,  ///< Interrupt when voltage is vicinity of 4.20 volts
    Lvd0IrqDetectVoltageMax,        ///< Max index of LVD interrupt voltage
#elif (PDL_MCU_TYPE == PDL_FM4_TYPE3) || (PDL_MCU_TYPE == PDL_FM4_TYPE4) || (PDL_MCU_TYPE == PDL_FM4_TYPE5) 
    Lvd0IrqDetectVoltage290 = 7u,   ///< Interrupt when voltage is vicinity of 2.90 volts
    Lvd0IrqDetectVoltage310 = 4u,   ///< Interrupt when voltage is vicinity of 3.10 volts
    Lvd0IrqDetectVoltage330 = 12u,  ///< Interrupt when voltage is vicinity of 3.30 volts
    Lvd0IrqDetectVoltage380 = 15u,  ///< Interrupt when voltage is vicinity of 3.80 volts
    Lvd0IrqDetectVoltage390 = 14u,  ///< Interrupt when voltage is vicinity of 3.90 volts
    Lvd0IrqDetectVoltage420 = 9u,   ///< Interrupt when voltage is vicinity of 4.20 volts
    Lvd0IrqDetectVoltage430 = 8u,   ///< Interrupt when voltage is vicinity of 4.30 volts
    Lvd0IrqDetectVoltage440 = 24u,  ///< Interrupt when voltage is vicinity of 4.40 volts   
    Lvd0IrqDetectVoltageMax,        ///< Max index of LVD interrupt voltage
#elif (PDL_MCU_TYPE == PDL_FM3_TYPE0) || (PDL_MCU_TYPE == PDL_FM3_TYPE1) || \
      (PDL_MCU_TYPE == PDL_FM3_TYPE2) || (PDL_MCU_TYPE == PDL_FM3_TYPE4) || \
      (PDL_MCU_TYPE == PDL_FM3_TYPE5)  
    Lvd0IrqDetectVoltage280 = 0u,   ///< Interrupt when voltage is vicinity of 2.80 volts
    Lvd0IrqDetectVoltage300 = 1u,   ///< Interrupt when voltage is vicinity of 3.00 volts
    Lvd0IrqDetectVoltage320 = 2u,   ///< Interrupt when voltage is vicinity of 3.20 volts
    Lvd0IrqDetectVoltage360 = 3u,   ///< Interrupt when voltage is vicinity of 3.60 volts
    Lvd0IrqDetectVoltage370 = 4u,   ///< Interrupt when voltage is vicinity of 3.70 volts
    Lvd0IrqDetectVoltage400 = 7u,   ///< Interrupt when voltage is vicinity of 4.00 volts
    Lvd0IrqDetectVoltage410 = 8u,   ///< Interrupt when voltage is vicinity of 4.10 volts
    Lvd0IrqDetectVoltage420 = 9u,   ///< Interrupt when voltage is vicinity of 4.20 volts    
    Lvd0IrqDetectVoltageMax,        ///< Max index of LVD interrupt voltage
#elif (PDL_MCU_TYPE == PDL_FM3_TYPE3) || (PDL_MCU_TYPE == PDL_FM3_TYPE7)
    Lvd0IrqDetectVoltage200 = 0u,   ///< Interrupt when voltage is vicinity of 2.00 volts
    Lvd0IrqDetectVoltage210 = 1u,   ///< Interrupt when voltage is vicinity of 2.10 volts  
    Lvd0IrqDetectVoltage220 = 2u,   ///< Interrupt when voltage is vicinity of 2.20 volts
    Lvd0IrqDetectVoltage230 = 3u,   ///< Interrupt when voltage is vicinity of 2.30 volts
    Lvd0IrqDetectVoltage240 = 4u,   ///< Interrupt when voltage is vicinity of 2.40 volts
    Lvd0IrqDetectVoltage250 = 5u,   ///< Interrupt when voltage is vicinity of 2.50 volts
    Lvd0IrqDetectVoltage260 = 6u,   ///< Interrupt when voltage is vicinity of 2.60 volts
    Lvd0IrqDetectVoltage280 = 7u,   ///< Interrupt when voltage is vicinity of 2.80 volts
    Lvd0IrqDetectVoltage300 = 8u,   ///< Interrupt when voltage is vicinity of 3.00 volts
    Lvd0IrqDetectVoltage320 = 9u,   ///< Interrupt when voltage is vicinity of 3.20 volts
    Lvd0IrqDetectVoltage360 = 10u,  ///< Interrupt when voltage is vicinity of 3.60 volts
    Lvd0IrqDetectVoltage370 = 11u,  ///< Interrupt when voltage is vicinity of 3.70 volts
    Lvd0IrqDetectVoltage400 = 12u,  ///< Interrupt when voltage is vicinity of 4.00 volts
    Lvd0IrqDetectVoltage410 = 13u,  ///< Interrupt when voltage is vicinity of 4.10 volts
    Lvd0IrqDetectVoltage420 = 14u,  ///< Interrupt when voltage is vicinity of 4.20 volts
    Lvd0IrqDetectVoltageMax,        ///< Max index of LVD interrupt voltage
#elif (PDL_MCU_TYPE == PDL_FM3_TYPE6) || (PDL_MCU_TYPE == PDL_FM3_TYPE8)
    Lvd0IrqDetectVoltage170 = 4u,   ///< Interrupt when voltage is vicinity of 1.70 volts
    Lvd0IrqDetectVoltage175 = 5u,   ///< Interrupt when voltage is vicinity of 1.75 volts  
    Lvd0IrqDetectVoltage180 = 6u,   ///< Interrupt when voltage is vicinity of 1.80 volts
    Lvd0IrqDetectVoltage185 = 7u,   ///< Interrupt when voltage is vicinity of 1.85 volts
    Lvd0IrqDetectVoltage190 = 8u,   ///< Interrupt when voltage is vicinity of 1.90 volts
    Lvd0IrqDetectVoltage195 = 9u,   ///< Interrupt when voltage is vicinity of 1.95 volts
    Lvd0IrqDetectVoltage200 = 10u,  ///< Interrupt when voltage is vicinity of 2.00 volts
    Lvd0IrqDetectVoltage205 = 11u,  ///< Interrupt when voltage is vicinity of 2.05 volts
    Lvd0IrqDetectVoltage250 = 12u,  ///< Interrupt when voltage is vicinity of 2.50 volts    
    Lvd0IrqDetectVoltage260 = 13u,  ///< Interrupt when voltage is vicinity of 2.60 volts
    Lvd0IrqDetectVoltage270 = 14u,  ///< Interrupt when voltage is vicinity of 2.70 volts
    Lvd0IrqDetectVoltage280 = 15u,  ///< Interrupt when voltage is vicinity of 2.80 volts
    Lvd0IrqDetectVoltage290 = 16u,  ///< Interrupt when voltage is vicinity of 2.90 volts
    Lvd0IrqDetectVoltage300 = 17u,  ///< Interrupt when voltage is vicinity of 3.00 volts
    Lvd0IrqDetectVoltage310 = 18u,  ///< Interrupt when voltage is vicinity of 3.10 volts
    Lvd0IrqDetectVoltage320 = 19u,  ///< Interrupt when voltage is vicinity of 3.20 volts
    Lvd0IrqDetectVoltageMax,        ///< Max index of LVD interrupt voltage
#elif (PDL_MCU_TYPE == PDL_FM0P_TYPE2) || (PDL_MCU_TYPE == PDL_FM0P_TYPE3)
    Lvd0IrqDetectVoltage150 = 0u,   ///< Interrupt when voltage is vicinity of 1.50 volts
    Lvd0IrqDetectVoltage155 = 1u,   ///< Interrupt when voltage is vicinity of 1.55 volts  
    Lvd0IrqDetectVoltage160 = 2u,   ///< Interrupt when voltage is vicinity of 1.60 volts
    Lvd0IrqDetectVoltage165 = 3u,   ///< Interrupt when voltage is vicinity of 1.65 volts
    Lvd0IrqDetectVoltage170 = 4u,   ///< Interrupt when voltage is vicinity of 1.70 volts
    Lvd0IrqDetectVoltage175 = 5u,   ///< Interrupt when voltage is vicinity of 1.75 volts  
    Lvd0IrqDetectVoltage180 = 6u,   ///< Interrupt when voltage is vicinity of 1.80 volts
    Lvd0IrqDetectVoltage185 = 7u,   ///< Interrupt when voltage is vicinity of 1.85 volts
    Lvd0IrqDetectVoltage190 = 8u,   ///< Interrupt when voltage is vicinity of 1.90 volts
    Lvd0IrqDetectVoltage195 = 9u,   ///< Interrupt when voltage is vicinity of 1.95 volts
    Lvd0IrqDetectVoltage200 = 10u,  ///< Interrupt when voltage is vicinity of 2.00 volts
    Lvd0IrqDetectVoltage205 = 11u,  ///< Interrupt when voltage is vicinity of 2.05 volts
    Lvd0IrqDetectVoltage250 = 12u,  ///< Interrupt when voltage is vicinity of 2.50 volts    
    Lvd0IrqDetectVoltage260 = 13u,  ///< Interrupt when voltage is vicinity of 2.60 volts
    Lvd0IrqDetectVoltage270 = 14u,  ///< Interrupt when voltage is vicinity of 2.70 volts
    Lvd0IrqDetectVoltage280 = 15u,  ///< Interrupt when voltage is vicinity of 2.80 volts
    Lvd0IrqDetectVoltage290 = 16u,  ///< Interrupt when voltage is vicinity of 2.90 volts
    Lvd0IrqDetectVoltage300 = 17u,  ///< Interrupt when voltage is vicinity of 3.00 volts
    Lvd0IrqDetectVoltage310 = 18u,  ///< Interrupt when voltage is vicinity of 3.10 volts
    Lvd0IrqDetectVoltage320 = 19u,  ///< Interrupt when voltage is vicinity of 3.20 volts
    Lvd0IrqDetectVoltageMax,        ///< Max index of LVD interrupt voltage
#endif      
} en_lvd0_irq_detect_voltage_t;

/**
 ******************************************************************************
 ** \brief LVD ch.0 Interruption release voltage settings for Low Voltage Detection
 ******************************************************************************/  
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)
typedef enum en_lvd0_irq_release_voltage
{
    Lvd0IrqReleaseVoltage155 = 0u,   ///< Interrupt when voltage is vicinity of 1.55 volts  
    Lvd0IrqReleaseVoltage160 = 1u,   ///< Interrupt when voltage is vicinity of 1.60 volts
    Lvd0IrqReleaseVoltage165 = 2u,   ///< Interrupt when voltage is vicinity of 1.65 volts
    Lvd0IrqReleaseVoltage170 = 3u,   ///< Interrupt when voltage is vicinity of 1.70 volts
    Lvd0IrqReleaseVoltage175 = 4u,   ///< Interrupt when voltage is vicinity of 1.75 volts  
    Lvd0IrqReleaseVoltage180 = 5u,   ///< Interrupt when voltage is vicinity of 1.80 volts
    Lvd0IrqReleaseVoltage185 = 6u,   ///< Interrupt when voltage is vicinity of 1.85 volts
    Lvd0IrqReleaseVoltage190 = 7u,   ///< Interrupt when voltage is vicinity of 1.90 volts
    Lvd0IrqReleaseVoltage195 = 8u,   ///< Interrupt when voltage is vicinity of 1.95 volts
    Lvd0IrqReleaseVoltage200 = 9u,   ///< Interrupt when voltage is vicinity of 2.00 volts
    Lvd0IrqReleaseVoltage205 = 10u,  ///< Interrupt when voltage is vicinity of 2.05 volts
    Lvd0IrqReleaseVoltage250 = 11u,  ///< Interrupt when voltage is vicinity of 2.50 volts    
    Lvd0IrqReleaseVoltage260 = 12u,  ///< Interrupt when voltage is vicinity of 2.60 volts
    Lvd0IrqReleaseVoltage270 = 13u,  ///< Interrupt when voltage is vicinity of 2.70 volts
    Lvd0IrqReleaseVoltage280 = 14u,  ///< Interrupt when voltage is vicinity of 2.80 volts
    Lvd0IrqReleaseVoltage290 = 15u,  ///< Interrupt when voltage is vicinity of 2.90 volts
    Lvd0IrqReleaseVoltage300 = 16u,  ///< Interrupt when voltage is vicinity of 3.00 volts
    Lvd0IrqReleaseVoltage310 = 17u,  ///< Interrupt when voltage is vicinity of 3.10 volts
    Lvd0IrqReleaseVoltage320 = 18u,  ///< Interrupt when voltage is vicinity of 3.20 volts
    Lvd0IrqReleaseVoltage330 = 19u,  ///< Interrupt when voltage is vicinity of 3.20 volts
    Lvd0IrqReleaseVoltageMax,        ///< Max index of LVD interrupt voltage
    
}en_lvd0_irq_release_voltage_t;
#endif


#if (PDL_MCU_TYPE == PDL_FM0P_TYPE1) ||  \
    (PDL_MCU_TYPE == PDL_FM3_TYPE3) || (PDL_MCU_TYPE == PDL_FM3_TYPE7) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE6) || (PDL_MCU_TYPE == PDL_FM3_TYPE8) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE9) || (PDL_MCU_TYPE == PDL_FM3_TYPE10) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE11) || (PDL_MCU_TYPE == PDL_FM3_TYPE12) 
/**
 ******************************************************************************
 ** \brief LVD ch.0 Reset voltage settings for Low Voltage Detection
 ******************************************************************************/
typedef enum en_lvd_reset_voltage
{
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE1) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE9)  || (PDL_MCU_TYPE == PDL_FM3_TYPE10) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE11) || (PDL_MCU_TYPE == PDL_FM3_TYPE12)
    LvdResetVoltage245 = 0u,   ///< Interrupt when voltage is vicinity of 2.45 volts
    LvdResetVoltage260 = 1u,   ///< Interrupt when voltage is vicinity of 2.60 volts
    LvdResetVoltage270 = 2u,   ///< Interrupt when voltage is vicinity of 2.70 volts
    LvdResetVoltage280 = 3u,   ///< Interrupt when voltage is vicinity of 2.80 volts
    LvdResetVoltage300 = 4u,   ///< Interrupt when voltage is vicinity of 3.00 volts
    LvdResetVoltage320 = 5u,   ///< Interrupt when voltage is vicinity of 3.20 volts
    LvdResetVoltage360 = 6u,   ///< Interrupt when voltage is vicinity of 3.60 volts
    LvdResetVoltage370 = 7u,   ///< Interrupt when voltage is vicinity of 3.70 volts
    LvdResetVoltage400 = 8u,   ///< Interrupt when voltage is vicinity of 4.00 volts
    LvdResetVoltage410 = 9u,   ///< Interrupt when voltage is vicinity of 4.10 volts
    LvdResetVoltage420 = 10u,  ///< Interrupt when voltage is vicinity of 4.20 volts
    LvdResetVoltageMax,        ///< Max index of LVD reset voltage
#elif (PDL_MCU_TYPE == PDL_FM3_TYPE3) || (PDL_MCU_TYPE == PDL_FM3_TYPE7)
    Lvd0ResetVoltage153 = 1u,   ///< Interrupt when voltage is vicinity of 1.53 volts
    Lvd0ResetVoltage193 = 4u,   ///< Interrupt when voltage is vicinity of 1.93 volts
    Lvd0ResetVoltageMax,        ///< Max index of LVD reset voltage
#elif (PDL_MCU_TYPE == PDL_FM3_TYPE6) || (PDL_MCU_TYPE == PDL_FM3_TYPE8)
    LvdResetVoltage150 = 0u,   ///< Interrupt when voltage is vicinity of 1.50 volts
    LvdResetVoltage155 = 1u,   ///< Interrupt when voltage is vicinity of 1.55 volts
    LvdResetVoltage160 = 2u,   ///< Interrupt when voltage is vicinity of 1.60 volts
    LvdResetVoltage165 = 3u,   ///< Interrupt when voltage is vicinity of 1.65 volts
    LvdResetVoltage170 = 4u,   ///< Interrupt when voltage is vicinity of 1.70 volts
    LvdResetVoltage175 = 5u,   ///< Interrupt when voltage is vicinity of 1.75 volts
    LvdResetVoltage180 = 6u,   ///< Interrupt when voltage is vicinity of 1.80 volts
    LvdResetVoltage185 = 7u,   ///< Interrupt when voltage is vicinity of 1.85 volts
    LvdResetVoltage190 = 8u,   ///< Interrupt when voltage is vicinity of 1.90 volts
    LvdResetVoltage195 = 9u,   ///< Interrupt when voltage is vicinity of 1.95 volts
    LvdResetVoltage200 = 10u,  ///< Interrupt when voltage is vicinity of 2.00 volts
    LvdResetVoltage205 = 11u,  ///< Interrupt when voltage is vicinity of 2.05 volts
    LvdResetVoltage250 = 12u,  ///< Interrupt when voltage is vicinity of 2.50 volts    
    LvdResetVoltage260 = 13u,  ///< Interrupt when voltage is vicinity of 2.60 volts    
    LvdResetVoltage270 = 14u,  ///< Interrupt when voltage is vicinity of 2.70 volts    
    LvdResetVoltage280 = 15u,  ///< Interrupt when voltage is vicinity of 2.80 volts    
    LvdResetVoltage290 = 16u,  ///< Interrupt when voltage is vicinity of 2.90 volts
    LvdResetVoltage300 = 17u,  ///< Interrupt when voltage is vicinity of 3.00 volts    
    LvdResetVoltage310 = 18u,  ///< Interrupt when voltage is vicinity of 3.10 volts 
    LvdResetVoltage320 = 19u,  ///< Interrupt when voltage is vicinity of 3.20 volts  
    LvdResetVoltageMax,        ///< Max index of LVD reset voltage
#endif  
    
}en_lvd_reset_voltage_t;
#endif

#if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)
/**
 ******************************************************************************
 ** \brief LVD ch.1 Interruption voltage settings for Low Voltage Detection
 ******************************************************************************/  
typedef enum en_lvd1_irq_detect_voltage
{
    Lvd1IrqDetectVoltage150 = 0u,   ///< Interrupt when voltage is vicinity of 1.50 volts
    Lvd1IrqDetectVoltage155 = 1u,   ///< Interrupt when voltage is vicinity of 1.55 volts  
    Lvd1IrqDetectVoltage160 = 2u,   ///< Interrupt when voltage is vicinity of 1.60 volts
    Lvd1IrqDetectVoltage165 = 3u,   ///< Interrupt when voltage is vicinity of 1.65 volts
    Lvd1IrqDetectVoltage170 = 4u,   ///< Interrupt when voltage is vicinity of 1.70 volts
    Lvd1IrqDetectVoltage175 = 5u,   ///< Interrupt when voltage is vicinity of 1.75 volts  
    Lvd1IrqDetectVoltage180 = 6u,   ///< Interrupt when voltage is vicinity of 1.80 volts
    Lvd1IrqDetectVoltage185 = 7u,   ///< Interrupt when voltage is vicinity of 1.85 volts
    Lvd1IrqDetectVoltage190 = 8u,   ///< Interrupt when voltage is vicinity of 1.90 volts
    Lvd1IrqDetectVoltage195 = 9u,   ///< Interrupt when voltage is vicinity of 1.95 volts
    Lvd1IrqDetectVoltage200 = 10u,  ///< Interrupt when voltage is vicinity of 2.00 volts
    Lvd1IrqDetectVoltage205 = 11u,  ///< Interrupt when voltage is vicinity of 2.05 volts
    Lvd1IrqDetectVoltage250 = 12u,  ///< Interrupt when voltage is vicinity of 2.50 volts    
    Lvd1IrqDetectVoltage260 = 13u,  ///< Interrupt when voltage is vicinity of 2.60 volts
    Lvd1IrqDetectVoltage270 = 14u,  ///< Interrupt when voltage is vicinity of 2.70 volts
    Lvd1IrqDetectVoltage280 = 15u,  ///< Interrupt when voltage is vicinity of 2.80 volts
    Lvd1IrqDetectVoltage290 = 16u,  ///< Interrupt when voltage is vicinity of 2.90 volts
    Lvd1IrqDetectVoltage300 = 17u,  ///< Interrupt when voltage is vicinity of 3.00 volts
    Lvd1IrqDetectVoltage310 = 18u,  ///< Interrupt when voltage is vicinity of 3.10 volts
    Lvd1IrqDetectVoltage320 = 19u,  ///< Interrupt when voltage is vicinity of 3.20 volts
    Lvd1IrqDetectVoltageMax,        ///< Max index of LVD interrupt voltage
 
} en_lvd1_irq_detect_voltage_t;
#endif    

/**
 ******************************************************************************
 ** \brief LVD ch.1 Interruption release voltage settings for Low Voltage Detection
 ******************************************************************************/  
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)
typedef enum en_lvd1_irq_release_voltage
{
    Lvd1IrqReleaseVoltage155 = 0u,   ///< Interrupt when voltage is vicinity of 1.55 volts  
    Lvd1IrqReleaseVoltage160 = 1u,   ///< Interrupt when voltage is vicinity of 1.60 volts
    Lvd1IrqReleaseVoltage165 = 2u,   ///< Interrupt when voltage is vicinity of 1.65 volts
    Lvd1IrqReleaseVoltage170 = 3u,   ///< Interrupt when voltage is vicinity of 1.70 volts
    Lvd1IrqReleaseVoltage175 = 4u,   ///< Interrupt when voltage is vicinity of 1.75 volts  
    Lvd1IrqReleaseVoltage180 = 5u,   ///< Interrupt when voltage is vicinity of 1.80 volts
    Lvd1IrqReleaseVoltage185 = 6u,   ///< Interrupt when voltage is vicinity of 1.85 volts
    Lvd1IrqReleaseVoltage190 = 7u,   ///< Interrupt when voltage is vicinity of 1.90 volts
    Lvd1IrqReleaseVoltage195 = 8u,   ///< Interrupt when voltage is vicinity of 1.95 volts
    Lvd1IrqReleaseVoltage200 = 9u,   ///< Interrupt when voltage is vicinity of 2.00 volts
    Lvd1IrqReleaseVoltage205 = 10u,  ///< Interrupt when voltage is vicinity of 2.05 volts
    Lvd1IrqReleaseVoltage250 = 11u,  ///< Interrupt when voltage is vicinity of 2.50 volts    
    Lvd1IrqReleaseVoltage260 = 12u,  ///< Interrupt when voltage is vicinity of 2.60 volts
    Lvd1IrqReleaseVoltage270 = 13u,  ///< Interrupt when voltage is vicinity of 2.70 volts
    Lvd1IrqReleaseVoltage280 = 14u,  ///< Interrupt when voltage is vicinity of 2.80 volts
    Lvd1IrqReleaseVoltage290 = 15u,  ///< Interrupt when voltage is vicinity of 2.90 volts
    Lvd1IrqReleaseVoltage300 = 16u,  ///< Interrupt when voltage is vicinity of 3.00 volts
    Lvd1IrqReleaseVoltage310 = 17u,  ///< Interrupt when voltage is vicinity of 3.10 volts
    Lvd1IrqReleaseVoltage320 = 18u,  ///< Interrupt when voltage is vicinity of 3.20 volts
    Lvd1IrqReleaseVoltage330 = 19u,  ///< Interrupt when voltage is vicinity of 3.20 volts
    Lvd1IrqReleaseVoltageMax,        ///< Max index of LVD interrupt voltage
    
}en_lvd1_irq_release_voltage_t;
#endif

/** \} GroupLVD_Types */

/**
* \addtogroup GroupLVD_DataStructures
* \{
*/

/**
 ******************************************************************************
 ** \brief Low Voltage Detection internal data
 ******************************************************************************/ 
typedef struct stc_lvd_intern_data
{
    func_ptr_t  pfnIrqCallback[2];    ///< LVD interrupt callback function
} stc_lvd_intern_data_t ;

/**
 ******************************************************************************
 ** \brief Clock Supervisor configuration
 ** 
 ** The Clock Supervisor configuration settings
 ******************************************************************************/
typedef struct stc_lvd_config
{
    en_lvd0_irq_detect_voltage_t   enLvd0IrqDetectVoltage;     ///< LVD ch.0 interrupt detect voltage, see #en_lvd0_irq_detect_voltage_t for details.
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)    
    en_lvd0_irq_release_voltage_t  enLvd0IrqReleaseVoltage;    ///< LVD ch.0 interrupt release voltage, see #en_lvd0_irq_release_voltage_t for details.
    boolean_t bLvd0ReleaseVoltageEnable;
#endif    
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE1) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE3) || (PDL_MCU_TYPE == PDL_FM3_TYPE7) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE6) || (PDL_MCU_TYPE == PDL_FM3_TYPE8) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE9) || (PDL_MCU_TYPE == PDL_FM3_TYPE10) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE11) || (PDL_MCU_TYPE == PDL_FM3_TYPE12)     
    en_lvd_reset_voltage_t enLvdResetVoltage;  ///< LVD reset voltage, see #en_lvd_reset_voltage_t for details.
#endif      
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE2)       
    en_lvd1_irq_detect_voltage_t  enLvd1IrqDetectVoltage;   ///< LVD ch.1 interrupt detect voltage, see #en_lvd1_irq_detect_voltage_t for details.
    en_lvd1_irq_release_voltage_t enLvd1IrqReleaseVoltage;  ///< LVD ch.1 interrupt release voltage, see #en_lvd1_irq_detect_voltage_t for details.
    boolean_t bLvd1ReleaseVoltageEnable;
#endif    
    func_ptr_t           pfnIrqCallback[2];   ///< LVD interrupt callback function
} stc_lvd_config_t;

/** \} GroupLVD_DataStructures */

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/

/**
* \addtogroup GroupLVD_Functions
* \{
*/

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/

/* LVD Interupt IRQ */
void Lvd_IrqHandler(void);

/* Init/De-Init */
en_result_t Lvd_Init(const stc_lvd_config_t* pstcConfig);
en_result_t Lvd_DeInit(void);

/* Reset */
#if (PDL_MCU_TYPE == PDL_FM0P_TYPE1) || (PDL_MCU_TYPE == PDL_FM0P_TYPE2) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE3)  || (PDL_MCU_TYPE == PDL_FM3_TYPE7) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE6)  || (PDL_MCU_TYPE == PDL_FM3_TYPE8) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE9)  || (PDL_MCU_TYPE == PDL_FM3_TYPE10) || \
    (PDL_MCU_TYPE == PDL_FM3_TYPE11) || (PDL_MCU_TYPE == PDL_FM3_TYPE12)   
en_result_t Lvd_EnableReset(void);
en_result_t Lvd_DisableReset(void);
boolean_t Lvd_GetResetOperationStatus(void);
#endif

/* Interrupt */
en_result_t Lvd_EnableIrqDetect(uint8_t u8Channel);
en_result_t Lvd_DisableIrqDetect(uint8_t u8Channel);
boolean_t Lvd_GetIrqStatus(uint8_t u8Channel);
en_result_t Lvd_ClrIrqStatus(uint8_t u8Channel);
boolean_t Lvd_GetIrqOperationStatus(uint8_t u8Channel);

/** \} GroupLVD_Functions */
/** \} GroupLVD */

#ifdef __cplusplus
}
#endif

#endif /* #if (defined(PDL_PERIPHERAL_LVD_ACTIVE)) */

#endif /* __LVD_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
