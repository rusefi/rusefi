/*
 * @file toyota_jzs147.cpp
 *
 * toyota aristo
 * https://en.wikipedia.org/wiki/Lexus_GS
 * http://rusefi.com/forum/viewtopic.php?f=15&t=969
 *
 * http://wilbo666.pbworks.com/w/page/37134472/2JZ-GTE%20JZS147%20Aristo%20Engine%20Wiring
 *
 * NON VVTi
 * set engine_type 38
 *
 * VVTi
 * set engine_type 44
 *
 * @date Dec 30, 2015
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "toyota_jzs147.h"
#include "custom_engine.h"
#include "thermistors.h"

EXTERN_ENGINE;

static void common2jz(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE); // default pinout

	engineConfiguration->specs.displacement = 3.0;
	engineConfiguration->specs.cylindersCount = 6;
	engineConfiguration->specs.firingOrder = FO_1_5_3_6_2_4;
	// set ignition_mode 1
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;

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

	// chartsize 450
	engineConfiguration->engineChartSize = 450;

	//	engineConfiguration->useOnlyRisingEdgeForTrigger = true;

	engineConfiguration->map.sensor.type = MT_CUSTOM;

	engineConfiguration->injector.flow = 430;

	/**
	 * http://thesafetyaversionsociety.com/wordpress/wp-content/uploads/2010/08/Troubleshooting-2JZ-GTE.pdf
	 * pages 44&46
	 */
	setThermistorConfiguration(&engineConfiguration->clt, -20, 15000, 40, 1200, 120.0, 200.0);
	setThermistorConfiguration(&engineConfiguration->iat, -20, 15000, 40, 1200, 120.0, 200.0);

}

void setToyota_jzs147EngineConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	common2jz(PASS_ENGINE_PARAMETER_SIGNATURE);

	setOperationMode(engineConfiguration, FOUR_STROKE_CAM_SENSOR);
	engineConfiguration->trigger.type = TT_2JZ_1_12;

//// temporary while I am fixing trigger bug
//	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
//	//set trigger_type 16
//	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;
//	engineConfiguration->trigger.customTotalToothCount = 36;
//	engineConfiguration->trigger.customSkippedToothCount = 2;
//
//	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
//	engineConfiguration->twoWireBatchIgnition = true;
//
//	engineConfiguration->crankingInjectionMode = IM_BATCH;
//	engineConfiguration->injectionMode = IM_BATCH;
//	engineConfiguration->twoWireBatchInjection = true;

//	boardConfiguration->triggerInputPins[0] = GPIOA_5;
//	boardConfiguration->triggerInputPins[1] = GPIOC_6;

	boardConfiguration->isSdCardEnabled = false;


}

void setToyota_2jz_vics(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	common2jz(PASS_ENGINE_PARAMETER_SIGNATURE);

	boardConfiguration->isSdCardEnabled = true;

	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_2JZ_3_34;

	boardConfiguration->triggerInputPins[0] = GPIOA_5; // crank sensor
	boardConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED; // cam sensor will he handled by custom vtti code

	engineConfiguration->camInput = GPIOC_6;
	engineConfiguration->vvtMode = VVT_2GZ;

	// set global_trigger_offset_angle 155
	engineConfiguration->globalTriggerAngleOffset = 155; // todo

	engineConfiguration->ignitionMode = IM_WASTED_SPARK; // just for now
	engineConfiguration->injectionMode = IM_BATCH; // just for now
	engineConfiguration->twoWireBatchIgnition = true;
	engineConfiguration->twoWireBatchInjection = true;



}


