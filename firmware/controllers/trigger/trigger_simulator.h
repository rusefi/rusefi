/*
 * @file trigger_simulator.h
 * @brief This class knows how to produce synthetic shaft signals based on triggerShape
 *
 * @date Sep 23, 2015
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef CONTROLLERS_TRIGGER_TRIGGER_SIMULATOR_H_
#define CONTROLLERS_TRIGGER_TRIGGER_SIMULATOR_H_

#include "trigger_decoder.h"

class TriggerStimulatorHelper {
public:
	TriggerStimulatorHelper();

	uint32_t findTriggerSyncPoint(TriggerShape * shape,
			TriggerState *state DECLARE_ENGINE_PARAMETER_SUFFIX);

	void assertSyncPositionAndSetDutyCycle(const uint32_t index, TriggerState *state, TriggerShape * shape
			DECLARE_ENGINE_PARAMETER_SUFFIX);

private:
	// send next event so that we can see how state reacts
	void feedSimulatedEvent(TriggerState *state, TriggerShape * shape, int i DECLARE_ENGINE_PARAMETER_SUFFIX);
};

bool isUsefulSignal(trigger_event_e signal, engine_configuration_s *engineConfiguration);

#endif /* CONTROLLERS_TRIGGER_TRIGGER_SIMULATOR_H_ */
