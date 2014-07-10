/**
 * @file	speed_density.h
 *
 * @date May 29, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */
#ifndef SPEED_DENSITY_H_
#define SPEED_DENSITY_H_

#include "engine_configuration.h"
#include "ec2.h"

float getTCharge(int rpm, int tps, float coolantTemp, float airTemp);
void setDetaultVETable(engine_configuration_s *engineConfiguration);
float sdMath(engine_configuration_s *engineConfiguration, float VE, float MAP, float AFR, float temp);

#define gramm_second_to_cc_minute(gs) ((gs) / 0.0119997981)

#define cc_minute_to_gramm_second(ccm) ((ccm) * 0.0119997981)

void setDetaultVETable(engine_configuration_s *engineConfiguration);
void initSpeedDensity(engine_configuration_s *engineConfiguration);

#endif /* SPEED_DENSITY_H_ */
