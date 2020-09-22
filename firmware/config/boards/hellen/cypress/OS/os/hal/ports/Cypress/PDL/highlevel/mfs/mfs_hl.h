/*******************************************************************************
* Copyright (C) 2013 Spansion LLC. All Rights Reserved. 
*
* This software is owned and published by: 
* Spansion LLC, 915 DeGuigne Dr. Sunnyvale, CA  94088-3453 ("Spansion").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND 
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with Spansion 
* components. This software is licensed by Spansion to be adapted only 
* for use in systems utilizing Spansion components. Spansion shall not be 
* responsible for misuse or illegal use of this software for devices not 
* supported herein.  Spansion is providing this software "AS IS" and will 
* not be responsible for issues arising from incorrect user implementation 
* of the software.  
*
* SPANSION MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS), 
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING, 
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED 
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED 
* WARRANTY OF NONINFRINGEMENT.  
* SPANSION SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT, 
* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT 
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, 
* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR 
* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA, 
* SAVINGS OR PROFITS, 
* EVEN IF SPANSION HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 
* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED 
* FROM, THE SOFTWARE.  
*
* This software may be replicated in part or whole for the licensed use, 
* with the restriction that this Disclaimer and Copyright notice must be 
* included with each copy of this software, whether used in part or whole, 
* at all times.  
*/
/******************************************************************************/
/** \file mfs_hl.h
 **
 ** Headerfile for MFS High Level functions
 **
 ** History:
 **   - 2013-03-26  1.0  NT   First version.
 **   - 2014-05-26  1.1  EZ   Add a function to get the pointer of TX or RX 
 **                           buffer.
 **   - 2014-05-28  1.2  MWi  Doxygen comments corrected.
 **
 ******************************************************************************/

#ifndef __MFS_HL_H__
#define __MFS_HL_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mfs/mfs.h"

#if (defined(PDL_PERIPHERAL_MFS_ACTIVE))

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup MfshlGroup MFS High Level Functions (MFS_HL)
 **
 ** Provided functions of MFS_HL module:
 ** 
 ** - Mfs_Hl_Uart_Init()
 ** - Mfs_Hl_Uart_DeInit() 
 ** - Mfs_Hl_Csio_Init() 
 ** - Mfs_Hl_Csio_DeInit()
 ** - Mfs_Hl_Csio_SynchronousTrans()
 ** - Mfs_Hl_Lin_Init()
 ** - Mfs_Hl_Lin_DeInit()
 ** - Mfs_Hl_Lin_SetBreak()
 ** - Mfs_Hl_Lin_SetNewBaudDivisor()
 ** - Mfs_Hl_Lin_TransferRxBuffer()
 ** - Mfs_Hl_Lin_DisableRxInterrupt()
 ** - Mfs_Hl_I2c_Init()
 ** - Mfs_Hl_I2c_DeInit()
 ** - Mfs_Hl_I2c_Write()
 ** - Mfs_Hl_I2c_Read()
 ** - Mfs_Hl_I2c_WaitTxComplete()
 ** - Mfs_Hl_I2c_WaitRxComplete()
 ** - Mfs_Hl_Read()
 ** - Mfs_Hl_Write()
 ** - Mfs_Hl_GetBufferPointer()
 **
 ** \brief How to use MFS High Level module
 **
 ** Mfs_Hl_Uart_Init() is used to initialize an MFS instance to UART mode with
 ** parameter pstcConfig of type #stc_mfs_hl_uart_config_t.
 ** Mfs_Hl_Uart_DeInit() is used to reset all MFS UART related register.
 ** So this function is used after initialization by Mfs_Hl_Uart_Init().
 **
 ** Mfs_Hl_Csio_Init() is used to initialize an MFS instance to CSIO mode with
 ** parameter pstcConfig of type #stc_mfs_hl_csio_config_t.
 ** This function can be able to set timer mode or SPI mode using chip select
 ** (CS) with parameter pstcConfig->pstcMfsSpiConfig of type 
 ** #stc_mfs_hl_spi_config_t.
 ** Mfs_Hl_Csio_DeInit() is used to reset all MFS CSIO related register.
 ** So this function is used after initialization by Mfs_Hl_Csio_Init().
 ** Mfs_Hl_Csio_SynchronousTrans() is used to transfer and receive data
 ** simultaneously. This function is only used by blocking. So interrupt is
 ** not used for this function.
 **
 ** Mfs_Hl_Lin_Init() is used to initialize an MFS instance to LIN mode with
 ** its dedicated LIN configuration (#stc_mfs_hl_lin_config_t).
 ** Mfs_Hl_Lin_DeInit() is used to reset all MFS LIN related register.
 ** So this function is used after initialization by Mfs_Hl_Lin_Init().
 ** Mfs_Hl_Lin_SetBreak() set the LIN Break in LIN Master mode. The baud rate
 ** divisor (not the rate itself!) can be adjusted by Mfs_Hl_Lin_SetNewBaudDivisor()
 ** after measurement with a dedicated ICU in LIN Slave mode.
 ** Note that the LIN functionality only works properly when the MFS is
 ** connected to a LIN transceiver, which means, that the SOT line as always
 ** read-back by SIN input!
 ** Mfs_Hl_Lin_DisableRxInterrupt() is used to disable the Rx interrupt, if a 
 ** LIN frame was completely read and a new frame beginning with the LIN break
 ** is awaited to avoid unnecessary reception of a '0'-Byte with a framing
 ** error.
 ** Mfs_Hl_Lin_TransferRxBuffer() transfers the reception data from the internal
 ** ring buffer to a user buffer. This function can be used for LIN Master and
 ** Slave mode, because of the external LIN transceiver, every data
 ** (transmission and/or reception) is always read completely to the reception
 ** buffer.
 **
 ** Mfs_Hl_I2c_Init() is used to initialize an MFS instance to I2C mode with
 ** parameter pstcConfig of type #stc_mfs_hl_i2c_config_t.
 ** Mfs_Hl_I2c_DeInit() is used to reset all MFS I2C related register.
 ** So this function is used after initialization by Mfs_Hl_I2c_Init().
 ** Mfs_Hl_I2c_Write() is used to transmit data, and Mfs_Hl_I2c_Read() is used to
 ** receive data on I2C mode. These function can use synchronously (blocking-call)
 ** or asynchronously(non-blocking-call). If user wants to use it synchronously,
 ** the parameter bBlocking of Mfs_Hl_I2c_Write() or/and Mfs_Hl_I2c_Read()
 ** should specified TRUE. Otherwise, if user wants to use it asynchronously,
 ** the parameter bBlocking of Mfs_Hl_I2c_Write() or/and Mfs_Hl_I2c_Read()
 ** should specified FALSE.
 ** These functions are used for I2C mode. But Mfs_Hl_Read() and Mfs_Hl_Write() 
 ** can't use for I2C.
 ** Mfs_Hl_I2c_WaitTxComplete() and Mfs_Hl_I2c_WaitRxComplete() are used to check
 ** complete transmission or receiption.
 ** If Mfs_Hl_I2c_Write() is used by non-blocking, please call Mfs_Hl_I2c_WaitTxComplete()
 ** at the application periodically because execution status doesn't change to
 ** standby if stop condition isn't detected.
 ** If Mfs_Hl_I2c_Read() is used by non-blocking, please call Mfs_Hl_I2c_WaitRxComplete()
 ** at the application periodically because execution status doesn't change to
 ** standby if stop condition isn't detected.
 **
 ** For UART, CSIO or LIN, Mfs_Hl_Read() and Mfs_Hl_Write() can use for communication.
 ** See the description of these functions for detail.
 **
 ******************************************************************************/
//@{

/**
 ******************************************************************************
 ** \brief MFS_HL callback function prototypes.
 ******************************************************************************/
typedef void (*mfs_hl_tx_cb_func_ptr_t)(uint16_t);
typedef void (*mfs_hl_rx_cb_func_ptr_t)(uint16_t);
typedef uint8_t (*mfs_hl_i2c_slv_cb_func_prt_t)(uint8_t);
typedef void (*mfs_hl_lin_brk_func_ptr_t)(void);

/*****************************************************************************/
/* Gloval pre-processor symbols/macros ('#define")                           */
/*****************************************************************************/
/**
 ******************************************************************************
 ** \brief MFS_HL mode group
 ******************************************************************************/
#define MFS_HL_MODE_GROUP_CHK   (0xF0u)
#define MFS_HL_MODE_GROUP_UART  (0x10u)
#define MFS_HL_MODE_GROUP_CSIO  (0x20u)
#define MFS_HL_MODE_GROUP_I2C   (0x30u)
#define MFS_HL_MODE_GROUP_LIN   (0x40u)

/******************************************************************************
 * Global type definitions
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief MFS_HL FIFO usage
 ******************************************************************************/
typedef enum en_mfs_hl_fifo_usage
{
    MfsHlUseNoFifo = 0,         ///< Don't use MFS FIFO function.
    MfsHlUseFifo   = 1          ///< Use MFS FIFO function.
} en_mfs_hl_fifo_usage_t;

/**
 ******************************************************************************
 ** \brief MFS_HL Execution status
 ******************************************************************************/
typedef enum en_mfs_hl_exec
{
    MfsHlExecStby          = 0, ///< MFS doesn't active.
    MfsHlExecReceiving     = 1, ///< MFS is performing reception.
    MfsHlExecTransmitting  = 2, ///< MFS is performing transmission.
    MfsHlExecWaitTxBusIdle = 3  ///< MFS is waiting TX bus idle.
} en_mfs_hl_exec_t;

/**
 ******************************************************************************
 ** \brief MFS_HL MFS mode
 ******************************************************************************/
typedef enum en_mfs_hl_mfs_mode
{
    MfsHlModeStby                  = 0x00,      ///< MFS is no active
    MfsHlModeUartNormal            = 0x10,      ///< UART normal mode
    MfsHlModeUartMulti             = 0x11,      ///< UART multi-processor mode
    MfsHlModeCsioNormalMaster      = 0x20,      ///< CSIO normal mode/master
    MfsHlModeCsioNormalMasterTimer = 0x21,      ///< CSIO normal mode/master/use serial timer
    MfsHlModeCsioNormalSlave       = 0x22,      ///< CSIO normal mode/slave
    MfsHlModeCsioNormalSlaveTimer  = 0x23,      ///< CSIO normal mode/slave/use serial timer
    MfsHlModeCsioSpiMaster         = 0x28,      ///< CSIO SPI mode/master
    MfsHlModeCsioSpiMasterWithCs   = 0x29,      ///< CSIO SPI mode/master/use chip select
    MfsHlModeCsioSpiSlave          = 0x2A,      ///< CSIO SPI mode/slave
    MfsHlModeCsioSpiSlaveWithCs    = 0x2B,      ///< CSIO SPI mode/slave/use chip select
    MfsHlModeI2cMaster             = 0x30,      ///< I2C mode/master
    MfsHlModeI2cSlave              = 0x31,      ///< I2C mode/slave
    MfsHlModeI2cMasterFast         = 0x32,      ///< I2C mode/master/Fast mode
    MfsHlModeI2cSlaveFast          = 0x33,      ///< I2C mode/master/Fast mode
    MfsHlModeLinMaster             = 0x40,      ///< LIN mode/master
    MfsHlModeLinSlave              = 0x41       ///< LIN mode/slave
} en_mfs_hl_mfs_mode_t;

/**
 ******************************************************************************
 ** \brief MFS_HL I2C wait selection
 ******************************************************************************/
typedef enum en_mfs_hl_i2c_wait_select
{
    MfsHlI2cWaitAfterAck      = 0,  ///< Waits (9bits) after acknowledgement.
    MfsHlI2cWaitAfterDataTxRx = 1   ///< Waits (8bits) after data transmission or reception.
} en_mfs_hl_i2c_wait_select_t;

/**
 ******************************************************************************
 ** \brief MFS_HL I2C interrupt status
 ******************************************************************************/
typedef enum en_mfs_hl_i2c_int_state
{
    MfsHlI2cIntStateStby     = 0,       ///< Non interruption
    MfsHlI2cIntStateEndData  = 1,       ///< End to transmit data
    MfsHlI2cIntStateNack     = 2,       ///< Receive NACK
    MfsHlI2cIntStateStopCond = 3,       ///< Stop condition
    MfsHlI2cIntStateErr      = 0xEE     ///< Error
} en_mfs_hl_i2c_int_state_t;

/**
 ******************************************************************************
 ** \brief MFS_HL ring buffer.
 ** 
 ** Contains all parameter for ring buffer handling. 
 ******************************************************************************/
typedef struct stc_mfs_hl_buffer
{
    uint8_t*  pu8Buffer;        ///< Pointer to communication buffer.
    uint16_t  u16BufferSize;    ///< Size of buffer.
    uint16_t  u16InIndex;       ///< Index of next element to store on buffer.
    uint16_t  u16OutIndex;      ///< Index of next element to read from buffer.
    uint16_t  u16FillCount;     ///< Indicates if elements are available in buffer.
    uint16_t  u16TxCounter;     ///< Transmitted size
    uint16_t  u16TxCount;       ///< Transfer size for timer mode.
    boolean_t bOverflow;        ///< TRUE: Indicates Overrun Condition
} stc_mfs_hl_buffer_t;

/**
 ******************************************************************************
 ** \brief MFS_HL UART configuration.
 **
 ** Contains all parameter for configuratin a MFS channel, if set to UART mode.
 ******************************************************************************/
typedef struct stc_mfs_hl_uart_config
{
    uint32_t                u32DataRate;        ///< Bits per second
    boolean_t               bBitDirection;      ///< TRUE: MSB first, FALSE: LSB first
    boolean_t               bSignalSystem;      ///< TRUE: Inverted NRZ, FALSE: NRZ
    boolean_t               bHwFlow;            ///< TRUE: Use Hardware Flow
    uint8_t*                pu8TxBuf;           ///< Pointer to tranasmit FIFO buffer.
    uint8_t*                pu8RxBuf;           ///< Pointer to receive FIFO buffer.
    uint16_t                u16TxBufSize;       ///< Size of transmit FIFO buffer.
    uint16_t                u16RxBufSize;       ///< Size of receive FIFO buffer.
    uint16_t                u16RxCbBufFillLvl;  ///< Unread counts of data buffer to call RX Callback function
    uint8_t                 u8UartMode;         ///< Uart mode, see description of #en_mfs_uart_mode_t on mfs.h
    uint8_t                 u8Parity;           ///< Parity, see description of #en_mfs_parity_t on mfs.h
    uint8_t                 u8StopBit;          ///< Stop bit, see description of #en_mfs_stopbit_t on mfs.h
    uint8_t                 u8CharLength;       ///< 5..9 Bit Character Length, see description of #en_mfs_characterlength_t on mfs.h
    uint8_t                 u8FifoUsage;        ///< Usage of MFS FIFO, see description of #en_mfs_hl_fifo_usage_t
    mfs_hl_rx_cb_func_ptr_t pfnRxCb;            ///< Callback function, if RX Buffer is filled more than u16RxCbBufFillLvl
    mfs_hl_tx_cb_func_ptr_t pfnTxCb;            ///< Callback function, if TX Buffer is empty
} stc_mfs_hl_uart_config_t;

/**
 ******************************************************************************
 ** \brief MFS_HL SPI configuration.
 **
 ** Contains all parameter for configuratin a MFS channel, if set to CSIO mode.
 ******************************************************************************/
typedef struct stc_mfs_hl_spi_config
{
    boolean_t bCsLevel;         /**< Chip select active level
                                     - TRUE: Chip select active HIGH(in-active LOW)
                                     - FALSE: Chip select active LOW (in-active HIGH) */
    uint16_t  u16CsDeSelect;    ///< Chip de-select bit
    uint8_t   u8CsSetDelay;     ///< Chip select setup delay
    uint8_t   u8CsHoldDelay;    ///< Chip select hold delay
    uint8_t   u8CsDivision;     ///< Setting for Chip select timing divider
} stc_mfs_hl_spi_config_t;

/**
 ******************************************************************************
 ** \brief MFS_HL Timer mode configuration.
 **
 ** Contains all parameter for configuratin a MFS channel, if set to CSIO mode.
 ******************************************************************************/
typedef struct stc_mfs_hl_timer_config
{
    boolean_t bTimerSyncEnable; ///< TRUE: Enable synchronous tansfer, FALSE: Disable synchronous transfer
    uint16_t  u16SerialTimer;   ///< Serial timer value. If chip select disables and this value is not zero serial timer is activate.
    uint8_t   u8TimerDivision;  ///< Setting for serial timer divider
    uint8_t   u8TxByte;         ///< Transfer length when timer mode. This is used if bTimerSyncEnable is TRUE.
} stc_mfs_hl_timer_config_t;

/**
 ******************************************************************************
 ** \brief MFS_HL CSIO configuration.
 **
 ** Contains all parameter for configuratin a MFS channel, if set to CSIO mode.
 ******************************************************************************/
typedef struct stc_mfs_hl_csio_config
{
    uint32_t                   u32DataRate;         ///< Baud rate (bps)
    boolean_t                  bBitDirection;       ///< TRUE: MSB first, FALSE: LSB first
    boolean_t                  bSignalSystem;       ///< TRUE: SCK Mark Level Low, FALSE: SCK Mark Level High
    stc_mfs_hl_spi_config_t*   pstcMfsSpiCsConfig;  ///< See description of #stc_mfs_hl_spi_config_t. Only applicable when u8CsioActMode is MfsCsioActSpiMode
    stc_mfs_hl_timer_config_t* pstcMfsTimerConfig;  ///< See description of #stc_mfs_hl_timer_config_t. Only applicable when u8CsioMode is MfsCsioMaster.
    uint8_t*                   pu8TxBuf;            ///< Data store for transmit buffer.
    uint8_t*                   pu8RxBuf;            ///< Char store for receive buffer.
    uint16_t                   u16TxBufSize;        ///< Size of transmit fifo buffer.
    uint16_t                   u16RxBufSize;        ///< Size of receive fifo buffer.
    uint16_t                   u16RxCbBufFillLvl;   ///< Unread counts of data buffer to call RX Callback function
    uint8_t                    u8CsioMode;          ///< CSIO mode, see description of #en_mfs_csio_mode_t on mfs.h
    uint8_t                    u8CsioActMode;       ///< CSIO active mode, see description of #en_mfs_csio_act_mode_t on mfs.h
    uint8_t                    u8SyncWaitTime;      ///< Sync wait time insersion, see description of #en_mfs_csio_sync_wait_time_t on mfs.h
    uint8_t                    u8CharLength;        ///< 5..32 Bit Character Length, see description of #en_mfs_characterlength_t
    uint8_t                    u8FifoUsage;         ///< Usage of MFS FIFO, see description of #en_mfs_hl_fifo_usage_t
    mfs_hl_rx_cb_func_ptr_t    pfnRxCb;             ///< Callback function, if RX Buffer is filled specified bytes of u16RxCbBufFillLvl
    mfs_hl_tx_cb_func_ptr_t    pfnTxCb;             ///< Callback function, if TX Buffer is empty
} stc_mfs_hl_csio_config_t;

/**
 ******************************************************************************
 ** \brief MFS_HL I2C configuration.
 **
 ** Contains all parameter for configuratin a MFS channel, if set to I2C mode.
 ******************************************************************************/
typedef struct stc_mfs_hl_i2c_config
{
    uint32_t                     u32DataRate;   ///< Baud rate (bps)
    uint8_t                      u8I2cMode;     ///< I2C mode, see description of #en_mfs_i2c_mode_t on mfs.h
    uint8_t                      u8SlvAddr;     ///< Slave address (This is effective on slave mode.)
    uint8_t                      u8FastMode;    ///< Fast mode, see description of #en_mfs_i2c_fast_mode_t. on mfs.h
    uint8_t                      u8FifoUsage;   ///< Usage of MFS FIFO, see description of #en_mfs_hl_fifo_usage_t
    mfs_hl_rx_cb_func_ptr_t      pfnRxCb;       ///< Callback function, if RX process is completed
    mfs_hl_tx_cb_func_ptr_t      pfnTxCb;       ///< Callback function, if TX process is completed
    mfs_hl_i2c_slv_cb_func_prt_t pfnI2cSlvStCb; ///< Callback function, if slave address is detected. This is used for Slave mode.
} stc_mfs_hl_i2c_config_t;

/**
 ******************************************************************************
 ** \brief MFS_HL LIN configuration.
 **
 ** Contains all parameter for configuratin a MFS channel, if set to LIN mode.
 ** This configuration was rolled-out from #stc_mfs_hl_lin_config_t to save RAM
 ** memory since LIN is a special mode. Only use functions with prefix
 ** 'Mfs_Hl_Lin_'!
 ******************************************************************************/
typedef struct stc_mfs_hl_lin_config
{
    uint32_t                  u32DataRate;        ///< Baud rate (bps)
    boolean_t                 bExtWakeUp;         ///< TRUE: Sets external wake-up function
    boolean_t                 bLinBreakIrqEnable; ///< TRUE: Enable LIN break receptioninterrupt
    uint8_t*                  pu8TxBuf;           ///< Data store for transmit buffer.
    uint8_t*                  pu8RxBuf;           ///< Char store for receive buffer.
    uint16_t                  u16TxBufSize;       ///< Size of transmit fifo buffer.
    uint16_t                  u16RxBufSize;       ///< Size of receive fifo buffer.
    uint8_t                   u8LinMode;          ///< LIN mode, see #en_mfs_lin_mode_t (mfs.h) for details
    uint8_t                   u8StopBits;         ///< Stop bits length, see description of #en_mfs_stopbit_t on mfs.h
    uint8_t                   u8BreakLength;      ///< Break length, see description of #en_mfs_lin_break_length_t on mfs.h. Only applicable in LIN master mode
    uint8_t                   u8DelimiterLength;  ///< Delimiter length, see description of #en_mfs_lin_delimiter_length_t on mfs.h
    uint8_t                   u8FifoUsage;        ///< Usage of MFS FIFO, see description of #en_mfs_hl_fifo_usage_t
    mfs_hl_rx_cb_func_ptr_t   pfnRxCb;            ///< Callback function, if RX Buffer is full
    mfs_hl_tx_cb_func_ptr_t   pfnTxCb;            ///< Callback function, if TX Buffer is empty
    mfs_hl_lin_brk_func_ptr_t pfnLinBrkCb;        ///< Callback function, if LIN break was detected
} stc_mfs_hl_lin_config_t;

/**
 ******************************************************************************
 ** \brief Get the poitner to internal RX or TX buffer.
 **
 ** The buffer information can be gotten with this pointer.
 ******************************************************************************/
typedef enum en_mfs_hl_buffer
{
    MfsHlRxBuffer = 0u,     ///< MFS high level RX buffer
    MfsHlTxBuffer = 1u,     ///< MFS high level TX buffer
    
}en_mfs_hl_buffer_t;

/******************************************************************************/
/* Local type definitions ('typedef')                                         */
/******************************************************************************/
/// MFS_HL module internal data, storing internal information for each enabled MFS instance.
typedef struct stc_mfs_hl_intern_data
{
    mfs_hl_tx_cb_func_ptr_t      pfnTransmitCbFunction;     ///< Transmission complete callback function
    mfs_hl_rx_cb_func_ptr_t      pfnReceiveCbFunction;      ///< Receive callback function
    mfs_hl_i2c_slv_cb_func_prt_t pfnI2cSlvStCbFunction;     ///< Starting I2C slave callback function.
    mfs_hl_lin_brk_func_ptr_t    pfnLinBreakCbFunction;     ///< LIN break detection callback function
    stc_mfs_hl_buffer_t          stcRxBuffer;               ///< Store location for receive Fifo buffer
    stc_mfs_hl_buffer_t          stcTxBuffer;               ///< Store location for transmission Fifo buffer
    uint32_t                     u32I2cProcCnt;             ///< TX or RX processing count for polling (for I2C)
    uint16_t                     u16RxCallbackBufFillLevel; ///< Unread counts of data buffer to call RX Callback function
    uint8_t                      u8FifoUsage;               ///< Usage of MFS FIFO, see description of #en_mfs_hl_fifo_usage_t
    uint8_t                      u8MfsMode;                 ///< Usage of MFS, see description of #en_mfs_hl_mfs_mode_t
    uint8_t                      u8Exec;                    ///< Execution status, see description of #en_mfs_hl_exec_t
    uint8_t                      u8DataWidth;               ///< Data width
} stc_mfs_hl_intern_data_t;

/// MFS_HL instance data type
typedef struct stc_mfs_hl_instance_data
{
    volatile stc_mfsn_t*     pstcInstance;  ///< pointer to registers of an instance
    stc_mfs_hl_intern_data_t stcInternData; ///< module internal data of instance
} stc_mfs_hl_instance_data_t;

/******************************************************************************/
/* Global variable definitions ('extern')                                     */
/******************************************************************************/

/******************************************************************************/
/* Global function prototypes (definition in C source)                        */
/******************************************************************************/
/* for UART */
/* Init/Deinit */
extern en_result_t Mfs_Hl_Uart_Init(volatile stc_mfsn_t*      pstcUart,
                                    stc_mfs_hl_uart_config_t* pstcConfig
                                   );

extern en_result_t Mfs_Hl_Uart_DeInit(volatile stc_mfsn_t* pstcUart);

/* for CSIO */
/* Init/Deinit */
extern en_result_t Mfs_Hl_Csio_Init(volatile stc_mfsn_t*      pstcCsio,
                                    stc_mfs_hl_csio_config_t* pstcConfig
                                   );

extern en_result_t Mfs_Hl_Csio_DeInit(volatile stc_mfsn_t* pstcCsio);

/* Data Transfer */
extern en_result_t Mfs_Hl_Csio_SynchronousTrans(volatile stc_mfsn_t* pstcCsio,
                                                const uint8_t*       pu8TxData,
                                                uint8_t*             pu8RxData,
                                                uint16_t             u16TransferSize,
                                                boolean_t            bCsHolding
                                               );

/* for LIN */
/* Init/Deinit */
extern en_result_t Mfs_Hl_Lin_Init(volatile stc_mfsn_t*     pstcLin,
                                   stc_mfs_hl_lin_config_t* pstcConfig
                                  );

extern en_result_t Mfs_Hl_Lin_DeInit(volatile stc_mfsn_t* pstcLin);

/* Set configuration */
extern en_result_t Mfs_Hl_Lin_SetBreak(volatile stc_mfsn_t* pstcLin);

extern en_result_t Mfs_Hl_Lin_SetNewBaudDivisor(volatile stc_mfsn_t* pstcLin,
                                                uint16_t             u16BaudDivisor
                                               );

extern en_result_t Mfs_Hl_Lin_DisableRxInterrupt(volatile stc_mfsn_t* pstcLin);

/* Data Transfer */
extern en_result_t Mfs_Hl_Lin_TransferRxBuffer(volatile stc_mfsn_t* pstcLin,
                                               uint8_t*             pu8Data,
                                               uint16_t             u16ReadCount
                                              );

/* for I2C */
/* Init/Deinit */
extern en_result_t Mfs_Hl_I2c_Init(volatile stc_mfsn_t*     pstcI2c,
                                   stc_mfs_hl_i2c_config_t* pstcConfig
                                  );

extern en_result_t Mfs_Hl_I2c_DeInit(volatile stc_mfsn_t* pstcI2c);

/* Data Transfer */
extern en_result_t Mfs_Hl_I2c_Write(volatile stc_mfsn_t* pstcI2c,
                                    uint8_t              u8SlaveAddr,
                                    uint8_t*             pu8Data,
                                    uint16_t*            pu16WriteCnt,
                                    boolean_t            bBlocking
                                   );

extern en_result_t Mfs_Hl_I2c_Read(volatile stc_mfsn_t* pstcI2c,
                                   uint8_t              u8SlaveAddr,
                                   uint8_t*             pu8Data,
                                   uint16_t*            pu16ReadCnt,
                                   boolean_t            bBlocking
                                  );

extern en_result_t Mfs_Hl_I2c_WaitTxComplete(volatile stc_mfsn_t* pstcI2c,
                                             uint32_t             u32MaxCnt
                                            );

extern en_result_t Mfs_Hl_I2c_WaitRxComplete(volatile stc_mfsn_t* pstcI2c,
                                             uint32_t             u32MaxCnt
                                            );

/* for Common(UART/CSIO/LIN) */
/* Data Transfer */
extern en_result_t Mfs_Hl_Read(volatile stc_mfsn_t* pstcMfs,
                               uint8_t*             pu8Data,
                               uint16_t*            pu16DataCnt,
                               uint16_t             u16ReadCnt,
                               boolean_t            bBlocking
                              );

extern en_result_t Mfs_Hl_Write(volatile stc_mfsn_t* pstcMfs,
                                uint8_t*             pu8Data,
                                uint16_t             u16WriteCnt,
                                boolean_t            bBlocking,
                                boolean_t            bCsHolding
                               );
/* Get the RX/TX buffer pointer */
extern en_result_t Mfs_Hl_GetBufferPointer(volatile stc_mfsn_t* pstcMfs,
                                           en_mfs_hl_buffer_t   enBufIndex,
                                           stc_mfs_hl_buffer_t** ppstcBuffer);

//@} // MfshlGroup

#ifdef __cplusplus
}
#endif

#endif /* #if (defined(PDL_PERIPHERAL_MFS_ACTIVE)) */

#endif /* __MFS_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
