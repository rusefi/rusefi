/*
 * @file launch_control.h
 *
 * @date Mar 23, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_ptr.h"
#include "timer.h"

void initLaunchControl(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void setDefaultLaunchParameters(DECLARE_CONFIG_PARAMETER_SIGNATURE);

void updateLaunchConditions(DECLARE_ENGINE_PARAMETER_SIGNATURE);

class LaunchControlBase : public EnginePtr {
public:
	// Update the state of the launch control system
	void update();

	bool isInsideSpeedCondition() const;
	bool isInsideTpsCondition() const;
	bool isInsideSwitchCondition() const;
	bool isInsideRPMCondition(int rpm) const;
	bool isLaunchConditionMet(int rpm) const;
	int retardThresholdRpm;
	void applyLaunchControlLimiting(bool *limitedSpark, bool *limitedFuel DECLARE_ENGINE_PARAMETER_SUFFIX);

private:
	Timer m_launchTimer;
};
