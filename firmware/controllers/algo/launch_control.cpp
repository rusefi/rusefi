/*
 * @file launch_control.cpp
 *
 *  @date 10. sep. 2019
 *      Author: Ola Ruud
 */

#include "pch.h"

#if EFI_LAUNCH_CONTROL
#include "boost_control.h"
#include "launch_control.h"
#include "periodic_task.h"
#include "advance_map.h"
#include "engine_state.h"
#include "advance_map.h"
#include "tinymt32.h"

/**
 * We can have active condition from switch or from clutch.
 * In case we are dependent on VSS we just return true.
 */
bool LaunchControlBase::isInsideSwitchCondition() {
	isSwitchActivated = engineConfiguration->launchActivationMode == SWITCH_INPUT_LAUNCH;
	isClutchActivated = engineConfiguration->launchActivationMode == CLUTCH_INPUT_LAUNCH;


	if (isSwitchActivated) {
#if !EFI_SIMULATOR
		if (isBrainPinValid(engineConfiguration->launchActivatePin)) {
			launchActivatePinState = engineConfiguration->launchActivateInverted ^ efiReadPin(engineConfiguration->launchActivatePin);
		}
#endif // EFI_PROD_CODE
		return launchActivatePinState;
	} else if (isClutchActivated) {
		if (isBrainPinValid(engineConfiguration->clutchDownPin)) {
			return engine->engineState.clutchDownState;
		} else {
			return false;
		}
	} else {
		// ALWAYS_ACTIVE_LAUNCH
		return true;
	}
}

/**
 * Returns True in case Vehicle speed is less then threshold.
 * This condition would only return true based on speed if DisablebySpeed is true
 * The condition logic is written in that way, that if we do not use disable by speed
 * then we have to return true, and trust that we would disable by other condition!
 */ 
bool LaunchControlBase::isInsideSpeedCondition() const {
	int speed = Sensor::getOrZero(SensorType::VehicleSpeed);
	
	return (engineConfiguration->launchSpeedThreshold > speed) || (!(engineConfiguration->launchActivationMode ==  ALWAYS_ACTIVE_LAUNCH));
}

/**
 * Returns false if TPS is invalid or TPS > preset threshold
 */
bool LaunchControlBase::isInsideTpsCondition() const {
	auto tps = Sensor::get(SensorType::DriverThrottleIntent);

	// Disallow launch without valid TPS
	if (!tps.Valid) {
		return false;
	}

    // todo: should this be 'launchTpsThreshold <= tps.Value' so that nicely calibrated TPS of zero does not prevent launch?
	return engineConfiguration->launchTpsThreshold < tps.Value;
}

/**
 * Condition is true as soon as we are above LaunchRpm
 */
bool LaunchControlBase::isInsideRPMCondition(int rpm) const {
	int launchRpm = engineConfiguration->launchRpm;
	return (launchRpm < rpm);
}

bool LaunchControlBase::isLaunchConditionMet(int rpm) {

	activateSwitchCondition = isInsideSwitchCondition();
	rpmCondition = isInsideRPMCondition(rpm);
	speedCondition = isInsideSpeedCondition();
	tpsCondition = isInsideTpsCondition();

	return speedCondition && activateSwitchCondition && rpmCondition && tpsCondition;
}

LaunchControlBase::LaunchControlBase() {
	launchActivatePinState = false;
	isLaunchCondition = false;
}

void LaunchControlBase::update() {
	if (!engineConfiguration->launchControlEnabled) {
		return;
	}

	int rpm = Sensor::getOrZero(SensorType::Rpm);
	combinedConditions = isLaunchConditionMet(rpm);

	//and still recalculate in case user changed the values
	retardThresholdRpm = engineConfiguration->launchRpm
	/*
	we never had UI for 'launchAdvanceRpmRange' so it was always zero. are we supposed to forget about this dead line
	or it is supposed to be referencing 'launchTimingRpmRange'?
	         + (engineConfiguration->enableLaunchRetard ? engineConfiguration->launchAdvanceRpmRange : 0)
*/
	         + engineConfiguration->hardCutRpmRange;

	if (!combinedConditions) {
		// conditions not met, reset timer
		m_launchTimer.reset();
		isLaunchCondition = false;
	} else {
		// If conditions are met...
		isLaunchCondition = m_launchTimer.hasElapsedSec(engineConfiguration->launchActivateDelay);
	}
}

bool LaunchControlBase::isLaunchRpmRetardCondition() const {
	return isLaunchCondition && (retardThresholdRpm < Sensor::getOrZero(SensorType::Rpm));
}

bool LaunchControlBase::isLaunchSparkRpmRetardCondition() const {
	return isLaunchRpmRetardCondition() && engineConfiguration->launchSparkCutEnable;
}

bool LaunchControlBase::isLaunchFuelRpmRetardCondition() const {
	return isLaunchRpmRetardCondition() && engineConfiguration->launchFuelCutEnable;
}

void SoftSparkLimiter::setTargetSkipRatio(float targetSkipRatio) {
	this->targetSkipRatio = targetSkipRatio;
}

static tinymt32_t tinymt;

bool SoftSparkLimiter::shouldSkip()  {
	if (targetSkipRatio == 0 || wasJustSkipped) {
		wasJustSkipped = false;
		return false;
	}

	float r = tinymt32_generate_float(&tinymt);
	wasJustSkipped = r < 2 * targetSkipRatio;
	return wasJustSkipped;
}

void initLaunchControl() {
    tinymt32_init(&tinymt, 1345135);
}

#endif /* EFI_LAUNCH_CONTROL */
