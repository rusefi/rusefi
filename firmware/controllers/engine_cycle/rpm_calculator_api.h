/**
 * @file rpm_calculator_api.h
 */

#include "rusefi_types.h"

class EngineRotationState {
public:
	virtual floatus_t getOneDegreeUs() = 0;
};

EngineRotationState * getEngineRotationState();
