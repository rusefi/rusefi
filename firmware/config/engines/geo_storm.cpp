/*
 * @file geo_storm.h
 *
 * @date Mar 26, 2016
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "geo_storm.h"
#include "custom_engine.h"

EXTERN_ENGINE;

void setGeoStormConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
}

