/**
 * @file	ford_fiesta.cpp
 * @brief	European 1990 Ford Fiesta
 *
 * FORD_FIESTA = 4
 * set_engine_type 4
 *
 * @date Nov 22, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"

#if EFI_SUPPORT_FORD_FIESTA || defined(__DOXYGEN__)

#include "ford_fiesta.h"
#include "engine_configuration.h"
#include "engine_math.h"

void setFordFiestaDefaultEngineConfiguration(engine_configuration_s *engineConfiguration) {
	engineConfiguration->rpmHardLimit = 7000;
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);

	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_36_1;

	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->specs.firingOrder = FO_1_THEN_3_THEN_4_THEN2;
	engineConfiguration->hasMafSensor = true;
}

#endif /* EFI_SUPPORT_FORD_FIESTA */
