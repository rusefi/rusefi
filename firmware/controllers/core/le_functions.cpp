/**
 * @file le_functions.cpp
 *
 * @date Oct 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "le_functions.h"
#include "allsensors.h"
#include "rpm_calculator.h"

extern LENameOrdinalPair * LE_FIRST;

static LENameOrdinalPair leLess(LE_OPERATOR_LESS, "<");
static LENameOrdinalPair leLessEquals(LE_OPERATOR_LESS_OR_EQUAL, "<=");
static LENameOrdinalPair leMore(LE_OPERATOR_MORE, ">");
static LENameOrdinalPair leMoreEquals(LE_OPERATOR_MORE_OR_EQUAL, ">=");
static LENameOrdinalPair leRpm(LE_METHOD_RPM, "rpm");
static LENameOrdinalPair leTps(LE_METHOD_TPS, "tps");
static LENameOrdinalPair leMaf(LE_METHOD_MAF, "maf");
static LENameOrdinalPair leVBatt(LE_METHOD_VBATT, "vbatt");
static LENameOrdinalPair leFan(LE_METHOD_FAN, "fan");
static LENameOrdinalPair leCoolant(LE_METHOD_COOLANT, "coolant");
static LENameOrdinalPair leFanOnSetting(LE_METHOD_FAN_ON_SETTING, "fan_on_setting");
static LENameOrdinalPair leFanOffSetting(LE_METHOD_FAN_OFF_SETTING, "fan_off_setting");
static LENameOrdinalPair leTimeSinceBoot(LE_METHOD_TIME_SINCE_BOOT, "time_since_boot");

#if EFI_PROD_CODE || EFI_SIMULATOR
float getLEValue(Engine *engine, le_action_e action) {
	efiAssert(engine!=NULL, "getLEValue", NAN);
	switch (action) {
//	case LE_METHOD_FAN:
	//	return ;
	case LE_METHOD_COOLANT:
		return getCoolantTemperature(engine);
	case LE_METHOD_INTAKE_AIR:
		return getIntakeAirTemperature(engine);
	case LE_METHOD_RPM:
		return engine->rpmCalculator.rpm();
	case LE_METHOD_TIME_SINCE_BOOT:
		return getTimeNowSeconds();
	case LE_METHOD_FAN_OFF_SETTING:
		return engine->engineConfiguration->fanOffTemperature;
	case LE_METHOD_FAN_ON_SETTING:
		return engine->engineConfiguration->fanOnTemperature;
	case LE_METHOD_VBATT:
		return getVBatt(engine->engineConfiguration);
	default:
		firmwareError("No value for %d", action);
		return NAN;
	}
}
#endif
