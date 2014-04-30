/**
 * @file	engine_math.h
 *
 * @date Jul 13, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef ENGINE_MATH_H_
#define ENGINE_MATH_H_

#include "engine_configuration.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

//float getDefaultVE(int rpm);

float getDefaultFuel(int rpm, float map);
//float getTCharge(int rpm, int tps, float coolantTemp, float airTemp);

float getOneDegreeTimeMs(int rpm);
float getCrankshaftRevolutionTimeMs(int rpm);

int isCrankingRT(engine_configuration_s *engineConfiguration, int rpm);
#define isCrankingR(rpm) isCrankingRT(engineConfiguration, rpm)

float fixAngle(float angle);
float getTriggerEventAngle(int triggerEventIndex);

float getEngineLoadT(engine_configuration_s *engineConfiguration);
#define getEngineLoad() getEngineLoadT(engineConfiguration)

float getSparkDwellMsT(engine_configuration_s *engineConfiguration, int rpm);
#define getSparkDwellMs(rpm) getSparkDwellMsT(engineConfiguration, rpm)

int getCylinderId(firing_order_e firingOrder, int index);

void setTableBin(float array[], int size, float l, float r);
void setFuelRpmBin(engine_configuration_s *engineConfiguration, float l, float r);
void setFuelLoadBin(engine_configuration_s *engineConfiguration, float l, float r);
void setTimingRpmBin(engine_configuration_s *engineConfiguration, float l, float r);
void setTimingLoadBin(engine_configuration_s *engineConfiguration, float l, float r);

void setSingleCoilDwell(engine_configuration_s *engineConfiguration);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ENGINE_MATH_H_ */
