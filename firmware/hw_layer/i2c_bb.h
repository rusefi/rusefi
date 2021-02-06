#pragma once

#if EFI_PROD_CODE

#include "hal.h"
#include "rusefi_hw_enums.h"
#include <cstdint>
#include <cstddef>

class BitbangI2c {
public:
	void init(brain_pin_e scl, brain_pin_e sda);

	// Write a sequence of bytes to the specified device
	void write(uint8_t addr, const uint8_t* data, size_t size);
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

	void start();
	void stop();

	void sendBit(bool val);
	bool readBit();

	void waitQuarterBit();

	ioportid_t m_sclPort = 0;
	ioportmask_t m_sclPin = 0;
	ioportid_t m_sdaPort = 0;
	ioportmask_t m_sdaPin = 0;
};

#endif // EFI_PROD_CODE
