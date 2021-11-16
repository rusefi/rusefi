/**
 * @file	engine_math.h
 *
 * @date Jul 13, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "trigger_structure.h"

void setAlgorithm(engine_load_mode_e algo);

#define assertEngineReference() efiAssertVoid(CUSTOM_ENGINE_REF, engine != NULL, "engine is NULL")

void setFlatInjectorLag(float value);

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

floatms_t getCrankshaftRevolutionTimeMs(int rpm);
floatms_t getEngineCycleDuration(int rpm);

float getFuelingLoad();
float getIgnitionLoad();

floatms_t getSparkDwell(int rpm);

ignition_mode_e getCurrentIgnitionMode();

/**
 * This lightweight method is invoked in case of a configuration change or initialization.
 * But also it's used for "Spinning-up to Cranking" transition.
 */
void prepareIgnitionPinIndices(ignition_mode_e ignitionMode);

int getCylinderId(int index);
int getNextFiringCylinderId(int prevCylinderId);

void setTimingRpmBin(float from, float to);
void setTimingLoadBin(float from, float to);

void setSingleCoilDwell();

// we combine trigger-defined triggerShape.tdcPosition with user-defined CONFIG(globalTriggerAngleOffset)
// expectation is that for well-known triggers CONFIG(globalTriggerAngleOffset) would usually be zero
// while for toothed wheels user would have to provide a value
#define tdcPosition() \
		(TRIGGER_WAVEFORM(tdcPosition) + CONFIG(globalTriggerAngleOffset))
