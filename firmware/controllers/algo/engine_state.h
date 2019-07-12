/**
 * @file engine_state.h
 * @brief One header which acts as gateway to current engine state
 *
 * @date Dec 20, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef ENGINE_STATE_H_
#define ENGINE_STATE_H_

#include "global.h"
#include "engine_parts.h"
#include "pid.h"
#include "engine_state_generated.h"

class EngineState : public engine_state2_s {
public:
	EngineState();
	void periodicFastCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	void updateSlowSensors(DECLARE_ENGINE_PARAMETER_SIGNATURE);
	void updateTChargeK(int rpm, float tps DECLARE_ENGINE_PARAMETER_SUFFIX);

	FuelConsumptionState fuelConsumption;

	/**
	 * ETB position adjustment related to idle RPM control
	 */
	percent_t etbIdleAddition = 0;

	efitick_t crankingTime = 0;
	efitick_t timeSinceCranking = 0;

	WarningCodeState warnings;

	/**
	 * speed-density logic, calculated air flow in kg/h for tCharge Air-Interp. method
	 */
	float airFlow = 0;

	float engineNoiseHipLevel = 0;

	float auxValveStart = 0;
	float auxValveEnd = 0;

	// too much copy-paste here, something should be improved :)
	ThermistorMath iatCurve;
	ThermistorMath cltCurve;
	ThermistorMath auxTemp1Curve;
	ThermistorMath auxTemp2Curve;

	/**
	 * MAP averaging angle start, in relation to 'mapAveragingSchedulingAtIndex' trigger index index
	 */
	angle_t mapAveragingStart[INJECTION_PIN_COUNT];
	angle_t mapAveragingDuration = 0;

	angle_t timingAdvance = 0;
	// spark-related
	/**
	 * ignition dwell duration in ms
	 * See also dwellAngle
	 */
	floatms_t sparkDwell = 0;
	/**
	 * ignition dwell duration as crankshaft angle
	 * NAN if engine is stopped
	 * See also sparkDwell
	 */
	angle_t dwellAngle = NAN;

	angle_t cltTimingCorrection = 0;

	// fuel-related;
	float iatFuelCorrection = 0;
	float cltFuelCorrection = 0;
	float postCrankingFuelCorrection = 0;
	float fuelCutoffCorrection = 0;
	efitick_t coastingFuelCutStartTime = 0;
	/**
	 * injectorLag(VBatt)
	 *
	 * this value depends on a slow-changing VBatt value, so
	 * we update it once in a while
	 */
	floatms_t injectorLag = 0;

	/**
	 * See useWarmupPidAfr
	 */
	Pid warmupAfrPid;
	float warmupTargetAfr = 0;

	float baroCorrection = 0;

	efitick_t timeSinceLastTChargeK;

	float currentRawVE = 0;

	int vssEventCounter = 0;
	int totalLoggedBytes = 0;


	/**
	 * pre-calculated value from simple fuel lookup
	 */
	floatms_t baseTableFuel = 0;
	/**
	 * Raw fuel injection duration produced by current fuel algorithm, without any correction
	 */
	floatms_t baseFuel = 0;

	/**
	 * closed-loop fuel correction
	 */
	floatms_t fuelPidCorrection = 0;

	/**
	 * Total fuel with CLT, IAT and TPS acceleration corrections per cycle,
	 * as squirt duration.
	 * Without injector lag.
	 * @see baseFuel
	 * @see actualLastInjection
	 */
	floatms_t runningFuel = 0;

	/**
	 * TPS acceleration: extra fuel amount
	 */
	floatms_t tpsAccelEnrich = 0;

	angle_t injectionOffset = 0;

#if EFI_ENABLE_MOCK_ADC
	MockAdcState mockAdcState;
#endif /* EFI_ENABLE_MOCK_ADC */
};


#endif /* ENGINE_STATE_H_ */
