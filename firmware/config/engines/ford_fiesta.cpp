/**
 * @file	ford_fiesta.cpp
 * @brief	European 1990 Ford Fiesta
 *
 * FORD_FIESTA = 4
 *
 * @date Nov 22, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"

#if EFI_SUPPORT_FORD_FIESTA || defined(__DOXYGEN__)

#include "ford_fiesta.h"
#include "engine_configuration.h"
#include "engine_math.h"

void setFordFiestaDefaultEngineConfiguration(engine_configuration_s *engineConfiguration) {
	engineConfiguration->rpmHardLimit = 7000;
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);

	engineConfiguration->triggerConfig.triggerType = TT_TOOTHED_WHEEL_36_1;

	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->firingOrder = FO_1_THEN_3_THEN_4_THEN2;
}

#endif /* EFI_SUPPORT_FORD_FIESTA */
