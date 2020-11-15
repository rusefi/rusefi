/*
 * @file connector_uart_dma.h
 *
 * @date Jun 21, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once
#include "global.h"
#include "engine_configuration.h"

// See uart_dma_s
#define TS_FIFO_BUFFER_SIZE (BLOCKING_FACTOR + 30)
// This must be a power of 2!
#define TS_DMA_BUFFER_SIZE 32

// struct needed for async DMA transfer mode (see TS_UART_DMA_MODE)
typedef struct {
	// circular DMA buffer
	uint8_t dmaBuffer[TS_DMA_BUFFER_SIZE];
	// current read position for the DMA buffer
	volatile int readPos;
	// secondary FIFO buffer for async. transfer
	uint8_t buffer[TS_FIFO_BUFFER_SIZE];
#if EFI_PROD_CODE || EFI_SIMULATOR
	// input FIFO Rx queue
	input_queue_t fifoRxQueue;
#endif
} uart_dma_s;

#if TS_UART_DMA_MODE || PRIMARY_UART_DMA_MODE
void startUartDmaConnector(UARTDriver *uartp DECLARE_CONFIG_PARAMETER_SUFFIX);
#endif


