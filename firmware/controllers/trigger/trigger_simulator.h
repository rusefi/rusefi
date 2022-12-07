/*
 * @file trigger_simulator.h
 * @brief This class knows how to produce synthetic shaft signals based on triggerShape
 *
 * @date Sep 23, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#define SIMULATION_CYCLE_PERIOD 720000

#include "trigger_decoder.h"

int getSimulatedEventTime(const TriggerWaveform& shape, int i);

class TriggerStimulatorHelper {
public:

	static expected<uint32_t> findTriggerSyncPoint(
			TriggerWaveform& shape,
			const TriggerConfiguration& triggerConfiguration,
			TriggerDecoderBase& state);

	static void assertSyncPosition(
			const TriggerConfiguration& triggerConfiguration,
			const uint32_t index,
			TriggerDecoderBase& state,
			TriggerWaveform& shape
			);
private:
	// send next event so that we can see how state reacts
	static void feedSimulatedEvent(
			const TriggerConfiguration& triggerConfiguration,
			TriggerDecoderBase& state,
			const TriggerWaveform& shape,
			int i);
};

bool isUsefulSignal(trigger_event_e signal, const TriggerWaveform& shape);
