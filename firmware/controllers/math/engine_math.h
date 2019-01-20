/**
 * @file	engine_math.h
 *
 * @date Jul 13, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef ENGINE_MATH_H_
#define ENGINE_MATH_H_

#include "engine_configuration.h"

#include "trigger_structure.h"
#include "table_helper.h"
#include "engine.h"

void setAlgorithm(engine_load_mode_e algo DECLARE_CONFIG_PARAMETER_SUFFIX);

#define assertEngineReference() efiAssertVoid(CUSTOM_ENGINE_REF, engine != NULL, "engine is NULL")

void setFlatInjectorLag(float value DECLARE_CONFIG_PARAMETER_SUFFIX);

#define fixAngle(angle, msg, code) fixAngle2(angle, msg, code, ENGINE(engineCycle))


/**
 * @return time needed to rotate crankshaft by one degree, in milliseconds.
 * @deprecated use at least Us, maybe even Nt
 */
#define getOneDegreeTimeMs(rpm) (1000.0f * 60 / 360 / (rpm))

/**
 * @return float, time needed to rotate crankshaft by one degree, in microseconds.
 * See also engine->rpmCalculator.oneDegreeUs
 */
#define getOneDegreeTimeUs(rpm) (1000000.0f * 60 / 360 / (rpm))

/**
 * @return float, time needed to rotate crankshaft by one degree, in native clicks.
 */
#define getOneDegreeTimeNt(rpm) (US2NT(1000000) * 60.0f / 360 / (rpm))

floatms_t getCrankshaftRevolutionTimeMs(int rpm);
floatms_t getEngineCycleDuration(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX);

float getEngineLoadT(DECLARE_ENGINE_PARAMETER_SIGNATURE);

floatms_t getSparkDwell(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX);

ignition_mode_e getCurrentIgnitionMode(DECLARE_ENGINE_PARAMETER_SIGNATURE);

/**
 * This lightweight method is invoked in case of a configuration change or initialization.
 * But also it's used for "Spinning-up to Cranking" transition.
 */
void prepareIgnitionPinIndices(ignition_mode_e ignitionMode DECLARE_ENGINE_PARAMETER_SUFFIX);

int getCylinderId(int index DECLARE_ENGINE_PARAMETER_SUFFIX);

void setFuelRpmBin(float from, float to DECLARE_CONFIG_PARAMETER_SUFFIX);
void setFuelLoadBin(float from, float to DECLARE_CONFIG_PARAMETER_SUFFIX);
void setTimingRpmBin(float from, float to DECLARE_CONFIG_PARAMETER_SUFFIX);
void setTimingLoadBin(float from, float to DECLARE_CONFIG_PARAMETER_SUFFIX);

void setSingleCoilDwell(engine_configuration_s *engineConfiguration);

// we combine trigger-defined triggerShape.tdcPosition with user-defined CONFIG(globalTriggerAngleOffset)
// expectation is that for well-known triggers CONFIG(globalTriggerAngleOffset) would usually be zero
// while for toothed wheels user would have to provide a value
#define tdcPosition() \
		(ENGINE(triggerCentral.triggerShape.tdcPosition) + CONFIG(globalTriggerAngleOffset))

#endif /* ENGINE_MATH_H_ */
