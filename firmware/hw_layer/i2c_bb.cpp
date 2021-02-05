#include "i2c_bb.h"

#include "io_pins.h"
#include "efi_gpio.h"

void BitbangI2c::init(brain_pin_e scl, brain_pin_e sda) {
	efiSetPadMode("i2c", scl, PAL_STM32_OTYPE_OPENDRAIN);
	efiSetPadMode("i2c", sda, PAL_STM32_OTYPE_OPENDRAIN);

	m_sclPort = getHwPort("i2c", scl);
	m_sclPin = getHwPin("i2c", scl);

	m_sdaPort = getHwPort("i2c", sda);
	m_sdaPin = getHwPin("i2c", sda);

	// Both lines idle high
	scl_high();
	sda_high();
}

void BitbangI2c::start() {
	// SDA goes low while SCL is high
	sda_high();
	scl_high();
	sda_low();
	scl_low();
}

void BitbangI2c::stop() {
	// Clock goes high while data is low
	scl_low();
	sda_low();
	scl_high();
	sda_high();
}

void BitbangI2c::sendBit(bool val) {
	// Write the bit (write while SCL is low)
	if (val) {
		sda_high();
	} else {
		sda_low();
	}

	// Strobe the clock
	scl_high();
	scl_low();
}

bool BitbangI2c::readBit() {
	scl_high();

	// Read while the clock is high
	bool val = palReadPad(m_sdaPort, m_sdaPin);

	scl_low();

	return val;
}

bool BitbangI2c::writeByte(uint8_t data) {
	// write out 8 data bits
	for (size_t i = 0; i < 8; i++)
	{
		// Send the MSB
		sendBit((data & 0x80) != 0);

		data = data << 1;
	}
	
	// Force a release of the data line so the slave can ACK
	sda_high();

	// Read the ack bit
	bool ackBit = readBit();

	// ACK = 0 -> acknowledged, so return true
	return !ackBit;
}

uint8_t BitbangI2c::readByte() {
	uint8_t result;


}
