/**
 * @file    pwm_generator.c
 * @brief   software PWM generator
 *
 * Software PWM implementation. Considering how low all frequencies are, we can totally afford a couple of float multiplications.
 * By generating PWM programmatically we are saving the timers for better purposes. This implementation also supports generating
 * synchronized waves as needed for example to emulate dual Hall-effect crankshaft position sensors.
 *
 *
 * @date May 28, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 *
 */

#include "pwm_generator.h"

#include "pin_repository.h"
#include "wave_math.h"
#include "datalogging.h"

static Logging logger;

/**
 * This method controls the actual hardware pins
 */
void applyPinState(PwmConfig *state, int stateIndex) {
	chDbgAssert(state->multiWave.waveCount <= PWM_PHASE_MAX_WAVE_PER_PWM, "invalid waveCount", NULL);
	for (int waveIndex = 0; waveIndex < state->multiWave.waveCount; waveIndex++) {
		io_pin_e ioPin = state->outputPins[waveIndex];
		chDbgAssert(stateIndex < PWM_PHASE_MAX_COUNT, "invalid stateIndex", NULL);
		int value = state->multiWave.waves[waveIndex].pinStates[stateIndex];
		setOutputPinValue(ioPin, value);
	}
}

void wePlainInit(char *msg, PwmConfig *state, GPIO_TypeDef * port, int pin,
		float dutyCycle, float freq, io_pin_e ioPin) {
	float switchTimes[] = { dutyCycle, 1 };
	int pinStates0[] = { 0, 1 };

	int *pinStates[1] = { pinStates0 };

	state->outputPins[0] = ioPin;

	outputPinRegister(msg, state->outputPins[0], port, pin);

	state->period = frequency2period(freq);
	weComplexInit(msg, state, 2, switchTimes, 1, pinStates, NULL, applyPinState);
}

void initPwmGenerator(void) {
	initLogging(&logger, "PWM gen");
}

