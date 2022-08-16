/*******************************************************************************
* \file              mft_wfg.h
* 
* \version           1.20
*
* \brief             Headerfile for MFT_WFG functions.
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

#ifndef __MFT_WFG_H__
#define __MFT_WFG_H__

#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_MFT_WFG_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
  
/**
* \defgroup GroupMFT_WFG Waveform Generator (MFT WFG)
* \{
* \defgroup GroupMFT_WFG_Macros Macros
* \defgroup GroupMFT_WFG_Functions Functions
* \defgroup GroupMFT_WFG_GlobalVariables Global Variables
* \defgroup GroupMFT_WFG_DataStructures Data Structures
* \defgroup GroupMFT_WFG_Types Enumerated Types
* \}
*/

/**
* \addtogroup GroupMFT_WFG
* \{
* The Waveform Generator (WFG) is part of the Multi-function Timer (MFT). The MFT is a collection 
* of peripherals used primarily for motor control. There may be up to three MFTs supported on a
* particular FM microcontroller. One MFT can control a 3-phase motor. A microcontroller that 
* supports multiple MFTs can control multiple 3-phase motors. <br>
* The peripherals in the MFT are:<br>
* - Free-run Timer (FRT)
* - Output Compare Unit (OCU)
* - Waveform Generator (WFG)
* - Input Capture Unit (ICU)
* - ADC Start Compare (ADCMP)<br>
* The WFG generates signals on the RTO output pins. There are three WFGs in each MFT instance, 
* corresponding to the three OCUs. Each OCU (2 channels each) is connected directly to a WFG. 
* Each channel of the OCU generates an RTO signal (RTO0-RTO5) through the WFG.<br>
* Depending on the WFG mode, the WFG can<br>
* - Generate the waveform signals based on input from the OCU (Through mode)
* - Superimpose a signal from the Programmable Pulse Generator (PPG) onto the 
* signals from the OCU (RT-PPG mode)
* - Bypass the OCU and generate signals using only the PPG (Timer-PPG mode)<br>
* Superimposing a PPG signal requires a gate-signal. <br>
* The WFG can automatically insert dead time when switching the RTO signals. Dead time is 
* the amount of time inserted between turning off one output, and turning on the next, 
* to prevent shoot-through (where the power signal has a direct path to ground). 
* In dead time operating modes, you specify the amount of time to insert.<br>
* The WFG also supports a function block called the noise canceller (NZCL). 
* You can use the NZCL to implement emergency stop for a motor.<br>
* See the Peripheral Manual – Timer Subsystem for more information.
* \section SectionMFT_WFG_ConfigurationConsideration Configuration Consideration
* You must set up other peripherals first. Which peripherals depends upon the WFG’s mode of operation. 
* For information about how to set up and initialize another peripheral, see the documentation 
* for that peripheral.<br>
* For Through mode, initialize the FRT and OCU first. For how to configure FRT, see the description in the \link GroupMFT_FRT MFT FRT \endlink.
* For how to configure OCU, see the description in the \link GroupMFT_OCU MFT OCU \endlink. <br> 
* For operating modes that require a PPG, (e.g. RT-PPG, timer-PPG), initialize the PPG peripheral. 
* For how to configure PPG, see the description in the \link GroupPPG PPG\endlink. <br>
* In all modes, you then set the fields in an stc_wfg_config_t structure, such as the mode of 
* operation and the behavior of the RTO signals (inverted or not). Then call Mft_Wfg_Init() 
* with the configured structure.<br>
* If you are using an operating mode that inserts dead time, call Mft_Wfg_WriteTimerCountCycle() to 
* specify the amount of dead time to insert.<br>
* Finally start/enable all the peripherals required for your operating mode. <br>
* To set up the WFG NZCL (used for motor emergency shutdown) you provide configuration 
* parameters in the stc_wfg_nzcl_config_t structure. For example, you can use an analog 
* noise filter and/or a digital noise filter to filter the signal input from the DTTIX pin. 
* You also provide interrupt handling routines. Then call Mft_Wfg_Nzcl_Init().
* Use Mft_Wfg_Nzcl_SwTriggerDtif() to trigger the DTIF interrupt by software, regardless 
* of DTTIX pin input status. With polling mode, there are API function calls to get and clear 
* either the analog or digital interrupt flags.
*
* \section SectionMFT_WFG_MoreInfo More Information
* For more information on the MFT WFG peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/223041/download">FM0+ Peripheral Manual - Timer Subsystem TRM.pdf</a><br>
* <a href="http://www.cypress.com/file/222986/download">FM4 Peripheral Manual - Timer Subsystem TRM.pdf</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/**
* \addtogroup GroupMFT_WFG_Macros
* \{
*/

/******************************************************************************
 * Global definitions
 ******************************************************************************/
#define stc_mftn_wfg_t FM_MFT_WFG_TypeDef

#define MFT0_WFG       (*((volatile stc_mftn_wfg_t *) FM_MFT0_WFG_BASE))
#define MFT1_WFG       (*((volatile stc_mftn_wfg_t *) FM_MFT1_WFG_BASE))
#define MFT2_WFG       (*((volatile stc_mftn_wfg_t *) FM_MFT2_WFG_BASE))

#define  MFT_WFG_CH0             0u
#define  MFT_WFG_CH1             1u
#define  MFT_WFG_CH2             2u
#define  MFT_WFG_CH3             3u 
#define  MFT_WFG_CH4             4u
#define  MFT_WFG_CH5             5u

#define  MFT_WFG_CH10            0u
#define  MFT_WFG_CH32            1u
#define  MFT_WFG_CH54            2u
#define  MFT_WFG_MAXCH           3u
  
#define WFG_INSTANCE_COUNT       (PDL_PERIPHERAL_ENABLE_MFT0_WFG == PDL_ON ? 1u : 0u) + \
                                 (PDL_PERIPHERAL_ENABLE_MFT1_WFG == PDL_ON ? 1u : 0u) + \
                                 (PDL_PERIPHERAL_ENABLE_MFT2_WFG == PDL_ON ? 1u : 0u)  

/** \} GroupMFT_WFG_Macros */

/**
* \addtogroup GroupMFT_WFG_Types
* \{
*/

/**
 ******************************************************************************
 ** \brief Enumeration to define an index for each WFG instance
 ******************************************************************************/   
typedef enum en_wfg_instance_index
{
#if (PDL_PERIPHERAL_ENABLE_MFT0_WFG == PDL_ON)  
    WfgInstanceIndexWfg0 = 0u, ///< Instance index of WFG0
#endif
#if (PDL_PERIPHERAL_ENABLE_MFT1_WFG == PDL_ON)
    WfgInstanceIndexWfg1,      ///< Instance index of WFG1
#endif 
#if (PDL_PERIPHERAL_ENABLE_MFT2_WFG == PDL_ON)
    WfgInstanceIndexWfg2,      ///< Instance index of WFG2
#endif    

} en_wfg_instance_index_t;  
  
/******************************************************************************
 * \brief WFG operation mode
 ******************************************************************************/
typedef enum en_mft_wfg_mode
{
    WfgThroughMode                 = 0u,           ///< through mode
    WfgRtPpgMode                   = 1u,           ///< RT-PPG mode
    WfgTimerPpgMode                = 2u,           ///< Timer-PPG mode
    WfgRtDeadTimerMode             = 4u,           ///< RT-dead timer mode
    WfgRtDeadTimerFilterMode       = 5u,           ///< RT-dead timer filter mode
    WfgPpgDeadTimerFilterMode      = 6u,           ///< PPG-dead timer filter mode
    WfgPpgDeadTimerMode            = 7u,           ///< PPG-dead timer mode
    
}en_mft_wfg_mode_t;

/******************************************************************************
 * \brief Enumeration of GTEN bits setting
 ******************************************************************************/
typedef enum en_gten_bits
{
    GtenBits00B = 0u,         ///< GTEN == b'00 No CH_GATE signal output
    GtenBits01B = 1u,         ///< GTEN == b'01 Outputs channel 0
    GtenBits10B = 2u,         ///< GTEN == b'10 Outputs channel 1
    GtenBits11B = 3u,         ///< GTEN == b'11 Outputs the logic OR of signals
  
}en_gten_bits_t;

/******************************************************************************
 * \brief Enumeration of PSEL bits setting
 ******************************************************************************/
typedef enum en_psel_bits
{
    PselBits00B = 0u,    ///< PSEL == b'00 Ch.0 of PPG is connected to for WFG
    PselBits01B = 1u,    ///< PSEL == b'01 Ch.2 of PPG is connected to for WFG
    PselBits10B = 2u,    ///< PSEL == b'10 Ch.4 of PPG is connected to for WFG
    PselBits11B = 3u,    ///< PSEL == b'11 Prohobited value
  
}en_psel_bits_t;

/******************************************************************************
 * \brief Enumeration of PGEN bits setting
 ******************************************************************************/
typedef enum en_pgen_bits
{
    PgenBits00B  = 0u,       ///< PGEN == b'00 No CH_PPG signal input; RTO0=RT0, RTO1=RT1
    PgenBits01B  = 1u,       ///< PGEN == b'01 PPG: RTO0=RT0&PPG0, RTO1=RT1
    PgenBits10B  = 2u,       ///< PGEN == b'10 PPG: RTO0=RT0, RTO1=RT1&PPG0
    PgenBits11B  = 3u,       ///< PGEN == b'11 PPG: RTO0=RT0&PPG0, RTO1=RT1&PPG0
    
}en_pgen_bits_t;

/******************************************************************************
 * \brief Enumeration of DMOD bit setting
 ******************************************************************************/
typedef enum en_dmod_bits
{
    DmodBits00B = 0u,     ///< output RTO1 and RTO0 signals without changing the level
    DmodBits01B = 1u,     ///< output both RTO1 and RTO0 signals reversed
#if (PDL_MCU_CORE == PDL_FM4_CORE) ||  (PDL_MCU_CORE == PDL_FM0P_CORE)   
    DmodBits10B = 2u,     ///< Outputs the RTO(0) signal reversed, outputs the RTO(1) signal without changing the level.
    DmodBits11B = 3u,     ///< Outputs the RTO(0) signal without changing the level, Outputs the RTO(1 ) signal reversed.
#endif    
}en_dmod_bits_t;

/******************************************************************************
 * \brief Enumeration to set count clock prescaler
 ******************************************************************************/
typedef enum en_wfg_timer_clock
{
    WfgPlckDiv1    = 0u,     ///< WFG timer clock prescaler: None
    WfgPlckDiv2    = 1u,     ///< WFG timer clock prescaler: 1/2
    WfgPlckDiv4    = 2u,     ///< WFG timer clock prescaler: 1/4
    WfgPlckDiv8    = 3u,     ///< WFG timer clock prescaler: 1/8
    WfgPlckDiv16   = 4u,     ///< WFG timer clock prescaler: 1/16
    WfgPlckDiv32   = 5u,     ///< WFG timer clock prescaler: 1/32
    WfgPlckDiv64   = 6u,     ///< WFG timer clock prescaler: 1/64
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)     
    WfgPlckDiv128  = 7u,     ///< WFG timer clock prescaler: 1/128
#endif  
}en_wfg_timer_clock_t;

/******************************************************************************
 * brief noise canceling width for a digital noise-canceler
 ******************************************************************************/
typedef enum en_nzcl_filter_width
{
    NzlcNoFilter,      ///< no noise-canceling
    NzlcWidth4Cycle,   ///< 4 PCLK cycles
    NzlcWidth8Cycle,   ///< 8 PCLK cycles
    NzlcWidth16Cycle,  ///< 16 PCLK cycles
    NzlcWidth32Cycle,  ///< 32 PCLK cycles
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)       
    NzlcWidth64Cycle,  ///< 64 PCLK cycles
    NzlcWidth128Cycle, ///< 128 PCLK cycles
    #if (PDL_MCU_TYPE >= PDL_FM4_TYPE3)
    NzlcWidth256Cycle, ///< 256 PCLK cycles
    #endif
#endif    
    
}en_nzcl_filter_width_t;

/** \} GroupMFT_WFG_Types */

/**
* \addtogroup GroupMFT_WFG_DataStructures
* \{
*/

/******************************************************************************
 * \brief Configuration structure of waveform generator
 ******************************************************************************/
typedef struct stc_wfg_config
{
    en_mft_wfg_mode_t enMode;    ///< select the output condition
    en_gten_bits_t   enGtenBits; ///< select the output condition
    en_psel_bits_t   enPselBits; ///< select the PPG timer unit
    en_pgen_bits_t   enPgenBits; ///< how to reflect the CH_PPG signal
    en_dmod_bits_t   enDmodBits; ///< polarity for RTO0 and RTO1 signal output
    
    en_wfg_timer_clock_t enClk;  ///< clock division of WFG timer

#if (PDL_INTERRUPT_ENABLE_MFT0_WFG == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_WFG == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_WFG == PDL_ON)      
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
    boolean_t   bWfgimerIrqMask;      ///< TRUE: mask timer interrupt, FALSE: don't mask timer interrupt
    #endif    
    func_ptr_t  pfnWfgTimerIrqCallback;  ///< Poitner to WFG timer callback function
    boolean_t   bTouchNvic;              ///< TRUE: enable WFG NVIC, FALSE, don't enable WFG NVIC
#endif    
    
}stc_wfg_config_t;

/******************************************************************************
 * \brief WFG internal data
 ******************************************************************************/
typedef struct stc_mft_wfg_intern_data
{
    func_ptr_t  pfnWfg10TimerIrqCallback;   ///< Callback function pointer of WFG10 timer interrupt callback  
    func_ptr_t  pfnWfg32TimerIrqCallback;   ///< Callback function pointer of WFG32 timer interrupt callback  
    func_ptr_t  pfnWfg54TimerIrqCallback;   ///< Callback function pointer of WFG54 timer interrupt callback  
    func_ptr_t  pfnDtifAnalogFilterIrqCallback;    ///< Callback function pointer of analog filter interrupt callback 
    func_ptr_t  pfnDtifDigtalFilterIrqCallback;    ///< Callback function pointer of digital filter interrupt callback 
}stc_mft_wfg_intern_data_t;

/******************************************************************************
 * brief NZCL configure
 ******************************************************************************/
typedef struct stc_wfg_nzcl_config
{
    boolean_t               bEnDigitalFilter;      ///< enable digital filter
    en_nzcl_filter_width_t  enDigitalFilterWidth;  ///< digital filter width
#if(PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
    boolean_t               bEnAnalogFilter;       ///< enable analog filter
    #if(PDL_MCU_TYPE >= PDL_FM4_TYPE3)
    boolean_t               bHoldRto;              ///< TRUE: hold RTO when DTIF interrupt occurs, FALSE: don't hold RTO when DTIF interrupt occurs
    #endif
#endif    
    boolean_t               bSwitchToGpio;         ///< switch to GPIO   

#if (PDL_INTERRUPT_ENABLE_MFT0_WFG == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT1_WFG == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFT2_WFG == PDL_ON)      
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
    boolean_t  bDtifDigitalFilterIrqMask;          ///< Digital filter interrupt selection  
    boolean_t  bDtifAnalogFilterIrqMask;           ///< Analog filter interrupt selection
    #endif      
    
    func_ptr_t  pfnDtifDigtalFilterIrqCallback;    ///< Pointer to digital filter interrupt callback function
    #if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
    func_ptr_t  pfnDtifAnalogFilterIrqCallback;    ///< Pointer to analog filter interrupt callback function
    #endif
    
    boolean_t               bTouchNvic;            ///< TRUE: 
#endif
    
}stc_wfg_nzcl_config_t;

/******************************************************************************
 * brief structure of WFG instance data
 ******************************************************************************/
typedef struct stc_mft_wfg_instance_data
{
  volatile stc_mftn_wfg_t*  pstcInstance;  ///< pointer to registers of an instance
  stc_mft_wfg_intern_data_t stcInternData; ///< module internal data of instance
} stc_mft_wfg_instance_data_t;

/** \} GroupMFT_WFG_DataStructures */

/**
* \addtogroup GroupMFT_WFG_GlobalVariables
* \{
*/

/******************************************************************************/
/* Global variable declarations ('extern', definition in C source)            */
/******************************************************************************/
/// Look-up table for all enabled WFG  instances and their internal data
extern stc_mft_wfg_instance_data_t m_astcMftWfgInstanceDataLut[WFG_INSTANCE_COUNT];

/** \} GroupMFT_WFG_GlobalVariables */

/**
* \addtogroup GroupMFT_WFG_Functions
* \{
*/

/******************************************************************************/
/* Global function prototypes ('extern', definition in C source)              */
/******************************************************************************/
/* 1. WFG function configuration */
en_result_t Mft_Wfg_Init(volatile stc_mftn_wfg_t* pstcWfg,
		                 uint8_t u8CoupleCh, 
                         const stc_wfg_config_t* pstcConfig);
en_result_t Mft_Wfg_DeInit(volatile stc_mftn_wfg_t* pstcWfg, 
                           uint8_t u8CoupleCh, 
                           boolean_t bTouchNvic);

en_result_t Mft_Wfg_StartTimer(volatile stc_mftn_wfg_t* pstcWfg, uint8_t u8CoupleCh);
en_result_t Mft_Wfg_StopTimer(volatile stc_mftn_wfg_t* pstcWfg, uint8_t u8CoupleCh);
en_irq_flag_t Mft_Wfg_GetTimerIrqFlag(volatile stc_mftn_wfg_t* pstcWfg, uint8_t u8CoupleCh);
en_result_t Mft_Wfg_ClrTimerIrqFlag(volatile stc_mftn_wfg_t* pstcWfg, uint8_t u8CoupleCh);
en_result_t Mft_Wfg_WriteTimerCountCycle(volatile stc_mftn_wfg_t* pstcWfg,
                                         uint8_t u8CoupleCh, 
                                         uint16_t u16CycleA, 
                                         uint16_t u16CycleB);
#if(PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE) 
en_result_t Mft_Wfg_SetFilterCountValue( volatile stc_mftn_wfg_t* pstcWfg,
                                         uint8_t u8CoupleCh, uint16_t u16Count);
#endif
/* 2. WFG NZCL configuration */
en_result_t Mft_Wfg_Nzcl_Init(volatile stc_mftn_wfg_t* pstcWfg,
                              stc_wfg_nzcl_config_t* pstcNzclConfig,
                              boolean_t bTouchNvic);
en_result_t Mft_Wfg_Nzcl_DeInit(volatile stc_mftn_wfg_t* pstcWfg, 
                                boolean_t bTouchNvic);
en_result_t Mft_Wfg_Nzcl_SwTriggerDtif(volatile stc_mftn_wfg_t* pstcWfg);
en_irq_flag_t Mft_Wfg_Nzcl_GetDigitalFilterIrqFlag(volatile stc_mftn_wfg_t* pstcWfg);
en_result_t Mft_Wfg_Nzcl_ClrDigitalFilterIrqFlag(volatile stc_mftn_wfg_t* pstcWfg);
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
en_irq_flag_t Mft_Wfg_Nzcl_GetAnalogFilterIrqFlag(volatile stc_mftn_wfg_t* pstcWfg);
en_result_t Mft_Wfg_Nzcl_ClrAnalogFilterIrqFlag(volatile stc_mftn_wfg_t* pstcWfg);
#endif

/* 3. IRQ handler */
void Mft_Wfg_IrqHandler(volatile  stc_mftn_wfg_t* pstcWfg,
                        stc_mft_wfg_intern_data_t* pstcMftWfgInternData) ;

/** \} GroupMFT_WFG_Functions */
/** \} GroupMFT_WFG */
                        
#ifdef __cplusplus
}
#endif

#endif // #if (defined(PDL_PERIPHERAL_ADC_ACTIVE))
#endif
