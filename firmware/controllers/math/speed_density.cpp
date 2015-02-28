/**
 * @file	speed_density.cpp
 *
 * See http://rusefi.com/wiki/index.php?title=Manual:Software:Fuel_Control#Speed_Density for details
 *
 * @date May 29, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"
#include "speed_density.h"
#include "interpolation.h"
#include "rpm_calculator.h"
#include "engine_math.h"
#include "engine_state.h"

#define K_AT_MIN_RPM_MIN_TPS 0.25
#define K_AT_MIN_RPM_MAX_TPS 0.25
#define K_AT_MAX_RPM_MIN_TPS 0.25
#define K_AT_MAX_RPM_MAX_TPS 0.9

#define rpmMin 500
#define rpmMax 8000

Map3D1616 veMap;
Map3D1616 ve2Map;
Map3D1616 afrMap;

#define tpMin 0
#define tpMax 100
//  http://rusefi.com/math/t_charge.html
float getTCharge(int rpm, float tps, float coolantTemp, float airTemp) {
	float minRpmKcurrentTPS = interpolate(tpMin, K_AT_MIN_RPM_MIN_TPS, tpMax,
	K_AT_MIN_RPM_MAX_TPS, tps);
	float maxRpmKcurrentTPS = interpolate(tpMin, K_AT_MAX_RPM_MIN_TPS, tpMax,
	K_AT_MAX_RPM_MAX_TPS, tps);

	float Tcharge_coff = interpolate(rpmMin, minRpmKcurrentTPS, rpmMax, maxRpmKcurrentTPS, rpm);

	float Tcharge = coolantTemp * (1 - Tcharge_coff) + airTemp * Tcharge_coff;

	return Tcharge;
}

/**
 * is J/g*K
 */
#define GAS_R 0.28705

/**
 * @return per cylinder injection time, in seconds
 */
float sdMath(engine_configuration_s *engineConfiguration, float VE, float MAP, float AFR, float tempK) {
	if (MAP < 0.001 || cisnan(MAP)) {
		warning(OBD_PCM_Processor_Fault, "invalid MAP value");
		return 0;
	}

	/**
	 * todo: pre-calculate gramm/second injector flow to save one multiplication
	 * open question if that's needed since that's just a multiplication
	 */
	float injectorFlowRate = cc_minute_to_gramm_second(engineConfiguration->injector.flow);
	// todo: pre-calculate cylinder displacement to save one division
	float cylinderDisplacement = engineConfiguration->specs.displacement / engineConfiguration->specs.cylindersCount;
	float airMass = (cylinderDisplacement * VE * MAP) / (GAS_R * tempK);
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
float getSpeedDensityFuel(int rpm DECLARE_ENGINE_PARAMETER_S) {
	//int rpm = engine->rpmCalculator->rpm();

	float tps = getTPS(PASS_ENGINE_PARAMETER_F);
	float coolantC = engine->engineState.clt;
	float intakeC = engine->engineState.iat;
	float tChargeK = convertCelsiusToKelvin(getTCharge(rpm, tps, coolantC, intakeC));
	float map = getMap() + engine->accelEnrichment.getEnrichment(PASS_ENGINE_PARAMETER_F);
	float VE = veMap.getValue(map, rpm);
	float AFR = afrMap.getValue(map, rpm);

	return sdMath(engineConfiguration, VE, map, AFR, tChargeK) * 1000;
}

void setDetaultVETable(DECLARE_ENGINE_PARAMETER_F) {
	setRpmTableBin(config->veRpmBins, FUEL_RPM_COUNT);
	setTableBin2(config->veLoadBins, FUEL_LOAD_COUNT, 10, 300, 1);
	veMap.setAll(0.8);

	setRpmTableBin(config->ve2RpmBins, FUEL_RPM_COUNT);
	setTableBin2(config->ve2LoadBins, FUEL_LOAD_COUNT, 10, 300, 1);
	ve2Map.setAll(0.81);

	setRpmTableBin(config->afrRpmBins, FUEL_RPM_COUNT);
	setTableBin2(config->afrLoadBins, FUEL_LOAD_COUNT, 10, 300, 1);
	afrMap.setAll(14.7);
}

void initSpeedDensity(DECLARE_ENGINE_PARAMETER_F) {
	efiAssertVoid(config!=NULL, "config is NULL");
	engine_configuration_s *e = &config->engineConfiguration;
	veMap.init(config->veTable, config->veLoadBins, config->veRpmBins);
	ve2Map.init(config->ve2Table, config->ve2LoadBins, config->ve2RpmBins);
	afrMap.init(config->afrTable, config->afrLoadBins, config->afrRpmBins);
}
