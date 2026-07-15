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

	// Write a sequence of bytes to the specified device
	virtual msg_t write(uint8_t addr, const uint8_t* data, size_t size) = 0;
	// Read a sequence of bytes from the device
	virtual msg_t read(uint8_t addr, uint8_t* data, size_t size) = 0;
	// Write some bytes then read some bytes back after a repeated start bit
	virtual msg_t writeRead(uint8_t addr, const uint8_t* writeData, size_t writeSize, uint8_t* readData, size_t readSize) = 0;

	brain_pin_e m_scl;
	brain_pin_e m_sda;
};

#if HAL_USE_I2C

class HardwareI2c : public i2cBus {
public:
	bool init(brain_pin_e scl, brain_pin_e sda, i2c_speed_e speed) override;
	void deinit() override;

	msg_t write(uint8_t addr, const uint8_t* data, size_t size) override;
	msg_t read(uint8_t addr, uint8_t* data, size_t size) override;
	msg_t writeRead(uint8_t addr, const uint8_t* writeData, size_t writeSize, uint8_t* readData, size_t readSize) override;

	bool isInitialized() {
		return m_driver != nullptr;
	}
private:
	I2CDriver* m_driver;
};

constexpr I2CDriver * getI2cDevice(i2c_bus_e i2cDevice);
void turnOnI2c(i2c_bus_e device);

brain_pin_e getSclPin(i2c_bus_e device);
brain_pin_e getSdaPin(i2c_bus_e device);

void lockI2c(i2c_bus_e device);
void unlockI2c(i2c_bus_e device);

void initEarlyI2c();
void initI2cModules();
void stopI2cModules();

i2cBus *getI2cBus(i2c_bus_e n);
i2cBus *getI2cBus(brain_pin_e scl, brain_pin_e sda);

void printI2cConfig(const char *msg, i2c_bus_e device);

#endif /* HAL_USE_I2C */
