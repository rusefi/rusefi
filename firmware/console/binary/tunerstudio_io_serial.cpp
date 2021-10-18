/**
 * Implementation for hardware-serial TunerStudio ports
 */

#include "tunerstudio_io.h"

#if HAS_PRIMARY || HAS_PRIMARY
#if HAL_USE_SERIAL
void SerialTsChannel::start(uint32_t baud) {
	SerialConfig cfg = {
		#if EFI_PROD_CODE
			.speed = baud,
			.cr1 = 0,
			.cr2 = USART_CR2_STOP1_BITS | USART_CR2_LINEN,
			.cr3 = 0
		#endif // EFI_PROD_CODE
	};

	sdStart(m_driver, &cfg);
}

void SerialTsChannel::stop() {
	sdStop(m_driver);
}

void SerialTsChannel::write(const uint8_t* buffer, size_t size, bool) {
	chnWriteTimeout(m_driver, buffer, size, BINARY_IO_TIMEOUT);
}

size_t SerialTsChannel::readTimeout(uint8_t* buffer, size_t size, int timeout) {
	return chnReadTimeout(m_driver, buffer, size, timeout);
}

#endif // HAL_USE_SERIAL

#if HAL_USE_UART
void UartTsChannel::start(uint32_t baud) {
	m_config = { 
		.txend1_cb 		= NULL,
		.txend2_cb 		= NULL,
		.rxend_cb 		= NULL,
		.rxchar_cb		= NULL,
		.rxerr_cb		= NULL,
		.timeout_cb		= NULL,
		.speed 			= baud,
		.cr1 			= 0,
		.cr2 			= 0/*USART_CR2_STOP1_BITS*/ | USART_CR2_LINEN,
		.cr3 			= 0,
		.rxhalf_cb		= NULL
	};

	uartStart(m_driver, &m_config);
}

void UartTsChannel::stop() {
	uartStop(m_driver);
}

void UartTsChannel::write(const uint8_t* buffer, size_t size) {
	uartSendTimeout(m_driver, &size, buffer, BINARY_IO_TIMEOUT);
}

size_t UartTsChannel::readTimeout(uint8_t* buffer, size_t size, int timeout) {
	uartReceiveTimeout(m_driver, &size, buffer, timeout);
	return size;
}
#endif // HAL_USE_UART
#endif // HAS_PRIMARY || HAS_PRIMARY
