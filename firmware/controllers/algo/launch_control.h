/*
 * @file launch_control.h
 *
 * @date Mar 23, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_ptr.h"
#include "timer.h"

void initLaunchControl();

class LaunchControlBase : public EnginePtr {
public:
	// Update the state of the launch control system
	void update();

	bool isInsideSpeedCondition() const;
	bool isInsideTpsCondition() const;
	bool isInsideSwitchCondition();
	bool isInsideRPMCondition(int rpm) const;
	bool isLaunchConditionMet(int rpm);

	bool isLaunchSparkRpmRetardCondition() const;
	bool isLaunchFuelRpmRetardCondition() const;

	int retardThresholdRpm;
	bool launchActivatePinState = false;
	bool isLaunchCondition = false;

private:
	bool isLaunchRpmRetardCondition() const;

	Timer m_launchTimer;
};
