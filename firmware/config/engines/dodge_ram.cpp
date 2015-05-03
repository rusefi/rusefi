/*
 * @file dodge_ram.cpp
 *
 * set_engine_type 31
 *
 * @date Apr 22, 2015
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"
#include "dodge_ram.h"

EXTERN_ENGINE
;

void setDodgeRam1996(DECLARE_ENGINE_PARAMETER_F) {
	board_configuration_s * boardConfiguration = &engineConfiguration->bc;

	setOperationMode(engineConfiguration, FOUR_STROKE_CAM_SENSOR);
	engineConfiguration->trigger.type = TT_DODGE_RAM;

	//Base engine setting
	engineConfiguration->specs.cylindersCount = 8;
	engineConfiguration->specs.firingOrder = FO_1_8_4_3_6_5_7_2;

	boardConfiguration->triggerInputPins[0] = GPIOC_6;
	boardConfiguration->triggerInputPins[1] = GPIOA_5;

	boardConfiguration->injectionPins[0] = GPIOD_3;
	boardConfiguration->injectionPins[1] = GPIOE_2;
	boardConfiguration->injectionPins[2] = GPIOE_3;
	boardConfiguration->injectionPins[3] = GPIOE_4;
	boardConfiguration->injectionPins[4] = GPIOD_5;
	boardConfiguration->injectionPins[5] = GPIOB_7;
	boardConfiguration->injectionPins[6] = GPIOB_8;
	boardConfiguration->injectionPins[7] = GPIOB_9;

	boardConfiguration->ignitionPins[0] = GPIOC_9;

	boardConfiguration->mainRelayPin = GPIOE_5;
	boardConfiguration->fuelPumpPin = GPIOE_6;

	engineConfiguration->vbattAdcChannel = EFI_ADC_NONE; // todo: conflict with what?
}
