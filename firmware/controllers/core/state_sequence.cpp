/**
 * @file	state_sequence.cpp
 *
 * @date May 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "state_sequence.h"
#include "trigger_structure.h"

void MultiChannelStateSequence::checkSwitchTimes(const float scale) const {
	if (getSwitchTime(phaseCount - 1) != 1) {
		firmwareError(CUSTOM_ERR_WAVE_1, "last switch time has to be 1/%f not %.2f/%f",
			      scale, getSwitchTime(phaseCount - 1),
			      scale * getSwitchTime(phaseCount - 1));
		return;
	}
	for (int i = 0; i < phaseCount - 1; i++) {
		if (getSwitchTime(i) >= getSwitchTime(i + 1)) {
			firmwareError(CUSTOM_ERR_WAVE_2, "invalid switchTimes @%d: %.2f/%.2f",
				      i, getSwitchTime(i), getSwitchTime(i + 1));
		}
	}
}

int MultiChannelStateSequence::findInsertionAngle(const float angle) const {
	for (int i = phaseCount - 1; i >= 0; i--) {
		if (angle > getSwitchTime(i))
			return i + 1;
	}
	return 0;
}

int MultiChannelStateSequence::findAngleMatch(const float angle) const {
	for (int i = 0; i < phaseCount; i++) {
		if (isSameF(getSwitchTime(i), angle))
			return i;
	}
	return EFI_ERROR_CODE;
}
