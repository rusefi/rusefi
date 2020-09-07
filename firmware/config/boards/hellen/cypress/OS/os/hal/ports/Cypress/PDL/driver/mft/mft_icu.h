/*******************************************************************************
* \file              mft_icu.h
* 
* \version           1.20
*
* \brief             Headerfile for ICU functions.
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

#ifndef __MFT_ICU_H__
#define __MFT_ICU_H__

#include "mcu.h"
#include "pdl_user.h"   
   
#if (defined(PDL_PERIPHERAL_MFT_ICU_ACTIVE))

/**
* \defgroup GroupMFT_ICU Input Capture Unit (MFT ICU)
* \{
* \defgroup GroupMFT_ICU_Macros Macros
* \defgroup GroupMFT_ICU_Functions Functions
* \defgroup GroupMFT_ICU_GlobalVariables Global Variables
* \defgroup GroupMFT_ICU_DataStructures Data Structures
* \defgroup GroupMFT_ICU_Types Enumerated Types 
* \}
*/

/**
* \addtogroup GroupMFT_ICU
* \{
* The Input Capture Unit (ICU) is part of the Multi-function Timer (MFT). 
* The MFT is a collection of peripherals used primarily for motor control. 
* There may be up to three MFTs supported on a particular FM microcontroller. 
* One MFT can control a 3-phase motor. A microcontroller that supports multiple 
* MFTs can control multiple 3-phase motors.<br>
* The peripherals in the MFT are:<br>
* - Free-run Timer (FRT)
* - Output Compare Unit (OCU)
* - Waveform Generator (WFG)
* - Input Capture Unit (ICU)
* - ADC Start Compare (ADCMP)<br>
* Each MFT has an ICU. An ICU is a function block that captures an FRT count value 
* and generates an interrupt when a valid edge is detected at an external input pin signal. 
* This enables external control of the MFT.<br>
* An ICU has four input channels (0-3), which correspond to signal pins. For each channel you can:<br>
* - Select which FRT to use as a timer
* - Enable or disable any channel (and provide the interrupt handler for that channel)
* - Specify what edge is valid (rising, falling, or both)
* - Capture the value of the counter at the time of the interrupt<br>
* There are five possible sources for the FRT used. Three inputs are from the three FRT instances of the corresponding 
* MFT module. The other two FRT inputs are routed from the other possible MFT instances.<br>
* \section SectionMFT_ICU_ConfigurationConsideration Configuration Consideration
* To set up an ICU, first you should configure an FRT. See the description in the MFT FRT.<br>
* The ICU does not have a configuration structure. You make API function calls to specify the ICU behavior. 
* Make the calls in this order for each of the four channels you use.<br>
* * - Mft_Icu_SelFrt() to connect an FRT to the ICU.
* - Mft_Icu_EnableIrq() to enable the interrupt. This function also sets the callback function for the channel.
* - Mft_Icu_ConfigDetectMode() to specify the valid edge and start ICU operation at the same time. 
* The interrupt occurs when a valid edge is detected. The interrupt flag will be cleared and control passes 
* to the interrupt callback function. In the callback function, you can get the FRT counter value with 
* Mft_Icu_GetCaptureData().<br>
* If you handle interrupts by polling, you can get and clear the interrupt flag for each channel.<br>
* If both rising and falling edges are valid, you can call Mft_Icu_GetLastEdge() to get the edge information.<br>
*  When stopping the ICU, use Mft_Icu_ConfigDetectMode() to disable the ICU and Mft_Icu_DisableIrq() to disable ICU interrupt.
*
* \section SectionMFT_ICU_MoreInfo More Information
* For more information on the MFT FRT peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/223041/download">FM0+ Peripheral Manual - Timer Subsystem TRM.pdf</a><br>
* <a href="http://www.cypress.com/file/222986/download">FM4 Peripheral Manual - Timer Subsystem TRM.pdf</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/**
* \addtogroup GroupMFT_ICU_Macros
* \{
*/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/
#define stc_mftn_icu_t   FM_MFT_ICU_TypeDef
#define MFT0_ICU  (*((volatile stc_mftn_icu_t *) FM_MFT0_ICU_BASE))
#define MFT1_ICU  (*((volatile stc_mftn_icu_t *) FM_MFT1_ICU_BASE))
#define MFT2_ICU  (*((volatile stc_mftn_icu_t *) FM_MFT2_ICU_BASE))   

#define  MFT_ICU_CH0        0u
#define  MFT_ICU_CH1        1u
#define  MFT_ICU_CH2        2u
#define  MFT_ICU_CH3        3u

#define  MFT_ICU_CHx_MAX    3u

#define MFT_ICU_INSTANCE_COUNT \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_MFT0_ICU == PDL_ON) + \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_MFT1_ICU == PDL_ON) + \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_MFT2_ICU == PDL_ON)

/** \} GroupMFT_ICU_Macros */

/**
* \addtogroup GroupMFT_ICU_Types
* \{
*/

/**
 ******************************************************************************
 ** \brief Enumeration to define an index for each ICU instance
 ******************************************************************************/   
typedef enum en_icu_instance_index
{
#if (PDL_PERIPHERAL_ENABLE_MFT0_ICU == PDL_ON)   
    IcuInstanceIndexIcu0,              ///< Instance index of ICU0
#endif
#if (PDL_PERIPHERAL_ENABLE_MFT1_ICU == PDL_ON)   
    IcuInstanceIndexIcu1,              ///< Instance index of ICU1
#endif
#if (PDL_PERIPHERAL_ENABLE_MFT2_ICU == PDL_ON)
    IcuInstanceIndexIcu2,              ///< Instance index of ICU2
#endif
} en_icu_instance_index_t;  
   
/**
 ******************************************************************************
 ** \brief Frt channel
 ** 
 ** To select FRT channel to connect to ICU
 ******************************************************************************/
typedef enum en_mft_icu_frt
{
    Frt0ToIcu    = 0u,                      ///< connect Frt channel 0 to Icu
    Frt1ToIcu    = 1u,                      ///< connect Frt channel 1 to Icu
    Frt2ToIcu    = 2u,                      ///< connect Frt channel 2 to Icu
    IcuFrtToExt0 = 3u,                      ///< connect extern Frt channel 0 to Icu
    IcuFrtToExt1 = 4u,                      ///< connect extern Frt channel 1 to Icu
  
}en_mft_icu_frt_t;

/**
 ******************************************************************************
 ** \brief Icu mode
 ** 
 ** To select Icu edge detection mode
 ******************************************************************************/
typedef enum en_mft_icu_mode
{
    IcuDisable       = 0u,                  ///< disable Icu edge detection
    IcuRisingDetect  = 1u,                  ///< detect Icu rising edge
    IcuFallingDetect = 2u,                  ///< detect Icu falling edge
    IcuBothDetect    = 3u,                  ///< detect Icu rising/falling edge

}en_mft_icu_mode_t;

/**
 ******************************************************************************
 ** \brief Icu edge
 ** 
 ** To select Icu edge
 ******************************************************************************/
typedef enum en_icu_edge
{
    IcuFallingEdge = 0u,                    ///< select Icu falling edge
    IcuRisingEdge  = 1u,                    ///< select Icu rising edge
    
}en_mft_icu_edge_t;

/** \} GroupMFT_ICU_Types */

/**
* \addtogroup GroupMFT_ICU_DataStructures
* \{
*/

/**
 ******************************************************************************
 ** \brief Icu instance internal data,
 **        storing internal information for each enabled Icu instance
 ******************************************************************************/
typedef struct stc_mft_icu_intern_data
{
    func_ptr_t  pfnIcu0IrqCb;               ///< Callback function pointer of ICU0 interrupt
    func_ptr_t  pfnIcu1IrqCb;               ///< Callback function pointer of ICU1 interrupt
    func_ptr_t  pfnIcu2IrqCb;               ///< Callback function pointer of ICU2 interrupt
    func_ptr_t  pfnIcu3IrqCb;               ///< Callback function pointer of ICU3 interrupt
    
}stc_mft_icu_intern_data_t;
/**
 ******************************************************************************
 ** \brief Mft_icu instance data type
 ******************************************************************************/
typedef struct stc_mft_icu_instance_data
{
    volatile stc_mftn_icu_t*  pstcInstance; ///< pointer to registers of an instance
    stc_mft_icu_intern_data_t stcInternData;///< module internal data of instance
    
}stc_mft_icu_instance_data_t;
/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/

/** \} GroupMFT_ICU_DataStructures */

/**
* \addtogroup GroupMFT_ICU_GlobalVariables
* \{
*/

/** Look-up table for all enabled MFT_ICU instances and their internal data */
extern stc_mft_icu_instance_data_t m_astcMftIcuInstanceDataLut[MFT_ICU_INSTANCE_COUNT];

/** \} GroupMFT_ICU_GlobalVariables */

/**
* \addtogroup GroupMFT_ICU_Functions
* \{
*/

/** C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
/** Clock */
en_result_t Mft_Icu_SelFrt(volatile stc_mftn_icu_t*pstcMft, uint8_t u8Ch, en_mft_icu_frt_t enFrt);
/** mode set */
en_result_t Mft_Icu_ConfigDetectMode(volatile stc_mftn_icu_t*pstcMft, uint8_t u8Ch, en_mft_icu_mode_t enMode);
/** Interrupt */
#if (PDL_INTERRUPT_ENABLE_MFT0_ICU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_ICU == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_ICU == PDL_ON)
en_result_t Mft_Icu_EnableIrq(volatile stc_mftn_icu_t*pstcMft, 
                              uint8_t u8Ch, 
                              func_ptr_t pfnIrqCb,
                              boolean_t bTouchNvic);
en_result_t Mft_Icu_DisableIrq(volatile stc_mftn_icu_t*pstcMft, 
                               uint8_t u8Ch,
                               boolean_t bTouchNvic);
#endif
en_irq_flag_t Mft_Icu_GetIrqFlag(volatile stc_mftn_icu_t*pstcMft, uint8_t u8Ch);
en_result_t Mft_Icu_ClrIrqFlag(volatile stc_mftn_icu_t*pstcMft, uint8_t u8Ch);
/** Status read/write */
en_mft_icu_edge_t Mft_Icu_GetLastEdge(volatile stc_mftn_icu_t*pstcMft, uint8_t u8Ch);
/** Count read */
uint16_t Mft_Icu_GetCaptureData(volatile stc_mftn_icu_t*pstcMft, uint8_t u8Ch);
/** IRQ handler */
void Mft_Icu_IrqHandler(volatile stc_mftn_icu_t*pstcMft, stc_mft_icu_intern_data_t* pstcMftIcuInternData) ;

#ifdef __cplusplus
}
#endif

/** \} GroupMFT_ICU_Functions */
/** \} GroupMFT_ICU */

#endif

#endif
