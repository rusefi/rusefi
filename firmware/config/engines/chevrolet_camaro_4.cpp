/*
 * @file chevrolet_camaro_4.cpp
 *
 * CAMARO_4
 * set engine_type 35
 *
 * @date Oct 16, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "chevrolet_camaro_4.h"
#include "custom_engine.h"

void setCamaro4() {
	setFrankensoConfiguration();


	setAlgorithm(LM_SPEED_DENSITY);

	engineConfiguration->specs.displacement = 5.7;
	engineConfiguration->specs.cylindersCount = 8;
	strcpy(CONFIG(engineMake), ENGINE_MAKE_GM);

	engineConfiguration->specs.firingOrder = FO_1_8_7_2_6_5_4_3;
	engineConfiguration->triggerInputPins[0] = GPIOA_5;
	engineConfiguration->triggerInputPins[1] = GPIOC_6;

	engineConfiguration->injectionMode = IM_BATCH;
	engineConfiguration->twoWireBatchInjection = true;
	// set ignition_mode 2
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;

	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_GM_LS_24;

	engineConfiguration->map.sensor.hwChannel = EFI_ADC_0; // PA0

	engineConfiguration->mafAdcChannel = EFI_ADC_NONE;


	engineConfiguration->tps1_1AdcChannel = EFI_ADC_2;

	// todo: move this into trigger definition
	// set global_trigger_offset_angle 50
	engineConfiguration->globalTriggerAngleOffset = 50;

	engineConfiguration->iat.adcChannel = EFI_ADC_11;

	engineConfiguration->clt.adcChannel = EFI_ADC_12;
	engineConfiguration->clt.config.bias_resistor = 2700;

	engineConfiguration->vbattAdcChannel = EFI_ADC_14;

	engineConfiguration->afr.hwChannel = EFI_ADC_13;


	engineConfiguration->idle.solenoidPin = GPIO_UNASSIGNED;
	engineConfiguration->fuelPumpPin = GPIO_UNASSIGNED;

	engineConfiguration->injectionPins[0] = GPIOE_6;
	engineConfiguration->injectionPins[1] = GPIOE_5;
	engineConfiguration->injectionPins[2] = GPIOD_7;
	engineConfiguration->injectionPins[3] = GPIOC_13;
	engineConfiguration->injectionPins[4] = GPIOE_3;
	engineConfiguration->injectionPins[5] = GPIOE_4;
	engineConfiguration->injectionPins[6] = GPIOD_3;
	engineConfiguration->injectionPins[7] = GPIOE_2;


	engineConfiguration->ignitionPins[0] = GPIOC_9;
	engineConfiguration->ignitionPins[1] = GPIOC_7;
	engineConfiguration->ignitionPins[2] = GPIOE_14;
	engineConfiguration->ignitionPins[3] = GPIOE_12;
	engineConfiguration->ignitionPins[4] = GPIOE_10;
	engineConfiguration->ignitionPins[5] = GPIOE_8;
	engineConfiguration->ignitionPins[6] = GPIOD_9;
	engineConfiguration->ignitionPins[7] = GPIOD_8;

	engineConfiguration->fuelPumpPin = GPIOB_8;
	engineConfiguration->fanPin = GPIO_UNASSIGNED;
	engineConfiguration->mainRelayPin = GPIOD_5;
}


