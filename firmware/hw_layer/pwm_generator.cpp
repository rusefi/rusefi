/**
 * @file    pwm_generator.cpp
 * @brief   software PWM generator
 *
 * Software PWM implementation. Considering how low all frequencies are, we can totally afford a couple of float multiplications.
 * By generating PWM programmatically we are saving the timers for better purposes. This implementation also supports generating
 * synchronized waves as needed for example to emulate dual Hall-effect crankshaft position sensors.
 *
 *
 * @date May 28, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 *
 */

#include "pwm_generator.h"

#include "pin_repository.h"
#include "datalogging.h"


void initPwmGenerator(void) {
}

