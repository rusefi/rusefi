/**
 * @file	engine_math.h
 *
 * @date Jul 13, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef ENGINE_MATH_H_
#define ENGINE_MATH_H_

#include "engine_configuration.h"

#include "trigger_structure.h"
#include "table_helper.h"
#include "engine.h"

#define OUTPUT_SIGNAL_MAX_SIZE 90

typedef ArrayList<OutputSignal, OUTPUT_SIGNAL_MAX_SIZE> OutputSignalList;

void findTriggerPosition(
		event_trigger_position_s *position, float angleOffset DECLARE_ENGINE_PARAMETER_S);

int isInjectionEnabled(engine_configuration_s *engineConfiguration);

void initializeIgnitionActions(angle_t advance, angle_t dwellAngle,
		IgnitionEventList *list DECLARE_ENGINE_PARAMETER_S);

/**
 * @brief Shifts angle into the [0..720) range for four stroke and [0..360) for two stroke
 * I guess this implementation would be faster than 'angle % engineCycle'
 */
#define fixAngle(angle)                     \
	while (angle < 0)                       \
		angle += CONFIG(engineCycle);       \
	while (angle >= CONFIG(engineCycle))    \
		angle -= CONFIG(engineCycle);

/**
 * @return time needed to rotate crankshaft by one degree, in milliseconds.
 * @deprecated use at least Us, maybe even Nt
 */
#define getOneDegreeTimeMs(rpm) (1000.0f * 60 / 360 / (rpm))

/**
 * @return float, time needed to rotate crankshaft by one degree, in microseconds.
 */
#define getOneDegreeTimeUs(rpm) (1000000.0f * 60 / 360 / (rpm))

/**
 * @return float, time needed to rotate crankshaft by one degree, in native clicks.
 */
#define getOneDegreeTimeNt(rpm) (US2NT(1000000) * 60.0f / 360 / (rpm))

float getCrankshaftRevolutionTimeMs(int rpm);

#define isCrankingR(rpm) ((rpm) > 0 && (rpm) < engineConfiguration->crankingSettings.crankingRpm)

float getEngineLoadT(DECLARE_ENGINE_PARAMETER_F);


float getSparkDwellMsT(int rpm DECLARE_ENGINE_PARAMETER_S);

int getCylinderId(firing_order_e firingOrder, int index);

void setFuelRpmBin(engine_configuration_s *engineConfiguration, float l, float r);
void setFuelLoadBin(engine_configuration_s *engineConfiguration, float l, float r);
void setTimingRpmBin(engine_configuration_s *engineConfiguration, float l, float r);
void setTimingLoadBin(engine_configuration_s *engineConfiguration, float l, float r);

void setSingleCoilDwell(engine_configuration_s *engineConfiguration);

#endif /* ENGINE_MATH_H_ */
