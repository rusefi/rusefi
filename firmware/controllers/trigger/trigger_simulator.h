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

	uint32_t doFindTrigger(TriggerShape * shape,
			trigger_config_s const*triggerConfig, TriggerState *state DECLARE_ENGINE_PARAMETER_S);

	void nextStep(TriggerState *state, TriggerShape * shape, int i, trigger_config_s const*triggerConfig DECLARE_ENGINE_PARAMETER_S);

	void assertSyncPositionAndSetDutyCycle(uint32_t index, TriggerState *state, TriggerShape * shape,
			trigger_config_s const*triggerConfig DECLARE_ENGINE_PARAMETER_S);
};


#endif /* CONTROLLERS_TRIGGER_TRIGGER_SIMULATOR_H_ */
