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
#include "efiGpio.h"

//extern LENameOrdinalPair * LE_FIRST;

/**
 * Here we define all rusEfi-specific methods
 */
static LENameOrdinalPair leRpm(LE_METHOD_RPM, "rpm");
static LENameOrdinalPair leTps(LE_METHOD_TPS, "tps");
static LENameOrdinalPair leMaf(LE_METHOD_MAF, "maf");
static LENameOrdinalPair leVBatt(LE_METHOD_VBATT, "vbatt");
static LENameOrdinalPair leFan(LE_METHOD_FAN, "fan");
static LENameOrdinalPair leCoolant(LE_METHOD_COOLANT, "coolant");
static LENameOrdinalPair leAcToggle(LE_METHOD_AC_TOGGLE, "ac_on_switch");
static LENameOrdinalPair leFanOnSetting(LE_METHOD_FAN_ON_SETTING, "fan_on_setting");
static LENameOrdinalPair leFanOffSetting(LE_METHOD_FAN_OFF_SETTING, "fan_off_setting");
static LENameOrdinalPair leTimeSinceBoot(LE_METHOD_TIME_SINCE_BOOT, "time_since_boot");
static LENameOrdinalPair leFsioSsetting(LE_METHOD_FSIO_SETTING, "fsio_setting");

#if EFI_PROD_CODE || EFI_SIMULATOR
float getLEValue(Engine *engine, calc_stack_t *s, le_action_e action) {
	efiAssert(engine!=NULL, "getLEValue", NAN);
	switch (action) {
	case LE_METHOD_FAN:
		return getOutputPinValue(FAN_RELAY);
	case LE_METHOD_AC_TOGGLE:
		return getAcToggle(engine);
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

void setFsio(engine_configuration_s *engineConfiguration, int index, brain_pin_e pin, const char * exp) {
	board_configuration_s *boardConfiguration = &engineConfiguration->bc;

	boardConfiguration->fsioPins[index] = pin;
	strcpy(boardConfiguration->le_formulas[index], exp);
}
