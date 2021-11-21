/**
 * @file	state_sequence.cpp
 *
 * @date May 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "state_sequence.h"
#include "trigger_structure.h"

MultiChannelStateSequence::MultiChannelStateSequence(unsigned maxWaveCount)
	: wavePtr(reinterpret_cast<uint8_t *>(&switchTimes[maxWaveCount])) {
	reset();
}

void MultiChannelStateSequence::reset(void) {
	waveCount = 0;
}

float MultiChannelStateSequence::getSwitchTime(const int index) const {
	return switchTimes[index];
}

void MultiChannelStateSequence::checkSwitchTimes(const float scale) const {
	if (switchTimes[phaseCount - 1] != 1) {
		firmwareError(CUSTOM_ERR_WAVE_1, "last switch time has to be 1/%f not %.2f/%f", scale,
				switchTimes[phaseCount - 1], scale * switchTimes[phaseCount - 1]);
		return;
	}
	for (int i = 0; i < phaseCount - 1; i++) {
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
	return ((wavePtr[phaseIndex] >> channelIndex) & 1) ? TV_RISE : TV_FALL;
}

void MultiChannelStateSequence::setChannelState(const int channelIndex, const int phaseIndex,
						pin_state_t state) {
	if (channelIndex >= waveCount) {
		// todo: would be nice to get this asserting working
		//firmwareError(OBD_PCM_Processor_Fault, "channel index %d/%d", channelIndex, waveCount);
	}
	uint8_t & ref = wavePtr[phaseIndex];
	ref = (ref & ~(1U << channelIndex)) | ((state == TV_RISE ? 1 : 0) << channelIndex);
}

/**
 * returns the index at which given value would need to be inserted into sorted array
 */
int MultiChannelStateSequence::findInsertionAngle(const float angle) const {
	for (int i = phaseCount - 1; i >= 0; i--) {
		if (angle > switchTimes[i])
			return i + 1;
	}
	return 0;
}

int MultiChannelStateSequence::findAngleMatch(const float angle) const {
	for (int i = 0; i < phaseCount; i++) {
		if (isSameF(switchTimes[i], angle))
			return i;
	}
	return EFI_ERROR_CODE;
}

void MultiChannelStateSequence::setSwitchTime(const int index, const float value) {
	efiAssertVoid(CUSTOM_ERR_PWM_SWITCH_ASSERT, switchTimes != NULL, "switchTimes");
	switchTimes[index] = value;
}
