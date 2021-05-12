/*******************************************************************************
* \file             pdl.h
* 
* \version          1.0
*                       
* \brief            Common headerfile for Peripheral Driver Library.
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

#ifndef __PDL_H__
#define __PDL_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "base_types.h"

/* C binding of definitions if building with C++ compiler                     */
#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/* Macro for initializing local structures to zero                            */
/******************************************************************************/
#define PDL_ZERO_STRUCT(x) pdl_memclr((uint8_t*)&(x), (uint32_t)(sizeof(x)))

/**
 ******************************************************************************
 ** All definitions needed for pdl_user.h are stated here
 ******************************************************************************/
#define PDL_ON            1u    ///< Switches a feature on
#define PDL_OFF           0u    ///< Switches a feature off

/**
 ******************************************************************************
 ** Global Device type definitions
 ** Note that an existing definition does not implicitly means full device
 ** type support of this library!
 ******************************************************************************/
// FM0+ device type  
#define PDL_FM0P_TYPE0   0100u  ///< FM0+ device type0
#define PDL_FM0P_TYPE1   0101u  ///< FM0+ device type1
#define PDL_FM0P_TYPE2   0102u  ///< FM0+ device type2
#define PDL_FM0P_TYPE3   0103u  ///< FM0+ device type3
#define PDL_FM0P_TYPE4   0104u  ///< FM0+ device type4
#define PDL_FM0P_TYPE5   0105u  ///< FM0+ device type5
#define PDL_FM0P_TYPE6   0106u  ///< FM0+ device type6
#define PDL_FM0P_TYPE7   0107u  ///< FM0+ device type7
#define PDL_FM0P_TYPE8   0108u  ///< FM0+ device type8
#define PDL_FM0P_TYPE9   0109u  ///< FM0+ device type9
// FM3 device type  
#define PDL_FM3_TYPE0    3000u  ///< FM3 device type0
#define PDL_FM3_TYPE1    3001u  ///< FM3 device type1
#define PDL_FM3_TYPE2    3002u  ///< FM3 device type2
#define PDL_FM3_TYPE3    3003u  ///< FM3 device type3
#define PDL_FM3_TYPE4    3004u  ///< FM3 device type4
#define PDL_FM3_TYPE5    3005u  ///< FM3 device type5
#define PDL_FM3_TYPE6    3006u  ///< FM3 device type6
#define PDL_FM3_TYPE7    3007u  ///< FM3 device type7
#define PDL_FM3_TYPE8    3008u  ///< FM3 device type8
#define PDL_FM3_TYPE9    3009u  ///< FM3 device type9
#define PDL_FM3_TYPE10   3010u  ///< FM3 device type10
#define PDL_FM3_TYPE11   3011u  ///< FM3 device type11
#define PDL_FM3_TYPE12   3012u  ///< FM3 device type12
// FM4 device type  
#define PDL_FM4_TYPE0    4000u  ///< FM4 device type0
#define PDL_FM4_TYPE1    4001u  ///< FM4 device type1
#define PDL_FM4_TYPE2    4002u  ///< FM4 device type2
#define PDL_FM4_TYPE3    4003u  ///< FM4 device type3
#define PDL_FM4_TYPE4    4004u  ///< FM4 device type4
#define PDL_FM4_TYPE5    4005u  ///< FM4 device type5
#define PDL_FM4_TYPE6    4006u  ///< FM4 device type6
#define PDL_FM4_TYPE7    4007u  ///< FM4 device type7
#define PDL_FM4_TYPE8    4008u  ///< FM4 device type8
#define PDL_FM4_TYPE9    4009u  ///< FM4 device type9
  
#define PDL_MCU_TYPE     FM_DEVICE_TYPE    // Definition from peripheral header

/**
 ******************************************************************************
 ** Global Device Core list
 ******************************************************************************/
#define PDL_FM0P_CORE            10u
#define PDL_FM3_CORE             20u
#define PDL_FM4_CORE             30u  
  
#ifdef FM_CORE_TYPE_FM0P                // Definition from peripheral header
#define PDL_MCU_CORE     PDL_FM0P_CORE 
#endif
  
#ifdef FM_CORE_TYPE_FM3                 // Definition from peripheral header
#define PDL_MCU_CORE     PDL_FM3_CORE 
#endif
  
#ifdef FM_CORE_TYPE_FM4                 // Definition from peripheral header
#define PDL_MCU_CORE     PDL_FM4_CORE 
#endif
  
/**
 ******************************************************************************
 ** Global Device interrupt list
 ******************************************************************************/  
    
#if FM_GENERAL_MCUHEADER_VERSION == 0200
    // FM0+ interrupt type
    #define PDL_FM0P_INT_TYPE_1_A                0100u
    #define PDL_FM0P_INT_TYPE_1_B                0101u
    #define PDL_FM0P_INT_TYPE_2_A                0102u
    #define PDL_FM0P_INT_TYPE_2_B                0103u   
    #define PDL_FM0P_INT_TYPE_3                  0104u   
      
    // FM3 interrupt type
    #define PDL_FM3_INT_TYPE_A                   3000u
    #define PDL_FM3_INT_TYPE_B                   3001u
    #define PDL_FM3_INT_TYPE_C                   3002u
       
    // FM4 interrupt type
    #define PDL_FM4_INT_TYPE_A                   4000u
    #define PDL_FM4_INT_TYPE_B                   4001u
    #define PDL_FM4_INT_TYPE_C                   4002u   
#elif FM_GENERAL_MCUHEADER_VERSION >= 0201 
    // FM0+ interrupt type
    #define PDL_FM0P_INT_TYPE_1_A                0x011Au
    #define PDL_FM0P_INT_TYPE_1_B                0x011Bu
    #define PDL_FM0P_INT_TYPE_2_A                0x012Au
    #define PDL_FM0P_INT_TYPE_2_B                0x012Bu   
    #define PDL_FM0P_INT_TYPE_3                  0x0130u   
      
    // FM3 interrupt type
    #define PDL_FM3_INT_TYPE_A                   0x300Au
    #define PDL_FM3_INT_TYPE_B                   0x300Bu
    #define PDL_FM3_INT_TYPE_C                   0x300Cu
       
    // FM4 interrupt type
    #define PDL_FM4_INT_TYPE_A                   0x400Au
    #define PDL_FM4_INT_TYPE_B                   0x400Bu
    #define PDL_FM4_INT_TYPE_C                   0x400Cu  
#else
    #error unknown FM_GENERAL_MCUHEADER_VERSION configuration
#endif
    
#define PDL_MCU_INT_TYPE    FM_INTERRUPT_TYPE // Definition from peripheral header

#if (PDL_MCU_CORE == PDL_FM0P_CORE)

  #if (PDL_MCU_INT_TYPE == PDL_FM0P_INT_TYPE_1_A)
    #define   __INTERRUPTS_FM0P_TYPE_1_A_C__

  #elif (PDL_MCU_INT_TYPE == PDL_FM0P_INT_TYPE_1_B)
    #define   __INTERRUPTS_FM0P_TYPE_1_B_C__

  #elif (PDL_MCU_INT_TYPE == PDL_FM0P_INT_TYPE_2_A)
    #define   __INTERRUPTS_FM0P_TYPE_2_A_C__

  #elif (PDL_MCU_INT_TYPE == PDL_FM0P_INT_TYPE_2_B)
    #define   __INTERRUPTS_FM0P_TYPE_2_B_C__

  #elif (PDL_MCU_INT_TYPE == PDL_FM0P_INT_TYPE_3)
    #define   __INTERRUPTS_FM0P_TYPE_3_C__

  #else
    #error Interrupt Type for FM0P not defined!
  #endif

#elif (PDL_MCU_CORE == PDL_FM3_CORE)

  #if (PDL_MCU_INT_TYPE == PDL_FM3_INT_TYPE_A)
    #define   __INTERRUPTS_FM3_TYPE_A_C__

  #elif (PDL_MCU_INT_TYPE == PDL_FM3_INT_TYPE_B)
    #define   __INTERRUPTS_FM3_TYPE_B_C__

  #elif (PDL_MCU_INT_TYPE == PDL_FM3_INT_TYPE_C)
    #define   __INTERRUPTS_FM3_TYPE_C_C__

  #else
    #error Interrupt Type for FM3 not defined!
  #endif

#elif (PDL_MCU_CORE == PDL_FM4_CORE)

  #if (PDL_MCU_INT_TYPE == PDL_FM4_INT_TYPE_A)
    #define   __INTERRUPTS_FM4_TYPE_A_C__

  #elif (PDL_MCU_INT_TYPE == PDL_FM4_INT_TYPE_B)
    #define   __INTERRUPTS_FM4_TYPE_B_C__

  #elif (PDL_MCU_INT_TYPE == PDL_FM4_INT_TYPE_C)
    #define   __INTERRUPTS_FM4_TYPE_C_C__

  #else
    #error Interrupt Type for FM4 not defined!
  #endif

#else
    #error FM core not defined!
#endif

/******************************************************************************/
/* User Device Setting Include file                                           */
/******************************************************************************/
//#include "pdl_device.h"  // MUST be included here!

/**
 ******************************************************************************
 ** Default Interrupt Level (lowest priority, used for De-Init functions)
 ******************************************************************************/
#if (PDL_MCU_CORE == PDL_FM3_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)       
#define PDL_DEFAULT_INTERRUPT_LEVEL 0x0Fu
#else
#define PDL_DEFAULT_INTERRUPT_LEVEL 0x03u        
#endif  

/******************************************************************************/
/* Global type definitions ('typedef')                                        */
/******************************************************************************/
/**
 ******************************************************************************
 ** \brief Level
 **
 ** Specifies levels.
 **
 ******************************************************************************/
typedef enum en_level
{
    PdlLow      = 0u,  ///< Low level '0'
    PdlHigh     = 1u   ///< High level '1'
} en_level_t;

/**
 ******************************************************************************
 ** \brief Generic Flag Code
 **
 ** Specifies flags.
 **
 ******************************************************************************/
typedef enum en_flag
{
    PdlClr = 0u,       ///< Flag clr '0'
    PdlSet = 1u        ///< Flag set '1'
} en_stat_flag_t, en_irq_flag_t;

/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/

/******************************************************************************
 * Global function prototypes
 ******************************************************************************/
extern void pdl_memclr(uint8_t* pu32Address, uint32_t u32Count);

/**
 ******************************************************************************
 ** This hook is part of wait loops.
 ******************************************************************************/
extern void PDL_WAIT_LOOP_HOOK(void);

#ifdef __cplusplus
}
#endif

#endif /* __PDL_H__ */

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
