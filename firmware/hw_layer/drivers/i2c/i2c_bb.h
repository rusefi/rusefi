/**
 * @file        i2c_bb.h
 * @brief       Bit-banged I2C driver
 *
 * @date February 6, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#pragma once

#if EFI_PROD_CODE
#include "hal.h"
#endif

#include "rusefi_hw_enums.h"
#include <cstdint>
#include <cstddef>

class BitbangI2c {
public:
	// Initialize the I2C driver
	bool init(brain_pin_e scl, brain_pin_e sda);

	// Write a sequence of bytes to the specified device
	void write(uint8_t addr, const uint8_t* data, size_t size);
	// Read a sequence of bytes from the device
	void read(uint8_t addr, uint8_t* data, size_t size);
	// Write some bytes then read some bytes back after a repeated start bit
	void writeRead(uint8_t addr, const uint8_t* writeData, size_t writeSize, uint8_t* readData, size_t readSize);

	// Read a register at the specified address and register index
	uint8_t readRegister(uint8_t addr, uint8_t reg);
	// Write a register at the specified address and register index
	void writeRegister(uint8_t addr, uint8_t reg, uint8_t val);

private:
	// Returns true if the remote device acknowledged the transmission
	bool writeByte(uint8_t data);
	uint8_t readByte(bool ack);

	void sda_low();
	void sda_high();
	void scl_low();
	void scl_high();

	// Send an I2C start condition
	void start();
	// Send an I2C stop condition
	void stop();

	// Send a single bit
	void sendBit(bool val);
	// Read a single bit
	bool readBit();

	// Wait for 1/4 of a bit time
	void waitQuarterBit();

#if EFI_PROD_CODE
	ioportid_t m_sclPort = 0;
	ioportmask_t m_sclPin = 0;
	ioportid_t m_sdaPort = 0;
	ioportmask_t m_sdaPin = 0;
#endif
};
