#pragma once

#include "globalaccess.h"
#include "engine_configuration_generated_structures.h"

float fuelClosedLoopCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE);
size_t computeStftBin(int rpm, float load, stft_s& cfg);
