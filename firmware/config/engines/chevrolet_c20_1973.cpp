/*
 * @file chevrolet_c20_1973.cpp
 *
 * 5.7L SBC
 * 4 wire IAC
 * 8 pin HEI module
 *
 * set_engine_type 37
 *
 *
 * @date Dec 20, 2015
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "chevrolet_c20_1973.h"

EXTERN_ENGINE;

void set1973c20(DECLARE_ENGINE_PARAMETER_F) {

	engineConfiguration->specs.displacement = 5.7;
	engineConfiguration->specs.cylindersCount = 8;

	setOperationMode(engineConfiguration, FOUR_STROKE_CAM_SENSOR);

	engineConfiguration->ignitionMode = IM_ONE_COIL;
	engineConfiguration->specs.firingOrder = FO_1_8_4_3_6_5_7_2;
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;

	/**
	 * 8/0 toothed wheel
	 */
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;
	engineConfiguration->trigger.customTotalToothCount = 8;
	engineConfiguration->trigger.customSkippedToothCount = 0;



}


