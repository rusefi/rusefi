/**
 * @file trigger_emulator_algo.h
 *
 * @date Mar 3, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef TRIGGER_EMULATOR_ALGO_H_
#define TRIGGER_EMULATOR_ALGO_H_

#include "engine.h"
#include "pwm_generator_logic.h"

class TriggerEmulatorHelper {
public:
	bool primaryWheelState;
	bool secondaryWheelState;
	bool thirdWheelState;

    TriggerEmulatorHelper();
	void handleEmulatorCallback(PwmConfig *state, int stateIndex);
};

void initTriggerEmulatorLogic(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX);

#endif /* TRIGGER_EMULATOR_ALGO_H_ */
