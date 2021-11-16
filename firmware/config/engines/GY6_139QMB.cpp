/**
 * @file	GY6_139QMB.cpp
 * @brief	139qmb default engine configuration
 *
 * 4-stroke one cylinder
 *
 * set engine_type 8
 *
 * @date Feb 13, 2014
 * @author rus084, (c) 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "GY6_139QMB.h"
#include "advance_map.h"

static void setDefault139qmbMaps() {
	setTimingLoadBin(1.2, 4.4);
	setTimingRpmBin(1000, 11000);
}

void setGy6139qmbDefaultEngineConfiguration() {
	setDefault139qmbMaps();
//        engineConfiguration->map.sensor.type = MT_3V_SENSOR;
	setEgoSensor(ES_NarrowBand);

	engineConfiguration->rpmHardLimit = 9000;
	engineConfiguration->cranking.rpm = 1100;
	setTargetRpmCurve(2000);
	engineConfiguration->analogInputDividerCoefficient = 1;
	engineConfiguration->globalTriggerAngleOffset = 45;
	engineConfiguration->sensorChartMode = SC_MAP;
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

	engineConfiguration->injectionPins[0] = GPIOC_9;
	engineConfiguration->ignitionPins[0] = GPIOC_8;

	// set injection_mode 1
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	// set ignition_mode 0
	engineConfiguration->ignitionMode = IM_ONE_COIL;
}
