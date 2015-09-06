/**
 * @file dodge_stratus.cpp
 *
 * set_engine_type 33
 *
 * @date Sep 5, 2015
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"
#include "dodge_stratus.h"

EXTERN_ENGINE
;

void setDodgeStratus(DECLARE_ENGINE_PARAMETER_F) {
	engineConfiguration->trigger.type = TT_DODGE_STRATUS;
	engineConfiguration->specs.cylindersCount = 6;
	engineConfiguration->specs.firingOrder = FO_1_THEN_2_THEN_3_THEN_4_THEN_5_THEN_6;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;

	boardConfiguration->ignitionPins[0] = GPIOC_9;
	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[2] = GPIOE_8;
	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[4] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[5] = GPIO_UNASSIGNED;

}


