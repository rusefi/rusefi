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

void clearTriggerState(trigger_state_s *state) {
	state->shaft_is_synchronized = FALSE;
	state->toothed_previous_time = 0;
	state->toothed_previous_duration = 0;
	state->current_index = 0;
}

void triggerShapeInit(trigger_shape_s *trigger) {
	memset(trigger, 0, sizeof(trigger_shape_s));
}

void triggerAddEvent(trigger_shape_s *trigger, float angle, trigger_wheel_e waveIndex, trigger_value_e state) {
	angle /= 720;
	if (trigger->size == 0) {
		trigger->size = 1;
		for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++)
			trigger->wave.waves[i].pinStates[0] = 0;

		trigger->wave.switchTimes[0] = angle;
		trigger->wave.waves[waveIndex].pinStates[0] = state;
		return;
	}

//	if(angle!=trigger->wave.switchTimes[trigger->currentIndex])

	int index = trigger->size++;

	for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++)
		trigger->wave.waves[i].pinStates[index] = trigger->wave.waves[i].pinStates[index - 1];
	trigger->wave.switchTimes[index] = angle;
	trigger->wave.waves[waveIndex].pinStates[index] = state;
}

void checkSwitchTimes(int size, float *switchTimes) {
	for (int i = 0; i < size - 1; i++)
		chDbgCheck(switchTimes[i] < switchTimes[i + 1], "invalid switchTimes");
}
