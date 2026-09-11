/**
 * @file	stm32_i2c.h
 * @brief	Low level common STM32 I2C code header
 *
 * @date Jul 10, 2026
 * @author Andrey Gusakov
 */

#pragma once

#include "hal.h"

#if HAL_USE_I2C

#include "rusefi_enums.h"

I2CDriver *getI2cHwDriverOnPins(brain_pin_e scl, brain_pin_e sda);
bool initI2cModule(I2CDriver *driver, brain_pin_e scl, brain_pin_e sda, i2c_speed_e speed);
void deinitI2cModule(I2CDriver *driver);

#endif /* HAL_USE_I2C */
