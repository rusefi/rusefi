/**
 * @file	fuel_math.h
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef FUEL_MAP_H_
#define FUEL_MAP_H_

#include "engine.h"

void prepareFuelMap(DECLARE_ENGINE_PARAMETER_F);

/**
 * @return total injection time into all cylinders, before CLT & IAT corrections
 */
floatms_t getBaseFuel(int rpm DECLARE_ENGINE_PARAMETER_S);

/**
 * @return baseFuel with CLT and IAT corrections
 */
floatms_t getRunningFuel(floatms_t baseFuel, int rpm DECLARE_ENGINE_PARAMETER_S);

floatms_t getRealMafFuel(float airMass, int rpm DECLARE_ENGINE_PARAMETER_S);

floatms_t getBaseTableFuel(engine_configuration_s *engineConfiguration, int rpm, float engineLoad);
float getBaroCorrection(DECLARE_ENGINE_PARAMETER_F);
int getNumberOfInjections(injection_mode_e mode DECLARE_ENGINE_PARAMETER_S);
angle_t getinjectionOffset(float rpm DECLARE_ENGINE_PARAMETER_S);
float getIatCorrection(float iat DECLARE_ENGINE_PARAMETER_S);
floatms_t getInjectorLag(float vBatt DECLARE_ENGINE_PARAMETER_S);
float getCltCorrection(float clt DECLARE_ENGINE_PARAMETER_S);
floatms_t getCrankingFuel(DECLARE_ENGINE_PARAMETER_F);
floatms_t getCrankingFuel3(float coolantTemperature, uint32_t revolutionCounterSinceStart DECLARE_ENGINE_PARAMETER_S);
floatms_t getInjectionDuration(int rpm DECLARE_ENGINE_PARAMETER_S);
percent_t getInjectorDutyCycle(int rpm DECLARE_ENGINE_PARAMETER_S);

#endif /* FUEL_MAP_H_ */
