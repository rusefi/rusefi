/**
 * @file    allsensors.cpp
 * @brief
 *
 *
 * @date Nov 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine.h"
#include "allsensors.h"

EXTERN_ENGINE;

void initSensors(Logging *sharedLogger DECLARE_ENGINE_PARAMETER_SUFFIX) {
	initMapDecoder(sharedLogger PASS_ENGINE_PARAMETER_SUFFIX);
}
}
