/*
 * @file launch_control.h
 *
 * @date Mar 23, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <rusefi/timer.h>
#include "launch_control_state_generated.h"

void initLaunchControl();

enum class LaunchCondition {
	Launch,
	NotMet
};

class LaunchControlBase : public launch_control_state_s {
public:
	LaunchControlBase();
	// Update the state of the launch control system
	void update();

  float getFuelCoefficient() const;
	bool isInsideSpeedCondition() const;
	bool isInsideTpsCondition() const;
	bool isInsideSwitchCondition();
	LaunchCondition isInsideRPMCondition(int rpm) const;
	LaunchCondition isLaunchConditionMet(int rpm);

	bool isLaunchSparkRpmRetardCondition() const;
	bool isLaunchFuelRpmRetardCondition() const;

private:
	bool isLaunchRpmRetardCondition() const;

	Timer m_launchTimer;
};

/**
 * See also SoftLimiterSandbox.java
 */
class SoftSparkLimiter {
public:
    SoftSparkLimiter(bool allowHardCut);
	/**
	 * targetSkipRatio of '0' means 'do not skip', would always return false
	 */
	void setTargetSkipRatio(float targetSkipRatio);

	bool shouldSkip();
private:
    bool allowHardCut;
	bool wasJustSkipped = false;
	float targetSkipRatio = 0;
};
