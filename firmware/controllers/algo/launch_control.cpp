/*
 * @file launch_control.cpp
 *
 *  @date 10. sep. 2019
 *      Author: Ola Ruud
 *      Rework: Patryk Chmura
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
 * Returns True when Vehicle speed ALLOWS launch control
 */
bool LaunchControlBase::isInsideSpeedCondition() const {
	if (engineConfiguration->launchSpeedThreshold == 0) {
		return true; // allow launch, speed does not matter
	}

	int speed = Sensor::getOrZero(SensorType::VehicleSpeed);

	return engineConfiguration->launchSpeedThreshold > speed;
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

LaunchCondition LaunchControlBase::calculateRPMLaunchCondition(const int rpm) {
	const int launchRpm = engineConfiguration->launchRpm;
	const int preLaunchRpm = launchRpm - engineConfiguration->launchRpmWindow;
	if (rpm < preLaunchRpm) {
		return LaunchCondition::NotMet;
	} else if (launchRpm <= rpm) {
		return LaunchCondition::Launch;
	} else {
		return LaunchCondition::PreLaunch;
	}
}

bool LaunchControlBase::isInsideRPMCondition(const int rpm) {
	const LaunchCondition rpmLaunchCondition = calculateRPMLaunchCondition(rpm);
	bool result = false;
	switch (rpmLaunchCondition) {
		case LaunchCondition::NotMet: {
			isAfterLaunch = false;
			break;
		}
		case LaunchCondition::PreLaunch: {
			result = isAfterLaunch;
			break;
		}
		case LaunchCondition::Launch: {
			isAfterLaunch = true;
			result = true;
			break;
		}
	}
	return result;
}

bool LaunchControlBase::isLaunchConditionMet(const int rpm) {
	activateSwitchCondition = isInsideSwitchCondition();
	rpmCondition = isInsideRPMCondition(rpm);
	speedCondition = isInsideSpeedCondition();
	tpsCondition = isInsideTpsCondition();

	return speedCondition && activateSwitchCondition && rpmCondition && tpsCondition;
}

LaunchControlBase::LaunchControlBase() {
	launchActivatePinState = false;
	isLaunchPreCondition = false;
	isLaunchCondition = false;
	isSmoothRetardCondition = false;
}

float LaunchControlBase::getFuelCoefficient() const {
    return 1 + (isLaunchCondition && engineConfiguration->launchControlEnabled ? engineConfiguration->launchFuelAdderPercent / 100.0 : 0);
}

void LaunchControlBase::update() {
	if (!engineConfiguration->launchControlEnabled) {
		return;
	}

	const int rpm = Sensor::getOrZero(SensorType::Rpm);
	combinedConditions = isLaunchConditionMet(rpm);

	//and still recalculate in case user changed the values
	retardThresholdRpm = engineConfiguration->launchRpm
	/*
	we never had UI for 'launchAdvanceRpmRange' so it was always zero. are we supposed to forget about this dead line
	or it is supposed to be referencing 'launchTimingRpmRange'?
	         + (engineConfiguration->enableLaunchRetard ? engineConfiguration->launchAdvanceRpmRange : 0)
	*/;

	if (!combinedConditions) {
		// conditions not met, reset timer
		m_launchTimer.reset();
		isLaunchCondition = false;
	} else {
		// If conditions are met...
		isLaunchCondition = m_launchTimer.hasElapsedSec(engineConfiguration->launchActivateDelay);
	}

	isSmoothRetardCondition = isSmoothRetardRpmCondition(rpm);
}

bool LaunchControlBase::isLaunchRpmRetardCondition() const {
	return isLaunchCondition && (retardThresholdRpm < Sensor::getOrZero(SensorType::Rpm));
}

bool LaunchControlBase::isLaunchSparkRpmRetardCondition() const {
	return isLaunchRpmRetardCondition() && engineConfiguration->launchSparkCutEnable;
}

bool LaunchControlBase::isSmoothRetardRpmCondition(const int rpm) const {
	const int smoothRetardStartRpm = engineConfiguration->launchRpm - engineConfiguration->launchRpmWindow;
	const int smoothRetardEndRpm = engineConfiguration->launchRpm - engineConfiguration->smoothRetardEndRpm;
	return (smoothRetardStartRpm <= rpm) && (rpm <= smoothRetardEndRpm);
}

bool LaunchControlBase::isLaunchFuelRpmRetardCondition() const {
	return isLaunchRpmRetardCondition() && engineConfiguration->launchFuelCutEnable;
}

SoftSparkLimiter::SoftSparkLimiter(bool p_allowHardCut) {
    this->allowHardCut = p_allowHardCut;
#if EFI_UNIT_TEST
    initLaunchControl();
#endif // EFI_UNIT_TEST
}

void SoftSparkLimiter::setTargetSkipRatio(float p_targetSkipRatio) {
	this->targetSkipRatio = p_targetSkipRatio;
}

static tinymt32_t tinymt;

bool SoftSparkLimiter::shouldSkip()  {
	if (targetSkipRatio == 0 || (!allowHardCut && wasJustSkipped)) {
		wasJustSkipped = false;
		return false;
	}

	float r = tinymt32_generate_float(&tinymt);
	wasJustSkipped = r < (allowHardCut ? 1 : 2) * targetSkipRatio;
	return wasJustSkipped;
}

void initLaunchControl() {
    tinymt32_init(&tinymt, 1345135);
}

#endif /* EFI_LAUNCH_CONTROL */
