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

/**
 * This method controls the actual hardware pins
 *
 * This method takes ~350 ticks.
 */
void applyPinState(PwmConfig *state, int stateIndex) {
	efiAssertVoid(CUSTOM_ERR_6663, stateIndex < PWM_PHASE_MAX_COUNT, "invalid stateIndex");
	efiAssertVoid(CUSTOM_ERR_6664, state->multiWave.waveCount <= PWM_PHASE_MAX_WAVE_PER_PWM, "invalid waveCount");
	for (int waveIndex = 0; waveIndex < state->multiWave.waveCount; waveIndex++) {
		OutputPin *output = state->outputPins[waveIndex];
		int value = state->multiWave.waves[waveIndex].pinStates[stateIndex];
		output->setValue(value);
	}
}

void initPwmGenerator(void) {
}

