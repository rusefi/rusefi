/******************************************************************************
* \file         hsspi.h
*
* \version      1.20
*
* \brief        Headerfile for High Speed Quad Serial Peripheral Interface functions
*
******************************************************************************* 
\copyright
* Copyright 2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
* CYPRESS PROVIDES THIS SOFTWARE "AS IS" AND MAKES NO WARRANTY
* OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS SOFTWARE,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
* PURPOSE.                         
*******************************************************************************//*****************************************************************************/


#ifndef __HSSPI_H__
#define __HSSPI_H__

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "mcu.h"
#include "pdl_user.h"

#if (defined(PDL_PERIPHERAL_HSSPI_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
* \defgroup GroupHSSPI High Speed Quad Serial Peripheral Interface (HSSPI)
* \{ 
* \defgroup GroupHSSPI_Macros Macros
* \defgroup GroupHSSPI_Functions Functions
* \defgroup GroupHSSPI_GlobalVariables Global Variables
* \defgroup GroupHSSPI_Types Enumerated Types
* \defgroup GroupHSSPI_DataStructures Data Structures
* \}
*/
/**
* \addtogroup GroupHSSPI
* \{
* The High-speed Quad Serial Peripheral Interface (HSSPI) peripheral implements 
* Quad-SPI for increased performance with Flash memory. HSSPI is available on 
* certain devices in the FM4 portfolio. See the datasheet for your device to 
* determine whether it supports HSSPI.<br>
* HSSPI supports two operating modes: direct mode and command sequencer mode. 
* It supports the direct master mode for setting up the external device and 
* command sequencer mode to read/write to memory devices attached to a HSSPI 
* peripheral. Note that the command sequencer mode is not compatible with all 
* SPI devices. Refer to the datasheet provided with the product you are using to 
* determine whether your device is compatible with this mode.<br>
* The HSSPI controller has these features:
* * Compatible with single bit, dual bit and quad bit
* * Compatible with up to four slave devices
* * Can set format of serial interface for each slave (direct mode only)
* * Supports four clocking modes
* * Supports DMA transfer <br>
* This driver covers functionality for all HSSPI modules including the command sequencer mode.<br>
* Direct slave mode is not supported.
*
* \section SectionHSSPI_ConfigurationConsideration Configuration Consideration
* To set up communication with an HSSPI device, you go through several steps.<br>
* First, specify the configuration options in the stc_hsspi_config_t structure, 
* and call HsSpi_Init().<br>
* Then set the operating mode. Call HsSpi_SetMode() to change operating modes and 
* set the mode protocol. <br>
* Next, specify the configuration for the external device in the 
* stc_hsspi_ext_device_config_t structure and call HsSpi_SetExternalDeviceConfig(). 
* You do this for each external device (up to four).
* 
* \section SectionHSSPI_MoreInfo More Information
* For more information on the HSSPI peripheral, refer to:<br>
* <a href="http://www.cypress.com/file/222976/download">FM4 Peripheral Manual Communication Subsystem</a><br>
* The Peripheral Manual is divided into several subsystems. Click the link to see:<br>
* <a href="http://www.cypress.com/documentation/technical-reference-manuals/fm4-family-32-bit-arm-cortex-m4-microcontrollers">all 
* FM4 Family 32-Bit Microcontrollers Peripheral Manuals, and Errata Sheets</a><br>
*/

/**
* \addtogroup GroupHSSPI_Macros
* \{
*/
/*****************************************************************************/
/* Global pre-processor symbols/macros ('define')                            */
/*****************************************************************************/
#define stc_hsspin_t   FM_HSSPI_TypeDef
#define HSSPI0         (*((volatile stc_hsspin_t *) FM_HSSPI_BASE))

#define HS_SPI_INSTANCE_COUNT                                \
        (uint8_t)(PDL_PERIPHERAL_ENABLE_HSSPI0 == PDL_ON)

/// Default clock divider (will set 20MHz @ 160MHz main clock)
#define HS_SPI_DEFAULT_CLK_DIV     HsSpiClkDividerDiv8

/// Default SPI mode
#define HS_SPI_DEFAULT_MODE        HsSpiClkHighOutFallingInRising  // Mode 3

/// Default SPI protocol mode
#define HS_SPI_DEFAULT_PROTOCOL_MODE   HsSpiProtocolModeLegacy

/** \} GroupHSSPI_Macros */

/**
* \addtogroup GroupHSSPI_Types
* \{
*/
/*****************************************************************************/
/* Global type definitions ('typedef')                                       */
/*****************************************************************************/

/**
******************************************************************************
** \brief High speed SPI reception status.
**
** Status value used either in SpiRxStatus callback function or in
** HsSpi_GetDirectModeRxStatus() function (for direct mode polling operation).
******************************************************************************/
typedef enum en_hsspi_rx_status
{
  HsSpiRxFinished          = 0u,  ///< Receive FIFO and shift register is empty.
  HsSpiRxFifoLow           = 1u,  ///< Receive FIFO fill level is less or equal to threshold.
  HsSpiRxNoStatusAvailable = 2u   ///< SPI instance is not valid, mode is set to command sequencer or
                                  ///< reception is ongoing and FIFO level is higher than threshold.
} en_hsspi_rx_status_t;

/**
******************************************************************************
** \brief High speed SPI transmission status.
**
** Status value used either in SpiTxStatus callback function or in
** HsSpi_GetDirectModeTxStatus() function (for direct mode polling operation).
******************************************************************************/
typedef enum en_hsspi_tx_status
{
  HsSpiModeError            = -1,   ///< Illegal mode or parameter error
  HsSpiTxFinished           = 0u,   ///< Transmit FIFO and shift register is empty.
  HsSpiTxFifoLow            = 1u,   ///< Transmit FIFO fill level is less or equal to threshold.
  HsSpiTxNoStatusAvailable  = 2u    ///< SPI instance is not valid, mode is set to command sequencer or
                                    ///< transmission is ongoing and FIFO level is higher than threshold.
} en_hsspi_tx_status_t;

/**
******************************************************************************
** \brief High speed SPI FIFO width
**
** Width of FIFO. Depending on the configured width, the usable size of the
** shift register in the SPI core also changes.
******************************************************************************/
typedef enum en_hsspi_fifo_width
{
  HsSpiFifoWidth8       = 0u,  ///<  8-bit FiFo width
  HsSpiFifoWidth16      = 1u,  ///< 16-bit FiFo width
  HsSpiFifoWidth24      = 2u,  ///< 24-bit FiFo width
  HsSpiFifoWidth32      = 3u   ///< 32-bit FiFo width
} en_hsspi_fifo_width_t;

/**
*******************************************************************************
** \brief High speed SPI error types.
**
** These error codes will be returned by HsSpi_HandleError() function.
******************************************************************************/
typedef enum en_hsspi_error
{
  HsSpiErrorNo                        = 0u,     ///< No error indicated.
  HsSpiErrorUnmappedMemoryAccessFault = 1u,     ///< An unmapped memory access fault has occurred.
  HsSpiErrorWriteAccessFault          = 2u,     ///< Write access fault has occurred (is used in command sequencer mode).
  HsSpiErrorProtectionViolation       = 3u,     ///< PPU protection violation has occurred.
  HsSpiErrorDmaWrChBlockSize          = 4u,     ///< DMA Write Channel Block Size Fault has occurred.
  HsSpiErrorDmaRdChBlockSize          = 5u      ///< DMA Read Channel Block Size Fault has occurred.
} en_hsspi_error_t;

/**
*******************************************************************************
** \brief High speed SPI receive callback function.
******************************************************************************/
///< Prototype for transmit status callback function
typedef void (*hsspi_rx_func_ptr_t)(en_hsspi_rx_status_t enRxStatus);

/**
*******************************************************************************
** \brief High speed SPI transmit callback function.
******************************************************************************/
///< Prototype for transmit status callback function
typedef void (*hsspi_tx_func_ptr_t)(en_hsspi_tx_status_t enTxStatus);

/**
*******************************************************************************
** \brief High speed SPI clock selection.
**
** The internal clock divider can divide either the AHB clock (i.e. HCLK)
** or the peripheral clock (i.e. PCLKn).
******************************************************************************/
typedef enum en_hsspi_clk_selection
{
  HsSpiClockAhb           = 0u,    ///< Clock divider uses AHB clock.
  HsSpiClockPeripheral    = 1u     ///< Clock divider uses peripheral clock.
} en_hsspi_clk_selection_t;

/**
*******************************************************************************
** \brief High speed SPI bank size.
**
** This enum indicates the range of the AHB address space associated
** with each slave select line. It also indicates the size of each memory
** banks in the selected device.
******************************************************************************/
typedef enum en_hsspi_bank_size
{
  HsSpiBankSize8kB    = 0x0u,  ///<   8 kB
  HsSpiBankSize16kB   = 0x1u,  ///<  16 kB
  HsSpiBankSize32kB   = 0x2u,  ///<  32 kB
  HsSpiBankSize64kB   = 0x3u,  ///<  64 kB
  HsSpiBankSize128kB  = 0x4u,  ///< 128 kB
  HsSpiBankSize256kB  = 0x5u,  ///< 256 kB
  HsSpiBankSize512kB  = 0x6u,  ///< 512 kB
  HsSpiBankSize1MB    = 0x7u,  ///<   1 MB
  HsSpiBankSize2MB    = 0x8u,  ///<   2 MB
  HsSpiBankSize4MB    = 0x9u,  ///<   4 MB
  HsSpiBankSize8MB    = 0xAu,  ///<   8 MB
  HsSpiBankSize16MB   = 0xBu,  ///<  16 MB
  HsSpiBankSize32MB   = 0xCu,  ///<  32 MB
  HsSpiBankSize64MB   = 0xDu,  ///<  64 MB
  HsSpiBankSize128MB  = 0xEu,  ///< 128 MB
  HsSpiBankSize256MB  = 0xFu   ///< 256 MB
} en_hsspi_bank_size_t;

/**
*******************************************************************************
** \brief High speed SPI command sequencer control.
**
** This enum is used to configure the phases of the serial transaction
** generated by the command sequencer for memory read or write operations.
******************************************************************************/
typedef enum en_hsspi_command_sequencer_control
{
  HsSpiCmdTransmitAddress07To00       = 0u,    ///< Transmit address bits 7:0 of serial memory address,          DEC = 1
  HsSpiCmdTransmitAddress08To15       = 1u,    ///< Transmit address bits 15:8 of serial memory address,         DEC = 1
  HsSpiCmdTransmitAddress16To23       = 2u,    ///< Transmit address bits 23:16 of serial memory address,        DEC = 1
  HsSpiCmdTransmitAddress24To31       = 3u,    ///< Transmit address bits 31:24 of serial memory address,        DEC = 1
  HsSpiCmdTransmitHighZStateByte      = 4u,    ///< High-Z byte, output signals are tri-stated for 1 byte time,  DEC = 1
  HsSpiCmdTransmitHighZStateNibble    = 5u,    ///< High-Z nibble, output signals are tri-stated for 1
                                               ///< nibble time after 4 bits of the transmit data have been sent,DEC = 1
  HsSpiCmdTransmitData                = 6u,    ///< Data "as it is", no decoding,                                DEC = 0
  HsSpiCmdEndOfList                   = 7u     ///< End of list,                                                 DEC = 1
} en_hsspi_command_sequencer_control_t;

/**
*******************************************************************************
** \brief High speed SPI clock mode.
**
** The API must use the enum with the attributes that match the communication
** attributes of the serial peripheral that is to be interfaced on the
** corresponding slave select line.
******************************************************************************/
typedef enum en_hsspi_clk_mode
{
  HsSpiClkLowOutFallingInRising   = 0u,  ///< Serial clock low during idle, data output set with falling clock edge, data input sampling with rising clock edge.
  HsSpiClkLowOutRisingInFalling   = 1u,  ///< Serial clock low during idle, data output set with rising clock edge, data input sampling with falling clock edge.
  HsSpiClkHighOutRisingInFalling  = 2u,  ///< Serial clock high during idle, data output set with rising clock edge, data input sampling with falling clock edge.
  HsSpiClkHighOutFallingInRising  = 3u,  ///< Serial clock high during idle, data output set with falling clock edge, data input sampling with rising clock edge.
  HsSpiClkLowOutFallingInFalling  = 4u,  ///< Serial clock low during idle, data output set with falling clock edge, data input sampling with falling clock edge.
  HsSpiClkLowOutRisingInRising    = 5u,  ///< Serial clock low during idle, data output set with rising clock edge, data input sampling with rising clock edge.
  HsSpiClkHighOutRisingInRising   = 6u,  ///< Serial clock high during idle, data output set with rising clock edge, data input sampling with rising clock edge.
  HsSpiClkHighOutFallingInFalling = 7u   ///< Serial clock high during idle, data output set with falling clock edge, data input sampling with falling clock edge.
} en_hsspi_clk_mode_t;

/**
*******************************************************************************
** \brief SPI transfer mode (used for Command Sequencer and Direct mode)
**
** Selects transfer protocol mode. The direct mode is active
** after call of HsSpi_Init, and can be changed by function HsSpi_SetMode().
** RxTx-,RxOnly- and TxOnly modes will be selected automatically by the transfer
** functions.
******************************************************************************/
typedef enum en_hsspi_protocol_mode
{
  HsSpiProtocolModeSingle = 0u,         ///< Single mode
  HsSpiProtocolModeDual   = 1u,         ///< Dual mode, can only be used with the half-duplex functions
  HsSpiProtocolModeQuad   = 2u          ///< Quad mode, can only be used with the half-duplex functions
} en_hsspi_protocol_mode_t;

/**
*******************************************************************************
** \brief SPI transfer direction
******************************************************************************/
typedef enum en_hsspi_protocol_direction
{
  HsSpiProtocolTxRx = 0u,         ///< TX and RX direction
  HsSpiProtocolTx   = 1u,         ///< TX direction only
  HsSpiProtocolRx   = 2u          ///< RX direction only
} en_hsspi_protocol_direction_t;

/**
*******************************************************************************
** \brief High speed SPI memory type.
**
** Serial SRAM or Serial Flash Memory Type Select (SRAM)
** This bit should only be set if serial SRAM devices are memory mapped through
** (HS)SPI.
******************************************************************************/
typedef enum en_hsspi_memory_type
{
  HsSpiFlash  = 0u,    ///< Serial Flash memory devices are connected. Writes are disabled.
  HsSpiSram   = 1u     ///< Serial SRAM memory devices are connected. Writes are enabled.
} en_hsspi_memory_type_t;

/**
*******************************************************************************
** \brief High Speed SPI clock divider.
**
** This field decides the clock division ratio of the internal clock divider.
******************************************************************************/
typedef enum en_hsspi_clk_divider
{
  HsSpiClkDividerDiv1       = 0u,      ///< Clock divider is 1
  HsSpiClkDividerDiv2       = 1u,      ///< Clock divider is 2
  HsSpiClkDividerDiv3       = 2u,      ///< Clock divider is 3
  HsSpiClkDividerDiv4       = 3u,      ///< Clock divider is 4
  HsSpiClkDividerDiv5       = 4u,      ///< Clock divider is 5
  HsSpiClkDividerDiv6       = 5u,      ///< Clock divider is 6
  HsSpiClkDividerDiv7       = 6u,      ///< Clock divider is 7
  HsSpiClkDividerDiv8       = 7u,      ///< Clock divider is 8
  HsSpiClkDividerDiv9       = 8u,      ///< Clock divider is 9
  HsSpiClkDividerDiv10      = 9u,      ///< Clock divider is 10
  HsSpiClkDividerDiv11      = 10u,     ///< Clock divider is 11
  HsSpiClkDividerDiv12      = 11u,     ///< Clock divider is 12
  HsSpiClkDividerDiv13      = 12u,     ///< Clock divider is 13
  HsSpiClkDividerDiv14      = 13u,     ///< Clock divider is 14
  HsSpiClkDividerDiv15      = 14u,     ///< Clock divider is 15
  HsSpiClkDividerDiv16      = 15u,     ///< Clock divider is 16
  HsSpiClkDividerDiv17      = 16u,     ///< Clock divider is 17
  HsSpiClkDividerDiv18      = 17u,     ///< Clock divider is 18
  HsSpiClkDividerDiv19      = 18u,     ///< Clock divider is 19
  HsSpiClkDividerDiv20      = 19u,     ///< Clock divider is 20
  HsSpiClkDividerDiv21      = 20u,     ///< Clock divider is 21
  HsSpiClkDividerDiv22      = 21u,     ///< Clock divider is 22
  HsSpiClkDividerDiv23      = 22u,     ///< Clock divider is 23
  HsSpiClkDividerDiv24      = 23u,     ///< Clock divider is 24
  HsSpiClkDividerDiv25      = 24u,     ///< Clock divider is 25
  HsSpiClkDividerDiv26      = 25u,     ///< Clock divider is 26
  HsSpiClkDividerDiv27      = 26u,     ///< Clock divider is 27
  HsSpiClkDividerDiv28      = 27u,     ///< Clock divider is 28
  HsSpiClkDividerDiv29      = 28u,     ///< Clock divider is 29
  HsSpiClkDividerDiv30      = 29u,     ///< Clock divider is 30
  HsSpiClkDividerDiv31      = 30u,     ///< Clock divider is 31
  HsSpiClkDividerDiv32      = 31u,     ///< Clock divider is 32
  HsSpiClkDividerDiv33      = 32u,     ///< Clock divider is 33
  HsSpiClkDividerDiv34      = 33u,     ///< Clock divider is 34
  HsSpiClkDividerDiv35      = 34u,     ///< Clock divider is 35
  HsSpiClkDividerDiv36      = 35u,     ///< Clock divider is 36
  HsSpiClkDividerDiv37      = 36u,     ///< Clock divider is 37
  HsSpiClkDividerDiv38      = 37u,     ///< Clock divider is 38
  HsSpiClkDividerDiv39      = 38u,     ///< Clock divider is 39
  HsSpiClkDividerDiv40      = 39u,     ///< Clock divider is 40
  HsSpiClkDividerDiv41      = 40u,     ///< Clock divider is 41
  HsSpiClkDividerDiv42      = 41u,     ///< Clock divider is 42
  HsSpiClkDividerDiv43      = 42u,     ///< Clock divider is 43
  HsSpiClkDividerDiv44      = 43u,     ///< Clock divider is 44
  HsSpiClkDividerDiv45      = 44u,     ///< Clock divider is 45
  HsSpiClkDividerDiv46      = 45u,     ///< Clock divider is 46
  HsSpiClkDividerDiv47      = 46u,     ///< Clock divider is 47
  HsSpiClkDividerDiv48      = 47u,     ///< Clock divider is 48
  HsSpiClkDividerDiv49      = 48u,     ///< Clock divider is 49
  HsSpiClkDividerDiv50      = 49u,     ///< Clock divider is 50
  HsSpiClkDividerDiv51      = 50u,     ///< Clock divider is 51
  HsSpiClkDividerDiv52      = 51u,     ///< Clock divider is 52
  HsSpiClkDividerDiv53      = 52u,     ///< Clock divider is 53
  HsSpiClkDividerDiv54      = 53u,     ///< Clock divider is 54
  HsSpiClkDividerDiv55      = 54u,     ///< Clock divider is 55
  HsSpiClkDividerDiv56      = 55u,     ///< Clock divider is 56
  HsSpiClkDividerDiv57      = 56u,     ///< Clock divider is 57
  HsSpiClkDividerDiv58      = 57u,     ///< Clock divider is 58
  HsSpiClkDividerDiv59      = 58u,     ///< Clock divider is 59
  HsSpiClkDividerDiv60      = 59u,     ///< Clock divider is 60
  HsSpiClkDividerDiv61      = 60u,     ///< Clock divider is 61
  HsSpiClkDividerDiv62      = 61u,     ///< Clock divider is 62
  HsSpiClkDividerDiv63      = 62u,     ///< Clock divider is 63
  HsSpiClkDividerDiv64      = 63u,     ///< Clock divider is 64
  HsSpiClkDividerDiv65      = 64u,     ///< Clock divider is 65
  HsSpiClkDividerDiv66      = 65u,     ///< Clock divider is 66
  HsSpiClkDividerDiv67      = 66u,     ///< Clock divider is 67
  HsSpiClkDividerDiv68      = 67u,     ///< Clock divider is 68
  HsSpiClkDividerDiv69      = 68u,     ///< Clock divider is 69
  HsSpiClkDividerDiv70      = 69u,     ///< Clock divider is 70
  HsSpiClkDividerDiv71      = 70u,     ///< Clock divider is 71
  HsSpiClkDividerDiv72      = 71u,     ///< Clock divider is 72
  HsSpiClkDividerDiv73      = 72u,     ///< Clock divider is 73
  HsSpiClkDividerDiv74      = 73u,     ///< Clock divider is 74
  HsSpiClkDividerDiv75      = 74u,     ///< Clock divider is 75
  HsSpiClkDividerDiv76      = 75u,     ///< Clock divider is 76
  HsSpiClkDividerDiv77      = 76u,     ///< Clock divider is 77
  HsSpiClkDividerDiv78      = 77u,     ///< Clock divider is 78
  HsSpiClkDividerDiv79      = 78u,     ///< Clock divider is 79
  HsSpiClkDividerDiv80      = 79u,     ///< Clock divider is 80
  HsSpiClkDividerDiv81      = 80u,     ///< Clock divider is 81
  HsSpiClkDividerDiv82      = 81u,     ///< Clock divider is 82
  HsSpiClkDividerDiv83      = 82u,     ///< Clock divider is 83
  HsSpiClkDividerDiv84      = 83u,     ///< Clock divider is 84
  HsSpiClkDividerDiv85      = 84u,     ///< Clock divider is 85
  HsSpiClkDividerDiv86      = 85u,     ///< Clock divider is 86
  HsSpiClkDividerDiv87      = 86u,     ///< Clock divider is 87
  HsSpiClkDividerDiv88      = 87u,     ///< Clock divider is 88
  HsSpiClkDividerDiv89      = 88u,     ///< Clock divider is 89
  HsSpiClkDividerDiv90      = 89u,     ///< Clock divider is 90
  HsSpiClkDividerDiv91      = 90u,     ///< Clock divider is 91
  HsSpiClkDividerDiv92      = 91u,     ///< Clock divider is 92
  HsSpiClkDividerDiv93      = 92u,     ///< Clock divider is 93
  HsSpiClkDividerDiv94      = 93u,     ///< Clock divider is 94
  HsSpiClkDividerDiv95      = 94u,     ///< Clock divider is 95
  HsSpiClkDividerDiv96      = 95u,     ///< Clock divider is 96
  HsSpiClkDividerDiv97      = 96u,     ///< Clock divider is 97
  HsSpiClkDividerDiv98      = 97u,     ///< Clock divider is 98
  HsSpiClkDividerDiv99      = 98u,     ///< Clock divider is 99
  HsSpiClkDividerDiv100     = 99u,     ///< Clock divider is 100
  HsSpiClkDividerDiv101     = 100u,    ///< Clock divider is 101
  HsSpiClkDividerDiv102     = 101u,    ///< Clock divider is 102
  HsSpiClkDividerDiv103     = 102u,    ///< Clock divider is 103
  HsSpiClkDividerDiv104     = 103u,    ///< Clock divider is 104
  HsSpiClkDividerDiv105     = 104u,    ///< Clock divider is 105
  HsSpiClkDividerDiv106     = 105u,    ///< Clock divider is 106
  HsSpiClkDividerDiv107     = 106u,    ///< Clock divider is 107
  HsSpiClkDividerDiv108     = 107u,    ///< Clock divider is 108
  HsSpiClkDividerDiv109     = 108u,    ///< Clock divider is 109
  HsSpiClkDividerDiv110     = 109u,    ///< Clock divider is 110
  HsSpiClkDividerDiv111     = 110u,    ///< Clock divider is 111
  HsSpiClkDividerDiv112     = 111u,    ///< Clock divider is 112
  HsSpiClkDividerDiv113     = 112u,    ///< Clock divider is 113
  HsSpiClkDividerDiv114     = 113u,    ///< Clock divider is 114
  HsSpiClkDividerDiv115     = 114u,    ///< Clock divider is 115
  HsSpiClkDividerDiv116     = 115u,    ///< Clock divider is 116
  HsSpiClkDividerDiv117     = 116u,    ///< Clock divider is 117
  HsSpiClkDividerDiv118     = 117u,    ///< Clock divider is 118
  HsSpiClkDividerDiv119     = 118u,    ///< Clock divider is 119
  HsSpiClkDividerDiv120     = 119u,    ///< Clock divider is 120
  HsSpiClkDividerDiv121     = 120u,    ///< Clock divider is 121
  HsSpiClkDividerDiv122     = 121u,    ///< Clock divider is 122
  HsSpiClkDividerDiv123     = 122u,    ///< Clock divider is 123
  HsSpiClkDividerDiv124     = 123u,    ///< Clock divider is 124
  HsSpiClkDividerDiv125     = 124u,    ///< Clock divider is 125
  HsSpiClkDividerDiv126     = 125u,    ///< Clock divider is 126
  HsSpiClkDividerDiv127     = 126u,    ///< Clock divider is 127
  HsSpiClkDividerDiv128     = 127u,    ///< Clock divider is 128
  HsSpiClkDividerDiv129     = 128u,    ///< Clock divider is 129
  HsSpiClkDividerDiv130     = 129u,    ///< Clock divider is 130
  HsSpiClkDividerDiv131     = 130u,    ///< Clock divider is 131
  HsSpiClkDividerDiv132     = 131u,    ///< Clock divider is 132
  HsSpiClkDividerDiv133     = 132u,    ///< Clock divider is 133
  HsSpiClkDividerDiv134     = 133u,    ///< Clock divider is 134
  HsSpiClkDividerDiv135     = 134u,    ///< Clock divider is 135
  HsSpiClkDividerDiv136     = 135u,    ///< Clock divider is 136
  HsSpiClkDividerDiv137     = 136u,    ///< Clock divider is 137
  HsSpiClkDividerDiv138     = 137u,    ///< Clock divider is 138
  HsSpiClkDividerDiv139     = 138u,    ///< Clock divider is 139
  HsSpiClkDividerDiv140     = 139u,    ///< Clock divider is 140
  HsSpiClkDividerDiv141     = 140u,    ///< Clock divider is 141
  HsSpiClkDividerDiv142     = 141u,    ///< Clock divider is 142
  HsSpiClkDividerDiv143     = 142u,    ///< Clock divider is 143
  HsSpiClkDividerDiv144     = 143u,    ///< Clock divider is 144
  HsSpiClkDividerDiv145     = 144u,    ///< Clock divider is 145
  HsSpiClkDividerDiv146     = 145u,    ///< Clock divider is 146
  HsSpiClkDividerDiv147     = 146u,    ///< Clock divider is 147
  HsSpiClkDividerDiv148     = 147u,    ///< Clock divider is 148
  HsSpiClkDividerDiv149     = 148u,    ///< Clock divider is 149
  HsSpiClkDividerDiv150     = 149u,    ///< Clock divider is 150
  HsSpiClkDividerDiv151     = 150u,    ///< Clock divider is 151
  HsSpiClkDividerDiv152     = 151u,    ///< Clock divider is 152
  HsSpiClkDividerDiv153     = 152u,    ///< Clock divider is 153
  HsSpiClkDividerDiv154     = 153u,    ///< Clock divider is 154
  HsSpiClkDividerDiv155     = 154u,    ///< Clock divider is 155
  HsSpiClkDividerDiv156     = 155u,    ///< Clock divider is 156
  HsSpiClkDividerDiv157     = 156u,    ///< Clock divider is 157
  HsSpiClkDividerDiv158     = 157u,    ///< Clock divider is 158
  HsSpiClkDividerDiv159     = 158u,    ///< Clock divider is 159
  HsSpiClkDividerDiv160     = 159u,    ///< Clock divider is 160
  HsSpiClkDividerDiv161     = 160u,    ///< Clock divider is 161
  HsSpiClkDividerDiv162     = 161u,    ///< Clock divider is 162
  HsSpiClkDividerDiv163     = 162u,    ///< Clock divider is 163
  HsSpiClkDividerDiv164     = 163u,    ///< Clock divider is 164
  HsSpiClkDividerDiv165     = 164u,    ///< Clock divider is 165
  HsSpiClkDividerDiv166     = 165u,    ///< Clock divider is 166
  HsSpiClkDividerDiv167     = 166u,    ///< Clock divider is 167
  HsSpiClkDividerDiv168     = 167u,    ///< Clock divider is 168
  HsSpiClkDividerDiv169     = 168u,    ///< Clock divider is 169
  HsSpiClkDividerDiv170     = 169u,    ///< Clock divider is 170
  HsSpiClkDividerDiv171     = 170u,    ///< Clock divider is 171
  HsSpiClkDividerDiv172     = 171u,    ///< Clock divider is 172
  HsSpiClkDividerDiv173     = 172u,    ///< Clock divider is 173
  HsSpiClkDividerDiv174     = 173u,    ///< Clock divider is 174
  HsSpiClkDividerDiv175     = 174u,    ///< Clock divider is 175
  HsSpiClkDividerDiv176     = 175u,    ///< Clock divider is 176
  HsSpiClkDividerDiv177     = 176u,    ///< Clock divider is 177
  HsSpiClkDividerDiv178     = 177u,    ///< Clock divider is 178
  HsSpiClkDividerDiv179     = 178u,    ///< Clock divider is 179
  HsSpiClkDividerDiv180     = 179u,    ///< Clock divider is 180
  HsSpiClkDividerDiv181     = 180u,    ///< Clock divider is 181
  HsSpiClkDividerDiv182     = 181u,    ///< Clock divider is 182
  HsSpiClkDividerDiv183     = 182u,    ///< Clock divider is 183
  HsSpiClkDividerDiv184     = 183u,    ///< Clock divider is 184
  HsSpiClkDividerDiv185     = 184u,    ///< Clock divider is 185
  HsSpiClkDividerDiv186     = 185u,    ///< Clock divider is 186
  HsSpiClkDividerDiv187     = 186u,    ///< Clock divider is 187
  HsSpiClkDividerDiv188     = 187u,    ///< Clock divider is 188
  HsSpiClkDividerDiv189     = 188u,    ///< Clock divider is 189
  HsSpiClkDividerDiv190     = 189u,    ///< Clock divider is 190
  HsSpiClkDividerDiv191     = 190u,    ///< Clock divider is 191
  HsSpiClkDividerDiv192     = 191u,    ///< Clock divider is 192
  HsSpiClkDividerDiv193     = 192u,    ///< Clock divider is 193
  HsSpiClkDividerDiv194     = 193u,    ///< Clock divider is 194
  HsSpiClkDividerDiv195     = 194u,    ///< Clock divider is 195
  HsSpiClkDividerDiv196     = 195u,    ///< Clock divider is 196
  HsSpiClkDividerDiv197     = 196u,    ///< Clock divider is 197
  HsSpiClkDividerDiv198     = 197u,    ///< Clock divider is 198
  HsSpiClkDividerDiv199     = 198u,    ///< Clock divider is 199
  HsSpiClkDividerDiv200     = 199u,    ///< Clock divider is 200
  HsSpiClkDividerDiv201     = 200u,    ///< Clock divider is 201
  HsSpiClkDividerDiv202     = 201u,    ///< Clock divider is 202
  HsSpiClkDividerDiv203     = 202u,    ///< Clock divider is 203
  HsSpiClkDividerDiv204     = 203u,    ///< Clock divider is 204
  HsSpiClkDividerDiv205     = 204u,    ///< Clock divider is 205
  HsSpiClkDividerDiv206     = 205u,    ///< Clock divider is 206
  HsSpiClkDividerDiv207     = 206u,    ///< Clock divider is 207
  HsSpiClkDividerDiv208     = 207u,    ///< Clock divider is 208
  HsSpiClkDividerDiv209     = 208u,    ///< Clock divider is 209
  HsSpiClkDividerDiv210     = 209u,    ///< Clock divider is 210
  HsSpiClkDividerDiv211     = 210u,    ///< Clock divider is 211
  HsSpiClkDividerDiv212     = 211u,    ///< Clock divider is 212
  HsSpiClkDividerDiv213     = 212u,    ///< Clock divider is 213
  HsSpiClkDividerDiv214     = 213u,    ///< Clock divider is 214
  HsSpiClkDividerDiv215     = 214u,    ///< Clock divider is 215
  HsSpiClkDividerDiv216     = 215u,    ///< Clock divider is 216
  HsSpiClkDividerDiv217     = 216u,    ///< Clock divider is 217
  HsSpiClkDividerDiv218     = 217u,    ///< Clock divider is 218
  HsSpiClkDividerDiv219     = 218u,    ///< Clock divider is 219
  HsSpiClkDividerDiv220     = 219u,    ///< Clock divider is 220
  HsSpiClkDividerDiv221     = 220u,    ///< Clock divider is 221
  HsSpiClkDividerDiv222     = 221u,    ///< Clock divider is 222
  HsSpiClkDividerDiv223     = 222u,    ///< Clock divider is 223
  HsSpiClkDividerDiv224     = 223u,    ///< Clock divider is 224
  HsSpiClkDividerDiv225     = 224u,    ///< Clock divider is 225
  HsSpiClkDividerDiv226     = 225u,    ///< Clock divider is 226
  HsSpiClkDividerDiv227     = 226u,    ///< Clock divider is 227
  HsSpiClkDividerDiv228     = 227u,    ///< Clock divider is 228
  HsSpiClkDividerDiv229     = 228u,    ///< Clock divider is 229
  HsSpiClkDividerDiv230     = 229u,    ///< Clock divider is 230
  HsSpiClkDividerDiv231     = 230u,    ///< Clock divider is 231
  HsSpiClkDividerDiv232     = 231u,    ///< Clock divider is 232
  HsSpiClkDividerDiv233     = 232u,    ///< Clock divider is 233
  HsSpiClkDividerDiv234     = 233u,    ///< Clock divider is 234
  HsSpiClkDividerDiv235     = 234u,    ///< Clock divider is 235
  HsSpiClkDividerDiv236     = 235u,    ///< Clock divider is 236
  HsSpiClkDividerDiv237     = 236u,    ///< Clock divider is 237
  HsSpiClkDividerDiv238     = 237u,    ///< Clock divider is 238
  HsSpiClkDividerDiv239     = 238u,    ///< Clock divider is 239
  HsSpiClkDividerDiv240     = 239u,    ///< Clock divider is 240
  HsSpiClkDividerDiv241     = 240u,    ///< Clock divider is 241
  HsSpiClkDividerDiv242     = 241u,    ///< Clock divider is 242
  HsSpiClkDividerDiv243     = 242u,    ///< Clock divider is 243
  HsSpiClkDividerDiv244     = 243u,    ///< Clock divider is 244
  HsSpiClkDividerDiv245     = 244u,    ///< Clock divider is 245
  HsSpiClkDividerDiv246     = 245u,    ///< Clock divider is 246
  HsSpiClkDividerDiv247     = 246u,    ///< Clock divider is 247
  HsSpiClkDividerDiv248     = 247u,    ///< Clock divider is 248
  HsSpiClkDividerDiv249     = 248u,    ///< Clock divider is 249
  HsSpiClkDividerDiv250     = 249u,    ///< Clock divider is 250
  HsSpiClkDividerDiv251     = 250u,    ///< Clock divider is 251
  HsSpiClkDividerDiv252     = 251u,    ///< Clock divider is 252
  HsSpiClkDividerDiv253     = 252u,    ///< Clock divider is 253
  HsSpiClkDividerDiv254     = 253u,    ///< Clock divider is 254
  HsSpiClkDividerDiv255     = 254u,    ///< Clock divider is 255
  HsSpiClkDividerDiv256     = 255u,    ///< Clock divider is 256
} en_hsspi_clk_divider_t;


/**
*******************************************************************************
** \brief High Speed SPI clock delay.
**
** Slave-Select to Clock Delay of Peripheral 0 (SS2CD[1:0]).
** This bit is used only when HS-SPI is configured as SPI master in
** direct mode or in command sequencer mode.
** It defines a setup time for the slave device. By delaying the toggling
** of SCLK, SPI delays the data transmission (of slave) from the chip
** select active edge by a multiple of SCLK cycles.
**
** If SPIn_PCC0~3:CPHA = '0', the delay between assertion of slave select and
** first edge on the SCLK is given by:
** (SS2CD + 0.5) number of clock periods of SCLK.
** If SPIn_PCC0~3:CPHA = '1', the delay between assertion of slave select and
** first edge on the SCLK is given by:
** (SS2CD) number of clock periods of SCLK.
**
** When the slave select becomes active, the slave has to prepare
** data transfer within the delay time de?ned by SS2CD bits.
******************************************************************************/
typedef enum en_hsspi_clk_delay
{
  HsSpiClkStart1ClkAfterSlaveSelect  = 0u,
  HsSpiClkStart2ClkAfterSlaveSelect  = 1u,
  HsSpiClkStart3ClkAfterSlaveSelect  = 2u,
  HsSpiClkStart4ClkAfterSlaveSelect  = 3u
} en_hsspi_clk_delay_t;

/**
******************************************************************************
** \brief High speed SPI mode.
**
** Used to switch between direct and command sequencer mode.
******************************************************************************/
typedef enum en_hsspi_mode
{
  HsSpiModeDirect         = 0u,    ///< Direct mode is enabled, command sequencer is disabled.
  HsSpiModeCmdSequencer   = 1u     ///< Command sequencer is enabled, direct mode is disabled.
} en_hsspi_mode_t;

/**
******************************************************************************
** \brief High speed SPI endianess
******************************************************************************/
typedef enum en_hsspi_endianess
{
  HsSpiBigEndian          = 0u,     ///< Big Endian
  HsSpiLittleEndian       = 1u      ///< Little Endian
} en_hsspi_endianess_t;

/**
******************************************************************************
** \brief High speed SPI transfer protocol.
******************************************************************************/
typedef enum en_hsspi_transfer_protocol
{
  HsSpiSerialIf      = 0u,      ///< Serial interface bit width complies with HSSPIn_CSCFG:MDM[1:0] setting
  HsSpiDualBitMode   = 1u,      ///< Dual-bit mode
  HsSpiQuadBitMode   = 2u,      ///< Quad-bit mode
  HsSpiSingleBitMode = 3u       ///< Single-bit mode
} en_hsspi_transfer_protocol_t;

/// Enumeration to define an index for each enabled HS-SPI instance
typedef enum en_hsspi_instance_index
{
#if (PDL_PERIPHERAL_ENABLE_HSSPI0 == PDL_ON)
  HsSpiInstanceIndexHsSpi0,
#endif
  HsSpiInstanceIndexMax
} en_hsspi_instance_index_t;

/** \}GroupHSSPI_Types */

/**
* \addtogroup GroupHSSPI_DataStructures
* \{
*/

/**
******************************************************************************
** \brief (High speed) SPI configuration.
**
** Struct used for initialisation of SSPI module in direct mode, direct and
** command sequence related parameter can be configured at once, afterwards it
** is possible to switch between modes with HsSpi_SetMode().
******************************************************************************/
typedef struct stc_hsspi_config
{
  en_hsspi_clk_divider_t   enClockDivider;                 ///< See description of #en_hsspi_clk_divider_t.
  en_hsspi_clk_selection_t enClockSelection;               ///< See description of #en_hsspi_clk_selection_t.
  uint8_t                  u8TxFifoThresholdLow;           ///< Possible values from 0 to 15 (maximum FIFO depth is 16).
  hsspi_tx_func_ptr_t      pfnTxStatusCallback;            ///< Callback function for transmission status.
  en_hsspi_bank_size_t     enMemoryBankSize;               ///< See description of #en_hsspi_bank_size_t.
  en_hsspi_protocol_mode_t enDirectModeProtocol;           ///< See description of #en_hsspi_protocol_mode_t.
  en_hsspi_protocol_mode_t enCommandSequencerModeProtocol; ///< See description of #en_hsspi_protocol_mode_t.
  en_hsspi_memory_type_t   enMemoryType;                   ///< See description of #en_hsspi_memory_type_t.
  uint16_t                 u16IdleTimeOut;                 ///< The idle timeout interval is in terms of the AHB clock period. Slave select will stay asserted for this time in command sequencer mode.
  en_hsspi_fifo_width_t    enFifoWidth;                    ///< See description of #en_hsspi_fifo_width
  boolean_t                bMasterOperation;               ///< TRUE: selects HS-SPI interface operates in Master Mode (default), FALSE: operating in Slave mode (only to be used in single bit mode)
  boolean_t                bTxDmaBridgeEnable;             ///< TRUE: enables TX DMA bridge, FALSE: disable TX DMA bridge
  boolean_t                bRxDmaBridgeEnable;             ///< TRUE: enables RX DMA bridge, FALSE: disable RX DMA bridge
} stc_hsspi_config_t;

/**
******************************************************************************
** \brief (High Speed) SPI external device configuration.
**
** This struct is used to configure 1 of 4 possible devices by calling the
** HsSpi_SetExternalDeviceConfig() function.
******************************************************************************/
typedef struct stc_hsspi_ext_device_config
{
  en_hsspi_clk_divider_t enClockDivider;           ///< See description of #en_hsspi_clk_divider_t.
  boolean_t              bShiftLsbFirst;           ///< TRUE: Least significant bit is transmitted first, FALSE: most significant bit is transmitted first.
  en_hsspi_clk_delay_t   enClockDelay;             ///< See description of #en_hsspi_clk_delay_t.
  boolean_t              bSlaveSelectPolarityHigh; ///< FALSE: Slave select is low-active and held during default state, TRUE: Slave select is high-active and held low during default state
  en_hsspi_clk_mode_t    enClockMode;              ///< See description of #en_hsspi_clk_mode_t.
  boolean_t              bSafeSync;                ///< SAFESYNC bit set or not.
  en_hsspi_endianess_t   enEndianess;              ///< Endianess, see #en_hsspi_endianess_t for details.
  uint8_t                u8ReadDeselectTime;       ///< Read Deselect Time. Allowed values: 1 ... 4 serial clock times
  uint8_t                u8WriteDeselectTime;      ///< Write/different Command Deselect Time. Allowed values: 1 ... 16 serial clock times.
  boolean_t              bCompensatedClock;        ///< TRUE: reception by time-compensated clock, FALSE: No time-compensation
} stc_hsspi_ext_device_config_t;

/**
******************************************************************************
** \brief (High Speed) SPI command sequencer control command.
**
** Used to configure the command sequence with defined parameters or with user
** defined data values (by setting enCommand to #HsSpiCmdTransmitData).
******************************************************************************/
typedef struct stc_hsspi_command_sequencer_control
{
  en_hsspi_command_sequencer_control_t enCommand;  ///< See description of #en_hsspi_command_sequencer_control_t.
  uint8_t                              u8Data;     ///< 4 bits must be evaluated in case of SpiCmdTransmitHighZStateNibble, otherwise it is evaluated if enCommand is SpiCmdTransmitData.
  boolean_t                            bCont;      ///< TRUE: Omit the sequence list, FALSE: Do not omit the list
  en_hsspi_transfer_protocol_t         enTransferProtocol; ///< see #en_hsspi_transfer_protocol_t for details
} stc_hsspi_command_sequencer_control_t;

/**
******************************************************************************
** \brief (High Speed) SPI linked data list.
**
** Used to connect various different buffer sources that shall be transmitted
** during the same transaction, e.g. Ethernet packet with different buffers for
** for the ISO/OSI layer headers (external Ethernet MAC connected via HS-SPI).
******************************************************************************/
typedef struct stc_hsspi_linked_data_list
{
  const uint8_t*                     pu8Data;              ///< Pointer to transfer data
  uint16_t                           u16ListItemDataSize;  ///< Size of transfer data
  struct stc_hsspi_linked_data_list* pstcNext;             ///< Pointer to next #stc_hsspi_linked_data_list_t item or NULL if end of list
} stc_hsspi_linked_data_list_t;


/// HS-SPI instance internal data, storing internal information for each enabled HS-SPI instance.
typedef struct stc_hsspi_intern_data
{
  hsspi_rx_func_ptr_t       pfnRxStatusCallbackFunction;
  hsspi_tx_func_ptr_t       pfnTxStatusCallbackFunction;
  uint8_t*                  pu8RXBuffer;
  uint8_t                   u8BkRXFTH;
  uint8_t                   u8BkRFMTC;
  en_hsspi_clk_mode_t       enClkMode;
  en_hsspi_protocol_mode_t  enDirectModeProtocol;
  boolean_t                 bMasterOperation;
} stc_hsspi_intern_data_t;

/// HS-SPI instance data type
typedef struct stc_hsspi_instance_data
{
  volatile stc_hsspin_t*  pstcInstance;  ///< pointer to registers of an instance
  boolean_t               bIsHsSpi;      ///< Is HSSPI type peripheral
  stc_hsspi_intern_data_t stcInternData; ///< module internal data of instance
} stc_hsspi_instance_data_t;

/** \} GroupHSSPI_DataStructures */

/**
* \addtogroup GroupHSSPI_GlobalVariables
* \{
*/ 
/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/

/// Look-up table for all enabled ADC instances and their internal data
extern stc_hsspi_instance_data_t m_astcHsSpiInstanceDataLut[HS_SPI_INSTANCE_COUNT];
/** \} GroupHSSPI_GlobalVariables */

/**
* \addtogroup GroupHSSPI_Functions
* \{
*/ 
/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/
extern en_result_t HsSpi_Init(volatile stc_hsspin_t* pstcHsSpi, const stc_hsspi_config_t* pstcConfig);
extern en_result_t HsSpi_DeInit(volatile stc_hsspin_t* pstcHsSpi);
extern en_result_t HsSpi_SetMode(volatile stc_hsspin_t* pstcHsSpi, en_hsspi_mode_t enMode, en_hsspi_protocol_mode_t enProtocolMode, en_hsspi_protocol_direction_t enProtocolDirection);
extern en_result_t HsSpi_SetExternalDeviceConfig(volatile stc_hsspin_t* pstcHsSpi, uint8_t u8DeviceNumber, const stc_hsspi_ext_device_config_t* pstcConfig);
extern en_result_t HsSpi_DirectModeTransferHalfDuplex(volatile stc_hsspin_t* pstcHsSpi, uint8_t u8DeviceNumber, const uint8_t* pu8TxData, uint16_t u16TxSize, uint8_t* pu8RxData, uint16_t u16RxSize, boolean_t bEnableTxDmaRequest);
extern en_result_t HsSpi_DirectModeTransferHalfDuplexList(volatile stc_hsspin_t* pstcHsSpi, uint8_t u8DeviceNumber, const stc_hsspi_linked_data_list_t* pstcTxData, uint8_t* pu8RxData, uint16_t u16RxSize, boolean_t bEnableTxDmaRequest);
extern en_result_t HsSpi_MultiModeTransfer(volatile stc_hsspin_t* pstcHsSpi, uint8_t u8DeviceNumber, const uint8_t* pu8LegacyData, uint8_t u8LegacyTxSize, const uint8_t* pu8TxData, uint8_t u8TxSize, uint8_t* pu8RxData, uint8_t u8RxSize);
extern en_result_t HsSpi_SetReadCommandSequence(volatile stc_hsspin_t* pstcHsSpi, const stc_hsspi_command_sequencer_control_t* pstcCmdSequence);
extern en_result_t HsSpi_SetWriteCommandSequence(volatile stc_hsspin_t* pstcHsSpi, const stc_hsspi_command_sequencer_control_t* pstcCmdSequence);

/** \} GroupHSSPI_Functions */
/** \} GroupHSSPI */
#endif // #if (defined(PDL_PERIPHERAL_HSSPI_ACTIVE))

#ifdef __cplusplus
}
#endif

#endif /* __HSSPI_H__ */
/*****************************************************************************/
/* EOF (not truncated)                                                       */
/*****************************************************************************/
