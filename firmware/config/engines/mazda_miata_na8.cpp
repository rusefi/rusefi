/*
 * @file mazda_miata_na8.cpp
 *
 * set engine_type 56
 *
 * MAZDA_MIATA_NA8
 *
 * @date Jan 31, 2018
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "mazda_miata_na8.h"
#include "custom_engine.h"
#include "mazda_miata_1_6.h"

EXTERN_ENGINE;

/**
 * See also setMiataNA_1_6_Configuration
 */
void setMazdaMiataNA8Configuration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);

	// blue jumper wire to W45 bottom jumper, not OEM
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_7;

	/**
	 * http://miataturbo.wikidot.com/fuel-injectors
	 * 94-97 (tan) - #195500-2180
	 */
	engineConfiguration->injector.flow = 265;

	engineConfiguration->specs.displacement = 1.839;


	engineConfiguration->vbattDividerCoeff = 9.75;// ((float) (8.2 + 33)) / 8.2 * 2;


	CONFIGB(injectionPins)[0] = GPIOD_3; // #1 pin 3U
	CONFIGB(injectionPins)[1] = GPIOE_2; // #2 pin 3V
	CONFIGB(injectionPins)[2] = GPIOB_8; // #3 pin 3Y
	CONFIGB(injectionPins)[3] = GPIOB_7; // #4 pin 3Z

	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	miataNAcommon(PASS_ENGINE_PARAMETER_SIGNATURE);

}



