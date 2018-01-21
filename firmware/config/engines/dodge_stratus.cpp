/**
 * @file dodge_stratus.cpp
 *
 * set engine_type 33
 *
 * @date Sep 5, 2015
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"
#include "dodge_stratus.h"
#include "engine_math.h"

EXTERN_ENGINE
;

void setDodgeStratus(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engineConfiguration->trigger.type = TT_DODGE_STRATUS;
	engineConfiguration->specs.cylindersCount = 6;
	engineConfiguration->specs.firingOrder = FO_1_2_3_4_5_6;

	engineConfiguration->map.sensor.type = MT_DODGE_NEON_2003;
	setAlgorithm(LM_SPEED_DENSITY PASS_ENGINE_PARAMETER_SUFFIX);
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;

	boardConfiguration->ignitionPins[0] = GPIOC_9;
	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[2] = GPIOE_8;
	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[4] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[5] = GPIO_UNASSIGNED;

}


