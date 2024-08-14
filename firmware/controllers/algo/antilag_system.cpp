/*
 * @file antilag_system.cpp
 *
 *  @date 26. nov. 2022
 *      Author: Turbo Marian
 */

#include "pch.h"

#if EFI_ANTILAG_SYSTEM
#include "antilag_system.h"
#include "advance_map.h"
#include "engine_state.h"
#include "advance_map.h"
#include "fuel_math.h"

bool AntilagSystemBase::isInsideALSSwitchCondition() {
	isALSSwitchActivated = engineConfiguration->antiLagActivationMode == SWITCH_INPUT_ANTILAG;

	if (isALSSwitchActivated) {
		if (isBrainPinValid(engineConfiguration->ALSActivatePin)) {
#if EFI_PROD_CODE
			ALSActivatePinState = engineConfiguration->ALSActivateInverted ^ efiReadPin(engineConfiguration->ALSActivatePin);
#else
			ALSActivatePinState = false;
#endif
		}
		return ALSActivatePinState;
	} else {
		// ALWAYS_ACTIVE_ANTILAG
		return true;
	}
}

bool AntilagSystemBase::isALSMinRPMCondition(int rpm) const {
	return engineConfiguration->ALSMinRPM < rpm;
}

bool AntilagSystemBase::isALSMaxRPMCondition(int rpm) const {
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
	int throttleIntent = Sensor::getOrZero(SensorType::DriverThrottleIntent);

	return engineConfiguration->ALSMaxTPS > throttleIntent;
}

bool AntilagSystemBase::isInsideALSTimerCondition() {
	auto ALStime = ALStimer.getElapsedSeconds();

	return ALStime < engineConfiguration->ALSMaxDuration;
}

bool AntilagSystemBase::isAntilagConditionMet(int rpm) {


	ALSMinRPMCondition = isALSMinRPMCondition(rpm);
	ALSMaxRPMCondition = isALSMaxRPMCondition(rpm);
	ALSMinCLTCondition = isALSMinCLTCondition();
	ALSMaxCLTCondition = isALSMaxCLTCondition();
	ALSMaxThrottleIntentCondition = isALSMaxThrottleIntentCondition();
	ALSSwitchCondition = isInsideALSSwitchCondition();
	ALSTimerCondition = true;
/*
todo: looking for a hero to figure out unit test part of this
	ALSTimerCondition = isInsideALSTimerCondition();
	*/

	return ALSMinRPMCondition &&
	    ALSMaxRPMCondition &&
	    ALSMinCLTCondition &&
	    ALSMaxCLTCondition &&
	    ALSMaxThrottleIntentCondition &&
	    ALSSwitchCondition &&
		ALSTimerCondition;
}

void AntilagSystemBase::update() {
	int rpm = Sensor::getOrZero(SensorType::Rpm);
    isAntilagCondition = engineConfiguration->antiLagEnabled && isAntilagConditionMet(rpm);

	if (!ALSMaxRPMCondition) {
	    ALStimer.reset();
	}

#if EFI_ANTILAG_SYSTEM
	fuelALSCorrection = getFuelALSCorrection(rpm);
#endif // EFI_ANTILAG_SYSTEM
}

#endif /* EFI_ANTILAG_SYSTEM */
