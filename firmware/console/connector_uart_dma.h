/*
 * @file connector_uart_dma.h
 *
 * @date Jun 21, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once
#include "global.h"
#include "tunerstudio_io.h"

#if HAL_USE_UART && EFI_USE_UART_DMA

// See uart_dma_s
#define TS_FIFO_BUFFER_SIZE (BLOCKING_FACTOR + 30)
// This must be a power of 2!
#define TS_DMA_BUFFER_SIZE 32

class UartDmaTsChannel : public UartTsChannel {
public:
	UartDmaTsChannel(UARTDriver& uartDriver);

	void start(uint32_t baud) override;

	// Override only read from UartTsChannel
	size_t readTimeout(uint8_t* buffer, size_t size, int timeout) override;

	void copyDataFromDMA();

private:
	// RX FIFO implementation
	// circular DMA buffer
	uint8_t dmaBuffer[TS_DMA_BUFFER_SIZE];
	// current read position for the DMA buffer
	volatile int readPos;
	// secondary FIFO buffer for async. transfer
	uint8_t buffer[TS_FIFO_BUFFER_SIZE];
	// input FIFO Rx queue
	input_queue_t fifoRxQueue;
};

#endif // HAL_USE_UART && EFI_USE_UART_DMA
