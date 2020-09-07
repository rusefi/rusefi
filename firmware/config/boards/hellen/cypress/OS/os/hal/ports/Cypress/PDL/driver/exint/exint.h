/*******************************************************************************
* \file             exint.h
* 
* \version          1.30
*                       
* \brief            Headerfile for External Interrupts functions
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

#ifndef __EXINT_H__
#define __EXINT_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "pdl_user.h"
#include "mcu.h"

#if (defined(PDL_PERIPHERAL_EXINT_ACTIVE)) || \
    (defined(PDL_PERIPHERAL_NMI_ACTIVE))


/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \defgroup GroupEXINT External Interrupts (EXINT)
* \{
* \defgroup GroupEXINT_Macros Macros
* \defgroup GroupEXINT_Functions Functions 
* \defgroup GroupEXINT_GlobalVariables Global Variables
* \defgroup GroupEXINT_DataStructures Data Structures
* \defgroup GroupEXINT_Types Enumerated Types
* \}
*/
/**
* \addtogroup GroupEXINT
* \{
* The External Interrupt (EXINT) peripheral enables you to set up a response to an external 
* interrupt. There are 32 possible external interrupts, corresponding to physical pins designated 
* INT00-INT031. Check the schematic for your hardware to see which interrupt pins are available.<br>
* For each of the 32 possible interrupts you can:<br>
* - Enable or disable the interrupt<br>
* - Set the detection condition (level) that triggers the interrupt<br>
* - Provide the callback routine to handle the interrupt<br>
* You set the level from these choices:
* - High level<br>
* - Low level<br>
* - Rising edge<br>
* - Falling edge<br>
* - Both rising edge and falling edges (Certain processors only).<br>
* The EXINT peripheral also manages non-maskable interrupts (NMI). An 
* NMI interrupt request is sent to the CPU if the edge or level is detected 
* from the signal input to the NMI input pin. The edge or level varies depending 
* on the state of the system, and is not configurable:<br>
* - Run mode: Falling edge<br>
* - Sleep mode: Falling edge<br>
* - Timer mode: Low level<br>
* - RTC mode: Low level<br>
* - Stop mode: Low level<br>
* - Deep standby mode: NMI request is not available in this mode<br>
* 
* \section SectionEXINT_ConfigurationConsideration Configuration Consideration
* <b>External Interrupts</b><br>
* You set fields in the stc_exint_config_t structure to enable an external interrupt, 
* specify the detect level, and specify the callback routine. To do so you set individual 
* bits and bitfields. A sample code snippet that configures the response to pin INT00 might look like this:<br>
* myConfig.abEnable[ExintInstanceIndexExint0] = 1u;  // enabled<br>
* myConfig. aenLevel[ExintInstanceIndexExint0] = ExIntFallingEdge;<br>
* myConfig.apfnExintCallback[ExintInstanceIndexExint0] = MyCallback;<br>
* You configure the response to any or all 32 interrupts in this one configuration structure.<br>
* Call Exint_Init() to initialize and enable all interrupts you have configured.<br>
* After initializing, you can control single channels independently. Use Exint_DisableChannel()  
* and  Exint_EnableChannel(). You can change the detect mode with Exint_SetDetectMode(). <br>
* Call Exint_DeInit() to disable all external interrupts.<br>
* <b>NMI</b><br>
* Specify the interrupt handler routine in the stc_exint_nmi_config_t structure. Then call Exint_Nmi_Init().
* This initializes and enables the NMI. Call Exint_NMI_DeInit() to disable NMI handling.<br>
* 
* \section SectionEXINT_MoreInfo More Information
* For more information on the EXINT peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/223051/download">FM0+ Peripheral Manual Core Subsystem TRM.pdf</a><br>
* <a href="http://www.cypress.com/file/222996/download">FM4 Peripheral Manual Core Subsystem TRM.pdf</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/**
* \addtogroup GroupEXINT_Macros
* \{
*/  
/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/
#define PDL_EXINT_CHANNELS       (32u)  
#define PDL_EXINT_INSTANCE_COUNT (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT0  == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT1  == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT2  == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT3  == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT4  == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT5  == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT6  == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT7  == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT8  == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT9  == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT10 == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT11 == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT12 == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT13 == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT14 == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT15 == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT16 == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT17 == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT18 == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT19 == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT20 == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT21 == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT22 == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT23 == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT24 == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT25 == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT26 == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT27 == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT28 == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT29 == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT30 == PDL_ON) + \
                                 (uint8_t)(PDL_INTERRUPT_ENABLE_EXINT31 == PDL_ON) 

/** \} GroupEXINT_Macros */

/**
* \addtogroup GroupEXINT_Types
* \{
*/
/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/
  
typedef enum en_exint_level
{
  ExIntLowLevel     = 0u,  ///< "L" level detection
  ExIntHighLevel    = 1u,  ///< "H" level detection
  ExIntRisingEdge   = 2u,  ///< Rising edge detection
  ExIntFallingEdge  = 3u,  ///< Falling edge detection
#if (PDL_MCU_TYPE == PDL_FM4_TYPE5) ||  (PDL_MCU_TYPE == PDL_FM4_TYPE6) || (PDL_MCU_TYPE == PDL_FM0P_TYPE3)
  ExIntBothEdge     = 4u,  ///< Both edge detection  
#endif    
} en_exint_level_t;

/// Enumeration to define an index for each enabled external interrupt instance
typedef enum en_exint_instance_index
{
  #if (PDL_PERIPHERAL_ENABLE_EXINT0 == PDL_ON)
    ExintInstanceIndexExint0,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT1 == PDL_ON)
    ExintInstanceIndexExint1,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT2 == PDL_ON)
    ExintInstanceIndexExint2,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT3 == PDL_ON)
    ExintInstanceIndexExint3,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT4 == PDL_ON)
    ExintInstanceIndexExint4,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT5 == PDL_ON)
    ExintInstanceIndexExint5,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT6 == PDL_ON)
    ExintInstanceIndexExint6,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT7 == PDL_ON)
    ExintInstanceIndexExint7,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT8 == PDL_ON)
    ExintInstanceIndexExint8,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT9 == PDL_ON)
    ExintInstanceIndexExint9,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT10 == PDL_ON)
    ExintInstanceIndexExint10,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT11 == PDL_ON)
    ExintInstanceIndexExint11,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT12 == PDL_ON)
    ExintInstanceIndexExint12,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT13 == PDL_ON)
    ExintInstanceIndexExint13,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT14 == PDL_ON)
    ExintInstanceIndexExint14,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT15 == PDL_ON)
    ExintInstanceIndexExint15,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT16 == PDL_ON)
    ExintInstanceIndexExint16,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT17 == PDL_ON)
    ExintInstanceIndexExint17,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT18 == PDL_ON)
    ExintInstanceIndexExint18,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT19 == PDL_ON)
    ExintInstanceIndexExint19,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT20 == PDL_ON)
    ExintInstanceIndexExint20,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT21 == PDL_ON)
    ExintInstanceIndexExint21,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT22 == PDL_ON)
    ExintInstanceIndexExint22,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT23 == PDL_ON)
    ExintInstanceIndexExint23,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT24 == PDL_ON)
    ExintInstanceIndexExint24,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT25 == PDL_ON)
    ExintInstanceIndexExint25,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT26 == PDL_ON)
    ExintInstanceIndexExint26,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT27 == PDL_ON)
    ExintInstanceIndexExint27,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT28 == PDL_ON)
    ExintInstanceIndexExint28,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT29 == PDL_ON)
    ExintInstanceIndexExint29,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT30 == PDL_ON)
    ExintInstanceIndexExint30,
  #endif
  #if (PDL_PERIPHERAL_ENABLE_EXINT31 == PDL_ON)
    ExintInstanceIndexExint31,
  #endif      
    ExintInstanceIndexMax
} en_exint_instance_index_t;

/** \}GroupEXINT_Types */

/**
* \addtogroup GroupEXINT_DataStructures
* \{
*/
/**
 ******************************************************************************
 ** \brief EXINT configuration
 ******************************************************************************/

#if (defined(PDL_PERIPHERAL_EXINT_ACTIVE))
/**
 *****************************************************************************
 ** \brief External Interrupt configuration
 ** 
 ** The EXINT configuration
 *****************************************************************************/
typedef struct stc_exint_config
{
  boolean_t         abEnable[PDL_EXINT_INSTANCE_COUNT];           ///< TRUE: External Interrupt enable
  en_exint_level_t  aenLevel[PDL_EXINT_INSTANCE_COUNT];           ///< level detection, see #en_exint_level_t for details
  /* [andreika]: func_ptr_t -> func_ptr_arg1_t */
  func_ptr_arg1_t        apfnExintCallback[PDL_EXINT_INSTANCE_COUNT];  ///< Callback pointers  
  boolean_t         bTouchNvic;      ///< TRUE: Enable NVIC, FALSE: don't enable NVIC, 
  
} stc_exint_config_t;

/**
 ******************************************************************************
 ** \brief Datatype for holding internal data needed for EXINT
 ******************************************************************************/
typedef struct stc_exint_intern_data
{
  /* [andreika]: func_ptr_t -> func_ptr_arg1_t */
  func_ptr_arg1_t    apfnExintCallback[PDL_EXINT_INSTANCE_COUNT];  ///< Callback pointer array
} stc_exint_intern_data_t ;
#endif

#if (defined(PDL_PERIPHERAL_NMI_ACTIVE))
/**
 *****************************************************************************
 ** \brief NMI configuration
 ** 
 ** The NMI configuration
 *****************************************************************************/
typedef struct stc_exint_nmi_config
{ 
  func_ptr_t        pfnNmiCallback;   ///< NMI Callback pointers
} stc_exint_nmi_config_t;

/**
 ******************************************************************************
 ** \brief Datatype for holding internal data needed for NMI
 ******************************************************************************/
typedef struct stc_exint_nmi_intern_data
{
  func_ptr_t    pfnNmiCallback;  ///< Callback pointer
} stc_exint_nmi_intern_data_t ;
#endif

/** \} GroupEXINT_DataStructures */

/**
* \addtogroup GroupEXINT_GlobalVariables
* \{
*/ 

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/
#if (defined(PDL_PERIPHERAL_EXINT_ACTIVE))
extern stc_exint_intern_data_t stcExintInternData;
#endif
#if (defined(PDL_PERIPHERAL_NMI_ACTIVE))
extern stc_exint_nmi_intern_data_t stcExintNMIInternData;
#endif

/** \} GroupEXINT_GlobalVariables */

/**
* \addtogroup GroupEXINT_Functions
* \{
*/ 
/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/
#if (defined(PDL_PERIPHERAL_EXINT_ACTIVE))
void Exint_IrqHandler(uint8_t u8Channel) ;

en_result_t Exint_Init(const stc_exint_config_t* pstcConfig) ;

en_result_t Exint_DeInit(void) ;

en_result_t Exint_EnableChannel(uint8_t u8Channel) ;

en_result_t Exint_DisableChannel(uint8_t u8Channel) ;

en_result_t Exint_SetDetectMode(uint8_t u8Channel, en_exint_level_t enLevel);

en_exint_level_t Exint_GetDetectMode(uint8_t u8Channel);
#endif
#if (defined(PDL_PERIPHERAL_NMI_ACTIVE))
void Exint_Nmi_IrqHandler(void) ;

en_result_t Exint_Nmi_Init(stc_exint_nmi_config_t* pstcConfig) ;

en_result_t Exint_Nmi_DeInit(stc_exint_nmi_config_t* pstcConfig) ;
#endif 

/** \} GroupEXINT_Functions */
/** \} GroupEXINT */
#ifdef __cplusplus
}
#endif

#endif // #if (defined(PDL_PERIPHERAL_EXINT_ACTIVE))

#endif /* __EXINT_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
