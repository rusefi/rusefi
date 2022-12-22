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
	isALSSwitchActivated = engineConfiguration->antiLagActivationMode != SWITCH_INPUT_ANTILAG;

	if (isALSSwitchActivated) {
	if (isBrainPinValid(engineConfiguration->ALSActivatePin)) {
		ALSActivatePinState = engineConfiguration->ALSActivateInverted ^ efiReadPin(engineConfiguration->ALSActivatePin);
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

bool AntilagSystemBase::isALSMaxTPSCondition() const {
	int tps = Sensor::getOrZero(SensorType::Tps1);

	return engineConfiguration->ALSMaxTPS > tps;
}

bool AntilagSystemBase::isAntilagConditionMet() {

	ALSMinRPMCondition = isALSMinRPMCondition();
	ALSMaxRPMCondition = isALSMaxRPMCondition();
	ALSMinCLTCondition = isALSMinCLTCondition();
	ALSMaxCLTCondition = isALSMaxCLTCondition();
	ALSMaxTPSCondition = isALSMaxTPSCondition();
	ALSSwitchCondition = isInsideALSSwitchCondition();

	return ALSMinRPMCondition && ALSMaxRPMCondition && ALSMinCLTCondition && ALSMaxCLTCondition && ALSMaxTPSCondition && ALSSwitchCondition;
}

void AntilagSystemBase::update() {
	if (!engineConfiguration->antiLagEnabled) {
		return;
	}

    isAntilagCondition = isAntilagConditionMet();
}

#endif /* EFI_ANTILAG_SYSTEM */
