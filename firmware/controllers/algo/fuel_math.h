/**
 * @file	fuel_math.h
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef FUEL_MAP_H_
#define FUEL_MAP_H_

#include "engine.h"

void prepareFuelMap(engine_configuration_s *engineConfiguration);

float getBaseFuel(int rpm DECLARE_ENGINE_PARAMETER_S);
float getBaseTableFuel(engine_configuration_s *engineConfiguration, int rpm, float engineLoad);
float getIatCorrection(float iat DECLARE_ENGINE_PARAMETER_S);
float getInjectorLag(float vBatt DECLARE_ENGINE_PARAMETER_S);
float getCltCorrection(float clt DECLARE_ENGINE_PARAMETER_S);
float getRunningFuel(float baseFuel, int rpm DECLARE_ENGINE_PARAMETER_S);
float getCrankingFuel(Engine *engine);
float getCrankingFuel3(engine_configuration_s *engineConfiguration, float coolantTemperature, uint32_t revolutionCounterSinceStart);
float getFuelMs(int rpm DECLARE_ENGINE_PARAMETER_S);

#endif /* FUEL_MAP_H_ */
