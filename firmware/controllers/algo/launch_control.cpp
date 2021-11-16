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

/**
 * We can have active condition from switch or from clutch.
 * In case we are dependent on VSS we just return true.
 */
bool LaunchControlBase::isInsideSwitchCondition() {
	switch (CONFIG(launchActivationMode)) {
	case SWITCH_INPUT_LAUNCH:
		if (isBrainPinValid(CONFIG(launchActivatePin))) {
			//todo: we should take into consideration if this sw is pulled high or low!
			launchActivatePinState = efiReadPin(CONFIG(launchActivatePin));
		}
		return launchActivatePinState;

	case CLUTCH_INPUT_LAUNCH:
		if (isBrainPinValid(CONFIG(clutchDownPin))) {
			return engine->clutchDownState;
		} else {
			return false;
		}
		
	default:
		// ALWAYS_ACTIVE_LAUNCH
		return true;
	}
}

/**
 * Returns True in case Vehicle speed is less then trashold. 
 * This condiiion would only return true based on speed if DisablebySpeed is true
 * The condition logic is written in that way, that if we do not use disable by speed
 * then we have to return true, and trust that we would disable by other condition!
 */ 
bool LaunchControlBase::isInsideSpeedCondition() const {
	int speed = Sensor::getOrZero(SensorType::VehicleSpeed);
	
	return (CONFIG(launchSpeedThreshold) > speed) || (!(CONFIG(launchActivationMode) ==  ALWAYS_ACTIVE_LAUNCH));
}

/**
 * Returns false if TPS is invalid or TPS > preset trashold
 */
bool LaunchControlBase::isInsideTpsCondition() const {
	auto tps = Sensor::get(SensorType::DriverThrottleIntent);

	// Disallow launch without valid TPS
	if (!tps.Valid) {
		return false;
	}

	return CONFIG(launchTpsTreshold) < tps.Value;
}

/**
 * Condition is true as soon as we are above LaunchRpm
 */
bool LaunchControlBase::isInsideRPMCondition(int rpm) const {
	int launchRpm = CONFIG(launchRpm);
	return (launchRpm < rpm);
}

bool LaunchControlBase::isLaunchConditionMet(int rpm) {

	bool activateSwitchCondition = isInsideSwitchCondition();
	bool rpmCondition = isInsideRPMCondition(rpm);
	bool speedCondition = isInsideSpeedCondition();
	bool tpsCondition = isInsideTpsCondition();

#if EFI_TUNER_STUDIO
	if (engineConfiguration->debugMode == DBG_LAUNCH) {
		tsOutputChannels.debugIntField1 = rpmCondition;
		tsOutputChannels.debugIntField2 = tpsCondition;
		tsOutputChannels.debugIntField3 = speedCondition;
		tsOutputChannels.debugIntField4 = activateSwitchCondition;
	}
#endif /* EFI_TUNER_STUDIO */

	return speedCondition && activateSwitchCondition && rpmCondition && tpsCondition;
}

void LaunchControlBase::update() {
	if (!CONFIG(launchControlEnabled)) {
		return;
	}

	int rpm = GET_RPM();
	bool combinedConditions = isLaunchConditionMet(rpm);

	//and still recalculat in case user changed the values
	retardThresholdRpm = CONFIG(launchRpm) + (CONFIG(enableLaunchRetard) ? 
	                     CONFIG(launchAdvanceRpmRange) : 0) + CONFIG(hardCutRpmRange);

	if (!combinedConditions) {
		// conditions not met, reset timer
		m_launchTimer.reset();
		isLaunchCondition = false;
	} else {
		// If conditions are met...
		isLaunchCondition = m_launchTimer.hasElapsedSec(CONFIG(launchActivateDelay));
	}

#if EFI_TUNER_STUDIO
	if (CONFIG(debugMode) == DBG_LAUNCH) {
		tsOutputChannels.debugIntField5 = engine->clutchDownState;
		tsOutputChannels.debugFloatField1 = launchActivatePinState;
		tsOutputChannels.debugFloatField2 = isLaunchCondition;
		tsOutputChannels.debugFloatField3 = combinedConditions;
	}
#endif /* EFI_TUNER_STUDIO */
}

bool LaunchControlBase::isLaunchRpmRetardCondition() const {
	return isLaunchCondition && (retardThresholdRpm < GET_RPM());
}

bool LaunchControlBase::isLaunchSparkRpmRetardCondition() const {
	return isLaunchRpmRetardCondition() && engineConfiguration->launchSparkCutEnable;
}

bool LaunchControlBase::isLaunchFuelRpmRetardCondition() const {
	return isLaunchRpmRetardCondition() && engineConfiguration->launchFuelCutEnable;
}

void initLaunchControl(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engine->launchController.inject();
}

#endif /* EFI_LAUNCH_CONTROL */
