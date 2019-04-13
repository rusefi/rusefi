/**
 * @file pwm_generator.h
 *
 * @date May 28, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef PWM_GENERATOR_H_
#define PWM_GENERATOR_H_

#include "global.h"
#include "pwm_generator_logic.h"

#define DEBUG_PWM FALSE

#include "efi_gpio.h"

void initPwmGenerator(void);

/**
 * default implementation of pwm_gen_callback which simply toggles the pins
 *
 */
void applyPinState(PwmConfig *unused, int stateIndex, PwmConfig* state) /* pwm_gen_callback */;

#endif /* PWM_GENERATOR_H_ */
