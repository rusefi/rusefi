/*
 * @file antilag_system.h
 *
 * @date 26. nov. 2022
 *      Author: Turbo Marian
 */

#pragma once

#include "timer.h"
#include "antilag_system_state_generated.h"

void initAntilagSystem();

class AntilagSystemBase : public antilag_system_state_s {
public:
	void update();

    bool isALSMinRPMCondition(int rpm) const;
	bool isALSMaxRPMCondition(int rpm) const;
	bool isALSMinCLTCondition() const;
	bool isALSMaxCLTCondition() const;
	bool isALSMaxThrottleIntentCondition() const;
	bool isInsideALSSwitchCondition();
    /* enabled and all conditions above */
	bool isAntilagConditionMet(int rpm);
};
