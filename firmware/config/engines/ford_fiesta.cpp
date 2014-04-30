/**
 * @file	ford_fiesta.c
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
	// only crankshaft sensor so far
	engineConfiguration->rpmMultiplier = 1;

	engineConfiguration->triggerConfig.totalToothCount = 36;
	engineConfiguration->triggerConfig.skippedToothCount = 1;

	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->firingOrder = FO_1_THEN_3_THEN_4_THEN2;
}

#endif /* EFI_SUPPORT_FORD_FIESTA */
