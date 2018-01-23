/**
 * @file daihatsu.cpp
 *
 * Daihatsu 3-Cylinder KF-VE
 * set engine_type 34
 *
 * @date Sep 10, 2015
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"
#include "daihatsu.h"

EXTERN_ENGINE;

void setDaihatsu(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engineConfiguration->trigger.type = TT_36_2_2_2;

	engineConfiguration->specs.cylindersCount = 3;
	engineConfiguration->specs.firingOrder = FO_1_2_3;

}
