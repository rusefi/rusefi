/**
 * @file	GY6_139QMB.cpp
 * @brief	139qmb default engine configuration
 *
 * @date Feb 13, 2014
 * @author rus084, (c) 2015
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"
#include "GY6_139QMB.h"
#include "engine_math.h"
#include "engine_configuration.h"
#include "allsensors.h"
#include "advance_map.h"

EXTERN_ENGINE
;

static const fuel_table_t default_139qmb_fuel_table = {

{ 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
		6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
		6.000000, 6.000000 }, { 6.000000, 6.000000, 6.000000, 6.000000,
		6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
		6.000000, 6.000000, 6.000000, 6.000000, 6.000000 }, { 6.000000,
		6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
		6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
		6.000000 }, { 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
		6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
		6.000000, 6.000000, 6.000000, 6.000000 }, { 6.000000, 6.000000,
		6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
		6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000 },
		{ 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000 }, { 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000 }, { 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000, 6.000000, 6.000000 }, { 6.000000,
				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000 }, { 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000 }, { 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000, 6.000000, 6.000000 }, { 6.000000,
				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000 }, { 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000 }, { 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000, 6.000000, 6.000000 }, { 6.000000,
				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000 }, { 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000, 6.000000, 6.000000, 6.000000, 6.000000, 6.000000,
				6.000000 }

};

static void setDefault139qmbMaps(DECLARE_ENGINE_PARAMETER_SIGNATURE) {

	setFuelLoadBin(1.2, 4.4 PASS_ENGINE_PARAMETER_SUFFIX);
	setFuelRpmBin(1000, 11000 PASS_ENGINE_PARAMETER_SUFFIX);
	setTimingLoadBin(1.2, 4.4 PASS_ENGINE_PARAMETER_SUFFIX);
	setTimingRpmBin(1000, 11000 PASS_ENGINE_PARAMETER_SUFFIX);

	copyFuelTable(default_139qmb_fuel_table, config->fuelTable);
	//copyTimingTable(default_139qmb_timing_table, config->ignitionTable);
}

void setGy6139qmbDefaultEngineConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setDefault139qmbMaps(PASS_ENGINE_PARAMETER_SIGNATURE);
//        engineConfiguration->map.sensor.type = MT_3V_SENSOR;
	setEgoSensor(ES_NarrowBand PASS_ENGINE_PARAMETER_SUFFIX);

	engineConfiguration->rpmHardLimit = 9000;
	engineConfiguration->cranking.rpm = 1100;
	setTargetRpmCurve(2000 PASS_ENGINE_PARAMETER_SUFFIX);
	engineConfiguration->analogInputDividerCoefficient = 1;
	engineConfiguration->fuelAlgorithm = LM_MAP;
	engineConfiguration->globalTriggerAngleOffset = 45;
	boardConfiguration->sensorChartMode = SC_MAP;
	engineConfiguration->specs.displacement = 0.072; // 72cc
	engineConfiguration->specs.cylindersCount = 1;
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);

	engineConfiguration->specs.firingOrder = FO_1;

	/**
	 * We treat the trigger as 8-1 toothed wheel
	 */
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;
	engineConfiguration->trigger.customTotalToothCount = 8;
	engineConfiguration->trigger.customSkippedToothCount = 1;

	boardConfiguration->injectionPins[0] = GPIOC_9;
	boardConfiguration->ignitionPins[0] = GPIOC_8;

}
