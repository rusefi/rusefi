/*
 * @file mazda_miata_na8.cpp
 *
 * set engine_type 56
 *
 * MAZDA_MIATA_NA8
 *
 * @date Jan 31, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "mazda_miata_na8.h"
#include "custom_engine.h"
#include "mazda_miata_1_6.h"
#include "table_helper.h"

void setMazdaMiata96() {
	miataNAcommonEngineSettings();
	/**
	 * http://miataturbo.wikidot.com/fuel-injectors
	 * 94-97 (tan) - #195500-2180
	 */
	engineConfiguration->injector.flow = 265;
	engineConfiguration->cylinderBore = 83;

	engineConfiguration->idleMode = IM_AUTO;
	// below 20% this valve seems to be opening for fail-safe idle air
	engineConfiguration->idleRpmPid.pFactor = 0.0065;
	engineConfiguration->idleRpmPid.iFactor = 0.3;
	engineConfiguration->idle_derivativeFilterLoss = 0.08;
	engineConfiguration->idle_antiwindupFreq = 0.03;
	engineConfiguration->idleRpmPid.dFactor = 0.002;
	engineConfiguration->idleRpmPid.minValue = -20;
	engineConfiguration->idleRpmPid.maxValue = 20;
	engineConfiguration->idlerpmpid_iTermMin = -15;
	engineConfiguration->idlerpmpid_iTermMax = 30;
	engineConfiguration->acIdleExtraOffset = 15;
	engineConfiguration->idlePidRpmDeadZone = 100;
	engineConfiguration->idlePidRpmUpperLimit = 350;

	engineConfiguration->iacByTpsTaper = 6;
	engineConfiguration->useIdleTimingPidControl = true;

	engineConfiguration->wwaeTau = 0.1;
	setTable(config->alternatorVoltageTargetTable, 14.2);
	engineConfiguration->crankingIACposition = 36;
	engineConfiguration->afterCrankingIACtaperDuration = 189;

	engineConfiguration->crankingTimingAngle = 6;
	engineConfiguration->ignitionDwellForCrankingMs = 4;
	engineConfiguration->displayLogicLevelsInEngineSniffer = true;

	engineConfiguration->displacement = 1.839;
	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_MAZDA);
	strcpy(engineConfiguration->engineCode, "NA8");

	engineConfiguration->injectionMode = IM_SEQUENTIAL;

  engineConfiguration->map.sensor.type = MT_MPXH6400;
}
