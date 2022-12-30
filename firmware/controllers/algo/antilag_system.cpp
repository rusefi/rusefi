/*
 * @file antilag_system.cpp
 *
 *  @date 26. nov. 2022
 *      Author: Turbo Marian
 */

#include "pch.h"

#if EFI_ANTILAG_SYSTEM
#include "antilag_system.h"
#include "periodic_task.h"
#include "advance_map.h"
#include "engine_state.h"
#include "advance_map.h"

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

bool AntilagSystemBase::isALSMinRPMCondition() const {
	int rpm = Sensor::getOrZero(SensorType::Rpm);

	return engineConfiguration->ALSMinRPM < rpm;
}

bool AntilagSystemBase::isALSMaxRPMCondition() const {
	int rpm = Sensor::getOrZero(SensorType::Rpm);

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

bool AntilagSystemBase::isAntilagConditionMet() {

	ALSMinRPMCondition = isALSMinRPMCondition();
	ALSMaxRPMCondition = isALSMaxRPMCondition();
	ALSMinCLTCondition = isALSMinCLTCondition();
	ALSMaxCLTCondition = isALSMaxCLTCondition();
	ALSMaxThrottleIntentCondition = isALSMaxThrottleIntentCondition();
	ALSSwitchCondition = isInsideALSSwitchCondition();

	return ALSMinRPMCondition &&
	    ALSMaxRPMCondition &&
	    ALSMinCLTCondition &&
	    ALSMaxCLTCondition &&
	    ALSMaxThrottleIntentCondition &&
	    ALSSwitchCondition;
}

void AntilagSystemBase::update() {
    isAntilagCondition = engineConfiguration->antiLagEnabled && isAntilagConditionMet();
}

#endif /* EFI_ANTILAG_SYSTEM */
