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

#if EFI_UNIT_TEST
	extern bool printTriggerTrace;
#endif

// this is not the only place where we have 'isUpEvent'. todo: reuse
static const bool isRisingEdge[HW_EVENT_TYPES] = { false, true, false, true, false, true };

/**
 * todo: should this method be invoked somewhere deeper? at the moment we have too many usages too high
 * @return true if front should be decoded further, false if we are not interested
 */
bool isUsefulSignal(trigger_event_e signal, const TriggerConfiguration& triggerConfiguration) {
	return !triggerConfiguration.UseOnlyRisingEdgeForTrigger || isRisingEdge[(int) signal];
}

#if EFI_UNIT_TEST
extern bool printTriggerDebug;
#endif /* ! EFI_UNIT_TEST */

int getSimulatedEventTime(const TriggerWaveform& shape, int i) {
	int stateIndex = i % shape.getSize();
	int loopIndex = i / shape.getSize();

	return (int) (SIMULATION_CYCLE_PERIOD * (loopIndex + shape.wave.getSwitchTime(stateIndex)));
}

void TriggerStimulatorHelper::feedSimulatedEvent(
		const TriggerStateCallback triggerCycleCallback,
		const TriggerConfiguration& triggerConfiguration,
		TriggerState& state,
		const TriggerWaveform& shape,
		int i
		) {
	efiAssertVoid(CUSTOM_ERR_6593, shape.getSize() > 0, "size not zero");
	int stateIndex = i % shape.getSize();
	int size = shape.getSize();

	int time = getSimulatedEventTime(shape, i);

	const MultiChannelStateSequence& multiChannelStateSequence = shape.wave;

#if EFI_UNIT_TEST
	int prevIndex = getPreviousIndex(stateIndex, shape.getSize());

	pin_state_t primaryWheelState = multiChannelStateSequence.getChannelState(0, prevIndex);
	pin_state_t newPrimaryWheelState = multiChannelStateSequence.getChannelState(0, stateIndex);

	pin_state_t secondaryWheelState = multiChannelStateSequence.getChannelState(1, prevIndex);
	pin_state_t newSecondaryWheelState = multiChannelStateSequence.getChannelState(1, stateIndex);

//	pin_state_t thirdWheelState = multiChannelStateSequence->getChannelState(2, prevIndex);
//	pin_state_t new3rdWheelState = multiChannelStateSequence->getChannelState(2, stateIndex);

	if (printTriggerDebug) {
		printf("TriggerStimulator: simulatedEvent: %d>%d primary %d>%d secondary %d>%d\r\n", prevIndex, stateIndex, primaryWheelState, newPrimaryWheelState,
				secondaryWheelState, newSecondaryWheelState );
	}
#endif /* EFI_UNIT_TEST */


	// todo: code duplication with TriggerEmulatorHelper::handleEmulatorCallback?

	constexpr trigger_event_e riseEvents[] = { SHAFT_PRIMARY_RISING, SHAFT_SECONDARY_RISING, SHAFT_3RD_RISING };
	constexpr trigger_event_e fallEvents[] = { SHAFT_PRIMARY_FALLING, SHAFT_SECONDARY_FALLING, SHAFT_3RD_FALLING };


	for (size_t i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
		if (needEvent(stateIndex, size, multiChannelStateSequence, i)) {
			pin_state_t currentValue = multiChannelStateSequence.getChannelState(/*phaseIndex*/i, stateIndex);
			trigger_event_e event = (currentValue ? riseEvents : fallEvents)[i];
			if (isUsefulSignal(event, triggerConfiguration)) {
				state.decodeTriggerEvent(shape,
					triggerCycleCallback,
					/* override */ nullptr,
					triggerConfiguration,
					event, time);
			}
		}
	}

}

void TriggerStimulatorHelper::assertSyncPositionAndSetDutyCycle(
		const TriggerStateCallback triggerCycleCallback,
		const TriggerConfiguration& triggerConfiguration,
		const uint32_t syncIndex,
		TriggerState& state,
		TriggerWaveform& shape
		) {

// todo: is anything limiting this TEST_REVOLUTIONS? why does value '8' not work for example?
#define TEST_REVOLUTIONS 6

	/**
	 * let's feed two more cycles to validate shape definition
	 */
	for (uint32_t i = syncIndex + 1; i <= syncIndex + TEST_REVOLUTIONS * shape.getSize(); i++) {
		feedSimulatedEvent(triggerCycleCallback,
				triggerConfiguration,
				state, shape, i);
	}
	int revolutionCounter = state.getTotalRevolutionCounter();
	if (revolutionCounter != TEST_REVOLUTIONS + 1) {
		warning(CUSTOM_OBD_TRIGGER_WAVEFORM, "sync failed/wrong gap parameters trigger=%s revolutionCounter=%d",
				getTrigger_type_e(triggerConfiguration.TriggerType),
				revolutionCounter);
		shape.setShapeDefinitionError(true);
		return;
	}
	shape.shapeDefinitionError = false;
#if EFI_UNIT_TEST
		if (printTriggerTrace) {
			printf("Happy %s revolutionCounter=%d\r\n",
					getTrigger_type_e(triggerConfiguration.TriggerType),
					revolutionCounter);
		}
#endif /* EFI_UNIT_TEST */


	for (int i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
		shape.expectedDutyCycle[i] = 1.0 * state.expectedTotalTime[i] / SIMULATION_CYCLE_PERIOD;
	}
}

/**
 * @return trigger synchronization point index, or error code if not found
 */
uint32_t TriggerStimulatorHelper::findTriggerSyncPoint(
		TriggerWaveform& shape,
		const TriggerConfiguration& triggerConfiguration,
		TriggerState& state) {
	for (int i = 0; i < 4 * PWM_PHASE_MAX_COUNT; i++) {
		feedSimulatedEvent(nullptr,
				triggerConfiguration,
				state, shape, i);

		if (state.getShaftSynchronized()) {
			return i;
		}
	}
	shape.setShapeDefinitionError(true);
	warning(CUSTOM_ERR_TRIGGER_SYNC, "findTriggerZeroEventIndex() failed");
	return EFI_ERROR_CODE;
}
