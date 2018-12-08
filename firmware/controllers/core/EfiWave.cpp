/**
 * @file	EfiWave.cpp
 *
 * @date May 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"
#include "EfiWave.h"
#include "trigger_structure.h"

single_wave_s::single_wave_s() {
	init(NULL);
}

single_wave_s::single_wave_s(pin_state_t *ps) {
	init(ps);
}

void single_wave_s::init(pin_state_t *pinStates) {
	this->pinStates = pinStates;
}

void multi_wave_s::baseConstructor() {
	waves = NULL;
	switchTimes = NULL;
	reset();
}

multi_wave_s::multi_wave_s() {
	baseConstructor();
}

multi_wave_s::multi_wave_s(float *switchTimes, single_wave_s *waves) {
	baseConstructor();
	init(switchTimes, waves);
}

void multi_wave_s::init(float *switchTimes, single_wave_s *waves) {
	this->switchTimes = switchTimes;
	this->waves = waves;
}

void multi_wave_s::reset(void) {
	waveCount = 0;
}

float multi_wave_s::getSwitchTime(int index) const {
	return switchTimes[index];
}

void checkSwitchTimes2(int size, float *switchTimes) {
	if (switchTimes[size - 1] != 1) {
		firmwareError(CUSTOM_ERR_WAVE_1, "last switch time has to be 1 not %.2f", switchTimes[size - 1]);
		return;
	}
	for (int i = 0; i < size - 1; i++) {
		if (switchTimes[i] >= switchTimes[i + 1]) {
			firmwareError(CUSTOM_ERR_WAVE_2, "invalid switchTimes @%d: %.2f/%.2f", i, switchTimes[i], switchTimes[i + 1]);
		}
	}
}

int multi_wave_s::getChannelState(int channelIndex, int phaseIndex) const {
	return waves[channelIndex].pinStates[phaseIndex];
}

/**
 * returns the index at which given value would need to be inserted into sorted array
 */
int multi_wave_s::findInsertionAngle(float angle, int size) const {
	for (int i = size - 1; i >= 0; i--) {
		if (angle > switchTimes[i])
			return i + 1;
	}
	return 0;
}

int multi_wave_s::findAngleMatch(float angle, int size) const {
	for (int i = 0; i < size; i++) {
		if (isSameF(switchTimes[i], angle))
			return i;
	}
	return EFI_ERROR_CODE;
}

void multi_wave_s::setSwitchTime(int index, float value) {
	efiAssertVoid(CUSTOM_ERR_6690, switchTimes != NULL, "switchTimes");
	switchTimes[index] = value;
}

