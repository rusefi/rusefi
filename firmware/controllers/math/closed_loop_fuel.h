#pragma once

#include "globalaccess.h"

float fuelClosedLoopCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE);
size_t computeStftBin(int rpm, float load, stft_s& cfg);
