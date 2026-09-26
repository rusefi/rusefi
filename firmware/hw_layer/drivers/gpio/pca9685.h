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

#include "efifeatures.h"

#define PCA9685_OUTPUTS 16

/**
 * @return return gpio chip base
 */
int pca9685_add(brain_pin_e base, unsigned int index, const struct pca9685_config_s *cfg);

int initPca9685Gpios();
