/**
 * @file	fuel_math.cpp
 * @brief	Fuel amount calculation logic
 *
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
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

#include "pch.h"

#include "airmass.h"
#include "alphan_airmass.h"
#include "maf_airmass.h"
#include "speed_density_airmass.h"
#include "fuel_math.h"
#include "fuel_computer.h"
#include "injector_model.h"
#include "speed_density.h"
#include "speed_density_base.h"
#include "lua_hooks.h"

static fuel_Map3D_t fuelPhaseMap;
extern fuel_Map3D_t veMap;
extern lambda_Map3D_t lambdaMap;
extern baroCorr_Map3D_t baroCorrMap;
static mapEstimate_Map3D_t mapEstimationTable;

#if EFI_ENGINE_CONTROL

DISPLAY_STATE(Engine)

DISPLAY(DISPLAY_FIELD(sparkDwell))
DISPLAY(DISPLAY_FIELD(dwellAngle))
DISPLAY(DISPLAY_FIELD(cltTimingCorrection))
DISPLAY_TEXT(eol);

DISPLAY(DISPLAY_IF(isCrankingState)) float getCrankingFuel3(
	float baseFuel,
		uint32_t revolutionCounterSinceStart DECLARE_ENGINE_PARAMETER_SUFFIX) {
	// these magic constants are in Celsius
	float baseCrankingFuel;
	if (engineConfiguration->useRunningMathForCranking) {
		baseCrankingFuel = baseFuel;
	} else {
		// parameter is in milligrams, convert to grams
		baseCrankingFuel = engineConfiguration->cranking.baseFuel * 0.001f;
	}
	/**
	 * Cranking fuel changes over time
	 */
	DISPLAY_TEXT(Duration_coef);
	engine->engineState.DISPLAY_PREFIX(cranking).DISPLAY_FIELD(durationCoefficient) = interpolate2d(revolutionCounterSinceStart, config->crankingCycleBins,
			config->crankingCycleCoef);
	DISPLAY_TEXT(eol);

	/**
	 * Cranking fuel is different depending on engine coolant temperature
	 * If the sensor is failed, use 20 deg C
	 */
	auto clt = Sensor::get(SensorType::Clt);
	DISPLAY_TEXT(Coolant_coef);
	engine->engineState.DISPLAY_PREFIX(cranking).DISPLAY_FIELD(coolantTemperatureCoefficient) =
		interpolate2d(clt.value_or(20), config->crankingFuelBins, config->crankingFuelCoef);
	DISPLAY_SENSOR(CLT);
	DISPLAY_TEXT(eol);

	auto tps = Sensor::get(SensorType::DriverThrottleIntent);

	DISPLAY_TEXT(TPS_coef);
	engine->engineState.DISPLAY_PREFIX(cranking).DISPLAY_FIELD(tpsCoefficient) = tps.Valid ? 1 : interpolate2d(tps.Value, engineConfiguration->crankingTpsBins,
			engineConfiguration->crankingTpsCoef);


	/*
	engine->engineState.DISPLAY_PREFIX(cranking).DISPLAY_FIELD(tpsCoefficient) =
		tps.Valid 
		? interpolate2d(tps.Value, engineConfiguration->crankingTpsBins, engineConfiguration->crankingTpsCoef)
		: 1; // in case of failed TPS, don't correct.*/
	DISPLAY_SENSOR(TPS);
	DISPLAY_TEXT(eol);

	floatms_t crankingFuel = baseCrankingFuel
			* engine->engineState.cranking.durationCoefficient
			* engine->engineState.cranking.coolantTemperatureCoefficient
			* engine->engineState.cranking.tpsCoefficient;

	DISPLAY_TEXT(Cranking_fuel);
	engine->engineState.DISPLAY_PREFIX(cranking).DISPLAY_FIELD(fuel) = crankingFuel * 1000;

	if (crankingFuel <= 0) {
		warning(CUSTOM_ERR_ZERO_CRANKING_FUEL, "Cranking fuel value %f", crankingFuel);
	}
	return crankingFuel;
}

/* DISPLAY_ELSE */

floatms_t getRunningFuel(floatms_t baseFuel DECLARE_ENGINE_PARAMETER_SUFFIX) {
	ScopePerf perf(PE::GetRunningFuel);

	DISPLAY_TEXT(Base_fuel);
	ENGINE(engineState.DISPLAY_PREFIX(running).DISPLAY_FIELD(baseFuel)) = baseFuel;
	DISPLAY_TEXT(eol);


	DISPLAY_TEXT(Intake_coef);
	float iatCorrection = ENGINE(engineState.DISPLAY_PREFIX(running).DISPLAY_FIELD(intakeTemperatureCoefficient));
	DISPLAY_SENSOR(IAT);
	DISPLAY_TEXT(eol);

	DISPLAY_TEXT(Coolant_coef);
	float cltCorrection = ENGINE(engineState.DISPLAY_PREFIX(running).DISPLAY_FIELD(coolantTemperatureCoefficient));
	DISPLAY_SENSOR(CLT);
	DISPLAY_TEXT(eol);

	DISPLAY_TEXT(Post_cranking_coef);
	float postCrankingFuelCorrection = ENGINE(engineState.DISPLAY_PREFIX(running).DISPLAY_FIELD(postCrankingFuelCorrection));
	DISPLAY_TEXT(eol);

	float baroCorrection = ENGINE(engineState.baroCorrection);

	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(iatCorrection), "NaN iatCorrection", 0);
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(cltCorrection), "NaN cltCorrection", 0);
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(postCrankingFuelCorrection), "NaN postCrankingFuelCorrection", 0);

	floatms_t runningFuel = baseFuel * baroCorrection * iatCorrection * cltCorrection * postCrankingFuelCorrection;
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(runningFuel), "NaN runningFuel", 0);
	DISPLAY_TEXT(eol);

	DISPLAY_TEXT(Running_fuel);
	ENGINE(engineState.DISPLAY_PREFIX(running).DISPLAY_FIELD(fuel)) = runningFuel * 1000;
	DISPLAY_TEXT(eol);

	DISPLAY_TEXT(Injector_lag);
	DISPLAY(DISPLAY_PREFIX(running).DISPLAY_FIELD(injectorLag));
	DISPLAY_SENSOR(VBATT);
	return runningFuel;
}

/* DISPLAY_ENDIF */

static SpeedDensityAirmass sdAirmass(veMap, mapEstimationTable);
static MafAirmass mafAirmass(veMap);
static AlphaNAirmass alphaNAirmass(veMap);

AirmassModelBase* getAirmassModel(engine_load_mode_e mode DECLARE_ENGINE_PARAMETER_SUFFIX) {
	switch (mode) {
		case LM_SPEED_DENSITY: return &sdAirmass;
		case LM_REAL_MAF: return &mafAirmass;
		case LM_ALPHA_N: return &alphaNAirmass;
#if EFI_LUA
		case LM_LUA: return &(getLuaAirmassModel());
#endif
#if EFI_UNIT_TEST
		case LM_MOCK: return engine->mockAirmassModel;
#endif
		default:
			// this is a bad work-around for https://github.com/rusefi/rusefi/issues/1690 issue
			warning(CUSTOM_ERR_ASSERT, "Invalid airmass mode %d", CONFIG(fuelAlgorithm));
			return &sdAirmass;
/* todo: this should be the implementation
			return nullptr;
*/
	}
}

// Per-cylinder base fuel mass
static float getBaseFuelMass(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {
	ScopePerf perf(PE::GetBaseFuel);

	// airmass modes - get airmass first, then convert to fuel
	auto model = getAirmassModel(CONFIG(fuelAlgorithm) PASS_ENGINE_PARAMETER_SUFFIX);
	efiAssert(CUSTOM_ERR_ASSERT, model != nullptr, "Invalid airmass mode", 0.0f);

	auto airmass = model->getAirmass(rpm);

	// Plop some state for others to read
	ENGINE(engineState.sd.airMassInOneCylinder) = airmass.CylinderAirmass;
	ENGINE(engineState.fuelingLoad) = airmass.EngineLoadPercent;
	ENGINE(engineState.ignitionLoad) = getLoadOverride(airmass.EngineLoadPercent, CONFIG(ignOverrideMode) PASS_ENGINE_PARAMETER_SUFFIX);

	float baseFuelMass = ENGINE(fuelComputer)->getCycleFuel(airmass.CylinderAirmass, rpm, airmass.EngineLoadPercent);

	// Fudge it by the global correction factor
	baseFuelMass *= CONFIG(globalFuelCorrection);
	engine->engineState.baseFuel = baseFuelMass;

	if (cisnan(baseFuelMass)) {
		// todo: we should not have this here but https://github.com/rusefi/rusefi/issues/1690 
		return 0;
	}

	return baseFuelMass;
}

angle_t getInjectionOffset(float rpm, float load DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (cisnan(rpm)) {
		return 0; // error already reported
	}

	if (cisnan(load)) {
		return 0; // error already reported
	}

	angle_t value = fuelPhaseMap.getValue(rpm, load);

	if (cisnan(value)) {
		// we could be here while resetting configuration for example
		warning(CUSTOM_ERR_6569, "phase map not ready");
		return 0;
	}

	angle_t result = value + CONFIG(extraInjectionOffset);
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

float getInjectionModeDurationMultiplier(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	injection_mode_e mode = ENGINE(getCurrentInjectionMode(PASS_ENGINE_PARAMETER_SIGNATURE));

	switch (mode) {
	case IM_SIMULTANEOUS: {
		auto cylCount = engineConfiguration->specs.cylindersCount;

		if (cylCount == 0) {
			// we can end up here during configuration reset
			return 0;
		}

		return 1.0f / cylCount;
	}
	case IM_SEQUENTIAL:
	case IM_SINGLE_POINT:
		return 1;
	case IM_BATCH:
		return 0.5f;
	default:
		firmwareError(CUSTOM_ERR_INVALID_INJECTION_MODE, "Unexpected injection_mode_e %d", mode);
		return 0;
	}
}

/**
 * This is more like MOSFET duty cycle since durations include injector lag
 * @see getCoilDutyCycle
 */
percent_t getInjectorDutyCycle(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {
	floatms_t totalInjectiorAmountPerCycle = ENGINE(injectionDuration) * getNumberOfInjections(engineConfiguration->injectionMode PASS_ENGINE_PARAMETER_SUFFIX);
	floatms_t engineCycleDuration = getEngineCycleDuration(rpm PASS_ENGINE_PARAMETER_SUFFIX);
	return 100 * totalInjectiorAmountPerCycle / engineCycleDuration;
}

static float getCycleFuelMass(bool isCranking, float baseFuelMass DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (isCranking) {
		return getCrankingFuel(baseFuelMass PASS_ENGINE_PARAMETER_SUFFIX);
	} else {
		return getRunningFuel(baseFuelMass PASS_ENGINE_PARAMETER_SUFFIX);
	}
}

/**
 * @returns	Length of each individual fuel injection, in milliseconds
 *     in case of single point injection mode the amount of fuel into all cylinders, otherwise the amount for one cylinder
 */
floatms_t getInjectionMass(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {
	ScopePerf perf(PE::GetInjectionDuration);

#if EFI_SHAFT_POSITION_INPUT
	// Always update base fuel - some cranking modes use it
	float baseFuelMass = getBaseFuelMass(rpm PASS_ENGINE_PARAMETER_SUFFIX);

	bool isCranking = ENGINE(rpmCalculator).isCranking();
	float cycleFuelMass = getCycleFuelMass(isCranking, baseFuelMass PASS_ENGINE_PARAMETER_SUFFIX);
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(cycleFuelMass), "NaN cycleFuelMass", 0);

	// Fuel cut-off isn't just 0 or 1, it can be tapered
	cycleFuelMass *= ENGINE(engineState.fuelCutoffCorrection);

	float durationMultiplier = getInjectionModeDurationMultiplier(PASS_ENGINE_PARAMETER_SIGNATURE);
	float injectionFuelMass = cycleFuelMass * durationMultiplier;

	// Prepare injector flow rate & deadtime
	ENGINE(injectorModel)->prepare();

	floatms_t tpsAccelEnrich = ENGINE(tpsAccelEnrichment.getTpsEnrichment(PASS_ENGINE_PARAMETER_SIGNATURE));
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(tpsAccelEnrich), "NaN tpsAccelEnrich", 0);
	ENGINE(engineState.tpsAccelEnrich) = tpsAccelEnrich;

	// For legacy reasons, the TPS accel table is in units of milliseconds, so we have to convert BACK to mass
	float tpsAccelPerInjection = durationMultiplier * tpsAccelEnrich;

	float tpsFuelMass = ENGINE(injectorModel)->getFuelMassForDuration(tpsAccelPerInjection);

	return injectionFuelMass + tpsFuelMass;
#else
	return 0;
#endif
}

static FuelComputer fuelComputer(lambdaMap);
static InjectorModel injectorModel;

/**
 * @brief	Initialize fuel map data structure
 * @note this method has nothing to do with fuel map VALUES - it's job
 * is to prepare the fuel map data structure for 3d interpolation
 */
void initFuelMap(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	INJECT_ENGINE_REFERENCE(&sdAirmass);
	INJECT_ENGINE_REFERENCE(&mafAirmass);
	INJECT_ENGINE_REFERENCE(&alphaNAirmass);

	INJECT_ENGINE_REFERENCE(&fuelComputer);
	INJECT_ENGINE_REFERENCE(&injectorModel);

	ENGINE(fuelComputer) = &fuelComputer;
	ENGINE(injectorModel) = &injectorModel;

	mapEstimationTable.init(config->mapEstimateTable, config->mapEstimateTpsBins, config->mapEstimateRpmBins);

#if (IGN_LOAD_COUNT == FUEL_LOAD_COUNT) && (IGN_RPM_COUNT == FUEL_RPM_COUNT)
	fuelPhaseMap.init(config->injectionPhase, config->injPhaseLoadBins, config->injPhaseRpmBins);
#endif /* (IGN_LOAD_COUNT == FUEL_LOAD_COUNT) && (IGN_RPM_COUNT == FUEL_RPM_COUNT) */
}

/**
 * @brief Engine warm-up fuel correction.
 */
float getCltFuelCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	const auto [valid, clt] = Sensor::get(SensorType::Clt);
	
	if (!valid)
		return 1; // this error should be already reported somewhere else, let's just handle it

	return interpolate2d(clt, config->cltFuelCorrBins, config->cltFuelCorr);
}

angle_t getCltTimingCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	const auto [valid, clt] = Sensor::get(SensorType::Clt);

	if (!valid)
		return 0; // this error should be already reported somewhere else, let's just handle it

	return interpolate2d(clt, engineConfiguration->cltTimingBins, engineConfiguration->cltTimingExtra);
}

float getIatFuelCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	const auto [valid, iat] = Sensor::get(SensorType::Iat);

	if (!valid)
		return 1; // this error should be already reported somewhere else, let's just handle it

	return interpolate2d(iat, config->iatFuelCorrBins, config->iatFuelCorr);
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
	if (CONFIG(coastingFuelCutEnabled)) {
		auto [tpsValid, tpsPos] = Sensor::get(SensorType::Tps1);
		if (!tpsValid) {
			return 1.0f;
		}

		const auto [cltValid, clt] = Sensor::get(SensorType::Clt);
		if (!cltValid) {
			return 1.0f;
		}

		const auto [mapValid, map] = Sensor::get(SensorType::Map);
		if (!mapValid) {
			return 1.0f;
		}

		// gather events
		bool mapDeactivate = (map >= CONFIG(coastingFuelCutMap));
		bool tpsDeactivate = (tpsPos >= CONFIG(coastingFuelCutTps));
		// If no CLT sensor (or broken), don't allow DFCO
		bool cltDeactivate = clt < (float)CONFIG(coastingFuelCutClt);
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

float getBaroCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (Sensor::hasSensor(SensorType::BarometricPressure)) {
		// Default to 1atm if failed
		float pressure = Sensor::get(SensorType::BarometricPressure).value_or(101.325f);

		float correction = baroCorrMap.getValue(GET_RPM(), pressure);
		if (cisnan(correction) || correction < 0.01) {
			warning(OBD_Barometric_Press_Circ_Range_Perf, "Invalid baro correction %f", correction);
			return 1;
		}

		return correction;
	} else {
		return 1;
	}
}

#if EFI_ENGINE_CONTROL
/**
 * @return Duration of fuel injection while craning
 */
float getCrankingFuel(float baseFuel DECLARE_ENGINE_PARAMETER_SUFFIX) {
	return getCrankingFuel3(baseFuel, engine->rpmCalculator.getRevolutionCounterSinceStart() PASS_ENGINE_PARAMETER_SUFFIX);
}

float getStandardAirCharge(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	float totalDisplacement = CONFIG(specs.displacement);
	float cylDisplacement = totalDisplacement / CONFIG(specs.cylindersCount);

	// Calculation of 100% VE air mass in g/cyl - 1 cylinder filling at 1.204/L
	// 101.325kpa, 20C
	return idealGasLaw(cylDisplacement, 101.325f, 273.15f + 20.0f);
}

#endif
#endif
