/**
 * @file	fuel_math.cpp
 * @brief	Fuel amount calculation logic
 *
 * While engine running, fuel amount is an interpolated value from the fuel map by getRpm() and getEngineLoad()
 * On top of the value from the fuel map we also apply
 * <BR>1) getInjectorLag() correction to account for fuel injector lag
 * <BR>2) getCltFuelCorrection() for warm-up
 * <BR>3) getIatCorrection() to account for cold weather
 *
 * getCrankingFuel() depents only on getCoolantTemperature()
 *
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "global.h"
#include "fuel_math.h"
#include "interpolation.h"
#include "engine_configuration.h"
#include "allsensors.h"
#include "engine_math.h"
#include "rpm_calculator.h"
#include "speed_density.h"

EXTERN_ENGINE
;

fuel_Map3D_t fuelMap("fuel");
static fuel_Map3D_t fuelPhaseMap("fl ph");
extern fuel_Map3D_t ve2Map;
extern afr_Map3D_t afrMap;
extern baroCorr_Map3D_t baroCorrMap;

#if EFI_ENGINE_CONTROL

/**
 * @return total duration of fuel injection per engine cycle, in milliseconds
 */
float getRealMafFuel(float airSpeed, int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (rpm == 0)
		return 0;
	// duration of engine cycle, in hours
	float engineCycleDurationHr = 1.0 / 60 / rpm;

	float airMassKg = airSpeed * engineCycleDurationHr;

	/**
	 * todo: pre-calculate gramm/second injector flow to save one multiplication
	 * open question if that's needed since that's just a multiplication
	 */
	float injectorFlowRate = cc_minute_to_gramm_second(engineConfiguration->injector.flow);

	float afr = afrMap.getValue(rpm, airSpeed);
	float fuelMassGramm = airMassKg / afr * 1000;

	return 1000 * fuelMassGramm / injectorFlowRate;
}

/**
 * per-cylinder fuel amount
 * todo: rename this method since it's now base+TPSaccel
 */
floatms_t getBaseFuel(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {
	floatms_t tpsAccelEnrich = ENGINE(tpsAccelEnrichment.getTpsEnrichment(PASS_ENGINE_PARAMETER_SIGNATURE));
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(tpsAccelEnrich), "NaN tpsAccelEnrich", 0);
	ENGINE(engineState.tpsAccelEnrich) = tpsAccelEnrich;

	floatms_t baseFuel;
	if (CONFIG(fuelAlgorithm) == LM_SPEED_DENSITY) {
		baseFuel = getSpeedDensityFuel(getMap(PASS_ENGINE_PARAMETER_SIGNATURE) PASS_ENGINE_PARAMETER_SUFFIX);
		efiAssert(CUSTOM_ERR_ASSERT, !cisnan(baseFuel), "NaN sd baseFuel", 0);
	} else if (engineConfiguration->fuelAlgorithm == LM_REAL_MAF) {
		float maf = getRealMaf(PASS_ENGINE_PARAMETER_SIGNATURE) + engine->engineLoadAccelEnrichment.getEngineLoadEnrichment(PASS_ENGINE_PARAMETER_SIGNATURE);
		baseFuel = getRealMafFuel(maf, rpm PASS_ENGINE_PARAMETER_SUFFIX);
		efiAssert(CUSTOM_ERR_ASSERT, !cisnan(baseFuel), "NaN rm baseFuel", 0);
	} else {
		baseFuel = engine->engineState.baseTableFuel;
		efiAssert(CUSTOM_ERR_ASSERT, !cisnan(baseFuel), "NaN bt baseFuel", 0);
	}
	engine->engineState.baseFuel = baseFuel;

	return tpsAccelEnrich + baseFuel;
}

angle_t getInjectionOffset(float rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (cisnan(rpm)) {
		return 0; // error already reported
	}
	float engineLoad = getEngineLoadT(PASS_ENGINE_PARAMETER_SIGNATURE);
	if (cisnan(engineLoad)) {
		return 0; // error already reported
	}
	angle_t value = fuelPhaseMap.getValue(rpm, engineLoad);
	if (cisnan(value)) {
		// we could be here while resetting configuration for example
		warning(CUSTOM_ERR_6569, "phase map not ready");
		return 0;
	}
	angle_t result =  value + CONFIG(extraInjectionOffset);
	fixAngle(result, "inj offset#2", CUSTOM_ERR_6553);
	return result;
}

/**
 * Number of injections using each injector per engine cycle
 * @see getNumberOfSparks
 */
int getNumberOfInjections(injection_mode_e mode DECLARE_ENGINE_PARAMETER_SUFFIX) {
	switch (mode) {
	case IM_SIMULTANEOUS:
	case IM_SINGLE_POINT:
		return engineConfiguration->specs.cylindersCount;
	case IM_BATCH:
		return 2;
	case IM_SEQUENTIAL:
		return 1;
	default:
		firmwareError(CUSTOM_ERR_INVALID_INJECTION_MODE, "Unexpected injection_mode_e %d", mode);
		return 1;
	}
}

/**
 * This is more like MOSFET duty cycle since durations include injector lag
 * @see getCoilDutyCycle
 */
percent_t getInjectorDutyCycle(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {
	floatms_t totalInjectiorAmountPerCycle = getInjectionDuration(rpm PASS_ENGINE_PARAMETER_SUFFIX) * getNumberOfInjections(engineConfiguration->injectionMode PASS_ENGINE_PARAMETER_SUFFIX);
	floatms_t engineCycleDuration = getEngineCycleDuration(rpm PASS_ENGINE_PARAMETER_SUFFIX);
	return 100 * totalInjectiorAmountPerCycle / engineCycleDuration;
}

/**
 * @returns	Length of each individual fuel injection, in milliseconds
 *     in case of single point injection mode the amount of fuel into all cylinders, otherwise the amount for one cylinder
 */
floatms_t getInjectionDuration(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT
	bool isCranking = ENGINE(rpmCalculator).isCranking(PASS_ENGINE_PARAMETER_SIGNATURE);
	injection_mode_e mode = isCranking ?
			engineConfiguration->crankingInjectionMode :
			engineConfiguration->injectionMode;
	int numberOfInjections = getNumberOfInjections(mode PASS_ENGINE_PARAMETER_SUFFIX);
	if (numberOfInjections == 0) {
		warning(CUSTOM_CONFIG_NOT_READY, "config not ready");
		return 0; // we can end up here during configuration reset
	}
	floatms_t fuelPerCycle;
	if (isCranking) {
		fuelPerCycle = getCrankingFuel(PASS_ENGINE_PARAMETER_SIGNATURE);
		efiAssert(CUSTOM_ERR_ASSERT, !cisnan(fuelPerCycle), "NaN cranking fuelPerCycle", 0);
	} else {
		floatms_t baseFuel = getBaseFuel(rpm PASS_ENGINE_PARAMETER_SUFFIX);
		fuelPerCycle = getRunningFuel(baseFuel PASS_ENGINE_PARAMETER_SUFFIX);
		efiAssert(CUSTOM_ERR_ASSERT, !cisnan(fuelPerCycle), "NaN fuelPerCycle", 0);
#if EFI_PRINTF_FUEL_DETAILS
	printf("baseFuel=%.2f fuelPerCycle=%.2f \t\n",
			baseFuel, fuelPerCycle);
#endif /*EFI_PRINTF_FUEL_DETAILS */
	}
	if (mode == IM_SINGLE_POINT) {
		// here we convert per-cylinder fuel amount into total engine amount since the single injector serves all cylinders
		fuelPerCycle *= engineConfiguration->specs.cylindersCount;
	}
	// Fuel cut-off isn't just 0 or 1, it can be tapered
	fuelPerCycle *= ENGINE(engineState.fuelCutoffCorrection);
	// If no fuel, don't add injector lag
	if (fuelPerCycle == 0.0f)
		return 0;

	floatms_t theoreticalInjectionLength = fuelPerCycle / numberOfInjections;
	floatms_t injectorLag = ENGINE(engineState.injectorLag);
	if (cisnan(injectorLag)) {
		warning(CUSTOM_ERR_INJECTOR_LAG, "injectorLag not ready");
		return 0; // we can end up here during configuration reset
	}
	return theoreticalInjectionLength * engineConfiguration->globalFuelCorrection + injectorLag;
#else
	return 0;
#endif
}

floatms_t getRunningFuel(floatms_t baseFuel DECLARE_ENGINE_PARAMETER_SUFFIX) {
	float iatCorrection = ENGINE(engineState.iatFuelCorrection);
	float cltCorrection = ENGINE(engineState.cltFuelCorrection);
	float postCrankingFuelCorrection = ENGINE(engineState.postCrankingFuelCorrection);
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(iatCorrection), "NaN iatCorrection", 0);
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(cltCorrection), "NaN cltCorrection", 0);
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(postCrankingFuelCorrection), "NaN postCrankingFuelCorrection", 0);

	floatms_t runningFuel = baseFuel * iatCorrection * cltCorrection * postCrankingFuelCorrection + ENGINE(engineState.fuelPidCorrection);
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(runningFuel), "NaN runningFuel", 0);
	ENGINE(engineState.runningFuel) = runningFuel;

	return runningFuel;
}

/**
 * @brief	Injector lag correction
 * @param	vBatt	Battery voltage.
 * @return	Time in ms for injection opening time based on current battery voltage
 */
floatms_t getInjectorLag(float vBatt DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (cisnan(vBatt)) {
		warning(OBD_System_Voltage_Malfunction, "vBatt=%.2f", vBatt);
		return 0;
	}
	float vBattCorrection = interpolate2d("lag", vBatt, INJECTOR_LAG_CURVE);
	return vBattCorrection;
}

/**
 * @brief	Initialize fuel map data structure
 * @note this method has nothing to do with fuel map VALUES - it's job
 * is to prepare the fuel map data structure for 3d interpolation
 */
void initFuelMap(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	fuelMap.init(config->fuelTable, config->fuelLoadBins, config->fuelRpmBins);
	fuelPhaseMap.init(config->injectionPhase, config->injPhaseLoadBins, config->injPhaseRpmBins);
}

/**
 * @brief Engine warm-up fuel correction.
 */
float getCltFuelCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (cisnan(engine->sensors.clt))
		return 1; // this error should be already reported somewhere else, let's just handle it
	return interpolate2d("cltf", engine->sensors.clt, WARMUP_CLT_EXTRA_FUEL_CURVE);
}

angle_t getCltTimingCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (cisnan(engine->sensors.clt))
		return 0; // this error should be already reported somewhere else, let's just handle it
	return interpolate2d("timc", engine->sensors.clt, engineConfiguration->cltTimingBins, engineConfiguration->cltTimingExtra, CLT_TIMING_CURVE_SIZE);
}

float getIatFuelCorrection(float iat DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (cisnan(iat))
		return 1; // this error should be already reported somewhere else, let's just handle it
	return interpolate2d("iatc", iat, IAT_FUEL_CORRECTION_CURVE);
}

/**
 * @brief	Called from EngineState::periodicFastCallback to update the state.
 * @note The returned value is float, not boolean - to implement taper (smoothed correction).
 * @return	Fuel duration correction for fuel cut-off control (ex. if coasting). No correction if 1.0
 */
float getFuelCutOffCorrection(efitick_t nowNt, int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {
	// no corrections by default
	float fuelCorr = 1.0f;

	// coasting fuel cut-off correction
	if (CONFIGB(coastingFuelCutEnabled)) {
		percent_t tpsPos = getTPS(PASS_ENGINE_PARAMETER_SIGNATURE);
		float map = getMap(PASS_ENGINE_PARAMETER_SIGNATURE);
	
		// gather events
		bool mapDeactivate = (map >= CONFIG(coastingFuelCutMap));
		bool tpsDeactivate = (tpsPos >= CONFIG(coastingFuelCutTps));
		bool cltDeactivate = cisnan(engine->sensors.clt) ? false : (engine->sensors.clt < (float)CONFIG(coastingFuelCutClt));
		bool rpmDeactivate = (rpm < CONFIG(coastingFuelCutRpmLow));
		bool rpmActivate = (rpm > CONFIG(coastingFuelCutRpmHigh));
		
		// state machine (coastingFuelCutStartTime is also used as a flag)
		if (!mapDeactivate && !tpsDeactivate && !cltDeactivate && rpmActivate) {
			ENGINE(engineState.coastingFuelCutStartTime) = nowNt;
		} else if (mapDeactivate || tpsDeactivate || rpmDeactivate || cltDeactivate) {
			ENGINE(engineState.coastingFuelCutStartTime) = 0;
		}
		// enable fuelcut?
		if (ENGINE(engineState.coastingFuelCutStartTime) != 0) {
			// todo: add taper - interpolate using (nowNt - coastingFuelCutStartTime)?
			fuelCorr = 0.0f;
		}
	}
	
	// todo: add other fuel cut-off checks here (possibly cutFuelOnHardLimit?)
	return fuelCorr;
}

/**
 * @return Fuel injection duration injection as specified in the fuel map, in milliseconds
 */
floatms_t getBaseTableFuel(int rpm, float engineLoad) {
#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT
	if (cisnan(engineLoad)) {
		warning(CUSTOM_NAN_ENGINE_LOAD_2, "NaN engine load");
		return 0;
	}
	floatms_t result = fuelMap.getValue(rpm, engineLoad);
	if (cisnan(result)) {
		// result could be NaN in case of invalid table, like during initialization
		result = 0;
		warning(CUSTOM_ERR_FUEL_TABLE_NOT_READY, "baseFuel table not ready");
	}
	return result;
#else
	return 0;
#endif
}

float getBaroCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (hasBaroSensor(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		return baroCorrMap.getValue(GET_RPM(), getBaroPressure(PASS_ENGINE_PARAMETER_SIGNATURE));
	} else {
		return 1;
	}
}

#if EFI_ENGINE_CONTROL
/**
 * @return Duration of fuel injection while craning
 */
floatms_t getCrankingFuel(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return getCrankingFuel3(getCoolantTemperature(PASS_ENGINE_PARAMETER_SIGNATURE),
			engine->rpmCalculator.getRevolutionCounterSinceStart() PASS_ENGINE_PARAMETER_SUFFIX);
}
#endif

floatms_t getCrankingFuel3(float coolantTemperature,
		uint32_t revolutionCounterSinceStart DECLARE_ENGINE_PARAMETER_SUFFIX) {
	// these magic constants are in Celsius
	float baseCrankingFuel = engineConfiguration->cranking.baseFuel;
	float durationCoef = interpolate2d("crank", revolutionCounterSinceStart, config->crankingCycleBins,
			config->crankingCycleCoef, CRANKING_CURVE_SIZE);

	float coolantTempCoef = cisnan(coolantTemperature) ? 1 : interpolate2d("crank", coolantTemperature, config->crankingFuelBins,
			config->crankingFuelCoef, CRANKING_CURVE_SIZE);

	percent_t tps = getTPS(PASS_ENGINE_PARAMETER_SIGNATURE);

	float tpsCoef = cisnan(tps) ? 1 : interpolate2d("crankTps", tps, engineConfiguration->crankingTpsBins,
			engineConfiguration->crankingTpsCoef, CRANKING_CURVE_SIZE);

	return baseCrankingFuel * durationCoef * coolantTempCoef * tpsCoef;
}

float getFuelRate(floatms_t totalInjDuration, efitick_t timePeriod DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (timePeriod <= 0.0f)
		return 0.0f;
	float timePeriodMs = (float)NT2US(timePeriod) / 1000.0f;
	float fuelRate = totalInjDuration / timePeriodMs;
	const float cc_min_to_L_h = 60.0f / 1000.0f;
	return fuelRate * CONFIG(injector.flow) * cc_min_to_L_h;
}

#endif
