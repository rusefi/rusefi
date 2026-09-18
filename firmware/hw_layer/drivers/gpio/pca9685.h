/*
 * @file pca9685.h
 *
 * PCA9685 16-channel 12-bit PWM I2C-bus LED controller
 *
 * @date Jul 02, 2026
 * @author Andrey Gusakov
 */

#pragma once

#include "global.h"

#if HAL_USE_I2C || defined(__DOXYGEN__)

#include <hal.h>
#include "efifeatures.h"

#define PCA9685_OUTPUTS 16

struct pca9685_config {
#if HAL_USE_I2C
	I2CDriver *i2c_bus;
	i2caddr_t i2c_addr;
#endif
};

/**
 * @return return gpio chip base
 */
int pca9685_add(brain_pin_e base, unsigned int index, const struct pca9685_config *cfg);

#endif /* HAL_USE_I2C */
