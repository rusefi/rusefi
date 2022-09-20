/**
 * @file rpm_calculator_api.h
 */

#include "rusefi_types.h"

#pragma once

class EngineRotationState {
public:
	virtual floatus_t getOneDegreeUs() = 0;
	virtual bool isCranking() const = 0;
	/**
	 * Returns true if the engine is not spinning (RPM==0)
	 */
	virtual bool isStopped() const = 0;

	// todo: move to triggerCentral/triggerShape since has nothing to do with rotation state!
	virtual operation_mode_e getOperationMode() const = 0;
};

EngineRotationState * getEngineRotationState();

injection_mode_e getCurrentInjectionMode();
