/******************************************************************************
* \file         hsspi.c
*
* \version      1.20
*
* \brief        High Speed Quad Serial Peripheral Interface driver
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

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "hsspi/hsspi.h"

#if (defined(PDL_PERIPHERAL_HSSPI_ACTIVE))

/*****************************************************************************/
/* Local pre-processor symbols/macros ('define')                             */
/*****************************************************************************/
/**
 *****************************************************************************
 ** \brief Transmit and receive maximum FIFO depth.
 *****************************************************************************/
#define HS_SPI_MAX_FIFO_DEPTH         (16u)

/**
*******************************************************************************
** \brief HS-SPI transfer mode (hardware).
**
** Selects hardware transfer mode of the HS-SPI core.
******************************************************************************/
#define HSSPI_DIRECT_MODE_TX_RX_SINGLE_MODE    (0x0u)    ///< Transmit and receive in single mode.
#define HSSPI_DIRECT_MODE_RX_ONLY_SINGLE_MODE  (0x4u)    ///< Receive only, in single mode
#define HSSPI_DIRECT_MODE_RX_ONLY_DUAL_MODE    (0x5u)    ///< Receive only, in dual mode
#define HSSPI_DIRECT_MODE_RX_ONLY_QUAD_MODE    (0x6u)    ///< Receive only, in quad mode
#define HSSPI_DIRECT_MODE_TX_ONLY_SINGLE_MODE  (0x8u)    ///< Transmit only, in single mode. 
#define HSSPI_DIRECT_MODE_TX_ONLY_DUAL_MODE    (0x9u)    ///< Transmit only, in dual mode.
#define HSSPI_DIRECT_MODE_TX_ONLY_QUAD_MODE    (0xAu)    ///< Transmit only, in quad mode.

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

/// Look-up table for all enabled I2S instances and their internal data
stc_hsspi_instance_data_t m_astcHsSpiInstanceDataLut[HS_SPI_INSTANCE_COUNT] =
{
  #if (PDL_PERIPHERAL_ENABLE_HSSPI0 == PDL_ON)
  { 
     &HSSPI0,  	// pstcInstance
     0u,
     {0u, 0u, 0u, 0u, 0u, HsSpiClkLowOutFallingInRising, HsSpiProtocolModeSingle, 0u}   // stcInternData (not initialized yet)
  }
  #endif
};

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/
static en_result_t HsSpi_ModuleEnable (volatile stc_hsspin_t* pstcHsSpi);
static en_result_t HsSpi_ModuleDisable(volatile stc_hsspin_t* pstcHsSpi);

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 ******************************************************************************
 ** \brief Return the internal data for a certain HS-SPI instance.
 **
 ** \param [in] pstcHsSpi       Pointer to HS-SPI instance
 **
 ** \return                     Pointer to internal data or NULL if instance is 
 **                             not enabled (or not known)
 ******************************************************************************/
static stc_hsspi_intern_data_t* HsSpiGetInternDataPtr(volatile stc_hsspin_t* pstcHsSpi) 
{
    uint8_t u8Instance;
   
    for (u8Instance = 0u; u8Instance < HS_SPI_INSTANCE_COUNT; u8Instance++)
    {
        if (pstcHsSpi == m_astcHsSpiInstanceDataLut[u8Instance].pstcInstance)
        {
            return &m_astcHsSpiInstanceDataLut[u8Instance].stcInternData;
        }
    }

    return NULL;
}

/**
 *****************************************************************************
 ** \brief Globally enable SPI module by setting bit MEN
 **
 ** \param [in] pstcHsSpi         Pointer to HS-SPI instance
 **
 ** \retval Ok                    HS-SPI module successfully Enaabled.
 ** \retval Error                 pstcHsSpi == NULL
 ** \retval ErrorTimeout          Operation timed out
 *****************************************************************************/
static en_result_t HsSpi_ModuleEnable(volatile stc_hsspin_t* pstcHsSpi)
{
    uint32_t u32TimeOut;
    u32TimeOut = 100000000u;
    
    if(NULL == pstcHsSpi)
    {
        return Error;
    }
    
    // Set module enable bit in module control register.
    pstcHsSpi->MCTRL_f.MEN = 1u;

    // Wait until module enable status bit is set by hardware
    while ((0u == pstcHsSpi->MCTRL_f.MES) && (u32TimeOut > 0u))
    {
        PDL_WAIT_LOOP_HOOK();
        u32TimeOut--;
    }
    
    if(0ul == u32TimeOut)
    {
      return ErrorTimeout;
    }
    
    return Ok;
} // HsSpiModuleEnable

/**
 *****************************************************************************
 ** \brief Globally disable HS-SPI module by clearing bit MEN
 **
 ** \param [in] pstcHsSpi         Pointer to HS-SPI instance
 **
 ** \retval Ok                    HS-SPI module successfully Disabled.
 ** \retval Error                 Operation timed out, pstcHsSpi == NULL
 *****************************************************************************/
static en_result_t HsSpi_ModuleDisable(volatile stc_hsspin_t* pstcHsSpi)
{
    uint32_t u32TimeOut;
    u32TimeOut = 100000000u;
    
    if(NULL == pstcHsSpi)
    {
        return Error;
    }
    
    // Clear module enable bit in module control register.
    pstcHsSpi->MCTRL_f.MEN = 0u;

    // Wait until module enable status bit is reset by hardware...
    while ((0u != pstcHsSpi->MCTRL_f.MES) && (u32TimeOut > 0u))
    {
        PDL_WAIT_LOOP_HOOK();
        u32TimeOut--;
    }
    
    if(0ul == u32TimeOut)
    {
      return Error;
    }
    
    return Ok;
} // HsSpiModuleDisable

/**
 *****************************************************************************
 ** \brief Initialisation of the HS-SPI module.
 ** FIFO width is limited to 8 bit and DMA is not supported in
 ** direct mode. The (high speed) SPI module is initialized in direct mode after
 ** call of Spi_Init().
 **
 ** \param [in] pstcHsSpi         HS-SPI module instance (register start
 **                               address of HS-SPI module).
 ** \param [in] pstcConfig        HS-SPI configuration parameters.
 **
 ** \retval Ok                    HS-SPI module successfully initialised.
 ** \retval ErrorInvalidParameter If one of the following conditions are met:
 **          - pstcHsSpi == NULL
 **          - pstcConfig == NULL
 **          - pstcConfig->u8TxFifoThresholdLow > (HS_SPI_MAX_FIFO_DEPTH - 1)
 **          - pstcHsSpiInternData == NULL
 **               (invalid or disabled HS-SPI unit (PDL_PERIPHERAL_ENABLE_HSSPIn))
 ** \retval Error                 If HsSpi disable/enable failed
 *****************************************************************************/
en_result_t HsSpi_Init(volatile stc_hsspin_t* pstcHsSpi, const stc_hsspi_config_t* pstcConfig)
{
    stc_hsspi_intern_data_t* pstcHsSpiInternData;                       // Pointer to internal data
    stc_hsspi_cscfg_field_t  stcCSCFG;
    en_result_t enResult;
    
    PDL_ZERO_STRUCT(stcCSCFG);
    
    // Check for NULL-Pointers
    if (NULL == pstcHsSpi ||
        NULL == pstcConfig
       )
    {
        return ErrorInvalidParameter;
    }

    // Check for Threshold in Range
    if (pstcConfig->u8TxFifoThresholdLow > (HS_SPI_MAX_FIFO_DEPTH - 1u))
    {
        return ErrorInvalidParameter;
    }

    // Get ptr to internal data struct ...
    pstcHsSpiInternData = HsSpiGetInternDataPtr(pstcHsSpi);
    // ... and check
    if (NULL == pstcHsSpiInternData)
    {
        return ErrorInvalidParameter;
    }

    // Just to ensure, Everything is switched off
    enResult = HsSpi_ModuleDisable(pstcHsSpi);
    //.... Check if operation timed out
    if(Ok != enResult)
    {
      return Error;
    }

    // Release STOP bit
    pstcHsSpi->DMSTOP = 0x00u;                                   // .STOP = 0;

    // Store callback functions to intern data struct.
    pstcHsSpiInternData->pfnTxStatusCallbackFunction = pstcConfig->pfnTxStatusCallback;

    // Reset direct mode DMA enable register.
    pstcHsSpi->DMDMAEN = 0x00u;                                  // Rx and Tx DMA disabled

    // Save protocol mode
    pstcHsSpiInternData->enDirectModeProtocol = pstcConfig->enDirectModeProtocol;

    // Save operation mode
    pstcHsSpiInternData->bMasterOperation = pstcConfig->bMasterOperation;

    // Set prescaler
    switch(pstcConfig->enClockDivider)
    {
      case HsSpiClkDividerDiv1:
        pstcHsSpi->QDCLKR_f.QHDIV = 0x00u;
        break;
      case HsSpiClkDividerDiv2:
        pstcHsSpi->QDCLKR_f.QHDIV = 0x01u;
        break;
      case HsSpiClkDividerDiv3:
        pstcHsSpi->QDCLKR_f.QHDIV = 0x02u;
        break;
      case HsSpiClkDividerDiv4:
        pstcHsSpi->QDCLKR_f.QHDIV = 0x03u;
        break;
      case HsSpiClkDividerDiv5:
        pstcHsSpi->QDCLKR_f.QHDIV = 0x04u;
        break;
      case HsSpiClkDividerDiv6:
        pstcHsSpi->QDCLKR_f.QHDIV = 0x05u;
        break;
      case HsSpiClkDividerDiv7:
        pstcHsSpi->QDCLKR_f.QHDIV = 0x06u;
        break;
      case HsSpiClkDividerDiv8:
        pstcHsSpi->QDCLKR_f.QHDIV = 0x07u;
        break;
      case HsSpiClkDividerDiv9:
        pstcHsSpi->QDCLKR_f.QHDIV = 0x08u;
        break;
      case HsSpiClkDividerDiv10:
        pstcHsSpi->QDCLKR_f.QHDIV = 0x09u;
        break;
      case HsSpiClkDividerDiv11:
        pstcHsSpi->QDCLKR_f.QHDIV = 0x0Au;
        break;
      case HsSpiClkDividerDiv12:
        pstcHsSpi->QDCLKR_f.QHDIV = 0x0Bu;
        break;
      case HsSpiClkDividerDiv13:
        pstcHsSpi->QDCLKR_f.QHDIV = 0x0Cu;
        break;
      case HsSpiClkDividerDiv14:
        pstcHsSpi->QDCLKR_f.QHDIV = 0x0Du;
        break;
      case HsSpiClkDividerDiv15:
        pstcHsSpi->QDCLKR_f.QHDIV = 0x0Eu;
        break;
      case HsSpiClkDividerDiv16:
        pstcHsSpi->QDCLKR_f.QHDIV = 0x0Fu;
        break;
      default:
        return ErrorInvalidParameter;
    }
    
    // Just a default (will be setup by the transfer function)
    pstcHsSpi->DMTRP = (uint8_t)pstcConfig->enDirectModeProtocol;

    // Set direct mode configuration register.
    pstcHsSpi->DMCFG_f.SSDC     = 1u;	                                // Slave Select Deassert is controlled by DMBCC:BCC

    // Reset direct mode byte count control register.
    pstcHsSpi->DMBCC = 1u;

    // Set FIFO configuration register.
    pstcHsSpi->FIFOCFG_f.FWIDTH = pstcConfig->enFifoWidth;               // FIFO width
    pstcHsSpi->FIFOCFG_f.RXFTH  = 0x7u; // RX FIFO threshold
    pstcHsSpi->FIFOCFG_f.TXFTH  = pstcConfig->u8TxFifoThresholdLow;      // TX FIFO threshold
    
    // Reset command sequencer address extension register.
    pstcHsSpi->CSAEXT = 0x00ul;
      
    // Set command sequencer idle time register.
    pstcHsSpi->CSITIME = (uint32_t)pstcConfig->u16IdleTimeOut;

    // Set command sequencer configuration register.
    stcCSCFG.SSEL3EN = 0u;                                                      // Disable access to slave select 3 memory
    stcCSCFG.SSEL2EN = 0u;                                                      // Disable access to slave select 2 memory
    stcCSCFG.SSEL1EN = 0u;                                                      // Disable access to slave select 1 memory
    stcCSCFG.SSEL0EN = 0u;                                                      // Disable access to slave select 0 memory
    //stcCSCFG.SSEL0EN = 1u;                                                      // Enable access to slave select 0 memory
    stcCSCFG.MSEL    = pstcConfig->enMemoryBankSize;                            // Range of the AHB address space associated with each slave select line
    stcCSCFG.MBM     = pstcConfig->enCommandSequencerModeProtocol;              // Multi bit mode
    stcCSCFG.SRAM    = pstcConfig->enMemoryType;                                // Serial Flash or SRAM device

    pstcHsSpi->CSCFG_f = stcCSCFG;                                       // Save to register
    
    // Set TX/RX DMA bridge
    pstcHsSpi->DBCNT_f.TXDBEN = (TRUE == pstcConfig->bTxDmaBridgeEnable) ? 1u : 0u;
    pstcHsSpi->DBCNT_f.RXDBEN = (TRUE == pstcConfig->bRxDmaBridgeEnable) ? 1u : 0u;

    enResult = HsSpi_ModuleEnable(pstcHsSpi);                                   // Now switch module on
    //.... Check if operation timed out
    if(Ok != enResult)
    {
      return Error;
    }
    
    return Ok;
} // HsSpi_Init

/**
 *****************************************************************************
 ** \brief De-Initializes the HS-SPI unit.
 ** All registers which are used by HS-SPI will be reset to their default value.
 **
 ** \param [in] pstcHsSpi           HS-SPI module instance (register
 **                                 start address of HS-SPI module).
 **
 ** \retval Ok
 ** \retval ErrorInvalidParameter   If one of the following conditions are met:
 **          - pstcHsSpi == NULL
 **          - pstcHsSpiInternData == NULL
 **               (invalid or disabled HS-SPI unit (PDL_PERIPHERAL_ENABLE_HSSPIn))
 ** \retval Error                   If HsSpi disable/enable failed
 *****************************************************************************/
en_result_t HsSpi_DeInit(volatile stc_hsspin_t* pstcHsSpi)
{
    stc_hsspi_intern_data_t* pstcHsSpiInternData; // Pointer to internal data
    en_result_t enResult;
    
    // Check for NULL-Pointer
    if (NULL == pstcHsSpi)
    {
        return ErrorInvalidParameter;
    }

    // Get ptr to internal data struct ...
    pstcHsSpiInternData = HsSpiGetInternDataPtr(pstcHsSpi);
    // ... and check
    if (NULL == pstcHsSpiInternData)
    {
        return ErrorInvalidParameter;
    }

    // Stop any transfer and de-assert slave.
    pstcHsSpi->DMSTOP = 0x01u; // STOP = 1;

    // Just to ensure, anything is switched off
    enResult = HsSpi_ModuleDisable(pstcHsSpi);                                  
    //.... Check if operation timed out
    if(Ok != enResult)
    {
      return Error;
    }

    // Reset callback functions and receive buffer.
    pstcHsSpiInternData->pfnTxStatusCallbackFunction = NULL;
    pstcHsSpiInternData->pu8RXBuffer = NULL;

    // Reset peripheral communication register (PCC0 .. PCC3).
    pstcHsSpi->PCC0_f.SAFESYNC = 1u;
    pstcHsSpi->PCC1_f.SAFESYNC = 1u;
    pstcHsSpi->PCC2_f.SAFESYNC = 1u;
    pstcHsSpi->PCC3_f.SAFESYNC = 1u;

    // Disable and clear transmit interrupt.
    pstcHsSpi->TXE = 0u;
	
    pstcHsSpi->TXC_f.TSSRC  = 1u;
    pstcHsSpi->TXC_f.TFMTC  = 1u;
    pstcHsSpi->TXC_f.TFLETC = 1u;
    pstcHsSpi->TXC_f.TFUC   = 1u;
    pstcHsSpi->TXC_f.TFOC   = 1u;
    pstcHsSpi->TXC_f.TFEC   = 1u;
    pstcHsSpi->TXC_f.TFFC   = 1u;

    // Disable and clear receive interrupt.
    pstcHsSpi->RXE = 0u;
    pstcHsSpi->RXC_f.RSSRC  = 1u;
    pstcHsSpi->RXC_f.RFMTC  = 1u;
    pstcHsSpi->RXC_f.RFLETC = 1u;
    pstcHsSpi->RXC_f.RFUC   = 1u;
    pstcHsSpi->RXC_f.RFOC   = 1u;
    pstcHsSpi->RXC_f.RFEC   = 1u;
    pstcHsSpi->RXC_f.RFFC   = 1u;

    // Clear fault interrupts.
    pstcHsSpi->FAULTC_f.DWCBSFC = 1u;
    pstcHsSpi->FAULTC_f.DRCBSFC = 1u;
    pstcHsSpi->FAULTC_f.PVFC    = 1u;
    pstcHsSpi->FAULTC_f.WAFC    = 1u;
    pstcHsSpi->FAULTC_f.UMAFC   = 1u;

    // Reset direct mode configuration register.
    pstcHsSpi->DMCFG = 0u;

    // Reset direct mode DMA enable register.
    pstcHsSpi->DMDMAEN = 0u;

    // Reset direct mode peripheral select register.
    pstcHsSpi->DMPSEL = 0u;

    // Reset direct mode transfer protocol register.
    pstcHsSpi->DMTRP = 0u;

    // Reset direct mode byte count control register.
    pstcHsSpi->DMBCC = 0u;

    // Reset receive FIFO, transmit FIFO, and flush FIFOs.
    pstcHsSpi->FIFOCFG_f.TXFLSH = 1u;
    pstcHsSpi->FIFOCFG_f.RXFLSH = 1u;
    pstcHsSpi->FIFOCFG_f.FWIDTH = 0u;
    pstcHsSpi->FIFOCFG_f.TXFTH  = 7u;
    pstcHsSpi->FIFOCFG_f.RXFTH  = 7u;

    // Reset command sequencer configuration register.
    pstcHsSpi->CSCFG = 0u;

    // Reset command sequencer idle time register.
    pstcHsSpi->CSITIME = 0x0000FFFFul;

    // Reset command sequencer address extension register.
    pstcHsSpi->CSAEXT_f.AEXT = 0x00u;

    // Reset command sequence data/control register.
    pstcHsSpi->RDCSDC0 = 0u;
    pstcHsSpi->RDCSDC1 = 0u;
    pstcHsSpi->RDCSDC2 = 0u;
    pstcHsSpi->RDCSDC3 = 0u;
    pstcHsSpi->RDCSDC4 = 0u;
    pstcHsSpi->RDCSDC5 = 0u;
    pstcHsSpi->RDCSDC6 = 0u;
    pstcHsSpi->RDCSDC7 = 0u;

    pstcHsSpi->WRCSDC0 = 0u;
    pstcHsSpi->WRCSDC1 = 0u;
    pstcHsSpi->WRCSDC2 = 0u;
    pstcHsSpi->WRCSDC3 = 0u;
    pstcHsSpi->WRCSDC4 = 0u;
    pstcHsSpi->WRCSDC5 = 0u;
    pstcHsSpi->WRCSDC6 = 0u;
    pstcHsSpi->WRCSDC7 = 0u;

    // Reset module control register.
    pstcHsSpi->MCTRL = 0u;

    return Ok;
} // HsSpi_DeInit

/**
 *****************************************************************************
 ** \brief Set mode of the HS-SPI unit.
 **
 ** Selects if HS-SPI unit shall be operated in Direct or Command Sequencer mode
 ** and which protocol mode (legacy, dual, quad) shall be used.
 ** Any transmission has to be completed before a change of HS-SPI mode is
 ** possible, otherwise any ongoing transmission will be interrupted.
 **
 ** \param [in] pstcHsSpi         HS-SPI module instance (register start
 **                               address of HS-SPI module).
 ** \param [in] enMode            Direct mode or command sequencer mode.
 ** \param [in] enProtocolMode    HS-SPI Mode (Legacy, Dual or Quad)
 ** \param [in] enProtocolDirection   HS-SPI protocol direction
 **
 ** \retval Ok                    New mode was successfully set.
 ** \retval ErrorInvalidParameter If following condition is met:
 **          - pstcHsSpi == NULL
 **          - pstcHsSpiInternData == NULL
 **               (invalid or disabled SPI unit (PDL_PERIPHERAL_ENABLE_HSSPIn))
 ** \retval Error                   If HsSpi disable/enable failed
 *****************************************************************************/
en_result_t HsSpi_SetMode(volatile stc_hsspin_t*        pstcHsSpi,
                          en_hsspi_mode_t               enMode,
                          en_hsspi_protocol_mode_t      enProtocolMode,
                          en_hsspi_protocol_direction_t enProtocolDirection)
{
    stc_hsspi_intern_data_t* pstcHsSpiInternData;                               // Pointer to internal data
    en_result_t enResult;
    
    // Check for NULL-Pointer
    if (NULL == pstcHsSpi)
    {
        return ErrorInvalidParameter;
    }

    // Get ptr to internal data struct ...
    pstcHsSpiInternData = HsSpiGetInternDataPtr(pstcHsSpi);
    // ... and check
    if (NULL == pstcHsSpiInternData)
    {
        return ErrorInvalidParameter;
    }

    enResult = HsSpi_ModuleDisable(pstcHsSpi);                                  // Disable before any changes
    //.... Check if operation timed out
    if(Ok != enResult)
    {
      return Error;
    }
    
    // Disable transmit complete and transmit FIFO less or equal to threshold interrupt.
    pstcHsSpi->TXE_f.TFLETE = 0u;
    pstcHsSpi->TXE_f.TFEE   = 0u;

    // Reset byte counter.
    pstcHsSpi->DMBCC = 0u;

    // Flush transmit and receive FIFOs
    pstcHsSpi->FIFOCFG_f.TXFLSH = 1u;
    pstcHsSpi->FIFOCFG_f.RXFLSH = 1u;

    if (enMode == HsSpiModeDirect)
    {
        // Update protocol mode so that transfer functions have access to it
        pstcHsSpiInternData->enDirectModeProtocol = enProtocolMode;
        // Just a default (will be setup by the transfer function)
        
        // Set bit mode
        switch(enProtocolMode)
        {
          case HsSpiProtocolModeSingle:
            pstcHsSpi->DMTRP = 0x00u;
            break;
          case HsSpiProtocolModeDual:
            pstcHsSpi->DMTRP = 0x01u;
            break;
          case HsSpiProtocolModeQuad:
            pstcHsSpi->DMTRP = 0x02u;
            break;
          default:
            return ErrorInvalidParameter;
        }

        // Set direction mode
        switch(enProtocolDirection)
        {
          case HsSpiProtocolTxRx:
            pstcHsSpi->DMTRP |= 0x00u;
            break;
          case HsSpiProtocolTx:
            pstcHsSpi->DMTRP |= 0x04u;
            break;
          case HsSpiProtocolRx:
            pstcHsSpi->DMTRP |= 0x08u;
            break;
          default:
            return ErrorInvalidParameter;
        }
        
        // Setup new stuff
        pstcHsSpi->FIFOCFG_f.RXFTH = 7u;
        pstcHsSpi->RXC_f.RFMTC     = 1u;

        // Set new mode (direct).
        pstcHsSpi->MCTRL_f.CSEN    = 0u;
    }
    else
    {
        // Bit mode
        switch(enProtocolMode)
        {
          case HsSpiProtocolModeSingle:
            pstcHsSpi->CSCFG_f.MBM = 0x0u;
            break;
          case HsSpiProtocolModeDual:
            pstcHsSpi->CSCFG_f.MBM = 0x1u;
            break;
          case HsSpiProtocolModeQuad:
            pstcHsSpi->CSCFG_f.MBM = 0x2u;
            break;
          default:
            return ErrorInvalidParameter;
        }
        
        // Set new mode (command sequencer).
        pstcHsSpi->MCTRL_f.CSEN    = 1u;
    }

    pstcHsSpi->MCTRL_f.SYNCON = 0u;           // AHB synchronizer circuit on
    
    enResult = HsSpi_ModuleEnable(pstcHsSpi);
    //.... Check if operation timed out
    if(Ok != enResult)
    {
      return Error;
    }
    
    return Ok;
} // HsSpi_SetMode

/**
 *****************************************************************************
 ** \brief Configures the communication parameters of an external device.
 **
 ** The external device specific parameters (e.g. HS-SPI mode, LSB/MSB first, ...)
 ** can be configured
 ** In case of high speed SPI the slave u8DeviceNumber is also enabled for the
 ** memory map in command sequencer mode.
 ** (See HW-manual for more information)
 **
 ** \param [in] pstcHsSpi             HS-SPI module instance
 **                                   (register start address of HS-SPI module).
 ** \param [in] u8DeviceNumber        Slave which is connected to corresponding
 **                                   signals SSEL0..3
 ** \param [in] pstcConfig            HS-SPI configuration parameters.
 **
 ** \retval Ok                        External device configuration success  
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **          - pstcHsSpi == NULL
 **          - pstcConfig == NULL
 **          - pstcHsSpiInternData == NULL
 **               (invalid or disabled HS-SPI unit (PDL_PERIPHERAL_ENABLE_HSSPIn))
 **          - u8DeviceNumber > 3
 ** \retval Error                     If HsSpi disable/enable failed
 *****************************************************************************/
en_result_t HsSpi_SetExternalDeviceConfig(volatile stc_hsspin_t* pstcHsSpi, uint8_t u8DeviceNumber, const stc_hsspi_ext_device_config_t* pstcConfig)
{
    stc_hsspi_intern_data_t* pstcHsSpiInternData;                      // Pointer to internal data
    stc_hsspi_pcc0_field_t* pstcHsSpiPcc;                              // Pointer to communication configuration register
    en_result_t enResult;
    
    // Check for NULL-Pointers
    if ((NULL == pstcHsSpi) ||
        (NULL == pstcConfig)
       )
    {
        return ErrorInvalidParameter;
    }

    // Get ptr to internal data struct ...
    pstcHsSpiInternData = HsSpiGetInternDataPtr(pstcHsSpi);
    // ... and check
    if (NULL == pstcHsSpiInternData)
    {
        return ErrorInvalidParameter;
    }

    // Check for slave number in range
    if (u8DeviceNumber > 3u)
    {
        return ErrorInvalidParameter;
    }

    enResult = HsSpi_ModuleDisable(pstcHsSpi);
    //.... Check if operation timed out
    if(Ok != enResult)
    {
      return Error;
    }

    // Update command sequencer configuration register.
    switch (u8DeviceNumber)
    {
        case 0u:
            pstcHsSpiPcc = (stc_hsspi_pcc0_field_t*)&pstcHsSpi->PCC0;
            break;
        case 1u:
            pstcHsSpiPcc = (stc_hsspi_pcc0_field_t*)&pstcHsSpi->PCC1;
            break;
        case 2u:
            pstcHsSpiPcc = (stc_hsspi_pcc0_field_t*)&pstcHsSpi->PCC2;
            break;
        case 3u:
            pstcHsSpiPcc = (stc_hsspi_pcc0_field_t*)&pstcHsSpi->PCC3;
            break;
        default: // Should never see the daylight
            return ErrorInvalidParameter;
    } // switch (u8DeviceNumber)

    // Update command sequencer configuration register.
    switch (u8DeviceNumber)
    {
        case 0u:
            pstcHsSpi->CSCFG_f.SSEL0EN = 1u;
            break;
        case 1u:
            pstcHsSpi->CSCFG_f.SSEL1EN = 1u;
            break;
        case 2u:
            pstcHsSpi->CSCFG_f.SSEL2EN = 1u;
            break;
        case 3u:
            pstcHsSpi->CSCFG_f.SSEL3EN = 1u;
            break;
        default: // Should never see the daylight
            return ErrorInvalidParameter;
    } // switch (u8DeviceNumber)
    
    // Backup for later checks
    pstcHsSpiInternData->enClkMode = pstcConfig->enClockMode;

    // Set peripheral communication register (PCC0 .. PCC3).
    pstcHsSpiPcc->SAFESYNC = (TRUE == pstcConfig->bSafeSync) ? 1u : 0u;
    pstcHsSpiPcc->CDRS     = pstcConfig->enClockDivider;
    pstcHsSpiPcc->SDIR     = (FALSE == pstcConfig->bShiftLsbFirst) ? 0u : 1u;
    pstcHsSpiPcc->SS2CD    = pstcConfig->enClockDelay;
    pstcHsSpiPcc->SSPOL    = (FALSE == pstcConfig->bSlaveSelectPolarityHigh) ? 0u : 1u;
    pstcHsSpiPcc->RTM      = (TRUE == pstcConfig->bCompensatedClock) ? 1u : 0u;
    pstcHsSpiPcc->ACES     = ((pstcConfig->enClockMode >> 2u) & 0x01u);
    pstcHsSpiPcc->CPOL     = ((pstcConfig->enClockMode >> 1u) & 0x01u);
    pstcHsSpiPcc->CPHA     = (pstcConfig->enClockMode & 0x0u);
    
    // Set endianess
    switch(pstcConfig->enEndianess)
    {
      case HsSpiBigEndian:
        pstcHsSpiPcc->SENDIAN = 0u;
        break;
      case HsSpiLittleEndian:
        pstcHsSpiPcc->SENDIAN = 1u;
        break;
      default:
        return ErrorInvalidParameter;
    }

    // Set read deselect time
    if ((pstcConfig->u8ReadDeselectTime < 1u) || (pstcConfig->u8ReadDeselectTime > 4u))
    {
      return ErrorInvalidParameter;
    }
    pstcHsSpiPcc->RDDSEL = pstcConfig->u8ReadDeselectTime  - 1u;
    
    // Set write/different command deselect time
    if ((pstcConfig->u8WriteDeselectTime < 1u) || (pstcConfig->u8WriteDeselectTime > 16u))
    {
      return ErrorInvalidParameter;
    }
    pstcHsSpiPcc->WRDSEL = pstcConfig->u8WriteDeselectTime  - 1u;
    
    enResult = HsSpi_ModuleEnable(pstcHsSpi);
    //.... Check if operation timed out
    if(Ok != enResult)
    {
      return Error;
    }

    return Ok;
} // HsSpi_SetExternalDeviceConfig

/**
 *****************************************************************************
 ** \brief Transfers data in direct half-duplex mode.
 **
 ** Note that direct mode transfer protocol is changed during operation and
 ** previous mode is restored after operation has finished.
 **
 ** \note This function does not work for HS-SPI clock modes 4-7
 **
 ** \note This function is restricted to HS-SPI protocol mode 'Legacy'.
 **       i.e. does not support half duplex transfer in case of 'Dual' or 'Quad' mode.
 **
 ** \param [in] pstcHsSpi             HS-SPI module instance (register
 **                                   start address of (high speed) SPI module).
 ** \param [in] u8DeviceNumber        Slave which is connected to corresponding
 **                                   signal SSEL0..3.
 ** \param [in] pu8TxData             Pointer to transmit buffer
 ** \param [in] u16TxSize             Number of data bytes to transmit.
 **                                   Must be greater than 0.
 ** \param [in] pu8RxData             Pointer to receive buffer, can be NULL.
 ** \param [in] u16RxSize             Number of data bytes to receive, can be 0
 ** \param [in] bEnableTxDmaRequest   Enable or disable DMA request on TX FIFO
 **                                   interrupts (less or equal threshold level).
 **                                   -> currently not supported
 **
 ** \retval Ok                        Setup or transmission of data successful.
 ** \retval ErrorOperationInProgress  A transmission is still ongoing.
 ** \retval ErrorInvalidMode          HS-SPI module is in command sequencer mode.
 **                                   or protocol mode is not "SpiProtocolModeLegacy"
 **                                   or clock mode >= "SpiClkLowOutFallingInFalling"
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **          - pstcHsSpi == NULL
 **          - pu8TxData == NULL
 **          - u8DeviceNumber > 3
 **          - u16TxSize == 0
 **          - u16TxSize + u16RxSize > (2^16 - 1)
 **          - pstcHsSpiInternData == NULL
 **             (invalid or disabled HS-SPI unit (PDL_PERIPHERAL_ENABLE_HSSPIn))
 *****************************************************************************/
en_result_t HsSpi_DirectModeTransferHalfDuplex(volatile stc_hsspin_t* pstcHsSpi, uint8_t u8DeviceNumber, const uint8_t* pu8TxData, uint16_t u16TxSize, uint8_t* pu8RxData, uint16_t u16RxSize, boolean_t bEnableTxDmaRequest)
{
    stc_hsspi_linked_data_list_t stcDataList;
    
    stcDataList.pu8Data             = pu8TxData;
    stcDataList.u16ListItemDataSize = u16TxSize;
    stcDataList.pstcNext            = 0ul;
    
    return HsSpi_DirectModeTransferHalfDuplexList(pstcHsSpi, u8DeviceNumber, &stcDataList, pu8RxData, u16RxSize, bEnableTxDmaRequest);
}

/**
 *****************************************************************************
 ** \brief Transfers linked data in direct half-duplex mode.
 **
 ** Note that direct mode transfer protocol is changed during operation and
 ** previous mode is restored after operation has finished.
 **
 ** \note This function does not work for HS-SPI clock modes 4-7
 **
 ** \note This function is restricted to HS-SPI protocol mode 'Legacy'.
 **       i.e. does not support half duplex transfer in case of 'Dual' or 'Quad' mode.
 **
 ** \param [in] pstcHsSpi             HS-SPI module instance (register
 **                                   start address of (high speed) SPI module).
 ** \param [in] u8DeviceNumber        Slave which is connected to corresponding
 **                                   signal SSEL0..3.
 ** \param [in] pstcTxData            Pointer to first item of linked transmit data list
 **                                   The total size of transmit data must be greater than 0
 ** \param [in] pu8RxData             Pointer to receive buffer, can be NULL.
 ** \param [in] u16RxSize             Number of data bytes to receive, can be 0
 ** \param [in] bEnableTxDmaRequest   Enable or disable DMA request on TX FIFO
 **                                   interrupts (less or equal threshold level).
 **                                   -> currently not supported
 **
 ** \retval Ok                        Setup or transmission of data successful.
 ** \retval ErrorOperationInProgress  A transmission is still ongoing.
 ** \retval ErrorInvalidMode          SPI module is in command sequencer mode.
 **                                   or protocol mode is not "HsSpiProtocolModeLegacy"
 **                                   or clock mode >= "HsSpiClkLowOutFallingInFalling"
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **          - pstcHsSpi == NULL
 **          - pstcTxData == NULL
 **          - pstcTxData->pu8Data == NULL
 **          - u8DeviceNumber > 3
 **          - total size of transmit data == 0
 **          - total size of transmit data + u16RxSize > (2^16 - 1)
 **          - pstcHsSpiInternData == NULL
 **             (invalid or disabled HS-SPI unit (PDL_PERIPHERAL_ENABLE_HSSPIn))
 ** \retval Error                     If HsSpi disable/enable failed
 *****************************************************************************/
extern en_result_t HsSpi_DirectModeTransferHalfDuplexList(volatile stc_hsspin_t* pstcHsSpi, uint8_t u8DeviceNumber, const stc_hsspi_linked_data_list_t* pstcTxData, uint8_t* pu8RxData, uint16_t u16RxSize, boolean_t bEnableTxDmaRequest)
{
    stc_hsspi_intern_data_t* pstcHsSpiInternData;
    const stc_hsspi_linked_data_list_t* pstcListItem;
    uint8_t  u8TrpBackup;
    uint8_t  u8Index;
    uint8_t  u8LoopEndIndex;
    uint16_t u16TransmissionLength;
    uint16_t u16TxSize = 0u;
    uint16_t u16RxDataCount = 0u;
    uint16_t u16TxDataCount = 0u;
    uint16_t u16ListItemDataCount = 0u;
    en_result_t enResult;
    
    // Check for NULL-Pointer
    if ((NULL == pstcHsSpi) ||
        (NULL == pstcTxData) ||
        (NULL == pstcTxData->pu8Data) ||
        (TRUE == bEnableTxDmaRequest)               // Currently not supported
       )
    {
        return ErrorInvalidParameter;
    }

    // Get ptr to internal data struct ...
    pstcHsSpiInternData = HsSpiGetInternDataPtr(pstcHsSpi);
    // ... and check
    if (NULL == pstcHsSpiInternData)
    {
        return ErrorInvalidParameter;
    }

    // Check for slave number in range
    if (u8DeviceNumber > 3u)
    {
        return ErrorInvalidParameter;
    }
    
    // Iterate through linked list to sum up total TX size
    pstcListItem = pstcTxData;
    do
    {
        u16TxSize += pstcListItem->u16ListItemDataSize;
        pstcListItem = pstcListItem->pstcNext;
    } while((uint32_t)pstcListItem != 0ul);  

    // Check for valid transfer size
    if (0u == u16TxSize)
    {
        return ErrorInvalidParameter;
    }
    
    // Check for valid total transmission size
    if (((uint32_t) u16TxSize + (uint32_t) u16RxSize) > 65535u)
    {
        return ErrorInvalidParameter;
    }

    // Check if there is a pending transmission
    if (pstcHsSpi->DMSTATUS_f.TXACTIVE != 0u)
    {
        return ErrorOperationInProgress;
    }

    // Check if we're in direct mode.
    if (pstcHsSpi->MCTRL_f.CSEN != 0u)
    {
        return ErrorInvalidMode;
    }

    // Check for SPI modes which may be used in faked half-duplex mode
    if ((pstcHsSpiInternData->enClkMode) >= HsSpiClkLowOutFallingInFalling)
    {
        // Modes >= SPI Mode 4 are not supported in full duplex mode (see hardware manual) which is
        // used for faking half duplex mode. Legacy mode must be used too, because switching from
        // TX-only to RX-only inside a transmission is not recommended (see hardware manual)
        return ErrorInvalidMode;
    }

    // Store receive buffer pointer
    pstcHsSpiInternData->pu8RXBuffer = pu8RxData;

    // Select current device for transmission.
    pstcHsSpi->DMPSEL = u8DeviceNumber;

    // Backup current protocol mode
    u8TrpBackup = pstcHsSpi->DMTRP;

    enResult = HsSpi_ModuleDisable(pstcHsSpi);
    //.... Check if operation timed out
    if(Ok != enResult)
    {
      return Error;
    }
    
    // We are faking a half duplex mode for the upper layer by ignoring and not storing the unwanted data.
    // Setting to SpiDirectModeTxRxLegacyMode
    pstcHsSpi->DMTRP = HSSPI_DIRECT_MODE_TX_RX_SINGLE_MODE;
    
    enResult = HsSpi_ModuleEnable(pstcHsSpi);
    //.... Check if operation timed out
    if(Ok != enResult)
    {
      return Error;
    }
    
    // Flush transmit and receive FIFOs
    pstcHsSpi->FIFOCFG_f.TXFLSH = 1u;
    pstcHsSpi->FIFOCFG_f.RXFLSH = 1u;

    // In half-duplex mode the number of bytes in this transaction is the sum of TX and RX
    u16TransmissionLength = u16TxSize + u16RxSize;
    pstcHsSpi->DMBCC = u16TransmissionLength;
    
    // Preset current list item with the first list item
    pstcListItem = pstcTxData;

    // Trigger start of transfer.
    pstcHsSpi->DMSTART_f.START = 1u;    
    
    // Loop until whole transmission has finished
    while(u16TxDataCount < u16TransmissionLength)
    {   
        // Put max FIFO size or remaining bytes of transmission (whichever is less) in TX FIFO
        u8LoopEndIndex = MIN(HS_SPI_MAX_FIFO_DEPTH, (u16TransmissionLength - u16TxDataCount));
        for (u8Index = 0u; u8Index < u8LoopEndIndex; u8Index++)
        {  
            // Put actual transmit data in FIFO or just dummy data for second part (=receive part) of half duplex transaction
            *((uint8_t*)&pstcHsSpi->TXFIFO0) = (u16TxDataCount < u16TxSize) ? (uint8_t)pstcListItem->pu8Data[u16ListItemDataCount] : 0u;
            
            u16TxDataCount++;
            // Pointer will be 0 when RX phase has been reached
            if((uint32_t)pstcListItem != 0ul)
            {
                u16ListItemDataCount++;
                // Set next list item (or 0) as new current list item when
                // all data of the current list item has been transmitted
                if(u16ListItemDataCount >= pstcListItem->u16ListItemDataSize)
                {
                    pstcListItem = pstcListItem->pstcNext;
                    u16ListItemDataCount = 0u;                
                }  
            }      
        }
        
        // Clear "transmit FIFO & shift register" empty flag.
        pstcHsSpi->TXC_f.TFEC = 1u;

        // Wait until transfer is finished (no more data in FIFO and shift register empty)
        while ((pstcHsSpi->DMSTATUS_f.TXFLEVEL > 0u) ||
               (pstcHsSpi->TXF_f.TFES == 0u)
              )
        {
            PDL_WAIT_LOOP_HOOK();
        }

        // At the end of transaction (indicated by DMBCS) waiting here is necessary to ensure
        // that the last item has been pushed from the shift register to the FIFO (especially at slow SPI clock)
        if(0u == pstcHsSpi->DMBCS)
        {
            while (1u == pstcHsSpi->DMSTATUS_f.RXACTIVE)
            {
                PDL_WAIT_LOOP_HOOK() ;
            }
        }
        
        // Check if receive data has been requested
        if ((pu8RxData != NULL) && (u16RxSize > 0u))
        {
            // Data from FIFO is stored in user buffer if RX phase has been reached
            while (pstcHsSpi->DMSTATUS_f.RXFLEVEL > 0u)
            {
                if(u16RxDataCount >= u16TxSize)
                {
                  pu8RxData[u16RxDataCount - u16TxSize] = *((uint8_t*)&pstcHsSpi->RXFIFO0);
                }
                else
                {
                    (void)(*((uint8_t*)&pstcHsSpi->RXFIFO0));      // empty FIFO
                }
                u16RxDataCount++;
            }
        }
        else
        {
            // Flush receive FIFO in case no receive data has been requested
            pstcHsSpi->FIFOCFG_f.RXFLSH = 1u;
        }
    }

    enResult = HsSpi_ModuleDisable(pstcHsSpi);
    //.... Check if operation timed out
    if(Ok != enResult)
    {
      return Error;
    }
    
    // Restore protocol mode
    pstcHsSpi->DMTRP = u8TrpBackup;

    enResult = HsSpi_ModuleEnable(pstcHsSpi);
    //.... Check if operation timed out
    if(Ok != enResult)
    {
      return Error;
    }
    
    return Ok;
} // HsSpi_DirectModeTransferHalfDuplexList

/**
 *****************************************************************************
 ** \brief Set read command sequence for command sequencer mode.
 **
 ** Every command sequence is either 8 commands long, or has to be finished
 ** with command #HsSpiCmdEndOfList. To find out if a 8 commands long sequence
 ** is valid or not, the 9th command (which will not be transferred) must be
 ** a #HsSpiCmdEndOfList command. The new sequence will be set after internally
 ** disabling the HS-SPI module. After update, the SPI module is enabled
 ** again.
 **
 ** \param [in] pstcHsSpi         HS-SPI module instance (register start address of high speed SPI module).
 ** \param [in] pstcCmdSequence   Pointer to command sequence list (maximum
 **                               9 entries, last entry must be SpiCmdEndOfList).
 **
 ** \retval Ok                    Read command sequence successfully set.
 ** \retval ErrorInvalidMode      HS-SPI module is in direct mode.
 ** \retval ErrorInvalidParameter If one of following conditions are met:
 **          - pstcCmdSequence == NULL
 **          - SpiCmdEndOfList command missing in pstcCmdSequence
 **          - pstcHsSpi == NULL
 ** \retval Error                     If HsSpi disable/enable failed
 *****************************************************************************/
en_result_t HsSpi_SetReadCommandSequence(volatile stc_hsspin_t* pstcHsSpi, const stc_hsspi_command_sequencer_control_t* pstcCmdSequence)
{
    uint8_t             u8Index;
    uint16_t            u16Data;
    boolean_t           bEndOfListFound;
    volatile uint16_t*  pu16HsSpiRdcsdc;
    en_result_t         enResult;

    // Check for NULL-Pointer
    if ((NULL == pstcHsSpi) ||
        (NULL == pstcCmdSequence)
       )
    {
        return ErrorInvalidParameter;
    }

    // Check if direct mode is configured
    if (0u == pstcHsSpi->MCTRL_f.CSEN)
    {
        return ErrorInvalidMode;
    }

    bEndOfListFound = FALSE; // Preset to FALSE

    // Loop over all possible entries and check for a valid terminator
    for (u8Index = 0u; u8Index < 9u; u8Index++)
    {
        if (HsSpiCmdEndOfList == (pstcCmdSequence + u8Index)->enCommand)
        {
            bEndOfListFound = TRUE;
            break;
        }
    }

    // If no end-of-list terminator found
    if (FALSE == bEndOfListFound)
    {
        return ErrorInvalidParameter;
    }
    
    // Note that there are more limitations regarding the command
    // sequence which are not checked here. Refer to the Hardware Manual!
    
    enResult = HsSpi_ModuleDisable(pstcHsSpi); // Disable module before changes are made
    //.... Check if operation timed out
    if(Ok != enResult)
    {
      return Error;
    }
    
    // Set configuration register.
    pu16HsSpiRdcsdc = &pstcHsSpi->RDCSDC0;

    // Loop over all possible "real" commands
    for(u8Index = 0u; u8Index < 8u; u8Index++)
    {
        switch ((pstcCmdSequence + u8Index)->enCommand)
        {
            case HsSpiCmdTransmitData:
                u16Data = ((pstcCmdSequence + u8Index)->u8Data << 8u);
                break;

            case HsSpiCmdTransmitHighZStateNibble:
                u16Data   = (pstcCmdSequence + u8Index)->u8Data;
                u16Data  &= 0xF0u; // Only higher nibble used
                u16Data  |= 0x05u; // Control High-Z nibble
                u16Data <<= 8u;    // Make it the high byte
                u16Data  |= 1u;    // Set DECode bit
                break;

            case HsSpiCmdTransmitHighZStateByte:
                u16Data   = 0x04u; // Control High-Z Byte
                u16Data <<= 8u;    // Make it the high byte
                u16Data  |= 1u;    // Set DECode bit
                break;

            default:
                u16Data = ((pstcCmdSequence + u8Index)->enCommand << 8u);
                u16Data |= 1u;  // Set DECode bit
        }
        
        // Transfer protocol
        switch ((pstcCmdSequence + u8Index)->enTransferProtocol)
        {
          case HsSpiSerialIf:
            u16Data &= 0xFFF9u; // TRP[1:0] = 2b00
            break;
          case HsSpiDualBitMode:
            u16Data &= 0xFFF9u;
            u16Data |= 0x0002u; // TRP[1:0] = 2b01
            break;
          case HsSpiQuadBitMode:
            u16Data &= 0xFFF9u;
            u16Data |= 0x0004u; // TRP[1:0] = 2b10
            break;
          case HsSpiSingleBitMode:
            u16Data |= 0x0006u; // TRP[1:0] = 2b11
            break;
          default:
            return ErrorInvalidParameter;
        }     
        
        if (TRUE == ((pstcCmdSequence + u8Index)->bCont))
        {
          u16Data |= 0x0008u;
        }
        else
        {
          u16Data &= 0xFFF7u;
        }
        
        // Write data and/or command
        pu16HsSpiRdcsdc[u8Index] = u16Data;

        // Stop, if end of list is reached
        if (HsSpiCmdEndOfList == (pstcCmdSequence + u8Index)->enCommand)
        {
            break;
        }
    } // for(u8Index = 0u; u8Index < 8u; u8Index++)

    enResult = HsSpi_ModuleEnable(pstcHsSpi);
    //.... Check if operation timed out
    if(Ok != enResult)
    {
      return Error;
    }

    return Ok;
} // HsSpi_SetReadCommandSequence

/**
 *****************************************************************************
 ** \brief Set write command sequence for command sequencer mode.
 **
 ** Every command sequence is either 8 commands long, or has to be finished
 ** with command #HsSpiCmdEndOfList. To find out if a 8 commands long sequence
 ** is valid or not, the 9th command (which will not be transferred) must be
 ** a #HsSpiCmdEndOfList command. The new sequence will be set after internally
 ** disabling the SPI module. After update, the SPI module is enabled
 ** again.
 **
 ** \param [in] pstcHsSpi           HS-SPI module instance (register start address of HS-SPI module).
 ** \param [in] pstcCmdSequence     Pointer to command sequence list (maximum
 **                                 9 entries, last entry must be SpiCmdEndOfList).
 **
 ** \retval Ok                      Write command sequence successfully set.
 ** \retval ErrorInvalidMode        SPI module is in direct mode.
 ** \retval ErrorInvalidParameter   If one of following conditions are met:
 **          - pstcHsSpi == NULL
 **          - pstcCmdSequence == NULL
 **          - SpiCmdEndOfList command missing in pstcCmdSequence
 ** \retval Error                   If HsSpi disable/enable failed
 *****************************************************************************/
en_result_t HsSpi_SetWriteCommandSequence(volatile stc_hsspin_t* pstcHsSpi, const stc_hsspi_command_sequencer_control_t* pstcCmdSequence)
{
    uint8_t             u8Index;
    uint16_t            u16Data;
    boolean_t           bEndOfListFound;
    volatile uint16_t*  pu16HsSpiWrcsdc;
    en_result_t         enResult;

    // Check for NULL-Pointer
    if ((NULL == pstcHsSpi) ||
        (NULL == pstcCmdSequence)
       )
    {
        return ErrorInvalidParameter;
    }

    // Check if direct mode is configured
    if (0u == pstcHsSpi->MCTRL_f.CSEN)
    {
        return ErrorInvalidMode;
    }

    bEndOfListFound = FALSE; // Preset to FALSE

    // Loop over all possible entries and check for a valid terminator
    for (u8Index = 0u; u8Index < 9u; u8Index++)
    {
        if (HsSpiCmdEndOfList == (pstcCmdSequence + u8Index)->enCommand)
        {
            bEndOfListFound = TRUE;
            break;
        }
    }
    
    // If no end-of-list terminator found
    if (FALSE == bEndOfListFound)
    {
        return ErrorInvalidParameter;
    }
    
    // Note that there are more limitations regarding the command
    // sequence which are not checked here. Refer to the Hardware Manual!

    enResult = HsSpi_ModuleDisable(pstcHsSpi); // Disable module before changes are made
    //.... Check if operation timed out
    if(Ok != enResult)
    {
      return Error;
    }

    // Set configuration register.
    pu16HsSpiWrcsdc = &pstcHsSpi->WRCSDC0;

    // Loop over all possible "real" commands
    for(u8Index = 0u; u8Index < 8u; u8Index++)
    {
        switch ((pstcCmdSequence + u8Index)->enCommand)
        {
            case HsSpiCmdTransmitData:
                u16Data = ((pstcCmdSequence + u8Index)->u8Data << 8u);
                break;

            case HsSpiCmdTransmitHighZStateNibble:
                u16Data   = (pstcCmdSequence + u8Index)->u8Data;
                u16Data  &= 0xF0u; // Only higher nibble used
                u16Data  |= 0x05u; // Control High_z nibble
                u16Data <<= 8u;    // Make it the high byte
                u16Data  |= 1u;    // Set DECode bit
                break;

            case HsSpiCmdTransmitHighZStateByte:
                u16Data   = 0x04u; // Control High-Z Byte
                u16Data <<= 8u;    // Make it the high byte
                u16Data  |= 1u;    // Set DECode bit
                break;

            default:
                u16Data = ((pstcCmdSequence + u8Index)->enCommand << 8u);
                u16Data |= 1u;  // Set DECode bit
        }

        // Transfer protocol
        switch ((pstcCmdSequence + u8Index)->enTransferProtocol)
        {
          case HsSpiSerialIf:
            u16Data &= 0xFFF9u; // TRP[1:0] = 2b00
            break;
          case HsSpiDualBitMode:
            u16Data &= 0xFFF9u;
            u16Data |= 0x0002u; // TRP[1:0] = 2b01
            break;
          case HsSpiQuadBitMode:
            u16Data &= 0xFFF9u;
            u16Data |= 0x0004u; // TRP[1:0] = 2b10
            break;
          case HsSpiSingleBitMode:
            u16Data |= 0x0006u; // TRP[1:0] = 2b11
            break;
          default:
            return ErrorInvalidParameter;
        }     
        
        if (TRUE == ((pstcCmdSequence + u8Index)->bCont))
        {
          u16Data |= 0x0008u;
        }
        else
        {
          u16Data &= 0xFFF7u;
        }
        
        // Write data and/or command
        pu16HsSpiWrcsdc[u8Index] = u16Data;

        // Stop, if end of list is reached
        if((pstcCmdSequence + u8Index)->enCommand == HsSpiCmdEndOfList)
        {
            break;
        }
    } // for(u8Index = 0u; u8Index < 8u; u8Index++)

    enResult = HsSpi_ModuleEnable(pstcHsSpi);
    //.... Check if operation timed out
    if(Ok != enResult)
    {
      return Error;
    }

    return Ok;
} // HsSpi_SetWriteCommandSequence

/**
 *****************************************************************************
 ** \brief Starts transfer in legacy mode and then switches to quad mode
 **
 ** Some HS-SPI devices may require an access sequence where the first part of
 ** a command is transmitted in legacy mode and afterwards further parameters
 ** or the response from the device are transmitted in quad mode.
 ** This function allows to execute such an SPI transaction.
 **
 ** Note that direct mode transfer protocol is changed during operation and
 ** previous mode is restored after operation has finished.
 **
 ** \param [in] pstcHsSpi             HS-SPI module instance
 **                                   (register start address of high speed SPI module).
 ** \param [in] u8DeviceNumber        Slave which is connected to corresponding
 **                                   SSEL0..3 line
 ** \param [in] pu8LegacyData         Pointer to transmit buffer for legacy mode
 ** \param [in] u8LegacyTxSize        Number of data bytes to transmit in legacy mode
 ** \param [in] pu8TxData             Pointer to transmit buffer for quad mode, can be NULL.
 ** \param [in] u8TxSize              Number of data bytes to transmit in quad mode
 **                                   Valid range from 0 to HS_SPI_MAX_FIFO_DEPTH.
 ** \param [in] pu8RxData             Pointer to receive buffer for quad mode, can be NULL.
 ** \param [in] u8RxSize              Number of data bytes to receive in quad mode.
 **                                   Valid range from 0 to HS_SPI_MAX_FIFO_DEPTH.
 **
 ** \retval Ok                        Setup or transmission of data successful.
 ** \retval ErrorOperationInProgress  u8TxSize > 0 and a transmission is still ongoing.
 ** \retval ErrorInvalidMode          SPI module is in command sequencer mode.
 ** \retval ErrorInvalidParameter     If one of following conditions are met:
 **          - pstcHsSpi == NULL
 **          - pu8TxData == NULL
 **          - u8DeviceNumber > 3
 **          - u8TxSize == 0
 **          - u8RxSize > HS_SPI_MAX_FIFO_DEPTH
 **          - u8RxSize   > 0 && pu8RxData == NULL
 **          - u8LegacyTxSize > 0 && pu8LegacyData == NULL
 **          - pstcHsSpiInternData == NULL
 **               (invalid or disabled HS-SPI unit (PDL_PERIPHERAL_ENABLE_HSSPIn))
 *****************************************************************************/
en_result_t HsSpi_MultiModeTransfer(volatile stc_hsspin_t* pstcHsSpi, uint8_t u8DeviceNumber, const uint8_t* pu8LegacyData, uint8_t u8LegacyTxSize, const uint8_t* pu8TxData, uint8_t u8TxSize, uint8_t* pu8RxData, uint8_t u8RxSize)
{
    stc_hsspi_intern_data_t* pstcHsSpiInternData;
    uint8_t u8Index;
    uint8_t u8TrpBackup;

    // Check for NULL-Pointer
    if ((NULL == pstcHsSpi) ||
        (NULL == pu8TxData)
       )
    {
        return ErrorInvalidParameter;
    }

    // Get ptr to internal data struct ...
    pstcHsSpiInternData = HsSpiGetInternDataPtr(pstcHsSpi);
    // ... and check
    if (NULL == pstcHsSpiInternData)
    {
        return ErrorInvalidParameter;
    }

    // Check for slave number in range
    if (u8DeviceNumber > 3u)
    {
        return ErrorInvalidParameter;
    }

    // Check for legacy data pointer
    if ((u8LegacyTxSize > 0u) &&
        (NULL == pu8LegacyData)
       )
    {
        return ErrorInvalidParameter;
    }
    
    // Check for valid quad transfer size
    if ((0u == u8TxSize) ||
        (u8TxSize > HS_SPI_MAX_FIFO_DEPTH)
       )
    {
        return ErrorInvalidParameter;
    }

    // Check for valid quad receive size
    if (u8RxSize > HS_SPI_MAX_FIFO_DEPTH)
    {
        return ErrorInvalidParameter;
    }

    // Check if there are is a pending transmission
    if (pstcHsSpi->DMSTATUS_f.TXACTIVE != 0u)
    {
        return ErrorOperationInProgress;
    }

    // Backup current protocol mode
    u8TrpBackup = pstcHsSpi->DMTRP;

    // Store receive buffer pointer
    pstcHsSpiInternData->pu8RXBuffer = pu8RxData;

    // Flush transmit and receive FIFOs
    pstcHsSpi->FIFOCFG_f.TXFLSH = 1u;
    pstcHsSpi->FIFOCFG_f.RXFLSH = 1u;

    // Select current device for transmission.
    pstcHsSpi->DMPSEL_f.PSEL = u8DeviceNumber;

    // In half-duplex mode the number of bytes in this transaction is the sum of TX and RX
    pstcHsSpi->DMBCC = (uint16_t)(u8LegacyTxSize + u8TxSize + u8RxSize);

    // Trigger start of transfer.
    pstcHsSpi->DMSTART = 1u;
        
    if (u8LegacyTxSize > 0u)
    {
        // Set Transfer Protocol to direct mode TX only
        pstcHsSpi->DMTRP = HSSPI_DIRECT_MODE_TX_ONLY_SINGLE_MODE;

        // Fill Fifo with data which must be send in legacy mode
        for (u8Index = 0u; u8Index < u8LegacyTxSize; u8Index++)
        {
           *((uint8_t*)(&pstcHsSpi->TXFIFO0)) = (uint8_t)pu8LegacyData[u8Index];
        }
        
        // Clear "transmit FIFO & shift register" empty flag.
        pstcHsSpi->TXC_f.TFEC = 1u;
        
        // Now wait until all data in legacy mode is sent (no more data in FIFO and shift register empty)
        while ((pstcHsSpi->DMSTATUS_f.TXFLEVEL > 0u) ||
               (pstcHsSpi->TXF_f.TFES != 1u)
              )
        {
            PDL_WAIT_LOOP_HOOK();
        }
    }
    
    if(u8RxSize != 0)
    {
      // Use this ONLY in Dual/Quad I/O modes. Comment this line for other modes.
      // Set Transfer Protocol to quad mode TX for Fast Read QUAD I/O OPN
      pstcHsSpi->DMTRP = HSSPI_DIRECT_MODE_TX_ONLY_QUAD_MODE;
    }
    
    // Flush transmit and receive FIFOs
    pstcHsSpi->FIFOCFG_f.TXFLSH = 1u;
    pstcHsSpi->FIFOCFG_f.RXFLSH = 1u;
    
    // Fill FIFO with new data
    for (u8Index = 0u; u8Index < u8TxSize; u8Index++)
    {
      *((uint8_t*)(&pstcHsSpi->TXFIFO0)) = (uint8_t)pu8TxData[u8Index];
    }
    
    // Clear "transmit FIFO & shift register" empty flag.
    pstcHsSpi->TXC_f.TFEC = 1u;

    // Wait until quad transfer is finished (no more data in FIFO and shift register empty)
    while ((pstcHsSpi->DMSTATUS_f.TXFLEVEL > 0u) ||
           (pstcHsSpi->TXF_f.TFES != 1u)
          )
    {
        PDL_WAIT_LOOP_HOOK();
    }

    // If no receive data is requested function can return here
    if ((NULL == pu8RxData) || 
        (0u == u8RxSize)
       )
    {
        // Wait unit all data went out before restoring mode
        while (pstcHsSpi->DMSTATUS_f.TXACTIVE != 0u)
        {
            PDL_WAIT_LOOP_HOOK();
        }

        // Restore previous mode
        pstcHsSpi->DMTRP = u8TrpBackup;
        return Ok;
    }

    // Flush transmit and receive FIFOs
    pstcHsSpi->FIFOCFG_f.TXFLSH = 1u;
    pstcHsSpi->FIFOCFG_f.RXFLSH = 1u;
    
    // Set Transfer Protocol to quad mode RX only
    pstcHsSpi->DMTRP = HSSPI_DIRECT_MODE_RX_ONLY_QUAD_MODE;
    
    // By switching to RX only mode the SPI automatically outputs the
    // required clocks for the remaining byte count (SPIn_DMBCS)

    // Initialise receive buffer index
    u8Index = 0u;
    // Read from the RX FIFO, the FIFO will only contain the number
    // of requested data, hence u8Index does not need to be checked
    while ((1u == pstcHsSpi->DMSTATUS_f.RXACTIVE) ||
           (pstcHsSpi->DMSTATUS_f.RXFLEVEL > 0u)
          )
    {
        if (pstcHsSpi->DMSTATUS_f.RXFLEVEL > 0u)
        {
            pu8RxData[u8Index] = *((uint8_t*)(&pstcHsSpi->RXFIFO0)); 
            u8Index++;
        }
    }

    // Restore previous mode
    pstcHsSpi->DMTRP = u8TrpBackup;

    return Ok;
} // HsSpi_MultiModeTransfer

#endif //#if (defined(PDL_PERIPHERAL_HSSPI_ACTIVE))

/*****************************************************************************/
/* EOF (not truncated)                                                       */
/*****************************************************************************/
