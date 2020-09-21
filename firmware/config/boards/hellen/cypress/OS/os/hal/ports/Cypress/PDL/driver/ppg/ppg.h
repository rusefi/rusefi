/*******************************************************************************
* \file              ppg.h
* 
* \version           1.20
*
* \brief             Headerfile for PPG functions.
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

#ifndef __PPG_H__
#define __PPG_H__

#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_PPG_ACTIVE))

/**
* \defgroup GroupPPG Programmable Pulse Generator (PPG)
* \{ 
* \defgroup GroupPPG_Macros Macros
* \defgroup GroupPPG_Functions Functions
* \defgroup GroupPPG_DataStructures Data Structures
* \defgroup GroupPPG_Types Enumerated Types
* \} 
*/

/**
* \addtogroup GroupPPG
* \{
* Use the Programmable Pulse Generator (PPG) peripheral in conjunction 
* with a Waveform Generator (WFG) to create an arbitrary pulse output 
* (both count and duty cycle). The WFG is one of the elements of the 
* Multi-function Timer (MFT). The Waveform Generator (WFG) gets a 
* signal from the Output Compare Unit (OCU) and/or a PPG. <br>
*
* There are up to 24 available PPG channels. However, only 9 channels 
* can generate an output signal. The output is wired downstream to a 
* WFG. There may be as many as three WFGs, each with three inputs. 
* The number of available channels (PPG and WFG) varies among microcontroller 
* series. <br>
* These PPG channels generate output:<br>
* * PPG channels 0, 2, 4 generate output to MFT0
* * PPG channels 8, 10, 12 generate output to MFT1
* * PPG channels 16,18,20 generate output to MFT2 <br>
* Each PPG channel has an 8-bit counter. You connect two or more PPG 
* channels to generate output pulses with longer count lengths. There are four 
* operating modes:<br>
* * 8-bit – one channel, the output pulse width specified by an 8-bit value
* * 8+8-bit – two channels used, one is a counter, the other a prescalar
* * 16-bit – two channels are connected, the output pulse width specified by a 16-bit value
* * 16+16-bit mode – two connected channels are a 16-bit counter, and two connected 
* channels are a 16-bit prescalar <br>
* There are three timing generators. Each consists of an 8-bit up counter and four 
* compare values. Each timing generator controls four PPG channels.<br>
* * Timing generator 0 – channels 0,2,4,6
* * Timing generator 1 – channels 8,10,12,14
* * Timing generator 2 – channels 16,18,20, 22<br>
* Finally you can configure the PPG trigger to be: <br>
* * Software
* * The timing generator
* * A GATE signal from the MFT
* 
* \section SectionPPG_ConfigurationConsideration Configuration Consideration
* To set up a PPG, you must also configure a WFG. See the description in MFT WFG.<br>
* For configuration purposes, the PDL treats PPG channels as pairs, one even, one odd. 
* To set up the PPG, fill in the fields of the stc_ppg_config_t structure for 
* each channel you wish to configure. For example, you provide the operating mode, 
* the clock prescalar for the even and odd numbered channel, the trigger mode, and so on. 
* Call Ppg_Init() with the channel pair, and the address of the configuration structure. 
* You can configure each channel pair independently.<br>
* If you use the timing generator to trigger a PPG, you must configure the required 
* timer for the PPG channel. Use the corresponding configuration structure such as 
* stc_ppg_upcnt0_config_t. Then call the corresponding configuration function such as 
* Ppg_ConfigUpCnt0(). <br>
* How you start a PPG depends on how you trigger it:<br>
* * Software trigger – call Ppg_StartSoftwareTrig()
* * Timing generator – start the timer with a call such as Ppg_StartUpCnt0().
* * GATE signal – set the GATE signal to an valid level in the MFT module <br>
* If you use the GATE level as a trigger, use Ppg_SelGateLevel() to select the valid level.<br>
* How you stop a PPG depends upon the way you trigger it:<br>
* * Software trigger – call Ppg_StopSoftwareTrig()
* * Timing generator – call the correct disable routine, such as Ppg_DisableTimerGen0StartTrig()
* * GATE signal - set the GATE signal to an invalid level in the MFT module<br>
* When an enabled interrupt occurs, the interrupt flag is cleared and control passes 
* to the callback function. You can also use polling to manage interrupts. With polling mode, 
* use Ppg_GetIrqFlag() to poll for an interrupt. Clear the interrupt flag with Ppg_ClrIrqFlag().<br>
* 
*
* \section SectionPPG_MoreInfo More Information
* For more information on the PPG peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/223041/download">FM0+ Peripheral Manual - Timer Subsystem TRM.pdf</a><br>
* <a href="http://www.cypress.com/file/222986/download">FM4 Peripheral Manual - Timer Subsystem TRM.pdf</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*
*/

/**
* \addtogroup GroupPPG_Macros
* \{
*/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/
#define  PPG_CH0            0u
#define  PPG_CH1            1u
#define  PPG_CH2            2u
#define  PPG_CH3            3u
#define  PPG_CH4            4u
#define  PPG_CH5            5u
#define  PPG_CH6            6u
#define  PPG_CH7            7u
#define  PPG_CH8            8u
#define  PPG_CH9            9u
#define  PPG_CH10           10u
#define  PPG_CH11           11u
#define  PPG_CH12           12u
#define  PPG_CH13           13u
#define  PPG_CH14           14u
#define  PPG_CH15           15u
#define  PPG_CH16           16u
#define  PPG_CH17           17u
#define  PPG_CH18           18u
#define  PPG_CH19           19u
#define  PPG_CH20           20u
#define  PPG_CH21           21u
#define  PPG_CH22           22u
#define  PPG_CH23           23u

#define  PPG_COUPLE_CH01        0u
#define  PPG_COUPLE_CH23        2u
#define  PPG_COUPLE_CH45        4u
#define  PPG_COUPLE_CH67        6u
#define  PPG_COUPLE_CH89        8u
#define  PPG_COUPLE_CH1011      10u
#define  PPG_COUPLE_CH1213      12u
#define  PPG_COUPLE_CH1415      14u
#define  PPG_COUPLE_CH1617      16u
#define  PPG_COUPLE_CH1819      18u
#define  PPG_COUPLE_CH2021      20u
#define  PPG_COUPLE_CH2223      22u

#define  PPG_INT_MAX_CH     9u  // Including channel.0,2,4,8,10,12,16,18,20

#define  PPG_UP_COUNTER_CH0     0u
#define  PPG_UP_COUNTER_CH1     1u
#define  PPG_UP_COUNTER_CH2     2u

/** \} GroupPPG_Macros */

/**
* \addtogroup GroupPPG_Types
* \{
*/

/**
 ******************************************************************************
 ** \brief Enumeration to define PPG operation mode
 ** \note only following combination of 4 channel is valid:
 **       Ch0_Ch1            Ch2_Ch3
 **       Ppg8Bit8Bit        Ppg8Bit8Bit
 **       Ppg8Bit8Bit        Ppg8Bit8Pres
 **       Ppg8Bit8Bit        Ppg16Bit
 **       Ppg8Bit8Pres       Ppg8Bit8Bit
 **       Ppg8Bit8Pres       Ppg8Bit8Pres
 **       Ppg8Bit8Pres       Ppg16Bit
 **       Ppg16Bit           Ppg8Bit8Bit
 **       Ppg16Bit           Ppg8Bit8Pres
 **       Ppg16Bit           Ppg16Bit
 **       Ppg16Bit16Pres     Ppg16Bit16Pres [Act as Prescaler]       
 **
 ******************************************************************************/
typedef enum en_ppg_opt_mode
{
    Ppg8Bit8Bit     = 0u,   ///< Even channel: 8bit PPG, odd channel: 8bit PPG
    Ppg8Bit8Pres    = 1u,   ///< Even channel: 8bit PPG, odd channel: 8bit prescaler
    Ppg16Bit        = 2u,   ///< 16bit PPG
    Ppg16Bit16Pres  = 3u,   ///< 16bit PPG + 16 prescaler (provided by next couple channel)
    
}en_ppg_opt_mode_t;

/**
 ******************************************************************************
 ** \brief Enumeration to define PPG count clock prescaler
 ******************************************************************************/
typedef enum en_ppg_clock
{
    PpgPclkDiv1   = 0u,     ///< PPG count clock prescaler: 1
    PpgPclkDiv4   = 1u,     ///< PPG count clock prescaler: 1/4
    PpgPclkDiv16  = 2u,     ///< PPG count clock prescaler: 1/16
    PpgPclkDiv64  = 3u,     ///< PPG count clock prescaler: 1/64
  
}en_ppg_clock_t;

/**
 ******************************************************************************
 ** \brief Enumeration to define PPG trigger
 ******************************************************************************/
typedef enum en_ppg_trig
{
    PpgSoftwareTrig   = 0u,     ///< Use software to trigger PPG
    PpgMftGateTrig    = 1u,     ///< Use GATE signal from MFT to trigger PPG
    PpgTimingGenTrig  = 2u,     ///< Use Timing Generator to trigger PPG
  
}en_ppg_trig_t;

/**
 ******************************************************************************
 ** \brief Enumeration to define PPG output initial level
 ******************************************************************************/
typedef enum en_ppg_level
{
    PpgNormalLevel   = 0u,      ///< Initial level: Low
    PpgReverseLevel  = 1u,      ///< Initial level: High
 
}en_ppg_level_t;

/**
 ******************************************************************************
 ** \brief PPG interrupt mode
 ******************************************************************************/
typedef enum en_ppg_irq_mode
{
    PpgIrqHighOrLowUnderflow = 0u,   ///< Generate interrupt when high or low width count underflows
    PpgIrqHighUnderflow      = 1u,   ///< Generate interrupt when high width count underflows

}en_ppg_irq_mode_t;
/**
 ******************************************************************************
 ** \brief Enumeration to define valid level of GATE signal from MFT
 ******************************************************************************/
typedef enum en_ppg_gate_level
{
    PpgGateHighActive = 0u,  ///< GATE valid level: High
    PpgGateLowActive  = 1u,  ///< GATE valid level: Low
    
}en_ppg_gate_level_t;

/**
 ******************************************************************************
 ** \brief Enumeration to define clock prescaler of up counter 
 **        in timing generator
 ******************************************************************************/
typedef enum en_ppg_upcnt_clk
{ 
    PpgUpCntPclkDiv2   = 0u,      ///< Up counter clock prescaler: 1/2
    PpgUpCntPclkDiv8   = 1u,      ///< Up counter clock prescaler: 1/8
    PpgUpCntPclkDiv32  = 2u,      ///< Up counter clock prescaler: 1/32
    PpgUpCntPclkDiv64  = 3u,      ///< Up counter clock prescaler: 1/64
    
}en_ppg_upcnt_clk_t;
/**
 ******************************************************************************
 ** \brief Enumeration to define IBGT prohibition mode
 ******************************************************************************/
typedef enum en_igbt_prohibition_mode
{
    IgbtNormalMode             = 0u,       ///< Normal mode
    IgbtStopProhibitionMode    = 1u,       ///< Stop prohibition mode in output active
    
}en_igbt_prohibition_mode_t;

/**
 ******************************************************************************
 ** \brief Enumeration to define IBGT noise filter width
 ******************************************************************************/
typedef enum en_igbt_filter_width
{
    IgbtNoFilter     = 0u,             ///< No noise filter
    IgbtFilter4Pclk  = 1u,             ///< noise filter width: 4PCLK
    IgbtFilter8Pclk  = 2u,             ///< noise filter width: 8PCLK
    IgbtFilter16Pclk = 3u,             ///< noise filter width: 16PCLK 
    IgbtFilter32Pclk = 4u,             ///< noise filter width: 32PCLK
  
}en_igbt_filter_width_t;

/**
 ******************************************************************************
 ** \brief Enumeration to define IBGT input/out level
 ******************************************************************************/
typedef enum en_igbt_level
{
    IgbtLevelNormal = 0u,          ///< Normal
    IgbtLevelInvert = 1u,          ///< Invert
    
}en_igbt_level_t;

/**
 ******************************************************************************
 ** \brief PPG IRQ channel
 ******************************************************************************/
typedef enum en_ppg_irq_ch
{
    PpgCh024    = 0u,               ///< IRQ 36 for  PPG ch.0,2,4
    PpgCh81012  = 1u,               ///< IRQ 37 for  PPG ch.8,10,12
    PpgCh161820 = 2u,               ///< IRQ 38 for  PPG ch.16,18,20
    
}en_ppg_irq_ch_t;

/** \} GroupPPG_Types */

/**
* \addtogroup GroupPPG_DataStructures
* \{
*/

/**
 ******************************************************************************
 ** \brief Structure to config PPG
 ******************************************************************************/
typedef struct stc_ppg_config
{
    en_ppg_opt_mode_t       enMode;       ///< PPG mode configuration
    en_ppg_clock_t          enEvenClock;  ///< Clock prescaler of even channel
    en_ppg_clock_t          enOddClock;   ///< Clock prescaler of odd channel
    en_ppg_level_t          enEvenLevel;  ///< Output level of even channel
    en_ppg_level_t          enOddLevel;   ///< Output level of odd channel
    en_ppg_trig_t           enTrig;       ///< PPG trigger mode configuration
    en_ppg_irq_mode_t       enIrqMode;    ///< PPG interrupt mode
    
    boolean_t               bIrqEnable;     ///< TRUE: enable IRQ, FALSE: don't enable IRQ
    func_ptr_t              pfnIrqCallback; ///< Pointer to PPG interrupt callback function
    boolean_t               bTouchNvic;     ///< TRUE: enable NVIC, FALSE: don't enable NVIC
    
}stc_ppg_config_t;

/**
 ******************************************************************************
 ** \brief Up counter0 configuration
 ******************************************************************************/
typedef struct stc_ppg_upcnt0_config
{
    en_ppg_upcnt_clk_t   enClk;        ///< Up counter clock prescaler
    uint8_t              u8CmpValue0;  ///< Up counter compare value for Ch0
    uint8_t              u8CmpValue2;  ///< Up counter compare value for Ch2
    uint8_t              u8CmpValue4;  ///< Up counter compare value for Ch4
    uint8_t              u8CmpValue6;  ///< Up counter compare value for Ch6
    
}stc_ppg_upcnt0_config_t;

/**
 ******************************************************************************
 ** \brief Up counter0 stop channels selection
 ******************************************************************************/
typedef struct stc_timer0_gen_ch
{
    boolean_t            bPpgCh0;  ///< 1: select Ch.0, 0: not selected
    boolean_t            bPpgCh2;  ///< 1: select Ch.2, 0: not selected
    boolean_t            bPpgCh4;  ///< 1: select Ch.4, 0: not selected
    boolean_t            bPpgCh6;  ///< 1: select Ch.6, 0: not selected
    
}stc_timer0_gen_ch_t;

/**
 ******************************************************************************
 ** \brief Up counter1 configuration
 ******************************************************************************/
typedef struct stc_ppg_upcnt1_config
{
    en_ppg_upcnt_clk_t   enClk;            ///< Up counter clock prescaler
    uint8_t              u8CmpValue8;      ///< Up counter compare value for Ch8
    uint8_t              u8CmpValue10;     ///< Up counter compare value for Ch10
    uint8_t              u8CmpValue12;     ///< Up counter compare value for Ch12
    uint8_t              u8CmpValue14;     ///< Up counter compare value for Ch14
    
}stc_ppg_upcnt1_config_t;

/**
 ******************************************************************************
 ** \brief Up counter1 stop channels selection
 ******************************************************************************/
typedef struct stc_timer1_gen_ch
{
    boolean_t            bPpgCh8;    ///< 1: select Ch.8, 0: not selected
    boolean_t            bPpgCh10;   ///< 1: select Ch.10, 0: not selected
    boolean_t            bPpgCh12;   ///< 1: select Ch.12, 0: not selected
    boolean_t            bPpgCh14;   ///< 1: select Ch.14, 0: not selected
    
}stc_timer1_gen_ch_t;

/**
 ******************************************************************************
 ** \brief Up counter2 configuration
 ******************************************************************************/
typedef struct stc_ppg_upcnt2_config
{
    en_ppg_upcnt_clk_t   enClk;         ///< Up counter clock prescaler
    uint8_t              u8CmpValue16;  ///< Up counter compare value for Ch16
    uint8_t              u8CmpValue18;  ///< Up counter compare value for Ch18
    uint8_t              u8CmpValue20;  ///< Up counter compare value for Ch20
    uint8_t              u8CmpValue22;  ///< Up counter compare value for Ch22
    
}stc_ppg_upcnt2_config_t;

/**
 ******************************************************************************
 ** \brief Up counter2 stop channels selection
 ******************************************************************************/
typedef struct stc_timer2_gen_ch
{
    boolean_t            bPpgCh16;  ///< 1: select Ch.16, 0: not selected
    boolean_t            bPpgCh18;  ///< 1: select Ch.18, 0: not selected
    boolean_t            bPpgCh20;  ///< 1: select Ch.20, 0: not selected 
    boolean_t            bPpgCh22;  ///< 1: select Ch.22, 0: not selected 
    
}stc_timer2_gen_ch_t;

/**
 ******************************************************************************
 ** \brief structure of PPG IGBT configuration
 ******************************************************************************/
typedef struct stc_ppg_igbt_config
{
    en_igbt_prohibition_mode_t  enMode;              ///< prohibition mode
    en_igbt_filter_width_t      enWidth;             ///< noise filter width
    en_igbt_level_t             enTrigInputLevel;    ///< Trigger input level
    en_igbt_level_t             enIgbt0OutputLevel;  ///< IGBT0 output level (PPG0)
    en_igbt_level_t             enIgbt1OutputLevel;  ///< IGBT1 output level (PPG4) 
    
}stc_ppg_igbt_config_t;

/** \} GroupPPG_DataStructures */

/**
* \addtogroup GroupPPG_Functions
* \{
*/

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
/* 1. Init/De-Init */
en_result_t Ppg_Init( uint8_t u8CoupleCh, const stc_ppg_config_t *pstcConfig);
en_result_t Ppg_DeInit(uint8_t u8CoupleCh, boolean_t bTouchNvic);
/* 2. Trigger configuration */
/* software */
en_result_t Ppg_StartSoftwareTrig(uint8_t u8Ch);
en_result_t Ppg_StopSoftwareTrig(uint8_t u8Ch);
/* MFT gate signal */
en_result_t Ppg_SelGateLevel(uint8_t u8EvenCh, en_ppg_gate_level_t enLevel);
/* Timing generator configuration */
en_result_t  Ppg_ConfigUpCnt0(stc_ppg_upcnt0_config_t* pstcConfig);
void  Ppg_StartUpCnt0(void);
en_stat_flag_t  Ppg_GetUpCnt0Status(void);
en_result_t  Ppg_DisableTimerGen0StartTrig(stc_timer0_gen_ch_t* pstcTimer0GenCh);

en_result_t  Ppg_ConfigUpCnt1(stc_ppg_upcnt1_config_t* pstcConfig);
void  Ppg_StartUpCnt1(void);
en_stat_flag_t  Ppg_GetUpCnt1Status(void);
en_result_t  Ppg_DisableTimerGen1StartTrig(stc_timer1_gen_ch_t* pstcTimer1GenCh);

en_result_t  Ppg_ConfigUpCnt2(stc_ppg_upcnt2_config_t* pstcConfig);
void  Ppg_StartUpCnt2(void);
en_stat_flag_t  Ppg_GetUpCnt2Status(void);
en_result_t  Ppg_DisableTimerGen2StartTrig(stc_timer2_gen_ch_t* pstcTimer2GenCh);
/* 3. Int */
#if (PDL_INTERRUPT_ENABLE_PPG == PDL_ON) 
en_result_t  Ppg_EnableIrq(uint8_t u8CoupleCh);
en_result_t  Ppg_DisableIrq(uint8_t u8CoupleCh);
#endif
en_irq_flag_t  Ppg_GetIrqFlag(uint8_t u8CoupleCh);
en_result_t  Ppg_ClrIrqFlag(uint8_t u8CoupleCh);
/* 4. H/L width set */
en_result_t  Ppg_SetLevelWidth(uint8_t u8Ch, uint8_t u8LowWidth, uint8_t u8HighWidth);

/* 5. IGBT mode */
#if (defined(FM_MFT_PPG_IGBTC))
    en_result_t  Ppg_InitIgbt(stc_ppg_igbt_config_t* pstcPpgIgbt);
    void Ppg_EnableIgbtMode(void);
    void Ppg_DisableIgbtMode(void);
#endif
    
/* 6. IRQ handler */
void Ppg_IrqHandler(void);

/** \} GroupPPG_Functions */
/** \} GroupPPG */

#ifdef __cplusplus
}
#endif

#endif

#endif
