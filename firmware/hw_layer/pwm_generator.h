/**
 * @file pwm_generator.h
 *
 * @date May 28, 2013
 * @author Andrey Belomutskiy, (c) 2012-2013
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

void wePlainInit(char *msg, PwmConfig *state, GPIO_TypeDef * port, int pin,
		float dutyCycle, float freq, io_pin_e ioPin);

void initPwmGenerator(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PWM_GENERATOR_H_ */
