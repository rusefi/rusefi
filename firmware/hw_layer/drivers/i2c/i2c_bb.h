/**
 * @file        i2c_bb.h
 * @brief       Bit-banged I2C driver
 *
 * @date February 6, 2020
 * @author Matthew Kennedy, (c) 2020
 */

#pragma once

#include "i2c.h"

class BitbangI2c : public i2cBus {
public:
	// Initialize the I2C driver
	bool init(brain_pin_e scl, brain_pin_e sda, i2c_speed_e speed = I2C_SPEED_400K) override;
	// Release resources
	void deinit() override;

	// Write a sequence of bytes to the specified device
	msg_t __write(uint8_t addr, const uint8_t* data, size_t size) override;
	// Read a sequence of bytes from the device
	msg_t __read(uint8_t addr, uint8_t* data, size_t size) override;
	// Write some bytes then read some bytes back after a repeated start bit
	msg_t __writeRead(uint8_t addr, const uint8_t* writeData, size_t writeSize, uint8_t* readData, size_t readSize) override;

	msg_t lock() override;
	msg_t unlock() override;

private:
	// Returns true if the remote device acknowledged the transmission
	bool writeByte(uint8_t data);
	uint8_t readByte(bool ack);

	void sda_low();
	void sda_high();
	virtual bool sda_get();
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
	//Mutex protecting the bus.
	mutex_t mutex;

	ioportid_t m_sclPort = 0;
	ioportmask_t m_sclPin = 0;
	ioportid_t m_sdaPort = 0;
	ioportmask_t m_sdaPin = 0;
#endif
};
