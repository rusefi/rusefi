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
#include "engine_state.h"
#include "tinymt32.h" // TL,DR: basic implementation of 'random'

/**
 * We can have active condition from switch or from clutch.
 * In case we are dependent on VSS we just return true.
 */
bool LaunchControlBase::isInsideSwitchCondition() {
	isSwitchActivated = engineConfiguration->launchActivationMode == SWITCH_INPUT_LAUNCH;
	isClutchActivated = engineConfiguration->launchActivationMode == CLUTCH_INPUT_LAUNCH;
    isBrakePedalActivated = engineConfiguration->launchActivationMode == STOP_INPUT_LAUNCH;

	if (isSwitchActivated) {
#if !EFI_SIMULATOR
		if (isBrainPinValid(engineConfiguration->launchActivatePin)) {
			launchActivatePinState = efiReadPin(engineConfiguration->launchActivatePin, engineConfiguration->launchActivatePinMode);
		}
#endif // EFI_PROD_CODE
		return launchActivatePinState;
	} else if (isClutchActivated) {
		  return getClutchDownState();
	} else if (isBrakePedalActivated) {
      return getBrakePedalState();
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

LaunchCondition LaunchControlBase::calculateRPMLaunchCondition(const float rpm) {
	if ((engineConfiguration->launchActivationMode == SWITCH_INPUT_LAUNCH)
		&& (engineConfiguration->torqueReductionActivationMode == LAUNCH_BUTTON)
		&& engineConfiguration->torqueReductionEnabled
		&& (engineConfiguration->torqueReductionArmingRpm <= rpm)
	) {
		// We need perform Shift Torque Reduction stuff (see
		// https://github.com/rusefi/rusefi/issues/5608#issuecomment-2391500472 and
		// https://github.com/rusefi/rusefi/issues/5608#issuecomment-2391772899 for details)
		return LaunchCondition::NotMet;
	}

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

LaunchCondition LaunchControlBase::calculateLaunchCondition(const float rpm) {
	const LaunchCondition currentRpmLaunchCondition = calculateRPMLaunchCondition(rpm);
	activateSwitchCondition = isInsideSwitchCondition();
	rpmLaunchCondition = (currentRpmLaunchCondition == LaunchCondition::Launch);
	rpmPreLaunchCondition = (currentRpmLaunchCondition == LaunchCondition::PreLaunch);
	speedCondition = isInsideSpeedCondition();
	tpsCondition = isInsideTpsCondition();

	if(speedCondition && activateSwitchCondition && tpsCondition) {
		return currentRpmLaunchCondition;
	} else {
		return LaunchCondition::NotMet;
	}
}

LaunchControlBase::LaunchControlBase() {
	launchActivatePinState = false;
	isPreLaunchCondition = false;
	isLaunchCondition = false;
}

float LaunchControlBase::getFuelCoefficient() const {
    return 1 + (isLaunchCondition && engineConfiguration->launchControlEnabled ? engineConfiguration->launchFuelAdderPercent / 100.0 : 0);
}

void LaunchControlBase::update() {
	if (!engineConfiguration->launchControlEnabled) {
		return;
	}

	const float rpm = Sensor::getOrZero(SensorType::Rpm);
	const LaunchCondition launchCondition = calculateLaunchCondition(rpm);
	isLaunchCondition = (launchCondition == LaunchCondition::Launch);
	isPreLaunchCondition = (launchCondition == LaunchCondition::PreLaunch);

	//and still recalculate in case user changed the values
	retardThresholdRpm = engineConfiguration->launchRpm;

	sparkSkipRatio = calculateSparkSkipRatio(rpm);
}

bool LaunchControlBase::isLaunchRpmRetardCondition() const {
	return isLaunchCondition && engineConfiguration->launchControlEnabled && (retardThresholdRpm < Sensor::getOrZero(SensorType::Rpm));
}

bool LaunchControlBase::isLaunchSparkRpmRetardCondition() const {
	return isLaunchRpmRetardCondition() && engineConfiguration->launchSparkCutEnable;
}

bool LaunchControlBase::isLaunchFuelRpmRetardCondition() const {
	return isLaunchRpmRetardCondition() && engineConfiguration->launchFuelCutEnable;
}

float LaunchControlBase::calculateSparkSkipRatio(const float rpm) const {
	float result = 0.0f;
	if (engineConfiguration->launchControlEnabled && engineConfiguration->launchSparkCutEnable) {
		if (isLaunchCondition) {
			result = 1.0f;
		} else if (isPreLaunchCondition) {
			const int launchRpm = engineConfiguration->launchRpm;
			const int sparkSkipStartRpm = launchRpm - engineConfiguration->launchRpmWindow;
			if (sparkSkipStartRpm <= rpm) {
				const float initialIgnitionCutRatio = engineConfiguration->initialIgnitionCutPercent / 100.0f;
				const int sparkSkipEndRpm = launchRpm - engineConfiguration->launchCorrectionsEndRpm;
				const float finalIgnitionCutRatio = engineConfiguration->finalIgnitionCutPercentBeforeLaunch / 100.0f;
				result = interpolateClamped(sparkSkipStartRpm, initialIgnitionCutRatio, sparkSkipEndRpm, finalIgnitionCutRatio, rpm);
			}
		}
	}
	return result;
}

SoftSparkLimiter::SoftSparkLimiter(const bool p_allowHardCut)
	: allowHardCut(p_allowHardCut) {
#if EFI_UNIT_TEST
    initLaunchControl();
#endif // EFI_UNIT_TEST
}

void SoftSparkLimiter::updateTargetSkipRatio(
	const float luaSparkSkip,
	const float tractionControlSparkSkip,
	const float launchOrShiftTorqueReductionControllerSparkSkipRatio
) {
	targetSkipRatio = luaSparkSkip;
	if (engineConfiguration->useHardSkipInTraction) {
		if (allowHardCut) {
			targetSkipRatio += tractionControlSparkSkip;
		}
	} else if (!allowHardCut) {
		targetSkipRatio += tractionControlSparkSkip;
	}

	if (allowHardCut) {
		/*
		 * We are applying launch controller spark skip ratio only for hard skip limiter (see
		 * https://github.com/rusefi/rusefi/issues/6566#issuecomment-2153149902).
		 */
		targetSkipRatio += launchOrShiftTorqueReductionControllerSparkSkipRatio;
	}
}

static tinymt32_t tinymt;

bool SoftSparkLimiter::shouldSkip()  {
	if (targetSkipRatio == 0 || (!allowHardCut && wasJustSkipped)) {
		wasJustSkipped = false;
		return false;
	}

	float random = tinymt32_generate_float(&tinymt);
	wasJustSkipped = random < (allowHardCut ? 1 : 2) * targetSkipRatio;
	return wasJustSkipped;
}

void initLaunchControl() {
    tinymt32_init(&tinymt, 1345135);
}

#endif /* EFI_LAUNCH_CONTROL */
