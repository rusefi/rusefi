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

single_wave_s::single_wave_s(int *ps) {
	this->pinStates = ps;
}

multi_wave_s::multi_wave_s(float *switchTimes, single_wave_s *waves) {
	this->switchTimes = switchTimes;
	this->waves = waves;
}

void multi_wave_s::reset(void) {
	phaseCount = 0;
	waveCount = 0;
}

float multi_wave_s::getSwitchTime(int index) const {
	return switchTimes[index];
}

trigger_shape_s::trigger_shape_s() :
		wave(switchTimes, h.waves) {
	reset();
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

void triggerShapeInit(trigger_shape_s *trigger) {
	// todo: inline & remove method
	trigger->reset();
}

void triggerAddEvent(trigger_shape_s *trigger, float angle, trigger_wheel_e waveIndex, trigger_value_e state) {
	angle /= 720;
	if (trigger->size == 0) {
		trigger->size = 1;
		for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
			single_wave_s *wave = &trigger->wave.waves[i];

			efiAssert(wave!=NULL, "wave is NULL");
			efiAssert(wave->pinStates!=NULL, "wave pinStates is NULL");
			wave->pinStates[0] = trigger->initialState[i];
		}

		trigger->wave.setSwitchTime(0, angle);
		trigger->wave.waves[waveIndex].pinStates[0] = state;
		return;
	}

//	if(angle!=trigger->wave.switchTimes[trigger->currentIndex])

	int index = trigger->size++;

	for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++)
		trigger->wave.waves[i].pinStates[index] = trigger->wave.getChannelState(i, index - 1);
	trigger->wave.setSwitchTime(index, angle);
	trigger->wave.waves[waveIndex].pinStates[index] = state;
}

void checkSwitchTimes2(int size, float *switchTimes) {

	for (int i = 0; i < size - 1; i++)
		chDbgCheck(switchTimes[i] < switchTimes[i + 1], "invalid switchTimes");
}

void multi_wave_s::checkSwitchTimes(int size) {
	checkSwitchTimes2(size, switchTimes);
}
