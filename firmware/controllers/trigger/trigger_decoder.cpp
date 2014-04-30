/**
 * @file	trigger_decoder.c
 *
 * @date Dec 24, 2013
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
#include "obd_error_codes.h"
#include "trigger_decoder.h"
#include "cyclic_buffer.h"
extern "C" {
#include "trigger_mazda.h"
#include "trigger_chrysler.h"
#include "trigger_gm.h"
#include "trigger_structure.h"
#include "wave_math.h"
}

#if EFI_PROD_CODE || EFI_SIMULATOR
static Logging logger;
#endif

static cyclic_buffer errorDetection;

/**
 * @return TRUE is something is wrong with trigger decoding
 */
int isTriggerDecoderError(void) {
	return errorDetection.sum(6) > 4;
}

static inline int isSynchronizationGap(trigger_state_s const *shaftPositionState, trigger_shape_s const *triggerShape,
		trigger_config_s const *triggerConfig, const int currentDuration) {
	if (!triggerConfig->isSynchronizationNeeded)
		return FALSE;

	return currentDuration > shaftPositionState->toothed_previous_duration * triggerConfig->syncRatioFrom
			&& currentDuration < shaftPositionState->toothed_previous_duration * triggerConfig->syncRatioTo;
}

static inline int noSynchronizationResetNeeded(trigger_state_s const *shaftPositionState,
		trigger_shape_s const *triggerShape, trigger_config_s const*triggerConfig) {
	if (triggerConfig->isSynchronizationNeeded)
		return FALSE;
	if (!shaftPositionState->shaft_is_synchronized)
		return TRUE;
	/**
	 * in case of noise the counter could be above the expected number of events
	 */
	return shaftPositionState->current_index >= triggerShape->shaftPositionEventCount - 1;
}

/**
 * @brief This method changes the state of trigger_state_s data structure according to the trigger event
 */
void processTriggerEvent(trigger_state_s *shaftPositionState, trigger_shape_s const *triggerShape,
		trigger_config_s const *triggerConfig, ShaftEvents signal, uint64_t nowUs) {

	int isLessImportant = (triggerConfig->useRiseEdge && signal != SHAFT_PRIMARY_UP)
			|| (!triggerConfig->useRiseEdge && signal != SHAFT_PRIMARY_DOWN);

	if (isLessImportant) {
		/**
		 * For less important events we simply increment the index.
		 */
		shaftPositionState->current_index++;
		return;
	}

	int64_t currentDuration = nowUs - shaftPositionState->toothed_previous_time;
	chDbgCheck(currentDuration >= 0, "negative duration?");

// todo: skip a number of signal from the beginning

#if EFI_PROD_CODE
//	scheduleMsg(&logger, "from %f to %f %d %d", triggerConfig->syncRatioFrom, triggerConfig->syncRatioTo, currentDuration, shaftPositionState->toothed_previous_duration);
//	scheduleMsg(&logger, "ratio %f", 1.0 * currentDuration/ shaftPositionState->toothed_previous_duration);
#else
	if (shaftPositionState->toothed_previous_duration != 0) {
//		printf("ratio %f: cur=%d pref=%d\r\n", 1.0 * currentDuration / shaftPositionState->toothed_previous_duration,
//				currentDuration, shaftPositionState->toothed_previous_duration);
	}
#endif

	if (noSynchronizationResetNeeded(shaftPositionState, triggerShape, triggerConfig)
			|| isSynchronizationGap(shaftPositionState, triggerShape, triggerConfig, currentDuration)) {
		/**
		 * We can check if things are fine by comparing the number of events in a cycle with the expected number of event.
		 */
		int isDecodingError = shaftPositionState->current_index != triggerShape->shaftPositionEventCount - 1;
		errorDetection.add(isDecodingError);

		if (isTriggerDecoderError())
			warning(OBD_PCM_Processor_Fault, "trigger decoding issue");

		shaftPositionState->shaft_is_synchronized = TRUE;
		shaftPositionState->current_index = 0;
	} else {
		shaftPositionState->current_index++;
	}

	shaftPositionState->toothed_previous_duration = currentDuration;
	shaftPositionState->toothed_previous_time = nowUs;

}

static void initializeSkippedToothTriggerShape(trigger_shape_s *s, int totalTeethCount, int skippedCount) {
	efiAssert(s != NULL, "trigger_shape_s is NULL");
	s->reset();

	float toothWidth = 0.5;

	for (int i = 0; i < totalTeethCount - skippedCount - 1; i++) {
		float angleDown = 720.0 / totalTeethCount * (i + toothWidth);
		float angleUp = 720.0 / totalTeethCount * (i + 1);
		triggerAddEvent(s, angleDown, T_PRIMARY, TV_HIGH);
		triggerAddEvent(s, angleUp, T_PRIMARY, TV_LOW);
	}

	float angleDown = 720.0 / totalTeethCount * (totalTeethCount - skippedCount - 1 + toothWidth);
	triggerAddEvent(s, angleDown, T_PRIMARY, TV_HIGH);
	triggerAddEvent(s, 720, T_PRIMARY, TV_LOW);
}

void initializeSkippedToothTriggerShapeExt(engine_configuration2_s *engineConfiguration2, int totalTeethCount,
		int skippedCount) {
	trigger_shape_s *s = &engineConfiguration2->triggerShape;
	initializeSkippedToothTriggerShape(s, totalTeethCount, skippedCount);

	s->shaftPositionEventCount = ((totalTeethCount - skippedCount) * 2);
	s->wave.checkSwitchTimes(s->size);
}

static void configureFordAspireTriggerShape(trigger_shape_s * s) {
	triggerShapeInit(s);

	s->shaftPositionEventCount = 10;

	triggerAddEvent(s, 53.747, T_SECONDARY, TV_HIGH);
	triggerAddEvent(s, 121.90, T_SECONDARY, TV_LOW); // delta = 68.153
	triggerAddEvent(s, 232.76, T_SECONDARY, TV_HIGH); // delta = 110.86
	triggerAddEvent(s, 300.54, T_SECONDARY, TV_LOW); // delta = 67.78
	triggerAddEvent(s, 360, T_PRIMARY, TV_HIGH);

	triggerAddEvent(s, 409.8412, T_SECONDARY, TV_HIGH); // delta = 49.8412
	triggerAddEvent(s, 478.6505, T_SECONDARY, TV_LOW); // delta = 68.8093
	triggerAddEvent(s, 588.045, T_SECONDARY, TV_HIGH); // delta = 109.3945
	triggerAddEvent(s, 657.03, T_SECONDARY, TV_LOW);
	triggerAddEvent(s, 720, T_PRIMARY, TV_LOW);
}

void initializeTriggerShape(engine_configuration_s *engineConfiguration,
		engine_configuration2_s *engineConfiguration2) {
	trigger_config_s *tt = &engineConfiguration->triggerConfig;
	switch (tt->triggerType) {

	case TT_TOOTHED_WHEEL:
		initializeSkippedToothTriggerShapeExt(engineConfiguration2, tt->totalToothCount, tt->skippedToothCount);
		return;

	case TT_MAZDA_MIATA_NB:
		initializeMazdaMiataNbShape(&engineConfiguration2->triggerShape);
		return;

	case TT_DODGE_NEON:
		configureNeonTriggerShape(&engineConfiguration2->triggerShape);
		return;

	case TT_FORD_ASPIRE:
		configureFordAspireTriggerShape(&engineConfiguration2->triggerShape);
		return;

	case TT_GM_7X:
		configureGmTriggerShape(&engineConfiguration2->triggerShape);
		return;

	case TT_FORD_ESCORT_GT:
		configureMazdaProtegeLx(engineConfiguration, engineConfiguration2);
		return;

	default:
		firmwareError("initializeTriggerShape() not implemented: %d", tt->triggerType);
		;
	}
}

int findTriggerZeroEventIndex(trigger_shape_s const * shape, trigger_config_s const*triggerConfig) {

	trigger_state_s state;
	clearTriggerState(&state);
	errorDetection.clear();

	int primaryWheelState = FALSE;
	int secondaryWheelState = FALSE;

	for (int i = 0; i < 100; i++) {

		int stateIndex = i % shape->size;

		int loopIndex = i / shape->size;

		int time = 10000 * (loopIndex + shape->wave.getSwitchTime(stateIndex));

		int newPrimaryWheelState = shape->wave.getChannelState(0, stateIndex);
		int newSecondaryWheelState = shape->wave.getChannelState(1, stateIndex);

		if (primaryWheelState != newPrimaryWheelState) {
			primaryWheelState = newPrimaryWheelState;
			ShaftEvents s = primaryWheelState ? SHAFT_PRIMARY_UP : SHAFT_PRIMARY_DOWN;
			processTriggerEvent(&state, shape, triggerConfig, s, time);
		}

		if (secondaryWheelState != newSecondaryWheelState) {
			secondaryWheelState = newSecondaryWheelState;
			ShaftEvents s = secondaryWheelState ? SHAFT_SECONDARY_UP : SHAFT_SECONDARY_DOWN;
			processTriggerEvent(&state, shape, triggerConfig, s, time);
		}

		if (state.shaft_is_synchronized) {
			return stateIndex;
		}
	}
	firmwareError("findTriggerZeroEventIndex() failed");
	return -1;
}

void initTriggerDecoder(void) {
#if EFI_PROD_CODE || EFI_SIMULATOR
	initLogging(&logger, "trigger decoder");
#endif
}

