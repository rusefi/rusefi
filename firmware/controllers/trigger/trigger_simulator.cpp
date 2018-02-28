/*
 * @file trigger_simulator.cpp
 *
 * @date Sep 23, 2015
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"
#include "engine.h"
#include "trigger_simulator.h"
#include "trigger_emulator_algo.h"

#define SIMULATION_CYCLE_PERIOD 720000

EXTERN_ENGINE;

TriggerStimulatorHelper::TriggerStimulatorHelper() {
}

// this is not the only place where we have 'isUpEvent'. todo: reuse
static bool isRisingEdge[6] = { false, true, false, true, false, true };

// todo: should this method be invoked somewhere deeper? at the moment we have too many usages too high
bool isUsefulSignal(trigger_event_e signal, engine_configuration_s *engineConfiguration) {
	return !engineConfiguration->useOnlyRisingEdgeForTrigger || isRisingEdge[(int) signal];
}

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
extern bool printTriggerDebug;
#endif /* ! EFI_UNIT_TEST */

void TriggerStimulatorHelper::feedSimulatedEvent(TriggerState *state, TriggerShape * shape, int i
		DECLARE_ENGINE_PARAMETER_SUFFIX) {
	efiAssertVoid(shape->getSize() > 0, "size not zero");
	int stateIndex = i % shape->getSize();
	int prevIndex = (stateIndex + shape->getSize() - 1 ) % shape->getSize();


	int loopIndex = i / shape->getSize();

	int time = (int) (SIMULATION_CYCLE_PERIOD * (loopIndex + shape->wave.getSwitchTime(stateIndex)));

	bool primaryWheelState = shape->wave.getChannelState(0, prevIndex);
	bool newPrimaryWheelState = shape->wave.getChannelState(0, stateIndex);

	bool secondaryWheelState = shape->wave.getChannelState(1, prevIndex);
	bool newSecondaryWheelState = shape->wave.getChannelState(1, stateIndex);

	bool thirdWheelState = shape->wave.getChannelState(2, prevIndex);
	bool new3rdWheelState = shape->wave.getChannelState(2, stateIndex);

#if EFI_UNIT_TEST || defined(__DOXYGEN__)
	if (printTriggerDebug) {
		printf("feedSimulatedEvent: %d>%d primary %d>%d secondary %d>%d\r\n", prevIndex, stateIndex, primaryWheelState, newPrimaryWheelState,
				secondaryWheelState, newSecondaryWheelState );
	}
#endif /* EFI_UNIT_TEST */


	// todo: code duplication with TriggerEmulatorHelper::handleEmulatorCallback?

	if (primaryWheelState != newPrimaryWheelState) {
		primaryWheelState = newPrimaryWheelState;
		trigger_event_e s = primaryWheelState ? SHAFT_PRIMARY_RISING : SHAFT_PRIMARY_FALLING;
		if (isUsefulSignal(s, engineConfiguration))
			state->decodeTriggerEvent(s, time PASS_ENGINE_PARAMETER_SUFFIX);
	}

	if (secondaryWheelState != newSecondaryWheelState) {
		secondaryWheelState = newSecondaryWheelState;
		trigger_event_e s = secondaryWheelState ? SHAFT_SECONDARY_RISING : SHAFT_SECONDARY_FALLING;
		if (isUsefulSignal(s, engineConfiguration))
			state->decodeTriggerEvent(s, time PASS_ENGINE_PARAMETER_SUFFIX);
	}

	if (thirdWheelState != new3rdWheelState) {
		thirdWheelState = new3rdWheelState;
		trigger_event_e s = thirdWheelState ? SHAFT_3RD_RISING : SHAFT_3RD_FALLING;
		if (isUsefulSignal(s, engineConfiguration))
			state->decodeTriggerEvent(s, time PASS_ENGINE_PARAMETER_SUFFIX);
	}
}

void TriggerStimulatorHelper::assertSyncPositionAndSetDutyCycle(const uint32_t syncIndex, TriggerState *state, TriggerShape * shape
		DECLARE_ENGINE_PARAMETER_SUFFIX) {

	/**
	 * let's feed two more cycles to validate shape definition
	 */
	for (uint32_t i = syncIndex + 1; i <= syncIndex + 2 * shape->getSize(); i++) {
		feedSimulatedEvent(state, shape, i PASS_ENGINE_PARAMETER_SUFFIX);
	}
	int revolutionCounter = state->getTotalRevolutionCounter();
	if (revolutionCounter != 3) {
		warning(CUSTOM_OBD_TRIGGER_SHAPE, "sync failed/wrong gap parameters trigger=%s rc=%d", getTrigger_type_e(engineConfiguration->trigger.type), revolutionCounter);
		shape->shapeDefinitionError = true;
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
uint32_t TriggerStimulatorHelper::findTriggerSyncPoint(TriggerShape * shape,
		 TriggerState *state DECLARE_ENGINE_PARAMETER_SUFFIX) {
	for (int i = 0; i < 4 * PWM_PHASE_MAX_COUNT; i++) {
		feedSimulatedEvent(state, shape, i PASS_ENGINE_PARAMETER_SUFFIX);

		if (state->shaft_is_synchronized)
			return i;
	}
	engine->triggerCentral.triggerShape.shapeDefinitionError = 1;
	warning(CUSTOM_ERR_TRIGGER_SYNC, "findTriggerZeroEventIndex() failed");
	return EFI_ERROR_CODE;
}
