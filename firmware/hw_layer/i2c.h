/*
 * @file i2c.h
 *
 * @date Jul 10, 2026
 * @author Andrey Gusakov
 */

#pragma once

#if HAL_USE_I2C

#include "rusefi_enums.h"

constexpr I2CDriver * getI2cDevice(i2c_bus_e i2cDevice);
void turnOnI2c(i2c_bus_e device);

i2c_config_s *getI2cCfg(i2c_bus_e device);
brain_pin_e getSclPin(i2c_bus_e device);
brain_pin_e getSdaPin(i2c_bus_e device);

void lockI2c(i2c_bus_e device);
void unlockI2c(i2c_bus_e device);

void initEarlyI2c();
void initI2cModules();
void stopI2cModules();

void printI2cConfig(const char *msg, i2c_bus_e device);

#endif /* HAL_USE_I2C */
