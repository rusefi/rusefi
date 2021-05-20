/**
 * @file	fuel_math.h
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_ptr.h"
#include "rusefi_types.h"

void initFuelMap(DECLARE_ENGINE_PARAMETER_SIGNATURE);

/**
 * @return baseFuel with CLT and IAT corrections
 */
floatms_t getRunningFuel(floatms_t baseFuel DECLARE_ENGINE_PARAMETER_SUFFIX);

float getBaroCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE);
int getNumberOfInjections(injection_mode_e mode DECLARE_ENGINE_PARAMETER_SUFFIX);
angle_t getInjectionOffset(float rpm, float load DECLARE_ENGINE_PARAMETER_SUFFIX);
float getIatFuelCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE);

float getCltFuelCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE);
float getFuelCutOffCorrection(efitick_t nowNt, int rpm DECLARE_ENGINE_PARAMETER_SUFFIX);
angle_t getCltTimingCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE);
float getCrankingFuel(float baseFuel DECLARE_ENGINE_PARAMETER_SUFFIX);
float getCrankingFuel3(float baseFuel, uint32_t revolutionCounterSinceStart DECLARE_ENGINE_PARAMETER_SUFFIX);
floatms_t getInjectionMass(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX);
percent_t getInjectorDutyCycle(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX);

float getStandardAirCharge(DECLARE_ENGINE_PARAMETER_SIGNATURE);

struct AirmassModelBase;
AirmassModelBase* getAirmassModel(engine_load_mode_e mode DECLARE_ENGINE_PARAMETER_SUFFIX);
