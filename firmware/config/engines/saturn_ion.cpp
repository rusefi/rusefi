/**
 * @file saturn_ion.cpp
 *
 * SATURN_ION_2004 = 12
 *
 * 7x GM DIS trigger
 *
 * @date Mar 28, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "saturn_ion.h"

void setSaturnIonEngineConfiguration(engine_configuration_s *engineConfiguration) {
	engineConfiguration->trigger.type = TT_GM_7X;
}
