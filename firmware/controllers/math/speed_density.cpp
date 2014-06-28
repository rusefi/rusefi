/**
 * @file	speed_density.cpp
 *
 * @date May 29, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "speed_density.h"
#include "interpolation.h"
#include "engine.h"
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
float getTCharge(int rpm, int tps, float coolantTemp, float airTemp) {
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

float sdMath(engine_configuration_s *engineConfiguration, float VE, float MAP, float AFR, float temp) {
	float injectorFlowRate = cc_minute_to_gramm_second(engineConfiguration->injectorFlow);
	float Vol = engineConfiguration->displacement / engineConfiguration->cylindersCount;
	return (Vol * VE * MAP) / (AFR * injectorFlowRate * GAS_R * temp);
}

float getSpeedDensityFuel(Engine *engine) {
	int rpm = engine->rpmCalculator->rpm();

	float tps = getTPS();
	float coolantC = getCoolantTemperature();
	float intakeC = getIntakeAirTemperature();
	float tChargeK = convertCelciusToKelvin(getTCharge(rpm, tps, coolantC, intakeC));
	float MAP = getMap();
	float VE = 0.8;//veMap.getValue(rpm)
	float AFR = 14.7;

	return sdMath(engine->engineConfiguration, VE, MAP, AFR, tChargeK);

}

void setDetaultVETable(engine_configuration_s *engineConfiguration) {
	setTableBin(engineConfiguration->veRpmBins, FUEL_RPM_COUNT, 800, 7000);

	veMap.setAll(0.8);
}

void initSpeedDensity(configuration_s *config) {
	veMap.init(config->engineConfiguration->veTable);
	afrMap.init(config->engineConfiguration->afrTable);
}
