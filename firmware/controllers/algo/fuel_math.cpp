/**
 * @file	fuel_math.cpp
 * @brief	Fuel amount calculation logic
 *
 * While engine running, fuel amount is an interpolated value from the fuel map by getRpm() and getEngineLoad()
 * On top of the value from the fuel map we also apply
 * <BR>1) getInjectorLag() correction to account for fuel injector lag
 * <BR>2) getCltCorrection() for warm-up
 * <BR>3) getIatCorrection() to account for cold weather
 *
 * getCrankingFuel() depents only on getCoolantTemperature()
 *
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
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
#if EFI_ACCEL_ENRICHMENT
#include "accel_enrichment.h"
#endif /* EFI_ACCEL_ENRICHMENT */

EXTERN_ENGINE;

static Map3D1616 fuelMap;

float getBaseFuel(int rpm DECLARE_ENGINE_PARAMETER_S) {
	if (engine->engineConfiguration->algorithm == LM_SPEED_DENSITY) {
		return getSpeedDensityFuel(engine, rpm);
	} else {
		float engineLoad = getEngineLoadT(PASS_ENGINE_PARAMETER_F);
		return getBaseTableFuel(engine->engineConfiguration, rpm, engineLoad);
	}
}

/**
 * Number of injections into each cylinder per engine cycle
 */
static int getNumberOfInjections(engine_configuration_s const *engineConfiguration, injection_mode_e mode) {
	switch (mode) {
	case IM_SIMULTANEOUS:
		return engineConfiguration->specs.cylindersCount;
	case IM_SEQUENTIAL:
		return 1;
	case IM_BATCH:
		return engineConfiguration->specs.cylindersCount / 2;
	default:
		firmwareError("Unexpected getFuelMultiplier %d", mode);
		return 1;
	}
}


/**
 * @returns	Length of fuel injection, in milliseconds
 */
float getFuelMs(int rpm DECLARE_ENGINE_PARAMETER_S) {
	float theoreticalInjectionLength;
	if (isCrankingR(rpm)) {
		theoreticalInjectionLength = getCrankingFuel(engine) / getNumberOfInjections(engineConfiguration, engineConfiguration->crankingInjectionMode);
	} else {
		float baseFuel = getBaseFuel(rpm PASS_ENGINE_PARAMETER);
		float fuelPerCycle = getRunningFuel(baseFuel, rpm PASS_ENGINE_PARAMETER);
		theoreticalInjectionLength = fuelPerCycle / getNumberOfInjections(engineConfiguration, engine->engineConfiguration->injectionMode);
	}
	return theoreticalInjectionLength + ENGINE(injectorLagMs);
}

float getRunningFuel(float baseFuelMs, int rpm DECLARE_ENGINE_PARAMETER_S) {
	float iatCorrection = getIatCorrection(engine->engineState.iat PASS_ENGINE_PARAMETER);
	float cltCorrection = getCltCorrection(engine->engineState.clt PASS_ENGINE_PARAMETER);

#if EFI_ACCEL_ENRICHMENT
	float accelEnrichment = getAccelEnrichment();
	// todo: accelEnrichment
#endif /* EFI_ACCEL_ENRICHMENT */

	return baseFuelMs * cltCorrection * iatCorrection;
}

/**
 * @brief	Injector lag correction
 * @param	vBatt	Battery voltage.
 * @return	Time in ms for injection opening time based on current battery voltage
 */
float getInjectorLag(float vBatt DECLARE_ENGINE_PARAMETER_S) {
	if (cisnan(vBatt)) {
		warning(OBD_System_Voltage_Malfunction, "vBatt=%f", vBatt);
		return engineConfiguration->injector.lag;
	}
	float vBattCorrection = interpolate2d(vBatt, engineConfiguration->injector.battLagCorrBins,
			engineConfiguration->injector.battLagCorr, VBAT_INJECTOR_CURVE_SIZE);
	return engineConfiguration->injector.lag + vBattCorrection;
}

/**
 * @brief	Initialize fuel map data structure
 * @note this method has nothing to do with fuel map VALUES - it's job
 * is to prepare the fuel map data structure for 3d interpolation
 */
void prepareFuelMap(engine_configuration_s *engineConfiguration) {
	fuelMap.init(engineConfiguration->fuelTable);
}

/**
 * @brief Engine warm-up fuel correction.
 */
float getCltCorrection(float clt DECLARE_ENGINE_PARAMETER_S) {
	if (cisnan(clt))
		return 1; // this error should be already reported somewhere else, let's just handle it
	return interpolate2d(clt, engineConfiguration->cltFuelCorrBins, engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE);
}

float getIatCorrection(float iat DECLARE_ENGINE_PARAMETER_S) {
	if (cisnan(iat))
		return 1; // this error should be already reported somewhere else, let's just handle it
	return interpolate2d(iat, engineConfiguration->iatFuelCorrBins, engineConfiguration->iatFuelCorr, IAT_CURVE_SIZE);
}

/**
 * @return Fuel injection duration injection as specified in the fuel map, in milliseconds
 */
float getBaseTableFuel(engine_configuration_s *engineConfiguration, int rpm, float engineLoad) {
	if (cisnan(engineLoad)) {
		warning(OBD_PCM_Processor_Fault, "NaN engine load");
		return NAN;
	}
	return fuelMap.getValue(engineLoad, engineConfiguration->fuelLoadBins, rpm,
			engineConfiguration->fuelRpmBins);
}

#if EFI_ENGINE_CONTROL
/**
 * @return Duration of fuel injection while craning, in milliseconds
 */
float getCrankingFuel(Engine *engine) {
	return getCrankingFuel3(engine->engineConfiguration, getCoolantTemperature(engine),
			engine->rpmCalculator.getRevolutionCounterSinceStart()
		);
}
#endif

float getCrankingFuel3(engine_configuration_s *engineConfiguration, float coolantTemperature,
		uint32_t revolutionCounterSinceStart) {
	// these magic constants are in Celsius
	float baseCrankingFuel = engineConfiguration->cranking.baseFuel;
	if (cisnan(coolantTemperature))
		return baseCrankingFuel;
	float durationCoef = interpolate2d(revolutionCounterSinceStart,
			engineConfiguration->crankingCycleBins,
			engineConfiguration->crankingCycleCoef, CRANKING_CURVE_SIZE);

	return interpolate2d(coolantTemperature, engineConfiguration->crankingFuelBins,
			engineConfiguration->crankingFuelCoef, CRANKING_CURVE_SIZE)
			* baseCrankingFuel
			* durationCoef;
}
