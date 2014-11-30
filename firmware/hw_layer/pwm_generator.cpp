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
 *
 * This method takes ~350 ticks.
 */
void applyPinState(PwmConfig *state, int stateIndex) {
	efiAssertVoid(stateIndex < PWM_PHASE_MAX_COUNT, "invalid stateIndex");
	efiAssertVoid(state->multiWave.waveCount <= PWM_PHASE_MAX_WAVE_PER_PWM, "invalid waveCount");
	for (int waveIndex = 0; waveIndex < state->multiWave.waveCount; waveIndex++) {
		io_pin_e ioPin = state->outputPins[waveIndex];
		int value = state->multiWave.waves[waveIndex].pinStates[stateIndex];
		setOutputPinValue(ioPin, value);
	}
}

void startSimplePwm(PwmConfig *state, const char *msg, io_pin_e ioPin, float frequency, float dutyCycle, pwm_gen_callback *stateChangeCallback) {
	efiAssertVoid(dutyCycle >= 0 && dutyCycle <= 1, "dutyCycle");

	float switchTimes[] = { dutyCycle, 1 };
	int pinStates0[] = { 0, 1 };

	int *pinStates[1] = { pinStates0 };

	state->outputPins[0] = ioPin;

	state->periodNt = US2NT(frequency2periodUs(frequency));
	weComplexInit(msg, state, 2, switchTimes, 1, pinStates, NULL, stateChangeCallback);
}

void startSimplePwmExt(PwmConfig *state, const char *msg, brain_pin_e brainPin, io_pin_e ioPin, float frequency,
		float dutyCycle, pwm_gen_callback *stateChangeCallback) {

	GPIO_TypeDef * port = getHwPort(brainPin);
	int pin = getHwPin(brainPin);
	outputPinRegister(msg, ioPin, port, pin);

	startSimplePwm(state, msg, ioPin, frequency, dutyCycle, stateChangeCallback);
}

void initPwmGenerator(void) {
	initLogging(&logger, "PWM gen");
}

