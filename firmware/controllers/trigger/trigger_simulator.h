/*
 * @file trigger_simulator.h
 * @brief This class knows how to produce synthetic shaft signals based on triggerShape
 *
 * @date Sep 23, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "trigger_decoder.h"

class TriggerStimulatorHelper {
public:

	uint32_t findTriggerSyncPoint(
			TriggerWaveform& shape,
			const TriggerConfiguration& triggerConfiguration,
			TriggerState& state);

	void assertSyncPositionAndSetDutyCycle(
			const TriggerStateCallback triggerCycleCallback,
			const TriggerConfiguration& triggerConfiguration,
			const uint32_t index,
			TriggerState& state,
			TriggerWaveform& shape
			);

private:
	// send next event so that we can see how state reacts
	void feedSimulatedEvent(const TriggerStateCallback triggerCycleCallback,
			const TriggerConfiguration& triggerConfiguration,
			TriggerState& state,
			const TriggerWaveform& shape,
			int i);
};

bool isUsefulSignal(trigger_event_e signal, const TriggerConfiguration& triggerConfiguration);
