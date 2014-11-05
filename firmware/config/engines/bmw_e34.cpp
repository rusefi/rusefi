/**
 * @file bmw_e34.cpp
 *
 * set_engine_type 25
 *
 * @date Nov 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "global.h"
#include "bmw_e34.h"

void setBmwE43(engine_configuration_s *engineConfiguration) {
	engineConfiguration->rpmHardLimit = 6000;
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);

	engineConfiguration->triggerConfig.triggerType = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->cylindersCount = 6;

}
