/*
 * @file connector_uart_dma.cpp
 *
 * @date Jun 21, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "connector_uart_dma.h"

#if HAL_USE_UART && EFI_USE_UART_DMA

/* Common function for all DMA-UART IRQ handlers. */
void UartDmaTsChannel::copyDataFromDMA() {
	chSysLockFromISR();
	// get 0-based DMA buffer position
	int dmaPos = TS_DMA_BUFFER_SIZE - dmaStreamGetTransactionSize(m_driver->dmarx);
	// if the position is wrapped (circular DMA-mode enabled)
	if (dmaPos < readPos)
		dmaPos += TS_DMA_BUFFER_SIZE;
	// we need to update the current readPos
	int newReadPos = readPos;
	for (int i = newReadPos; i < dmaPos; ) {
		if (iqPutI(&fifoRxQueue, dmaBuffer[newReadPos]) != Q_OK) {
			break; // todo: ignore overflow?
		}
		// the read position should always stay inside the buffer range
		newReadPos = (++i) & (TS_DMA_BUFFER_SIZE - 1);
	}
	readPos = newReadPos;
	chSysUnlockFromISR();
}

/* We use the same handler code for both halves. */
static void tsRxIRQHalfHandler(UARTDriver *uartp, uartflags_t full) {
	UNUSED(full);
	reinterpret_cast<UartDmaTsChannel*>(uartp->dmaAdapterInstance)->copyDataFromDMA();
}

/* This handler is called right after the UART receiver has finished its work. */
static void tsRxIRQIdleHandler(UARTDriver *uartp) {
	UNUSED(uartp);
	reinterpret_cast<UartDmaTsChannel*>(uartp->dmaAdapterInstance)->copyDataFromDMA();
}

/* Called from the UART error ISR (overrun/framing/noise). */
static void tsRxIRQErrHandler(UARTDriver *uartp, uartflags_t e) {
	UNUSED(e);
	reinterpret_cast<UartDmaTsChannel*>(uartp->dmaAdapterInstance)->onRxError();
}

void UartDmaTsChannel::onRxError() {
	// A UART RX error - typically an overrun while the ECU is busy transmitting a large response
	// and not draining RX - can abort the circular DMA receive. With rxerr_cb previously NULL,
	// nothing re-armed it, so the receive stayed stopped and the tuning link went permanently
	// silent until the next start() (a physical power cycle). Re-arm the receive here so a single
	// glitch cannot wedge the link. This runs on the error path only; normal reception is untouched.
	chSysLockFromISR();
	rxErrorCounter++;
	// uartStopReceiveI is a no-op if the receive is not active; either way rxstate ends up != ACTIVE
	// so uartStartReceiveI's precondition holds. The DMA restarts filling dmaBuffer from index 0.
	uartStopReceiveI(m_driver);
	uartStartReceiveI(m_driver, sizeof(dmaBuffer), dmaBuffer);
	readPos = 0;
	chSysUnlockFromISR();
}

UartDmaTsChannel::UartDmaTsChannel(UARTDriver& driver)
	: UartTsChannel(driver)
{
	// Store a pointer to this instance so we can get it back later in the DMA callback
	driver.dmaAdapterInstance = this;

	iqObjectInit(&fifoRxQueue, buffer, sizeof(buffer), nullptr, nullptr);
}

void UartDmaTsChannel::start(uint32_t baud) {
	m_config = {
		.txend1_cb		= NULL,
		.txend2_cb		= NULL,
		.rxend_cb		= NULL,
		.rxchar_cb		= NULL,
		.rxerr_cb		= tsRxIRQErrHandler,
		.timeout_cb		= tsRxIRQIdleHandler,
		.speed			= baud,
		.cr1			= 0,
		.cr2			= USART_CR2_STOP1_BITS | USART_CR2_LINEN,
		.cr3			= 0,
		.rxhalf_cb		= tsRxIRQHalfHandler
	};

	uartStart(m_driver, &m_config);

	// Start the buffered read process
	readPos = 0;
	uartStartReceive(m_driver, sizeof(dmaBuffer), dmaBuffer);
}

size_t UartDmaTsChannel::readTimeout(uint8_t* p_buffer, size_t size, int timeout) {
	// Instead of reading from the device, read from our custom RX queue
	size_t transferred = iqReadTimeout(&fifoRxQueue, p_buffer, size, timeout);
	bytesIn += transferred;
    return transferred;
}

#endif // HAL_USE_UART && EFI_USE_UART_DMA
