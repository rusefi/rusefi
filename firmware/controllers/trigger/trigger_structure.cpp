/**
 * @file	trigger_structure.c
 *
 * @date Jan 20, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "main.h"
#include "trigger_structure.h"

trigger_shape_helper::trigger_shape_helper() {
	waves[0].init(pinStates0);
	waves[1].init(pinStates1);
}

trigger_shape_s::trigger_shape_s() :
		wave(switchTimes, NULL) {
	reset();
	wave.waves = h.waves;
}

int trigger_shape_s::getSize() {
	return size;
}

void trigger_shape_s::reset() {
	size = 0;
	shaftPositionEventCount = 0;
	triggerShapeSynchPointIndex = 0;
	memset(initialState, 0, sizeof(initialState));
	memset(switchTimes, 0, sizeof(switchTimes));
	wave.reset();
}

int multi_wave_s::getChannelState(int channelIndex, int phaseIndex) const {
	return waves[channelIndex].pinStates[phaseIndex];
}

void multi_wave_s::setSwitchTime(int index, float value) {
	switchTimes[index] = value;
}

void clearTriggerState(trigger_state_s *state) {
	state->shaft_is_synchronized = FALSE;
	state->toothed_previous_time = 0;
	state->toothed_previous_duration = 0;
	state->current_index = 0;
}

void trigger_shape_s::addEvent(float angle, trigger_wheel_e waveIndex, trigger_value_e state) {
	angle /= 720;
	if (size == 0) {
		size = 1;
		for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
			single_wave_s *wave = &this->wave.waves[i];

			if (wave == NULL) {
				firmwareError("wave is NULL");
				return;
			}
			if (wave->pinStates == NULL) {
				firmwareError("wave pinStates is NULL");
				return;
			}
			wave->pinStates[0] = initialState[i];
		}

		wave.setSwitchTime(0, angle);
		wave.waves[waveIndex].pinStates[0] = state;
		return;
	}

//	if(angle!=trigger->wave.switchTimes[trigger->currentIndex])

	int index = size++;

	for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++)
		wave.waves[i].pinStates[index] = wave.getChannelState(i, index - 1);
	wave.setSwitchTime(index, angle);
	wave.waves[waveIndex].pinStates[index] = state;
}

void triggerAddEvent(trigger_shape_s *trigger, float angle, trigger_wheel_e waveIndex, trigger_value_e state) {
	// todo: inline this method
	trigger->addEvent(angle, waveIndex, state);
}

void multi_wave_s::checkSwitchTimes(int size) {
	checkSwitchTimes2(size, switchTimes);
}
