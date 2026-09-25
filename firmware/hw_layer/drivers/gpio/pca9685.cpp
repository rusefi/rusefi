/**
 * @file pca9685.cpp
 *
 * PCA9685 16-channel 12-bit PWM I2C-bus LED controller
 *
 * @date Jul 02, 2026
 * @author Andrey Gusakov
 */

#include "pch.h"

#include "gpio/pca9685.h"

#if HAL_USE_I2C

#include "gpio/gpio_ext.h"

#define DRIVER_NAME "pca9685"

// PCA9685 Registers
#define PCA9685_MODE1      0x00
#define PCA9685_MODE2      0x01
#define PCA9685_SUBADR1    0x02
#define PCA9685_SUBADR2    0x03
#define PCA9685_SUBADR3    0x04
#define PCA9685_ALLCALLADR 0x05
#define PCA9685_LED0_ON_L  0x06
#define PCA9685_LED0_ON_H  0x07
#define PCA9685_LED0_OFF_L 0x08
#define PCA9685_LED0_OFF_H 0x09
#define PCA9685_ALL_LED_ON_L  0xFA
#define PCA9685_ALL_LED_ON_H  0xFB
#define PCA9685_ALL_LED_OFF_L 0xFC
#define PCA9685_ALL_LED_OFF_H 0xFD
#define PCA9685_PRE_SCALE     0xFE

#define MODE1_RESTART 0x80
#define MODE1_SLEEP   0x10
#define MODE1_AI      0x20

struct Pca9685 : public GpioChip {
	int init() override;
	int writePad(size_t pin, int value) override;
	int setPadPWM(size_t pin, float frequency, float duty) override;
	void debug() override;

	i2cBus		*i2c;

	const pca9685_config* cfg;

private:
	int writeReg(uint8_t reg, uint8_t value);
	int writeRegs(uint8_t reg, const uint8_t *regs, size_t n);
	int readReg(uint8_t reg, uint8_t *value);

	int setPwm(uint8_t pin, uint16_t on, uint16_t off);
};

#if BOARD_PCA9685_COUNT > 0
static Pca9685 chips[BOARD_PCA9685_COUNT];

int Pca9685::init() {
	/* Get pointer to actual bus instance, should be ready now */
	i2c = getI2cBus(cfg->i2c_bus);
	if (i2c == nullptr) {
		return -2;
	}

	// 1. Reset chip: set MODE1 to SLEEP=0
	if (writeReg(PCA9685_MODE1, MODE1_AI) != 0) {
		return -1;
	}

	// Default frequency? PCA9685 default is ~200Hz

	return 0;
}

int Pca9685::writePad(size_t pin, int value) {
	if (pin >= PCA9685_OUTPUTS) {
		return -1;
	}

	if (value) {
		// Full ON: LEDn_ON_H bit 4 set to 1
		return setPwm(pin, 0x1000, 0);
	} else {
		// Full OFF: LEDn_OFF_H bit 4 set to 1
		return setPwm(pin, 0, 0x1000);
	}
}

int Pca9685::setPadPWM(size_t pin, float frequency, float duty) {
	if (pin >= PCA9685_OUTPUTS) {
		return -1;
	}

	// frequency handling would require setting PRE_SCALE which affects all outputs
	// For now, skeleton just implements duty cycle on existing frequency
	uint16_t off = (uint16_t)(duty * 4095.0 / 100.0); // 0..4095
	return setPwm(pin, 0, off);
}

void Pca9685::debug() {
	// TODO: implement
}

int Pca9685::writeReg(uint8_t reg, uint8_t value) {
	uint8_t tx[] = { reg, value };
	msg_t msg = i2c->write(cfg->i2c_addr, tx, sizeof(tx));
	return (msg == MSG_OK) ? 0 : -1;
}

int Pca9685::writeRegs(uint8_t reg, const uint8_t *vals, size_t n)
{
	uint8_t cmd[1 + n] = { reg };
	memcpy(cmd + 1, vals, n);
	msg_t msg = i2c->write(cfg->i2c_addr, cmd, n + 1);
	return (msg == MSG_OK) ? 0 : -1;
}

int Pca9685::readReg(uint8_t reg, uint8_t *value) {
	msg_t msg = i2c->writeRead(cfg->i2c_addr, &reg, 1, value, 1);
	return (msg == MSG_OK) ? 0 : -1;
}

int Pca9685::setPwm(uint8_t pin, uint16_t on, uint16_t off) {
	uint8_t reg = PCA9685_LED0_ON_L + (4 * pin);
	uint8_t tx[] = {
		(uint8_t)(on & 0xFF),
		(uint8_t)((on >> 8) & 0xFF),
		(uint8_t)(off & 0xFF),
		(uint8_t)((off >> 8) & 0xFF)
	};

	return writeRegs(reg, tx, sizeof(tx));
}

int pca9685_add(brain_pin_e base, unsigned int index, const struct pca9685_config *cfg) {
	if (index >= BOARD_PCA9685_COUNT) {
		return -1;
	}

	Pca9685 &chip = chips[index];
	chip.cfg = cfg;

	return gpiochip_register(base, DRIVER_NAME, chip, PCA9685_OUTPUTS);
}

#else

int pca9685_add(brain_pin_e base, unsigned int index, const struct pca9685_config *cfg) {
	(void)base;
	(void)index;
	(void)cfg;
	return -1;
}

#endif // PCA9685_COUNT

#endif /* HAL_USE_I2C */
