/**
 * @file    i2c.cpp
 *
 * @date Jul 10, 2026
 * @author Andrey Gusakov
 */

#include "pch.h"
#include "i2c.h"

#include "i2c_bb.h"

// Some legacy devices are configured using SDA/SCL pins in config
// If old config used and no I2C bus is configured on given pins
// we can still start up to I2C_BUS_LEGACY_COUNT busses in BB mode on any pins
#ifndef I2C_BUS_LEGACY_COUNT
#define I2C_BUS_LEGACY_COUNT 1
#endif

/* zero index is I2C_NONE */
static i2cBus *i2cbuses[1 + I2C_BUS_TOTAL_COUNT + I2C_BUS_LEGACY_COUNT];

#if HAL_USE_I2C

#include "stm32_i2c.h"

bool HardwareI2c::init(brain_pin_e scl, brain_pin_e sda, i2c_speed_e speed) {
	I2CDriver *driver = getI2cHwDriverOnPins(scl, sda);
	if (driver == nullptr) {
		return false;
	}

	if (!initI2cModule(driver, scl, sda, speed)) {
		return false;
	}

	m_driver = driver;
	m_sda = sda;
	m_scl = scl;

	return true;
}

void HardwareI2c::deinit() {
	if (m_driver == nullptr) {
		return;
	}

	deinitI2cModule(m_driver);
	m_driver = nullptr;

	efiSetPadUnused(m_sda);
	efiSetPadUnused(m_scl);
	m_sda = Gpio::Unassigned;
	m_scl = Gpio::Unassigned;
}

msg_t HardwareI2c::__write(uint8_t addr, const uint8_t* data, size_t size) {
	if (size > sizeof(txnocache)) {
		// huh?
		return MSG_RESET;
	}
	memcpy(txnocache, data, size);
	return i2cMasterTransmitTimeout(m_driver, addr, txnocache, size, nullptr, 0, TIME_MS2I(10));
}

msg_t HardwareI2c::__read(uint8_t addr, uint8_t* data, size_t size) {
	if (size > sizeof(rxnocache)) {
		// huh?
		return MSG_RESET;
	}
	msg_t msg = i2cMasterReceiveTimeout(m_driver, addr, rxnocache, size, TIME_MS2I(10));
	if (msg != MSG_OK) {
		return msg;
	}
	memcpy(data, rxnocache, size);
	return MSG_OK;
}

msg_t HardwareI2c::__writeRead(uint8_t addr, const uint8_t* writeData, size_t writeSize, uint8_t* readData, size_t readSize) {
	if ((writeSize > sizeof(txnocache)) || (readSize > sizeof(rxnocache))) {
		// huh?
		return MSG_RESET;
	}
	memcpy(txnocache, writeData, writeSize);
	msg_t msg = i2cMasterTransmitTimeout(m_driver, addr, txnocache, writeSize, rxnocache, readSize, TIME_MS2I(10));
	if (msg != MSG_OK) {
		return msg;
	}
	memcpy(readData, rxnocache, readSize);
	return MSG_OK;
}

msg_t HardwareI2c::lock() {
	if (m_driver) {
		i2cAcquireBus(m_driver);
		return MSG_OK;
	}
	return MSG_RESET;
}

msg_t HardwareI2c::unlock() {
	if (m_driver) {
		i2cReleaseBus(m_driver);
		return MSG_OK;
	}
	return MSG_RESET;
}

constexpr I2CDriver * getI2cDevice(i2c_bus_e device) {
	switch(device) {
#if STM32_I2C_USE_I2C1
	case I2C_BUS_1:
		return &I2CD1;
#endif
#if STM32_I2C_USE_I2C2
	case I2C_BUS_2:
		return &I2CD2;
#endif
#if STM32_I2C_USE_I2C3
	case I2C_BUS_3:
		return &I2CD3;
#endif
#if STM32_I2C_USE_I2C4
	case I2C_BUS_4:
		return &I2CD4;
#endif
	default:
		return nullptr;
	}
}

static NO_CACHE HardwareI2c hardwareI2c[I2C_BUS_TOTAL_COUNT];

/* try to start harware i2c on given pins */
static i2cBus *initI2cHwBus(brain_pin_e scl, brain_pin_e sda, i2c_speed_e speed)
{
	I2CDriver *driver = getI2cHwDriverOnPins(scl, sda);
	if (driver == nullptr) {
		return nullptr;
	}

	/* this hardware unit is already started by another bus */
	for (size_t i = 0; i < efi::size(hardwareI2c); i++) {
		if (hardwareI2c[i].getDriver() == driver) {
			return nullptr;
		}
	}

	for (size_t i = 0; i < efi::size(hardwareI2c); i++) {
		HardwareI2c *i2c = &hardwareI2c[i];
		if (!i2c->isInitialized()) {
			if (i2c->init(scl, sda, speed)) {
				return i2c;
			}
		}
	}

	return nullptr;
}

#endif // HAL_USE_I2C

// Any bus can fallback to BB mode
// Few extra buses are available for legacy configs
static BitbangI2c bbI2c[I2C_BUS_TOTAL_COUNT + I2C_BUS_LEGACY_COUNT];

brain_pin_e getSclPin(i2c_bus_e device) {
	const i2c_config_s *cfg = getI2cCfg(device);
	if (cfg != nullptr) {
		return cfg->sclPin;
	}

	return Gpio::Unassigned;
}

brain_pin_e getSdaPin(i2c_bus_e device) {
	const i2c_config_s *cfg = getI2cCfg(device);
	if (cfg != nullptr) {
		return cfg->sdaPin;
	}

	return Gpio::Unassigned;
}

void turnOnI2c(i2c_bus_e n) {
	/* also validates n */
	const i2c_config_s *cfg = getI2cCfg(n);
	if (cfg == nullptr) {
		return;
	}

	/* already inited */
	if (i2cbuses[n]) {
		return;
	}

	if (!isBrainPinValid(cfg->sclPin) || !isBrainPinValid(cfg->sdaPin)) {
		// Just return, maybe it's not configured
		return;
	}

	i2cBus *bus = nullptr;

#if HAL_USE_I2C
	/* try hardware first */
	bus = initI2cHwBus(cfg->sclPin, cfg->sdaPin, cfg->speed);
	if (bus) {
		i2cbuses[n] = bus;
	}
#endif
	if (bus == nullptr) {
		/* fallback to bitbang */
		bus = &bbI2c[n - I2C_BUS_1];
		if (bus->init(cfg->sclPin, cfg->sdaPin, cfg->speed)) {
			i2cbuses[n] = bus;
		}
	}
}

static void stopI2c(i2c_bus_e n) {
	if ((n == I2C_NONE) || (n > I2C_BUS_TOTAL_COUNT)) {
		return;
	}

	if (!i2cbuses[n]) {
		return; // not turned on
	}

	i2cbuses[n]->deinit();
	i2cbuses[n] = nullptr;
}

i2cBus *getI2cBus(i2c_bus_e n) {
	if ((n == I2C_NONE) || (n > I2C_BUS_TOTAL_COUNT)) {
		return nullptr;
	}

	return i2cbuses[n];
}

// Legacy
i2cBus *getI2cBus(brain_pin_e scl, brain_pin_e sda) {
	if (!isBrainPinValid(scl) || !isBrainPinValid(sda)) {
		return nullptr;
	}

	// Check if bus is configured in config
	for (size_t idx = 0; idx < I2C_BUS_TOTAL_COUNT + I2C_BUS_LEGACY_COUNT; idx++) {
		i2cBus *bus = i2cbuses[I2C_BUS_1 + idx];

		if ((bus) && (bus->m_scl == scl) && (bus->m_sda == sda)) {
			return bus;
		}
	}

	// Try starting bitbang on given pins (legacy mode)
	for (size_t idx = I2C_BUS_TOTAL_COUNT; idx < I2C_BUS_TOTAL_COUNT + I2C_BUS_LEGACY_COUNT; idx++) {
		if (i2cbuses[I2C_BUS_1 + idx] == nullptr) {
			i2cBus *bus = &bbI2c[idx];
			if (bus->init(scl, sda, I2C_SPEED_400K)) {
				efiPrintf("I2C%d in bitbang mode started", I2C_BUS_1 + idx);
				i2cbuses[I2C_BUS_1 + idx] = bus;
				return bus;
			}
			// failed to init, do not try more
			return nullptr;
		}

	}

	return nullptr;
}

static void i2cInfo() {
	for (size_t i = 0; i < I2C_BUS_TOTAL_COUNT; i++) {
		printI2cConfig("i2cinfo", static_cast<i2c_bus_e>(I2C_BUS_1 + i));
	}
}

static void i2cScan(int n) {
	i2cBus *bus = nullptr;
	if ((n >= I2C_BUS_1) && (n <= I2C_BUS_TOTAL_COUNT)) {
		bus = getI2cBus(static_cast<i2c_bus_e>(n));
	}
	if (bus == nullptr) {
		efiPrintf("Bus %d is not configured/exist", n);
		return;
	}

	msg_t status;
	int found_count = 0;

	char line[128];
	char *ptr = line;

	efiPrintf("Starting I2C Bus Scan...");
	efiPrintf("     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");

	// Scan standard 7-bit user addresses from 0x00 to 0x7F
	for (uint8_t addr = 0x00; addr <= 0x7F; addr++) {
		// Print row headers for a clean matrix look
		if ((addr % 16) == 0) {
			ptr += sprintf(ptr, "%02X: ", addr);
		}

		// Skip the reserved/illegal address pools (0x00-0x07 and 0x78-0x7F)
		if (addr < 0x08 || addr > 0x77) {
			ptr += sprintf(ptr, "   ");
		} else {
			uint8_t dummy_rx = 0;
			// ChibiOS I2C call using 0 bytes for tx and rx (Zero-byte Write)
			status = bus->read(addr, &dummy_rx, 1);

			if (status == MSG_OK) {
				ptr += sprintf(ptr, "%02X ", addr);
				found_count++;
			} else {
				ptr += sprintf(ptr, "-- ");
			}
		}

		// Extra formatting line breaks for every 16 addresses
		if ((addr % 16) == 15) {
			efiPrintf("%s", line);
			ptr = line;
		}
	}

	efiPrintf("Scan complete. Found %d device(s).", found_count);
}

void initEarlyI2c() {
	addConsoleAction("i2cinfo", i2cInfo);
	addConsoleActionI("i2cscan", i2cScan);
}

void initI2cModules() {
	for (size_t i = 0; i < I2C_BUS_TOTAL_COUNT; i++) {
		if (engineConfiguration->i2c[i].enabled) {
			turnOnI2c(static_cast<i2c_bus_e>(I2C_BUS_1 + i));
		}
	}
}

void stopI2cModules() {
	for (size_t i = 0; i < I2C_BUS_TOTAL_COUNT; i++) {
		if (isConfigurationChanged(i2c[i].enabled)) {
			stopI2c(static_cast<i2c_bus_e>(I2C_BUS_1 + i));
		}
	}
}

void printI2cConfig(const char *msg, i2c_bus_e device) {
	const i2c_config_s *cfg = getI2cCfg(device);
	if (cfg == nullptr) {
		efiPrintf("%s %s not enabled in FW", msg, getI2c_bus_e(device));
		return;
	}
	if (!cfg->enabled) {
		efiPrintf("%s %s disabled", msg, getI2c_bus_e(device));
		return;
	}

	efiPrintf("%s %s SCL=%s", msg, getI2c_bus_e(device), hwPortname(cfg->sclPin));
	efiPrintf("%s %s SDA=%s", msg, getI2c_bus_e(device), hwPortname(cfg->sdaPin));
	efiPrintf("%s %s speed=%s", msg, getI2c_bus_e(device), getI2c_speed_e(cfg->speed));
}
