/*
    ChibiOS - Copyright (C) 2014-2015 Fabio Utzig

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    SPIv1/hal_spi_lld.c
 * @brief   Cypress SPI subsystem low level driver source.
 * @author  andreika <prometheus.pcb@gmail.com>
 *
 * @addtogroup SPI
 * @{
 */

#include <string.h>
#include "hal.h"

//_spi_isr_code

#if HAL_USE_SPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/


/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief SPI0 driver identifier.*/
#if CYPRESS_SPI_USE_SPI0 || defined(__DOXYGEN__)
SPIDriver SPID1;
#endif

/** @brief SPI1 driver identifier.*/
#if CYPRESS_SPI_USE_SPI1 || defined(__DOXYGEN__)
SPIDriver SPID2;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/
static en_cs_pin_sel_t spi_detectPCS(bool isMaster, ioportid_t ssport, uint16_t sspad, int *alt) {
	(void)isMaster;
	*alt = PAL_MODE_ALTERNATIVE_SPI;
	// todo: check if PCS corresponds to SPI number
	if (ssport == GPIOG && sspad == 11)	// P7B
		return CsPinScs0;
	if (ssport == GPIOH && sspad == 11)	// PAB
		return CsPinScs0;
	if (ssport == GPIOH && sspad == 12)	// PAC
		return CsPinScs1;
	// wrong/unrecognized PCS!
	*alt = 0;
	return -1;
}

static int32_t spi_detectBaudRate(SPIDriver *spip) {
	static const int baudRates[] = { 21000000, 10500000, 5250000, 2626000, 1312500, 656250, 328125, 164060 };
	int flags = 0;
	if (spip->config->cr1 & SPI_CR1_BR_0)
		flags |= 1;
	if (spip->config->cr1 & SPI_CR1_BR_1)
		flags |= 2;
	if (spip->config->cr1 & SPI_CR1_BR_2)
		flags |= 4;
	int br = baudRates[flags];
	// SPI1 is faster on STM32 (42 MHz max) so we imitate this behavior
	if (spip == &SPID1)
		br *= 2;
	return br;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

static void spi_lld_master_tx_callback(SPIDriver *spip) {
	(void)spip;
//!!!!!!!!!!!!!
#if 0
    if (spip->csioSendCnt >= spip->csioTxBufSize) {
        /* Disable interrupt */
        Mfs_Csio_DisableIrq(spip->spi, CsioTxIrq);
        return;
    }
  
    if (spip->csioTxBuf != NULL) {
	    Mfs_Csio_SendData(spip->spi, spip->csioTxBuf[spip->csioSendCnt++], TRUE);  
	}
#endif
}

static void spi_lld_master_rx_callback(SPIDriver *spip) {
	(void)spip;
//!!!!!!!!!!!!!
#if 0
    if (spip->csioRxBuf != NULL) {
	    spip->csioRxBuf[spip->csioReceiveCnt++] = Mfs_Csio_ReceiveData(spip->spi);
	}

    if (spip->csioReceiveCnt >= spip->csioRxBufSize) {
        /* Disable interrupt */
        Mfs_Csio_DisableIrq(spip->spi, CsioRxIrq);
        return;
    }
#endif
}

/*===========================================================================*/

#if CYPRESS_SPI_USE_SPI0
static void spi_lld_master_tx1_callback(void) {
	spi_lld_master_tx_callback(&SPID1);
}

static void spi_lld_master_rx1_callback(void) {
	spi_lld_master_rx_callback(&SPID1);
}
#endif /* CYPRESS_SPI_USE_SPI0 */

#if CYPRESS_SPI_USE_SPI1
static void spi_lld_master_tx2_callback(void) {
	spi_lld_master_tx_callback(&SPID2);
}

static void spi_lld_master_rx2_callback(void) {
	spi_lld_master_rx_callback(&SPID2);
}
#endif /* CYPRESS_SPI_USE_SPI1 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level SPI driver initialization.
 *
 * @notapi
 */
void spi_lld_init(void) {
#if CYPRESS_SPI_USE_SPI0
	spiObjectInit(&SPID1);
	SPID1.spi = &(CYPRESS_SPI_SPI0_CHANNEL);
	memset(&SPID1.stcCsioIrqCb, 0, sizeof(SPID1.stcCsioIrqCb));
	SPID1.stcCsioIrqCb.pfnTxIrqCb = spi_lld_master_tx1_callback;
	SPID1.stcCsioIrqCb.pfnRxIrqCb = spi_lld_master_rx1_callback;
#endif
#if CYPRESS_SPI_USE_SPI1
	spiObjectInit(&SPID2);
	SPID2.spi = &(CYPRESS_SPI_SPI1_CHANNEL);
	memset(&SPID2.stcCsioIrqCb, 0, sizeof(SPID2.stcCsioIrqCb));
	SPID2.stcCsioIrqCb.pfnTxIrqCb = spi_lld_master_tx2_callback;
	SPID2.stcCsioIrqCb.pfnRxIrqCb = spi_lld_master_rx2_callback;
#endif
}

/**
 * @brief   Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_start(SPIDriver *spip) {
  /* If in stopped state then enables the SPI and DMA clocks.*/
  if (spip->state == SPI_STOP) {

	spip->isMaster = (spip->config->cr1 & SPI_CR1_MSTR) != 0;
	spip->flags = 0;  // kLPSPI_MasterByteSwap;
	if (spip->config->circular)
		spip->flags |= SPI_CIRCULAR_FLAG;
	int pcsAlt = 0;
	//!!!!!!!!!!!!!!!!!!!!!!!!!!
	spip->stcCsioCsPin = -1; //spi_detectPCS(spip->isMaster, spip->config->ssport, spip->config->sspad, &pcsAlt);

	if (spip->stcCsioCsPin != (en_cs_pin_sel_t)-1) {
		// enable corresponding alt.mode for hardware PCS control
		palSetPadMode(spip->config->ssport, spip->config->sspad, PAL_MODE_ALTERNATE(pcsAlt));
		
		memset(&spip->stcCsioCsConfig, 0, sizeof(spip->stcCsioCsConfig));

    	spip->stcCsioCsConfig.enCsStartPin = spip->stcCsioCsPin;
    	spip->stcCsioCsConfig.enCsEndPin = spip->stcCsioCsPin;
    	spip->stcCsioCsConfig.enClkDiv = CsClkNoDiv;
    	spip->stcCsioCsConfig.bActiveHold = FALSE;
    	spip->stcCsioCsConfig.enLevel = CsLowActive;
		// todo: make it baudRate-dependent?
    	spip->stcCsioCsConfig.u8CsSetupDelayTime = 200u;
    	spip->stcCsioCsConfig.u8CsHoldDelayTime = 200u;
    	spip->stcCsioCsConfig.u16CsDeselectTime = 2000u;
    	
    	switch (spip->stcCsioCsPin) {
    	case CsPinScs0:
	    	spip->stcCsioCsConfig.bScs0En = TRUE;
	    	break;
    	case CsPinScs1:
	    	spip->stcCsioCsConfig.bScs1En = TRUE;
	    	break;
		case CsPinScs2:
	    	spip->stcCsioCsConfig.bScs2En = TRUE;
	    	break;
		case CsPinScs3:
    		spip->stcCsioCsConfig.bScs3En = TRUE;
    		break;
    	}
	} else if (spip->config->ssport != NULL) {
		spip->flags |= SPI_SOFTWARE_CS_FLAG;
		// software PCS control for non-standard pins
		palSetPadMode(spip->config->ssport, spip->config->sspad, /*PAL_MODE_OUTPUT_PUSHPULL*/PAL_MODE_OUTPUT_OPENDRAIN);
	}
	memset(&spip->stcMfsCsioCfg, 0, sizeof(spip->stcMfsCsioCfg));
	spip->stcMfsCsioCfg.enMsMode = (spip->isMaster) ? CsioMaster : CsioSlave;
    spip->stcMfsCsioCfg.enActMode = (spip->config->cr1 & SPI_CR1_CPHA) ? CsioActNormalMode : CsioActSpiMode;
    spip->stcMfsCsioCfg.bInvertClk = (spip->config->cr1 & SPI_CR1_CPOL) ? FALSE : TRUE;
	spip->stcMfsCsioCfg.u32BaudRate = spi_detectBaudRate(spip);
	spip->stcMfsCsioCfg.enDataLength = (spip->config->cr1 & SPI_CR1_DFF) ? CsioSixteenBits : CsioEightBits;
	spip->stcMfsCsioCfg.enBitDirection = (spip->config->cr1 & SPI_CR1_LSBFIRST) ? CsioDataLsbFirst : CsioDataMsbFirst;
	spip->stcMfsCsioCfg.enSyncWaitTime = CsioSyncWaitZero;
	spip->stcMfsCsioCfg.pstcFifoConfig = NULL;
	spip->stcMfsCsioCfg.pstcCsConfig = (spip->stcCsioCsPin != (en_cs_pin_sel_t)-1) ? &spip->stcCsioCsConfig : NULL;
	spip->stcMfsCsioCfg.pstcSerialTimer = NULL;
	spip->stcMfsCsioCfg.pstcIrqEn = NULL;
	spip->stcMfsCsioCfg.pstcIrqCb = &spip->stcCsioIrqCb;
	spip->stcMfsCsioCfg.bTouchNvic = TRUE;

   	en_result_t res = Mfs_Csio_Init(spip->spi, &spip->stcMfsCsioCfg);
   	
    //debugLog("***spi_detectPCS() = %d baud=%d res=%d\r\n", pcsIdx, spip->stcMfsCsioCfg.u32BaudRate, res);	//!!!!!!!!!!!!!!!!!!
    

   	osalDbgAssert(res == Ok, "SPI driver init failed");
  }
}

/**
 * @brief   Deactivates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_stop(SPIDriver *spip) {
  /* If in ready state then disables the SPI clock.*/
  if (spip->state == SPI_READY) {
#if CYPRESS_SPI_USE_SPI0
    if (&SPID1 == spip) {
    	Mfs_Csio_DeInit(spip->spi, TRUE);
    }
#endif

#if CYPRESS_SPI_USE_SPI1
    if (&SPID2 == spip) {
    	Mfs_Csio_DeInit(spip->spi, TRUE);
    }
#endif
  }
}

/**
 * @brief   Asserts the slave select signal and prepares for transfers.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_select(SPIDriver *spip) {
	// software PCS control for non-standard pins
	if (spip->flags & SPI_SOFTWARE_CS_FLAG) {
		palClearPad(spip->config->ssport, spip->config->sspad);
	}
}

/**
 * @brief   Deasserts the slave select signal.
 * @details The previously selected peripheral is unselected.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_unselect(SPIDriver *spip) {
	// software PCS control for non-standard pins
	if (spip->flags & SPI_SOFTWARE_CS_FLAG) {
		palSetPad(spip->config->ssport, spip->config->sspad);
	}
}

/**
 * @brief   Ignores data on the SPI bus.
 * @details This asynchronous function starts the transmission of a series of
 *          idle words on the SPI bus and ignores the received data.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be ignored
 *
 * @notapi
 */
void spi_lld_ignore(SPIDriver *spip, size_t n) {
	(void)spip;
	(void)n;
	// todo: implement
/*
  spip->count = n;
  spip->rxbuf = NULL;
  spip->txbuf = NULL;

  spi_start_xfer(spip, false);
*/
}

/**
 * @brief   Exchanges data on the SPI bus.
 * @details This asynchronous function starts a simultaneous transmit/receive
 *          operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf     the pointer to the transmit buffer
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void spi_lld_exchange(SPIDriver *spip, size_t n,
                      const void *txbuf, void *rxbuf) {
	spip->csioTxBuf = (uint8_t *)txbuf;
	spip->csioTxBufSize = (txbuf != NULL) ? n : 0;
	spip->csioSendCnt = 0;
	
	spip->csioRxBuf = (uint8_t *)rxbuf;
	spip->csioRxBufSize = (rxbuf != NULL) ? n : 0;
	spip->csioReceiveCnt = 0;
	
	if (txbuf != NULL) {
		/* Enable TX function of CSIO */
	    Mfs_Csio_EnableFunc(spip->spi, CsioTx);

		Mfs_Csio_SetCsTransferByteCount(spip->spi, spip->stcCsioCsPin, n);

	    /* Configure interrupt */
		Mfs_Csio_EnableIrq(spip->spi, CsioTxIrq);
	}
    
    if (rxbuf != NULL) {
		/* Enable RX function of CSIO */
	    Mfs_Csio_EnableFunc(spip->spi, CsioRx);
	    /* Configure interrupt */
	    Mfs_Csio_EnableIrq(spip->spi, CsioRxIrq);
	}
}

/**
 * @brief   Sends data over the SPI bus.
 * @details This asynchronous function starts a transmit operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
void spi_lld_send(SPIDriver *spip, size_t n, const void *txbuf) {
	spi_lld_exchange(spip, n, txbuf, NULL);
}

/**
 * @brief   Receives data from the SPI bus.
 * @details This asynchronous function starts a receive operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to receive
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf) {
	spi_lld_exchange(spip, n, NULL, rxbuf);
}

#if (SPI_SUPPORTS_CIRCULAR == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Aborts the ongoing SPI operation, if any.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_abort(SPIDriver *spip) {
	(void)spip;
	// todo: implement
#if 0
  //SPI_DisableDMA(base, kLPSPI_TxDmaEnable | kLPSPI_RxDmaEnable);
  /* Stopping DMAs.*/
  dmaStreamDisable(spip->dmatx);
  dmaStreamDisable(spip->dmarx);
#endif
}
#endif /* SPI_SUPPORTS_CIRCULAR == TRUE */

/**
 * @brief   Exchanges one frame using a polled wait.
 * @details This synchronous function exchanges one frame using a polled
 *          synchronization method. This function is useful when exchanging
 *          small amount of data on high speed channels, usually in this
 *          situation is much more efficient just wait for completion using
 *          polling than suspending the thread waiting for an interrupt.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] frame     the data frame to send over the SPI bus
 * @return              The received data frame from the SPI bus.
 */
uint16_t spi_lld_polled_exchange(SPIDriver *spip, uint16_t frame) {
    int numBytes;
    uint16_t rxFrame;

    Mfs_Csio_EnableFunc(spip->spi, CsioTx);
    Mfs_Csio_EnableFunc(spip->spi, CsioRx);
	/* wait until TX buffer empty */
    while (TRUE != Mfs_Csio_GetStatus(spip->spi, CsioTxEmpty))
    	;

	numBytes = (spip->stcMfsCsioCfg.enDataLength == CsioSixteenBits) ? 2 : 1;
	Mfs_Csio_SetCsTransferByteCount(spip->spi, spip->stcCsioCsPin, numBytes);
        
	/* Master sends data */
    Mfs_Csio_SendData(spip->spi, frame, TRUE);  

	/* Wait until master TX bus idle */
    while (TRUE != Mfs_Csio_GetStatus(spip->spi, CsioTxIdle))
    	;

	/* wait until RX buffer full */
	while(TRUE != Mfs_Csio_GetStatus(spip->spi, CsioRxFull))
		;
        
    /* Master receives data */
    rxFrame = Mfs_Csio_ReceiveData(spip->spi);

    Mfs_Csio_DisableFunc(spip->spi, CsioTx);
    Mfs_Csio_DisableFunc(spip->spi, CsioRx);

	return rxFrame;
}

#endif /* HAL_USE_SPI */

/** @} */
