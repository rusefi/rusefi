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

static void commonNA8() {
	/**
	 * http://miataturbo.wikidot.com/fuel-injectors
	 * 94-97 (tan) - #195500-2180
	 */
	engineConfiguration->injector.flow = 265;
	engineConfiguration->useInstantRpmForIdle = true;
	engineConfiguration->cylinderBore = 83;
	engineConfiguration->knockBandCustom = 13.81;

	engineConfiguration->idleMode = IM_AUTO;
	// below 20% this valve seems to be opening for fail-safe idle air
	engineConfiguration->idleRpmPid.pFactor = 0.0065;
	engineConfiguration->idleRpmPid.iFactor = 0.3;
	engineConfiguration->idle_derivativeFilterLoss = 0.08;
	engineConfiguration->idle_antiwindupFreq = 0.03;
	engineConfiguration->idleRpmPid.dFactor = 0.002;
	engineConfiguration->idleRpmPid.minValue = 76;
	engineConfiguration->idlerpmpid_iTermMin = -15;
	engineConfiguration->idlerpmpid_iTermMax = 30;
	engineConfiguration->acIdleExtraOffset = 15;
	engineConfiguration->idlePidRpmDeadZone = 100;
	engineConfiguration->idlePidRpmUpperLimit = 350;

	engineConfiguration->iacByTpsTaper = 6;
	engineConfiguration->useIdleTimingPidControl = true;
	engineConfiguration->engineChartSize = 300;
	engineConfiguration->silentTriggerError = true;
	engineConfiguration->wwaeTau = 0.1;
	engineConfiguration->targetVBatt = 14.2;
	engineConfiguration->crankingIACposition = 36;
	engineConfiguration->afterCrankingIACtaperDuration = 189;

	engineConfiguration->crankingTimingAngle = 6;
	engineConfiguration->ignitionDwellForCrankingMs = 4;
	engineConfiguration->displayLogicLevelsInEngineSniffer = true;

	engineConfiguration->specs.displacement = 1.839;
	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_MAZDA);
	strcpy(engineConfiguration->engineCode, "NA8");

	engineConfiguration->injectionMode = IM_SEQUENTIAL;
}

/**
 * 1994, 1995 NA
 * Note that ODB-II car have different wiring!
 *
 * https://rusefi.com/wiki/index.php?title=Vehicle:Mazda_Miata_1994
 *
 * See also setMiataNA_1_6_Configuration
 */
void setFrankensoMazdaMiataNA8Configuration() {
	setMiataNA6_MAP_Frankenso();
	commonNA8();

	// blue jumper wire to W45 bottom jumper, not OEM
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_7;


	engineConfiguration->vbattDividerCoeff = 9.75;// ((float) (8.2 + 33)) / 8.2 * 2;


	engineConfiguration->injectionPins[0] = Gpio::D3; // #1 pin 3U
	engineConfiguration->injectionPins[1] = Gpio::E2; // #2 pin 3V
	engineConfiguration->injectionPins[2] = Gpio::B8; // #3 pin 3Y
	engineConfiguration->injectionPins[3] = Gpio::B7; // #4 pin 3Z
}

void setHellenMiata96() {
	miataNAcommonEngineSettings();
	commonNA8();
    engineConfiguration->map.sensor.type = MT_MPXH6400;
}
