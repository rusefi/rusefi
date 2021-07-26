/*
 * @file launch_control.cpp
 *
 *  @date 10. sep. 2019
 *      Author: Ola Ruud
 */

#include "pch.h"

#if EFI_LAUNCH_CONTROL
#include "boost_control.h"
#include "vehicle_speed.h"
#include "launch_control.h"
#include "periodic_task.h"
#include "advance_map.h"
#include "engine_state.h"
#include "advance_map.h"

static bool isInit = false;

LaunchControlBase launchInstance;

static int retardThresholdRpm;

/**
 * We can have active condition from switch or from clutch.
 * In case we are dependent on VSS we just return true.
 */
bool LaunchControlBase::isInsideSwitchCondition() const {
	switch (CONFIG(launchActivationMode)) {
	case SWITCH_INPUT_LAUNCH:
		if (isBrainPinValid(CONFIG(launchActivatePin))) {
			//todo: we should take into consideration if this sw is pulled high or low!
			engine->launchActivatePinState = efiReadPin(CONFIG(launchActivatePin));
		}
		return engine->launchActivatePinState;

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
	int speed = getVehicleSpeed();
	
	return (CONFIG(launchSpeedTreshold) > speed) || (!(CONFIG(launchActivationMode) ==  ALWAYS_ACTIVE_LAUNCH));
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

bool LaunchControlBase::isLaunchConditionMet(int rpm) const {

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

void updateLaunchConditions(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	launchInstance.update();
}

void LaunchControlBase::update() {

	if (!CONFIG(launchControlEnabled)) {
		return;
	}

#if ! EFI_UNIT_TEST
	if(!isInit) {
		return;
	}
#endif

	int rpm = GET_RPM();
	bool combinedConditions = isLaunchConditionMet(rpm);
	float timeDelay = CONFIG(launchActivateDelay);

	//recalculate in periodic task, this way we save time in applyLaunchControlLimiting
	//and still recalculat in case user changed the values
	retardThresholdRpm = CONFIG(launchRpm) + (CONFIG(enableLaunchRetard) ? 
	                     CONFIG(launchAdvanceRpmRange) : 0) + CONFIG(hardCutRpmRange);

	if (!combinedConditions) {
		// conditions not met, reset timer
		m_launchTimer.reset();
		engine->isLaunchCondition = false;
		engine->setLaunchBoostDuty = false;
		engine->applyLaunchControlRetard = false;
		engine->applyLaunchExtraFuel = false;
	} else {
		// If conditions are met...
		if (m_launchTimer.hasElapsedMs(timeDelay*1000) && combinedConditions) {
			engine->isLaunchCondition = true;           // ...enable launch!
			engine->applyLaunchExtraFuel = true;
		}
		if (CONFIG(enableLaunchBoost)) {
			engine->setLaunchBoostDuty = true;           // ...enable boost!
		}
		if (CONFIG(enableLaunchRetard)) {
			engine->applyLaunchControlRetard = true;    // ...enable retard!
		}
	}

#if EFI_TUNER_STUDIO
	if (CONFIG(debugMode) == DBG_LAUNCH) {
		tsOutputChannels.debugIntField5 = engine->clutchDownState;
		tsOutputChannels.debugFloatField1 = engine->launchActivatePinState;
		tsOutputChannels.debugFloatField2 = engine->isLaunchCondition;
		tsOutputChannels.debugFloatField3 = combinedConditions;
	}
#endif /* EFI_TUNER_STUDIO */
}

void setDefaultLaunchParameters(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->launchRpm = 4000;    // Rpm to trigger Launch condition
	engineConfiguration->launchTimingRetard = 10; // retard in absolute degrees ATDC
	engineConfiguration->launchTimingRpmRange = 500; // Rpm above Launch triggered for full retard
	engineConfiguration->launchSparkCutEnable = true;
	engineConfiguration->launchFuelCutEnable = false;
	engineConfiguration->hardCutRpmRange = 500; //Rpm above Launch triggered +(if retard enabled) launchTimingRpmRange to hard cut
	engineConfiguration->launchSpeedTreshold = 10; //maximum speed allowed before disable launch
	engineConfiguration->launchFuelAdded = 10; // Extra fuel in % when launch are triggered
	engineConfiguration->launchBoostDuty = 70; // boost valve duty cycle at launch
	engineConfiguration->launchActivateDelay = 3; // Delay in seconds for launch to kick in
	engineConfiguration->enableLaunchRetard = true;
	engineConfiguration->enableLaunchBoost = true;
	engineConfiguration->launchSmoothRetard = true; //interpolates the advance linear from launchrpm to fully retarded at launchtimingrpmrange
	engineConfiguration->antiLagRpmTreshold = 3000;

}

void applyLaunchControlLimiting(bool *limitedSpark, bool *limitedFuel DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (( engine->isLaunchCondition ) && ( retardThresholdRpm < GET_RPM() )) {
		*limitedSpark = engineConfiguration->launchSparkCutEnable;
		*limitedFuel = engineConfiguration->launchFuelCutEnable;
	} 
}

void initLaunchControl(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	INJECT_ENGINE_REFERENCE(&launchInstance);

	isInit = true;
}

#endif /* EFI_LAUNCH_CONTROL */
