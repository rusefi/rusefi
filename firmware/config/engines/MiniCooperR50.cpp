/**
 * @file MiniCooperR50.cpp
 *
 * MINI_COOPER_R50 = 13
 *
 * @date Apr 9, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "MiniCooperR50.h"

void setMiniCooperR50(engine_configuration_s *engineConfiguration) {
	engineConfiguration->trigger.type = TT_MINI_COOPER_R50;

}
