/**
 * @file daihatsu.cpp
 *
 * @date Sep 10, 2015
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"
#include "daihatsu.h"

EXTERN_ENGINE;

void setDaihatsu(DECLARE_ENGINE_PARAMETER_F) {
	engineConfiguration->specs.cylindersCount = 3;
	engineConfiguration->specs.firingOrder = FO_1_THEN_2_THEN_3_THEN_4_THEN_5_THEN_6;

}
