/**
 * @file pwm_generator.h
 *
 * @date May 28, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef PWM_GENERATOR_H_
#define PWM_GENERATOR_H_

#include "global.h"
#include "pwm_generator_logic.h"

#define DEBUG_PWM FALSE

#include "gpio_helper.h"

void startSimplePwm(PwmConfig *state, const char *msg, OutputPin *output,
		float dutyCycle, float frequency, pwm_gen_callback *stateChangeCallback);
void applyPinState(PwmConfig *state, int stateIndex);

void initPwmGenerator(void);

void startSimplePwmExt(PwmConfig *state, const char *msg, brain_pin_e brainPin, OutputPin *output,
		float frequency, float dutyCycle, pwm_gen_callback *stateChangeCallback);

#endif /* PWM_GENERATOR_H_ */
