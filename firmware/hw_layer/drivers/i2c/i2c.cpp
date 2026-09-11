/**
 * @file    i2c.cpp
 *
 * @date Jul 10, 2026
 * @author Andrey Gusakov
 */

#include "pch.h"
#include "i2c.h"

#include "i2c_bb.h"

/* zero index is I2C_NONE */
static i2cBus *i2cbuses[I2C_BUS_TOTAL_COUNT + 1];

#if HAL_USE_I2C

#include "stm32_i2c.h"

bool HardwareI2c::init(brain_pin_e scl, brain_pin_e sda, i2c_speed_e speed) {
	m_driver = getI2cHwDriverOnPins(scl, sda);
	if (m_driver == nullptr) {
		return false;
	}
	m_sda = sda;
	m_scl = scl;

	return initI2cModule(m_driver, scl, sda, speed);
}

void HardwareI2c::deinit() {
	deinitI2cModule(m_driver);

	efiSetPadUnused(m_sda);
	efiSetPadUnused(m_scl);
}

msg_t HardwareI2c::write(uint8_t addr, const uint8_t* data, size_t size) {
	return i2cMasterTransmitTimeout(m_driver, addr, data, size, nullptr, 0, TIME_MS2I(10));
}

msg_t HardwareI2c::read(uint8_t addr, uint8_t* data, size_t size) {
	return i2cMasterReceiveTimeout(m_driver, addr, data, size, TIME_MS2I(10));
}

msg_t HardwareI2c::writeRead(uint8_t addr, const uint8_t* writeData, size_t writeSize, uint8_t* readData, size_t readSize) {
	return i2cMasterTransmitTimeout(m_driver, addr, writeData, writeSize, readData, readSize, TIME_MS2I(10));
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

static HardwareI2c hardwareI2c[I2C_BUS_TOTAL_COUNT];

/* try to start harware i2c on given pins */
static i2cBus *initI2cHwBus(brain_pin_e scl, brain_pin_e sda, i2c_speed_e speed)
{
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

static BitbangI2c bbI2c[I2C_BUS_TOTAL_COUNT];

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
	/* already inited */
	if (i2cbuses[n]) {
		return;
	}

	const i2c_config_s *cfg = getI2cCfg(n);
	if (cfg == nullptr) {
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

void lockI2c(i2c_bus_e device) {
#if HAL_USE_I2C
	I2CDriver *drv = getI2cDevice(device);
	if (drv) {
		i2cAcquireBus(drv);
	}
#endif
	/* TODO: lock BB i2c device too */
}

void unlockI2c(i2c_bus_e device) {
#if HAL_USE_I2C
	I2CDriver *drv = getI2cDevice(device);
	if (drv) {
		i2cReleaseBus(drv);
	}
#endif
	/* TODO: unlock BB i2c device too */
}

void stopI2c(i2c_bus_e n) {
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

	for (size_t n = I2C_BUS_1; n < I2C_BUS_TOTAL_COUNT; n++) {
		i2cBus *bus = i2cbuses[n];

		if ((bus) && (bus->m_scl == scl) && (bus->m_sda == sda)) {
			return bus;
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
	i2cBus *bus = getI2cBus(static_cast<i2c_bus_e>(n));
	if (bus == nullptr) {
		efiPrintf("Bus %d is not configured/exist", n);
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
