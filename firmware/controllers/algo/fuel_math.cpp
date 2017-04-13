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
 * @author Andrey Belomutskiy, (c) 2012-2017
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

#include "main.h"
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

/**
 * @return total duration of fuel injection per engine cycle, in milliseconds
 */
float getRealMafFuel(float airSpeed, int rpm DECLARE_ENGINE_PARAMETER_S) {
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

// todo: rename this method since it's now base+TPSaccel
floatms_t getBaseFuel(int rpm DECLARE_ENGINE_PARAMETER_S) {
	floatms_t tpsAccelEnrich = ENGINE(tpsAccelEnrichment.getTpsEnrichment(PASS_ENGINE_PARAMETER_F));
	efiAssert(!cisnan(tpsAccelEnrich), "NaN tpsAccelEnrich", 0);
	ENGINE(engineState.tpsAccelEnrich) = tpsAccelEnrich;

	floatms_t baseFuel;
	if (CONFIG(fuelAlgorithm) == LM_SPEED_DENSITY) {
		baseFuel = getSpeedDensityFuel(PASS_ENGINE_PARAMETER_F);
		efiAssert(!cisnan(baseFuel), "NaN sd baseFuel", 0);
	} else if (engineConfiguration->fuelAlgorithm == LM_REAL_MAF) {
		float maf = getRealMaf(PASS_ENGINE_PARAMETER_F) + engine->engineLoadAccelEnrichment.getEngineLoadEnrichment(PASS_ENGINE_PARAMETER_F);
		baseFuel = getRealMafFuel(maf, rpm PASS_ENGINE_PARAMETER);
		efiAssert(!cisnan(baseFuel), "NaN rm baseFuel", 0);
	} else {
		baseFuel = engine->engineState.baseTableFuel;
		efiAssert(!cisnan(baseFuel), "NaN bt baseFuel", 0);
	}
	engine->engineState.baseFuel = baseFuel;

	return tpsAccelEnrich + baseFuel;
}

angle_t getinjectionOffset(float rpm DECLARE_ENGINE_PARAMETER_S) {
	float engineLoad = getEngineLoadT(PASS_ENGINE_PARAMETER_F);
	angle_t result = fuelPhaseMap.getValue(rpm, engineLoad) + CONFIG(extraInjectionOffset);
	fixAngle(result, "inj offset");
	return result;
}

/**
 * Number of injections into each cylinder per engine cycle
 * @see getNumberOfSparks
 */
int getNumberOfInjections(injection_mode_e mode DECLARE_ENGINE_PARAMETER_S) {
	switch (mode) {
	case IM_SIMULTANEOUS:
		return engineConfiguration->specs.cylindersCount;
	case IM_SEQUENTIAL:
		return 1;
	case IM_BATCH:
		return 2;
	default:
		firmwareError(CUSTOM_ERR_INVALID_INJECTION_MODE, "Unexpected injection_mode_e %d", mode);
		return 1;
	}
}

/**
 * @see getCoilDutyCycle
 */
percent_t getInjectorDutyCycle(int rpm DECLARE_ENGINE_PARAMETER_S) {
	floatms_t totalPerCycle = getInjectionDuration(rpm PASS_ENGINE_PARAMETER) * getNumberOfInjections(engineConfiguration->injectionMode PASS_ENGINE_PARAMETER);
	floatms_t engineCycleDuration = getEngineCycleDuration(rpm PASS_ENGINE_PARAMETER);
	return 100 * totalPerCycle / engineCycleDuration;
}

/**
 * @returns	Length of each individual fuel injection, in milliseconds
 */
floatms_t getInjectionDuration(int rpm DECLARE_ENGINE_PARAMETER_S) {
	float theoreticalInjectionLength;
	bool isCranking = isCrankingR(rpm);
	int numberOfCylinders = getNumberOfInjections(isCranking ?
			engineConfiguration->crankingInjectionMode :
			engineConfiguration->injectionMode PASS_ENGINE_PARAMETER);
	if (numberOfCylinders == 0) {
		return 0; // we can end up here during configuration reset
	}
	if (isCranking) {
		theoreticalInjectionLength = getCrankingFuel(PASS_ENGINE_PARAMETER_F) / numberOfCylinders;
		efiAssert(!cisnan(theoreticalInjectionLength), "NaN cranking theoreticalInjectionLength", 0);
	} else {
		floatms_t baseFuel = getBaseFuel(rpm PASS_ENGINE_PARAMETER);
		floatms_t fuelPerCycle = getRunningFuel(baseFuel PASS_ENGINE_PARAMETER);
		theoreticalInjectionLength = fuelPerCycle / numberOfCylinders;
		efiAssert(!cisnan(theoreticalInjectionLength), "NaN fuelPerCycle", 0);
#if EFI_PRINTF_FUEL_DETAILS || defined(__DOXYGEN__)
	printf("baseFuel=%f fuelPerCycle=%f theoreticalInjectionLength=%f\t\n",
			baseFuel, fuelPerCycle, theoreticalInjectionLength);
#endif /*EFI_PRINTF_FUEL_DETAILS */
	}
	return theoreticalInjectionLength + ENGINE(engineState.injectorLag);
}

floatms_t getRunningFuel(floatms_t baseFuel DECLARE_ENGINE_PARAMETER_S) {
	float iatCorrection = ENGINE(engineState.iatFuelCorrection);
	float cltCorrection = ENGINE(engineState.cltFuelCorrection);
	efiAssert(!cisnan(iatCorrection), "NaN iatCorrection", 0);
	efiAssert(!cisnan(cltCorrection), "NaN cltCorrection", 0);

	floatms_t runningFuel = baseFuel * iatCorrection * cltCorrection + ENGINE(engineState.fuelPidCorrection);
	efiAssert(!cisnan(runningFuel), "NaN runningFuel", 0);
	ENGINE(engineState.runningFuel) = runningFuel;

	return runningFuel;
}

/**
 * @brief	Injector lag correction
 * @param	vBatt	Battery voltage.
 * @return	Time in ms for injection opening time based on current battery voltage
 */
floatms_t getInjectorLag(float vBatt DECLARE_ENGINE_PARAMETER_S) {
	if (cisnan(vBatt)) {
		warning(OBD_System_Voltage_Malfunction, "vBatt=%f", vBatt);
		return 0;
	}
	float vBattCorrection = interpolate2d(vBatt, engineConfiguration->injector.battLagCorrBins,
			engineConfiguration->injector.battLagCorr, VBAT_INJECTOR_CURVE_SIZE);
	return vBattCorrection;
}

/**
 * @brief	Initialize fuel map data structure
 * @note this method has nothing to do with fuel map VALUES - it's job
 * is to prepare the fuel map data structure for 3d interpolation
 */
void prepareFuelMap(DECLARE_ENGINE_PARAMETER_F) {
	fuelMap.init(config->fuelTable, config->fuelLoadBins, config->fuelRpmBins);
	fuelPhaseMap.init(config->injectionPhase, config->injPhaseLoadBins, config->injPhaseRpmBins);
}

/**
 * @brief Engine warm-up fuel correction.
 */
float getCltFuelCorrection(DECLARE_ENGINE_PARAMETER_F) {
	if (cisnan(engine->sensors.clt))
		return 1; // this error should be already reported somewhere else, let's just handle it
	return interpolate2d(engine->sensors.clt, config->cltFuelCorrBins, config->cltFuelCorr, CLT_CURVE_SIZE) / PERCENT_MULT;
}

angle_t getCltTimingCorrection(DECLARE_ENGINE_PARAMETER_F) {
	if (cisnan(engine->sensors.clt))
		return 0; // this error should be already reported somewhere else, let's just handle it
	return interpolate2d(engine->sensors.clt, engineConfiguration->cltTimingBins, engineConfiguration->cltTimingExtra, CLT_TIMING_CURVE_SIZE);
}

float getIatFuelCorrection(float iat DECLARE_ENGINE_PARAMETER_S) {
	if (cisnan(iat))
		return 1; // this error should be already reported somewhere else, let's just handle it
	return interpolate2d(iat, config->iatFuelCorrBins, config->iatFuelCorr, IAT_CURVE_SIZE);
}

/**
 * @return Fuel injection duration injection as specified in the fuel map, in milliseconds
 */
floatms_t getBaseTableFuel(int rpm, float engineLoad) {
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
}

float getBaroCorrection(DECLARE_ENGINE_PARAMETER_F) {
	if (hasBaroSensor(PASS_ENGINE_PARAMETER_F)) {
		return baroCorrMap.getValue(getRpmE(engine), getBaroPressure(PASS_ENGINE_PARAMETER_F));
	} else {
		return 1;
	}
}

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)
/**
 * @return Duration of fuel injection while craning
 */
floatms_t getCrankingFuel(DECLARE_ENGINE_PARAMETER_F) {
	return getCrankingFuel3(getCoolantTemperature(PASS_ENGINE_PARAMETER_F),
			engine->rpmCalculator.getRevolutionCounterSinceStart() PASS_ENGINE_PARAMETER);
}
#endif

floatms_t getCrankingFuel3(float coolantTemperature,
		uint32_t revolutionCounterSinceStart DECLARE_ENGINE_PARAMETER_S) {
	// these magic constants are in Celsius
	float baseCrankingFuel = engineConfiguration->cranking.baseFuel;
	if (cisnan(coolantTemperature))
		return baseCrankingFuel;
	float durationCoef = interpolate2d(revolutionCounterSinceStart, config->crankingCycleBins,
			config->crankingCycleCoef, CRANKING_CURVE_SIZE);

	float coolantTempCoef = interpolate2d(coolantTemperature, config->crankingFuelBins,
			config->crankingFuelCoef, CRANKING_CURVE_SIZE);

	float tpsCoef = interpolate2d(getTPS(PASS_ENGINE_PARAMETER_F), engineConfiguration->crankingTpsBins,
			engineConfiguration->crankingTpsCoef, CRANKING_CURVE_SIZE);

	return baseCrankingFuel * durationCoef * coolantTempCoef * tpsCoef;
}
