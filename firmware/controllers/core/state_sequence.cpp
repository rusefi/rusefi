/**
 * @file	state_sequence.cpp
 *
 * @date May 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "state_sequence.h"
#include "trigger_structure.h"

SingleChannelStateSequence::SingleChannelStateSequence() {
	init(NULL);
}

SingleChannelStateSequence::SingleChannelStateSequence(pin_state_t *ps) {
	init(ps);
}

void SingleChannelStateSequence::init(pin_state_t *pinStates) {
	this->pinStates = pinStates;
}

pin_state_t SingleChannelStateSequence::getState(int switchIndex) const {
	pin_state_t state = pinStates[switchIndex];
	efiAssert(OBD_PCM_Processor_Fault, state == 0 || state == 1, "wave state get", TV_FALL);
	return state;
}

void SingleChannelStateSequence::setState(int switchIndex, pin_state_t state) {
	efiAssertVoid(OBD_PCM_Processor_Fault, state == 0 || state == 1, "wave state set");
	pinStates[switchIndex] = state;
}

MultiChannelStateSequence::MultiChannelStateSequence() {
	reset();
}

MultiChannelStateSequence::MultiChannelStateSequence(float *switchTimes, SingleChannelStateSequence *waves) : MultiChannelStateSequence() {
	init(switchTimes, waves);
}

void MultiChannelStateSequence::init(float *switchTimes, SingleChannelStateSequence *channels) {
	this->switchTimes = switchTimes;
	this->channels = channels;
}

void MultiChannelStateSequence::reset(void) {
	waveCount = 0;
}

float MultiChannelStateSequence::getSwitchTime(const int index) const {
	return switchTimes[index];
}

void MultiChannelStateSequence::checkSwitchTimes(const int size, const float scale) {
	if (switchTimes[size - 1] != 1) {
		firmwareError(CUSTOM_ERR_WAVE_1, "last switch time has to be 1/%f not %.2f/%f", scale,
				switchTimes[size - 1], scale * switchTimes[size - 1]);
		return;
	}
	for (int i = 0; i < size - 1; i++) {
		if (switchTimes[i] >= switchTimes[i + 1]) {
			firmwareError(CUSTOM_ERR_WAVE_2, "invalid switchTimes @%d: %.2f/%.2f", i, switchTimes[i], switchTimes[i + 1]);
		}
	}
}

pin_state_t MultiChannelStateSequence::getChannelState(const int channelIndex, const int phaseIndex) const {
	if (channelIndex >= waveCount) {
		// todo: would be nice to get this asserting working
		//firmwareError(OBD_PCM_Processor_Fault, "channel index %d/%d", channelIndex, waveCount);
	}
	return channels[channelIndex].pinStates[phaseIndex];
}

/**
 * returns the index at which given value would need to be inserted into sorted array
 */
int MultiChannelStateSequence::findInsertionAngle(const float angle, const int size) const {
	for (int i = size - 1; i >= 0; i--) {
		if (angle > switchTimes[i])
			return i + 1;
	}
	return 0;
}

int MultiChannelStateSequence::findAngleMatch(const float angle, const int size) const {
	for (int i = 0; i < size; i++) {
		if (isSameF(switchTimes[i], angle))
			return i;
	}
	return EFI_ERROR_CODE;
}

void MultiChannelStateSequence::setSwitchTime(const int index, const float value) {
	efiAssertVoid(CUSTOM_ERR_PWM_SWITCH_ASSERT, switchTimes != NULL, "switchTimes");
	switchTimes[index] = value;
}
