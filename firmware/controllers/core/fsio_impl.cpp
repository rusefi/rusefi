/**
 * @file fsio_impl.cpp
 * @brief FSIO as it's used for GPIO
 *
 * set debug_mode 23
 * https://rusefi.com/wiki/index.php?title=Manual:Flexible_Logic
 *
 * 'fsioinfo' command in console shows current state of FSIO - formulas and current value
 *
 * @date Oct 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "fsio_impl.h"

#if EFI_PROD_CODE

// todo: that's about bench test mode, wrong header for sure!
#include "bench_test.h"
#endif // EFI_PROD_CODE

#if EFI_FSIO

#include "os_access.h"

/**
 * in case of zero frequency pin is operating as simple on/off. '1' for ON and '0' for OFF
 *
 */
#define NO_PWM 0

static fsio8_Map3D_f32t scriptTable1;
static fsio8_Map3D_u8t scriptTable2;
static fsio8_Map3D_u8t scriptTable3;
static fsio8_Map3D_u8t scriptTable4;

/**
 * Here we define all rusEfi-specific methods
 */
static LENameOrdinalPair leRpm(LE_METHOD_RPM, "rpm");
static LENameOrdinalPair leTps(LE_METHOD_TPS, "tps");
static LENameOrdinalPair lePps(LE_METHOD_PPS, "pps");
static LENameOrdinalPair leMaf(LE_METHOD_MAF, "maf");
static LENameOrdinalPair leMap(LE_METHOD_MAP, "map");
static LENameOrdinalPair leVBatt(LE_METHOD_VBATT, "vbatt");
static LENameOrdinalPair leFan(LE_METHOD_FAN, "fan");
static LENameOrdinalPair leCoolant(LE_METHOD_COOLANT, "coolant");
static LENameOrdinalPair leIntakeTemp(LE_METHOD_INTAKE_AIR, "iat");
static LENameOrdinalPair leIsCoolantBroken(LE_METHOD_IS_COOLANT_BROKEN, "is_clt_broken");
static LENameOrdinalPair leOilPressure(LE_METHOD_OIL_PRESSURE, "oilp");

// @returns boolean state of A/C toggle switch
static LENameOrdinalPair leAcToggle(LE_METHOD_AC_TOGGLE, "ac_on_switch");
// @returns float number of seconds since last A/C toggle
static LENameOrdinalPair leTimeSinceAcToggle(LE_METHOD_TIME_SINCE_AC_TOGGLE, "time_since_ac_on_switch");
static LENameOrdinalPair leTimeSinceBoot(LE_METHOD_TIME_SINCE_BOOT, "time_since_boot");
static LENameOrdinalPair leFsioSetting(LE_METHOD_FSIO_SETTING, FSIO_METHOD_FSIO_SETTING);
static LENameOrdinalPair leFsioAnalogInput(LE_METHOD_FSIO_ANALOG_INPUT, FSIO_METHOD_FSIO_ANALOG_INPUT);
static LENameOrdinalPair leFsioDigitalInput(LE_METHOD_FSIO_DIGITAL_INPUT, FSIO_METHOD_FSIO_DIGITAL_INPUT);
static LENameOrdinalPair leIntakeVVT(LE_METHOD_INTAKE_VVT, "ivvt");
static LENameOrdinalPair leExhaustVVT(LE_METHOD_EXHAUST_VVT, "evvt");
static LENameOrdinalPair leCrankingRpm(LE_METHOD_CRANKING_RPM, "cranking_rpm");
static LENameOrdinalPair leInShutdown(LE_METHOD_IN_SHUTDOWN, "in_shutdown");
static LENameOrdinalPair leInMrBench(LE_METHOD_IN_MR_BENCH, "in_mr_bench");
static LENameOrdinalPair leTimeSinceTrigger(LE_METHOD_TIME_SINCE_TRIGGER_EVENT, "time_since_trigger");
static LENameOrdinalPair leFuelRate(LE_METHOD_FUEL_FLOW_RATE, "fuel_flow");

#include "fsio_names.def"

#define SYS_ELEMENT_POOL_SIZE 24
#define UD_ELEMENT_POOL_SIZE 64

static LEElement sysElements[SYS_ELEMENT_POOL_SIZE] CCM_OPTIONAL;
CCM_OPTIONAL LEElementPool sysPool(sysElements, SYS_ELEMENT_POOL_SIZE);

static LEElement * starterRelayDisableLogic;

#if EFI_MAIN_RELAY_CONTROL
static LEElement * mainRelayLogic;
#endif /* EFI_MAIN_RELAY_CONTROL */

#if EFI_PROD_CODE || EFI_SIMULATOR

FsioResult getEngineValue(le_action_e action) {
	efiAssert(CUSTOM_ERR_ASSERT, engine!=NULL, "getLEValue", unexpected);
	switch (action) {
	case LE_METHOD_FAN:
		return enginePins.fanRelay.getLogicValue();
	case LE_METHOD_TIME_SINCE_AC_TOGGLE:
		return (getTimeNowUs() - engine->acSwitchLastChangeTime) / US_PER_SECOND_F;
	case LE_METHOD_AC_TOGGLE:
		return getAcToggle();
	case LE_METHOD_COOLANT:
		return Sensor::getOrZero(SensorType::Clt);
	case LE_METHOD_IS_COOLANT_BROKEN:
		return !Sensor::get(SensorType::Clt).Valid;
	case LE_METHOD_INTAKE_AIR:
		return Sensor::getOrZero(SensorType::Iat);
	case LE_METHOD_RPM:
		return Sensor::getOrZero(SensorType::Rpm);
	case LE_METHOD_MAF:
		return Sensor::getOrZero(SensorType::Maf);
	case LE_METHOD_MAP:
		return Sensor::getOrZero(SensorType::Map);
#if EFI_SHAFT_POSITION_INPUT
	case LE_METHOD_INTAKE_VVT:
		return engine->triggerCentral.getVVTPosition(0, 0);
	case LE_METHOD_EXHAUST_VVT:
		return engine->triggerCentral.getVVTPosition(0, 1);
#endif
	case LE_METHOD_TIME_SINCE_TRIGGER_EVENT:
		return engine->triggerCentral.getTimeSinceTriggerEvent(getTimeNowNt());
	case LE_METHOD_TIME_SINCE_BOOT:
#if EFI_MAIN_RELAY_CONTROL
		// in main relay control mode, we return the number of seconds since the ignition is turned on
		// (or negative if the ignition key is switched off)
		return engine->getTimeIgnitionSeconds();
#else
		return getTimeNowSeconds();
#endif /* EFI_MAIN_RELAY_CONTROL */
	case LE_METHOD_CRANKING_RPM:
		return engineConfiguration->cranking.rpm;
	case LE_METHOD_IN_SHUTDOWN:
		return engine->isInShutdownMode();
	case LE_METHOD_IN_MR_BENCH:
		return engine->isInMainRelayBench();
	case LE_METHOD_VBATT:
		return Sensor::getOrZero(SensorType::BatteryVoltage);
	case LE_METHOD_TPS:
		return Sensor::getOrZero(SensorType::DriverThrottleIntent);
	case LE_METHOD_FUEL_FLOW_RATE:
		return engine->engineState.fuelConsumption.getConsumptionGramPerSecond();
	case LE_METHOD_OIL_PRESSURE:
		return Sensor::getOrZero(SensorType::OilPressure);
	// cfg_xxx references are code generated
#include "fsio_getters.def"
	default:
		warning(CUSTOM_FSIO_UNEXPECTED, "FSIO ERROR no data for action=%d", action);
		return unexpected;
	}
}

#endif

void onConfigurationChangeFsioCallback(engine_configuration_s *previousConfiguration) {
	(void)previousConfiguration;
}

static LECalculator calc CCM_OPTIONAL;

static const char * action2String(le_action_e action) {
	static char buffer[_MAX_FILLER];
	switch(action) {
		case LE_METHOD_RPM:
			return "RPM";
		case LE_METHOD_CRANKING_RPM:
			return "cranking_rpm";
		case LE_METHOD_COOLANT:
			return "CLT";
		case LE_METHOD_FAN:
			return "fan";
		case LE_METHOD_IN_SHUTDOWN:
			return leInShutdown.name;
		case LE_METHOD_IN_MR_BENCH:
			return leInMrBench.name;
#include "fsio_strings.def"

		default: {
			// this is here to make compiler happy
		}
	}
	itoa10(buffer, (int)action);
	return buffer;
}

static void setPinState(const char * msg, OutputPin *pin, LEElement *element) {
#if EFI_PROD_CODE
	if (isRunningBenchTest()) {
		return; // let's not mess with bench testing
	}
#endif /* EFI_PROD_CODE */

	if (!element) {
		warning(CUSTOM_FSIO_INVALID_EXPRESSION, "invalid expression for %s", msg);
	} else {
		int value = (int)calc.evaluate(msg, pin->getLogicValue(), element);
		if (pin->isInitialized() && value != pin->getLogicValue()) {

			for (int i = 0;i < calc.currentCalculationLogPosition;i++) {
				efiPrintf("calc %d: action %s value %.2f", i, action2String(calc.calcLogAction[i]), calc.calcLogValue[i]);
			}

			efiPrintf("setPin %s %s", msg, value ? "on" : "off");
			pin->setValue(value);
		}
	}
}

/**
 * this method should be invoked periodically to calculate FSIO and toggle corresponding FSIO outputs
 */
void runFsio() {
#if EFI_MAIN_RELAY_CONTROL
	if (isBrainPinValid(engineConfiguration->mainRelayPin))
		// the MAIN_RELAY_LOGIC calls engine->isInShutdownMode()
		setPinState("main_relay", &enginePins.mainRelay, mainRelayLogic);
#else /* EFI_MAIN_RELAY_CONTROL */
	/**
	 * main relay is always on if ECU is on, that's a good enough initial implementation
	 */
	if (isBrainPinValid(engineConfiguration->mainRelayPin))
		enginePins.mainRelay.setValue(!engine->isInMainRelayBench());
#endif /* EFI_MAIN_RELAY_CONTROL */

	if (isBrainPinValid(engineConfiguration->starterRelayDisablePin))
		setPinState("starter_relay", &enginePins.starterRelayDisable, starterRelayDisableLogic);

	/**
	 * o2 heater is off during cranking
	 * todo: convert to FSIO?
	 * open question if heater should be ON during cranking
	 */
	enginePins.o2heater.setValue(engine->rpmCalculator.isRunning());
}


static void showFsio(const char *msg, LEElement *element) {
#if EFI_PROD_CODE || EFI_SIMULATOR
	if (msg != NULL)
		efiPrintf("%s:", msg);
	while (element->action != LE_METHOD_RETURN) {
		efiPrintf("action %d: fValue=%.2f", element->action, element->fValue);
		element++;
	}
	efiPrintf("<end>");
#endif
}

// todo: move somewhere else
static void showFsioInfo() {
#if EFI_PROD_CODE || EFI_SIMULATOR
	for (int i = 0; i < SCRIPT_SETTING_COUNT; i++) {
		float v = engineConfiguration->scriptSetting[i];
		if (!cisnan(v)) {
			efiPrintf("user property #%d: %.2f", i + 1, v);
		}
	}
#endif
}

ValueProvider3D *getscriptTable(int index) {
	switch (index) {
	default:
		return &scriptTable1;
	case 1:
		return &scriptTable2;
	case 2:
		return &scriptTable3;
	case 3:
		return &scriptTable4;
	}
}

// todo: template this copy-pasta
/**
 * @return zero-based index of curve with given name
 */
int getCurveIndexByName(const char *name) {
	for (int i = 0;i<SCRIPT_CURVE_COUNT;i++) {
		if (strEqualCaseInsensitive(name, engineConfiguration->scriptCurveName[i])) {
			return i;
		}
	}
	return EFI_ERROR_CODE;
}

int getTableIndexByName(const char *name) {
	for (int i = 0;i<SCRIPT_TABLE_COUNT;i++) {
		if (strEqualCaseInsensitive(name, engineConfiguration->scriptTableName[i])) {
			return i;
		}
	}
	return EFI_ERROR_CODE;
}

int getSettingIndexByName(const char *name) {
	for (int i = 0;i<SCRIPT_SETTING_COUNT;i++) {
		if (strEqualCaseInsensitive(name, engineConfiguration->scriptSettingName[i])) {
			return i;
		}
	}
	return EFI_ERROR_CODE;
}

float getCurveValue(int index, float key) {
	// not great code at all :(
	switch (index) {
	default:
		return interpolate2d(key, engineConfiguration->scriptCurve1Bins, engineConfiguration->scriptCurve1);
	case 1:
		return interpolate2d(key, engineConfiguration->scriptCurve2Bins, engineConfiguration->scriptCurve2);
	case 2:
		return interpolate2d(key, engineConfiguration->scriptCurve3Bins, engineConfiguration->scriptCurve3);
	case 3:
		return interpolate2d(key, engineConfiguration->scriptCurve4Bins, engineConfiguration->scriptCurve4);
	case 4:
		return interpolate2d(key, engineConfiguration->scriptCurve5Bins, engineConfiguration->scriptCurve5);
	case 5:
		return interpolate2d(key, engineConfiguration->scriptCurve6Bins, engineConfiguration->scriptCurve6);
	}
}

void initFsioImpl() {
#if EFI_UNIT_TEST
	// only unit test needs this
	sysPool.reset();
#endif

#if EFI_MAIN_RELAY_CONTROL
	if (isBrainPinValid(engineConfiguration->mainRelayPin))
		mainRelayLogic = sysPool.parseExpression(MAIN_RELAY_LOGIC);
#endif /* EFI_MAIN_RELAY_CONTROL */
	if (isBrainPinValid(engineConfiguration->starterRelayDisablePin))
		starterRelayDisableLogic = sysPool.parseExpression(STARTER_RELAY_LOGIC);

	scriptTable1.init(config->scriptTable1, config->scriptTable1LoadBins,
			config->scriptTable1RpmBins);
	scriptTable2.init(config->scriptTable2, config->scriptTable2LoadBins,
			config->scriptTable2RpmBins);
	scriptTable3.init(config->scriptTable3, config->scriptTable3LoadBins,
			config->scriptTable3RpmBins);
	scriptTable4.init(config->scriptTable4, config->scriptTable4LoadBins,
			config->scriptTable4RpmBins);

}

#else /* !EFI_FSIO */

// "Limp-mode" implementation for some RAM-limited configs without FSIO
void runHardcodedFsio() {
#if EFI_PROD_CODE
	if (isRunningBenchTest()) {
		return; // let's not mess with bench testing
	}
#endif /* EFI_PROD_CODE */

	// see MAIN_RELAY_LOGIC
	if (isBrainPinValid(engineConfiguration->mainRelayPin)) {
		enginePins.mainRelay.setValue((getTimeNowSeconds() < 2) || (Sensor::getOrZero(SensorType::BatteryVoltage) > LOW_VBATT) || engine->isInShutdownMode());
	}
	// see STARTER_RELAY_LOGIC
	if (isBrainPinValid(engineConfiguration->starterRelayDisablePin)) {
		enginePins.starterRelayDisable.setValue(engine->rpmCalculator.getRpm() < engineConfiguration->cranking.rpm);
	}

	enginePins.o2heater.setValue(engine->rpmCalculator.isRunning());
}

#endif /* EFI_FSIO */
