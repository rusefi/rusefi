/*
 * @file connector_uart_dma.cpp
 *
 * @date Jun 21, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "connector_uart_dma.h"

#if TS_UART_DMA_MODE || PRIMARY_UART_DMA_MODE

#if TS_UART_DMA_MODE && PRIMARY_UART_DMA_MODE
 #error "Only single-DMA implemented right now"
#endif

EXTERN_CONFIG;

// Async. FIFO buffer takes some RAM...
uart_dma_s tsUartDma;

/* Common function for all DMA-UART IRQ handlers. */
static void tsCopyDataFromDMA(UARTDriver *uartp) {
	chSysLockFromISR();
	// get 0-based DMA buffer position
	int dmaPos = TS_DMA_BUFFER_SIZE - dmaStreamGetTransactionSize(uartp->dmarx);
	// if the position is wrapped (circular DMA-mode enabled)
	if (dmaPos < tsUartDma.readPos)
		dmaPos += TS_DMA_BUFFER_SIZE;
	// we need to update the current readPos
	int newReadPos = tsUartDma.readPos;
	for (int i = newReadPos; i < dmaPos; ) {
		if (iqPutI(&tsUartDma.fifoRxQueue, tsUartDma.dmaBuffer[newReadPos]) != Q_OK) {
			break; // todo: ignore overflow?
		}
		// the read position should always stay inside the buffer range
		newReadPos = (++i) & (TS_DMA_BUFFER_SIZE - 1);
	}
	tsUartDma.readPos = newReadPos;
	chSysUnlockFromISR();
}

/* We use the same handler code for both halves. */
static void tsRxIRQHalfHandler(UARTDriver *uartp, uartflags_t full) {
	UNUSED(uartp);
	UNUSED(full);
	tsCopyDataFromDMA(uartp);
}

/* This handler is called right after the UART receiver has finished its work. */
static void tsRxIRQIdleHandler(UARTDriver *uartp) {
	UNUSED(uartp);
	tsCopyDataFromDMA(uartp);
}

/* Note: This structure is modified from the default ChibiOS layout! */
static UARTConfig tsDmaUartConfig = {
	.txend1_cb		= NULL,
	.txend2_cb		= NULL,
	.rxend_cb		= NULL,
	.rxchar_cb		= NULL,
	.rxerr_cb		= NULL,
	.timeout_cb		= tsRxIRQIdleHandler,
	.speed			= 0,
	.cr1			= 0,
	.cr2			= 0/*USART_CR2_STOP1_BITS*/ | USART_CR2_LINEN,
	.cr3			= 0,
	.rxhalf_cb		= tsRxIRQHalfHandler
};

void startUartDmaConnector(UARTDriver *uartp DECLARE_CONFIG_PARAMETER_SUFFIX) {
	print("Using UART-DMA mode");
	// init FIFO queue
	iqObjectInit(&tsUartDma.fifoRxQueue, tsUartDma.buffer, sizeof(tsUartDma.buffer), NULL, NULL);

	// start DMA driver
	tsDmaUartConfig.speed = CONFIG(tunerStudioSerialSpeed);
	uartStart(uartp, &tsDmaUartConfig);

	// start continuous DMA transfer using our circular buffer
	tsUartDma.readPos = 0;
	uartStartReceive(uartp, sizeof(tsUartDma.dmaBuffer), tsUartDma.dmaBuffer);
}

#endif // TS_UART_DMA_MODE || PRIMARY_UART_DMA_MODE
