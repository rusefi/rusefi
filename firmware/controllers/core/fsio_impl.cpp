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

static fsio8_Map3D_f32t fsioTable1;
static fsio8_Map3D_u8t fsioTable2;
static fsio8_Map3D_u8t fsioTable3;
static fsio8_Map3D_u8t fsioTable4;

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
static LENameOrdinalPair leFsioTable(LE_METHOD_FSIO_TABLE, FSIO_METHOD_FSIO_TABLE);
static LENameOrdinalPair leFsioAnalogInput(LE_METHOD_FSIO_ANALOG_INPUT, FSIO_METHOD_FSIO_ANALOG_INPUT);
static LENameOrdinalPair leFsioDigitalInput(LE_METHOD_FSIO_DIGITAL_INPUT, FSIO_METHOD_FSIO_DIGITAL_INPUT);
static LENameOrdinalPair leIntakeVVT(LE_METHOD_INTAKE_VVT, "ivvt");
static LENameOrdinalPair leExhaustVVT(LE_METHOD_EXHAUST_VVT, "evvt");
static LENameOrdinalPair leCrankingRpm(LE_METHOD_CRANKING_RPM, "cranking_rpm");
static LENameOrdinalPair leStartupFuelPumpDuration(LE_METHOD_STARTUP_FUEL_PUMP_DURATION, "startup_fuel_pump_duration");
static LENameOrdinalPair leInShutdown(LE_METHOD_IN_SHUTDOWN, "in_shutdown");
static LENameOrdinalPair leInMrBench(LE_METHOD_IN_MR_BENCH, "in_mr_bench");
static LENameOrdinalPair leTimeSinceTrigger(LE_METHOD_TIME_SINCE_TRIGGER_EVENT, "time_since_trigger");
static LENameOrdinalPair leFuelRate(LE_METHOD_FUEL_FLOW_RATE, "fuel_flow");

#include "fsio_names.def"

#define SYS_ELEMENT_POOL_SIZE 24
#define UD_ELEMENT_POOL_SIZE 64

static LEElement sysElements[SYS_ELEMENT_POOL_SIZE] CCM_OPTIONAL;
CCM_OPTIONAL LEElementPool sysPool(sysElements, SYS_ELEMENT_POOL_SIZE);

static LEElement userElements[UD_ELEMENT_POOL_SIZE] CCM_OPTIONAL;

class FsioPointers {
public:
	FsioPointers();
	LEElement * fsioLogics[FSIO_COMMAND_COUNT];
};

FsioPointers::FsioPointers() : fsioLogics() {
}

static FsioPointers state;

static LEElement * fuelPumpLogic;
static LEElement * starterRelayDisableLogic;

#if EFI_MAIN_RELAY_CONTROL
static LEElement * mainRelayLogic;
#endif /* EFI_MAIN_RELAY_CONTROL */

#if EFI_PROD_CODE || EFI_SIMULATOR

FsioResult getEngineValue(le_action_e action DECLARE_ENGINE_PARAMETER_SUFFIX) {
	efiAssert(CUSTOM_ERR_ASSERT, engine!=NULL, "getLEValue", unexpected);
	switch (action) {
	case LE_METHOD_FAN:
		return enginePins.fanRelay.getLogicValue();
	case LE_METHOD_TIME_SINCE_AC_TOGGLE:
		return (getTimeNowUs() - engine->acSwitchLastChangeTime) / US_PER_SECOND_F;
	case LE_METHOD_AC_TOGGLE:
		return getAcToggle(PASS_ENGINE_PARAMETER_SIGNATURE);
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
	case LE_METHOD_STARTUP_FUEL_PUMP_DURATION:
		return engineConfiguration->startUpFuelPumpDuration;

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

void onConfigurationChangeFsioCallback(engine_configuration_s *previousConfiguration DECLARE_ENGINE_PARAMETER_SUFFIX) {
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
		case LE_METHOD_STARTUP_FUEL_PUMP_DURATION:
			return leStartupFuelPumpDuration.name;
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

static void setPinState(const char * msg, OutputPin *pin, LEElement *element DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_PROD_CODE
	if (isRunningBenchTest()) {
		return; // let's not mess with bench testing
	}
#endif /* EFI_PROD_CODE */

	if (!element) {
		warning(CUSTOM_FSIO_INVALID_EXPRESSION, "invalid expression for %s", msg);
	} else {
		int value = (int)calc.evaluate(msg, pin->getLogicValue(), element PASS_ENGINE_PARAMETER_SUFFIX);
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
 * @param out param! current and new value as long as element is not NULL
 * @return 'true' if value has changed
 */
static bool updateValueOrWarning(int humanIndex, const char *msg, float *value DECLARE_ENGINE_PARAMETER_SUFFIX) {
	int fsioIndex = humanIndex - 1;
	LEElement * element = state.fsioLogics[fsioIndex];
	if (element == NULL) {
		warning(CUSTOM_FSIO_INVALID_EXPRESSION, "invalid expression for %s", msg);
		return false;
	} else {
		float beforeValue = *value;
		*value = calc.evaluate(msg, beforeValue, element PASS_ENGINE_PARAMETER_SUFFIX);
		// floating '==' comparison without EPS seems fine here
		return (beforeValue != *value);
	}
}

/**
 * this method should be invoked periodically to calculate FSIO and toggle corresponding FSIO outputs
 */
void runFsio(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_FUEL_PUMP
	if (isBrainPinValid(CONFIG(fuelPumpPin))) {
		setPinState("pump", &enginePins.fuelPumpRelay, fuelPumpLogic PASS_ENGINE_PARAMETER_SUFFIX);
	}
#endif /* EFI_FUEL_PUMP */

#if EFI_MAIN_RELAY_CONTROL
	if (isBrainPinValid(CONFIG(mainRelayPin)))
		// the MAIN_RELAY_LOGIC calls engine->isInShutdownMode()
		setPinState("main_relay", &enginePins.mainRelay, mainRelayLogic PASS_ENGINE_PARAMETER_SUFFIX);
#else /* EFI_MAIN_RELAY_CONTROL */
	/**
	 * main relay is always on if ECU is on, that's a good enough initial implementation
	 */
	if (isBrainPinValid(CONFIG(mainRelayPin)))
		enginePins.mainRelay.setValue(!engine->isInMainRelayBench(PASS_ENGINE_PARAMETER_SIGNATURE));
#endif /* EFI_MAIN_RELAY_CONTROL */

	if (isBrainPinValid(CONFIG(starterRelayDisablePin)))
		setPinState("starter_relay", &enginePins.starterRelayDisable, starterRelayDisableLogic PASS_ENGINE_PARAMETER_SUFFIX);

	/**
	 * o2 heater is off during cranking
	 * todo: convert to FSIO?
	 * open question if heater should be ON during cranking
	 */
	enginePins.o2heater.setValue(engine->rpmCalculator.isRunning());

#if EFI_ENABLE_ENGINE_WARNING
	if (engineConfiguration->useFSIO4ForSeriousEngineWarning) {
		updateValueOrWarning(MAGIC_OFFSET_FOR_ENGINE_WARNING, "eng warning", &ENGINE(fsioState.isEngineWarning) PASS_ENGINE_PARAMETER_SUFFIX);
	}
#endif /* EFI_ENABLE_ENGINE_WARNING */

#if EFI_ENABLE_CRITICAL_ENGINE_STOP
	if (engineConfiguration->useFSIO5ForCriticalIssueEngineStop) {
		bool changed = updateValueOrWarning(MAGIC_OFFSET_FOR_CRITICAL_ENGINE, "eng critical", &ENGINE(fsioState.isCriticalEngineCondition) PASS_ENGINE_PARAMETER_SUFFIX);
		if (changed && float2bool(ENGINE(fsioState.isCriticalEngineCondition))) {
			doScheduleStopEngine(PASS_ENGINE_PARAMETER_SIGNATURE);
		}
	}
#endif /* EFI_ENABLE_CRITICAL_ENGINE_STOP */
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
static void showFsioInfo(void) {
#if EFI_PROD_CODE || EFI_SIMULATOR
	showFsio("fuel", fuelPumpLogic);

	for (int i = 0; i < SCRIPT_SETTING_COUNT; i++) {
		float v = CONFIG(scriptSetting)[i];
		if (!cisnan(v)) {
			efiPrintf("user property #%d: %.2f", i + 1, v);
		}
	}
#endif
}

ValueProvider3D *getFSIOTable(int index) {
	switch (index) {
	default:
		return &fsioTable1;
	case 1:
		return &fsioTable2;
	case 2:
		return &fsioTable3;
	case 3:
		return &fsioTable4;
	}
}

/**
 * @return zero-based index of curve with given name
 */
int getCurveIndexByName(const char *name DECLARE_ENGINE_PARAMETER_SUFFIX) {
	for (int i = 0;i<SCRIPT_CURVE_COUNT;i++) {
		if (strEqualCaseInsensitive(name, engineConfiguration->scriptCurveName[i])) {
			return i;
		}
	}
	return EFI_ERROR_CODE;
}

int getTableIndexByName(const char *name DECLARE_ENGINE_PARAMETER_SUFFIX) {
	for (int i = 0;i<SCRIPT_TABLE_COUNT;i++) {
		if (strEqualCaseInsensitive(name, engineConfiguration->scriptTableName[i])) {
			return i;
		}
	}
	return EFI_ERROR_CODE;
}

float getCurveValue(int index, float key DECLARE_ENGINE_PARAMETER_SUFFIX) {
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

void initFsioImpl(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_UNIT_TEST
	// only unit test needs this
	sysPool.reset();
#endif

#if EFI_FUEL_PUMP
	fuelPumpLogic = sysPool.parseExpression(FUEL_PUMP_LOGIC);
#endif /* EFI_FUEL_PUMP */

#if EFI_MAIN_RELAY_CONTROL
	if (isBrainPinValid(CONFIG(mainRelayPin)))
		mainRelayLogic = sysPool.parseExpression(MAIN_RELAY_LOGIC);
#endif /* EFI_MAIN_RELAY_CONTROL */
	if (isBrainPinValid(CONFIG(starterRelayDisablePin)))
		starterRelayDisableLogic = sysPool.parseExpression(STARTER_RELAY_LOGIC);

	fsioTable1.init(config->fsioTable1, config->fsioTable1LoadBins,
			config->fsioTable1RpmBins);
	fsioTable2.init(config->fsioTable2, config->fsioTable2LoadBins,
			config->fsioTable2RpmBins);
	fsioTable3.init(config->fsioTable3, config->fsioTable3LoadBins,
			config->fsioTable3RpmBins);
	fsioTable4.init(config->fsioTable4, config->fsioTable4LoadBins,
			config->fsioTable4RpmBins);

}

#else /* !EFI_FSIO */

// "Limp-mode" implementation for some RAM-limited configs without FSIO
void runHardcodedFsio(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_PROD_CODE
	if (isRunningBenchTest()) {
		return; // let's not mess with bench testing
	}
#endif /* EFI_PROD_CODE */

	// see MAIN_RELAY_LOGIC
	if (isBrainPinValid(CONFIG(mainRelayPin))) {
		enginePins.mainRelay.setValue((getTimeNowSeconds() < 2) || (Sensor::getOrZero(SensorType::BatteryVoltage) > LOW_VBATT) || engine->isInShutdownMode());
	}
	// see STARTER_RELAY_LOGIC
	if (isBrainPinValid(CONFIG(starterRelayDisablePin))) {
		enginePins.starterRelayDisable.setValue(engine->rpmCalculator.getRpm() < engineConfiguration->cranking.rpm);
	}
	// see FUEL_PUMP_LOGIC
	if (isBrainPinValid(CONFIG(fuelPumpPin))) {
		int triggerActivityOrEcuStartSecond = maxI(0, engine->triggerActivityMs / 1000);

		enginePins.fuelPumpRelay.setValue((getTimeNowSeconds() < triggerActivityOrEcuStartSecond + engineConfiguration->startUpFuelPumpDuration) || (engine->rpmCalculator.getRpm() > 0));
	}
	
	enginePins.o2heater.setValue(engine->rpmCalculator.isRunning());
}

#endif /* EFI_FSIO */
