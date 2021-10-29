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
CCM_OPTIONAL LEElementPool userPool(userElements, UD_ELEMENT_POOL_SIZE);

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


#if EFI_PROD_CODE

static void setFsioAnalogInputPin(const char *indexStr, const char *pinName) {
// todo: reduce code duplication between all "set pin methods"
	int index = atoi(indexStr) - 1;
	if (index < 0 || index >= AUX_ANALOG_INPUT_COUNT) {
		efiPrintf("invalid FSIO index: %d", index);
		return;
	}
	brain_pin_e pin = parseBrainPin(pinName);
	// todo: extract method - code duplication with other 'set_xxx_pin' methods?
	if (pin == GPIO_INVALID) {
		efiPrintf("invalid pin name [%s]", pinName);
		return;
	}
	engineConfiguration->auxAnalogInputs[index] = (adc_channel_e) pin;
	efiPrintf("FSIO analog input pin #%d [%s]", (index + 1), hwPortname(pin));
}

static void setFsioDigitalInputPin(const char *indexStr, const char *pinName) {
	// todo: reduce code duplication between all "set pin methods"
	int index = atoi(indexStr) - 1;
	if (index < 0 || index >= FSIO_COMMAND_COUNT) {
		efiPrintf("invalid FSIO index: %d", index);
		return;
	}
	brain_pin_e pin = parseBrainPin(pinName);
	// todo: extract method - code duplication with other 'set_xxx_pin' methods?
	if (pin == GPIO_INVALID) {
		efiPrintf("invalid pin name [%s]", pinName);
		return;
	}
	CONFIG(fsioDigitalInputs)[index] = pin;
	efiPrintf("FSIO digital input pin #%d [%s]", (index + 1), hwPortname(pin));
}

static void setFsioPidOutputPin(const char *indexStr, const char *pinName) {
	int index = atoi(indexStr) - 1;
	if (index < 0 || index >= CAM_INPUTS_COUNT) {
		efiPrintf("invalid VVT index: %d", index);
		return;
	}
	brain_pin_e pin = parseBrainPin(pinName);
	// todo: extract method - code duplication with other 'set_xxx_pin' methods?
	if (pin == GPIO_INVALID) {
		efiPrintf("invalid pin name [%s]", pinName);
		return;
	}
	engineConfiguration->vvtPins[index] = pin;
	efiPrintf("VVT pid pin #%d [%s]", (index + 1), hwPortname(pin));
}

static void showFsioInfo(void);

static void setFsioOutputPin(const char *indexStr, const char *pinName) {
	int index = atoi(indexStr) - 1;
	if (index < 0 || index >= FSIO_COMMAND_COUNT) {
		efiPrintf("invalid FSIO index: %d", index);
		return;
	}
	brain_pin_e pin = parseBrainPin(pinName);
	// todo: extract method - code duplication with other 'set_xxx_pin' methods?
	if (pin == GPIO_INVALID) {
		efiPrintf("invalid pin name [%s]", pinName);
		return;
	}
	CONFIG(fsioOutputPins)[index] = pin;
	efiPrintf("FSIO output pin #%d [%s]", (index + 1), hwPortname(pin));
	efiPrintf("please writeconfig and reboot for pin to take effect");
	showFsioInfo();
}
#endif /* EFI_PROD_CODE */

#endif

/**
 * index is between zero and LE_COMMAND_LENGTH-1
 */
void setFsioExt(int index, brain_pin_e pin, const char * formula, int pwmFrequency DECLARE_CONFIG_PARAMETER_SUFFIX) {
	CONFIG(fsioOutputPins)[index] = pin;
	int len = strlen(formula);
	if (len >= LE_COMMAND_LENGTH) {
		return;
	}
	strcpy(config->fsioFormulas[index], formula);
	CONFIG(fsioFrequency)[index] = pwmFrequency;
}

void setFsio(int index, brain_pin_e pin, const char * exp DECLARE_CONFIG_PARAMETER_SUFFIX) {
	setFsioExt(index, pin, exp, NO_PWM PASS_CONFIG_PARAMETER_SUFFIX);
}

void applyFsioConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	userPool.reset();
	for (int i = 0; i < FSIO_COMMAND_COUNT; i++) {
		const char *formula = config->fsioFormulas[i];
		int len = strlen(formula);
		LEElement *logic = userPool.parseExpression(formula);
		if (len > 0 && logic == NULL) {
			warning(CUSTOM_FSIO_PARSING, "parsing [%s]", formula);
		}

		state.fsioLogics[i] = logic;
	}
}

void onConfigurationChangeFsioCallback(engine_configuration_s *previousConfiguration DECLARE_ENGINE_PARAMETER_SUFFIX) {
	(void)previousConfiguration;
#if EFI_FSIO
	applyFsioConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
#endif
}

static LECalculator calc CCM_OPTIONAL;

static SimplePwm fsioPwm[FSIO_COMMAND_COUNT] CCM_OPTIONAL;

// that's crazy, but what's an alternative? we need const char *, a shared buffer would not work for pin repository
static const char *getGpioPinName(int index) {
	switch (index) {
	case 0:
		return "FSIO_OUT_0";
	case 1:
		return "FSIO_OUT_1";
	case 10:
		return "FSIO_OUT_10";
	case 11:
		return "FSIO_OUT_11";
	case 12:
		return "FSIO_OUT_12";
	case 13:
		return "FSIO_OUT_13";
	case 14:
		return "FSIO_OUT_14";
	case 15:
		return "FSIO_OUT_15";
	case 2:
		return "FSIO_OUT_2";
	case 3:
		return "FSIO_OUT_3";
	case 4:
		return "FSIO_OUT_4";
	case 5:
		return "FSIO_OUT_5";
	case 6:
		return "FSIO_OUT_6";
	case 7:
		return "FSIO_OUT_7";
	case 8:
		return "FSIO_OUT_8";
	case 9:
		return "FSIO_OUT_9";
	}
	return NULL;
}

float getFsioOutputValue(int index DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (state.fsioLogics[index] == NULL) {
		warning(CUSTOM_NO_FSIO, "no FSIO for #%d %s", index + 1, hwPortname(CONFIG(fsioOutputPins)[index]));
		return NAN;
	} else {
		return calc.evaluate("FSIO", engine->fsioState.fsioLastValue[index], state.fsioLogics[index] PASS_ENGINE_PARAMETER_SUFFIX);
	}
}

/**
 * @param index from zero for (FSIO_COMMAND_COUNT - 1)
 */
static void runFsioCalculation(int index DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (strlen(config->fsioFormulas[index]) == 0) {
		engine->fsioState.fsioLastValue[index] = NAN;
		return;
	}

	bool isPwmMode = CONFIG(fsioFrequency)[index] != NO_PWM;

	float fvalue = getFsioOutputValue(index PASS_ENGINE_PARAMETER_SUFFIX);
	engine->fsioState.fsioLastValue[index] = fvalue;

	if (isPwmMode) {
		fsioPwm[index].setSimplePwmDutyCycle(fvalue);
	} else {
		int value = (int) fvalue;
		if (value != enginePins.fsioOutputs[index].getLogicValue()) {
			//		efiPrintf("setting %s %s", getIo_pin_e(pin), boolToString(value));
			enginePins.fsioOutputs[index].setValue(value);
		}
	}
}


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


#if EFI_PROD_CODE
static void setFsioFrequency(int index, int frequency) {
	index--;
	if (index < 0 || index >= FSIO_COMMAND_COUNT) {
		efiPrintf("invalid FSIO index: %d", index);
		return;
	}
	CONFIG(fsioFrequency)[index] = frequency;
	if (frequency == 0) {
		efiPrintf("FSIO output #%d@%s set to on/off mode", index + 1, hwPortname(CONFIG(fsioOutputPins)[index]));
	} else {
		efiPrintf("Setting FSIO frequency %dHz on #%d@%s", frequency, index + 1, hwPortname(CONFIG(fsioOutputPins)[index]));
	}
}
#endif /* EFI_PROD_CODE */

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
	for (int index = 0; index < FSIO_COMMAND_COUNT; index++) {
		runFsioCalculation(index PASS_ENGINE_PARAMETER_SUFFIX);
	}

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

static void showFsioInfo(void) {
#if EFI_PROD_CODE || EFI_SIMULATOR
	efiPrintf("sys used %d/user used %d", sysPool.getSize(), userPool.getSize());
	showFsio("fuel", fuelPumpLogic);

	for (int i = 0; i < CAM_INPUTS_COUNT ; i++) {
		brain_pin_e pin = engineConfiguration->vvtPins[i];
		if (isBrainPinValid(pin)) {
			efiPrintf("VVT pid #%d [%s]", (i + 1),
					hwPortname(pin));

		}
	}



	for (int i = 0; i < FSIO_COMMAND_COUNT; i++) {
		char * exp = config->fsioFormulas[i];
		if (exp[0] != 0) {
			/**
			 * in case of FSIO user interface indexes are starting with 0, the argument for that
			 * is the fact that the target audience is more software developers
			 */
			int freq = CONFIG(fsioFrequency)[i];
			const char *modeMessage = freq == 0 ? " (on/off mode)" : "";
			efiPrintf("FSIO #%d [%s] at %s@%dHz%s value=%.2f", (i + 1), exp,
					hwPortname(CONFIG(fsioOutputPins)[i]),
					freq, modeMessage,
					engine->fsioState.fsioLastValue[i]);
//			efiPrintf("user-defined #%d value=%.2f", i, engine->engineConfigurationPtr2->fsioLastValue[i]);
			showFsio(NULL, state.fsioLogics[i]);
		}
	}
	for (int i = 0; i < FSIO_COMMAND_COUNT; i++) {
		float v = CONFIG(fsio_setting)[i];
		if (!cisnan(v)) {
			efiPrintf("user property #%d: %.2f", i + 1, v);
		}
	}
	for (int i = 0; i < FSIO_COMMAND_COUNT; i++) {
		brain_pin_e inputPin = CONFIG(fsioDigitalInputs)[i];
		if (isBrainPinValid(inputPin)) {
			efiPrintf("FSIO digital input #%d: %s", i, hwPortname(inputPin));
		}
	}
#endif
}

/**
 * set_fsio_setting 1 0.11
 */
static void setFsioSetting(float humanIndexF, float value) {
#if EFI_PROD_CODE || EFI_SIMULATOR
	int index = (int)humanIndexF - 1;
	if (index < 0 || index >= FSIO_COMMAND_COUNT) {
		efiPrintf("invalid FSIO index: %d", (int)humanIndexF);
		return;
	}
	engineConfiguration->fsio_setting[index] = value;
	showFsioInfo();
#endif
}

void setFsioExpression(const char *indexStr, const char *quotedLine DECLARE_CONFIG_PARAMETER_SUFFIX) {
	int index = atoi(indexStr) - 1;
	if (index < 0 || index >= FSIO_COMMAND_COUNT) {
		efiPrintf("invalid FSIO index: %d", index);
		return;
	}
	char * l = unquote((char*) quotedLine);
	if (strlen(l) > LE_COMMAND_LENGTH - 1) {
		efiPrintf("Too long %d", strlen(l));
		return;
	}

	efiPrintf("setting user out #%d to [%s]", index + 1, l);
	strcpy(config->fsioFormulas[index], l);
}

void applyFsioExpression(const char *indexStr, const char *quotedLine DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setFsioExpression(indexStr, quotedLine PASS_CONFIG_PARAMETER_SUFFIX);

	// this would apply the changes
	applyFsioConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);
	showFsioInfo();
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

#if EFI_PROD_CODE
	for (int i = 0; i < FSIO_COMMAND_COUNT; i++) {
		brain_pin_e brainPin = CONFIG(fsioOutputPins)[i];

		if (isBrainPinValid(brainPin)) {
			int frequency = CONFIG(fsioFrequency)[i];
			if (frequency == 0) {
				enginePins.fsioOutputs[i].initPin(getGpioPinName(i), CONFIG(fsioOutputPins)[i]);
			} else {
				startSimplePwmExt(&fsioPwm[i], "FSIOpwm",
						&engine->executor,
						brainPin, &enginePins.fsioOutputs[i], frequency, 0.5f);
			}
		}
	}

	for (int i = 0; i < FSIO_COMMAND_COUNT; i++) {
		brain_pin_e inputPin = CONFIG(fsioDigitalInputs)[i];

		if (isBrainPinValid(inputPin)) {
			efiSetPadMode("FSIO input", inputPin, getInputMode(engineConfiguration->fsioInputModes[i]));
		}
	}

	addConsoleActionSS("set_fsio_pid_output_pin", (VoidCharPtrCharPtr) setFsioPidOutputPin);
	addConsoleActionSS("set_fsio_output_pin", (VoidCharPtrCharPtr) setFsioOutputPin);
	addConsoleActionII("set_fsio_output_frequency", (VoidIntInt) setFsioFrequency);
	addConsoleActionSS("set_fsio_digital_input_pin", (VoidCharPtrCharPtr) setFsioDigitalInputPin);
	addConsoleActionSS("set_fsio_analog_input_pin", (VoidCharPtrCharPtr) setFsioAnalogInputPin);

#endif /* EFI_PROD_CODE */

#if EFI_PROD_CODE || EFI_SIMULATOR
	addConsoleActionSS("set_rpn_expression", applyFsioExpression);
	addConsoleActionFF("set_fsio_setting", setFsioSetting);
	addConsoleAction("fsioinfo", showFsioInfo);
#endif /* EFI_PROD_CODE || EFI_SIMULATOR */

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
		enginePins.fuelPumpRelay.setValue((getTimeNowSeconds() < engine->triggerActivitySecond + engineConfiguration->startUpFuelPumpDuration) || (engine->rpmCalculator.getRpm() > 0));
	}
	
	enginePins.o2heater.setValue(engine->rpmCalculator.isRunning());
}

#endif /* EFI_FSIO */
