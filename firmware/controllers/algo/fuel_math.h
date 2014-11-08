/**
 * @file	fuel_math.h
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef FUEL_MAP_H_
#define FUEL_MAP_H_

#include "engine.h"

void prepareFuelMap(engine_configuration_s *engineConfiguration);

float getBaseFuel(Engine *engine, int rpm);
float getBaseTableFuel(engine_configuration_s *engineConfiguration, int rpm, float engineLoad);
float getIatCorrection(float iat DECLATE_ENGINE_PARAMETER);
float getInjectorLag(float vBatt DECLATE_ENGINE_PARAMETER);
float getCltCorrection(float clt DECLATE_ENGINE_PARAMETER);
float getRunningFuel(float baseFuel, int rpm DECLATE_ENGINE_PARAMETER);
float getCrankingFuel(Engine *engine);
float getCrankingFuel3(engine_configuration_s *engineConfiguration, float coolantTemperature, uint32_t revolutionCounterSinceStart);
float getFuelMs(int rpm DECLATE_ENGINE_PARAMETER);

#endif /* FUEL_MAP_H_ */
