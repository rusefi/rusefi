/**
 * @file	speed_density.cpp
 *
 * @date May 29, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
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

static Map3D1616 veMap;
static Map3D1616 afrMap;

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
 * @return value in seconds
 */
float sdMath(engine_configuration_s *engineConfiguration, float VE, float MAP, float AFR, float temp) {
	if (MAP < 0.001 || cisnan(MAP)) {
		warning(OBD_PCM_Processor_Fault, "invalid MAP value");
		return 0;
	}

	float injectorFlowRate = cc_minute_to_gramm_second(engineConfiguration->injectorFlow);
	float Vol = engineConfiguration->displacement / engineConfiguration->cylindersCount;
	return (Vol * VE * MAP) / (AFR * injectorFlowRate * GAS_R * temp);
}

/**
 * @return value in Milliseconds
 */
float getSpeedDensityFuel(Engine *engine, int rpm) {
	//int rpm = engine->rpmCalculator->rpm();

	engine_configuration_s *engineConfiguration = engine->engineConfiguration;

	float tps = getTPS();
	float coolantC = getCoolantTemperature(engine->engineConfiguration2);
	float intakeC = getIntakeAirTemperature(engine->engineConfiguration2);
	float tChargeK = convertCelsiusToKelvin(getTCharge(rpm, tps, coolantC, intakeC));
	float map = getMap();
	float VE = veMap.getValue(map, engineConfiguration->veLoadBins, rpm,
			engineConfiguration->veRpmBins);
	float AFR = afrMap.getValue(map, engineConfiguration->afrLoadBins, rpm,
			engineConfiguration->afrRpmBins);

	return sdMath(engine->engineConfiguration, VE, map, AFR, tChargeK) * 1000;
}

void setDetaultVETable(engine_configuration_s *engineConfiguration) {
	setRpmTableBin(engineConfiguration->veRpmBins, VE_RPM_COUNT);
	setTableBin2(engineConfiguration->veLoadBins, VE_LOAD_COUNT, 10, 300, 1);

	setRpmTableBin(engineConfiguration->afrRpmBins, AFR_RPM_COUNT);
	setTableBin2(engineConfiguration->afrLoadBins, VE_LOAD_COUNT, 10, 300, 1);

	veMap.setAll(0.8);
	afrMap.setAll(14.7);
}

void initSpeedDensity(engine_configuration_s *engineConfiguration) {
	veMap.init(engineConfiguration->veTable);
	afrMap.init(engineConfiguration->afrTable);
}
