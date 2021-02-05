#pragma once

#include "hal.h"
#include "rusefi_hw_enums.h"
#include <cstdint>
#include <cstddef>

class BitbangI2c {
public:
	void init(brain_pin_e scl, brain_pin_e sda);

	void write(uint8_t addr, uint8_t* data, size_t size);

	// Returns true if the remote device acknowledged the transmission
	bool writeByte(uint8_t data);


private:
	void sda_low();
	void sda_high();
	void scl_low();
	void scl_high();

	void start();
	void stop();

	void sendBit(bool val);
	bool readBit();

	ioportid_t m_sclPort = 0;
	ioportmask_t m_sclPin = 0;
	ioportid_t m_sdaPort = 0;
	ioportmask_t m_sdaPin = 0;
};
