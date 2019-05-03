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

#include "global.h"

#if EFI_SUPPORT_FORD_FIESTA

#include "ford_fiesta.h"
#include "engine_math.h"

EXTERN_CONFIG;

void setFordFiestaDefaultEngineConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engineConfiguration->rpmHardLimit = 7000;
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);

	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_36_1;

	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;
	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;
	engineConfiguration->mafAdcChannel = EFI_ADC_14;
//	engineConfiguration->mafAdcChannel = EFI_ADC_NONE; this would kill functional tests
}

#endif /* EFI_SUPPORT_FORD_FIESTA */
