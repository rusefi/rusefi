/*
 * @file geo_storm.h
 *
 * @date Mar 26, 2016
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "geo_storm.h"
#include "custom_engine.h"

EXTERN_CONFIG;

void setGeoStormConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
}

