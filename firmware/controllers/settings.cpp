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

#if EFI_INTERNAL_FLASH
#include "flash_main.h"
#endif // EFI_INTERNAL_FLASH

#if EFI_ENGINE_SNIFFER
#include "engine_sniffer.h"
extern int waveChartUsedSize;
extern WaveChart waveChart;
#endif // EFI_ENGINE_SNIFFER

static void setTimingMode(int value) {
	engineConfiguration->timingMode = (timing_mode_e) value;
	incrementGlobalConfigurationVersion();
}

static void setIdleSolenoidFrequency(int value) {
	engineConfiguration->idle.solenoidFrequency = value;
	incrementGlobalConfigurationVersion();
}

static void setSensorChartMode(int value) {
	engineConfiguration->sensorChartMode = (sensor_chart_e) value;
}

static void setCrankingRpm(int value) {
	engineConfiguration->cranking.rpm = value;
}

/**
 * this method is used in console - it also prints current configuration
 */
static void setAlgorithmInt(int value) {
	setAlgorithm((engine_load_mode_e) value);
}

static void setFiringOrder(int value) {
	engineConfiguration->firingOrder = (firing_order_e) value;
}

static void setRpmHardLimit(int value) {
	engineConfiguration->rpmHardLimit = value;
}

static void setCrankingIACExtra(float percent) {
	engineConfiguration->crankingIACposition = percent;
	efiPrintf("cranking_iac %.2f", percent);
}

static void setGlobalTriggerAngleOffset(float value) {
	engineConfiguration->globalTriggerAngleOffset = value;
	incrementGlobalConfigurationVersion();
}

static void setCrankingTimingAngle(float value) {
	engineConfiguration->crankingTimingAngle = value;
	incrementGlobalConfigurationVersion();
}

static void setInjectionMode(int value) {
	engineConfiguration->injectionMode = (injection_mode_e) value;
	incrementGlobalConfigurationVersion();
}

static void setIgnitionMode(int value) {
	engineConfiguration->ignitionMode = (ignition_mode_e) value;
	incrementGlobalConfigurationVersion();
	prepareOutputSignals();
}

static void setIndividualCoilsIgnition() {
	setIgnitionMode((int)IM_INDIVIDUAL_COILS);
}

static void setTriggerType(int value) {
	engineConfiguration->trigger.type = (trigger_type_e) value;
	incrementGlobalConfigurationVersion();
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

static void enableOrDisable(const char *param, bool isEnabled) {
	if (strEqualCaseInsensitive(param, "useTLE8888_cranking_hack")) {
		engineConfiguration->useTLE8888_cranking_hack = isEnabled;
#if EFI_SHAFT_POSITION_INPUT
	} else if (strEqualCaseInsensitive(param, CMD_TRIGGER_HW_INPUT)) {
		getTriggerCentral()->hwTriggerInputEnabled = isEnabled;
#endif // EFI_SHAFT_POSITION_INPUT
	} else if (strEqualCaseInsensitive(param, "verboseTLE8888")) {
		engineConfiguration->verboseTLE8888 = isEnabled;
	} else if (strEqualCaseInsensitive(param, "verboseCan")) {
		engineConfiguration->verboseCan = isEnabled;
	} else if (strEqualCaseInsensitive(param, "verboseIsoTp")) {
		engineConfiguration->verboseIsoTp = isEnabled;
	} else if (strEqualCaseInsensitive(param, "artificialMisfire")) {
		engineConfiguration->artificialTestMisfire = isEnabled;
	} else if (strEqualCaseInsensitive(param, "can_broadcast")) {
		engineConfiguration->enableVerboseCanTx = isEnabled;
	} else if (strEqualCaseInsensitive(param, "etb_auto")) {
		engine->etbAutoTune = isEnabled;
	} else if (strEqualCaseInsensitive(param, "step1limimter")) {
		engineConfiguration->enabledStep1Limiter = isEnabled;
#if EFI_PROD_CODE
	} else if (strEqualCaseInsensitive(param, "auto_idle")) {
#if EFI_IDLE_CONTROL
		setIdleMode(isEnabled ? IM_MANUAL : IM_AUTO);
#endif // EFI_IDLE_CONTROL
#endif // EFI_PROD_CODE
	} else if (strEqualCaseInsensitive(param, "stepperidle")) {
		engineConfiguration->useStepperIdle = isEnabled;
	} else if (strEqualCaseInsensitive(param, "boardUseTempPullUp")) {
		engineConfiguration->boardUseTempPullUp = isEnabled;
		incrementGlobalConfigurationVersion();
	} else if (strEqualCaseInsensitive(param, "boardUseTachPullUp")) {
		engineConfiguration->boardUseTachPullUp = isEnabled;
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
	} else if (strEqualCaseInsensitive(param, "map_avg")) {
		engineConfiguration->isMapAveragingEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "logic_analyzer")) {
		engineConfiguration->isWaveAnalyzerEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "manual_spinning")) {
		engineConfiguration->isManualSpinningMode = isEnabled;
	} else if (strEqualCaseInsensitive(param, "cylinder_cleanup")) {
		engineConfiguration->isCylinderCleanupEnabled = isEnabled;
	} else {
		efiPrintf("unexpected [%s]", param);
		return;
	}
	efiPrintf("[%s] %s", param, isEnabled ? "enabled" : "disabled");
}

static void enable(const char *param) {
	enableOrDisable(param, true);
}

static void disable(const char *param) {
	enableOrDisable(param, false);
}

/**
 * See 'LimpManager::isEngineStop' for code which actually stops engine
 */
void scheduleStopEngine() {
	doScheduleStopEngine();
}

static void getValue(const char *paramStr) {
	{
		float value = getConfigValueByName(paramStr);
		if (value != EFI_ERROR_CODE) {
			efiPrintf("%s value: %.2f", paramStr, value);
			return;
		}
	}

	if (strEqualCaseInsensitive(paramStr, CMD_DATE)) {
		printDateTime();
	} else {
		efiPrintf("Invalid Parameter: %s", paramStr);
	}
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
		{"cranking_iac", setCrankingIACExtra},
		{"cranking_timing_angle", setCrankingTimingAngle},
#endif // EFI_ENGINE_CONTROL

#if EFI_ELECTRONIC_THROTTLE_BODY
		{"etb", setThrottleDutyCycle},
#endif // EFI_ELECTRONIC_THROTTLE_BODY

};

const command_i_s commandsI[] = {{"ignition_mode", setIgnitionMode},
#if EFI_ENGINE_CONTROL
		{"cranking_rpm", setCrankingRpm},
		{"injection_mode", setInjectionMode},
		{"sensor_chart_mode", setSensorChartMode},
		{"timing_mode", setTimingMode},
		{CMD_ENGINE_TYPE, setEngineType},
		{"rpm_hard_limit", setRpmHardLimit},
		{"firing_order", setFiringOrder},
		{"algorithm", setAlgorithmInt},
		{"debug_mode", setDebugMode},
		{"trigger_type", setTriggerType},
		{"idle_solenoid_freq", setIdleSolenoidFrequency},
#endif // EFI_ENGINE_CONTROL
#if EFI_PROD_CODE
#if EFI_CAN_SUPPORT
		{"can_mode", setCanType},
		{"can_vss", setCanVss},
#endif // EFI_CAN_SUPPORT
#if EFI_IDLE_CONTROL
		{"idle_position", setManualIdleValvePosition},
		{"idle_rpm", setTargetIdleRpm},
#endif // EFI_IDLE_CONTROL
#endif // EFI_PROD_CODE

};

static void setValue(const char *paramStr, const char *valueStr) {
	float valueF = atoff(valueStr);
	int valueI = atoi(valueStr);

	const command_f_s *currentF = &commandsF[0];
	while (currentF < commandsF + efi::size(commandsF)) {
		if (strEqualCaseInsensitive(paramStr, currentF->token)) {
			currentF->callback(valueF);
			return;
		}
		currentF++;
	}

	const command_i_s *currentI = &commandsI[0];
	while (currentI < commandsI + efi::size(commandsI)) {
		if (strEqualCaseInsensitive(paramStr, currentI->token)) {
			currentI->callback(valueI);
			return;
		}
		currentI++;
	}

	if (strEqualCaseInsensitive(paramStr, "dwell")) {
		setConstantDwell(valueF);
	} else if (strEqualCaseInsensitive(paramStr, CMD_ENGINESNIFFERRPMTHRESHOLD)) {
		engineConfiguration->engineSnifferRpmThreshold = valueI;
#if EFI_EMULATE_POSITION_SENSORS
	} else if (strEqualCaseInsensitive(paramStr, CMD_RPM)) {
		setTriggerEmulatorRPM(valueI);
#endif // EFI_EMULATE_POSITION_SENSORS
	} else if (strEqualCaseInsensitive(paramStr, "wwaeTau")) {
		engineConfiguration->wwaeTau = valueF;
	} else if (strEqualCaseInsensitive(paramStr, "wwaeBeta")) {
		engineConfiguration->wwaeBeta = valueF;
	} else if (strEqualCaseInsensitive(paramStr, "cranking_dwell")) {
		engineConfiguration->ignitionDwellForCrankingMs = valueF;
#if EFI_PROD_CODE
	} else if (strEqualCaseInsensitive(paramStr, CMD_VSS_PIN)) {
		setVssPin(valueStr);
#endif // EFI_PROD_CODE
	} else if (strEqualCaseInsensitive(paramStr, CMD_DATE)) {
		// rusEfi console invokes this method with timestamp in local timezone
		setDateTime(valueStr);
	}

	engine->resetEngineSnifferIfInTestMode();
}

void initSettings() {
#if EFI_SIMULATOR
	printf("initSettings\n");
#endif // EFI_SIMULATOR

	addConsoleAction("set_individual_coils_ignition", setIndividualCoilsIgnition);

	addConsoleActionF("set_whole_timing_map", setWholeTimingMapCmd);

	addConsoleAction("stopengine", (Void) scheduleStopEngine);

	addConsoleActionS(CMD_ENABLE, enable);
	addConsoleActionS(CMD_DISABLE, disable);

	addConsoleActionSS("set", setValue);
	addConsoleActionS("get", getValue);

#if EFI_PROD_CODE
	addConsoleActionSS(CMD_IGNITION_PIN, setIgnitionPin);
	addConsoleActionSS(CMD_TRIGGER_PIN, setTriggerInputPin);
	addConsoleActionSS(CMD_TRIGGER_SIMULATOR_PIN, setTriggerSimulatorPin);

	addConsoleActionI(CMD_ECU_UNLOCK, unlockEcu);

	addConsoleActionS(CMD_ALTERNATOR_PIN, setAlternatorPin);
	addConsoleActionS(CMD_IDLE_PIN, setIdlePin);

#if HAL_USE_ADC
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
			return;
		}
	}
	efiPrintf("date_set Date parameter %s is wrong", isoDateTime);
#else // EFI_RTC
	efiPrintf("Cannot set time: RTC not supported");
#endif // EFI_RTC
}

#endif // ! EFI_UNIT_TEST

void setEngineType(int value) {
	setEngineType((engine_type_e)value);
}

void setEngineType(engine_type_e value) {
	{
#if EFI_PROD_CODE
		chibios_rt::CriticalSectionLocker csl;
#endif // EFI_PROD_CODE

		engineConfiguration->engineType = value;
		resetConfigurationExt(value);
		engine->resetEngineSnifferIfInTestMode();

#if EFI_INTERNAL_FLASH
		writeToFlashNow();
#endif // EFI_INTERNAL_FLASH
	}

	incrementGlobalConfigurationVersion();
#if ! EFI_UNIT_TEST
#endif // ! EFI_UNIT_TEST
}
