/*
 * @file i2c.h
 *
 * @date Jul 10, 2026
 * @author Andrey Gusakov
 */

#pragma once

#include "rusefi_enums.h"

#include <cstdint>
#include <cstddef>

class i2cBus {
public:
	// Initialize the I2C driver
	virtual bool init(brain_pin_e scl, brain_pin_e sda, i2c_speed_e speed) = 0;
	// Release resources
	virtual void deinit() = 0;

	msg_t write(uint8_t addr, const uint8_t* data, size_t size) {
		msg_t ret = lock();
		if (ret == MSG_OK) {
			ret = __write(addr, data, size);
			unlock();
		}
		return ret;
	}

	msg_t read(uint8_t addr, uint8_t* data, size_t size) {
		msg_t ret = lock();
		if (ret == MSG_OK) {
			ret = __read(addr, data, size);
			unlock();
		}
		return ret;
	}

	msg_t writeRead(uint8_t addr, const uint8_t* writeData, size_t writeSize, uint8_t* readData, size_t readSize) {
		msg_t ret = lock();
		if (ret == MSG_OK) {
			ret = __writeRead(addr, writeData, writeSize, readData, readSize);
			unlock();
		}
		return ret;
	}

	// Write a sequence of bytes to the specified device
	virtual msg_t __write(uint8_t addr, const uint8_t* data, size_t size) = 0;
	// Read a sequence of bytes from the device
	virtual msg_t __read(uint8_t addr, uint8_t* data, size_t size) = 0;
	// Write some bytes then read some bytes back after a repeated start bit
	virtual msg_t __writeRead(uint8_t addr, const uint8_t* writeData, size_t writeSize, uint8_t* readData, size_t readSize) = 0;

	virtual msg_t lock() = 0;
	virtual msg_t unlock() = 0;

	brain_pin_e m_scl;
	brain_pin_e m_sda;
};

i2cBus *getI2cBus(i2c_bus_e n);
i2cBus *getI2cBus(brain_pin_e scl, brain_pin_e sda);

#if HAL_USE_I2C

class HardwareI2c : public i2cBus {
public:
	bool init(brain_pin_e scl, brain_pin_e sda, i2c_speed_e speed) override;
	void deinit() override;

	msg_t __write(uint8_t addr, const uint8_t* data, size_t size) override;
	msg_t __read(uint8_t addr, uint8_t* data, size_t size) override;
	msg_t __writeRead(uint8_t addr, const uint8_t* writeData, size_t writeSize, uint8_t* readData, size_t readSize) override;

	msg_t lock() override;
	msg_t unlock() override;

	bool isInitialized() {
		return m_driver != nullptr;
	}
private:
	I2CDriver* m_driver;

	uint8_t txnocache[32];
	uint8_t rxnocache[32];
};

constexpr I2CDriver * getI2cDevice(i2c_bus_e i2cDevice);

#endif /* HAL_USE_I2C */

void turnOnI2c(i2c_bus_e device);

brain_pin_e getSclPin(i2c_bus_e device);
brain_pin_e getSdaPin(i2c_bus_e device);

void initEarlyI2c();
void initI2cModules();
void stopI2cModules();

void printI2cConfig(const char *msg, i2c_bus_e device);
