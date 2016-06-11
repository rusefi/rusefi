/*
 * @file toyota_jzs147.cpp
 *
 * toyota aristo
 * https://en.wikipedia.org/wiki/Lexus_GS
 * http://rusefi.com/forum/viewtopic.php?f=15&t=969
 *
 * http://wilbo666.pbworks.com/w/page/37134472/2JZ-GTE%20JZS147%20Aristo%20Engine%20Wiring
 *
 * set_engine_type 38
 *
 * @date Dec 30, 2015
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "toyota_jzs147.h"
#include "custom_engine.h"

EXTERN_ENGINE;

void setToyota_jzs147EngineConfiguration(DECLARE_ENGINE_PARAMETER_F) {
	setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_F); // default pinout

	engineConfiguration->isCanEnabled = true;

	engineConfiguration->specs.displacement = 3.0;
	engineConfiguration->specs.cylindersCount = 6;
	engineConfiguration->specs.firingOrder = FO_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4;
	// set_ignition_mode 1
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;

	setOperationMode(engineConfiguration, FOUR_STROKE_CAM_SENSOR);
	engineConfiguration->trigger.type = TT_2JZ;

	boardConfiguration->ignitionPins[0] = GPIOE_14;
	boardConfiguration->ignitionPins[1] = GPIOC_7;
	boardConfiguration->ignitionPins[2] = GPIOC_9;
	boardConfiguration->ignitionPins[3] = GPIOE_10;
	boardConfiguration->ignitionPins[4] = GPIOE_8;
	boardConfiguration->ignitionPins[5] = GPIOE_12;


	boardConfiguration->injectionPins[0] = GPIOB_9; // #1
	boardConfiguration->injectionPins[1] = GPIOE_2; // #2
	boardConfiguration->injectionPins[2] = GPIOB_8; // #3
	boardConfiguration->injectionPins[3] = GPIOB_7; // #4
	boardConfiguration->injectionPins[4] = GPIOE_3; // #5
	boardConfiguration->injectionPins[5] = GPIOE_4; // #6

	boardConfiguration->fuelPumpPin = GPIO_UNASSIGNED;

}



