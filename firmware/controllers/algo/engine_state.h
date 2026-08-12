/**
 * @file engine_state.h
 * @brief One header which acts as gateway to current engine state
 *
 * @date Dec 20, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_parts.h"
#include "engine_state_generated.h"
#include "trigger_decoder.h"

class EngineState : public engine_state_s {
public:
	EngineState();
	void periodicFastCallback();
	void updateTChargeK(float rpm, float tps);

	void updateSparkSkip();

	/**
	 * it's important for wrapAngle() that engineCycle field never has zero
	 * always 360 or 720, never zero
	 */
	angle_t engineCycle{getEngineCycle(FOUR_STROKE_CRANK_SENSOR)};

	bool useOddFireWastedSpark = false;

	float injectionStage2Fraction = 0;

	Timer crankingTimer{};
	Timer undervoltageCrankingTimer{};

	WarningCodeState warnings{};

	// Estimated airflow based on whatever airmass model is active
	float airflowEstimate = 0;

	float auxValveStart = 0;
	float auxValveEnd = 0;

	/**
	 * MAP averaging angle start, in relation to '0' trigger index index
	 */
	angle_t mapAveragingStart[MAX_CYLINDER_COUNT]{};
	angle_t mapAveragingDuration = 0;

	/**
	 * timing advance is angle distance before Top Dead Center (TDP), i.e. "10 degree timing advance" means "happens 10 degrees before TDC"
	 */
	angle_t timingAdvance[MAX_CYLINDER_COUNT]{};

	// Angle between firing the main (primary) spark and the secondary (trailing) spark
	angle_t trailingSparkAngle = 0;

	Timer timeSinceLastTChargeK{};

	/**
	 * Raw fuel injection duration produced by current fuel algorithm, without any correction
	 */
	floatms_t baseFuel = 0;

	/**
	 * TPS acceleration: extra fuel amount
	 */
	floatms_t tpsAccelEnrich = 0;

	/**
	 * Each individual fuel injection duration for current engine cycle, without wall wetting
	 * including everything including injector lag, both cranking and running
	 * @see getInjectionDuration()
	 */
	floatms_t injectionDuration = 0;
	floatms_t injectionDurationStage2 = 0;

	angle_t injectionOffset = 0;

	multispark_state multispark{};

	bool shouldUpdateInjectionTiming = true;
};

EngineState * getEngineState();

bool getClutchDownState();
// 0 not pressed
// 1 pressed
bool getBrakePedalState();
