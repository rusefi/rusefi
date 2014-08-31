/**
 * @file settings.cpp
 * @brief This file is about configuring engine via the human-readable protocol
 *
 * @date Dec 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "settings.h"
#include "eficonsole.h"
#include "engine_configuration.h"
#include "adc_inputs.h"
#include "engine_controller.h"
#include "thermistors.h"
#include "adc_inputs.h"
#include "interpolation.h"
#include "tps.h"
#include "ec2.h"
#include "map.h"
#include "trigger_decoder.h"
#include "console_io.h"

#if EFI_PROD_CODE
#include "rusefi.h"
#include "pin_repository.h"
#include "hardware.h"
#endif /* EFI_PROD_CODE */

#if EFI_INTERNAL_FLASH
#include "flash_main.h"
#endif /* EFI_INTERNAL_FLASH */

static Logging logger;

static char LOGGING_BUFFER[1000];

/*
 static void printIntArray(int array[], int size) {
 for (int j = 0; j < size; j++) {
 print("%d ", array[j]);
 }
 print("\r\n");
 }
 */

void printFloatArray(const char *prefix, float array[], int size) {
	appendMsgPrefix(&logger);
	appendPrintf(&logger, prefix);
	for (int j = 0; j < size; j++) {
		appendPrintf(&logger, "%f ", array[j]);
	}
	appendMsgPostfix(&logger);
	scheduleLogging(&logger);
}

const char* getConfigurationName(engine_configuration_s *engineConfiguration) {
	switch (engineConfiguration->engineType) {
#if EFI_SUPPORT_DODGE_NEON
	case DODGE_NEON_1995:
		return "Dodge Neon";
#endif /* EFI_SUPPORT_DODGE_NEON */
#if EFI_SUPPORT_FORD_ASPIRE
	case FORD_ASPIRE_1996:
		return "Ford Aspire";
#endif /* EFI_SUPPORT_FORD_ASPIRE */
#if EFI_SUPPORT_FORD_FIESTA
	case FORD_FIESTA:
		return "Ford Fiesta";
#endif /* EFI_SUPPORT_FORD_FIESTA */
#if EFI_SUPPORT_NISSAN_PRIMERA
	case NISSAN_PRIMERA:
		return "Nissan Primera";
#endif /* EFI_SUPPORT_NISSAN_PRIMERA */
	case HONDA_ACCORD_CD:
		return "Honda Accord 3w";
	case HONDA_ACCORD_CD_TWO_WIRES:
		return "Honda Accord 2w";
	case HONDA_ACCORD_CD_DIP:
		return "Honda Dip";
	case FORD_INLINE_6_1995:
		return "Ford 1995 inline 6";
	case GY6_139QMB:
		return "Gy6 139qmb";
	case MAZDA_MIATA_NB:
		return "Mazda Miata NB";
	case MAZDA_323:
		return "Mazda 323";
	case SATURN_ION_2004:
		return "Saturn Ion";
	case MINI_COOPER_R50:
		return "Mini Cooper R50";
	case FORD_ESCORT_GT:
		return "Ford Escort GT";
	case CITROEN_TU3JP:
		return "Citroen TU3JP";
	case ROVER_V8:
		return "Rover v8";
	case MITSU_4G93:
		return "Mitsu 4G93";
	case MIATA_1990:
		return "Miata 1990";
	case MIATA_1994:
		return "Miata 1994";
	case MIATA_1996:
		return "Miata 1996";
	default:
		firmwareError("Unexpected: engineType %d", engineConfiguration->engineType);
		return NULL;
	}
}

extern board_configuration_s *boardConfiguration;

/**
 * @brief	Prints current engine configuration to human-readable console.
 */
void printConfiguration(engine_configuration_s *engineConfiguration, engine_configuration2_s *engineConfiguration2) {

	scheduleMsg(&logger, "Template %s/%d trigger %d", getConfigurationName(engineConfiguration),
			engineConfiguration->engineType, engineConfiguration->triggerConfig.triggerType);

	scheduleMsg(&logger, "configurationVersion=%d", getGlobalConfigurationVersion());

	for (int k = 0; k < FUEL_LOAD_COUNT; k++) {
//		print("line %d (%f): ", k, engineConfiguration->fuelKeyBins[k]);
//		for (int r = 0; r < FUEL_RPM_COUNT; r++) {
//			print("%f ", engineConfiguration->fuelTable[k][r]);
//		}
//		print("\r\n");
	}

	printFloatArray("RPM bin: ", engineConfiguration->fuelRpmBins, FUEL_RPM_COUNT);

	printFloatArray("Y bin: ", engineConfiguration->fuelLoadBins, FUEL_LOAD_COUNT);

	printFloatArray("CLT: ", engineConfiguration->cltFuelCorr, CLT_CURVE_SIZE);
	printFloatArray("CLT bins: ", engineConfiguration->cltFuelCorrBins, CLT_CURVE_SIZE);

	printFloatArray("IAT: ", engineConfiguration->iatFuelCorr, IAT_CURVE_SIZE);
	printFloatArray("IAT bins: ", engineConfiguration->iatFuelCorrBins, IAT_CURVE_SIZE);

	printFloatArray("vBatt: ", engineConfiguration->battInjectorLagCorr, VBAT_INJECTOR_CURVE_SIZE);
	printFloatArray("vBatt bins: ", engineConfiguration->battInjectorLagCorrBins, VBAT_INJECTOR_CURVE_SIZE);

//	appendMsgPrefix(&logger);

	scheduleMsg(&logger, "rpmHardLimit: %d/rpmMultiplier=%f", engineConfiguration->rpmHardLimit,
			engineConfiguration->rpmMultiplier);

	scheduleMsg(&logger, "tpsMin: %d/tpsMax: %d", engineConfiguration->tpsMin, engineConfiguration->tpsMax);

	scheduleMsg(&logger, "ignitionMode: %d/enabled=%s", engineConfiguration->ignitionMode,
			boolToString(engineConfiguration->isIgnitionEnabled));
	scheduleMsg(&logger, "timingMode: %d", engineConfiguration->timingMode);
	scheduleMsg(&logger, "fixedModeTiming: %d", (int) engineConfiguration->fixedModeTiming);
	scheduleMsg(&logger, "ignitionOffset=%f", engineConfiguration->ignitionOffset);
	scheduleMsg(&logger, "injectionOffset=%f/enabled=%s", (double) engineConfiguration->injectionOffset,
			boolToString(engineConfiguration->isInjectionEnabled));

	scheduleMsg(&logger, "crankingChargeAngle=%f", engineConfiguration->crankingChargeAngle);
	scheduleMsg(&logger, "crankingTimingAngle=%f", engineConfiguration->crankingTimingAngle);
	scheduleMsg(&logger, "globalTriggerAngleOffset=%f", engineConfiguration->globalTriggerAngleOffset);

//	scheduleMsg(&logger, "analogChartMode: %d", engineConfiguration->analogChartMode);

	scheduleMsg(&logger, "crankingRpm: %d", engineConfiguration->crankingSettings.crankingRpm);

	scheduleMsg(&logger, "analogInputDividerCoefficient: %f", engineConfiguration->analogInputDividerCoefficient);

	#if EFI_PROD_CODE
	scheduleMsg(&logger, "idlePinMode: %s", pinModeToString(boardConfiguration->idleValvePinMode));
	scheduleMsg(&logger, "malfunctionIndicatorPinMode: %s",
			pinModeToString(boardConfiguration->malfunctionIndicatorPinMode));

	scheduleMsg(&logger, "idleValvePin: %s", hwPortname(boardConfiguration->idleValvePin));
	scheduleMsg(&logger, "fuelPumpPin: mode %s @ %s", pinModeToString(boardConfiguration->fuelPumpPinMode),
			hwPortname(boardConfiguration->fuelPumpPin));

	scheduleMsg(&logger, "injectionPins: mode %s", pinModeToString(boardConfiguration->injectionPinMode));
	for (int i = 0; i < engineConfiguration->cylindersCount; i++) {
		brain_pin_e brainPin = boardConfiguration->injectionPins[i];

		scheduleMsg(&logger, "injection %d @ %s", i, hwPortname(brainPin));
	}

	scheduleMsg(&logger, "ignitionPins: mode %s", pinModeToString(boardConfiguration->ignitionPinMode));
	// todo: calculate coils count based on ignition mode
	for (int i = 0; i < 4; i++) {
		brain_pin_e brainPin = boardConfiguration->ignitionPins[i];
		scheduleMsg(&logger, "ignition %d @ %s", i, hwPortname(brainPin));
	}

	scheduleMsg(&logger, "boardTestModeJumperPin: %s", hwPortname(boardConfiguration->boardTestModeJumperPin));

	scheduleMsg(&logger, "digitalPotentiometerSpiDevice %d", boardConfiguration->digitalPotentiometerSpiDevice);

	for (int i = 0; i < DIGIPOT_COUNT; i++) {
		scheduleMsg(&logger, "digitalPotentiometer CS%d %s", i,
				hwPortname(boardConfiguration->digitalPotentiometerChipSelect[i]));
	}

	scheduleMsg(&logger, "spi 1=%s/2=%s/3=%s", boolToString(boardConfiguration->is_enabled_spi_1),
			boolToString(boardConfiguration->is_enabled_spi_2), boolToString(boardConfiguration->is_enabled_spi_3));

#endif /* EFI_PROD_CODE */
}

extern engine_configuration_s *engineConfiguration;
extern engine_configuration2_s *engineConfiguration2;

static void doPrintConfiguration(void) {
	printConfiguration(engineConfiguration, engineConfiguration2);
}

static void setFixedModeTiming(int value) {
	engineConfiguration->fixedModeTiming = value;
	doPrintConfiguration();
	incrementGlobalConfigurationVersion();
}

static void setTimingMode(int value) {
	engineConfiguration->timingMode = (timing_mode_e) value;
	doPrintConfiguration();
	incrementGlobalConfigurationVersion();
}

static void setIdleMode(int mode) {
	engineConfiguration->idleMode = (idle_mode_e) mode;
}

void setEngineType(int value) {
	engineConfiguration->engineType = (engine_type_e) value;
	resetConfigurationExt(&logger, (engine_type_e) value, engineConfiguration, engineConfiguration2,
			boardConfiguration);
#if EFI_INTERNAL_FLASH
	writeToFlash();
//	scheduleReset();
#endif /* EFI_PROD_CODE */
	incrementGlobalConfigurationVersion();
	doPrintConfiguration();
}

static void setInjectionPinMode(int value) {
	boardConfiguration->injectionPinMode = (pin_output_mode_e) value;
	doPrintConfiguration();
}

static void setIgnitionPinMode(int value) {
	boardConfiguration->ignitionPinMode = (pin_output_mode_e) value;
	doPrintConfiguration();
}

static void setIdlePin(int value) {
	boardConfiguration->idleValvePin = (brain_pin_e) value;
	doPrintConfiguration();
}

static void setIdlePinMode(int value) {
	boardConfiguration->idleValvePinMode = (pin_output_mode_e) value;
	doPrintConfiguration();
}

static void setInjectionOffset(int value) {
	engineConfiguration->injectionOffset = value;
	doPrintConfiguration();
	incrementGlobalConfigurationVersion();
}

static void setIgnitionOffset(int value) {
	engineConfiguration->ignitionOffset = value;
	doPrintConfiguration();
	incrementGlobalConfigurationVersion();
}

static void setFuelPumpPinMode(int value) {
	boardConfiguration->fuelPumpPinMode = (pin_output_mode_e) value;
	doPrintConfiguration();
}

static void setMalfunctionIndicatorPinMode(int value) {
	boardConfiguration->malfunctionIndicatorPinMode = (pin_output_mode_e) value;
	doPrintConfiguration();
}

static void setAnalogChartMode(int value) {
	engineConfiguration->analogChartMode = value;
	doPrintConfiguration();
}

static void setRpmMultiplier(int value) {
	engineConfiguration->rpmMultiplier = value;
	doPrintConfiguration();
}

static char pinNameBuffer[16];

static void printThermistor(const char *msg, Thermistor *thermistor) {
	adc_channel_e adcChannel = thermistor->channel;
	float voltage = getVoltageDivided(adcChannel);
	float r = getResistance(thermistor);

	float t = getTemperatureC(thermistor);

	scheduleMsg(&logger, "%s v=%f C=%f R=%f on channel %d", msg, voltage, t, r, adcChannel);
	scheduleMsg(&logger, "bias=%f A=%f B=%f C=%f", thermistor->config->bias_resistor, thermistor->config->s_h_a,
			thermistor->config->s_h_b, thermistor->config->s_h_c);
#if EFI_ANALOG_INPUTS
	scheduleMsg(&logger, "@%s", getPinNameByAdcChannel(adcChannel, pinNameBuffer));
#endif
}

#if EFI_PROD_CODE
static void printMAPInfo(void) {
	scheduleMsg(&logger, "map type=%d raw=%f MAP=%f", engineConfiguration->map.sensor.sensorType, getRawMap(),
			getMap());
	if (engineConfiguration->map.sensor.sensorType == MT_CUSTOM) {
		scheduleMsg(&logger, "min=%f max=%f", engineConfiguration->map.sensor.Min, engineConfiguration->map.sensor.Max);
	}

	scheduleMsg(&logger, "baro type=%d value=%f", engineConfiguration->baroSensor.sensorType, getBaroPressure());
	if (engineConfiguration->baroSensor.sensorType == MT_CUSTOM) {
		scheduleMsg(&logger, "min=%f max=%f", engineConfiguration->baroSensor.Min, engineConfiguration->baroSensor.Max);
	}
}
#endif

static void printTPSInfo(void) {
#if EFI_PROD_CODE
	GPIO_TypeDef* port = getAdcChannelPort(engineConfiguration->tpsAdcChannel);
	int pin = getAdcChannelPin(engineConfiguration->tpsAdcChannel);

	scheduleMsg(&logger, "tps min %d/max %d v=%f @%s%d", engineConfiguration->tpsMin, engineConfiguration->tpsMax,
			getTPSVoltage(), portname(port), pin);
#endif
	scheduleMsg(&logger, "current 10bit=%d value=%f rate=%f", getTPS10bitAdc(), getTPS(), getTpsRateOfChange());
}

static void printTemperatureInfo(void) {
	printThermistor("CLT", &engineConfiguration2->clt);
	if (!isValidCoolantTemperature(getCoolantTemperature())) {
		scheduleMsg(&logger, "CLT sensing error");
	}
	printThermistor("IAT", &engineConfiguration2->iat);
	if (!isValidIntakeAirTemperature(getIntakeAirTemperature())) {
		scheduleMsg(&logger, "IAT sensing error");
	}

	float rClt = getResistance(&engineConfiguration2->clt);
	float rIat = getResistance(&engineConfiguration2->iat);

#if EFI_ANALOG_INPUTS
	adc_channel_e cltChannel = engineConfiguration2->clt.channel;
	scheduleMsg(&logger, "CLT R=%f on channel %d@%s", rClt, cltChannel,
			getPinNameByAdcChannel(cltChannel, pinNameBuffer));
	adc_channel_e iatChannel = engineConfiguration2->iat.channel;
	scheduleMsg(&logger, "IAT R=%f on channel %d@%s", rIat, iatChannel,
			getPinNameByAdcChannel(iatChannel, pinNameBuffer));

	scheduleMsg(&logger, "cranking fuel %fms @ %fC", engineConfiguration->crankingSettings.fuelAtMinTempMs,
			engineConfiguration->crankingSettings.coolantTempMinC);
	scheduleMsg(&logger, "cranking fuel %fms @ %fC", engineConfiguration->crankingSettings.fuelAtMaxTempMs,
			engineConfiguration->crankingSettings.coolantTempMaxC);
#endif
}

/**
 * For example
 * set_cranking_fuel_min 15 0
 * would be 15ms @ 0C
 */
static void setCrankingFuelMin(int timeMs, int tempC) {
	engineConfiguration->crankingSettings.coolantTempMinC = tempC;
	engineConfiguration->crankingSettings.fuelAtMinTempMs = timeMs;
	printTemperatureInfo();
}

static void setCrankingRpm(int value) {
	engineConfiguration->crankingSettings.crankingRpm = value;
	doPrintConfiguration();
}

static void setAlgorithm(int value) {
	engineConfiguration->algorithm = (engine_load_mode_e) value;
	doPrintConfiguration();
}

static void setFiringOrder(int value) {
	engineConfiguration->firingOrder = (firing_order_e) value;
	doPrintConfiguration();
}

static void setRpmHardLimit(int value) {
	engineConfiguration->rpmHardLimit = value;
	doPrintConfiguration();
}

static void setCrankingFuelMax(int timeMs, int tempC) {
	engineConfiguration->crankingSettings.coolantTempMaxC = tempC;
	engineConfiguration->crankingSettings.fuelAtMaxTempMs = timeMs;
	printTemperatureInfo();
}

static void setGlobalTriggerAngleOffset(int value) {
	engineConfiguration->globalTriggerAngleOffset = value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration();
}

static void setCrankingTimingAngle(float value) {
	engineConfiguration->crankingTimingAngle = value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration();
}

static void setCrankingInjectionMode(int value) {
	engineConfiguration->crankingInjectionMode = (injection_mode_e) value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration();
}

static void setInjectionMode(int value) {
	engineConfiguration->injectionMode = (injection_mode_e) value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration();
}

static void setIgnitionMode(int value) {
	engineConfiguration->ignitionMode = (ignition_mode_e) value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration();
}

static void setTriggerType(int value) {
	engineConfiguration->triggerConfig.triggerType = (trigger_type_e) value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration();
}

static void setToothedWheel(int total, int skipped) {
	setToothedWheelConfiguration(&engineConfiguration2->triggerShape, total, skipped, engineConfiguration);
//	initializeTriggerShape(&logger, engineConfiguration, engineConfiguration2);
	incrementGlobalConfigurationVersion();
	doPrintConfiguration();
}

static void setCrankingChargeAngle(float value) {
	engineConfiguration->crankingChargeAngle = value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration();
}

static void setGlobalFuelCorrection(float value) {
	if (value < 0.01 || value > 50)
		return;
	scheduleMsg(&logger, "setting fuel mult=%f", value);
	engineConfiguration->globalFuelCorrection = value;
}

static void setVBattDivider(float value) {
	engineConfiguration->vbattDividerCoeff = value;
}

static void setWholeTimingMap(float value) {
	// todo: table helper?
	scheduleMsg(&logger, "Setting whole timing map to %f", value);
	for (int l = 0; l < IGN_LOAD_COUNT; l++) {
		for (int r = 0; r < IGN_RPM_COUNT; r++) {
			engineConfiguration->ignitionTable[l][r] = value;
		}
	}
}

static void setWholeFuelMapCmd(float value) {
	scheduleMsg(&logger, "Setting whole fuel map to %f", value);
	if (engineConfiguration->algorithm == LM_SPEED_DENSITY) {
		scheduleMsg(&logger, "WARNING: setting fuel map in SD mode is pointless");
	}
	setWholeFuelMap(engineConfiguration, value);
}

#if EFI_PROD_CODE
static void setTriggerInputPin(const char *indexStr, const char *pinName) {
	int index = atoi(indexStr);
	if (index < 0 || index > 2)
		return;
	brain_pin_e pin = parseBrainPin(pinName);
	// todo: extract method - code duplication with other 'set_xxx_pin' methods?
	if (pin == GPIO_INVALID) {
		scheduleMsg(&logger, "invalid pin name [%s]", pinName);
		return;
	}
	scheduleMsg(&logger, "setting trigger pin[%d] to %s please save&restart", index, hwPortname(pin));
	boardConfiguration->triggerInputPins[index] = pin;
}

static void setTriggerSimulatorMode(const char *indexStr, const char *modeCode) {
	int index = atoi(indexStr);
	if (index < 0 || index > 2 || absI(index) == ERROR_CODE) {
		return;
	}
	int mode = atoi(modeCode);
	if (absI(mode) == ERROR_CODE) {
		return;
	}
	boardConfiguration->triggerSimulatorPinModes[index] = (pin_output_mode_e) mode;
}

static void setTriggerSimulatorPin(const char *indexStr, const char *pinName) {
	int index = atoi(indexStr);
	if (index < 0 || index > 2)
		return;
	brain_pin_e pin = parseBrainPin(pinName);
	if (pin == GPIO_INVALID) {
		scheduleMsg(&logger, "invalid pin name [%s]", pinName);
		return;
	}
	scheduleMsg(&logger, "setting trigger simulator pin[%d] to %s please save&restart", index, hwPortname(pin));
	boardConfiguration->triggerSimulatorPins[index] = pin;
}

static void setAnalogInputPin(const char *sensorStr, const char *pinName) {
	brain_pin_e pin = parseBrainPin(pinName);
	if (pin == GPIO_INVALID) {
		scheduleMsg(&logger, "invalid pin name [%s]", pinName);
		return;
	}
	adc_channel_e channel = getAdcChannel(pin);
	if (channel == EFI_ADC_ERROR) {
		scheduleMsg(&logger, "Error with [%s]", pinName);
		return;
	}
	if (strEqual("map", sensorStr)) {
		engineConfiguration->map.sensor.hwChannel = channel;
		scheduleMsg(&logger, "setting MAP to %s/%d", pinName, channel);
	} else if (strEqual("clt", sensorStr)) {
		engineConfiguration->cltAdcChannel = channel;
		scheduleMsg(&logger, "setting CLT to %s/%d", pinName, channel);
	} else if (strEqual("iat", sensorStr)) {
		engineConfiguration->iatAdcChannel = channel;
		scheduleMsg(&logger, "setting IAT to %s/%d", pinName, channel);
	} else if (strEqual("tps", sensorStr)) {
		engineConfiguration->tpsAdcChannel = channel;
		scheduleMsg(&logger, "setting TPS to %s/%d", pinName, channel);
	}
}

static void setLogicInputPin(const char *indexStr, const char *pinName) {
	int index = atoi(indexStr);
	if (index < 0 || index > 2) {
		return;
	}
	brain_pin_e pin = parseBrainPin(pinName);
	if (pin == GPIO_INVALID) {
		scheduleMsg(&logger, "invalid pin name [%s]", pinName);
		return;
	}
	scheduleMsg(&logger, "setting logic input pin[%d] to %s please save&restart", index, hwPortname(pin));
	boardConfiguration->logicAnalyzerPins[index] = pin;
}
#endif /* EFI_PROD_CODE */

static void setTimingMap(const char * rpmStr, const char *loadStr, const char *valueStr) {
	float rpm = atoff(rpmStr);
	float engineLoad = atoff(loadStr);
	float value = atoff(valueStr);

	int rpmIndex = findIndex(engineConfiguration->ignitionRpmBins, IGN_RPM_COUNT, rpm);
	rpmIndex = rpmIndex < 0 ? 0 : rpmIndex;
	int loadIndex = findIndex(engineConfiguration->ignitionLoadBins, IGN_LOAD_COUNT, engineLoad);
	loadIndex = loadIndex < 0 ? 0 : loadIndex;

	engineConfiguration->ignitionTable[loadIndex][rpmIndex] = value;
	scheduleMsg(&logger, "Setting timing map entry %d:%d to %f", rpmIndex, loadIndex, value);
}

static void setFuelMap(const char * rpmStr, const char *loadStr, const char *valueStr) {
	float rpm = atoff(rpmStr);
	float engineLoad = atoff(loadStr);
	float value = atoff(valueStr);

	int rpmIndex = findIndex(engineConfiguration->fuelRpmBins, FUEL_RPM_COUNT, rpm);
	rpmIndex = rpmIndex < 0 ? 0 : rpmIndex;
	int loadIndex = findIndex(engineConfiguration->fuelLoadBins, FUEL_LOAD_COUNT, engineLoad);
	loadIndex = loadIndex < 0 ? 0 : loadIndex;

	engineConfiguration->fuelTable[loadIndex][rpmIndex] = value;
	scheduleMsg(&logger, "Setting fuel map entry %d:%d to %f", rpmIndex, loadIndex, value);
}

static void setSpiMode(int index, bool mode) {
	switch (index) {
	case 1:
		boardConfiguration->is_enabled_spi_1 = mode;
		break;
	case 2:
		boardConfiguration->is_enabled_spi_2 = mode;
		break;
	case 3:
		boardConfiguration->is_enabled_spi_3 = mode;
		break;
	default:
		scheduleMsg(&logger, "invalid spi index %d", index);
		return;
	}
	scheduleMsg(&logger, "spi %d mode: %s", index, boolToString(mode));
}

static void enableSpi(int index) {
	setSpiMode(index, true);
}

static void disableSpi(int index) {
	setSpiMode(index, false);
}

static void enableInjection(void) {
	engineConfiguration->isInjectionEnabled = true;
	scheduleMsg(&logger, "injection enabled");
}

static void disableInjection(void) {
	engineConfiguration->isInjectionEnabled = false;
	scheduleMsg(&logger, "injection disabled");
}

static void enableIgnition(void) {
	engineConfiguration->isIgnitionEnabled = true;
	scheduleMsg(&logger, "ignition enabled");
}

static void disableIgnition(void) {
	engineConfiguration->isIgnitionEnabled = false;
	scheduleMsg(&logger, "ignition disabled");
}

static void enableSelfStimulation(void) {
	engineConfiguration->directSelfStimulation = true;
	scheduleMsg(&logger, "self stimulation enabled");
}

static void disableSelfStimulation(void) {
	engineConfiguration->directSelfStimulation = false;
	scheduleMsg(&logger, "self stimulation disabled");
}

#if EFI_WAVE_CHART
extern int waveChartUsedSize;
#endif

static void printAllInfo(void) {
	printTemperatureInfo();
	printTPSInfo();
#if EFI_WAVE_CHART
	scheduleMsg(&logger, "waveChartUsedSize=%d", waveChartUsedSize);
#endif
#if EFI_PROD_CODE
	printMAPInfo();
	scheduleMsg(&logger, "console mode jumper: %s", boolToString(!GET_CONSOLE_MODE_VALUE()));
	scheduleMsg(&logger, "board test mode jumper: %s", boolToString(GET_BOARD_TEST_MODE_VALUE()));
#endif
}

void initSettings(void) {
	initLoggingExt(&logger, "settings control", LOGGING_BUFFER, sizeof(LOGGING_BUFFER));

	addConsoleAction("showconfig", doPrintConfiguration);
	addConsoleAction("tempinfo", printTemperatureInfo);
	addConsoleAction("tpsinfo", printTPSInfo);
	addConsoleAction("info", printAllInfo);

	addConsoleActionI("set_ignition_offset", setIgnitionOffset);
	addConsoleActionI("set_injection_offset", setInjectionOffset);
	addConsoleActionI("set_global_trigger_offset_angle", setGlobalTriggerAngleOffset);
	addConsoleActionI("set_analog_chart_mode", setAnalogChartMode);
	addConsoleActionI("set_fixed_mode_timing", setFixedModeTiming);
	addConsoleActionI("set_timing_mode", setTimingMode);
	addConsoleActionI("set_engine_type", setEngineType);
	addConsoleActionI("set_idle_mode", setIdleMode);

	addConsoleActionI("set_injection_pin_mode", setInjectionPinMode);
	addConsoleActionI("set_ignition_pin_mode", setIgnitionPinMode);
	addConsoleActionI("set_idle_pin", setIdlePin);
	addConsoleActionI("set_idle_pin_mode", setIdlePinMode);
	addConsoleActionI("set_fuel_pump_pin_mode", setFuelPumpPinMode);
	addConsoleActionI("set_malfunction_indicator_pin_mode", setMalfunctionIndicatorPinMode);
	addConsoleActionI("set_rpm_multiplier", setRpmMultiplier);
	// todo: start saving values into flash right away?

	addConsoleActionF("set_global_fuel_correction", setGlobalFuelCorrection);

	addConsoleActionII("set_cranking_fuel_min", setCrankingFuelMin);
	addConsoleActionII("set_cranking_fuel_max", setCrankingFuelMax);
	addConsoleActionI("set_cranking_rpm", setCrankingRpm);
	addConsoleActionF("set_cranking_timing_angle", setCrankingTimingAngle);
	addConsoleActionF("set_cranking_charge_angle", setCrankingChargeAngle);
	addConsoleActionI("set_ignition_mode", setIgnitionMode);
	addConsoleActionI("set_cranking_injection_mode", setCrankingInjectionMode);
	addConsoleActionI("set_injection_mode", setInjectionMode);

	addConsoleActionF("set_whole_fuel_map", setWholeFuelMapCmd);
	addConsoleActionSSS("set_fuel_map", setFuelMap);

	addConsoleActionF("set_whole_timing_map", setWholeTimingMap);
	addConsoleActionSSS("set_timing_map", setTimingMap);

	addConsoleActionI("set_rpm_hard_limit", setRpmHardLimit);
	addConsoleActionI("set_firing_order", setFiringOrder);
	addConsoleActionI("set_algorithm", setAlgorithm);

	// todo: refactor this - looks like all boolean flags should be controlled with less code duplication
	addConsoleAction("enable_injection", enableInjection);
	addConsoleAction("disable_injection", disableInjection);
	addConsoleAction("enable_ignition", enableIgnition);
	addConsoleAction("disable_ignition", disableIgnition);
	addConsoleAction("enable_self_stimulation", enableSelfStimulation);
	addConsoleAction("disable_self_stimulation", disableSelfStimulation);

	addConsoleActionI("enable_spi", enableSpi);
	addConsoleActionI("disable_spi", disableSpi);

	addConsoleActionII("set_toothed_wheel", setToothedWheel);
	addConsoleActionI("set_trigger_type", setTriggerType);

	addConsoleActionF("set_vbatt_divider", setVBattDivider);

#if EFI_PROD_CODE
	addConsoleActionSS("set_trigger_input_pin", setTriggerInputPin);
	addConsoleActionSS("set_trigger_simulator_pin", setTriggerSimulatorPin);
	addConsoleActionSS("set_trigger_simulator_mode", setTriggerSimulatorMode);

	addConsoleAction("mapinfo", printMAPInfo);
	addConsoleActionSS("set_analog_input_pin", setAnalogInputPin);
	addConsoleActionSS("set_logic_input_pin", setLogicInputPin);
#endif /* EFI_PROD_CODE */
}

