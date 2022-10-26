/*
 * gm_ls_4.cpp
 *
 */

#include "gm_ls_4.h"
#include "defaults.h"

void setGmLs4() {
	engineConfiguration->globalTriggerAngleOffset = 86;

	engineConfiguration->fuelReferencePressure = 400; // 400 kPa, 58 psi
	engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;
	engineConfiguration->injector.flow = 440;

	engineConfiguration->specs.cylindersCount = 8;
	setLeftRightBanksNeedBetterName();
	engineConfiguration->specs.firingOrder = FO_1_8_7_2_6_5_4_3;
	engineConfiguration->specs.displacement = 6.2;

	engineConfiguration->useETBforIdleControl = true;
	engineConfiguration->etbIdleThrottleRange = 15;

	engineConfiguration->tChargeAirIncrLimit = 5;
	engineConfiguration->tChargeAirDecrLimit = 15;
}
