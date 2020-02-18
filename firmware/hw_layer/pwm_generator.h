/**
 * @file pwm_generator.h
 *
 * @date May 28, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"
#include "pwm_generator_logic.h"

#define DEBUG_PWM FALSE

#include "efi_gpio.h"

void initPwmGenerator(void);

/**
 * default implementation of pwm_gen_callback which simply toggles the pins
 *
 */
void applyPinState(int stateIndex, PwmConfig* state) /* pwm_gen_callback */;

