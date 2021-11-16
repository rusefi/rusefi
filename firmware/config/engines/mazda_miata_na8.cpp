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

/**
 * 1994, 1995 NA
 * Note that ODB-II car have different wiring!
 *
 * https://rusefi.com/wiki/index.php?title=Vehicle:Mazda_Miata_1994
 *
 * See also setMiataNA_1_6_Configuration
 */
void setMazdaMiataNA8Configuration() {
	setMiataNA6_MAP_Frankenso();

	// blue jumper wire to W45 bottom jumper, not OEM
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_7;

	/**
	 * http://miataturbo.wikidot.com/fuel-injectors
	 * 94-97 (tan) - #195500-2180
	 */
	engineConfiguration->injector.flow = 265;

	engineConfiguration->specs.displacement = 1.839;
	strcpy(CONFIG(engineMake), ENGINE_MAKE_MAZDA);
	strcpy(CONFIG(engineCode), "NA8");


	engineConfiguration->vbattDividerCoeff = 9.75;// ((float) (8.2 + 33)) / 8.2 * 2;


	engineConfiguration->injectionPins[0] = GPIOD_3; // #1 pin 3U
	engineConfiguration->injectionPins[1] = GPIOE_2; // #2 pin 3V
	engineConfiguration->injectionPins[2] = GPIOB_8; // #3 pin 3Y
	engineConfiguration->injectionPins[3] = GPIOB_7; // #4 pin 3Z

	engineConfiguration->injectionMode = IM_SEQUENTIAL;
}



