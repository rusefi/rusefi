/*
 * @file trigger_simulator.cpp
 *
 * @date Sep 23, 2015
 * @author Andrey Belomutskiy, (c) 2012-2017
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

bool isUsefulSignal(trigger_event_e signal, engine_configuration_s *engineConfiguration) {
	return !engineConfiguration->useOnlyRisingEdgeForTrigger || isRisingEdge[(int) signal];
}

void TriggerStimulatorHelper::nextStep(TriggerState *state, TriggerShape * shape, int i,
		trigger_config_s const*triggerConfig DECLARE_ENGINE_PARAMETER_S) {
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

	// todo: code duplication with TriggerEmulatorHelper::handleEmulatorCallback?

	if (primaryWheelState != newPrimaryWheelState) {
		primaryWheelState = newPrimaryWheelState;
		trigger_event_e s = primaryWheelState ? SHAFT_PRIMARY_RISING : SHAFT_PRIMARY_FALLING;
		state->decodeTriggerEvent(s, time PASS_ENGINE_PARAMETER);
	}

	if (secondaryWheelState != newSecondaryWheelState) {
		secondaryWheelState = newSecondaryWheelState;
		trigger_event_e s = secondaryWheelState ? SHAFT_SECONDARY_RISING : SHAFT_SECONDARY_FALLING;
		state->decodeTriggerEvent(s, time PASS_ENGINE_PARAMETER);
	}

	if (thirdWheelState != new3rdWheelState) {
		thirdWheelState = new3rdWheelState;
		trigger_event_e s = thirdWheelState ? SHAFT_3RD_RISING : SHAFT_3RD_FALLING;
		state->decodeTriggerEvent(s, time PASS_ENGINE_PARAMETER);
	}
}

void TriggerStimulatorHelper::assertSyncPositionAndSetDutyCycle(uint32_t index, TriggerState *state, TriggerShape * shape,
		trigger_config_s const*triggerConfig DECLARE_ENGINE_PARAMETER_S) {
	int startIndex = engineConfiguration->useOnlyRisingEdgeForTrigger ? index + 2 : index + 1;

	for (uint32_t i = startIndex; i <= index + 2 * shape->getSize(); i++) {
		nextStep(state, shape, i, triggerConfig PASS_ENGINE_PARAMETER);
	}
	int revolutionCounter = state->getTotalRevolutionCounter();
	if (revolutionCounter != 3) {
		warning(CUSTOM_OBD_TRIGGER_SHAPE, "sync failed/wrong gap parameters trigger=%s rc=%d", getTrigger_type_e(engineConfiguration->trigger.type), revolutionCounter);
		shape->shapeDefinitionError = true;
		return;
	}
	shape->shapeDefinitionError = false;

	for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
		shape->dutyCycle[i] = 1.0 * state->expectedTotalTime[i] / SIMULATION_CYCLE_PERIOD;
	}
}

uint32_t TriggerStimulatorHelper::doFindTrigger(TriggerShape * shape,
		trigger_config_s const*triggerConfig, TriggerState *state DECLARE_ENGINE_PARAMETER_S) {
	for (int i = 0; i < 4 * PWM_PHASE_MAX_COUNT; i++) {
		nextStep(state, shape, i, triggerConfig PASS_ENGINE_PARAMETER);

		if (state->shaft_is_synchronized)
			return i;
	}
	firmwareError(CUSTOM_ERR_TRIGGER_SYNC, "findTriggerZeroEventIndex() failed");
	return EFI_ERROR_CODE;
}
