/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    hal_uart_lld.c
 * @brief   KINETIS UART subsystem low level driver source.
 * @author  andreika <prometheus.pcb@gmail.com>
 *
 * @addtogroup UART
 * @{
 */

#include "hal.h"
#include "fsl_dmamux.h"

#if (HAL_USE_UART == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   UART1 driver identifier.
 */
#if (KINETIS_UART_USE_UART1 == TRUE) || defined(__DOXYGEN__)
UARTDriver UARTD1;
#endif
#if (KINETIS_UART_USE_UART2 == TRUE) || defined(__DOXYGEN__)
UARTDriver UARTD2;
#endif
#if (KINETIS_UART_USE_UART3 == TRUE) || defined(__DOXYGEN__)
UARTDriver UARTD3;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Puts the receiver in the UART_RX_IDLE state.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 */
static void uart_enter_rx_idle_loop(UARTDriver *uartp) {
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

void uart_lld_callback(LPUART_Type *base, void *handle, status_t status, void *userData) {
	UARTDriver *uartp = (UARTDriver *)userData;
#ifdef UART_USE_RING_BUFFER
	// FSL LPUART lib can call this callback from anywhere, that's not good! :(
	if (!port_is_isr_context()) {
		// we cannot just call _uart_rx_complete_isr_code() without failing on osalSysLockFromISR(),
		// so we have to trigger an IRQ interrupt to wake-up the thread after osalThreadSuspendTimeoutS()
		// (See uartReceiveTimeout())
		// We use 'LPUART_STAT_RXINV_MASK' just because it's unused flag 
		// and we can pass the status into the IRQ handler.
		// todo: this is a hack! :( is there a better way?
		base->STAT |= LPUART_STAT_RXINV_MASK;
		NVIC_SetPendingIRQ(uartp->pendingRxIrq);
		return;
	}

#endif /* UART_USE_RING_BUFFER */

	// this is a common part for both interrupt-ring mode and DMA mode
	switch (status) {
	case kStatus_LPUART_TxIdle:
		_uart_tx1_isr_code(uartp);
		break;
	case kStatus_LPUART_RxIdle:
		_uart_rx_complete_isr_code(uartp);
		break;
	case kStatus_LPUART_RxHardwareOverrun:
	case kStatus_LPUART_RxRingBufferOverrun:
		_uart_rx_error_isr_code(uartp, UART_OVERRUN_ERROR);
		break;
	case kStatus_LPUART_FramingError:
		_uart_rx_error_isr_code(uartp, UART_FRAMING_ERROR);
		break;
	case kStatus_LPUART_ParityError:
		_uart_rx_error_isr_code(uartp, UART_PARITY_ERROR);
		break;
	case kStatus_LPUART_NoiseError:
		_uart_rx_error_isr_code(uartp, UART_NOISE_ERROR);
		break;
	}
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level UART driver initialization.
 *
 * @notapi
 */
void uart_lld_init(void) {

#if KINETIS_UART_USE_UART1 == TRUE
  /* Driver initialization.*/
  uartObjectInit(&UARTD1);
#endif

#if KINETIS_UART_USE_UART2 == TRUE
  /* Driver initialization.*/
  uartObjectInit(&UARTD2);
#endif

#if KINETIS_UART_USE_UART3 == TRUE
  /* Driver initialization.*/
  uartObjectInit(&UARTD3);
#endif
}

/**
 * @brief   Configures and activates the UART peripheral.
 *
 * @param[in] uartp      pointer to the @p UARTDriver object
 *
 * @notapi
 */
void uart_lld_start(UARTDriver *uartp) {

  if (uartp->state == UART_STOP) {
	lpuart_config_t lpuartConfig;
	LPUART_GetDefaultConfig(&lpuartConfig);
    lpuartConfig.enableTx = true;
    lpuartConfig.enableRx = true;
	lpuartConfig.baudRate_Bps = uartp->config->speed;
	lpuartConfig.parityMode = (uartp->config->cr1 & USART_CR1_PCE) ? 
							((uartp->config->cr1 & USART_CR1_PS) ? kLPUART_ParityOdd : kLPUART_ParityEven) : 
							kLPUART_ParityDisabled;
	lpuartConfig.dataBitsCount = kLPUART_EightDataBits;	// todo: check for USART_CR1_M?
	lpuartConfig.stopBitCount = (uartp->config->cr2 & USART_CR2_STOP2_BITS) ? kLPUART_TwoStopBit : kLPUART_OneStopBit;
#ifdef UART_USE_RING_BUFFER
	lpuartConfig.txFifoWatermark = 0;
	lpuartConfig.rxFifoWatermark = 1;
#endif /* UART_USE_RING_BUFFER */

	dma_request_source_t reqRx, reqTx;
	uint32_t dmaStreamRx, dmaStreamTx;

#if KINETIS_UART_USE_UART1 == TRUE
    if (&UARTD1 == uartp) {
		uartp->lpuart = LPUART0;
		reqRx = kDmaRequestMux0LPUART0Rx;
		reqTx = kDmaRequestMux0LPUART0Tx;
		dmaStreamRx = KINETIS_DMA_STREAM_UART0_RX;
		dmaStreamTx = KINETIS_DMA_STREAM_UART0_TX;
		nvicEnableVector(LPUART0_TX_IRQn, KINETIS_UART0_IRQ_PRIORITY);
		nvicEnableVector(LPUART0_RX_IRQn, KINETIS_UART0_IRQ_PRIORITY);
		uartp->pendingRxIrq = LPUART0_RX_IRQn;
    }
#endif
#if KINETIS_UART_USE_UART2 == TRUE
    if (&UARTD2 == uartp) {
    	uartp->lpuart = LPUART1;
		reqRx = kDmaRequestMux0LPUART1Rx;
		reqTx = kDmaRequestMux0LPUART1Tx;
		dmaStreamRx = KINETIS_DMA_STREAM_UART1_RX;
		dmaStreamTx = KINETIS_DMA_STREAM_UART1_TX;
		nvicEnableVector(LPUART1_TX_IRQn, KINETIS_UART1_IRQ_PRIORITY);
		nvicEnableVector(LPUART1_RX_IRQn, KINETIS_UART1_IRQ_PRIORITY);
		uartp->pendingRxIrq = LPUART1_RX_IRQn;
    }
#endif
#if KINETIS_UART_USE_UART3 == TRUE
    if (&UARTD3 == uartp) {
    	uartp->lpuart = LPUART2;
		reqRx = kDmaRequestMux0LPUART2Rx;
		reqTx = kDmaRequestMux0LPUART2Tx;
		dmaStreamRx = KINETIS_DMA_STREAM_UART2_RX;
		dmaStreamTx = KINETIS_DMA_STREAM_UART2_TX;
		nvicEnableVector(LPUART2_TX_IRQn, KINETIS_UART2_IRQ_PRIORITY);
		nvicEnableVector(LPUART2_RX_IRQn, KINETIS_UART2_IRQ_PRIORITY);
		uartp->pendingRxIrq = LPUART2_RX_IRQn;
    }
#endif
    // Enable UART
	LPUART_Init(uartp->lpuart, &lpuartConfig, KINETIS_UART_FREQUENCY);

	//LPUART_EnableInterrupts(uartp->lpuart, kLPUART_IdleLineInterruptEnable);

	// Enable DMA
	nvicEnableVector(DMA0_IRQn + dmaStreamRx, KINETIS_UART_DMA_IRQ_PRIORITY);
	nvicEnableVector(DMA0_IRQn + dmaStreamTx, KINETIS_UART_DMA_IRQ_PRIORITY);
	
	EDMA_CreateHandle(&uartp->lpuartRxEdmaHandle, DMA0, dmaStreamRx); 
	EDMA_CreateHandle(&uartp->lpuartTxEdmaHandle, DMA0, dmaStreamTx); 

	DMAMUX_SetSource(DMAMUX, dmaStreamRx, reqRx);
	DMAMUX_EnableChannel(DMAMUX, dmaStreamRx);
	
	DMAMUX_SetSource(DMAMUX, dmaStreamTx, reqTx);
	DMAMUX_EnableChannel(DMAMUX, dmaStreamTx);

	LPUART_TransferCreateHandleEDMA(uartp->lpuart, &uartp->dmaHandle, (lpuart_edma_transfer_callback_t)uart_lld_callback, 
				uartp, &uartp->lpuartTxEdmaHandle, &uartp->lpuartRxEdmaHandle);

	// This is needed not only for the Ring-buffer mode, but also for the EDMA mode - for UART error handling!
	LPUART_TransferCreateHandle(uartp->lpuart, &uartp->rxHandle, (lpuart_transfer_callback_t)uart_lld_callback, uartp);

#ifdef UART_USE_RING_BUFFER
	LPUART_TransferStartRingBuffer(uartp->lpuart, &uartp->rxHandle, uartp->rxRingBuffer, KINETIS_UART_RX_RING_BUFFER_SIZE);
#endif
  }

  uartp->rxstate = UART_RX_IDLE;
  uartp->txstate = UART_TX_IDLE;
}

/**
 * @brief   Deactivates the UART peripheral.
 *
 * @param[in] uartp      pointer to the @p UARTDriver object
 *
 * @notapi
 */
void uart_lld_stop(UARTDriver *uartp) {

  if (uartp->state == UART_READY) {
    /* Resets the peripheral.*/

    /* Disables the peripheral.*/
#if KINETIS_UART_USE_UART1 == TRUE
    if (&UARTD1 == uartp) {
		nvicDisableVector(LPUART0_TX_IRQn);
		nvicDisableVector(LPUART0_RX_IRQn);
	    DMAMUX_DisableChannel(DMAMUX, KINETIS_DMA_STREAM_UART0_RX);
	    DMAMUX_DisableChannel(DMAMUX, KINETIS_DMA_STREAM_UART0_TX);
	    LPUART_Deinit(LPUART0);
    }
#endif
#if KINETIS_UART_USE_UART2 == TRUE
    if (&UARTD2 == uartp) {
		nvicDisableVector(LPUART1_TX_IRQn);
		nvicDisableVector(LPUART1_RX_IRQn);
	    DMAMUX_DisableChannel(DMAMUX, KINETIS_DMA_STREAM_UART1_RX);
	    DMAMUX_DisableChannel(DMAMUX, KINETIS_DMA_STREAM_UART1_TX);
	    LPUART_Deinit(LPUART1);
    }
#endif
#if KINETIS_UART_USE_UART3 == TRUE
    if (&UARTD3 == uartp) {
		nvicDisableVector(LPUART2_TX_IRQn);
		nvicDisableVector(LPUART2_RX_IRQn);
	    DMAMUX_DisableChannel(DMAMUX, KINETIS_DMA_STREAM_UART2_RX);
	    DMAMUX_DisableChannel(DMAMUX, KINETIS_DMA_STREAM_UART2_TX);
	    LPUART_Deinit(LPUART2);
    }
#endif
  }
}

/**
 * @brief   Starts a transmission on the UART peripheral.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] n         number of data frames to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
void uart_lld_start_send(UARTDriver *uartp, size_t n, const void *txbuf) {

	lpuart_transfer_t xfer;
    xfer.data = (uint8_t *)txbuf;
    xfer.dataSize = n;
    
    status_t status = LPUART_SendEDMA(uartp->lpuart, &uartp->dmaHandle, &xfer);
    // todo: check status
}

/**
 * @brief   Stops any ongoing transmission.
 * @note    Stopping a transmission also suppresses the transmission callbacks.
 *
 * @param[in] uartp      pointer to the @p UARTDriver object
 *
 * @return              The number of data frames not transmitted by the
 *                      stopped transmit operation.
 *
 * @notapi
 */
size_t uart_lld_stop_send(UARTDriver *uartp) {

  LPUART_TransferAbortSendEDMA(uartp->lpuart, &uartp->dmaHandle);

  return 0;
}

/**
 * @brief   Starts a receive operation on the UART peripheral.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] n         number of data frames to send
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void uart_lld_start_receive(UARTDriver *uartp, size_t n, void *rxbuf) {
	lpuart_transfer_t xfer;
    xfer.data = (uint8_t *)rxbuf;
    xfer.dataSize = n;
   
#ifdef UART_USE_RING_BUFFER
	size_t receivedBytes = 0U;
	status_t status = LPUART_TransferReceiveNonBlocking(uartp->lpuart, &uartp->rxHandle, &xfer, &receivedBytes);
	// todo: check receivedBytes?
#else
    status_t status = LPUART_ReceiveEDMA(uartp->lpuart, &uartp->dmaHandle, &xfer);
	// This is needed for UART error handling
	LPUART_EnableInterrupts(uartp->lpuart, kLPUART_RxDataRegFullInterruptEnable | kLPUART_RxOverrunInterruptEnable);
#endif /* UART_USE_RING_BUFFER */   
    // todo: check status
}

/**
 * @brief   Stops any ongoing receive operation.
 * @note    Stopping a receive operation also suppresses the receive callbacks.
 *
 * @param[in] uartp      pointer to the @p UARTDriver object
 *
 * @return              The number of data frames not received by the
 *                      stopped receive operation.
 *
 * @notapi
 */
size_t uart_lld_stop_receive(UARTDriver *uartp) {

  uint32_t numReceived = 0;
#ifdef UART_USE_RING_BUFFER
	if (LPUART_TransferGetReceiveCount(uartp->lpuart, &uartp->rxHandle, &numReceived) == kStatus_NoTransferInProgress)
    {
        return 0;
    }

	LPUART_TransferAbortReceive(uartp->lpuart, &uartp->rxHandle);
	
	return uartp->rxHandle.rxDataSizeAll - (size_t)numReceived;
#else
	// let's do it before LPUART_TransferGetReceiveCountEDMA()?
	LPUART_EnableRxDMA(uartp->lpuart, false);

	if (LPUART_TransferGetReceiveCountEDMA(uartp->lpuart, &uartp->dmaHandle, &numReceived) == kStatus_NoTransferInProgress) {
    	//numReceived = uartp->dmaHandle.rxDataSizeAll;
		return 0;
	}
  
	LPUART_TransferAbortReceiveEDMA(uartp->lpuart, &uartp->dmaHandle);

	return uartp->dmaHandle.rxDataSizeAll - (size_t)numReceived;
#endif /* UART_USE_RING_BUFFER */
}

#endif /* HAL_USE_UART == TRUE */

void uart_lld_blocking_send(UARTDriver *uartp, size_t n, const void *txbuf) {
	LPUART_WriteBlocking(uartp->lpuart, txbuf, n);
}

/** @} */
