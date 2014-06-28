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

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void applyPinState(PwmConfig *state, int stateIndex);

void startSimplePwm(PwmConfig *state, const char *msg, io_pin_e ioPin,
		float dutyCycle, float frequency);

void startSimplePwmExt(PwmConfig *state, const char *msg, brain_pin_e brainPin, io_pin_e ioPin,
		float frequency, float dutyCycle);

void initPwmGenerator(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PWM_GENERATOR_H_ */
