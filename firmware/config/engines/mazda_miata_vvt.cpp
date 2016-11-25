/*
 * @file	mazda_miata_vvt.cpp
 *
 * set_engine_type 47
 *
 * @date Oct 4, 2016
 * @author Andrey Belomutskiy, (c) 2012-2016
 * http://rusefi.com/forum/viewtopic.php?f=3&t=1095
 */

#include "mazda_miata_vvt.h"
#include "custom_engine.h"

EXTERN_ENGINE;

void setMazdaMiata2003EngineConfiguration(DECLARE_ENGINE_PARAMETER_F) {
	setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_F);

	engineConfiguration->hasMapSensor = true;

	engineConfiguration->trigger.type = TT_MIATA_VVT;
	setOperationMode(engineConfiguration, FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR);
	engineConfiguration->specs.displacement = 1.8;

	boardConfiguration->triggerInputPins[0] = GPIOA_5;
	boardConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->camInput = GPIOC_6;

	// set vvt_mode 3
	engineConfiguration->vvtMode = MIATA_NB2;
	boardConfiguration->vvtCamSensorUseRise = true;
	engineConfiguration->vvtOffset = -70;

//	boardConfiguration->fuelPumpPin = GPIOD_5; // todo: rewire my board to default E_4

	// set_global_trigger_offset_angle 670
	engineConfiguration->globalTriggerAngleOffset = 670;

	// enable trigger_details
	engineConfiguration->isPrintTriggerSynchDetails = false;

	// todo: change to 15?
	// set_cranking_timing_angle 0
	engineConfiguration->crankingTimingAngle = 3;
	// set_cranking_fuel 9
	engineConfiguration->cranking.baseFuel = 9;

	boardConfiguration->ignitionPins[0] = GPIOE_14;
	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[2] = GPIOC_9;
	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;


//	0.0825
//	0.1375
//	6.375
//	10.625
	boardConfiguration->nb2ratioFrom = 8.50 * 0.75;
	boardConfiguration->nb2ratioTo = 14;
	engineConfiguration->nbVvtIndex = 0;


	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
}


