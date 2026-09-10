/**
 * @file    i2c.cpp
 *
 * @date Jul 10, 2026
 * @author Andrey Gusakov
 */

#include "pch.h"
#include "i2c.h"

#if HAL_USE_I2C

#include "stm32_i2c.h"

/* zero index is I2C_NONE */
bool isI2cInitialized[I2C_BUS_TOTAL_COUNT + 1] = { true, false, false, false, false };

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

void turnOnI2c(i2c_bus_e device) {
	if (isI2cInitialized[device]) {
		return;
	}

	const i2c_config_s *cfg = getI2cCfg(device);
	if (cfg == nullptr) {
		return;
	}

	if (!isBrainPinValid(cfg->sclPin) || !isBrainPinValid(cfg->sdaPin)) {
		// Just return, maybe it's not configured
		return;
	}

	I2CDriver *driver = getI2cDevice(device);
	if (driver) {
		isI2cInitialized[device] = initI2cModule(driver, cfg->sclPin, cfg->sdaPin, cfg->speed);
	}
}

void lockI2c(i2c_bus_e device) {
	i2cAcquireBus(getI2cDevice(device));
}

void unlockI2c(i2c_bus_e device) {
	i2cReleaseBus(getI2cDevice(device));
}

void stopI2c(i2c_bus_e device) {
	if (!isI2cInitialized[device]) {
		return; // not turned on
	}

	I2CDriver *driver = getI2cDevice(device);
	if (driver) {
		deinitI2cModule(driver);
	}

	isI2cInitialized[device] = false;
	efiSetPadUnused(getSclPin(device));
	efiSetPadUnused(getSdaPin(device));
}

static void i2cInfo() {
	for (size_t i = 0; i < I2C_BUS_TOTAL_COUNT; i++) {
		printI2cConfig("i2cinfo", static_cast<i2c_bus_e>(I2C_BUS_1 + i));
	}
}

static void i2cScan(int bus) {
	I2CDriver *driver = getI2cDevice(static_cast<i2c_bus_e>(bus));
	if ((driver == NULL) || !isI2cInitialized[bus]) {
		efiPrintf("Bus %d is not configured/exist", bus);
	}

	msg_t status;
	int found_count = 0;

	char line[128];
	char *ptr = line;

	efiPrintf("Starting I2C Bus Scan...");
	efiPrintf("     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");

	// Scan standard 7-bit user addresses from 0x00 to 0x7F
	for (i2caddr_t addr = 0x00; addr <= 0x7F; addr++) {
		// Print row headers for a clean matrix look
		if ((addr % 16) == 0) {
			ptr += sprintf(ptr, "%02X: ", addr);
		}

		// Skip the reserved/illegal address pools (0x00-0x07 and 0x78-0x7F)
		if (addr < 0x08 || addr > 0x77) {
			ptr += sprintf(ptr, "   ");
		} else {
		#if 0
			uint8_t dummy_tx = 0;
			// ChibiOS I2C call using 0 bytes for tx and rx (Zero-byte Write)
			// We use a small timeout (e.g., 10ms) so a missing device doesn't hang the thread
			status = i2cMasterTransmitTimeout(driver, addr, &dummy_tx, 0, NULL, 0, TIME_MS2I(10));
		#else
			uint8_t dummy_rx = 0;
			// ChibiOS I2C call using 0 bytes for tx and rx (Zero-byte Write)
			// We use a small timeout (e.g., 10ms) so a missing device doesn't hang the thread
			status = i2cMasterReceiveTimeout(driver, addr, &dummy_rx, 1, TIME_MS2I(10));
		#endif

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
#if EFI_PROD_CODE
	addConsoleAction("i2cinfo", i2cInfo);
	addConsoleActionI("i2cscan", i2cScan);
#endif
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

#endif
