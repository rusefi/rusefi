/**
 * @file	trigger_decoder.cpp
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
#include "trigger_mazda.h"
#include "trigger_chrysler.h"
#include "trigger_gm.h"
#include "trigger_bmw.h"
#include "trigger_mitsubishi.h"

#include "trigger_structure.h"
#include "efiGpio.h"
#include "engine.h"

// todo: better name for this constant
#define HELPER_PERIOD 100000

static cyclic_buffer errorDetection;

#if ! EFI_PROD_CODE
bool printGapRatio = false;
#endif /* ! EFI_PROD_CODE */

#if (EFI_PROD_CODE || EFI_SIMULATOR)
static Logging logger;
#endif

/**
 * @return TRUE is something is wrong with trigger decoding
 */
bool_t isTriggerDecoderError(void) {
	return errorDetection.sum(6) > 4;
}

static ALWAYS_INLINE bool isSynchronizationGap(TriggerState *shaftPositionState, trigger_shape_s const *triggerShape,
		const int currentDuration) {
	if (!triggerShape->isSynchronizationNeeded) {
		return false;
	}

#if ! EFI_PROD_CODE
	if (printGapRatio) {

		float gap = 1.0 * currentDuration / shaftPositionState->toothed_previous_duration;
		print("current gap %f\r\n", gap);
	}
#else
//	float gap = 1.0 * currentDuration / shaftPositionState->toothed_previous_duration;
//	scheduleMsg(&logger, "gap=%f @ %d", gap, shaftPositionState->getCurrentIndex());

#endif /* ! EFI_PROD_CODE */

	return currentDuration > shaftPositionState->toothed_previous_duration * triggerShape->syncRatioFrom
			&& currentDuration < shaftPositionState->toothed_previous_duration * triggerShape->syncRatioTo;
}

static ALWAYS_INLINE bool noSynchronizationResetNeeded(TriggerState *shaftPositionState,
		trigger_shape_s const *triggerShape) {
	if (triggerShape->isSynchronizationNeeded) {
		return false;
	}
	if (!shaftPositionState->shaft_is_synchronized) {
		return true;
	}
	/**
	 * in case of noise the counter could be above the expected number of events
	 */
	return shaftPositionState->getCurrentIndex() >= triggerShape->shaftPositionEventCount - 1;
}

float TriggerState::getTriggerDutyCycle(int index) {
	float time = prevTotalTime[index];

	return 100 * time / prevCycleDuration;
}

static trigger_wheel_e eventIndex[6] = { T_PRIMARY, T_PRIMARY, T_SECONDARY, T_SECONDARY, T_CHANNEL_3, T_CHANNEL_3 };
static trigger_value_e eventType[6] = { TV_LOW, TV_HIGH, TV_LOW, TV_HIGH, TV_LOW, TV_HIGH };

#define getCurrentGapDuration(nowUs) \
	(isFirstEvent ? 0 : (nowUs) - toothed_previous_time)

/**
 * @brief Trigger decoding happens here
 * This method changes the state of trigger_state_s data structure according to the trigger event
 */
void TriggerState::decodeTriggerEvent(trigger_shape_s const*triggerShape, trigger_config_s const*triggerConfig,
		trigger_event_e const signal, uint64_t nowNt) {
	(void) triggerConfig; // we might want this for logging?
	efiAssertVoid(signal <= SHAFT_3RD_UP, "unexpected signal");

	trigger_wheel_e triggerWheel = eventIndex[signal];

	if (curSignal == prevSignal) {
		orderingErrorCounter++;
	}

	prevSignal = curSignal;
	curSignal = signal;

	eventCount[triggerWheel]++;
	eventCountExt[signal]++;

	int isLessImportant = (triggerShape->useRiseEdge && signal != SHAFT_PRIMARY_UP)
			|| (!triggerShape->useRiseEdge && signal != SHAFT_PRIMARY_DOWN);

	if (isLessImportant) {
		/**
		 * For less important events we simply increment the index.
		 */
		nextTriggerEvent(triggerWheel, nowNt);
		if (triggerShape->gapBothDirections) {
			toothed_previous_duration = getCurrentGapDuration(nowNt);
			isFirstEvent = false;
			toothed_previous_time = nowNt;
		}
		return;
	}

	currentDuration = getCurrentGapDuration(nowNt);
	isFirstEvent = false;
	efiAssertVoid(currentDuration >= 0, "decode: negative duration?");

// todo: skip a number of signal from the beginning

#if EFI_PROD_CODE
//	scheduleMsg(&logger, "from %f to %f %d %d", triggerConfig->syncRatioFrom, triggerConfig->syncRatioTo, currentDuration, shaftPositionState->toothed_previous_duration);
//	scheduleMsg(&logger, "ratio %f", 1.0 * currentDuration/ shaftPositionState->toothed_previous_duration);
#else
	if (toothed_previous_duration != 0) {
//		printf("ratio %f: cur=%d pref=%d\r\n", 1.0 * currentDuration / shaftPositionState->toothed_previous_duration,
//				currentDuration, shaftPositionState->toothed_previous_duration);
	}
#endif

	if (noSynchronizationResetNeeded(this, triggerShape) || isSynchronizationGap(this, triggerShape, currentDuration)) {
		/**
		 * We can check if things are fine by comparing the number of events in a cycle with the expected number of event.
		 */
		bool isDecodingError = eventCount[0] != triggerShape->expectedEventCount[0]
				|| eventCount[1] != triggerShape->expectedEventCount[1]
				|| eventCount[2] != triggerShape->expectedEventCount[2];

		setOutputPinValue(LED_TRIGGER_ERROR, isDecodingError);
		if (isDecodingError) {
			totalTriggerErrorCounter++;
		}

		errorDetection.add(isDecodingError);

		if (isTriggerDecoderError()) {
			warning(OBD_PCM_Processor_Fault, "trigger decoding issue. expected %d/%d/%d got %d/%d/%d",
					triggerShape->expectedEventCount[0], triggerShape->expectedEventCount[1],
					triggerShape->expectedEventCount[2], eventCount[0], eventCount[1], eventCount[2]);
		}

		shaft_is_synchronized = true;
		// this call would update duty cycle values
		nextTriggerEvent(triggerWheel, nowNt);

		nextRevolution(triggerShape->shaftPositionEventCount, nowNt);
	} else {
		nextTriggerEvent(triggerWheel, nowNt);
	}

	toothed_previous_duration = currentDuration;
	toothed_previous_time = nowNt;
}

static void initializeSkippedToothTriggerShape(trigger_shape_s *s, int totalTeethCount, int skippedCount,
		operation_mode_e operationMode) {
	efiAssertVoid(s != NULL, "trigger_shape_s is NULL");
	s->reset(operationMode);

	float toothWidth = 0.5;

	for (int i = 0; i < totalTeethCount - skippedCount - 1; i++) {
		float angleDown = 720.0 / totalTeethCount * (i + toothWidth);
		float angleUp = 720.0 / totalTeethCount * (i + 1);
		s->addEvent(angleDown, T_PRIMARY, TV_HIGH);
		s->addEvent(angleUp, T_PRIMARY, TV_LOW);
	}

	float angleDown = 720.0 / totalTeethCount * (totalTeethCount - skippedCount - 1 + toothWidth);
	s->addEvent(angleDown, T_PRIMARY, TV_HIGH);
	s->addEvent(720, T_PRIMARY, TV_LOW);
}

void initializeSkippedToothTriggerShapeExt(trigger_shape_s *s, int totalTeethCount, int skippedCount,
		operation_mode_e operationMode) {
	efiAssertVoid(totalTeethCount > 0, "totalTeethCount is zero");

	s->totalToothCount = totalTeethCount;
	s->skippedToothCount = skippedCount;
	initializeSkippedToothTriggerShape(s, totalTeethCount, skippedCount, operationMode);

	s->assignSize();
}

/**
 * External logger is needed because at this point our logger is not yet initialized
 */
void initializeTriggerShape(Logging *logger, engine_configuration_s const *engineConfiguration,
		Engine *engine) {
#if EFI_PROD_CODE
	scheduleMsg(logger, "initializeTriggerShape()");
#endif
	const trigger_config_s *triggerConfig = &engineConfiguration->triggerConfig;
	trigger_shape_s *triggerShape = &engine->triggerShape;

	setTriggerSynchronizationGap(triggerShape, 2);
	triggerShape->useRiseEdge = true;

	switch (triggerConfig->triggerType) {

	case TT_TOOTHED_WHEEL:
		// todo: move to into configuration definition		engineConfiguration2->triggerShape.needSecondTriggerInput = false;

		triggerShape->isSynchronizationNeeded =
				engineConfiguration->triggerConfig.customIsSynchronizationNeeded;

		initializeSkippedToothTriggerShapeExt(triggerShape, triggerConfig->customTotalToothCount,
				triggerConfig->customSkippedToothCount, getOperationMode(engineConfiguration));
		break;

	case TT_MAZDA_MIATA_NA:
		initializeMazdaMiataNaShape(triggerShape);
		break;

	case TT_MAZDA_MIATA_NB:
		initializeMazdaMiataNbShape(triggerShape);
		break;

	case TT_DODGE_NEON_1995:
		configureNeon1995TriggerShape(triggerShape);
		break;

	case TT_DODGE_NEON_2003:
		configureNeon2003TriggerShape(triggerShape);
		break;

	case TT_FORD_ASPIRE:
		configureFordAspireTriggerShape(triggerShape);
		break;

	case TT_GM_7X:
		configureGmTriggerShape(triggerShape);
		break;

	case TT_FORD_ESCORT_GT:
		configureMazdaProtegeLx(triggerShape);
		break;

	case TT_MINI_COOPER_R50:
		configureMiniCooperTriggerShape(triggerShape);
		break;

	case TT_TOOTHED_WHEEL_60_2:
		setToothedWheelConfiguration(triggerShape, 60, 2, engineConfiguration);
		setTriggerSynchronizationGap(triggerShape, 2.5);
		break;

	case TT_TOOTHED_WHEEL_36_1:
		setToothedWheelConfiguration(triggerShape, 36, 1, engineConfiguration);
		break;

	case TT_HONDA_ACCORD_CD_TWO_WIRES:
		configureHondaAccordCD(triggerShape, false);
		break;

	case TT_HONDA_ACCORD_CD:
		configureHondaAccordCD(triggerShape, true);
		break;

	case TT_HONDA_ACCORD_CD_DIP:
		configureHondaAccordCDDip(triggerShape);
		break;

	case TT_MITSU:
		initializeMitsubishi4g18(triggerShape);
		break;

	default:
		firmwareError("initializeTriggerShape() not implemented: %d", triggerConfig->triggerType);
		;
		return;
	}
	triggerShape->assignSize();
	triggerShape->wave.checkSwitchTimes(triggerShape->getSize());
}

TriggerStimulatorHelper::TriggerStimulatorHelper() {
	primaryWheelState = false;
	secondaryWheelState = false;
	thirdWheelState = false;
}

void TriggerStimulatorHelper::nextStep(TriggerState *state, trigger_shape_s * shape, int i,
		trigger_config_s const*triggerConfig) {
	int stateIndex = i % shape->getSize();

	int loopIndex = i / shape->getSize();

	int time = (int) (HELPER_PERIOD * (loopIndex + shape->wave.getSwitchTime(stateIndex)));

	bool newPrimaryWheelState = shape->wave.getChannelState(0, stateIndex);
	bool newSecondaryWheelState = shape->wave.getChannelState(1, stateIndex);
	bool new3rdWheelState = shape->wave.getChannelState(2, stateIndex);

	if (primaryWheelState != newPrimaryWheelState) {
		primaryWheelState = newPrimaryWheelState;
		trigger_event_e s = primaryWheelState ? SHAFT_PRIMARY_UP : SHAFT_PRIMARY_DOWN;
		state->decodeTriggerEvent(shape, triggerConfig, s, time);
	}

	if (secondaryWheelState != newSecondaryWheelState) {
		secondaryWheelState = newSecondaryWheelState;
		trigger_event_e s = secondaryWheelState ? SHAFT_SECONDARY_UP : SHAFT_SECONDARY_DOWN;
		state->decodeTriggerEvent(shape, triggerConfig, s, time);
	}

	if (thirdWheelState != new3rdWheelState) {
		thirdWheelState = new3rdWheelState;
		trigger_event_e s = thirdWheelState ? SHAFT_3RD_UP : SHAFT_3RD_DOWN;
		state->decodeTriggerEvent(shape, triggerConfig, s, time);
	}
}

static void onFindIndex(TriggerState *state) {
	for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
		// todo: that's not the best place for this intermediate data storage, fix it!
		state->expectedTotalTime[i] = state->totalTimeNt[i];
	}
}

static uint32_t doFindTrigger(TriggerStimulatorHelper *helper, trigger_shape_s * shape,
		trigger_config_s const*triggerConfig, TriggerState *state) {
	for (int i = 0; i < 4 * PWM_PHASE_MAX_COUNT; i++) {
		helper->nextStep(state, shape, i, triggerConfig);

		if (state->shaft_is_synchronized)
			return i;
	}
	firmwareError("findTriggerZeroEventIndex() failed");
	return EFI_ERROR_CODE;
}

/**
 * Trigger shape is defined in a way which is convenient for trigger shape definition
 * On the other hand, trigger decoder indexing begins from synchronization event.
 *
 * This function finds the index of synchronization event within trigger_shape_s
 */
uint32_t findTriggerZeroEventIndex(trigger_shape_s * shape, trigger_config_s const*triggerConfig) {

	TriggerState state;
	errorDetection.clear();

	TriggerStimulatorHelper helper;

	uint32_t index = doFindTrigger(&helper, shape, triggerConfig, &state);
	if (index == EFI_ERROR_CODE) {
		return index;
	}
	efiAssert(state.getTotalRevolutionCounter() == 1, "totalRevolutionCounter", EFI_ERROR_CODE);

	/**
	 * Now that we have just located the synch point, we can simulate the whole cycle
	 * in order to calculate expected duty cycle
	 *
	 * todo: add a comment why are we doing '2 * shape->getSize()' here?
	 */
	state.cycleCallback = onFindIndex;
	for (uint32_t i = index + 1; i <= index + 2 * shape->getSize(); i++) {
		helper.nextStep(&state, shape, i, triggerConfig);
	}
	efiAssert(state.getTotalRevolutionCounter() == 3, "totalRevolutionCounter2", EFI_ERROR_CODE);

	for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
		shape->dutyCycle[i] = 1.0 * state.expectedTotalTime[i] / HELPER_PERIOD;
	}

	return index % shape->getSize();
}

#if (EFI_PROD_CODE || EFI_SIMULATOR) || defined(__DOXYGEN__)
//static Logging logger;
#endif

void initTriggerDecoder(void) {
#if (EFI_PROD_CODE || EFI_SIMULATOR)
	initLogging(&logger, "trigger decoder");
#endif
}

