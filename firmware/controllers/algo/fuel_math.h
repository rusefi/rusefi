/**
 * @file	fuel_math.h
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef FUEL_MAP_H_
#define FUEL_MAP_H_

#include "engine.h"

void prepareFuelMap(DECLARE_ENGINE_PARAMETER_SIGNATURE);

/**
 * @return total injection time into all cylinders, before CLT & IAT corrections
 */
floatms_t getBaseFuel(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX);

/**
 * @return baseFuel with CLT and IAT corrections
 */
floatms_t getRunningFuel(floatms_t baseFuel DECLARE_ENGINE_PARAMETER_SUFFIX);

floatms_t getRealMafFuel(float airMass, int rpm DECLARE_ENGINE_PARAMETER_SUFFIX);

floatms_t getBaseTableFuel(int rpm, float engineLoad);
float getBaroCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE);
int getNumberOfInjections(injection_mode_e mode DECLARE_ENGINE_PARAMETER_SUFFIX);
angle_t getinjectionOffset(float rpm DECLARE_ENGINE_PARAMETER_SUFFIX);
float getIatFuelCorrection(float iat DECLARE_ENGINE_PARAMETER_SUFFIX);
floatms_t getInjectorLag(float vBatt DECLARE_ENGINE_PARAMETER_SUFFIX);
float getCltFuelCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE);
float getFuelCutOffCorrection(efitick_t nowNt, int rpm DECLARE_ENGINE_PARAMETER_SUFFIX);
angle_t getCltTimingCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE);
floatms_t getCrankingFuel(DECLARE_ENGINE_PARAMETER_SIGNATURE);
floatms_t getCrankingFuel3(float coolantTemperature, uint32_t revolutionCounterSinceStart DECLARE_ENGINE_PARAMETER_SUFFIX);
floatms_t getInjectionDuration(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX);
percent_t getInjectorDutyCycle(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX);

// convert injection duration (Ms/Nt) to fuel rate (L/h)
float getFuelRate(floatms_t totalInjDuration, efitick_t timePeriod DECLARE_ENGINE_PARAMETER_SUFFIX);

#endif /* FUEL_MAP_H_ */
