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

	uint32_t findTriggerSyncPoint(TriggerWaveform * shape,
			TriggerState *state DECLARE_CONFIG_PARAMETER_SUFFIX);

	void assertSyncPositionAndSetDutyCycle(const TriggerStateCallback triggerCycleCallback,
			const uint32_t index, TriggerState *state, TriggerWaveform * shape
			DECLARE_CONFIG_PARAMETER_SUFFIX);

private:
	// send next event so that we can see how state reacts
	void feedSimulatedEvent(const TriggerStateCallback triggerCycleCallback, TriggerState *state,
			TriggerWaveform * shape, int i DECLARE_CONFIG_PARAMETER_SUFFIX);
};

bool isUsefulSignal(trigger_event_e signal DECLARE_CONFIG_PARAMETER_SUFFIX);
