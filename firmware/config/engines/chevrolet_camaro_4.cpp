/*
 * @file chevrolet_camaro_4.cpp
 *
 *
 * set engine_type 35
 *
 * @date Oct 16, 2015
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "chevrolet_camaro_4.h"
#include "engine_math.h"
#include "custom_engine.h"


EXTERN_ENGINE
;

void setCamaro4(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);


	setAlgorithm(LM_SPEED_DENSITY PASS_ENGINE_PARAMETER_SUFFIX);

	engineConfiguration->specs.displacement = 5.7;
	engineConfiguration->specs.cylindersCount = 8;

	engineConfiguration->specs.firingOrder = FO_1_8_7_2_6_5_4_3;
	boardConfiguration->triggerInputPins[0] = GPIOA_5;
	boardConfiguration->triggerInputPins[1] = GPIOC_6;

	engineConfiguration->injectionMode = IM_BATCH;
	engineConfiguration->twoWireBatchInjection = true;
	// set ignition_mode 2
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;

	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_GM_LS_24;

	engineConfiguration->map.sensor.hwChannel = EFI_ADC_0; // PA0

	engineConfiguration->mafAdcChannel = EFI_ADC_NONE;


	engineConfiguration->tpsAdcChannel = EFI_ADC_2;

	// todo: move this into trigger definition
	// set global_trigger_offset_angle 230
	engineConfiguration->globalTriggerAngleOffset = 230;

	engineConfiguration->iat.adcChannel = EFI_ADC_11;

	engineConfiguration->clt.adcChannel = EFI_ADC_12;
	engineConfiguration->clt.config.bias_resistor = 2700;

	engineConfiguration->vbattAdcChannel = EFI_ADC_14;

	engineConfiguration->afr.hwChannel = EFI_ADC_13;


	boardConfiguration->idle.solenoidPin = GPIO_UNASSIGNED;
	boardConfiguration->fuelPumpPin = GPIO_UNASSIGNED;

	boardConfiguration->injectionPins[0] = GPIOE_6;
	boardConfiguration->injectionPins[1] = GPIOE_5;
	boardConfiguration->injectionPins[2] = GPIOD_7;
	boardConfiguration->injectionPins[3] = GPIOC_13;
	boardConfiguration->injectionPins[4] = GPIOE_3;
	boardConfiguration->injectionPins[5] = GPIOE_4;
	boardConfiguration->injectionPins[6] = GPIOD_3;
	boardConfiguration->injectionPins[7] = GPIOE_2;


	boardConfiguration->ignitionPins[0] = GPIOC_9;
	boardConfiguration->ignitionPins[1] = GPIOC_7;
	boardConfiguration->ignitionPins[2] = GPIOE_14;
	boardConfiguration->ignitionPins[3] = GPIOE_12;
	boardConfiguration->ignitionPins[4] = GPIOE_10;
	boardConfiguration->ignitionPins[5] = GPIOE_8;
	boardConfiguration->ignitionPins[6] = GPIOD_9;
	boardConfiguration->ignitionPins[7] = GPIOD_8;

	boardConfiguration->fuelPumpPin = GPIOB_8;
	boardConfiguration->fanPin = GPIO_UNASSIGNED;
	boardConfiguration->mainRelayPin = GPIOD_5;
}


