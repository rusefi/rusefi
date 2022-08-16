/*******************************************************************************
* \file             dt.h
* 
* \version          1.20
*                       
* \brief            Headerfile for DT functions.
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

#ifndef __DT_H__
#define __DT_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_DT_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \defgroup GroupDT Dual Timer (DT)
* \{ 
* \defgroup GroupDT_Macros Macros
* \defgroup GroupDT_Functions Functions
* \defgroup GroupDT_DataStructures Data Structures
* \defgroup GroupDT_Types Enumerated Types
* \}
*/

/**
* \addtogroup GroupDT
* \{
* The Dual Timer implements two programmable 32-/16-bit Down Counters, called timer channels.
* You can use a counter in one of three timer modes:<br> 
* −  Free-running mode. 
* The counter operates continuously and wraps around to its maximum value each
* time that it reaches zero.<br>
* −  Periodic mode. 
* The counter is reloaded and operates continuously each time
* that it reaches zero.<br>
* −  One-shot mode. 
* The counter stops when it reaches zero.<br>
* You can write a count value to a timer. If the timer is running, the counter 
* restarts immediately using the new value. When running in periodic mode, you 
* can also write a count value to a buffer (background). When the timer reaches zero, 
* it uses this background value as the new count value.<br>
* The counters have a common timer clock (TIMCLK). The source for TIMCLK is the APB bus clock 
* (PCLK). Each counter also has a prescaler
* that can divide TIMCLK by 1, 16, or 256, so you can control the count rate of each independently.<br>
* To calculate the timer frequency, use the formula:
* Timer_Frequency = Clock_Frequency / Count_Value <br>
* Where Clock_Frequency is the divided clock frequency, and Count_Value is the initial value of the 
* counter. For example, if the divided clock frequency is 40 MHz, and the counter is set to 1,000,000, 
* then the timer frequency will be 40 Hz. <br>
*
* \section SectionDT_ConfigurationConsideration Configuration Consideration
* To set up DT, you provide configuration parameters in the #stc_dt_channel_config_t
* structure. Then call Dt_Init(), where the second parameter is the number of 
* channel you use. Use Dt_EnableCount() to start the counter, and Dt_DisableCount() to stop it.<br>
* Call Dt_WriteLoadVal() to set a new count value immediately. Use 
* Dt_WriteBgLoadVal() to load the background value for the next counting period. This works only in periodic mode. It has no effect on free-running or one-shot mode.<br>
* Use Dt_ReadCurCntVal() to get the current counter value.<br>
*  
*
* \note Before calling Dt_DeInit(), you should
* disable all channels via Dt_DisableCount() and Dt_DisableIrq(), to avoid a 
* possible, unwanted interrupt.
* 
* \section SectionDT_MoreInfo More Information
* For more information on the DT peripheral, refer to:<br>
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
* \addtogroup GroupDT_Macros
* \{
*/

/*******************************************************************************
* Global definitions
*******************************************************************************/
#define DT0     (*((volatile stc_dtn_t *) FM_DT_BASE))

/* for TimerPre of TimerXControl */
#define DT_PRE_TIMER_DIV_1      (0x00)
#define DT_PRE_TIMER_DIV_16     (0x01)
#define DT_PRE_TIMER_DIV_256    (0x02)

/** \} GroupDT_Macros */

/**
* \addtogroup GroupDT_Types
* \{
*/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/
 
 /**
 ******************************************************************************
 ** \brief Dt mode
 ** 
 ** To select between Free-run, Periodic, and One-Shot mode
 ******************************************************************************/
typedef enum en_dt_mode
{
    DtFreeRun      = 0u,  ///< Free-running mode
    DtPeriodic     = 1u,  ///< Periodic mode
    DtOneShot      = 2u   ///< One-shot mode
} en_dt_mode_t;

/**
 ******************************************************************************
 ** \brief Dt Prescaler
 ** 
 ** To select clock divider
 ******************************************************************************/
typedef enum en_dt_prescaler
{
    DtPrescalerDiv1     = 0u,  ///< Prescaler divisor 1
    DtPrescalerDiv16    = 1u,  ///< Prescaler divisor 16
    DtPrescalerDiv256   = 2u   ///< Prescaler divisor 256
} en_dt_prescaler_t;

/**
 ******************************************************************************
 ** \brief Dt Counter Size
 ** 
 ** To select the size of the counter
 ******************************************************************************/
typedef enum en_dt_countersize
{
    DtCounterSize16     = 0u,  ///< 16 Bit counter size
    DtCounterSize32     = 1u   ///< 32 Bit counter size
} en_dt_countersize_t;

/**
 ******************************************************************************
 ** \brief Dt channel number
 ******************************************************************************/
typedef enum en_dt_channel
{
    DtChannel0     = 0u,  ///< channel 0
    DtChannel1     = 1u,  ///< channel 1
    DtMaxChannels  = 2u   ///< Number of channels
} en_dt_channel_t;
/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/

/// Enumeration to define an index for each enabled Dual timer instance
typedef enum en_dt_instance_index
{
#if (PDL_PERIPHERAL_ENABLE_DT0 == PDL_ON)
    DtInstanceIndexDt0 = 0u,
#endif
    DtInstanceIndexMax
} en_dt_instance_index_t;

/** \} GroupDT_Types */

/**
* \addtogroup GroupDT_DataStructures
* \{
*/

/**
 ******************************************************************************
 ** \brief Redefinition of DT register structure 
 ******************************************************************************/
typedef FM_DT_TypeDef stc_dtn_t;

/**
 *****************************************************************************
 ** \brief Dt channel configuration
 ** 
 ** The DT configuration is done on a per channel basis
 *****************************************************************************/
typedef struct stc_dt_channel_config
{
    uint8_t u8Mode;           ///< Mode, see description of #en_dt_mode_t
    uint8_t u8PrescalerDiv;   ///< Prescaler, see description of #en_dt_prescaler_t
    uint8_t u8CounterSize;    ///< Counter size, see description of #en_dt_countersize_t
#if (PDL_INTERRUPT_ENABLE_DT0 == PDL_ON)    
    boolean_t  bIrqEnable;      ///< TRUE: enable interrupt, FALSE: disable interrupt
    func_ptr_t pfnIrqCallback;  ///< Pointer to interrupt callback function
    boolean_t  bTouchNvic;      ///< TRUE: enable MVIC, FALSE: don't enable NVIC
#endif    
} stc_dt_channel_config_t;

/// Datatype for holding internal data needed for DT
typedef struct stc_dt_intern_data
{
    /// Callback for interrupts of DT channel 0
    func_ptr_t  pfnIrqCallbackIntern[DtMaxChannels] ;
} stc_dt_intern_data_t ;

/// DT instance data type
typedef struct stc_dt_instance_data
{
    volatile stc_dtn_t*  pstcInstance;  ///< pointer to registers of an instance
    stc_dt_intern_data_t stcInternData; ///< module internal data of instance
} stc_dt_instance_data_t;

/** \} GroupDT_DataStructures */

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/

/**
* \addtogroup GroupDT_Functions
* \{
*/

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/
#if (PDL_INTERRUPT_ENABLE_DT0 == PDL_ON)
  /* External function  */
  void DtIrqHandler1(void);
  void DtIrqHandler2(void);
  #define DtIrqHandler(u8Channel)      (((u8Channel) == (0)) ? ( DtIrqHandler1() ) : ( DtIrqHandler2() ))
  en_result_t Dt_EnableIrq (uint8_t u8Channel);
  en_result_t Dt_DisableIrq(uint8_t u8Channel);
#endif

/* Init/Deinit */
en_result_t Dt_Init(const stc_dt_channel_config_t* pstcConfig,
                    uint8_t u8Channel);
en_result_t Dt_DeInit(uint8_t u8Channel, boolean_t bTouchNvic);

/* Function Enable/Disable */
en_result_t Dt_EnableCount(uint8_t u8Channel);
en_result_t Dt_DisableCount(uint8_t u8Channel);

/* Get/Clr irq flag */
boolean_t Dt_GetIrqFlag(uint8_t u8Channel);
boolean_t Dt_GetMaskIrqFlag(uint8_t u8Channel);
en_result_t Dt_ClrIrqFlag(uint8_t u8Channel);

/* Write/Read count value */
en_result_t Dt_WriteLoadVal(uint32_t u32LoadVal,
                            uint8_t  u8Channel);
en_result_t Dt_WriteBgLoadVal(uint32_t u32BgLoadVal,
                              uint8_t  u8Channel);
uint32_t Dt_ReadCurCntVal(uint8_t u8Channel);

/** \} GroupDT_Functions */
/** \} GroupDT */

#ifdef __cplusplus
}
#endif

#endif /* #if (defined(PDL_PERIPHERAL_DT_ACTIVE)) */

#endif /* __DT_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
