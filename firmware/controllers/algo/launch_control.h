/*
 * @file launch_control.h
 *
 * @date Mar 23, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "launch_control_state_generated.h"

void initLaunchControl();

enum class LaunchCondition {
	PreLaunch,
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
	LaunchCondition calculateRPMLaunchCondition(float rpm);
	LaunchCondition calculateLaunchCondition(float rpm);

	bool isLaunchSparkRpmRetardCondition() const;
	bool isLaunchFuelRpmRetardCondition() const;

	float getSparkSkipRatio() const { return sparkSkipRatio; }

private:
	bool isLaunchRpmRetardCondition() const;

	float calculateSparkSkipRatio(int rpm) const;


	float sparkSkipRatio = 0.0f;
};

/**
 * See also SoftLimiterSandbox.java
 */
class SoftSparkLimiter {
public:
    SoftSparkLimiter(bool p_allowHardCut);
	/**
	 * targetSkipRatio of '0' means 'do not skip', would always return false
	 */
	void updateTargetSkipRatio(
		float luaSoftSparkSkip,
		float tractionControlSparkSkip,
		float launchControllerSparkSkipRatio = 0.0f
	);
	[[nodiscard]] float getTargetSkipRatio() const { return targetSkipRatio; }

	bool shouldSkip();
private:
    const bool allowHardCut;
	bool wasJustSkipped = false;
	float targetSkipRatio = 0;
};
