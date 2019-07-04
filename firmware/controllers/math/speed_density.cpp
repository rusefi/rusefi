/**
 * @file	speed_density.cpp
 *
 * See http://rusefi.com/wiki/index.php?title=Manual:Software:Fuel_Control#Speed_Density for details
 *
 * @date May 29, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"
#include "globalaccess.h"
#include "speed_density.h"
#include "interpolation.h"
#include "engine.h"
#include "engine_math.h"
#include "maf2map.h"
#include "config_engine_specs.h"

//todo#if defined(HAS_OS_ACCESS)
//todo#error "Unexpected OS ACCESS HERE"
//todo#endif

#define rpmMin 500
#define rpmMax 8000

EXTERN_ENGINE;

fuel_Map3D_t veMap("VE");
fuel_Map3D_t ve2Map("VE2");
afr_Map3D_t afrMap("AFR", 1.0 / AFR_STORAGE_MULT);
baroCorr_Map3D_t baroCorrMap("baro");

#define tpMin 0
#define tpMax 100
//  http://rusefi.com/math/t_charge.html
/***panel:Charge Temperature*/
temperature_t getTCharge(int rpm, float tps, float coolantTemp, float airTemp DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (cisnan(coolantTemp) || cisnan(airTemp)) {
		warning(CUSTOM_ERR_NAN_TCHARGE, "t-getTCharge NaN");
		return coolantTemp;
	}


	if ((engine->engineState.sd.DISPLAY_IF(isTChargeAirModel) = (CONFIG(tChargeMode) == TCHARGE_MODE_AIR_INTERP))) {
		const floatms_t gramsPerMsToKgPerHour = (3600.0f * 1000.0f) / 1000.0f;
		// We're actually using an 'old' airMass calculated for the previous cycle, but it's ok, we're not having any self-excitaton issues
		floatms_t airMassForEngine = engine->engineState.sd./***display*/airMassInOneCylinder * CONFIG(specs.cylindersCount);
		// airMass is in grams per 1 cycle for 1 cyl. Convert it to airFlow in kg/h for the engine.
		// And if the engine is stopped (0 rpm), then airFlow is also zero (avoiding NaN division)
		floatms_t airFlow = (rpm == 0) ? 0 : airMassForEngine * gramsPerMsToKgPerHour / getEngineCycleDuration(rpm PASS_ENGINE_PARAMETER_SUFFIX);
		// just interpolate between user-specified min and max coefs, based on the max airFlow value
		DISPLAY_TEXT(interpolate_Air_Flow)
		engine->engineState.DISPLAY_FIELD(airFlow) = airFlow;
		DISPLAY_TEXT(Between)
		engine->engineState.sd.Tcharge_coff = interpolateClamped(0.0,
				CONFIG(DISPLAY_CONFIG(tChargeAirCoefMin)),
				CONFIG(DISPLAY_CONFIG(tChargeAirFlowMax)),
				CONFIG(DISPLAY_CONFIG(tChargeAirCoefMax)), airFlow);
		// save it for console output (instead of MAF massAirFlow)
	} else/* DISPLAY_ELSE */ {
		// TCHARGE_MODE_RPM_TPS
		DISPLAY_TEXT(interpolate_3D)
		DISPLAY_SENSOR(RPM)
		DISPLAY_TEXT(and)
		DISPLAY_SENSOR(TPS)
		DISPLAY_TEXT(EOL)
		DISPLAY_TEXT(Between)
		float minRpmKcurrentTPS = interpolateMsg("minRpm", tpMin,
				CONFIG(DISPLAY_CONFIG(tChargeMinRpmMinTps)), tpMax,
				CONFIG(DISPLAY_CONFIG(tChargeMinRpmMaxTps)), tps);
		DISPLAY_TEXT(EOL)
		float maxRpmKcurrentTPS = interpolateMsg("maxRpm", tpMin,
				CONFIG(DISPLAY_CONFIG(tChargeMaxRpmMinTps)), tpMax,
				CONFIG(DISPLAY_CONFIG(tChargeMaxRpmMaxTps)), tps);

		engine->engineState.sd.Tcharge_coff = interpolateMsg("Kcurr", rpmMin, minRpmKcurrentTPS, rpmMax, maxRpmKcurrentTPS, rpm);
	/* DISPLAY_ENDIF */
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

/**
 * is J/g*K
 */
#define GAS_R 0.28705

/**
 * @return air mass in grams
 */
static float getCycleAirMass(float volumetricEfficiency, float MAP, float tempK DECLARE_GLOBAL_SUFFIX) {
	return (get_specs_displacement * volumetricEfficiency * MAP) / (GAS_R * tempK);
}

float getCylinderAirMass(float volumetricEfficiency, float MAP, float tempK DECLARE_GLOBAL_SUFFIX) {
	return getCycleAirMass(volumetricEfficiency, MAP, tempK PASS_GLOBAL_SUFFIX)
			/ get_specs_cylindersCount;
}

/**
 * @return per cylinder injection time, in seconds
 */
float sdMath(float airMass, float AFR DECLARE_GLOBAL_SUFFIX) {

	/**
	 * todo: pre-calculate gramm/second injector flow to save one multiplication
	 * open question if that's needed since that's just a multiplication
	 */
	float injectorFlowRate = cc_minute_to_gramm_second(get_injector_flow);
	/**
	 * injection_pulse_duration = fuel_mass / injector_flow
	 * fuel_mass = air_mass / target_afr
	 *
	 * injection_pulse_duration = (air_mass / target_afr) / injector_flow
	 */
	return airMass / (AFR * injectorFlowRate);
}

EXTERN_ENGINE;

/**
 * @return per cylinder injection time, in Milliseconds
 */
floatms_t getSpeedDensityFuel(float map DECLARE_GLOBAL_SUFFIX) {
	/**
	 * most of the values are pre-calculated for performance reasons
	 */
	float tChargeK = ENGINE(engineState.sd.tChargeK);
	if (cisnan(tChargeK)) {
		warning(CUSTOM_ERR_TCHARGE_NOT_READY2, "tChargeK not ready"); // this would happen before we have CLT reading for example
		return 0;
	}
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(map), "NaN map", 0);

	engine->engineState.sd.manifoldAirPressureAccelerationAdjustment = engine->engineLoadAccelEnrichment.getEngineLoadEnrichment(PASS_GLOBAL_SIGNATURE);

	float adjustedMap = engine->engineState.sd.adjustedManifoldAirPressure = map + engine->engineState.sd.manifoldAirPressureAccelerationAdjustment;
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(adjustedMap), "NaN adjustedMap", 0);

	float airMass = getCylinderAirMass(ENGINE(engineState.currentBaroCorrectedVE), adjustedMap, tChargeK PASS_GLOBAL_SUFFIX);
	if (cisnan(airMass)) {
		warning(CUSTOM_ERR_6685, "NaN airMass");
		return 0;
	}
#if EFI_PRINTF_FUEL_DETAILS
	printf("map=%.2f adjustedMap=%.2f airMass=%.2f\t\n",
			map, adjustedMap, engine->engineState.sd.adjustedManifoldAirPressure);
#endif /*EFI_PRINTF_FUEL_DETAILS */

	engine->engineState.sd.airMassInOneCylinder = airMass;
	return sdMath(airMass, ENGINE(engineState.targetAFR) PASS_GLOBAL_SUFFIX) * 1000;
}

// Default baro table is all 1.0, we can't recommend a reasonable default here
static const baro_corr_table_t default_baro_corr = {1};

void setDefaultVETable(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setRpmTableBin(config->veRpmBins, FUEL_RPM_COUNT);
	veMap.setAll(80);

//	setRpmTableBin(engineConfiguration->ve2RpmBins, FUEL_RPM_COUNT);
//	setLinearCurve(engineConfiguration->ve2LoadBins, FUEL_LOAD_COUNT, 10, 300, 1);
//	ve2Map.setAll(0.81);

	setRpmTableBin(config->afrRpmBins, FUEL_RPM_COUNT);
	afrMap.setAll(14.7);

	setRpmTableBin(engineConfiguration->baroCorrRpmBins, BARO_CORR_SIZE);
	setLinearCurve(engineConfiguration->baroCorrPressureBins, BARO_CORR_SIZE, 75, 105, 1);
	memcpy(engineConfiguration->baroCorrTable, default_baro_corr, sizeof(default_baro_corr));
}

void initSpeedDensity(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	veMap.init(config->veTable, config->veLoadBins, config->veRpmBins);
//	ve2Map.init(engineConfiguration->ve2Table, engineConfiguration->ve2LoadBins, engineConfiguration->ve2RpmBins);
	afrMap.init(config->afrTable, config->afrLoadBins, config->afrRpmBins);
	baroCorrMap.init(engineConfiguration->baroCorrTable, engineConfiguration->baroCorrPressureBins, engineConfiguration->baroCorrRpmBins);
	initMaf2Map();
}
