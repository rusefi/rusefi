#pragma once

#include "engine.h"

float idealGasLaw(float volume, float pressure, float temperature);

class SpeedDensityBase {
public:
	static float getAirmassImpl(float ve, float manifoldPressure, float temperature DECLARE_ENGINE_PARAMETER_SUFFIX);
};
