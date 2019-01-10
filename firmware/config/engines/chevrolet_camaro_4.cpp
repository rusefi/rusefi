/*
 * @file chevrolet_camaro_4.cpp
 *
 * CAMARO_4
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
	CONFIGB(triggerInputPins)[0] = GPIOA_5;
	CONFIGB(triggerInputPins)[1] = GPIOC_6;

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


	CONFIGB(idle).solenoidPin = GPIO_UNASSIGNED;
	CONFIGB(fuelPumpPin) = GPIO_UNASSIGNED;

	CONFIGB(injectionPins)[0] = GPIOE_6;
	CONFIGB(injectionPins)[1] = GPIOE_5;
	CONFIGB(injectionPins)[2] = GPIOD_7;
	CONFIGB(injectionPins)[3] = GPIOC_13;
	CONFIGB(injectionPins)[4] = GPIOE_3;
	CONFIGB(injectionPins)[5] = GPIOE_4;
	CONFIGB(injectionPins)[6] = GPIOD_3;
	CONFIGB(injectionPins)[7] = GPIOE_2;


	CONFIGB(ignitionPins)[0] = GPIOC_9;
	CONFIGB(ignitionPins)[1] = GPIOC_7;
	CONFIGB(ignitionPins)[2] = GPIOE_14;
	CONFIGB(ignitionPins)[3] = GPIOE_12;
	CONFIGB(ignitionPins)[4] = GPIOE_10;
	CONFIGB(ignitionPins)[5] = GPIOE_8;
	CONFIGB(ignitionPins)[6] = GPIOD_9;
	CONFIGB(ignitionPins)[7] = GPIOD_8;

	CONFIGB(fuelPumpPin) = GPIOB_8;
	CONFIGB(fanPin) = GPIO_UNASSIGNED;
	CONFIGB(mainRelayPin) = GPIOD_5;
}


