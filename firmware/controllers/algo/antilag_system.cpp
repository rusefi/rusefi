/*
 * @file antilag_system.cpp
 *
 *  @date 26. nov. 2022
 *      Author: Turbo Marian
 */

#include "pch.h"

#if EFI_ANTILAG_SYSTEM
#include "antilag_system.h"
#include "engine_state.h"
#include "fuel_math.h"

bool AntilagSystemBase::isInsideALSSwitchCondition() {
	isALSSwitchActivated = engineConfiguration->antiLagActivationMode == SWITCH_INPUT_ANTILAG;

	if (isALSSwitchActivated) {
		if (isBrainPinValid(engineConfiguration->ALSActivatePin)) {
#if EFI_PROD_CODE
			ALSActivatePinState = efiReadPin(engineConfiguration->ALSActivatePin, engineConfiguration->ALSActivatePinMode);
#else
			ALSActivatePinState = false;
#endif
		}
		return ALSActivatePinState;
	} else if (engineConfiguration->antiLagActivationMode == LUA_ANTILAG) {
	  return luaAntilagState;
	} else if (engineConfiguration->antiLagActivationMode == ALWAYS_ON_ANTILAG) {
		return true;
	} else {
		// [tag:user_facing_ini_labels] "Activation Mode" in the AntiLag INI dialog.
		criticalError("Invalid Activation Mode in AntiLag/Rotational Idle Settings: %d", engineConfiguration->antiLagActivationMode);
		return false;
	}
}

bool AntilagSystemBase::isALSMinRPMCondition(float rpm) const {
	return engineConfiguration->ALSMinRPM < rpm;
}

bool AntilagSystemBase::isALSMaxRPMCondition(float rpm) const {
	return engineConfiguration->ALSMaxRPM > rpm;
}

bool AntilagSystemBase::isALSMinCLTCondition() const {
	int clt = Sensor::getOrZero(SensorType::Clt);

	return engineConfiguration->ALSMinCLT < clt;
}

bool AntilagSystemBase::isALSMaxCLTCondition() const {
	int clt = Sensor::getOrZero(SensorType::Clt);

	return engineConfiguration->ALSMaxCLT > clt;
}

bool AntilagSystemBase::isALSMaxThrottleIntentCondition() const {
	auto throttleIntent = Sensor::get(SensorType::DriverThrottleIntent);

	return throttleIntent.Valid && engineConfiguration->ALSMaxTPS > static_cast<int>(throttleIntent.Value);
}

bool AntilagSystemBase::isInsideALSTimerCondition() {
	// A zero timeout leaves anti-lag duration unrestricted.
	if (engineConfiguration->ALSMaxDuration <= 0) {
		return true;
	}

	auto ALStime = ALStimer.getElapsedSeconds();

	return ALStime < engineConfiguration->ALSMaxDuration;
}

bool AntilagSystemBase::isAntilagConditionMet(float rpm) {


	ALSMinRPMCondition = isALSMinRPMCondition(rpm);
	ALSMaxRPMCondition = isALSMaxRPMCondition(rpm);
	ALSMinCLTCondition = isALSMinCLTCondition();
	ALSMaxCLTCondition = isALSMaxCLTCondition();
	ALSMaxThrottleIntentCondition = isALSMaxThrottleIntentCondition();
	ALSSwitchCondition = isInsideALSSwitchCondition();
	ALSTimerCondition = isInsideALSTimerCondition();

	return ALSMinRPMCondition &&
	    ALSMaxRPMCondition &&
	    ALSMinCLTCondition &&
	    ALSMaxCLTCondition &&
	    ALSMaxThrottleIntentCondition &&
	    ALSSwitchCondition &&
		ALSTimerCondition;
}

void AntilagSystemBase::update() {
	float rpm = Sensor::getOrZero(SensorType::Rpm);
    isAntilagCondition = engineConfiguration->antiLagEnabled && isAntilagConditionMet(rpm);

	if (!ALSMaxRPMCondition) {
	    ALStimer.reset();
	}

#if EFI_ANTILAG_SYSTEM
	fuelALSCorrection = getFuelALSCorrection(rpm);
#endif // EFI_ANTILAG_SYSTEM
}

#endif /* EFI_ANTILAG_SYSTEM */
