/*
 * @file suzuki_vitara.cpp
 *
 * set engine_type 36
 *
 * @author Andrey Belomutskiy, (c) 2012-2018
 * @date Oct 17, 2015
 */

#include "suzuki_vitara.h"

EXTERN_ENGINE;

void setSuzukiVitara(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engineConfiguration->trigger.type = TT_MAZDA_SOHC_4;
	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.displacement = 1.590;

	engineConfiguration->ignitionMode = IM_ONE_COIL;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;

	boardConfiguration->mainRelayPin = GPIOE_6;
}


