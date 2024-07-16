#pragma once

#include "pch.h"
#include "hellen_meta.h"
#include "i2c_bb.h"

#define PCA9555_I2C_ADDR	0x20

static uint32_t getBoardRevision() {
	static bool isFirstInvocation = true;
	static uint32_t variant;

	if (isFirstInvocation) {
		BitbangI2c m_i2c;
		const uint8_t set_out[3] = {0x06, 0xff, 0xff};
		const uint8_t read_inputs_cmd[1] = {0x00};
		uint8_t rx[2] = {0xff, 0xff};

		//same pins as for baso LPS25
		m_i2c.init(Gpio::B10, Gpio::B11);

		// configuration registers:
		// after reset all IO pins should be configured as output, so ttis step can be skipped
		m_i2c.write(PCA9555_I2C_ADDR, set_out, sizeof(set_out));

		// read registers 0 and 1: Input port registers
		m_i2c.writeRead(PCA9555_I2C_ADDR, read_inputs_cmd, sizeof(read_inputs_cmd), rx, sizeof(rx));

		variant = (rx[1] << 8) | (rx[0] << 0);

		efiPrintf("Board variant 0x%04x\n", (unsigned int)variant);

		isFirstInvocation = false;

		// release gpios for baso LPS25 driver
		m_i2c.deinit();
	}

	return variant;
}

int hackHellenBoardId(int /* detectedId */) {
	return getBoardRevision();
}
