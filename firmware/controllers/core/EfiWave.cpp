/**
 * @file	EfiWave.cpp
 *
 * @date May 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"
#include "EfiWave.h"
#include "trigger_structure.h"

SingleWave::SingleWave() {
	init(NULL);
}

SingleWave::SingleWave(pin_state_t *ps) {
	init(ps);
}

void SingleWave::init(pin_state_t *pinStates) {
	this->pinStates = pinStates;
}

int SingleWave::getState(int index) {
	return pinStates[index];
}

void SingleWave::setState(int index, int state) {
	pinStates[index] = state;
}

void MultiWave::baseConstructor() {
	waves = NULL;
	switchTimes = NULL;
	reset();
}

MultiWave::MultiWave() {
	baseConstructor();
}

MultiWave::MultiWave(float *switchTimes, SingleWave *waves) {
	baseConstructor();
	init(switchTimes, waves);
}

void MultiWave::init(float *switchTimes, SingleWave *waves) {
	this->switchTimes = switchTimes;
	this->waves = waves;
}

void MultiWave::reset(void) {
	waveCount = 0;
}

float MultiWave::getSwitchTime(int index) const {
	return switchTimes[index];
}

void MultiWave::checkSwitchTimes(int size) {
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

int MultiWave::getChannelState(int channelIndex, int phaseIndex) const {
	return waves[channelIndex].pinStates[phaseIndex];
}

/**
 * returns the index at which given value would need to be inserted into sorted array
 */
int MultiWave::findInsertionAngle(float angle, int size) const {
	for (int i = size - 1; i >= 0; i--) {
		if (angle > switchTimes[i])
			return i + 1;
	}
	return 0;
}

int MultiWave::findAngleMatch(float angle, int size) const {
	for (int i = 0; i < size; i++) {
		if (isSameF(switchTimes[i], angle))
			return i;
	}
	return EFI_ERROR_CODE;
}

void MultiWave::setSwitchTime(int index, float value) {
	efiAssertVoid(CUSTOM_ERR_6690, switchTimes != NULL, "switchTimes");
	switchTimes[index] = value;
}
