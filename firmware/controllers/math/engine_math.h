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


int cisnan(float f);

//float getDefaultVE(int rpm);

float getDefaultFuel(int rpm, float map);
//float getTCharge(int rpm, int tps, float coolantTemp, float airTemp);

float getOneDegreeTimeMs(int rpm);
float getOneDegreeTime(int rpm);
float getCrankshaftRevolutionTime(int rpm);

int isCrankingRT(engine_configuration_s *engineConfiguration, int rpm);
#define isCrankingR(rpm) isCrankingRT(engineConfiguration, rpm)

float fixAngle(float angle);
float getTriggerEventAngle(int triggerEventIndex);

float getEngineLoadT(engine_configuration_s *engineConfiguration);
#define getEngineLoad() getEngineLoadT(engineConfiguration)

void initializeIgnitionActions(float baseAngle, engine_configuration_s *engineConfiguration, engine_configuration2_s *engineConfiguration2);
void addFuelEvents(engine_configuration_s const *e,  engine_configuration2_s *engineConfiguration2, ActuatorEventList *list, injection_mode_e mode);

float getSparkDwellMsT(engine_configuration_s *engineConfiguration, int rpm);
#define getSparkDwellMs(rpm) getSparkDwellMsT(engineConfiguration, rpm)

void registerActuatorEventExt(engine_configuration_s const *engineConfiguration, trigger_shape_s * s, ActuatorEventList *list, OutputSignal *actuator, float angleOffset);

int getCylinderId(firing_order_e firingOrder, int index);
void prepareOutputSignals(engine_configuration_s *engineConfiguration,
		engine_configuration2_s *engineConfiguration2);

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
