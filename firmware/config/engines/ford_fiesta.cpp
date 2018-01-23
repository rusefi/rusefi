/**
 * @file	ford_fiesta.cpp
 * @brief	European 1990 Ford Fiesta
 *
 * FORD_FIESTA = 4
 * set engine_type 4
 *
 * @date Nov 22, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"

#if EFI_SUPPORT_FORD_FIESTA || defined(__DOXYGEN__)

#include "ford_fiesta.h"
#include "engine_math.h"

EXTERN_ENGINE;

void setFordFiestaDefaultEngineConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engineConfiguration->rpmHardLimit = 7000;
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);

	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_36_1;

	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;
	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;
	engineConfiguration->mafAdcChannel = EFI_ADC_14;
//	engineConfiguration->mafAdcChannel = EFI_ADC_NONE; this would kill functional tests


	// etb testing
//	boardConfiguration->clutchUpPin
	boardConfiguration->etbControlPin1 = GPIOD_3;
	engineConfiguration->pedalPositionChannel = EFI_ADC_1;
	boardConfiguration->etbDirectionPin2 = GPIOD_5;

	engineConfiguration->tpsMin = 337;
	engineConfiguration->tpsMax = 896;
}

#endif /* EFI_SUPPORT_FORD_FIESTA */
