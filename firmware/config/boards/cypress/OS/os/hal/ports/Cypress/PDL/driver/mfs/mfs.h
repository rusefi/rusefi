/*******************************************************************************
* \file              mfs.h
* 
* \version           1.20
*
* \brief             Headerfile for MFS functions.
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

#ifndef __MFS_H__
#define __MFS_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_MFS_ACTIVE))
  
#if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_OFF) && \
    (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_OFF) && \
    (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_OFF) && \
    (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_OFF)
  #error "please enable at least one MFS Mode in pdl_user.h"
#endif
      
/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \defgroup GroupMFS Multi-Functional Serial interface (MFS)
* \{ 
* \defgroup GroupMFS_Macros Macros
* \defgroup GroupMFS_Functions Functions
* \defgroup GroupMFS_GlobalVariables Global Variables
* \defgroup GroupMFS_DataStructures Data Structures
* \defgroup GroupMFS_Types Enumerated Types
* \}
*/

/**
* \addtogroup GroupMFS
* \{
* The Multi-function Serial (MFS) peripheral allows you to implement several 
* communication protocols. You can configure the MFS to implement the following 
* serial communication interfaces:
* * UART
* * CSIO (supports SPI and I2S)
* * I2C
* * LIN
* The FM family microcontrollers and the PDL support up to 16 MFS channels. 
* The number of available channels varies among processor series. You configure the 
* individual channels independently. This means that you can implement an arbitrary mix 
* of communication protocols in your firmware.<br>
* 
* <b> UART </b><br>
* The Universal Asynchronous Receiver/Transmitter (UART) has the following features:<br>
* * Full duplex double buffer
* * Transmit/receive FIFO (up to 128 bytes each)
* * Parity can be enabled or disabled.
* * Built-in dedicated baud rate generator
* * External clock available as a serial clock
* * Supports bidirectional communications (normal mode)
* * Supports master/slave communications (multi-processor mode)
* * Various error detection functions (parity errors, framing errors, and 
* overrun errors)
*
* <b> CSIO</b><br>
* The  Clock Synchronous Serial Interface (CSIO) supports both SPI and I2S 
* communication. It has the following features:
* * Full duplex double buffer
* * Transmit/receive FIFO (up to 128 bytes each)
* * Dedicated baud rate generator
* * Overrun error detection function
* * Serial chip select function (ch1 and ch3 only)
* * Data length: 5 to 16 bits
*
* <b> I2C </b><br>
* The Inter-Integrated Circuit (I2C) interface   mode enables communication via the I2C protocol. It
* operates as a master/slave device on the I2C bus. I2C mode has these features:
* * Full duplex double buffer
* * Transmit/receive FIFO (up to 128 bytes each)
* * Dedicated baud rate generator
* * Supports Standard-mode (max 100 kbps) and Fast-mode (max 400 kbps)
* * Synchronous communication
* * Data length: 8 bits
*
* <b> LIN </b><br> 
* The Local Interconnect Network (LIN) interface mode enables communication via the LIN protocol. <br>
* LIN mode has the following features:
* * Supports LIN protocol version.2.1 
* * Full duplex double buffer
* * Transmit/receive FIFO (up to 128 bytes each)
* * Supports Master/Slave mode 
* * LIN break field generation function (The length is variable between 13 bits 
* and 16 bits)
* * LIN break delimiter generation function (The length is variable between 1 
* bit and 4 bits)
* * Various error detection functions available (parity errors, framing errors, 
* and overrun errors)
*
* \section SectionMFS_ConfigurationConsideration Configuration Consideration
* To set up an MFS instance you provide configuration parameters in the configuration 
* structure of the type stc_mfs_xxx_config_t. Then call Mfs_Xxx_Init(). Where xxx is one of 
* uart, i2c, csio, or lin. In the Mfs_xxx_Init() call you provide the base hardware address 
* for the MFS channel (0-15) you are using (e.g. &UART7, or &CSIO9). These constants are defined 
* in the mfs.h file.<br>
* After initializing, you must call Mfs_Xxx_EnableFunc() to enable transmit or receive. Use the 
* correct enumerated constant for your mode of operation and function. For example:
* * Mfs_Uart_EnableFunc(&UART7, UartTx);
* * Mfs_Csio_EnableFunc(&CSIO9, CsioRx);
* 
* \section SectionMFS_MoreInfo More Information
* For more information on the MFS peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/223031/download">FM0+ Peripheral Manual - Communication Subsystem TRM.pdf</a><br>
* <a href="http://www.cypress.com/file/222976/download">FM4 Peripheral Manual - Communication Subsystem TRM.pdf</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm0-family-32-bit-arm-cortex-m0-microcontrollers">all 
* FM0+ Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/**
* \addtogroup GroupMFS_Macros
* \{
*/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/
#if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON) 
  #define stc_mfsn_uart_t FM_MFS_UART_TypeDef 

  #define UART0       (*((volatile stc_mfsn_uart_t *) FM_MFS0_UART_BASE))
  #define UART1       (*((volatile stc_mfsn_uart_t *) FM_MFS1_UART_BASE))
  #define UART2       (*((volatile stc_mfsn_uart_t *) FM_MFS2_UART_BASE))
  #define UART3       (*((volatile stc_mfsn_uart_t *) FM_MFS3_UART_BASE))
  #define UART4       (*((volatile stc_mfsn_uart_t *) FM_MFS4_UART_BASE))
  #define UART5       (*((volatile stc_mfsn_uart_t *) FM_MFS5_UART_BASE))
  #define UART6       (*((volatile stc_mfsn_uart_t *) FM_MFS6_UART_BASE))
  #define UART7       (*((volatile stc_mfsn_uart_t *) FM_MFS7_UART_BASE))
  #define UART8       (*((volatile stc_mfsn_uart_t *) FM_MFS8_UART_BASE))
  #define UART9       (*((volatile stc_mfsn_uart_t *) FM_MFS9_UART_BASE))
  #define UART10      (*((volatile stc_mfsn_uart_t *) FM_MFS10_UART_BASE))
  #define UART11      (*((volatile stc_mfsn_uart_t *) FM_MFS11_UART_BASE))
  #define UART12      (*((volatile stc_mfsn_uart_t *) FM_MFS12_UART_BASE))
  #define UART13      (*((volatile stc_mfsn_uart_t *) FM_MFS13_UART_BASE))
  #define UART14      (*((volatile stc_mfsn_uart_t *) FM_MFS14_UART_BASE))
  #define UART15      (*((volatile stc_mfsn_uart_t *) FM_MFS15_UART_BASE))
#endif
  
#if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON)   
  #define stc_mfsn_csio_t FM_MFS_CSIO_TypeDef 

  #define CSIO0       (*((volatile stc_mfsn_csio_t *) FM_MFS0_CSIO_BASE))
  #define CSIO1       (*((volatile stc_mfsn_csio_t *) FM_MFS1_CSIO_BASE))
  #define CSIO2       (*((volatile stc_mfsn_csio_t *) FM_MFS2_CSIO_BASE))
  #define CSIO3       (*((volatile stc_mfsn_csio_t *) FM_MFS3_CSIO_BASE))
  #define CSIO4       (*((volatile stc_mfsn_csio_t *) FM_MFS4_CSIO_BASE))
  #define CSIO5       (*((volatile stc_mfsn_csio_t *) FM_MFS5_CSIO_BASE))
  #define CSIO6       (*((volatile stc_mfsn_csio_t *) FM_MFS6_CSIO_BASE))
  #define CSIO7       (*((volatile stc_mfsn_csio_t *) FM_MFS7_CSIO_BASE))
  #define CSIO8       (*((volatile stc_mfsn_csio_t *) FM_MFS8_CSIO_BASE))
  #define CSIO9       (*((volatile stc_mfsn_csio_t *) FM_MFS9_CSIO_BASE))
  #define CSIO10      (*((volatile stc_mfsn_csio_t *) FM_MFS10_CSIO_BASE))
  #define CSIO11      (*((volatile stc_mfsn_csio_t *) FM_MFS11_CSIO_BASE))
  #define CSIO12      (*((volatile stc_mfsn_csio_t *) FM_MFS12_CSIO_BASE))
  #define CSIO13      (*((volatile stc_mfsn_csio_t *) FM_MFS13_CSIO_BASE))
  #define CSIO14      (*((volatile stc_mfsn_csio_t *) FM_MFS14_CSIO_BASE))
  #define CSIO15      (*((volatile stc_mfsn_csio_t *) FM_MFS15_CSIO_BASE))
#endif

#if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON) 
  #define stc_mfsn_i2c_t  FM_MFS_I2C_TypeDef  

  #define I2C0       (*((volatile stc_mfsn_i2c_t *) FM_MFS0_I2C_BASE))
  #define I2C1       (*((volatile stc_mfsn_i2c_t *) FM_MFS1_I2C_BASE))
  #define I2C2       (*((volatile stc_mfsn_i2c_t *) FM_MFS2_I2C_BASE))
  #define I2C3       (*((volatile stc_mfsn_i2c_t *) FM_MFS3_I2C_BASE))
  #define I2C4       (*((volatile stc_mfsn_i2c_t *) FM_MFS4_I2C_BASE))
  #define I2C5       (*((volatile stc_mfsn_i2c_t *) FM_MFS5_I2C_BASE))
  #define I2C6       (*((volatile stc_mfsn_i2c_t *) FM_MFS6_I2C_BASE))
  #define I2C7       (*((volatile stc_mfsn_i2c_t *) FM_MFS7_I2C_BASE))
  #define I2C8       (*((volatile stc_mfsn_i2c_t *) FM_MFS8_I2C_BASE))
  #define I2C9       (*((volatile stc_mfsn_i2c_t *) FM_MFS9_I2C_BASE))
  #define I2C10      (*((volatile stc_mfsn_i2c_t *) FM_MFS10_I2C_BASE))
  #define I2C11      (*((volatile stc_mfsn_i2c_t *) FM_MFS11_I2C_BASE))
  #define I2C12      (*((volatile stc_mfsn_i2c_t *) FM_MFS12_I2C_BASE))
  #define I2C13      (*((volatile stc_mfsn_i2c_t *) FM_MFS13_I2C_BASE))
  #define I2C14      (*((volatile stc_mfsn_i2c_t *) FM_MFS14_I2C_BASE))
  #define I2C15      (*((volatile stc_mfsn_i2c_t *) FM_MFS15_I2C_BASE))
#endif

#if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON)
  #define stc_mfsn_lin_t  FM_MFS_LIN_TypeDef  

  #define LIN0       (*((volatile stc_mfsn_lin_t *) FM_MFS0_LIN_BASE))
  #define LIN1       (*((volatile stc_mfsn_lin_t *) FM_MFS1_LIN_BASE))
  #define LIN2       (*((volatile stc_mfsn_lin_t *) FM_MFS2_LIN_BASE))
  #define LIN3       (*((volatile stc_mfsn_lin_t *) FM_MFS3_LIN_BASE))
  #define LIN4       (*((volatile stc_mfsn_lin_t *) FM_MFS4_LIN_BASE))
  #define LIN5       (*((volatile stc_mfsn_lin_t *) FM_MFS5_LIN_BASE))
  #define LIN6       (*((volatile stc_mfsn_lin_t *) FM_MFS6_LIN_BASE))
  #define LIN7       (*((volatile stc_mfsn_lin_t *) FM_MFS7_LIN_BASE))
  #define LIN8       (*((volatile stc_mfsn_lin_t *) FM_MFS8_LIN_BASE))
  #define LIN9       (*((volatile stc_mfsn_lin_t *) FM_MFS9_LIN_BASE))
  #define LIN10      (*((volatile stc_mfsn_lin_t *) FM_MFS10_LIN_BASE))
  #define LIN11      (*((volatile stc_mfsn_lin_t *) FM_MFS11_LIN_BASE))
  #define LIN12      (*((volatile stc_mfsn_lin_t *) FM_MFS12_LIN_BASE))
  #define LIN13      (*((volatile stc_mfsn_lin_t *) FM_MFS13_LIN_BASE))
  #define LIN14      (*((volatile stc_mfsn_lin_t *) FM_MFS14_LIN_BASE))
  #define LIN15      (*((volatile stc_mfsn_lin_t *) FM_MFS15_LIN_BASE))
#endif
  
  
#define MFS_INSTANCE_COUNT                                \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_MFS0 == PDL_ON) + \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_MFS1 == PDL_ON) + \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_MFS2 == PDL_ON) + \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_MFS3 == PDL_ON) + \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_MFS4 == PDL_ON) + \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_MFS5 == PDL_ON) + \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_MFS6 == PDL_ON) + \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_MFS7 == PDL_ON) + \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_MFS8 == PDL_ON) + \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_MFS9 == PDL_ON) + \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_MFS10 == PDL_ON) + \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_MFS11 == PDL_ON) + \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_MFS12 == PDL_ON) + \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_MFS13 == PDL_ON) + \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_MFS14 == PDL_ON) + \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_MFS15 == PDL_ON)  
  
#define MFS0_DATA_REG_ADDR   (uint32_t)(&FM_MFS0_UART->TDR)
#define MFS1_DATA_REG_ADDR   (uint32_t)(&FM_MFS1_UART->TDR)
#define MFS2_DATA_REG_ADDR   (uint32_t)(&FM_MFS2_UART->TDR)
#define MFS3_DATA_REG_ADDR   (uint32_t)(&FM_MFS3_UART->TDR)
#define MFS4_DATA_REG_ADDR   (uint32_t)(&FM_MFS4_UART->TDR)
#define MFS5_DATA_REG_ADDR   (uint32_t)(&FM_MFS5_UART->TDR)
#define MFS6_DATA_REG_ADDR   (uint32_t)(&FM_MFS6_UART->TDR)
#define MFS7_DATA_REG_ADDR   (uint32_t)(&FM_MFS7_UART->TDR)
#define MFS8_DATA_REG_ADDR   (uint32_t)(&FM_MFS8_UART->TDR)
#define MFS9_DATA_REG_ADDR   (uint32_t)(&FM_MFS9_UART->TDR)
#define MFS10_DATA_REG_ADDR  (uint32_t)(&FM_MFS10_UART->TDR)
#define MFS11_DATA_REG_ADDR  (uint32_t)(&FM_MFS11_UART->TDR)
#define MFS12_DATA_REG_ADDR  (uint32_t)(&FM_MFS12_UART->TDR)
#define MFS13_DATA_REG_ADDR  (uint32_t)(&FM_MFS13_UART->TDR)
#define MFS14_DATA_REG_ADDR  (uint32_t)(&FM_MFS14_UART->TDR)
#define MFS15_DATA_REG_ADDR  (uint32_t)(&FM_MFS15_UART->TDR)  
  
/** \} GroupMFS_Macros */
  
/**
* \addtogroup GroupMFS_Types
* \{
*/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/

/******************************************************************************
 * MFS FIFO type definitions
 ******************************************************************************/
 
/**
 ******************************************************************************
 ** \brief Mfs FIFO Selection
 ******************************************************************************/
typedef enum en_mfs_fifo_sel
{
    MfsTxFifo1RxFifo2 = 0u,          ///< Transmit FIFO:FIFO1, Received FIFO:FIFO2
    MfsTxFifo2RxFifo1 = 1u,          ///< Transmit FIFO:FIFO2, Received FIFO:FIFO1
} en_mfs_fifo_sel_t;

/**
 ******************************************************************************
 ** \brief Mfs FIFO Number
 ******************************************************************************/
typedef enum en_mfs_fifo
{
    MfsFifo1 = 0u,                   ///< FIFO No.1
    MfsFifo2 = 1u,                   ///< FIFO No.2
} en_mfs_fifo_t;  
  
/******************************************************************************
 * UART type definitions
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief MFS UART mode
 ******************************************************************************/
typedef enum en_mfs_uart_mode
{
    UartNormal = 0u,          ///< Normal mode
    UartMulti  = 1u,          ///< Multi-Processor Mode
} en_uart_mode_t;

/**
 ******************************************************************************
 ** \brief UART data length
 ******************************************************************************/
typedef enum en_uart_data_len
{
    UartEightBits      = 0u,      ///<  8 Bit character length
    UartFiveBits       = 1u,      ///<  5 Bit character length
    UartSixBits        = 2u,      ///<  6 Bit character length
    UartSevenBits      = 3u,      ///<  7 Bit character length
    UartNineBits       = 4u,      ///<  9 Bit character length
} en_uart_data_len_t;

/**
 ******************************************************************************
 ** \brief UART parity format
 ******************************************************************************/
typedef enum en_uart_parity
{
    UartParityNone  = 0u,         ///< No parity bit is used.
    UartParityEven  = 2u,         ///< Even parity bit is used.
    UartParityOdd   = 3u,         ///< Odd parity bit is used.
} en_uart_parity_t;

/**
 ******************************************************************************
 ** \brief UART stop bits length
 ******************************************************************************/
typedef enum en_uart_stop_bit
{
    UartOneStopBit    = 0u,       ///< 1 Stop Bit
    UartTwoStopBits   = 1u,       ///< 2 Stop Bits
    UartThreeStopBits = 2u,       ///< 3 Stop Bits
    UartFourStopBits  = 3u,       ///< 4 Stop Bits
} en_uart_stop_bit_t;

/**
 ******************************************************************************
 ** \brief UART data direction
 ******************************************************************************/
typedef enum en_uart_data_dir
{
    UartDataLsbFirst = 0u,       ///< LSB first
    UartDataMsbFirst = 1u,       ///< MSB first
}en_uart_data_dir_t; 

/**
 ******************************************************************************
 ** \brief UART functions
 ******************************************************************************/
typedef enum en_uart_func
{
    UartTx = 0u,         ///< UART TX
    UartRx = 1u,         ///< UART RX
  
}en_uart_func_t;

/**
 ******************************************************************************
 ** \brief UART interrupt selection
 ******************************************************************************/
typedef enum en_uart_irq_sel
{
    UartTxIrq       = 0u,          ///< UART TX interrupt
    UartRxIrq       = 1u,          ///< UART RX interrupt
    UartTxIdleIrq   = 2u,          ///< UART TX idle interrupt
    UartTxFifoIrq   = 3u,          ///< UART TX FIFO interrupt
    
}en_uart_irq_sel_t;

/**
 ******************************************************************************
 ** \brief UART status types
 ******************************************************************************/
typedef enum en_uart_status
{
    UartParityError     = 0u,   ///< Parity error
    UartFrameError      = 1u,   ///< Frame error
    UartOverrunError    = 2u,   ///< Overrun error
    UartRxFull          = 3u,   ///< RX completion
    UartTxEmpty         = 4u,   ///< TX buffer empty
    UartTxIdle          = 5u,   ///< TX idle
    UartTxFifoRequest   = 6u,   ///< TX FIFO request
  
}en_uart_status_t;

/******************************************************************************
 * CSIO type definitions
 ******************************************************************************/
 
/**
 ******************************************************************************
 ** \brief CSIO mode (Master/Slave)
 ******************************************************************************/
typedef enum en_csio_ms_mode
{
    CsioMaster = 0,          ///< Master mode (generating serial clock)
    CsioSlave  = 1           ///< Slave mode  (external serial clock)
} en_csio_ms_mode_t;

/**
 ******************************************************************************
 ** \brief CSIO active mode (Normal/SPI)
 ******************************************************************************/
typedef enum en_csio_act_mode
{
    CsioActNormalMode = 0u,   ///< Normal mode
    CsioActSpiMode    = 1u,   ///< SPI mode
} en_csio_act_mode_t;

/**
 ******************************************************************************
 ** \brief CSIO data length
 ******************************************************************************/
typedef enum en_csio_data_len
{
    CsioFiveBits       = 0u,      ///<  5 Bit character length
    CsioSixBits        = 1u,      ///<  6 Bit character length
    CsioSevenBits      = 2u,      ///<  7 Bit character length
    CsioEightBits      = 3u,      ///<  8 Bit character length
    CsioNineBits       = 4u,      ///<  9 Bit character length
#if (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)   
    CsioTenBits        = 5u,      ///< 10 Bit character length
    CsioElevenBits     = 6u,      ///< 11 Bit character length
    CsioTwelveBits     = 7u,      ///< 12 Bit character length
    CsioThirteenBits   = 8u,      ///< 13 Bit character length
    CsioFourteenBits   = 9u,      ///< 14 Bit character length
    CsioFifteenBits    = 10u,     ///< 15 Bit character length
    CsioSixteenBits    = 11u,     ///< 16 Bit character length
#endif    
} en_csio_data_len_t;

/**
 ******************************************************************************
 ** \brief CSIO synchronous wait time
 ******************************************************************************/
typedef enum en_csio_sync_wait_time
{
    CsioSyncWaitZero  = 0u,           ///< 0 wait time insertion
    CsioSyncWaitOne   = 1u,           ///< 1 wait time insertion
    CsioSyncWaitTwo   = 2u,           ///< 2 wait time insertion
    CsioSyncWaitThree = 3u,           ///< 3 wait time insertion
} en_csio_sync_wait_time_t;

/**
 ******************************************************************************
 ** \brief CSIO bit direction
 ******************************************************************************/
typedef enum en_csio_data_dir
{
    CsioDataLsbFirst = 0u,       ///< LSB first
    CsioDataMsbFirst = 1u,       ///< MSB first
}en_csio_data_dir_t; 

/**
 ******************************************************************************
 ** \brief CSIO serial timer clock division
 ******************************************************************************/
typedef enum en_csio_timer_clk
{
    CsioTimerNoDiv  = 0u,     ///< Serial Timer clock = PCLK
    CsioTimerDiv2   = 1u,     ///< Serial Timer clock = PCLK/2 
    CsioTimerDiv4   = 2u,     ///< Serial Timer clock = PCLK/4
    CsioTimerDiv8   = 3u,     ///< Serial Timer clock = PCLK/8
    CsioTimerDiv16  = 4u,     ///< Serial Timer clock = PCLK/16
    CsioTimerDiv32  = 5u,     ///< Serial Timer clock = PCLK/32
    CsioTimerDiv64  = 6u,     ///< Serial Timer clock = PCLK/64
    CsioTimerDiv128 = 7u,     ///< Serial Timer clock = PCLK/128
    CsioTimerDiv256 = 8u,     ///< Serial Timer clock = PCLK/256 
     
}en_csio_timer_clk_t;

/**
 ******************************************************************************
 ** \brief CSIO chip selection pin
 ******************************************************************************/
typedef enum en_cs_pin_sel
{
    CsPinScs0 = 0u,    ///< Use SCS0 as chip selection pin
    CsPinScs1 = 1u,    ///< Use SCS1 as chip selection pin
    CsPinScs2 = 2u,    ///< Use SCS2 as chip selection pin
    CsPinScs3 = 3u,    ///< Use SCS3 as chip selection pin
  
}en_cs_pin_sel_t;

/**
 ******************************************************************************
 ** \brief CSIO chip selection pin level
 ** \note It is only available for SCS0 pin
 ******************************************************************************/
typedef enum en_cs_pin_level
{
    CsLowActive   = 0u,   ///< Set high as active level for SCS0 pin
    CsHighActive  = 1u,   ///< Set low as active level for SCS0 pin
  
}en_cs_pin_level_t;

/**
 ******************************************************************************
 ** \brief CSIO chip selection clock division
 ******************************************************************************/
typedef enum en_cs_timing_clk
{
    CsClkNoDiv = 0u,   ///< Chip selection clock = PCLK
    CsClkDiv2  = 1u,   ///< Chip selection clock = PCLK/2
    CsClkDiv4  = 2u,   ///< Chip selection clock = PCLK/4
    CsClkDiv8  = 3u,   ///< Chip selection clock = PCLK/8
    CsClkDiv16 = 4u,   ///< Chip selection clock = PCLK/16
    CsClkDiv32 = 5u,   ///< Chip selection clock = PCLK/32
    CsClkDiv64 = 6u,   ///< Chip selection clock = PCLK/64
  
}en_cs_timing_clk_t;

/**
 ******************************************************************************
 ** \brief CSIO function
 ******************************************************************************/
typedef enum en_csio_func
{
    CsioTx          = 0u,    ///< CSIO TX
    CsioRx          = 1u,    ///< CSIO RX 
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
    CsioSerialTimer = 2u,    ///< CSIO Serial TImer 
    CsioCsErrOccur  = 3u,    ///< CSIO chip selection error occurrance
#endif  
}en_csio_func_t;

/**
 ******************************************************************************
 ** \brief CSIO interrupt selection
 ******************************************************************************/
typedef enum stc_csio_irq_sel
{
    CsioTxIrq           = 0u,           ///< CSIO TX interrupt
    CsioRxIrq           = 1u,           ///< CSIO RX interrupt
    CsioTxIdleIrq       = 2u,           ///< CSIO TX idle interrupt
    CsioTxFifoIrq       = 3u,           ///< CSIO TX FIFO interrupt
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
    CsioCsErrIrq        = 4u,           ///< CSIO chip selection interrupt
    CsioSerialTimerIrq  = 5u,           ///< CSIO serial timer interrupt
#endif    
}en_csio_irq_sel_t;

/**
 ******************************************************************************
 ** \brief CSIO status types
 ******************************************************************************/
typedef enum en_csio_status
{
    CsioOverrunError    = 0u,             ///< CSIO overrun error
    CsioRxFull          = 1u,             ///< CSIO RX completion 
    CsioTxEmpty         = 2u,             ///< CSIO TX buffer empty
    CsioTxIdle          = 3u,             ///< CSIO TX idle 
    CsioTxFifoRequest   = 4u,             ///< CSIO TX FIFO request
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
    CsioCsErrIntFlag    = 5u,             ///< CSIO chip selection error occurrance
    CsioTimerIntFlag    = 6u,             ///< CSIO serial timer interrupt 
#endif    
}en_csio_status_t;

/******************************************************************************
 * I2C type definitions
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief I2C mode
 ******************************************************************************/ 
typedef enum en_i2c_mode
{
    I2cMaster = 0u,  ///< I2C Master mode
    I2cSlave  = 1u,  ///< I2C Slave mode
  
}en_i2c_mode_t;
   
/**
 ******************************************************************************
 ** \brief I2C ACK types
 ******************************************************************************/ 
typedef enum en_i2c_ack
{
    I2cAck =  0u,   ///< I2C normal ACK
    I2cNAck = 1u,   ///< I2C NACK 

}en_i2c_ack_t; 

/**
 ******************************************************************************
 ** \brief I2C interrupt selection
 ******************************************************************************/
typedef enum en_i2c_irq_sel
{
    I2cTxIrq          = 0u,          ///< I2C TX interrupt
    I2cRxIrq          = 1u,          ///< I2C RX interrupt
    I2cTxIdleIrq      = 2u,          ///< I2C TX idle interrupt
    I2cTxFifoIrq      = 3u,          ///< I2C TX FIFO interrupt
    I2cTxRxIrq        = 4u,          ///< I2C TX and RX interrupt
    I2cStopDetectIrq  = 5u,          ///< I2C stop condition interrupt
  
}en_i2c_irq_sel_t;

/**
 ******************************************************************************
 ** \brief I2C status types
 ******************************************************************************/
typedef enum en_i2c_status
{
    I2cOverrunError         = 0u,   ///< I2C overrun error
    I2cRxFull               = 1u,   ///< I2C RX buffer full 
    I2cTxEmpty              = 2u,   ///< I2C TX buffer empty 
    I2cTxIdle               = 3u,   ///< I2C TX idle
    I2cTxFifoRequest        = 4u,   ///< I2C TX FIFO request
    I2cFirstByteDetect      = 5u,   ///< I2C First byte detection
    I2cReservedByteDetect   = 6u,   ///< I2C reserved byte detection
    I2cStopDetect           = 7u,   ///< I2C stop condition detection
    I2cBusStatus            = 8u,   ///< I2C Bus status
    I2cBusErr               = 9u,   ///< I2C Bus error 
    I2cRxTxIrq              = 10u,  ///< I2C transimission or reception interrupt flag
    I2cDevAddrMatch         = 11u,  ///< I2C received slave address matchs with pre-set address
}en_i2c_status_t;

/**
 ******************************************************************************
 ** \brief I2C data direction(used in slave mode)
 ******************************************************************************/
typedef enum en_i2c_data_dir
{
    i2c_master_tx_slave_rx = 0u,  ///< Data from master to slave
    i2c_slave_tx_master_rx = 1u,  ///< Data from slave to master
  
}en_i2c_data_dir_t;

/******************************************************************************
 * LIN type definitions
 ******************************************************************************/
/**
 ******************************************************************************
 ** \brief MFS LIN mode (Master/Slave)
 ******************************************************************************/
typedef enum en_lin_ms_mode
{
    LinMasterMode = 0u,            ///< LIN Master Mode
    LinSlaveMode  = 1u,            ///< LIN Slave Mode
} en_lin_ms_mode_t;

/**
 ******************************************************************************
 ** \brief LIN stop bit length
 ******************************************************************************/
typedef enum en_lin_stop_bit
{
    LinOneStopBit    = 0u,       ///< 1 Stop Bit
    LinTwoStopBits   = 1u,       ///< 2 Stop Bits
    LinThreeStopBits = 2u,       ///< 3 Stop Bits
    LinFourStopBits  = 3u,       ///< 4 Stop Bits
} en_lin_stop_bit_t;

/**
 ******************************************************************************
 ** \brief Mfs Lin Break Generation Length (only applicable in LIN master mode)
 ******************************************************************************/
typedef enum en_lin_break_length
{
    LinBreakLength13 = 0u,        ///< Lin Break Length 13 Bit Times
    LinBreakLength14 = 1u,        ///< Lin Break Length 14 Bit Times
    LinBreakLength15 = 2u,        ///< Lin Break Length 15 Bit Times
    LinBreakLength16 = 3u,        ///< Lin Break Length 16 Bit Times
} en_lin_break_len_t;

/**
 ******************************************************************************
 ** \brief Mfs Lin Break Delimiter Length (only applicable in LIN master mode)
 ******************************************************************************/
typedef enum en_lin_delimiter_length
{
    LinDelimiterLength1 = 0u,     ///< Lin Break Delimiter Length 1 Bit Time
    LinDelimiterLength2 = 1u,     ///< Lin Break Delimiter Length 2 Bit Times
    LinDelimiterLength3 = 2u,     ///< Lin Break Delimiter Length 3 Bit Times
    LinDelimiterLength4 = 3u,     ///< Lin Break Delimiter Length 4 Bit Times
} en_lin_delimiter_len_t;

/**
 ******************************************************************************
 ** \brief LIN function
 ******************************************************************************/
typedef enum en_lin_func
{
    LinTx = 0u,              ///< Lin TX
    LinRx = 1u,              ///< Lin RX 
  
}en_lin_func_t;

/**
 ******************************************************************************
 ** \brief LIN interrupt enumeration
 ******************************************************************************/
typedef enum en_lin_irq_sel
{
    LinTxIrq         = 0u,         ///< LIN TX interrupt
    LinRxIrq         = 1u,         ///< LIN RX interrupt
    LinBreakIrq      = 2u,         ///< LIN break field interrupt
    LinTxIdleIrq     = 3u,         ///< LIN TX idle interrupt
    LinTxFifoIrq     = 4u,         ///< LIN TX FIFO interrupt
     
}en_lin_irq_sel_t;

/**
 ******************************************************************************
 ** \brief LIN status types
 ******************************************************************************/
typedef enum en_lin_status
{
    LinFrameError       = 0u,       ///< LIN Frame error
    LinOverrunError     = 1u,       ///< LIN overrun error
    LinRxFull           = 2u,       ///< LIN RX buffer full 
    LinTxEmpty          = 3u,       ///< LIN TX buffer empty 
    LinTxIdle           = 4u,       ///< LIN TX idle 
    LinBreakFlag        = 5u,       ///< LIN break field detection flag
    LinTxFifoRequest    = 6u,       ///< LIN FIFO request
  
}en_lin_status_t;

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/
/// Enumeration to define an index for each enabled MFS instance
typedef enum en_mfs_instance_index
{  
#if (PDL_PERIPHERAL_ENABLE_MFS0 == PDL_ON)  
    MfsInstanceIndexMfs0,
#endif    
#if (PDL_PERIPHERAL_ENABLE_MFS1 == PDL_ON) 
    MfsInstanceIndexMfs1,
#endif    
#if (PDL_PERIPHERAL_ENABLE_MFS2 == PDL_ON)
    MfsInstanceIndexMfs2,
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS3 == PDL_ON)
    MfsInstanceIndexMfs3,
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS4 == PDL_ON)
    MfsInstanceIndexMfs4,
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS5 == PDL_ON)
    MfsInstanceIndexMfs5,
#endif  
#if (PDL_PERIPHERAL_ENABLE_MFS6 == PDL_ON)
    MfsInstanceIndexMfs6,
#endif  
#if (PDL_PERIPHERAL_ENABLE_MFS7 == PDL_ON)
    MfsInstanceIndexMfs7,
#endif    
#if (PDL_PERIPHERAL_ENABLE_MFS8 == PDL_ON)
    MfsInstanceIndexMfs8,
#endif 
#if (PDL_PERIPHERAL_ENABLE_MFS9 == PDL_ON)
    MfsInstanceIndexMfs9,
#endif 
#if (PDL_PERIPHERAL_ENABLE_MFS10 == PDL_ON)
    MfsInstanceIndexMfs10,
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS11 == PDL_ON)
    MfsInstanceIndexMfs11,
#endif 
#if (PDL_PERIPHERAL_ENABLE_MFS12 == PDL_ON)
    MfsInstanceIndexMfs12,
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS13 == PDL_ON)
    MfsInstanceIndexMfs13,
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS14 == PDL_ON)
    MfsInstanceIndexMfs14,
#endif
#if (PDL_PERIPHERAL_ENABLE_MFS15 == PDL_ON)
    MfsInstanceIndexMfs15,
#endif    
    MfsInstanceIndexMax,
    MfsInstanceIndexUnknown = 0xFFu,
    
} en_mfs_instance_index_t;

/// MFS mode
typedef enum en_mfs_mode
{
    MfsInitMode = 0u,    ///< MFS initial mode
    MfsUartMode = 1u,    ///< MFS UART mode
    MfsCsioMode = 2u,    ///< MFS CSIO mode
    MfsI2cMode  = 3u,    ///< MFS I2C mode
    MfsLinMode  = 4u,    ///< MFS LIN mode
  
}en_mfs_mode_t;

/** \} GroupMFS_Types */

/**
* \addtogroup GroupMFS_DataStructures
* \{
*/

/**
 ******************************************************************************
 ** \brief Mfs FIFO configuration.
 ******************************************************************************/
typedef struct stc_mfs_fifo_config
{
    en_mfs_fifo_sel_t enFifoSel;    ///< FIFO selection, see #en_mfs_fifo_sel_t for details
    uint8_t u8ByteCount1;           ///< Transfer data count for FIFO1
    uint8_t u8ByteCount2;           ///< Transfer data count for FIFO2
} stc_mfs_fifo_config_t;

/*******************************************************************************
* UART Data Structures
*******************************************************************************/

/**
 ******************************************************************************
 ** \brief UART interrupt enable structure
 ******************************************************************************/
typedef struct stc_uart_irq_en
{
    boolean_t bTxIrq;          ///< UART TX interrupt
    boolean_t bRxIrq;          ///< UART RX interrupt
    boolean_t bTxIdleIrq;      ///< UART TX idle interrupt
    boolean_t bTxFifoIrq;      ///< UART TX FIFO interrupt
    
}stc_uart_irq_en_t;

/**
 ******************************************************************************
 ** \brief UART interrupt callback function
 ******************************************************************************/
typedef struct stc_uart_irq_cb
{
    func_ptr_t pfnTxIrqCb;      ///< UART TX interrupt callback function pointer
    func_ptr_t pfnRxIrqCb;      ///< UART RX interrupt callback function pointer
    func_ptr_t pfnTxIdleCb;     ///< UART TX idle interrupt callback function pointer
    func_ptr_t pfnTxFifoIrqCb;  ///< UART TX FIFO interrupt callback function pointer
    
}stc_uart_irq_cb_t;

/**
 ******************************************************************************
 ** \brief UART configuration structure
 ******************************************************************************/
typedef struct stc_mfs_uart_config
{
    en_uart_mode_t      enMode;           ///< UART mode
    uint32_t            u32BaudRate;      ///< Baud rate (bps)
    en_uart_parity_t    enParity;         ///< Parity format
    en_uart_stop_bit_t  enStopBit;        ///< Stop bit
    en_uart_data_len_t  enDataLength;     ///< 5..9 Bit Character Length
    en_uart_data_dir_t  enBitDirection;   ///< UART data direction
    boolean_t bInvertData;                ///< FALSE: NRZ, TRUE : Inverted NRZ
    boolean_t bHwFlow;                    ///< FALSE: Not use Hardware Flow, TRUE : Use Hardware Flow
    boolean_t bUseExtClk;                 ///< FALSE: use internal clock, TRUE: use external clock which input via SCK pin
    
    stc_mfs_fifo_config_t* pstcFifoConfig;  ///< Pointer to FIFO configuration structure, if set to NULL, FIFO function will not be enabled.
#if (PDL_INTERRUPT_ENABLE_MFS0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS8 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS9 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS10 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS11 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS12 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS5 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS13 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS14 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS7 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS15 == PDL_ON)       
    stc_uart_irq_en_t *pstcIrqEn;         ///< Pointer to UART interrupt enable structure, if set to NULL, no interrupt enabled.
    stc_uart_irq_cb_t *pstcIrqCb;         ///< Pointer to UART interrupt callback functions structurei, f set to NULL, no interrupt callback initialized.
    boolean_t bTouchNvic;                 ///< TRUE: enable NVIC, FALSE: don't enable NVIC
#endif    
} stc_mfs_uart_config_t;

/*******************************************************************************
* CSIO Data Structures
*******************************************************************************/

/**
 ******************************************************************************
 ** \brief CSIO serial timer configuration
 ******************************************************************************/
typedef struct stc_csio_serial_timer
{
    en_csio_timer_clk_t    enClkDiv;           ///< Clock division
    uint8_t                u8TransferByteCnt;  ///< Transfer byte count
    uint16_t               u16CompareVal;      ///< The compare value depending on which the transfer starts
  
}stc_csio_serial_timer_t;

/**
 ******************************************************************************
 ** \brief CSIO chip selection configuration
 ******************************************************************************/
typedef struct stc_csio_cs
{
    en_cs_pin_sel_t     enCsStartPin;        ///< Chip selecetion pin selection
    en_cs_pin_sel_t     enCsEndPin;          ///< Chip selecetion pin selection
    en_cs_pin_level_t   enLevel;             ///< Active level selection, only available for SCS0
    boolean_t           bActiveHold;         ///< FALSE: don't hold active status, TRUE: hold active status until all bytes are transfered.
    en_cs_timing_clk_t  enClkDiv;            ///< Chip selecetion clock division
    uint8_t             u8CsSetupDelayTime;  ///< Chip selecetion delay time
    uint8_t             u8CsHoldDelayTime;   ///< Chip selecetion hold time
    uint16_t            u16CsDeselectTime;   ///< Chip selecetion deselection time (gap between two bytes's transfer)
    uint8_t             u8Scs0TransferByteCnt; ///< SCS0 Transfer byte count 
    uint8_t             u8Scs1TransferByteCnt; ///< SCS1 Transfer byte count 
    uint8_t             u8Scs2TransferByteCnt; ///< SCS2 Transfer byte count 
    uint8_t             u8Scs3TransferByteCnt; ///< SCS3 Transfer byte count 
    boolean_t           bScs0En;               ///< TRUE: SCS0 Enable  
    boolean_t           bScs1En;               ///< TRUE: SCS1 Enable  
    boolean_t           bScs2En;               ///< TRUE: SCS2 Enable  
    boolean_t           bScs3En;               ///< TRUE: SCS3 Enable
    
}stc_csio_cs_t;

/**
 ******************************************************************************
 ** \brief CSIO interrupt enable structure
 ******************************************************************************/
typedef struct stc_csio_irq_en
{
    boolean_t bTxIrq;           ///< CSIO TX interrupt
    boolean_t bRxIrq;           ///< CSIO RX interrupt
    boolean_t bTxIdleIrq;       ///< CSIO TX idle interrupt
    boolean_t bTxFifoIrq;       ///< CSIO TX FIFO interrupt
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
    boolean_t bCsErrIrq;        ///< CSIO chip selection interrupt
    boolean_t bSerialTimerIrq;  ///< CSIO serial timer interrupt
#endif
}stc_csio_irq_en_t;

/**
 ******************************************************************************
 ** \brief CSIO interrupt callback function
 ******************************************************************************/
typedef struct stc_csio_irq_cb
{
    func_ptr_t pfnTxIrqCb;            ///< CSIO TX interrupt callback function
    func_ptr_t pfnRxIrqCb;            ///< CSIO RX interrupt callback function
    func_ptr_t pfnTxIdleCb;           ///< CSIO TX idle interrupt callback function
    func_ptr_t pfnTxFifoIrqCb;        ///< CSIO TX FIFO interrupt callback function
#if (PDL_MCU_CORE == PDL_FM4_CORE) || (PDL_MCU_CORE == PDL_FM0P_CORE)    
    func_ptr_t pfnCsErrIrqCb;         ///< CSIO chip selection interrupt callback function
    func_ptr_t pfnSerialTimerIrqCb;   ///< CSIO serial timer interrupt callback function
#endif    
    
}stc_csio_irq_cb_t;

/**
 ******************************************************************************
 ** \brief CSIO configuration structure
 ******************************************************************************/
typedef struct stc_mfs_csio_config
{
    en_csio_ms_mode_t         enMsMode;         ///< Master or slave mode
    uint32_t                  u32BaudRate;      ///< Baud rate (bps)
    en_csio_act_mode_t        enActMode;        ///< CSIO or SPI mode
    en_csio_sync_wait_time_t  enSyncWaitTime;   ///< Sync wait time
    en_csio_data_len_t        enDataLength;     ///< 5..16 Bit Character Length, see description of #en_csio_data_len_t
    en_csio_data_dir_t        enBitDirection;   ///< Bit direction
    boolean_t                 bInvertClk ;      ///< FALSE: SCK Mark Level High, TRUE: SCK Mark Level Low
#if (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)    
    stc_csio_serial_timer_t* pstcSerialTimer;   ///< Pointer to serial timer configuration structure, if set to NULL, nothing will be done.
    stc_csio_cs_t* pstcCsConfig;                ///< Pointer to chip selection configuration structure, if set to NULL, nothing will be done.
#endif       
    stc_mfs_fifo_config_t* pstcFifoConfig;      ///< Pointer to FIFO configuration structure, if set to NULL, nothing will be done.
#if (PDL_INTERRUPT_ENABLE_MFS0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS8 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS9 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS10 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS11 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS12 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS5 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS13 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS14 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS7 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS15 == PDL_ON)    
    stc_csio_irq_en_t *pstcIrqEn;               ///< Pointer to CSIO interrupt enable structure, if set to NULL, no interrupt enabled.
    stc_csio_irq_cb_t *pstcIrqCb;               ///< Pointer to CSIO interrupt callback functions structurei, f set to NULL, no interrupt callback initialized.
    boolean_t bTouchNvic;                       ///< TRUE: enable NVIC, FALSE: don't enable NVIC
#endif    
} stc_mfs_csio_config_t;

/*******************************************************************************
* I2C Data Structures
*******************************************************************************/

/**
 ******************************************************************************
 ** \brief I2C interrupt enable structure
 ******************************************************************************/
typedef struct stc_i2c_irq_en
{
    boolean_t bTxIrq;         ///< I2C TX interrupt
    boolean_t bRxIrq;         ///< I2C RX interrupt
    boolean_t bTxIdleIrq;     ///< I2C TX idle interrupt
    boolean_t bTxFifoIrq;     ///< I2C TX FIFO interrupt
    boolean_t bTxRxIrq;       ///< I2C TX and RX interrupt
    boolean_t bStopDetectIrq; ///< I2C stop condition interrupt
  
}stc_i2c_irq_en_t;

/**
 ******************************************************************************
 ** \brief I2C interrupt callback function
 ******************************************************************************/
typedef struct stc_i2c_irq_cb
{
    func_ptr_t pfnTxIrqCb;         ///< I2C TX interrupt callback function pointer
    func_ptr_t pfnRxIrqCb;         ///< I2C RX interrupt callback function pointer
    func_ptr_t pfnTxIdleCb;        ///< I2C TX idle interrupt callback function pointer
    func_ptr_t pfnTxFifoIrqCb;     ///< I2C TX FIFO interrupt callback function pointer
    func_ptr_t pfnTxRxIrqCb;       ///< I2C TX and RX completion interrupt callback function
    func_ptr_t pfnStopDetectIrqCb; ///< I2C stop condition interrupt
    
}stc_i2c_irq_cb_t;

/**
 ******************************************************************************
 ** \brief I2C configuration structure
 ******************************************************************************/
typedef struct stc_mfs_i2c_config
{
    en_i2c_mode_t enMsMode;          ///< I2C master mode or slave mode
    uint32_t  u32BaudRate;           ///< Baud rate (bps)
    uint8_t   u8SlaveAddr;           ///< Slave address (This is effective on slave mode.)
    uint8_t   u8SlaveMaskAddr;       ///< Slave Mask address (This is effective on slave mode.)
    boolean_t bWaitSelection;        ///< FALSE: generate interrupt after ACK, TRUE: generate interrupt before ACK
    boolean_t bDmaEnable;            ///< FALSE: don't use DMA function, TRUE: use DMA function
      
    stc_mfs_fifo_config_t* pstcFifoConfig; ///< Pointer to FIFO configuration structure, if set to NULL, FIFO function will not be enabled.
#if (PDL_INTERRUPT_ENABLE_MFS0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS8 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS9 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS10 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS11 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS12 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS5 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS13 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS14 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS7 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS15 == PDL_ON)       
    stc_i2c_irq_en_t* pstcIrqEn;     ///< Pointer to I2C interrupt enable structure, if set to NULL, no interrupt enabled.
    stc_i2c_irq_cb_t* pstcIrqCb;     ///< Pointer to I2C interrupt callback functions structurei, if set to NULL, no interrupt callback initialized.
    boolean_t bTouchNvic;            ///< TRUE: enable NVIC, FALSE: don't enable NVIC
#endif    
} stc_mfs_i2c_config_t;

/*******************************************************************************
* LIN Data Structures
*******************************************************************************/

/**
 ******************************************************************************
 ** \brief LIN interrupt selection
 ******************************************************************************/
typedef struct stc_lin_irq_en
{
    boolean_t bTxIrq;         ///< LIN TX interrupt
    boolean_t bRxIrq;         ///< LIN RX interrupt
    boolean_t bLinBreakIrq;   ///< LIN break field interrupt
    boolean_t bTxIdleIrq;     ///< LIN TX idle interrupt
    boolean_t bTxFifoIrq;     ///< LIN TX FIFO interrupt
     
}stc_lin_irq_en_t;

/**
 ******************************************************************************
 ** \brief LIN interrupt callback function
 ******************************************************************************/
typedef struct stc_lin_irq_cb
{
    func_ptr_t  pfnTxIrqCb;        ///< LIN TX interrupt callback function
    func_ptr_t  pfnRxIrqCb;        ///< LIN RX interrupt callback function
    func_ptr_t  pfnLinBreakIrqCb;  ///< LIN break field interrupt callback function
    func_ptr_t  pfnTxIdleIrqCb;    ///< LIN TX idle interrupt callback function 
    func_ptr_t  pfnTxFifoIrqCb;    ///< LIN TX FIFO interrupt callback function 
    
}stc_lin_irq_cb_t;

/**
 ******************************************************************************
 ** \brief LIN configuration structure
 ******************************************************************************/
typedef struct stc_mfs_lin_config
{
    en_lin_ms_mode_t        enMsMode;             ///< LIN master or slave mode
    uint32_t                u32BaudRate;          ///< Baud rate (bps)
    en_lin_stop_bit_t       enStopBits;           ///< Stop bits length
    en_lin_break_len_t      enBreakLength;        ///< Break length
    en_lin_delimiter_len_t  enDelimiterLength;    ///< Delimiter length
    boolean_t bUseExtClk;                         ///< FALSE: use internal clock, TRUE: use external clock which input via SCK pin
    
    stc_mfs_fifo_config_t* pstcFifoConfig;        ///< Pointer to FIFO configuration structure, if set to NULL, FIFO function will not be enabled.
#if (PDL_INTERRUPT_ENABLE_MFS0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS8 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS9 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS10 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS11 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS12 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS5 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS13 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS14 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS7 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS15 == PDL_ON)    
    stc_lin_irq_en_t* pstcIrqEn;                 ///< Pointer to LIN interrupt enable structure, if set to NULL, no interrupt enabled.
    stc_lin_irq_cb_t* pstcIrqCb;                 ///< Pointer to LIN interrupt callback functions structurei, if set to NULL, no interrupt callback initialized.
    boolean_t bTouchNvic;                        ///< TRUE: enable NVIC, FALSE: don't enable NVIC
#endif    
} stc_mfs_lin_config_t;

/// MFS common instance structure
typedef struct stc_mfsn
{
  #if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON)
    volatile stc_mfsn_uart_t* pstcUartInstance;   ///< Pointer to UART instance
  #endif
  #if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON)
    volatile stc_mfsn_csio_t* pstcCsioInstance;   ///< Pointer to CSIO instance
  #endif
  #if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON)
    volatile stc_mfsn_i2c_t* pstcI2cInstance;     ///< Pointer to I2C instance
  #endif
  #if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON)
    volatile stc_mfsn_lin_t* pstcLinInstance;     ///< Pointer to LIN instance
  #endif
}stc_mfsn_t;

/// MFS module internal data, storing internal information for each enabled MFS instance.
typedef struct stc_mfs_intern_data
{
    en_mfs_mode_t               enMode;                 ///< MFS mode
    union
    {
        func_ptr_t fnMfsInternIntCb[6];
        stc_uart_irq_cb_t stcUartInternIrqCb;           ///< Uart internal interrupt callback function
        stc_csio_irq_cb_t stcCsioInternIrqCb;           ///< CSIO internal interrupt callback function
        stc_i2c_irq_cb_t stcI2cInternIrqCb;             ///< I2C internal interrupt callback function
        stc_lin_irq_cb_t stcLinInternIrqCb;             ///< LIN internal interrupt callback function
    };

} stc_mfs_intern_data_t;

/// MFS instance data type
typedef struct stc_mfs_instance_data
{
    stc_mfsn_t   stcInstance;     ///< pointer to registers of an instance
    stc_mfs_intern_data_t stcInternData;    ///< module internal data of instance
} stc_mfs_instance_data_t;

/** \} GroupMFS_DataStructures */

/**
* \addtogroup GroupMFS_GlobalVariables
* \{
*/

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/

/// Look-up table for all enabled MFS instances and their internal data
extern stc_mfs_instance_data_t m_astcMfsInstanceDataLut[MFS_INSTANCE_COUNT];

/** \} GroupMFS_GlobalVariables */

/**
* \addtogroup GroupMFS_Functions
* \{
*/

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/
#if (PDL_PERIPHERAL_ENABLE_MFS_UART_MODE == PDL_ON)
/* UART */
#if (PDL_INTERRUPT_ENABLE_MFS0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS8 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS9 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS10 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS11 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS12 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS5 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS13 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS14 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS7 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS15 == PDL_ON) 
// Interrupt
void MfsUartIrqHandlerTx(volatile stc_mfsn_uart_t*   pstcUart, 
                         stc_mfs_intern_data_t* pstcMfsInternData);
void MfsUartIrqHandlerRx(volatile stc_mfsn_uart_t*   pstcUart, 
                         stc_mfs_intern_data_t* pstcMfsInternData);
en_result_t Mfs_Uart_EnableIrq(volatile stc_mfsn_uart_t* pstcUart, 
                               en_uart_irq_sel_t enIrqSel);
en_result_t Mfs_Uart_DisableIrq(volatile stc_mfsn_uart_t* pstcUart, 
                                en_uart_irq_sel_t enIntSel);
#endif
// Init/De-Init
en_result_t Mfs_Uart_Init(volatile stc_mfsn_uart_t*  pstcUart,
                          const stc_mfs_uart_config_t* pstcConfig);
en_result_t Mfs_Uart_DeInit(volatile stc_mfsn_uart_t* pstcUart, boolean_t bTouchNvic);
// Baud rate
en_result_t Mfs_Uart_SetBaudRate(volatile stc_mfsn_uart_t* pstcUart,
                                 uint32_t u32BaudRate);
// Function enable/disable
en_result_t Mfs_Uart_EnableFunc(volatile stc_mfsn_uart_t* pstcUart, en_uart_func_t enFunc);
en_result_t Mfs_Uart_DisableFunc(volatile stc_mfsn_uart_t* pstcUart, en_uart_func_t enFunc);
// Status read/clear
boolean_t Mfs_Uart_GetStatus(volatile stc_mfsn_uart_t* pstcUart, 
                             en_uart_status_t enStatus);
en_result_t Mfs_Uart_ClrStatus(volatile stc_mfsn_uart_t* pstcUart,
                               en_uart_status_t enStatus);
// Data read/write
en_result_t Mfs_Uart_SendData(volatile stc_mfsn_uart_t* pstcUart, uint16_t Data);
uint16_t Mfs_Uart_ReceiveData(volatile stc_mfsn_uart_t* pstcUart);
// FIFO 
en_result_t Mfs_Uart_ResetFifo (volatile stc_mfsn_uart_t* pstcUart, 
                                en_mfs_fifo_t enFifo);
en_result_t Mfs_Uart_SetFifoCount(volatile stc_mfsn_uart_t* pstcUart,
                                  en_mfs_fifo_t enFifo,
                                  uint8_t u8Count);
uint8_t Mfs_Uart_GetFifoCount(volatile stc_mfsn_uart_t* pstcUart,
                              en_mfs_fifo_t enFifo);
#endif

#if (PDL_PERIPHERAL_ENABLE_MFS_CSIO_MODE == PDL_ON)
/* CSIO */
#if (PDL_INTERRUPT_ENABLE_MFS0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS8 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS9 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS10 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS11 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS12 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS5 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS13 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS14 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS7 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS15 == PDL_ON) 
// Interrupt
void MfsCsioIrqHandlerTx(volatile stc_mfsn_csio_t*   pstcCsio, 
                         stc_mfs_intern_data_t* pstcMfsInternData);
void MfsCsioIrqHandlerRx(volatile stc_mfsn_csio_t*   pstcCsio, 
                         stc_mfs_intern_data_t* pstcMfsInternData);
void MfsCsioIrqHandlerStatus(volatile stc_mfsn_csio_t*   pstcCsio, 
                             stc_mfs_intern_data_t* pstcMfsInternData);
en_result_t Mfs_Csio_EnableIrq(volatile stc_mfsn_csio_t* pstcCsio, 
                               en_csio_irq_sel_t enIrqSel);
en_result_t Mfs_Csio_DisableIrq(volatile stc_mfsn_csio_t* pstcCsio, 
                                en_csio_irq_sel_t enIrqSel);
#endif
// Init/De-Init
en_result_t Mfs_Csio_Init(volatile stc_mfsn_csio_t*         pstcCsio, 
                          const stc_mfs_csio_config_t* pstcConfig);
en_result_t Mfs_Csio_DeInit(volatile stc_mfsn_csio_t* pstcCsio, boolean_t bTouchNvic);
// Re-configuration 
en_result_t Mfs_Csio_SetBaudRate(volatile stc_mfsn_csio_t* pstcCsio,
                                 uint32_t u32BaudRate); 
#if (PDL_MCU_CORE == PDL_FM0P_CORE) || (PDL_MCU_CORE == PDL_FM4_CORE)                                     
en_result_t Mfs_Csio_SetTimerCompareValue(volatile stc_mfsn_csio_t* pstcCsio,
                                          uint16_t u16CompareValue);
en_result_t Mfs_Csio_SetCsTransferByteCount(volatile stc_mfsn_csio_t* pstcCsio,
                                            en_cs_pin_sel_t enCsPin,
                                            uint8_t u8ByteCnt);
en_result_t Mfs_Csio_SetCsHoldStatus(volatile stc_mfsn_csio_t* pstcCsio, 
                                     boolean_t bHold);
en_result_t Mfs_Csio_SetTimerTransferByteCount(volatile stc_mfsn_csio_t* pstcCsio,
                                               uint8_t u8ByteCnt);
#endif
// Function enable/disable
en_result_t Mfs_Csio_EnableFunc(volatile stc_mfsn_csio_t* pstcCsio, en_csio_func_t enFunc);
en_result_t Mfs_Csio_DisableFunc(volatile stc_mfsn_csio_t* pstcCsio, en_csio_func_t enFunc);

// Status read/clear
boolean_t Mfs_Csio_GetStatus(volatile stc_mfsn_csio_t* pstcCsio, 
                             en_csio_status_t enStatus);
en_result_t Mfs_Csio_ClrStatus(volatile stc_mfsn_csio_t* pstcCsio,
                               en_csio_status_t enStatus);
// Data read/write
en_result_t Mfs_Csio_SendData(volatile stc_mfsn_csio_t* pstcCsio, 
                              uint16_t u16Data,
                              boolean_t bSotEn);
uint16_t Mfs_Csio_ReceiveData(volatile stc_mfsn_csio_t* pstcCsio);
// FIFO 
en_result_t Mfs_Csio_ResetFifo (volatile stc_mfsn_csio_t* pstcCsio, 
                                en_mfs_fifo_t enFifo);
en_result_t Mfs_Csio_SetFifoCount(volatile stc_mfsn_csio_t* pstcCsio,
                                  en_mfs_fifo_t enFifo,
                                  uint8_t u8Count);
uint8_t Mfs_Csio_GetFifoCount(volatile stc_mfsn_csio_t* pstcCsio,
                              en_mfs_fifo_t enFifo);
#endif

#if (PDL_PERIPHERAL_ENABLE_MFS_I2C_MODE == PDL_ON)
/* I2C */
#if (PDL_INTERRUPT_ENABLE_MFS0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS8 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS9 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS10 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS11 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS12 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS5 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS13 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS14 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS7 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS15 == PDL_ON) 
// Interrupt
void MfsI2cIrqHandlerTx(volatile stc_mfsn_i2c_t*   pstcI2c, 
                        stc_mfs_intern_data_t* pstcMfsInternData);
void MfsI2cIrqHandlerRx(volatile stc_mfsn_i2c_t*   pstcI2c, 
                        stc_mfs_intern_data_t* pstcMfsInternData);
void MfsI2cIrqHandlerStatus(volatile stc_mfsn_i2c_t*   pstcI2c, 
                            stc_mfs_intern_data_t* pstcMfsInternData);
en_result_t Mfs_I2c_EnableIrq(volatile stc_mfsn_i2c_t* pstcI2c, 
                              en_i2c_irq_sel_t enIrqSel);
en_result_t Mfs_I2c_DisableIrq(volatile stc_mfsn_i2c_t* pstcI2c, 
                               en_i2c_irq_sel_t enIrqSel);
#endif
// Init/De-Init
en_result_t Mfs_I2c_Init(volatile stc_mfsn_i2c_t*        pstcI2c, 
                         const stc_mfs_i2c_config_t* pstcConfig);
en_result_t Mfs_I2c_DeInit(volatile stc_mfsn_i2c_t* pstcI2c, boolean_t bTouchNvic);

// Start/Stop
en_result_t Mfs_I2c_GenerateStart(volatile stc_mfsn_i2c_t*        pstcI2c);
en_result_t Mfs_I2c_GenerateRestart(volatile stc_mfsn_i2c_t*      pstcI2c);
en_result_t Mfs_I2c_GenerateStop(volatile stc_mfsn_i2c_t*      pstcI2c);

// Re-configure baud rate 
en_result_t Mfs_I2c_SetBaudRate(volatile stc_mfsn_i2c_t* pstcI2c,
                                uint32_t u32BaudRate); 
// Data read/write
en_result_t Mfs_I2c_SendData(volatile stc_mfsn_i2c_t*      pstcI2c, uint8_t u8Data);
uint8_t Mfs_I2c_ReceiveData(volatile stc_mfsn_i2c_t*      pstcI2c);

// ACK
en_result_t Mfs_I2c_ConfigAck(volatile stc_mfsn_i2c_t*        pstcI2c, en_i2c_ack_t enAck);
en_i2c_ack_t Mfs_I2c_GetAck(volatile stc_mfsn_i2c_t*        pstcI2c);

// Status read/clear
boolean_t Mfs_I2c_GetStatus(volatile stc_mfsn_i2c_t* pstcI2c, 
                            en_i2c_status_t enStatus);
en_result_t Mfs_I2c_ClrStatus(volatile stc_mfsn_i2c_t* pstcI2c,
                               en_i2c_status_t enStatus);

// Get Data direction in slave mode
en_i2c_data_dir_t Mfs_I2c_GetDataDir(volatile stc_mfsn_i2c_t* pstcI2c);

// FIFO 
en_result_t Mfs_I2c_ResetFifo  (volatile stc_mfsn_i2c_t* pstcI2c, 
                                en_mfs_fifo_t enFifo);
en_result_t Mfs_I2c_SetFifoCount(volatile stc_mfsn_i2c_t* pstcI2c,
                                 en_mfs_fifo_t enFifo,
                                 uint8_t u8Count);
uint8_t Mfs_I2c_GetFifoCount(volatile stc_mfsn_i2c_t* pstcI2c,
                             en_mfs_fifo_t enFifo);
#endif

#if (PDL_PERIPHERAL_ENABLE_MFS_LIN_MODE == PDL_ON)
/* LIN */
#if (PDL_INTERRUPT_ENABLE_MFS0 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS8 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS1 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS9 == PDL_ON) ||  \
    (PDL_INTERRUPT_ENABLE_MFS2 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS10 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS3 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS11 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS4 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS12 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS5 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS13 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS6 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS14 == PDL_ON) || \
    (PDL_INTERRUPT_ENABLE_MFS7 == PDL_ON) || (PDL_INTERRUPT_ENABLE_MFS15 == PDL_ON) 
// Interrupt
void MfsLinIrqHandlerTx(volatile stc_mfsn_lin_t*   pstcLin, 
                         stc_mfs_intern_data_t* pstcMfsInternData);
void MfsLinIrqHandlerRx(volatile stc_mfsn_lin_t*   pstcLin, 
                         stc_mfs_intern_data_t* pstcMfsInternData);
void MfsLinIrqHandlerStatus(volatile stc_mfsn_lin_t*   pstcLin, 
                            stc_mfs_intern_data_t* pstcMfsInternData);
en_result_t Mfs_Lin_EnableIrq(volatile stc_mfsn_lin_t* pstcLin, 
                              en_lin_irq_sel_t enIrqSel);
en_result_t Mfs_Lin_DisableIrq(volatile stc_mfsn_lin_t* pstcLin, 
                               en_lin_irq_sel_t enIrqSel);
#endif
// Init/De-Init
en_result_t Mfs_Lin_Init(volatile stc_mfsn_lin_t*  pstcLin,
                         const stc_mfs_lin_config_t* pstcConfig);
en_result_t Mfs_Lin_DeInit(volatile stc_mfsn_lin_t* pstcLin, boolean_t bTouchNvic);
// Baud rate
en_result_t Mfs_Lin_SetBaudRate(volatile stc_mfsn_lin_t* pstcLin,
                                uint32_t u32BaudRate);
// Generate break field
en_result_t Mfs_Lin_GenerateBreakField(volatile stc_mfsn_lin_t* pstcLin);

// Function enable/disable
en_result_t Mfs_Lin_EnableFunc(volatile stc_mfsn_lin_t* pstcLin, en_lin_func_t enFunc);
en_result_t Mfs_Lin_DisableFunc(volatile stc_mfsn_lin_t* pstcLin, en_lin_func_t enFunc);
// Status read/clear
boolean_t Mfs_Lin_GetStatus(volatile stc_mfsn_lin_t* pstcLin, 
                            en_lin_status_t enStatus);
en_result_t Mfs_Lin_ClrStatus(volatile stc_mfsn_lin_t* pstcLin,
                              en_lin_status_t enStatus);
// Data read/write
en_result_t Mfs_Lin_SendData(volatile stc_mfsn_lin_t* pstcLin, uint8_t Data);
uint8_t Mfs_Lin_ReceiveData(volatile stc_mfsn_lin_t* pstcLin);
// FIFO 
en_result_t Mfs_Lin_ResetFifo (volatile stc_mfsn_lin_t* pstcLin, 
                               en_mfs_fifo_t enFifo);
en_result_t Mfs_Lin_SetFifoCount(volatile stc_mfsn_lin_t* pstcLin,
                                 en_mfs_fifo_t enFifo,
                                 uint8_t u8Count);
uint8_t Mfs_Lin_GetFifoCount(volatile stc_mfsn_lin_t* pstcLin,
                             en_mfs_fifo_t enFifo);
#endif

/** \} GroupMFS_Functions */
/** \} GroupMFS */

#ifdef __cplusplus
}
#endif

#endif /* #if (defined(PDL_PERIPHERAL_MFS_ACTIVE)) */

#endif /* __MFS_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
