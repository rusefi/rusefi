/*******************************************************************************
* \file              icc.h
* 
* \version           1.20
*
* \brief             Headerfile for IC Card functions.
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

#ifndef __ICC_H__
#define __ICC_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_ICC_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \defgroup GroupICC Smart Card Interface (ICC)
* \{
* \defgroup GroupICC_Macros Macros
* \defgroup GroupICC_Functions Functions
* \defgroup GroupICC_GlobalVariables Global Variables
* \defgroup GroupICC_DataStructures Data Structures
* \defgroup GroupICC_Types Enumerated Types
* \}
*/

/**
* \addtogroup GroupICC
* \{
* The Smart Card Interface (ICC) peripheral supports communication with ISO 7816 smart cards. 
* Only asynchronous cards are supported. The peripheral has a parallel-to-serial and serial-to-parallel 
* converter with timer support. ICC supports 16-byte transmit and receive FIFO buffers.<br>
* The smart card pins are shared with GPIO pins. The physical smart card pins are configured 
* using the GPIO module. Please refer to the GPIO section for information on configuring the 
* physical pins of the smart card. <br>
* ICC features include: <br>
* * Support for ISO 7816-3
* * Card clock frequency adjustable up to 20 MHz
* * Programmable baud rate
* * Available protocols:
* - Transmitter: 8E2, 8O2, 8N2
* - Receiver: 8E1, 8O1, 8N2, 8N1, 9N1
* - Inverse mode
* * Resend option:
* - Transmitter: if receiver requests a resend, data will be sent again and interrupt is delayed
* - Receiver: if parity bit is wrong, block can request a resend
* * Inversion of output data is programmable
* * Card inserted or removed detection (used for interrupt generation)
* * Programmable guard time
* * FIFO size:
* - For receiver: 16-bytes
* - For transmitter: 16-bytes
* * Programmable idle timer (interrupt may occur when expired)
* * Interrupt controlled
* 
* \section SectionICC_ConfigurationConsideration Configuration Consideration
* To set up an ICC, you need to configure interrupts and the data FIFO (if you use the FIFO).
* You specify which interrupts to enable in an stc_icc_irq_en_t structure. Provide the required
* callback functions in an stc_icc_irq_cb_t structure. Similarly, if you use a FIFO data buffer, 
* set the fields in an stc_icc_fifo_config_t structure. The ICC config structure has fields for the 
* addresses of these structures. Use NULL if you do not use a feature.<br>
* To set up the ICC itself, you provide configuration parameters in the stc_icc_config_t structure. 
* For example, you set the clock divider, the baud rate, the transmit and receive data formats, and so on. 
* You also enable and set the level for a series of I/O pins.<br>
* Then call Icc_Init() andIcc_Enable(). Use Icc_DeInit() to reset all ICC related configuration registers.<br>
* While running, use API function calls to:<br>
* * Enable or disable selected interrupts
* * Change the baud rate
* * Send or receive data a word at a time, to either the ICC buffer or the FIFO buffer
* * Clear either the write or read FIFO buffer
* * Change pin levels
* * Get status and irq status
* For status and irq status, you pass in a constant identifying precisely what status you want. 
* See en_icc_status_t and en_icc_irq_sel_t for choices. See the ICC function documentation for 
* full information on the API.<br>
* \section SectionICC_MoreInfo More Information
* For more information on the ICC peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/223031/download">FM0+ Peripheral Manual - Communication Subsystem TRM.pdf</a><br>
* <a href="http://www.cypress.com/file/222996/download">FM4 Peripheral Manual - Communication Subsystem TRM.pdf</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/
 


/**
* \addtogroup GroupICC_Macros
* \{
*/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/
#define stc_iccn_t FM_SMCIF_TypeDef

#define ICC0       (*((volatile stc_iccn_t *) FM_SMCIF0_BASE))
#define ICC1       (*((volatile stc_iccn_t *) FM_SMCIF1_BASE))

#define ICC_INSTANCE_COUNT                                \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_ICC0 == PDL_ON) + \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_ICC1 == PDL_ON)
        
/** \} GroupICC_Macros */
  
/**
* \addtogroup GroupICC_Types
* \{
*/
  
/******************************************************************************
 * Global type definitions
 ******************************************************************************/
/**
 ******************************************************************************
 ** \brief ICC Idle timer source
 ******************************************************************************/
typedef enum en_icc_idletmrclk
{
    IccCardClk           = 0u,      ///< Idle timer is clocked by card clock(ICx_CLK)
    IccEtuClk            = 1u,      ///<  Idle timer is clocked by baud clock (ETU clock).
} en_icc_idletmrclk_t;

/**
 ******************************************************************************
 ** \brief ICC data direction
 ******************************************************************************/
typedef enum en_icc_data_dir
{
    IccDataLsbFirst = 0u,       ///< LSB first
    IccDataMsbFirst = 1u,       ///< MSB first
}en_icc_data_dir_t; 

/**
 ******************************************************************************
 ** \brief ICC transmit data format
 ******************************************************************************/
typedef enum en_icc_tx_data
{
    IccTx8Even2     = 0u,        ///< 8 bit with even parity and 2 stop bit
    IccTx8Odd2      = 1u,        ///< 8 bit with odd parity and 2 stop bit
    IccTx8None2     = 2u,        ///< 8 bit with none parity and 2 stop bit
}en_icc_tx_data_t; 

/**
 ******************************************************************************
 ** \brief ICC receive data format
 ******************************************************************************/
typedef enum en_icc_rx_data
{
    IccRx8Even1     = 0u,       ///< 8 bit with even parity and 2 stop bit
    IccRx8Odd1      = 1u,       ///< 8 bit with odd parity and 2 stop bit
    IccRx8None2     = 2u,       ///< 8 bit with none parity and 2 stop bit
    IccRx8None1     = 3u,       ///< 8 bit with none parity and 1 stop bit
    IccRx9None1     = 4u,       ///< 9 bit with none parity and 1 stop bit
}en_icc_rx_data_t; 

/**
 ******************************************************************************
 ** \brief ICC interrupt types
 ******************************************************************************/
typedef enum en_icc_irq_sel
{
    IccRxFullIrq          = 0u,             ///< Receive buffer full interrupt
    IccTxEmptyIrq         = 1u,             ///< Transmit buffer empty interrupt
    IccRxStartBitIrq      = 2u,             ///< Receive start bit detected interrupt
    IccCardEventIrq       = 3u,             ///< Card insert/pull out interrupt
    IccIdleTimerIrq       = 4u,             ///< Idle timer interrupt
    IccReadFifoIrq        = 5u,             ///< Receive FIFO interrupt
    IccWriteFifoIrq       = 6u,             ///< Transfer FIFO interrupt
    IccReadFifoOverrunIrq = 7u,             ///< Receive FIFO overrun interrupt

}en_icc_irq_sel_t;

/**
 ******************************************************************************
 ** \brief ICC status types
 ******************************************************************************/
typedef enum en_icc_status
{
    IccTxEmpty              = 0u,   ///< Transmit buffer empty
    IccRxFull               = 1u,   ///< Receive buffer full
    IccRxAction             = 2u,   ///< Receive is in action
    IccTxAction             = 3u,   ///< Transmit is in action
    IccCardDetect           = 4u,   ///< Card insertion is detected
    IccCardEvent            = 5u,   ///< Card event is detected
    IccReceiveOverflow      = 6u,   ///< Receive buffer overflow
    IccIdleTimerRun         = 7u,   ///< Idle timer running
    IccReadFifoOverrun      = 8u,   ///< Read FIFO overflow
    IccReadFifoFull         = 9u,   ///< Read FIFO full
    IccWriteFifoEmpty       = 10u,  ///< Write FIFO empty
    IccRxStartErr           = 11u,  ///< Receive start bit error
    IccTxResend             = 12u,  ///< Trasmit detect resend
    IccRxResend             = 13u,  ///< Receive detect resend
  
}en_icc_status_t;

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/
/// Enumeration to define an index for each enabled Icc instance
typedef enum en_icc_instance_index
{  
#if (PDL_PERIPHERAL_ENABLE_ICC0 == PDL_ON)  
    IccInstanceIndexIcc0,
#endif
#if (PDL_PERIPHERAL_ENABLE_ICC1 == PDL_ON)  
    IccInstanceIndexIcc1,
#endif
    IccInstanceIndexMax,
    IccInstanceIndexUnknown = 0xFFu,
} en_icc_instance_index_t;

/** \} GroupICC_Types */

/**
* \addtogroup GroupICC_DataStructures
* \{
*/

/**
 ******************************************************************************
 ** \brief ICC interrupt enable structure
 ******************************************************************************/
typedef struct stc_icc_irq_en
{
    boolean_t bRxFullIrq;               ///< Receive buffer full interrupt
    boolean_t bTxEmptyIrq;              ///< Transmit buffer empty interrupt
    boolean_t bRxStartBitIrq;           ///< Receive start bit detected interrupt
    boolean_t bCardEventIrq;            ///< Card insert/pull out interrupt
    boolean_t bIdleTimerIrq;            ///< Idle timer interrupt
    boolean_t bReadFifoIrq;             ///< Receive FIFO interrupt
    boolean_t bWriteFifoIrq;            ///< Transfer FIFO interrupt
    boolean_t bReadFifoOverrunIrq;      ///< Receive FIFO overrun interrupt

}stc_icc_irq_en_t;

/**
 ******************************************************************************
 ** \brief ICC interrupt callback function
 ******************************************************************************/
typedef struct stc_icc_irq_cb
{
    func_ptr_t pfnRxFullIrqCb;          ///< Receive buffer full interrupt callback function pointer
    func_ptr_t pfnTxEmptyIrqCb;         ///< Transmit buffer empty interrupt callback function pointer
    func_ptr_t pfnRxStartBitIrqCb;      ///< Receive start bit detected interrupt callback function pointer
    func_ptr_t pfnCardEventIrqCb;       ///< Card insert/pull out interrupt callback function pointer
    func_ptr_t pfnIdleTimerIrqCb;       ///< Idle timer interrupt callback function pointer
    func_ptr_t pfnReadFifoIrqCb;        ///< Receive FIFO interrupt callback function pointer
    func_ptr_t pfnWriteFifoIrqCb;       ///< Transfer FIFO interrupt callback function pointer
    func_ptr_t pfnReadFifoOverrunIrqCb; ///< Receive FIFO overrun interrupt callback function pointer
    
}stc_icc_irq_cb_t, stc_icc_intern_data_t;

/**
 ******************************************************************************
 ** \brief ICC FIFO configuration.
 ******************************************************************************/
typedef struct stc_icc_fifo_config
{
    uint8_t     TxFifoLevel;                ///< Transfer data FIFO level
    uint8_t     RxFifoLevel;                ///< Receive data FIFO level
} stc_icc_fifo_config_t;

/**
 ******************************************************************************
 ** \brief ICC configuration structure
 ******************************************************************************/
typedef struct stc_icc_config
{
    uint16_t                u16ClkDiv;          ///< Clock division of card clock. (Card clock = PCLK/u16ClkDiv)
    uint16_t                u16BaudRate;        ///< Baud rate value, which is F/D. (1 ETU = (F/D) * (1/CardClock[Hz]))
    en_icc_tx_data_t        enTxDataFormat;     ///< Transmit data format
    en_icc_rx_data_t        enRxDataFormat;     ///< Receive data format
    boolean_t               bUseGuardTimer;     ///< TRUE: enable guard timer, FALSE: disable guard timer.
    uint8_t                 u8GuardTime;        ///< Guard time
    en_icc_idletmrclk_t     enIdleTimerSrcClk;  ///< Source clock of idle timer   
    uint16_t                u16IdleTimerCnt;    ///< Idle timer count
    en_icc_data_dir_t       enBitDirection;     ///< ICC data direction
    boolean_t               bInvertData;        ///< Inversion of the output data
    boolean_t               bDataResend;        ///< TRUE: data resend enabled, FALSE: data resend disabled
    boolean_t               bClkPinMode;        ///< TRUE: ICx_CLK pin controlled by software, FALSE: ICx_CLK pin controlled by hardware
    boolean_t               bDataPinMode;       ///< TRUE: ICx_DATA pin controlled by software, FALSE: ICx_DATA pin controlled by hardware
    boolean_t               bVpenPinOutputEn;   ///< TRUE: enable ICx_VPEN output, FALSE: dsiable ICx_VPEN output
    boolean_t               bVccPinOutputEn;    ///< TRUE: enable ICx_VCC output, FALSE: dsiable ICx_VCC output
    boolean_t               bRstPinOutputEn;    ///< TRUE: enable ICx_RST output, FALSE: dsiable ICx_RST output
    boolean_t               bDataPinOutputEn;   ///< TRUE: enable ICx_DATA output, FALSE: dsiable ICx_DATA output
    boolean_t               bClkPinOutputEn;    ///< TRUE: enable ICx_CLK output, FALSE: dsiable ICx_CLK output
    boolean_t               bVpenPinLevel;      ///< ICx_VPEN Pin level (Only valid when output is enabled)
    boolean_t               bVccPinLevel;       ///< ICx_VCC Pin level (Only valid when output is enabled)
    boolean_t               bRstPinLevel;       ///< ICx_RST Pin level (Only valid when output is enabled)
    boolean_t               bDataPinLevel;      ///< ICx_DATA Pin level (Only valid when output is enabled and Data pin mode is set to software control)
    boolean_t               bClkPinLevel;       ///< ICx_CLK Pin level (Only valid when output is enabled and Clock pin mode is set to software control)
    stc_icc_fifo_config_t   *pstcFifoConfig;    ///< Pointer to FIFO configuration structure, FIFO is not used when set to NULL
    
#if (PDL_INTERRUPT_ENABLE_ICC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_ICC1 == PDL_ON)
    stc_icc_irq_en_t        *pstcIrqEn;         ///< Pointer to ICC interrupt enable structure, if set to NULL, no interrupt enabled.
    stc_icc_irq_cb_t        *pstcIrqCb;         ///< Pointer to ICC interrupt callback functions structurei, f set to NULL, no interrupt callback initialized.
    boolean_t               bTouchNvic;         ///< TRUE: enable NVIC, FALSE: don't enable NVIC
#endif

} stc_icc_config_t;

/// ICC instance data type
typedef struct stc_icc_instance_data
{
    volatile stc_iccn_t*   pstcInstance;     ///< pointer to registers of an instance
    stc_icc_intern_data_t stcInternData;     ///< module internal data of instance
} stc_icc_instance_data_t;

/** \} GroupICC_DataStructures */

/**
* \addtogroup GroupICC_GlobalVariables
* \{
*/

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/
/// Look-up table for all enabled Icc instances and their internal data
extern stc_icc_instance_data_t m_astcIccInstanceDataLut[ICC_INSTANCE_COUNT];

/** \} GroupICC_GlobalVariables */

/**
* \addtogroup GroupICC_Functions
* \{
*/

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/
#if (PDL_INTERRUPT_ENABLE_ICC0 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_ICC1 == PDL_ON)
// Interrupt
void Icc_IrqHandler(volatile stc_iccn_t*   pstcIcc, 
                    stc_icc_intern_data_t* pstcIccInternData);
en_result_t Icc_EnableIrq(volatile stc_iccn_t* pstcIcc, 
                             en_icc_irq_sel_t enIrqSel);
en_result_t Icc_DisableIrq(volatile stc_iccn_t* pstcIcc, 
                             en_icc_irq_sel_t enIrqSel);
#endif

// Init/De-Init
en_result_t Icc_Init(volatile stc_iccn_t*  pstcIcc,
                     const stc_icc_config_t* pstcConfig);
en_result_t Icc_DeInit(volatile stc_iccn_t* pstcIcc, 
                         boolean_t bTouchNvic);

// Function enable/disable
en_result_t Icc_Enable(volatile stc_iccn_t* pstcIcc);
en_result_t Icc_Disable(volatile stc_iccn_t* pstcIcc);

// Baud rate
en_result_t Icc_SetBaudRate(volatile stc_iccn_t* pstcIcc,
                            uint16_t u16BaudRate);

// Status read
boolean_t Icc_GetStatus(volatile stc_iccn_t* pstcIcc, 
                        en_icc_status_t enStatus);
boolean_t Icc_GetIrqStatus(volatile stc_iccn_t* pstcIcc, 
                           en_icc_irq_sel_t enIrqSel);

// Data read/write
en_result_t Icc_SendData(volatile stc_iccn_t* pstcIcc, uint16_t Data);
en_result_t Icc_SendFifoData(volatile stc_iccn_t* pstcIcc, uint16_t Data);
uint16_t Icc_ReceiveData(volatile stc_iccn_t* pstcIcc);
uint16_t Icc_ReceiveFifoData(volatile stc_iccn_t* pstcIcc);

// FIFO 
en_result_t Icc_ClearWriteFifo (volatile stc_iccn_t* pstcIcc);
en_result_t Icc_ClearReadFifo (volatile stc_iccn_t* pstcIcc);
en_result_t Icc_SetWriteFifoLevel(volatile stc_iccn_t* pstcIcc, uint8_t u8Level);
en_result_t Icc_SetReadFifoLevel(volatile stc_iccn_t* pstcIcc, uint8_t u8Level);
uint8_t Icc_GetWriteFifoCurCnt(volatile stc_iccn_t* pstcIcc);
uint8_t Icc_GetReadFifoCurCnt(volatile stc_iccn_t* pstcIcc);

// IO
en_result_t Icc_SetVpenPinLevel (volatile stc_iccn_t* pstcIcc, 
                                 boolean_t bLevel);
en_result_t Icc_SetVccPinLevel (volatile stc_iccn_t* pstcIcc, 
                                boolean_t bLevel);
en_result_t Icc_SetRstPinLevel (volatile stc_iccn_t* pstcIcc, 
                                boolean_t bLevel);
en_result_t Icc_SetDataPinLevel (volatile stc_iccn_t* pstcIcc, 
                                 boolean_t bLevel);
en_result_t Icc_SetClkPinLevel (volatile stc_iccn_t* pstcIcc, 
                                boolean_t bLevel);

// Idle timer
en_result_t Icc_SetIdleTimerCnt(volatile stc_iccn_t* pstcIcc, uint16_t u16Cnt);
en_result_t Icc_StartIdleTimer(volatile stc_iccn_t* pstcIcc);
en_result_t Icc_StopIdleTimer(volatile stc_iccn_t* pstcIcc);

/** \} GroupICC_Functions */
/** \} GroupICC */

#ifdef __cplusplus
}
#endif

#endif /* #if (defined(PDL_PERIPHERAL_ICC_ACTIVE)) */

#endif /* __ICC_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
