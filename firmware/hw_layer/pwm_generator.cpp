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
 * @author Andrey Belomutskiy, (c) 2012-2014
 *
 */

#include "pwm_generator.h"

#include "pin_repository.h"
#include "datalogging.h"

static Logging logger;

/**
 * This method controls the actual hardware pins
 */
void applyPinState(PwmConfig *state, int stateIndex) {
	efiAssertVoid(state->multiWave.waveCount <= PWM_PHASE_MAX_WAVE_PER_PWM, "invalid waveCount");
	for (int waveIndex = 0; waveIndex < state->multiWave.waveCount; waveIndex++) {
		io_pin_e ioPin = state->outputPins[waveIndex];
		efiAssertVoid(stateIndex < PWM_PHASE_MAX_COUNT, "invalid stateIndex");
		int value = state->multiWave.waves[waveIndex].pinStates[stateIndex];
		setOutputPinValue(ioPin, value);
	}
}

void startSimplePwm(PwmConfig *state, const char *msg, io_pin_e ioPin,
		float frequency, float dutyCycle) {
	efiAssertVoid(dutyCycle >= 0 && dutyCycle <= 1, "dutyCycle");

	float switchTimes[] = { dutyCycle, 1 };
	int pinStates0[] = { 0, 1 };

	int *pinStates[1] = { pinStates0 };

	state->outputPins[0] = ioPin;

	state->periodUs = frequency2periodUs(frequency);
	weComplexInit(msg, state, 2, switchTimes, 1, pinStates, NULL, applyPinState);
}

void startSimplePwmExt(PwmConfig *state, const char *msg, brain_pin_e brainPin, io_pin_e ioPin,
		float frequency, float dutyCycle) {

	GPIO_TypeDef * port = getHwPort(brainPin);
	int pin = getHwPin(brainPin);
	outputPinRegister(msg, ioPin, port, pin);

	startSimplePwm(state, msg, ioPin, frequency, dutyCycle);
}

void initPwmGenerator(void) {
	initLogging(&logger, "PWM gen");
}

