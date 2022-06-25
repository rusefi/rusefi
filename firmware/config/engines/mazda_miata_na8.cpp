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
}
