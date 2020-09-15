/*******************************************************************************
* \file              qprc.h
* 
* \version           1.20
*
* \brief             Headerfile for QPRC functions.
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

#ifndef __QPRC_H__
#define __QPRC_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_QPRC_ACTIVE))

/**
* \defgroup GroupQPRC Quadrature Position/Revolution Counter (QPRC)
* \{  
* \defgroup GroupQPRC_Macros Macros
* \defgroup GroupQPRC_Functions Functions
* \defgroup GroupQPRC_DataStructures Data Structures
* \defgroup GroupQPRC_Types Enumerated Types
* \defgroup GroupQPRC_GlobalVariables Global Variables
* \}
*/

/**
* \addtogroup GroupQPRC
* \{
* The Quadrature Position/Revolution Counter (QPRC) peripheral is typically 
* used in motor control applications to track the position, speed, or rotation of a motor.<br>
* The position counter (PC) receives an input signal from two pins denoted AIN and BIN. 
* The counter increments or decrements based on the signal edge on the pins (configurable). 
* The revolution counter (RC) receives an input from the ZIN pin and/or the output of the 
* position counter (underflow or overflow). The counter increments or decrements based on 
* your configuration choices. <br>
* There are two compare values: the Position compare value, and the Position and 
* Revolution compare value (PCRC).<br>
* All compare values and counters are 16-bit values.<br>
* There is a configurable noise filter for the signal on each of the three input pins.<br>
* Based on configuration choices, you can generate (and handle) interrupts when:<br>
* * The PC underflows or overflows
* * The PC matches the PC compare value
* * The PC and RC match
* * The PC matches the PC compare value and the RC matches the PCRC compare value
* * The PC is inverted (changes direction)
* * The RC is out of range
* \section SectionQPRC_ConfigurationConsideration Configuration Consideration
* To set up a QPRC, you provide configuration parameters in the stc_qprc_config_t structure. 
* For example, you set the operating mode for the position counter: up/down count, 
* phase difference count, or directional count. You set the operating mode for the RC: ZIN 
* trigger mode, PC underflow/overflow mode, or both ZIN and PC underflow/overflow. You 
* specify the valid signal edege for each pin: rising, falling, or both.<br>
* After setting all the fields, call Qprc_Init() with the configuration structure and a 
* hardware pointer to the QPRC channel. <br>
* At runtime you use API function calls to:<br>
* * Enable or disable an interrupt
* * Stop, start, get, set or set the max value of the PC counter
* * Get the most recent direction of the PC counter (it was incremented or decremented)
* * Get or set the RC counter
* * Get or set the PC compare value
* * Get or set the PCRC compare value
* * Set the operating mode for either the PC or RC<br>
* When an enabled interrupt occurs, the interrupt flag is cleared and control passes to 
* the callback function. You can also use polling to manage interrupts. With polling mode,
* use Qprc_GetIrqFlag() to poll for an interrupt. Clear the interrupt flag with 
* Qprc_ClrIrqFlag().<br>
* To stop the QPRC, use Qprc_ConfigPcMode() and Qprc_ConfigRcMode() to set each to Mode 0 
*(disabled). Use QPRC_DisableIrq() to disable each QPRC interrupt.
* \section SectionQPRC_MoreInfo More Information
* For more information on the QPRC peripheral, refer to <br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">FM0+ Family 32-Bit Microcontroller Peripheral Manual Timer Part</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers?source=search&keywords=Peripheral%20Manual&cat=technical_documents">FM4 Family 32-Bit Microcontroller Peripheral Manual Timer Part</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* 
*/

/**
* \addtogroup GroupQPRC_Macros
* \{
*/

/******************************************************************************
 * Global definitions
 ******************************************************************************/
#define stc_qprcn_t     FM_QPRC_TypeDef

#define QPRC0       (*((volatile stc_qprcn_t *) FM_QPRC0_BASE))
#define QPRC1       (*((volatile stc_qprcn_t *) FM_QPRC1_BASE))
#define QPRC2       (*((volatile stc_qprcn_t *) FM_QPRC2_BASE))
#define QPRC3       (*((volatile stc_qprcn_t *) FM_QPRC3_BASE))

#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)  
#define stc_qprc_nfn_t  FM_QPRC_NF_TypeDef   
   
#define QPRCNF0       (*((volatile stc_qprc_nfn_t *) FM_QPRC0_NF_BASE))
#define QPRCNF1       (*((volatile stc_qprc_nfn_t *) FM_QPRC1_NF_BASE))
#define QPRCNF2       (*((volatile stc_qprc_nfn_t *) FM_QPRC2_NF_BASE))
#define QPRCNF3       (*((volatile stc_qprc_nfn_t *) FM_QPRC3_NF_BASE))
#endif   

#define QPRC_INSTANCE_COUNT     (PDL_PERIPHERAL_ENABLE_QPRC0 == PDL_ON ? 1u : 0u) + \
                                (PDL_PERIPHERAL_ENABLE_QPRC1 == PDL_ON ? 1u : 0u) + \
                                (PDL_PERIPHERAL_ENABLE_QPRC2 == PDL_ON ? 1u : 0u) + \
                                (PDL_PERIPHERAL_ENABLE_QPRC3 == PDL_ON ? 1u : 0u) 																

#define QPRC_PC_OVERFLOW_INT       0u
#define QPRC_PC_UNDERFLOW_INT      1u
#define QPRC_PC_ZERO_INDEX_INT     2u

/** \} GroupQPRC_Macros */
   
/**
* \addtogroup GroupQPRC_Types
* \{
*/

/**
 ******************************************************************************
 ** \brief Enumeration to define an index for each QPRC instance
 ******************************************************************************/
typedef enum en_qprc_instance_index
{
#if (PDL_PERIPHERAL_ENABLE_QPRC0 == PDL_ON)  
    QprcInstanceIndexQprc0 = 0u,  ///< Instance index of QPRC0
#endif
#if (PDL_PERIPHERAL_ENABLE_QPRC1 == PDL_ON) 
    QprcInstanceIndexQprc1,      ///< Instance index of QPRC1
#endif
#if (PDL_PERIPHERAL_ENABLE_QPRC2 == PDL_ON) 
    QprcInstanceIndexQprc2,      ///< Instance index of QPRC2
#endif
#if (PDL_PERIPHERAL_ENABLE_QPRC3 == PDL_ON) 
    QprcInstanceIndexQprc3,      ///< Instance index of QPRC2
#endif	
} en_qprc_instance_index_t;

/**
 ******************************************************************************
 ** \brief Enumeration to define position counter mode
 ******************************************************************************/
typedef enum en_qprc_pcmode
{
    QprcPcMode0 = 0u,  ///< PC_Mode0: Disable position counter
    QprcPcMode1 = 1u,  ///< PC_Mode1: Increments with AIN active edge and decrements with BIN active edge
    QprcPcMode2 = 2u,  ///< PC_Mode2: Phase difference count mode: Counts up if AIN is leading BIN, down if BIN leading.
    QprcPcMode3 = 3u,  ///< PC_Mode3: Directional count mode: Counts up/down with BIN active edge and AIN level
} en_qprc_pcmode_t;

/**
 ******************************************************************************
 ** \brief Enumeration to define revolution counter mode
 ******************************************************************************/
typedef enum en_qprc_rcmode
{
    QprcRcMode0 = 0u,  ///< RC_Mode0: Disable revolution counter
    QprcRcMode1 = 1u,  ///< RC_Mode1: Up/down count of revolution counter with ZIN active edge
    QprcRcMode2 = 2u,  ///< RC_Mode2: Up/down count of revolution counter on overflow or underflow in position count match
    QprcRcMode3 = 3u,  ///< RC_Mode3: Up/down count of revolution counter on overflow or underflow in position count match and ZIN active edge
} en_qprc_rcmode_t;

/**
 ******************************************************************************
 ** \brief Enumeration to define ZIN valid edge
 ******************************************************************************/
typedef enum en_qprc_zinedge
{
    QprcZinDisable     = 0u,  ///< Disables edge and level detection
    QprcZinFallingEdge = 1u,  ///< ZIN active at falling edge
    QprcZinRisingEdge  = 2u,  ///< ZIN active at rising edge
    QprcZinBothEdges   = 3u,  ///< ZIN active at falling or rising edge
    QprcZinLowLevel    = 4u,  ///< ZIN active at low level detected
    QprcZinHighLevel   = 5u,  ///< ZIN active at high level detected
} en_qprc_zinedge_t;

/**
 ******************************************************************************
 ** \brief Enumeration to define BIN valid edge
 ******************************************************************************/
typedef enum en_qprc_binedge
{
    QprcBinDisable     = 0u,  ///< Disables edge detection
    QprcBinFallingEdge = 1u,  ///< BIN active at falling edge
    QprcBinRisingEdge  = 2u,  ///< BIN active at rising edge
    QprcBinBothEdges   = 3u,  ///< BIN active at falling or rising edge
} en_qprc_binedge_t;

/**
 ******************************************************************************
 ** \brief Enumeration to define AIN valid edge
 ******************************************************************************/
typedef enum en_qprc_ainedge
{
    QprcAinDisable     = 0u,  ///< Disables edge detection
    QprcAinFallingEdge = 1u,  ///< AIN active at falling edge
    QprcAinRisingEdge  = 2u,  ///< AIN active at rising edge
    QprcAinBothEdges   = 3u,  ///< AIN active at falling or rising edge
} en_qprc_ainedge_t;

/**
 ******************************************************************************
 ** \brief Enumeration to define reset mask times of position counter
 ******************************************************************************/
typedef enum en_qprc_pcresetmask
{
    QprcResetMaskDisable  = 0u,  ///< No reset mask
    QprcResetMask2Times   = 1u,  ///< The position counter reset or a revolution counter count-up or -down events are ignored until the position counter changes twice
    QprcResetMask4Times   = 2u,  ///< The position counter reset or a revolution counter count-up or -down events are ignored until the position counter changes four times
    QprcResetMask8Times   = 3u,  ///< The position counter reset or a revolution counter count-up or -down events are ignored until the position counter changes eight times
} en_qprc_pcresetmask_t;

/**
 ******************************************************************************
 ** \brief Enumeration to define compare object of QPRCR register
 ******************************************************************************/
typedef enum en_qprc_compmode
{
    QprcCompareWithPosition   = 0u,  ///< Compares the value of the QPRC Position and Revolution Counter Compare Register (QPRCR) with that of the position counter.
    QprcCompareWithRevolution = 1u,  ///< Compares the value of the QPRC Position and Revolution Counter Compare Register (QPRCR) with that of the revolution counter.
} en_qprc_compmode_t;

/**
 ******************************************************************************
 ** \brief Enumeration to define QPRC interrupt type
 ******************************************************************************/
typedef enum en_qprc_irq_sel
{
    QprcPcOfUfZeroIrq       = 0u,    ///< Overflow, underflow, zero match interrupt of position counter
    QprcPcMatchIrq          = 1u,    ///< PC match interrupt of position counter
    QprcPcRcMatchIrq        = 2u,    ///< PC and RC match interrupt
    QprcPcMatchRcMatchIrq   = 3u,    ///< PC match and RC match interrupt
    QprcPcCountInvertIrq    = 4u,    ///< PC invert interrupt
    QprcRcOutrangeIrq       = 5u,    ///< RC outrange interrupt

}en_qprc_irq_sel_t;
/**
 ******************************************************************************
 ** \brief Enumeration to define QPRC filter width
 ******************************************************************************/
typedef enum en_qprc_filter_width
{
    QprcNoFilter             = 0u,     ///< No filter
    QprcFilterWidth4Pclk     = 1u,     ///< QPRC filter width: 4 PCLK
    QprcFilterWidth8Pclk     = 2u,     ///< QPRC filter width: 8 PCLK
    QprcFilterWidth16Pclk    = 3u,     ///< QPRC filter width: 16 PCLK
    QprcFilterWidth32Pclk    = 4u,     ///< QPRC filter width: 32 PCLK
    QprcFilterWidth64Pclk    = 5u,     ///< QPRC filter width: 64 PCLK
    QprcFilterWidth128Pclk   = 6u,     ///< QPRC filter width: 128 PCLK
    QprcFilterWidth256Pclk   = 7u,     ///< QPRC filter width: 256 PCLK

}en_qprc_filter_width_t;

/** \} GroupQPRC_Types */

/**
* \addtogroup GroupQPRC_DataStructures
* \{
*/

/**
 ******************************************************************************
 ** \brief Enumeration to define QPRC interrupt selection
 ******************************************************************************/
typedef struct stc_qprc_irq_en
{
    boolean_t    bQprcPcOfUfZeroIrq;      ///< Overflow, underflow, zero match interrupt of position counter, 1: selected, 0: not selected
    boolean_t    bQprcPcMatchIrq;         ///< PC match interrupt of position counter, 1: selected, 0: not selected
    boolean_t    bQprcPcRcMatchIrq;       ///< PC and RC match interrupt, 1: selected, 0: not selected
    boolean_t    bQprcPcMatchRcMatchIrq;  ///< PC match and RC match interrupt, 1: selected, 0: not selected
    boolean_t    bQprcPcCountInvertIrq;   ///< PC invert interrupt, 1: selected, 0: not selected
    boolean_t    bQprcRcOutrangeIrq;      ///< RC outrange interrupt, 1: selected, 0: not selected

}stc_qprc_irq_en_t;

/**
 ******************************************************************************
 ** \brief Enumeration to define QPRC interrupt callback function
 ******************************************************************************/
typedef struct stc_qprc_irq_cb
{
    func_ptr_arg1_t   pfnPcOfUfZeroIrqCb;      ///< Overflow, underflow, zero match interrupt callback function of position counter
    func_ptr_t        pfnPcMatchIrqCb;         ///< PC match interrupt callback function of position counter
    func_ptr_t        pfnPcRcMatchIrqCb;       ///< PC and RC match interrupt callback function
    func_ptr_t        pfnPcMatchRcMatchIrqCb;  ///< PC match and RC match interrupt callback function
    func_ptr_t        pfnPcCountInvertIrqCb;   ///< PC invert interrupt callback function
    func_ptr_t        pfnRcOutrangeIrqCb;      ///< RC outrange interrupt callback function

}stc_qprc_intern_data_t, stc_qprc_irq_cb_t;

/**
 ******************************************************************************
 ** \brief Structure to configuration QPRC filter
 ******************************************************************************/
typedef struct stc_qprc_filter
{
    boolean_t               bInputMask;     ///< Input mask setting
    boolean_t               bInputInvert;   ///< Input invert setting
    en_qprc_filter_width_t  enWidth;        ///< QPRC filter width setting

}stc_qprc_filter_t;

/**
 ******************************************************************************
 ** \brief Structure to configuration QPRC
 ******************************************************************************/
typedef struct stc_qprc_config
{
    boolean_t             bSwapAinBin;                    ///< TRUE: Swap AIN and BIN inputs
    en_qprc_compmode_t    enCompareMode;                  ///< Description see #en_qprc_compmode_t
    en_qprc_zinedge_t     enZinEdge;                      ///< Detection mode of the ZIN pin
    en_qprc_binedge_t     enBinEdge;                      ///< Detection mode of the BIN pin
    en_qprc_ainedge_t     enAinEdge;                      ///< Detection mode of the AIN pin
    en_qprc_pcresetmask_t enPcResetMask;                  ///< Position counter reset mask
    boolean_t             b8KValue;                       ///< TRUE: Outrange mode from 0 to 0x7FFF, FALSE: Outrange mode from 0 to 0xFFFF:
#if (PDL_MCU_TYPE >= PDL_FM4_TYPE3) 
    boolean_t             bPhaseEdge;                     ///< Only meaningful for 1 -time frequency multiplication of PC_Mode2. TRUE: The QPCR is counted up or down by both edge (rising edge and falling edge).FALSE: The QPCR is counted up or down by the same edge (rising edge or falling edge).
#endif    
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)
    stc_qprc_filter_t     stcAinFilter;                   ///< AIN noise filter configuration
    stc_qprc_filter_t     stcBinFilter;                   ///< BIN noise filter configuration
    stc_qprc_filter_t     stcCinFilter;                   ///< CIN noise filter configuration
#endif
    stc_qprc_irq_en_t*    pstcIrqEn;                      ///< Pointer to interrupt enable structure
    stc_qprc_irq_cb_t*    pstcIrqCb;                      ///< Pointer to interrupt callback function structure    
    boolean_t             bTouchNvic;                     ///< TRUE: enable NVIC, FALSE: don't enable NVIC
    
} stc_qprc_config_t;

/**
 ******************************************************************************
 ** \brief Structure of QPRC instance data
 ******************************************************************************/
typedef struct stc_qprc_instance_data
{
    volatile stc_qprcn_t*  pstcInstance;       ///< pointer to registers of an instance
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
    volatile stc_qprc_nfn_t* pstcInstanceNf;   ///< pointer to registers of a QPRC-NF instance
#endif    
    stc_qprc_intern_data_t stcInternData;      ///< module internal data of instance
} stc_qprc_instance_data_t;

/** \} GroupQPRC_DataStructures */

/**
* \addtogroup GroupQPRC_GlobalVariables
* \{
*/

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/

/// Look-up table for all enabled MFS instances and their internal data
extern stc_qprc_instance_data_t m_astcQprcInstanceDataLut[QPRC_INSTANCE_COUNT];

/** \} GroupQPRC_GlobalVariables */

/**
* \addtogroup GroupQPRC_Functions
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

/* QPRC init */
en_result_t Qprc_Init( volatile stc_qprcn_t* pstcQprc, const stc_qprc_config_t*  pstcConfig );
en_result_t Qprc_DeInit( volatile stc_qprcn_t* pstcQprc, boolean_t bTouchNvic );
/* Stop/Restart Pc Count */
en_result_t Qprc_StopPcCount(volatile stc_qprcn_t *pstcQprc);
en_result_t Qprc_RestartPcCount(volatile stc_qprcn_t *pstcQprc);
/* Count write/read */
en_result_t Qprc_SetPcCount ( volatile stc_qprcn_t* pstcQprc, uint16_t u16PcValue ) ;
uint16_t Qprc_GetPcCount ( volatile stc_qprcn_t* pstcQprc );
en_result_t Qprc_SetRcCount ( volatile stc_qprcn_t* pstcQprc, uint16_t u16RcValue );
uint16_t Qprc_GetRcCount ( volatile stc_qprcn_t* pstcQprc );
en_result_t Qprc_SetPcMaxValue( volatile stc_qprcn_t* pstcQprc, uint16_t u16PcMaxValue ) ;
uint16_t Qprc_GetPcMaxValue(volatile stc_qprcn_t *pstcQprc);
en_result_t Qprc_SetPcCompareValue( volatile stc_qprcn_t* pstcQprc, uint16_t u16PcValue ) ;
uint16_t Qprc_GetPcCompareValue( volatile stc_qprcn_t *pstcQprc);
en_result_t Qprc_SetPcRcCompareValue( volatile stc_qprcn_t* pstcQprc, uint16_t u16PcRcValue ) ;
uint16_t Qprc_GetPcRcCompareValue(volatile stc_qprcn_t *pstcQprc);
/* Mode configuration */
en_result_t Qprc_ConfigPcMode( volatile stc_qprcn_t* pstcQprc, en_qprc_pcmode_t enMode );
en_result_t Qprc_ConfigRcMode( volatile stc_qprcn_t* pstcQprc, en_qprc_rcmode_t enMode );
/* Interrupt configuration */
#if (PDL_INTERRUPT_ENABLE_QPRC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_QPRC1 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_QPRC2 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_QPRC3 == PDL_ON)
en_result_t Qprc_EnableIrq( volatile stc_qprcn_t* pstcQprc, en_qprc_irq_sel_t enIrqSel);
en_result_t Qprc_DisableIrq( volatile stc_qprcn_t* pstcQprc, en_qprc_irq_sel_t enIrqSel );
#endif
en_irq_flag_t Qprc_GetIrqFlag( volatile stc_qprcn_t* pstcQprc, en_qprc_irq_sel_t enIrqSel );
en_result_t Qprc_ClrIrqFlag( volatile stc_qprcn_t *pstcQprc, en_qprc_irq_sel_t enIrqSel );
/* status */
en_stat_flag_t Qprc_GetPcOfUfDir( volatile stc_qprcn_t* pstcQprc );
en_stat_flag_t Qprc_GetPcDir( volatile stc_qprcn_t* pstcQprc );
/* IRQ handler */
void Qprc_IrqHandler ( volatile stc_qprcn_t* pstcQprc,
                       stc_qprc_intern_data_t* pstcQprcInternData );

/** \} GroupQPRC_Functions */
/** \} GroupQPRC */

#ifdef __cplusplus
}
#endif



#endif // #if (defined(PDL_PERIPHERAL_QPRC_ACTIVE))

#endif /* __QPRC_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
