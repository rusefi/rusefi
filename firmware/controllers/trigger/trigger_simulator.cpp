/*
 * @file trigger_simulator.cpp
 *
 * @date Sep 23, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "global.h"
#include "engine.h"
#include "trigger_simulator.h"
#include "trigger_emulator_algo.h"

#define SIMULATION_CYCLE_PERIOD 720000

EXTERN_ENGINE;

// this is not the only place where we have 'isUpEvent'. todo: reuse
static const bool isRisingEdge[HW_EVENT_TYPES] = { false, true, false, true, false, true };

// todo: should this method be invoked somewhere deeper? at the moment we have too many usages too high
bool isUsefulSignal(trigger_event_e signal DECLARE_CONFIG_PARAMETER_SUFFIX) {
	return !engineConfiguration->useOnlyRisingEdgeForTrigger || isRisingEdge[(int) signal];
}

#if EFI_UNIT_TEST
extern bool printTriggerDebug;
#endif /* ! EFI_UNIT_TEST */

void TriggerStimulatorHelper::feedSimulatedEvent(const TriggerStateCallback triggerCycleCallback,
		TriggerState *state, TriggerWaveform * shape, int i
		DECLARE_CONFIG_PARAMETER_SUFFIX) {
	efiAssertVoid(CUSTOM_ERR_6593, shape->getSize() > 0, "size not zero");
	int stateIndex = i % shape->getSize();
	int size = shape->getSize();

	int loopIndex = i / shape->getSize();

	int time = (int) (SIMULATION_CYCLE_PERIOD * (loopIndex + shape->wave.getSwitchTime(stateIndex)));

	MultiChannelStateSequence *multiChannelStateSequence = &shape->wave;

#if EFI_UNIT_TEST
	int prevIndex = getPreviousIndex(stateIndex, shape->getSize());

	pin_state_t primaryWheelState = multiChannelStateSequence->getChannelState(0, prevIndex);
	pin_state_t newPrimaryWheelState = multiChannelStateSequence->getChannelState(0, stateIndex);

	pin_state_t secondaryWheelState = multiChannelStateSequence->getChannelState(1, prevIndex);
	pin_state_t newSecondaryWheelState = multiChannelStateSequence->getChannelState(1, stateIndex);

//	pin_state_t thirdWheelState = multiChannelStateSequence->getChannelState(2, prevIndex);
//	pin_state_t new3rdWheelState = multiChannelStateSequence->getChannelState(2, stateIndex);

	if (printTriggerDebug) {
		printf("feedSimulatedEvent: %d>%d primary %d>%d secondary %d>%d\r\n", prevIndex, stateIndex, primaryWheelState, newPrimaryWheelState,
				secondaryWheelState, newSecondaryWheelState );
	}
#endif /* EFI_UNIT_TEST */


	// todo: code duplication with TriggerEmulatorHelper::handleEmulatorCallback?

	if (needEvent(stateIndex, size, multiChannelStateSequence, 0)) {
		pin_state_t currentValue = multiChannelStateSequence->getChannelState(/*phaseIndex*/0, stateIndex);
		trigger_event_e s = currentValue ? SHAFT_PRIMARY_RISING : SHAFT_PRIMARY_FALLING;
		if (isUsefulSignal(s PASS_CONFIG_PARAMETER_SUFFIX)) {
			state->decodeTriggerEvent(shape,
					triggerCycleCallback,
					/* override */ nullptr,
					s, time PASS_CONFIG_PARAMETER_SUFFIX);
		}
	}

	if (needEvent(stateIndex, size, multiChannelStateSequence, 1)) {
		pin_state_t currentValue = multiChannelStateSequence->getChannelState(/*phaseIndex*/1, stateIndex);
		trigger_event_e s = currentValue ? SHAFT_SECONDARY_RISING : SHAFT_SECONDARY_FALLING;
		if (isUsefulSignal(s PASS_CONFIG_PARAMETER_SUFFIX)) {
			state->decodeTriggerEvent(shape,
					triggerCycleCallback,
					/* override */ nullptr,
					s, time PASS_CONFIG_PARAMETER_SUFFIX);
		}
	}

	if (needEvent(stateIndex, size, multiChannelStateSequence, 2)) {
		pin_state_t currentValue = multiChannelStateSequence->getChannelState(/*phaseIndex*/2, stateIndex);
		trigger_event_e s = currentValue ? SHAFT_3RD_RISING : SHAFT_3RD_FALLING;
		if (isUsefulSignal(s PASS_CONFIG_PARAMETER_SUFFIX)) {
			state->decodeTriggerEvent(shape,
					triggerCycleCallback,
					/* override */ nullptr,
					s, time PASS_CONFIG_PARAMETER_SUFFIX);
		}
	}
}

void TriggerStimulatorHelper::assertSyncPositionAndSetDutyCycle(const TriggerStateCallback triggerCycleCallback,
		const uint32_t syncIndex, TriggerState *state, TriggerWaveform * shape
		DECLARE_CONFIG_PARAMETER_SUFFIX) {

	/**
	 * let's feed two more cycles to validate shape definition
	 */
	for (uint32_t i = syncIndex + 1; i <= syncIndex + GAP_TRACKING_LENGTH * shape->getSize(); i++) {
		feedSimulatedEvent(triggerCycleCallback, state, shape, i PASS_CONFIG_PARAMETER_SUFFIX);
	}
	int revolutionCounter = state->getTotalRevolutionCounter();
	if (revolutionCounter != GAP_TRACKING_LENGTH + 1) {
		warning(CUSTOM_OBD_TRIGGER_WAVEFORM, "sync failed/wrong gap parameters trigger=%s rc=%d", getTrigger_type_e(engineConfiguration->trigger.type), revolutionCounter);
		shape->setShapeDefinitionError(true);
		return;
	}
	shape->shapeDefinitionError = false;

	for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
		shape->expectedDutyCycle[i] = 1.0 * state->expectedTotalTime[i] / SIMULATION_CYCLE_PERIOD;
	}
}

/**
 * @return trigger synchronization point index, or error code if not found
 */
uint32_t TriggerStimulatorHelper::findTriggerSyncPoint(TriggerWaveform * shape,
		 TriggerState *state DECLARE_CONFIG_PARAMETER_SUFFIX) {
	for (int i = 0; i < 4 * PWM_PHASE_MAX_COUNT; i++) {
		feedSimulatedEvent(nullptr, state, shape, i PASS_CONFIG_PARAMETER_SUFFIX);

		if (state->shaft_is_synchronized) {
			return i;
		}
	}
	shape->setShapeDefinitionError(true);
	warning(CUSTOM_ERR_TRIGGER_SYNC, "findTriggerZeroEventIndex() failed");
	return EFI_ERROR_CODE;
}
