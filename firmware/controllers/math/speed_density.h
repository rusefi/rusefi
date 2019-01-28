/**
 * @file	speed_density.h
 *
 * @date May 29, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
#ifndef SPEED_DENSITY_H_
#define SPEED_DENSITY_H_

#include "engine.h"

#define gramm_second_to_cc_minute(gs) ((gs) / 0.0119997981)
#define cc_minute_to_gramm_second(ccm) ((ccm) * 0.0119997981)

float getTCharge(int rpm, float tps, float coolantTemp, float airTemp DECLARE_ENGINE_PARAMETER_SUFFIX);
float getCylinderAirMass(float VE, float MAP, float tempK DECLARE_ENGINE_PARAMETER_SUFFIX);
float sdMath(float airMass, float AFR DECLARE_ENGINE_PARAMETER_SUFFIX);

void setDefaultVETable(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void initSpeedDensity(DECLARE_ENGINE_PARAMETER_SIGNATURE);
floatms_t getSpeedDensityFuel(DECLARE_ENGINE_PARAMETER_SIGNATURE);

#endif /* SPEED_DENSITY_H_ */
