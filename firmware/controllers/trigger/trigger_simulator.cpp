/*
 * @file trigger_simulator.cpp
 *
 * @date Sep 23, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

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
bool isUsefulSignal(trigger_event_e signal, const TriggerWaveform& shape) {
	return !shape.useOnlyRisingEdges || isRisingEdge[(int) signal];
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
		const TriggerConfiguration& triggerConfiguration,
		TriggerDecoderBase& state,
		const TriggerWaveform& shape,
		int i
		) {
	efiAssertVoid(ObdCode::CUSTOM_ERR_6593, shape.getSize() > 0, "size not zero");
	int stateIndex = i % shape.getSize();

	int time = getSimulatedEventTime(shape, i);

	const auto & multiChannelStateSequence = shape.wave;

#if EFI_UNIT_TEST
	int prevIndex = getPreviousIndex(stateIndex, shape.getSize());

	bool primaryWheelState = multiChannelStateSequence.getChannelState(0, prevIndex);
	bool newPrimaryWheelState = multiChannelStateSequence.getChannelState(0, stateIndex);

	bool secondaryWheelState = multiChannelStateSequence.getChannelState(1, prevIndex);
	bool newSecondaryWheelState = multiChannelStateSequence.getChannelState(1, stateIndex);

	if (printTriggerDebug) {
		printf("TriggerStimulator: simulatedEvent: %d>%d primary %d>%d secondary %d>%d\r\n", prevIndex, stateIndex, primaryWheelState, newPrimaryWheelState,
				secondaryWheelState, newSecondaryWheelState );
	}
#endif /* EFI_UNIT_TEST */


	// todo: code duplication with TriggerEmulatorHelper::handleEmulatorCallback?

	constexpr trigger_event_e riseEvents[] = { SHAFT_PRIMARY_RISING, SHAFT_SECONDARY_RISING };
	constexpr trigger_event_e fallEvents[] = { SHAFT_PRIMARY_FALLING, SHAFT_SECONDARY_FALLING };

	for (size_t i = 0; i < PWM_PHASE_MAX_WAVE_PER_PWM; i++) {
		if (needEvent(stateIndex, multiChannelStateSequence, i)) {
			bool currentValue = multiChannelStateSequence.getChannelState(/*phaseIndex*/i, stateIndex);
			trigger_event_e event = (currentValue ? riseEvents : fallEvents)[i];
			if (isUsefulSignal(event, shape)) {
				state.decodeTriggerEvent(
					"sim",
						shape,
					/* override */ nullptr,
					triggerConfiguration,
					event, time);
			}
		}
	}
}

void TriggerStimulatorHelper::assertSyncPosition(
		const TriggerConfiguration& triggerConfiguration,
		const uint32_t syncIndex,
		TriggerDecoderBase& state,
		TriggerWaveform& shape
		) {

// todo: is anything limiting this TEST_REVOLUTIONS? why does value '8' not work for example?
#define TEST_REVOLUTIONS 6

	/**
	 * let's feed two more cycles to validate shape definition
	 */
	for (uint32_t i = syncIndex + 1; i <= syncIndex + TEST_REVOLUTIONS * shape.getSize(); i++) {
		feedSimulatedEvent(triggerConfiguration, state, shape, i);
	}

	int revolutionCounter = state.getCrankSynchronizationCounter();
	if (revolutionCounter != TEST_REVOLUTIONS) {
		warning(ObdCode::CUSTOM_OBD_TRIGGER_WAVEFORM, "sync failed/wrong gap parameters trigger=%s revolutionCounter=%d",
				getTrigger_type_e(triggerConfiguration.TriggerType.type),
				revolutionCounter);
		shape.setShapeDefinitionError(true);
		return;
	}
	shape.shapeDefinitionError = false;
#if EFI_UNIT_TEST
		if (printTriggerTrace) {
			printf("Happy %s revolutionCounter=%d\r\n",
					getTrigger_type_e(triggerConfiguration.TriggerType.type),
					revolutionCounter);
		}
#endif /* EFI_UNIT_TEST */
}

/**
 * @return trigger synchronization point index, or error code if not found
 */
expected<uint32_t> TriggerStimulatorHelper::findTriggerSyncPoint(
		TriggerWaveform& shape,
		const TriggerConfiguration& triggerConfiguration,
		TriggerDecoderBase& state) {
	for (int i = 0; i < 4 * PWM_PHASE_MAX_COUNT; i++) {
		feedSimulatedEvent(triggerConfiguration, state, shape, i);

		if (state.getShaftSynchronized()) {
			return i;
		}
	}
	shape.setShapeDefinitionError(true);

    if (engineConfiguration->overrideTriggerGaps) {
	    firmwareError(ObdCode::CUSTOM_ERR_CUSTOM_GAPS_BAD, "Your custom trigger gaps are not good.");
	} else {
	    firmwareError(ObdCode::CUSTOM_ERR_TRIGGER_SYNC, "findTriggerZeroEventIndex() failed");
	}
	return unexpected;
}
