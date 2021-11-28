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
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "toyota_jzs147.h"
#include "custom_engine.h"
#include "mazda_miata_vvt.h"

static void common2jz() {
	setFrankensoConfiguration(); // default pinout

	engineConfiguration->specs.displacement = 3.0;
	engineConfiguration->specs.cylindersCount = 6;
	engineConfiguration->specs.firingOrder = FO_1_5_3_6_2_4;
	// set ignition_mode 1
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;

	engineConfiguration->ignitionPins[0] = GPIOE_14;
	engineConfiguration->ignitionPins[1] = GPIOC_7;
	engineConfiguration->ignitionPins[2] = GPIOC_9;
	engineConfiguration->ignitionPins[3] = GPIOE_10;
	engineConfiguration->ignitionPins[4] = GPIOE_8;
	engineConfiguration->ignitionPins[5] = GPIOE_12;


	engineConfiguration->injectionPins[0] = GPIOB_9; // #1
	engineConfiguration->injectionPins[1] = GPIOE_2; // #2
	engineConfiguration->injectionPins[2] = GPIOB_8; // #3
	engineConfiguration->injectionPins[3] = GPIOB_7; // #4
	engineConfiguration->injectionPins[4] = GPIOE_3; // #5
	engineConfiguration->injectionPins[5] = GPIOE_4; // #6

	engineConfiguration->fuelPumpPin = GPIO_UNASSIGNED;

	// chartsize 450
	engineConfiguration->engineChartSize = 450;

	//	engineConfiguration->useOnlyRisingEdgeForTrigger = true;

	engineConfiguration->map.sensor.type = MT_CUSTOM;

	engineConfiguration->injector.flow = 430;

	/**
	 * http://thesafetyaversionsociety.com/wordpress/wp-content/uploads/2010/08/Troubleshooting-2JZ-GTE.pdf
	 * pages 44&46
	 */
	engineConfiguration->clt.config = {-20, 40, 120.0, 15000, 1200, 200.0, 2700};
	engineConfiguration->iat.config = {-20, 40, 120.0, 15000, 1200, 200.0, 2700};

}

void setToyota_jzs147EngineConfiguration() {
	common2jz();

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

//	engineConfiguration->triggerInputPins[0] = GPIOA_5;
//	engineConfiguration->triggerInputPins[1] = GPIOC_6;

	engineConfiguration->isSdCardEnabled = false;


}

/**
 * TOYOTA_2JZ_GTE_VVTi
 * set engine_type 44
 */
void setToyota_2jz_vics() {
	common2jz();

	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_36_2;

	engineConfiguration->triggerInputPins[0] = GPIOA_5; // crank sensor
	engineConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED; // cam sensor will he handled by custom vtti code

	engineConfiguration->camInputs[0] = GPIOC_6;
	engineConfiguration->vvtMode[0] = VVT_2JZ;

	// set global_trigger_offset_angle 155
	engineConfiguration->globalTriggerAngleOffset = 155; // todo

	engineConfiguration->ignitionMode = IM_WASTED_SPARK; // just for now
	engineConfiguration->injectionMode = IM_BATCH; // just for now
	engineConfiguration->twoWireBatchIgnition = true;
	engineConfiguration->twoWireBatchInjection = true;

	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_TOYOTA);
	strcpy(engineConfiguration->engineCode, "2JZ");
	strcpy(engineConfiguration->vehicleName, "VVT example");


	engineConfiguration->debugMode = DBG_VVT;

	// todo: these magic values would be hardcoded once we find out proper magic values
//	engineConfiguration->fsio_setting[14] = 175 - 45;
//	engineConfiguration->fsio_setting[15] = 175 + 45;

	engineConfiguration->vvtPins[0] = GPIOE_3; // VVT solenoid control

	// Mazda VVT settings have nothing to do wit Toyota 2JZ settings but those are a good starting point for settings
	setMazdaNB2VVTSettings();
}


