/**
 * @file settings.cpp
 * @brief This file is about configuring engine via the human-readable protocol
 *
 * @date Dec 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#if ! EFI_UNIT_TEST

#include "eficonsole.h"
#include "trigger_decoder.h"
#include "console_io.h"
#include "idle_thread.h"
#include "alternator_controller.h"
#include "trigger_emulator_algo.h"
#include "value_lookup.h"
#if EFI_RTC
#include "rtc_helper.h"
#endif // EFI_RTC

#if EFI_PROD_CODE
#include "can_hw.h"
#include "rusefi.h"
#include "hardware.h"
#endif // EFI_PROD_CODE

#if EFI_ELECTRONIC_THROTTLE_BODY
#include "electronic_throttle.h"
#endif // EFI_ELECTRONIC_THROTTLE_BODY

#if EFI_CONFIGURATION_STORAGE
#include "flash_main.h"
#endif // EFI_CONFIGURATION_STORAGE

#if EFI_ENGINE_SNIFFER
#include "engine_sniffer.h"
extern int waveChartUsedSize;
extern WaveChart waveChart;
#endif // EFI_ENGINE_SNIFFER

using namespace rusefi::stringutil;

void printSpiState() {
	efiPrintf("spi 1=%s/2=%s/3=%s/4=%s",
		boolToString(engineConfiguration->is_enabled_spi_1),
		boolToString(engineConfiguration->is_enabled_spi_2),
		boolToString(engineConfiguration->is_enabled_spi_3),
		boolToString(engineConfiguration->is_enabled_spi_4));
}

/**
 * @brief	Prints current engine configuration to human-readable console.
 */
void printConfiguration() {

	efiPrintf("Template %s/%d trigger %s/%s/%d", getEngine_type_e(engineConfiguration->engineType),
			(int)engineConfiguration->engineType,
			getTrigger_type_e(engineConfiguration->trigger.type),
			Enum2String(engineConfiguration->fuelAlgorithm), (int)engineConfiguration->fuelAlgorithm);

	efiPrintf("configurationVersion=%d", engine->getGlobalConfigurationVersion());

#if EFI_PROD_CODE
	printSpiState();
#endif // EFI_PROD_CODE
}

#if EFI_ENGINE_CONTROL
static void setIdleSolenoidFrequency(int value) {
	engineConfiguration->idle.solenoidFrequency = value;
	incrementGlobalConfigurationVersion();
}
#endif // EFI_ENGINE_CONTROL

#if EFI_ENGINE_CONTROL
static void setCrankingRpm(int value) {
	engineConfiguration->cranking.rpm = value;
	printConfiguration();
}

/**
 * this method is used in console - it also prints current configuration
 */
static void setAlgorithmInt(int value) {
	setAlgorithm((engine_load_mode_e) value);
	printConfiguration();
}

static void setFiringOrder(int value) {
	engineConfiguration->firingOrder = (firing_order_e) value;
	printConfiguration();
}

static void setRpmHardLimit(int value) {
	engineConfiguration->rpmHardLimit = value;
	printConfiguration();
}

static void setCrankingIACExtra(float percent) {
	for (uint8_t i = 0; i < CLT_CRANKING_CURVE_SIZE; i++) {
		config->cltCrankingCorr[i] = percent;
	}
	efiPrintf("cranking_iac %.2f", percent);
}

static void setCrankingFuel(float fuelMilligram) {
	setTable(config->crankingCycleBaseFuel, fuelMilligram);
	efiPrintf("cranking_fuel %.2f", fuelMilligram);
}

static void setGlobalTriggerAngleOffset(float value) {
	engineConfiguration->globalTriggerAngleOffset = value;
	incrementGlobalConfigurationVersion();
	printConfiguration();
}

static void setCrankingTimingAngle(float value) {
	engineConfiguration->crankingTimingAngle = value;
	incrementGlobalConfigurationVersion();
	printConfiguration();
}

static void setCrankingInjectionMode(int value) {
	engineConfiguration->crankingInjectionMode = (injection_mode_e) value;
	incrementGlobalConfigurationVersion();
	printConfiguration();
}

static void setInjectionMode(int value) {
	engineConfiguration->injectionMode = (injection_mode_e) value;
	incrementGlobalConfigurationVersion();
	printConfiguration();
}

static void setIgnitionMode(int value) {
#if EFI_ENGINE_CONTROL
	engineConfiguration->ignitionMode = (ignition_mode_e) value;
	incrementGlobalConfigurationVersion();
	prepareOutputSignals();
#endif // EFI_ENGINE_CONTROL
}

static void setIndividualCoilsIgnition() {
	setIgnitionMode((int)IM_INDIVIDUAL_COILS);
}

static void setTriggerType(int value) {
	engineConfiguration->trigger.type = (trigger_type_e) value;
	incrementGlobalConfigurationVersion();
	printConfiguration();
	efiPrintf("Do you need to also invoke set operation_mode X?");
	engine->resetEngineSnifferIfInTestMode();
}

static void setDebugMode(int value) {
	engineConfiguration->debugMode = (debug_mode_e) value;
}

static void setWholeTimingMap(float value) {
	setTable(config->ignitionTable, value);
}

static void setWholeTimingMapCmd(float value) {
	efiPrintf("Setting whole timing advance map to %.2f", value);
	setWholeTimingMap(value);
	engine->resetEngineSnifferIfInTestMode();
}

#endif // EFI_ENGINE_CONTROL

#if EFI_PROD_CODE

static brain_pin_e parseBrainPinWithErrorMessage(const char *pinName) {
	brain_pin_e pin = parseBrainPin(pinName);
	if (pin == Gpio::Invalid) {
		efiPrintf("invalid pin name [%s]", pinName);
	}
	return pin;
}

/**
 * For example:
 *   set_ignition_pin 1 PD7
 * todo: this method counts index from 1 while at least 'set_trigger_input_pin' counts from 0.
 * todo: make things consistent
 */
static void setIgnitionPin(const char *indexStr, const char *pinName) {
	int index = atoi(indexStr) - 1; // convert from human index into software index
	if (index < 0 || index >= MAX_CYLINDER_COUNT)
		return;
	brain_pin_e pin = parseBrainPinWithErrorMessage(pinName);
	if (pin == Gpio::Invalid) {
		return;
	}
	efiPrintf("setting ignition pin[%d] to %s please save&restart", index, hwPortname(pin));
	engineConfiguration->ignitionPins[index] = pin;
	incrementGlobalConfigurationVersion();
}

// this method is useful for desperate time debugging
// readpin PA0
void readPin(const char *pinName) {
	brain_pin_e pin = parseBrainPinWithErrorMessage(pinName);
	if (pin == Gpio::Invalid) {
		return;
	}
	int physicalValue = palReadPad(getHwPort("read", pin), getHwPin("read", pin));
	efiPrintf("pin %s value %d", hwPortname(pin), physicalValue);
}

// this method is useful for desperate time debugging or hardware validation
static void benchSetPinValue(const char *pinName, int bit) {
	brain_pin_e pin = parseBrainPinWithErrorMessage(pinName);
	if (pin == Gpio::Invalid) {
		return;
	}
	efiSetPadModeWithoutOwnershipAcquisition("bench_pin_test", pin, PAL_MODE_OUTPUT_PUSHPULL);
	// low-level API which does not care about 'qcDirectPinControlMode'
	palWritePad(getHwPort("write", pin), getHwPin("write", pin), bit);
	efiPrintf("pin %s set value", hwPortname(pin));
	readPin(pinName);
}

static void benchClearPin(const char *pinName) {
	benchSetPinValue(pinName, 0);
}

static void benchSetPin(const char *pinName) {
	benchSetPinValue(pinName, 1);
}

static void setIndividualPin(const char *pinName, brain_pin_e *targetPin, const char *name) {
	brain_pin_e pin = parseBrainPinWithErrorMessage(pinName);
	if (pin == Gpio::Invalid) {
		return;
	}
	efiPrintf("setting %s pin to %s please save&restart", name, hwPortname(pin));
	*targetPin = pin;
	incrementGlobalConfigurationVersion();
}

// set vss_pin
static void setVssPin(const char *pinName) {
	setIndividualPin(pinName, &engineConfiguration->vehicleSpeedSensorInputPin, "VSS");
}

// set_idle_pin none
static void setIdlePin(const char *pinName) {
	setIndividualPin(pinName, &engineConfiguration->idle.solenoidPin, "idle");
}

static void setAlternatorPin(const char *pinName) {
	setIndividualPin(pinName, &engineConfiguration->alternatorControlPin, "alternator");
}

/**
 * For example:
 *   set_trigger_input_pin 0 PA5
 * todo: this method counts index from 0 while at least 'set_ignition_pin' counts from 1.
 * todo: make things consistent
 */
static void setTriggerInputPin(const char *indexStr, const char *pinName) {
	int index = atoi(indexStr);
	if (index < 0 || index > 2)
		return;
	brain_pin_e pin = parseBrainPinWithErrorMessage(pinName);
	if (pin == Gpio::Invalid) {
		return;
	}
	efiPrintf("setting trigger pin[%d] to %s please save&restart", index, hwPortname(pin));
	engineConfiguration->triggerInputPins[index] = pin;
	incrementGlobalConfigurationVersion();
}

static void setTriggerSimulatorPin(const char *indexStr, const char *pinName) {
	int index = atoi(indexStr);
	if (index < 0 || index >= TRIGGER_SIMULATOR_PIN_COUNT)
		return;
	brain_pin_e pin = parseBrainPinWithErrorMessage(pinName);
	if (pin == Gpio::Invalid) {
		return;
	}
	efiPrintf("setting trigger simulator pin[%d] to %s please save&restart", index, hwPortname(pin));
	engineConfiguration->triggerSimulatorPins[index] = pin;
	incrementGlobalConfigurationVersion();
}

#if HAL_USE_ADC
// set_analog_input_pin pps pa4
// set_analog_input_pin afr none
static void setAnalogInputPin(const char *sensorStr, const char *pinName) {
	brain_pin_e pin = parseBrainPinWithErrorMessage(pinName);
	if (pin == Gpio::Invalid) {
		return;
	}
	adc_channel_e channel = getAdcChannel(pin);
	if (channel == EFI_ADC_ERROR) {
		efiPrintf("Error with [%s]", pinName);
		return;
	}
	if (strEqual("map", sensorStr)) {
		engineConfiguration->map.sensor.hwChannel = channel;
		efiPrintf("setting MAP to %s/%d", pinName, channel);
	} else if (strEqual("pps", sensorStr)) {
		engineConfiguration->throttlePedalPositionAdcChannel = channel;
		efiPrintf("setting PPS to %s/%d", pinName, channel);
	} else if (strEqual("afr", sensorStr)) {
		engineConfiguration->afr.hwChannel = channel;
		efiPrintf("setting AFR to %s/%d", pinName, channel);
	} else if (strEqual("clt", sensorStr)) {
		engineConfiguration->clt.adcChannel = channel;
		efiPrintf("setting CLT to %s/%d", pinName, channel);
	} else if (strEqual("iat", sensorStr)) {
		engineConfiguration->iat.adcChannel = channel;
		efiPrintf("setting IAT to %s/%d", pinName, channel);
	} else if (strEqual("tps", sensorStr)) {
		engineConfiguration->tps1_1AdcChannel = channel;
		efiPrintf("setting TPS1 to %s/%d", pinName, channel);
	} else if (strEqual("tps2", sensorStr)) {
		engineConfiguration->tps2_1AdcChannel = channel;
		efiPrintf("setting TPS2 to %s/%d", pinName, channel);
	}
	incrementGlobalConfigurationVersion();
}
#endif // HAL_USE_ADC

static void setLogicInputPin(const char *indexStr, const char *pinName) {
	int index = atoi(indexStr);
	if (index < 0 || index > 2) {
		return;
	}
	brain_pin_e pin = parseBrainPinWithErrorMessage(pinName);
	if (pin == Gpio::Invalid) {
		return;
	}
	efiPrintf("setting logic input pin[%d] to %s please save&restart", index, hwPortname(pin));
	engineConfiguration->logicAnalyzerPins[index] = pin;
	incrementGlobalConfigurationVersion();
}

#endif // EFI_PROD_CODE

static void setSpiMode(int index, bool mode) {
	switch (index) {
	case 1:
		engineConfiguration->is_enabled_spi_1 = mode;
		break;
	case 2:
		engineConfiguration->is_enabled_spi_2 = mode;
		break;
	case 3:
		engineConfiguration->is_enabled_spi_3 = mode;
		break;
	default:
		efiPrintf("invalid spi index %d", index);
		return;
	}
	printSpiState();
}

bool verboseRxCan = false;

static void enableOrDisable(const char *param, bool isEnabled) {
	if (strEqualCaseInsensitive(param, "useTLE8888_cranking_hack")) {
		engineConfiguration->useTLE8888_cranking_hack = isEnabled;
#if EFI_SHAFT_POSITION_INPUT
	} else if (strEqualCaseInsensitive(param, CMD_TRIGGER_HW_INPUT)) {
		getTriggerCentral()->hwTriggerInputEnabled = isEnabled;
#endif // EFI_SHAFT_POSITION_INPUT
	} else if (strEqualCaseInsensitive(param, "verboseTLE8888")) {
		engineConfiguration->verboseTLE8888 = isEnabled;
	} else if (strEqualCaseInsensitive(param, "verboseRxCan")) {
		verboseRxCan = isEnabled;
	} else if (strEqualCaseInsensitive(param, "verboseCan")) {
		engineConfiguration->verboseCan = isEnabled;
	} else if (strEqualCaseInsensitive(param, "verboseCan2")) {
		engineConfiguration->verboseCan2 = isEnabled;
#if (EFI_CAN_BUS_COUNT >= 3)
	} else if (strEqualCaseInsensitive(param, "verboseCan3")) {
		engineConfiguration->verboseCan3 = isEnabled;
#endif
	} else if (strEqualCaseInsensitive(param, "verboseIsoTp")) {
		engineConfiguration->verboseIsoTp = isEnabled;
	} else if (strEqualCaseInsensitive(param, "artificialMisfire")) {
		engineConfiguration->artificialTestMisfire = isEnabled;
	} else if (strEqualCaseInsensitive(param, "logic_level_trigger")) {
		engineConfiguration->displayLogicLevelsInEngineSniffer = isEnabled;
	} else if (strEqualCaseInsensitive(param, "can_broadcast")) {
		engineConfiguration->enableVerboseCanTx = isEnabled;
//	} else if (strEqualCaseInsensitive(param, "etb_auto")) {
//		engine->etbAutoTune = isEnabled;
	} else if (strEqualCaseInsensitive(param, "verboseKLine")) {
		engineConfiguration->verboseKLine = isEnabled;
	} else if (strEqualCaseInsensitive(param, "stepperidle")) {
		engineConfiguration->useStepperIdle = isEnabled;
	} else if (strEqualCaseInsensitive(param, "two_wire_wasted_spark")) {
		engineConfiguration->twoWireBatchIgnition = isEnabled;
		incrementGlobalConfigurationVersion();
	} else if (strEqualCaseInsensitive(param, "altcontrol")) {
		engineConfiguration->isAlternatorControlEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "sd")) {
		engineConfiguration->isSdCardEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, CMD_FUNCTIONAL_TEST_MODE)) {
		engine->isFunctionalTestMode = isEnabled;
	} else if (strEqualCaseInsensitive(param, "can_read")) {
		engineConfiguration->canReadEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "can_write")) {
		engineConfiguration->canWriteEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, CMD_INJECTION)) {
		engineConfiguration->isInjectionEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, CMD_PWM)) {
		engine->isPwmEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "trigger_details")) {
		engineConfiguration->verboseTriggerSynchDetails = isEnabled;
	} else if (strEqualCaseInsensitive(param, "vvt_details")) {
		engineConfiguration->verboseVVTDecoding = isEnabled;
	} else if (strEqualCaseInsensitive(param, "invertCamVVTSignal")) {
		engineConfiguration->invertCamVVTSignal = isEnabled;
	} else if (strEqualCaseInsensitive(param, CMD_IGNITION)) {
		engineConfiguration->isIgnitionEnabled = isEnabled;
#if EFI_EMULATE_POSITION_SENSORS
	} else if (strEqualCaseInsensitive(param, CMD_SELF_STIMULATION)) {
		if (isEnabled) {
			enableTriggerStimulator();
		} else {
			disableTriggerStimulator();
		}
	} else if (strEqualCaseInsensitive(param, CMD_EXTERNAL_STIMULATION)) {
		if (isEnabled) {
			enableExternalTriggerStimulator();
		} else {
			disableTriggerStimulator();
		}
#endif // EFI_EMULATE_POSITION_SENSORS
	} else {
		efiPrintf("unexpected [%s]", param);
		return; // well, MISRA would not like this 'return' here :(
	}
	efiPrintf("[%s] %s", param, isEnabled ? "enabled" : "disabled");
}

static void enable(const char *param) {
	enableOrDisable(param, true);
}

static void disable(const char *param) {
	enableOrDisable(param, false);
}

static void enableSpi(int index) {
	setSpiMode(index, true);
}

static void disableSpi(int index) {
	setSpiMode(index, false);
}

/**
 * See 'LimpManager::isEngineStop' for code which actually stops engine
 */
static void scheduleStopEngine() {
	doScheduleStopEngine(StopRequestedReason::Console);
}

static void getValue(const char *paramStr) {

	{
		float value = getConfigValueByName(paramStr);
		if (value != EFI_ERROR_CODE) {
			efiPrintf("%s value: %.2f", paramStr, value);
			return;
		}
	}

	if (strEqualCaseInsensitive(paramStr, "tps_min")) {
		efiPrintf("tps_min=%d", engineConfiguration->tpsMin);
	} else if (strEqualCaseInsensitive(paramStr, "tps_max")) {
		efiPrintf("tps_max=%d", engineConfiguration->tpsMax);
	} else if (strEqualCaseInsensitive(paramStr, "global_trigger_offset_angle")) {
		efiPrintf("global_trigger_offset=%.2f", engineConfiguration->globalTriggerAngleOffset);
#if EFI_SHAFT_POSITION_INPUT
	} else if (strEqualCaseInsensitive(paramStr, "trigger_hw_input")) {
		efiPrintf("trigger_hw_input=%s", boolToString(getTriggerCentral()->hwTriggerInputEnabled));
#endif // EFI_SHAFT_POSITION_INPUT
	} else if (strEqualCaseInsensitive(paramStr, CMD_DATE)) {
		printDateTime();
	} else {
		efiPrintf("Invalid Parameter: %s", paramStr);
	}
}

static void setScriptCurve1Value(float value) {
	setLinearCurve(config->scriptCurve1, value, value, 1);
}

static void setScriptCurve2Value(float value) {
	setLinearCurve(config->scriptCurve2, value, value, 1);
}

struct command_i_s {
	const char *token;
	VoidInt callback;
};

struct command_f_s {
	const char *token;
	VoidFloat callback;
};

const command_f_s commandsF[] = {
#if EFI_ENGINE_CONTROL
		{"global_trigger_offset_angle", setGlobalTriggerAngleOffset},
		{"cranking_fuel", setCrankingFuel},
		{"cranking_iac", setCrankingIACExtra},
		{"cranking_timing_angle", setCrankingTimingAngle},
		{"flat_injector_lag", setFlatInjectorLag},
#endif // EFI_ENGINE_CONTROL
		{"script_curve_1_value", setScriptCurve1Value},
		{"script_curve_2_value", setScriptCurve2Value},
};

const command_i_s commandsI[] = {
#if EFI_ENGINE_CONTROL
        {"ignition_mode", setIgnitionMode},
        {"driveWheelRevPerKm", [](int value) {
            engineConfiguration->driveWheelRevPerKm = value;
        }},
		{"cranking_rpm", setCrankingRpm},
		{"cranking_injection_mode", setCrankingInjectionMode},
		{"injection_mode", setInjectionMode},
		{CMD_ENGINE_TYPE, setEngineTypeAndSave},
		{"rpm_hard_limit", setRpmHardLimit},
		{"firing_order", setFiringOrder},
		{"algorithm", setAlgorithmInt},
		{"debug_mode", setDebugMode},
		{"trigger_type", setTriggerType},
		// used by HW CI
		{"idle_solenoid_freq", setIdleSolenoidFrequency},
#endif // EFI_ENGINE_CONTROL
#if EFI_PROD_CODE
#if EFI_BOR_LEVEL
		{"bor", setBor},
#endif // EFI_BOR_LEVEL
#if EFI_CAN_SUPPORT
		{"can_mode", setCanType},
		{"can_vss", setCanVss},
#endif // EFI_CAN_SUPPORT
#if EFI_IDLE_CONTROL
		{"idle_position", setManualIdleValvePosition},
		{"idle_rpm", setTargetIdleRpm},
#endif // EFI_IDLE_CONTROL
#endif // EFI_PROD_CODE

		//		{"", },
		//		{"", },
};

static void setValue(const char *paramStr, const char *valueStr) {
	float valueF = atoff(valueStr);
	int valueI = atoi(valueStr);

	const command_f_s *currentF = &commandsF[0];
	while (currentF < commandsF + sizeof(commandsF)/sizeof(commandsF[0])) {
		if (strEqualCaseInsensitive(paramStr, currentF->token)) {
			currentF->callback(valueF);
			return;
		}
		currentF++;
	}

	const command_i_s *currentI = &commandsI[0];
	while (currentI < commandsI + sizeof(commandsI)/sizeof(commandsI[0])) {
		if (strEqualCaseInsensitive(paramStr, currentI->token)) {
			currentI->callback(valueI);
			return;
		}
		currentI++;
	}


	if (strEqualCaseInsensitive(paramStr, "warning_period")) {
		engineConfiguration->warningPeriod = valueI;
	} else if (strEqualCaseInsensitive(paramStr, "dwell")) {
		setConstantDwell(valueF);
	} else if (strEqualCaseInsensitive(paramStr, CMD_ENGINESNIFFERRPMTHRESHOLD)) {
		engineConfiguration->engineSnifferRpmThreshold = valueI;
#if EFI_EMULATE_POSITION_SENSORS
	} else if (strEqualCaseInsensitive(paramStr, CMD_RPM)) {
		setTriggerEmulatorRPM(valueI);
#endif // EFI_EMULATE_POSITION_SENSORS
	} else if (strEqualCaseInsensitive(paramStr, "mc33_hvolt")) {
		engineConfiguration->mc33_hvolt = valueI;
	} else if (strEqualCaseInsensitive(paramStr, "mc33_i_peak")) {
		engineConfiguration->mc33_i_peak = valueI;
	} else if (strEqualCaseInsensitive(paramStr, "mc33_i_hold")) {
		engineConfiguration->mc33_i_hold = valueI;
	} else if (strEqualCaseInsensitive(paramStr, "mc33_t_max_boost")) {
		engineConfiguration->mc33_t_max_boost = valueI;
	} else if (strEqualCaseInsensitive(paramStr, "mc33_t_peak_off")) {
		engineConfiguration->mc33_t_peak_off = valueI;
	} else if (strEqualCaseInsensitive(paramStr, "vvt_offset")) {
		engineConfiguration->vvtOffsets[0] = valueF;
	} else if (strEqualCaseInsensitive(paramStr, "vvt_mode")) {
		engineConfiguration->vvtMode[0] = (vvt_mode_e)valueI;
	} else if (strEqualCaseInsensitive(paramStr, "wwaeTau")) {
		engineConfiguration->wwaeTau = valueF;
	} else if (strEqualCaseInsensitive(paramStr, "wwaeBeta")) {
		engineConfiguration->wwaeBeta = valueF;
	} else if (strEqualCaseInsensitive(paramStr, "benchTestOffTime")) {
		engineConfiguration->benchTestOffTime = valueI;
	} else if (strEqualCaseInsensitive(paramStr, "benchTestCount")) {
		engineConfiguration->benchTestCount = valueI;
	} else if (strEqualCaseInsensitive(paramStr, "cranking_dwell")) {
		engineConfiguration->ignitionDwellForCrankingMs = valueF;
#if EFI_PROD_CODE
	} else if (strEqualCaseInsensitive(paramStr, CMD_VSS_PIN)) {
		setVssPin(valueStr);
#endif // EFI_PROD_CODE
	} else if (strEqualCaseInsensitive(paramStr, "targetvbatt")) {
		setTable(config->alternatorVoltageTargetTable, valueF);
	} else if (strEqualCaseInsensitive(paramStr, CMD_DATE)) {
		// rusEfi console invokes this method with timestamp in local timezone
		setDateTime(valueStr);
	}

	bool isGoodName = setConfigValueByName(paramStr, valueF);
    if (isGoodName) {
       efiPrintf("Settings: applying [%s][%f]", paramStr, valueF);
    }

	engine->resetEngineSnifferIfInTestMode();
}

void initSettings() {
#if EFI_SIMULATOR
	printf("initSettings\n");
#endif // EFI_SIMULATOR

	// todo: start saving values into flash right away?

#if EFI_ENGINE_CONTROL
    // used by HW CI
	addConsoleAction(CMD_INDIVIDUAL_INJECTION, setIndividualCoilsIgnition);
	addConsoleAction("showconfig", printConfiguration);
	addConsoleActionF("set_whole_timing_map", setWholeTimingMapCmd);
#endif // EFI_ENGINE_CONTROL

	addConsoleAction("stopengine", (Void) scheduleStopEngine);

	// todo: refactor this - looks like all boolean flags should be controlled with less code duplication
	addConsoleActionI("enable_spi", enableSpi);
	addConsoleActionI("disable_spi", disableSpi);

	addConsoleActionS(CMD_ENABLE, enable);
	addConsoleActionS(CMD_DISABLE, disable);

	addConsoleActionSS(CMD_SET, setValue);
	addConsoleActionS(CMD_GET, getValue);

#if EFI_PROD_CODE
	addConsoleActionSS(CMD_IGNITION_PIN, setIgnitionPin);
	addConsoleActionSS(CMD_TRIGGER_PIN, setTriggerInputPin);
	addConsoleActionSS(CMD_TRIGGER_SIMULATOR_PIN, setTriggerSimulatorPin);

	addConsoleActionI(CMD_ECU_UNLOCK, unlockEcu);

	addConsoleActionS(CMD_ALTERNATOR_PIN, setAlternatorPin);
	addConsoleActionS(CMD_IDLE_PIN, setIdlePin);

	addConsoleActionS("bench_clearpin", benchClearPin);
	addConsoleActionS("bench_setpin", benchSetPin);
	addConsoleActionS("readpin", readPin);
	addConsoleAction("hw_qc_mode", [](){
  	setHwQcMode();
  });
	addConsoleActionS("bench_set_output_mode", [](const char *pinName){
	  brain_pin_e pin = parseBrainPinWithErrorMessage(pinName);
	  if (pin == Gpio::Invalid) {
		  return;
	  }
	  efiSetPadModeWithoutOwnershipAcquisition("manual-mode", pin, PAL_MODE_OUTPUT_PUSHPULL);
  });

#if HAL_USE_ADC
	addConsoleAction("adc_report", printFullAdcReport);
	addConsoleActionSS("set_analog_input_pin", setAnalogInputPin);
#endif // HAL_USE_ADC
	addConsoleActionSS(CMD_LOGIC_PIN, setLogicInputPin);
#endif // EFI_PROD_CODE
}

void printDateTime() {
#if EFI_RTC
	printRtcDateTime();
#else // EFI_RTC
	efiPrintf("Cannot print time: RTC not supported");
#endif // EFI_RTC
}

void setDateTime(const char * const isoDateTime) {
#if EFI_RTC
	printRtcDateTime();
	if (strlen(isoDateTime) >= 19 && isoDateTime[10] == 'T') {
		efidatetime_t dateTime;
		dateTime.year = atoi(isoDateTime);
		dateTime.month = atoi(isoDateTime + 5);
		dateTime.day = atoi(isoDateTime + 8);
		dateTime.hour = atoi(isoDateTime + 11);
		dateTime.minute = atoi(isoDateTime + 14);
		dateTime.second = atoi(isoDateTime + 17);
		if (dateTime.year != ATOI_ERROR_CODE &&
				dateTime.month >= 1 && dateTime.month <= 12 &&
				dateTime.day >= 1 && dateTime.day <= 31 &&
				dateTime.hour <= 23 &&
				dateTime.minute <= 59 &&
				dateTime.second <= 59) {
			// doesn't concern about leap years or seconds; ChibiOS doesn't support (added) leap seconds anyway
			setRtcDateTime(&dateTime);
			efiPrintf("Time is changed to");
			printRtcDateTime();
			return;
		}
	}
	efiPrintf("date_set Date parameter %s is wrong", isoDateTime);
#else // EFI_RTC
	efiPrintf("Cannot set time: RTC not supported");
#endif // EFI_RTC
}

#endif // ! EFI_UNIT_TEST

void setEngineTypeAndSave(int value) {
	setEngineType(value, true);
}

void setEngineType(int value, bool isWriteToFlash) {
	{
#if EFI_PROD_CODE
		chibios_rt::CriticalSectionLocker csl;
#endif // EFI_PROD_CODE

		engineConfiguration->engineType = (engine_type_e)value;
		resetConfigurationExt((engine_type_e)value);
		engine->resetEngineSnifferIfInTestMode();

#if EFI_CONFIGURATION_STORAGE
		if (isWriteToFlash) {
			writeToFlashNow();
		}
#endif /* EFI_CONFIGURATION_STORAGE */
	}
	incrementGlobalConfigurationVersion("engineType");
#if EFI_ENGINE_CONTROL && ! EFI_UNIT_TEST
	printConfiguration();
#endif // ! EFI_UNIT_TEST
}

void setLuaScript(const char *luaScript) {
	strncpy(config->luaScript, luaScript, efi::size(config->luaScript) - 1);
}
