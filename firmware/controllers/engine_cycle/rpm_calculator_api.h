/**
 * @file rpm_calculator_api.h
 */

#include "rusefi_types.h"

#pragma once

class EngineRotationState {
public:
	virtual floatus_t getOneDegreeUs() = 0;
	virtual bool isCranking() const = 0;
	virtual operation_mode_e getOperationMode() const = 0;
};

EngineRotationState * getEngineRotationState();

injection_mode_e getCurrentInjectionMode();
