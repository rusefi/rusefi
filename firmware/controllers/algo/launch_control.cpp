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

/**
 * Condition is true as soon as we are above LaunchRpm
 */
LaunchCondition LaunchControlBase::isInsideRPMCondition(int rpm) const {
	auto launchRpm = engineConfiguration->launchRpm;
	auto preLaunchRpm = launchRpm - engineConfiguration->launchRpmWindow;

	if (rpm >= launchRpm) {
		return LaunchCondition::Launch;
	} else if ((rpm >= preLaunchRpm) && (rpm < launchRpm)) {
		return LaunchCondition::PreLaunch;
	} else {
		return LaunchCondition::NotMet;
	}
}

LaunchCondition LaunchControlBase::isLaunchConditionMet(int rpm) {
	LaunchCondition rpmLaunchCondition = isInsideRPMCondition(rpm);
	activateSwitchCondition = isInsideSwitchCondition();
	rpmCondition = (rpmLaunchCondition == LaunchCondition::Launch);
	speedCondition = isInsideSpeedCondition();
	tpsCondition = isInsideTpsCondition();

	return ((speedCondition && activateSwitchCondition && tpsCondition) == true) ? rpmLaunchCondition : LaunchCondition::NotMet;
}

LaunchControlBase::LaunchControlBase() {
	launchActivatePinState = false;
	isLaunchPreCondition = false;
	isLaunchCondition = false;
}

float LaunchControlBase::getFuelCoefficient() const {
    return 1 + (isLaunchCondition && engineConfiguration->launchControlEnabled ? engineConfiguration->launchFuelAdderPercent / 100.0 : 0);
}

void LaunchControlBase::update() {
	if (!engineConfiguration->launchControlEnabled) {
		return;
	}

	int rpm = Sensor::getOrZero(SensorType::Rpm);
	combinedConditions = isLaunchConditionMet(rpm) == LaunchCondition::Launch;

	//and still recalculate in case user changed the values
	retardThresholdRpm = engineConfiguration->launchRpm - (engineConfiguration->enableLaunchRetard ? engineConfiguration->launchRpmWindow : 0);

	if (!combinedConditions) {
		// conditions not met, reset timer
		m_launchTimer.reset();
		isLaunchCondition = false;
	} else {
		// If conditions are met...
		isLaunchCondition = m_launchTimer.hasElapsedSec(engineConfiguration->launchActivateDelay);
	}

	isLaunchPreCondition = combinedConditions = (isLaunchConditionMet(rpm) == LaunchCondition::PreLaunch);;
	int targetLaunchSparkSkipPercent = engineConfiguration->launchSparkSkipPercent;
	if (isLaunchPreCondition) {
		m_preLaunchSparkSkipArmed = false;

	 	if (targetLaunchSparkSkipPercent > 0) {
			int launchRpm = engineConfiguration->launchRpm;
			int launchRpmWithWindow = launchRpm - engineConfiguration->launchRpmWindow;
			int launchSparkSkipInterpolated = interpolateClamped(launchRpmWithWindow, 0, launchRpm, targetLaunchSparkSkipPercent, rpm);
			float launchSparkSkip = ((float)launchSparkSkipInterpolated) / 100.0f;
			engine->hardSparkLimiter.setTargetSkipRatio(launchSparkSkip);
		}
	} else {
		if (m_preLaunchSparkSkipArmed == false) {
			engine->hardSparkLimiter.setTargetSkipRatio(0);
			m_preLaunchSparkSkipArmed = true;
		}
	}
}

bool LaunchControlBase::isLaunchRpmRetardCondition() const {
	return (isLaunchCondition || isLaunchPreCondition) && (retardThresholdRpm < Sensor::getOrZero(SensorType::Rpm));
}

bool LaunchControlBase::isLaunchSparkRpmRetardCondition() const {
	return isLaunchRpmRetardCondition() && engineConfiguration->launchSparkCutEnable;
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
