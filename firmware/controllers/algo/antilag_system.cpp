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
	int clt = static_cast<int>(Sensor::getOrZero(SensorType::Clt));

	return engineConfiguration->ALSMinCLT < clt;
}

bool AntilagSystemBase::isALSMaxCLTCondition() const {
	int clt = static_cast<int>(Sensor::getOrZero(SensorType::Clt));

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

	bool isRequested = ALSMinRPMCondition &&
	    ALSMaxRPMCondition &&
	    ALSMinCLTCondition &&
	    ALSMaxCLTCondition &&
	    ALSMaxThrottleIntentCondition &&
	    ALSSwitchCondition;

	// "ALS timeout" limits one activation: it counts from the moment all other
	// conditions become true, and re-arms once any of them drops out. See #10701
	if (isRequested && !m_wasRequested) {
		ALStimer.reset();
	}
	m_wasRequested = isRequested;

	// Not requested means not timed out: the timeout is not what holds anti-lag off.
	ALSTimerCondition = !isRequested || isInsideALSTimerCondition();

	return isRequested && ALSTimerCondition;
}

void AntilagSystemBase::update() {
	float rpm = Sensor::getOrZero(SensorType::Rpm);
	if (engineConfiguration->antiLagEnabled) {
		isAntilagCondition = isAntilagConditionMet(rpm);
	} else {
		isAntilagCondition = false;
		m_wasRequested = false;
	}

#if EFI_ANTILAG_SYSTEM
	fuelALSCorrection = getFuelALSCorrection(rpm);
#endif // EFI_ANTILAG_SYSTEM
}

#endif /* EFI_ANTILAG_SYSTEM */
