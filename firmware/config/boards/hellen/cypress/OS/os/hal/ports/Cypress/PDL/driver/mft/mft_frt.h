/*******************************************************************************
* \file              mft_frt.h
* 
* \version           1.20
*
* \brief             Headerfile for MFT_FRT functions.
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

#ifndef __MFT_FRT_H__
#define __MFT_FRT_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_MFT_FRT_ACTIVE))

/**
* \defgroup GroupMFT_FRT Free-run Timer (MFT FRT)
* \{ 
* \defgroup GroupMFT_FRT_Macros Macros
* \defgroup GroupMFT_FRT_Functions Functions
* \defgroup GroupMFT_FRT_GlobalVariables Global Variables 
* \defgroup GroupMFT_FRT_DataStructures Data Structures
* \defgroup GroupMFT_FRT_Types Enumerated Types
* \}
*/

/**
* \addtogroup GroupMFT_FRT
* \{
* The Multi-function Timer (MFT) is a collection of peripherals used primarily for 
* motor control. There may be up to three MFTs supported on a particular FM microcontroller. 
* One MFT can control a 3-phase motor. A microcontroller that supports multiple MFTs can 
* control multiple 3-phase motors.<br>
* The peripherals in the MFT are:<br>
* - Free-run Timer (FRT)<br>
* - Output Compare Unit (OCU)<br>
* - Waveform Generator (WFG)<br>
* - Input Capture Unit (ICU) <br>
* - ADC Start Compare (ADCMP) <br>
* The FRT is a timer function block that outputs the reference counter value for the 
* operation of each function block in the MFT.<br>
* All operations of the MFT are synchronized with this unit. There are three separate 
* FRT instances within the MFT. For example, any OCU instance (up to 6) can be connected 
* to any FRT instance (up to 3). This enables significant flexibility in timing and behavior.<br>
* The clock for the FRT is generated from the Peripheral bus clock (PCLK) or an external clock. 
* This clock drives the FRT’s 16-bit counter based on the clock frequency and a clock divider. 
* The counter counts from zero to a programmable maximum value (the count cycle value) up to 16-bits. <br>
* The counter operates in one of two modes: up-count, or up/down-count. In up-count mode the 
* counter runs from zero to the count cycle value, then returns to zero and counts up again.
* In up/down-count mode, the counter counts up to its maximum value, and then counts down to zero, 
* completing the cycle. <br>
* The FRT can generate interrupts when it reaches the top or bottom value. You can set a mask 
* value (0-15) that determines how often to generate either interrupt. For example, 
* generate an interrupt on every 4th bottom, and every 8th top. You can also configure the 
* FRT to trigger an ADC conversion when the counter reaches zero.  <br>
* 

* \section SectionMFT_FRT_ConfigurationConsideration Configuration Consideration
* To set up the FRT, you provide configuration parameters in the stc_mft_frt_config_t structure, 
* such as the operation mode, the clock divider, and whether to use an external clock. 
* You also provide the structures to define interrupt handlers. Then call Mft_Frt_Init().<br>
* You must also call Mft_Frt_SetCountCycle() to set the maximum value for the counter. 
* This is not part of the configuration structure, because you may modify this at runtime. <br>
* When done, call Mft_Frt_Start() to start the counter running.<br>
* When the counter is running, you can get and set the current count value.<br>
* You can enable or disable interrupts at runtime. In polling mode, use Mft_Frt_GetIrqFlag()
* to check whether the interrupt occurs, and clear the interrupt flag by Mft_Frt_ClrIrqFlag(). 
* You can also get or set the mask value for either the zero or peak value interrupts.<br>
* As noted, there are three FRT instances. You can use Mft_Frt_SetSimultaneousStart() to 
* start multiple FRTs at the same time.
* 
* \section SectionMFT_FRT_MoreInfo More Information
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
* \addtogroup GroupMFT_FRT_Macros
* \{
*/

/******************************************************************************
 * Global definitions
 ******************************************************************************/
#define stc_mftn_frt_t FM_MFT_FRT_TypeDef
#define MFT0_FRT       (*((volatile stc_mftn_frt_t *) FM_MFT0_FRT_BASE))
#define MFT1_FRT       (*((volatile stc_mftn_frt_t *) FM_MFT1_FRT_BASE))
#define MFT2_FRT       (*((volatile stc_mftn_frt_t *) FM_MFT2_FRT_BASE))

#define FRT_INSTANCE_COUNT (PDL_PERIPHERAL_ENABLE_MFT0_FRT == PDL_ON ? 1u : 0u) + \
                           (PDL_PERIPHERAL_ENABLE_MFT1_FRT == PDL_ON ? 1u : 0u) + \
                           (PDL_PERIPHERAL_ENABLE_MFT2_FRT == PDL_ON ? 1u : 0u)  

/**
 ******************************************************************************
 ** \brief MFT Frt channel definition
 ******************************************************************************/
#define  MFT_FRT_CH0             0u
#define  MFT_FRT_CH1             1u
#define  MFT_FRT_CH2             2u

#define  MFT_FRT_MAX_CH          3u
   
/** \} GroupMFT_FRT_Macros */

/**
* \addtogroup GroupMFT_FRT_Types
* \{
*/
 
/**
 ******************************************************************************
 ** \brief Enumeration to define an index for each FRT instance
 ******************************************************************************/
typedef enum en_frt_instance_index
{
#if (PDL_PERIPHERAL_ENABLE_MFT0_FRT == PDL_ON)  
    FrtInstanceIndexFrt0,      ///< Instance index of FRT0
#endif    
#if (PDL_PERIPHERAL_ENABLE_MFT1_FRT == PDL_ON) 
    FrtInstanceIndexFrt1,      ///< Instance index of FRT1
#endif
#if (PDL_PERIPHERAL_ENABLE_MFT2_FRT == PDL_ON) 
    FrtInstanceIndexFrt2,      ///< Instance index of FRT2
#endif
} en_frt_instance_index_t;

/**
 ******************************************************************************
 ** \brief Mft Clock Setting
 ******************************************************************************/
typedef enum en_mft_frt_clock
{
    FrtPclkDiv1   = 0u,    ///< FRT clock: PCLK
    FrtPclkDiv2   = 1u,    ///< FRT clock: PCLK/2
    FrtPclkDiv4   = 2u,    ///< FRT clock: PCLK/4
    FrtPclkDiv8   = 3u,    ///< FRT clock: PCLK/8
    FrtPclkDiv16  = 4u,    ///< FRT clock: PCLK/16
    FrtPclkDiv32  = 5u,    ///< FRT clock: PCLK/32
    FrtPclkDiv64  = 6u,    ///< FRT clock: PCLK/64
    FrtPclkDiv128 = 7u,    ///< FRT clock: PCLK/128
    FrtPclkDiv256 = 8u,    ///< FRT clock: PCLK/256
#if (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)      
    FrtPclkDiv512 =  9u,    ///< FRT clock: PCLK/512
    FrtPclkDiv1024 = 10u,  ///< FRT clock: PCLK/1024
#endif    
} en_mft_frt_clock_t;

/**
 ******************************************************************************
 ** \brief FRT count mode
 ******************************************************************************/
typedef enum en_mft_frt_mode
{
    FrtUpCount     = 0u,    ///< FRT up-count mode
    FrtUpDownCount = 1u     ///< FRT up-/down-count mode
} en_mft_frt_mode_t;

/**
 ******************************************************************************
 ** \brief Enumeration of FRT interrupt index
 ******************************************************************************/
typedef enum   en_mft_frt_int
{
    enFrtZeroMatchIrq = 0u,           ///<  zero match interrupt index
    enFrtPeakMatchIrq = 1u,           ///<  peak match interrupt index

}en_mft_frt_irq_t;

/**
 ******************************************************************************
 ** \brief Enumeration of FRT channels with offset mode
 ******************************************************************************/
typedef enum en_frt_offset_ch
{
    FrtOffsetCh1 = 0u, ///<  FRT ch.1 with offset mode
    FrtOffsetCh2 = 1u, ///<  FRT ch.2 with offset mode

}en_frt_offset_ch_t;

/** \} GroupMFT_FRT_Types */

/**
* \addtogroup GroupMFT_FRT_DataStructures
* \{
*/

/**
 ******************************************************************************
 ** \brief Enumeration of FRT interrupt selection
 ******************************************************************************/
typedef struct stc_frt_int_sel
{
    boolean_t bFrtZeroMatchIrq;       ///<  zero match interrupt selection
    boolean_t bFrtPeakMatchIrq;       ///<  peak match interrupt selection

}stc_frt_irq_sel_t, stc_frt_irq_en_t;
 
/**
 ******************************************************************************
 ** \brief FRT interrupt callback function
 ******************************************************************************/
typedef struct stc_frt_int_cb
{
    func_ptr_t  pfnFrtZeroIrqCb; ///< match zero interrupt callback function
    func_ptr_t  pfnFrtPeakIrqCb; ///< match peak interrupt callback function

}stc_frt_irq_cb_t;

/**
 ******************************************************************************
 ** \brief FRT configure
 ******************************************************************************/
typedef struct stc_mft_frt_config
{
    en_mft_frt_clock_t  enFrtClockDiv;  ///< FRT clock divide
    en_mft_frt_mode_t   enFrtMode;      ///< FRT count mode
    boolean_t           bEnBuffer;      ///< enable buffer
    boolean_t           bEnExtClock;    ///< enable external clock
#if (PDL_MCU_CORE == PDL_FM3_CORE)    
    boolean_t           bTriggerAdc0;    ///< TRUE: Output AD conversion start signal to ADC uint 0 upon FRT zreo match event
                                         ///< FALSE: Don't output AD conversion start signal to ADC uint 0 upon FRT zreo match event
    boolean_t           bTriggerAdc1;    ///< TRUE: Output AD conversion start signal to ADC uint 1 upon FRT zreo match event
                                         ///< FALSE: Don't output AD conversion start signal to ADC uint 1 upon FRT zreo match event
    boolean_t           bTriggerAdc2;    ///< TRUE: Output AD conversion start signal to ADC uint 2 upon FRT zreo match event
                                         ///< FALSE: Don't output AD conversion start signal to ADC uint 2 upon FRT zreo match event
#endif
    stc_frt_irq_en_t*   pstcIrqEn;       ///< Pointer to FRT interrupt enable structure
    stc_frt_irq_cb_t*   pstcIrqCb;       ///< Pointer to FRT interrupt callback functions structure
    boolean_t           bTouchNvic;      ///< TRUE: Enable NVIC, FALSE: don't enable NVIC
    
}stc_mft_frt_config_t;   
    
/**
 ******************************************************************************
 ** \brief FRT instance internal data, storing internal
 **        information for each enabled FRT instance.
 ******************************************************************************/
typedef struct stc_mft_frt_intern_data
{
    func_ptr_t  pfnFrt0PeakIrqCb;  ///< Callback function pointer of FRT0 peak detection interrupt
    func_ptr_t  pfnFrt0ZeroIrqCb;  ///< Callback function pointer of FRT0 zero detection interrupt
    func_ptr_t  pfnFrt1PeakIrqCb;  ///< Callback function pointer of FRT1 peak detection interrupt
    func_ptr_t  pfnFrt1ZeroIrqCb;  ///< Callback function pointer of FRT1 zero detection interrupt
    func_ptr_t  pfnFrt2PeakIrqCb;  ///< Callback function pointer of FRT2 peak detection interrupt
    func_ptr_t  pfnFrt2ZeroIrqCb;  ///< Callback function pointer of FRT2 zero detection interrupt    

}stc_mft_frt_intern_data_t;

/**
 ******************************************************************************
 ** \brief FRT instance data type
 ******************************************************************************/
typedef struct stc_mft_frt_instance_data
{
    volatile stc_mftn_frt_t*  pstcInstance;  ///< pointer to registers of an instance
    stc_mft_frt_intern_data_t stcInternData; ///< module internal data of instance
} stc_mft_frt_instance_data_t;

/** \} GroupMFT_FRT_DataStructures */

/**
* \addtogroup GroupMFT_FRT_GlobalVariables
* \{
*/

/******************************************************************************/
/* Global variable declarations ('extern', definition in C source)            */
/******************************************************************************/
/// Look-up table for all enabled FRT of MFT  instances and their internal data
extern stc_mft_frt_instance_data_t m_astcMftFrtInstanceDataLut[FRT_INSTANCE_COUNT];

/** \} GroupMFT_FRT_GlobalVariables */

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \addtogroup GroupMFT_FRT_Functions
* \{
*/

/******************************************************************************/
/* Global function prototypes ('extern', definition in C source)              */
/******************************************************************************/
/* Init */
en_result_t Mft_Frt_Init(volatile stc_mftn_frt_t* pstcMft,
                         uint8_t u8Ch, const stc_mft_frt_config_t*  pstcFrtConfig);
en_result_t Mft_Frt_DeInit(volatile stc_mftn_frt_t* pstcMft, uint8_t u8Ch, 
                           boolean_t bTouchNvic);
#if (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)    
/* Mask zero/peak write/read */
en_result_t Mft_Frt_SetMaskZeroTimes(volatile stc_mftn_frt_t* pstcMft, 
                                     uint8_t u8Ch, 
                                     uint8_t u8Times);
uint8_t Mft_Frt_GetCurMaskZeroTimes(volatile stc_mftn_frt_t* pstcMft, uint8_t u8Ch);
en_result_t Mft_Frt_SetMaskPeakTimes(volatile stc_mftn_frt_t* pstcMft, 
                                     uint8_t u8Ch, 
                                     uint8_t u8Times);
uint8_t Mft_Frt_GetCurMaskPeakTimes(volatile stc_mftn_frt_t* pstcMft, uint8_t u8Ch);
#endif

#if (PDL_MCU_CORE == PDL_FM4_CORE) 
#if (PDL_MCU_TYPE >= PDL_FM4_TYPE3)
en_result_t  Mft_Frt_SetOffsetMode(volatile stc_mftn_frt_t* pstcMft, 
                                   uint8_t u8Ch, 
                                   boolean_t bOffsetMode);
#endif
#endif
/* Func/Int enable/disable */
en_result_t Mft_Frt_Start(volatile stc_mftn_frt_t* pstcMft, uint8_t u8Ch);
en_result_t Mft_Frt_Stop(volatile stc_mftn_frt_t* pstcMft, uint8_t u8Ch);
#if (PDL_INTERRUPT_ENABLE_MFT0_FRT == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_FRT == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_FRT == PDL_ON)
en_result_t Mft_Frt_EnableIrq(volatile stc_mftn_frt_t* pstcMft,
                              uint8_t u8Ch,
                              stc_frt_irq_sel_t* pstcIrqSel);
en_result_t Mft_Frt_DisableIrq(volatile stc_mftn_frt_t* pstcMft,
                               uint8_t u8Ch,
                               stc_frt_irq_sel_t* pstcIrqSel);
#endif

/* Status get/clear */
en_irq_flag_t Mft_Frt_GetIrqFlag(volatile stc_mftn_frt_t* pstcMft,
                                 uint8_t u8Ch,
                                 en_mft_frt_irq_t enIrqType);
en_result_t Mft_Frt_ClrIrqFlag(volatile stc_mftn_frt_t* pstcMft,
                               uint8_t u8Ch,
                               en_mft_frt_irq_t enIrqType);

/* Count write/read */
en_result_t Mft_Frt_SetCountCycle(volatile stc_mftn_frt_t* pstcMft, uint8_t u8Ch, uint16_t u16Cycle);
en_result_t Mft_Frt_SetCountVal(volatile stc_mftn_frt_t* pstcMft, uint8_t u8Ch,uint16_t u16Count);
uint16_t Mft_Frt_GetCurCount(volatile stc_mftn_frt_t* pstcMft, uint8_t u8Ch);

#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)
/* Simultaneous Start */
en_result_t Mft_Frt_SetSimultaneousStart(uint16_t u16Stop, uint16_t u16Clr);
#endif

/* IRQ handler */
void Mft_Frt_IrqHandler(volatile stc_mftn_frt_t* pstcMft, stc_mft_frt_intern_data_t* pstcMftFrtInternData) ;

/** \} GroupMFT_FRT_Functions */
/** \} GroupMFT_FRT */

#ifdef __cplusplus
}
#endif

#endif // #if (defined(PDL_PERIPHERAL_MFT_FRT_ACTIVE))

#endif /* __MFT_H__ */
