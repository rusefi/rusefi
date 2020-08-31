/*
 * @file vw_b6.cpp
 *
 * @date Dec 26, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine.h"
#include "vw_b6.h"

EXTERN_CONFIG;

/**
 * set engine_type 62
 * VW_B6
 */
void setVwPassatB6(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->vvtMode = VVT_BOSCH_QUICK_START;

	strcpy(CONFIG(engineMake), ENGINE_MAKE_VAG);
	strcpy(CONFIG(engineCode), "BPY");


	engineConfiguration->verboseVVTDecoding = true;
	engineConfiguration->invertCamVVTSignal = true;
	engineConfiguration->vvtCamSensorUseRise = true;
}
