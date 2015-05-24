/*
 * @file vw.cpp
 *
 * set_engine_type 32
 *
 * @date May 24, 2015
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"
#include "vw.h"
#include "custom_engine.h"

EXTERN_ENGINE;

void setVwAba(DECLARE_ENGINE_PARAMETER_F) {
	setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_F);

//	setOperationMode(engineConfiguration, FOUR_STROKE_CAM_SENSOR);

	//Base engine setting
	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.displacement = 2.5;

	engineConfiguration->ignitionMode = IM_ONE_COIL;

	boardConfiguration->ignitionPins[0] = GPIOE_14; // Frankenso high side - pin 1G
	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[2] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPinMode = OM_DEFAULT;


}


