/**
 * @file	speed_density.cpp
 *
 * See http://rusefi.com/wiki/index.php?title=Manual:Software:Fuel_Control#Speed_Density for details
 *
 * @date May 29, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "speed_density.h"
#include "fuel_math.h"

#if defined(HAS_OS_ACCESS)
#error "Unexpected OS ACCESS HERE"
#endif

#define rpmMin 500
#define rpmMax 8000

fuel_Map3D_t veMap;
lambda_Map3D_t lambdaMap;
baroCorr_Map3D_t baroCorrMap;

#define tpMin 0
#define tpMax 100
//  http://rusefi.com/math/t_charge.html
/***panel:Charge Temperature*/
temperature_t getTCharge(int rpm, float tps DECLARE_ENGINE_PARAMETER_SUFFIX) {
	const auto clt = Sensor::get(SensorType::Clt);
	const auto iat = Sensor::get(SensorType::Iat);

	float airTemp = 0;

	// Without either valid, return 0C.  It's wrong, but it'll pretend to be nice and dense, so at least you won't go lean.
	if (!iat && !clt) {
		return 0;
	} else if (!clt && iat) {
		// Intake temperature will almost always be colder (richer) than CLT - use that
		return airTemp;
	} else if (!iat && clt) {
		// Without valid intake temperature, assume intake temp is 0C, and interpolate anyway
		airTemp = 0;
	} else {
		// All is well - use real air temp
		airTemp = iat.Value;
	}

	float coolantTemp = clt.Value;

	if ((engine->engineState.sd.isTChargeAirModel = (CONFIG(tChargeMode) == TCHARGE_MODE_AIR_INTERP))) {
		const floatms_t gramsPerMsToKgPerHour = (3600.0f * 1000.0f) / 1000.0f;
		// We're actually using an 'old' airMass calculated for the previous cycle, but it's ok, we're not having any self-excitaton issues
		floatms_t airMassForEngine = engine->engineState.sd.airMassInOneCylinder * CONFIG(specs.cylindersCount);
		// airMass is in grams per 1 cycle for 1 cyl. Convert it to airFlow in kg/h for the engine.
		// And if the engine is stopped (0 rpm), then airFlow is also zero (avoiding NaN division)
		floatms_t airFlow = (rpm == 0) ? 0 : airMassForEngine * gramsPerMsToKgPerHour / getEngineCycleDuration(rpm PASS_ENGINE_PARAMETER_SUFFIX);
		// just interpolate between user-specified min and max coefs, based on the max airFlow value
		engine->engineState.airFlow = airFlow;
		engine->engineState.sd.Tcharge_coff = interpolateClamped(0.0,
				CONFIG(tChargeAirCoefMin),
				CONFIG(tChargeAirFlowMax),
				CONFIG(tChargeAirCoefMax), airFlow);
		// save it for console output (instead of MAF massAirFlow)
	} else {
		float minRpmKcurrentTPS = interpolateMsg("minRpm", tpMin,
				CONFIG(tChargeMinRpmMinTps), tpMax,
				CONFIG(tChargeMinRpmMaxTps), tps);
		float maxRpmKcurrentTPS = interpolateMsg("maxRpm", tpMin,
				CONFIG(tChargeMaxRpmMinTps), tpMax,
				CONFIG(tChargeMaxRpmMaxTps), tps);

		engine->engineState.sd.Tcharge_coff = interpolateMsg("Kcurr", rpmMin, minRpmKcurrentTPS, rpmMax, maxRpmKcurrentTPS, rpm);
	}

	if (cisnan(engine->engineState.sd.Tcharge_coff)) {
		warning(CUSTOM_ERR_T2_CHARGE, "t2-getTCharge NaN");
		return coolantTemp;
	}

	// We use a robust interp. function for proper tcharge_coff clamping.
	float Tcharge = interpolateClamped(0.0f, coolantTemp, 1.0f, airTemp, engine->engineState.sd.Tcharge_coff);

	if (cisnan(Tcharge)) {
		// we can probably end up here while resetting engine state - interpolation would fail
		warning(CUSTOM_ERR_TCHARGE_NOT_READY, "getTCharge NaN");
		return coolantTemp;
	}

	return Tcharge;
}

void initSpeedDensity(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	veMap.init(config->veTable, config->veLoadBins, config->veRpmBins);
//	ve2Map.init(engineConfiguration->ve2Table, engineConfiguration->ve2LoadBins, engineConfiguration->ve2RpmBins);
	lambdaMap.init(config->lambdaTable, config->lambdaLoadBins, config->lambdaRpmBins);
	baroCorrMap.init(engineConfiguration->baroCorrTable, engineConfiguration->baroCorrPressureBins, engineConfiguration->baroCorrRpmBins);
}
