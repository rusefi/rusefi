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

pin_state_t SingleWave::getState(int switchIndex) {
	return pinStates[switchIndex];
}

void SingleWave::setState(int switchIndex, pin_state_t state) {
	pinStates[switchIndex] = state;
}

void MultiWave::baseConstructor() {
	channels = NULL;
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

void MultiWave::init(float *switchTimes, SingleWave *channels) {
	this->switchTimes = switchTimes;
	this->channels = channels;
}

void MultiWave::reset(void) {
	waveCount = 0;
}

float MultiWave::getSwitchTime(const int index) const {
	return switchTimes[index];
}

void MultiWave::checkSwitchTimes(const int size) {
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

pin_state_t MultiWave::getChannelState(const int channelIndex, const int phaseIndex) const {
	if (channelIndex >= waveCount) {
		// todo: would be nice to get this asserting working
		//firmwareError(OBD_PCM_Processor_Fault, "channel index %d/%d", channelIndex, waveCount);
	}
	return channels[channelIndex].pinStates[phaseIndex];
}

/**
 * returns the index at which given value would need to be inserted into sorted array
 */
int MultiWave::findInsertionAngle(const float angle, const int size) const {
	for (int i = size - 1; i >= 0; i--) {
		if (angle > switchTimes[i])
			return i + 1;
	}
	return 0;
}

int MultiWave::findAngleMatch(const float angle, const int size) const {
	for (int i = 0; i < size; i++) {
		if (isSameF(switchTimes[i], angle))
			return i;
	}
	return EFI_ERROR_CODE;
}

void MultiWave::setSwitchTime(const int index, const float value) {
	efiAssertVoid(CUSTOM_ERR_6690, switchTimes != NULL, "switchTimes");
	switchTimes[index] = value;
}
