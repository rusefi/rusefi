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

    bool isALSMinRPMCondition() const;
	bool isALSMaxRPMCondition() const;
	bool isALSMinCLTCondition() const;
	bool isALSMaxCLTCondition() const;
	bool isALSMaxTPSCondition() const;
	bool isAntilagConditionMet();
	bool isInsideALSSwitchCondition();
};
