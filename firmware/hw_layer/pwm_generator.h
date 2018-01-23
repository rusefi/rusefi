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

#include "efiGpio.h"

void initPwmGenerator(void);

/**
 * Start a one-channel software PWM driver.
 *
 * This method should be called after scheduling layer is started by initSignalExecutor()
 */
void startSimplePwm(PwmConfig *state, const char *msg, OutputPin *output,
		float dutyCycle, float frequency, pwm_gen_callback *stateChangeCallback);

/**
 * initialize GPIO pin and start a one-channel software PWM driver.
 *
 * This method should be called after scheduling layer is started by initSignalExecutor()
 */
void startSimplePwmExt(PwmConfig *state, const char *msg, brain_pin_e brainPin, OutputPin *output,
		float frequency, float dutyCycle, pwm_gen_callback *stateChangeCallback);

/**
 * default implementation of pwm_gen_callback which simply toggles the pins
 */
void applyPinState(PwmConfig *state, int stateIndex);

#endif /* PWM_GENERATOR_H_ */
