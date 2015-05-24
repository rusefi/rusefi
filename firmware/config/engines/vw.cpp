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

	setOperationMode(engineConfiguration, FOUR_STROKE_CAM_SENSOR);

}


