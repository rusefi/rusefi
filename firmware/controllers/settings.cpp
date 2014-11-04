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
#include "engine.h"

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

void printSpiState(Logging *logger, board_configuration_s *boardConfiguration) {
	scheduleMsg(logger, "spi 1=%s/2=%s/3=%s", boolToString(boardConfiguration->is_enabled_spi_1),
			boolToString(boardConfiguration->is_enabled_spi_2), boolToString(boardConfiguration->is_enabled_spi_3));
}

extern board_configuration_s *boardConfiguration;

/**
 * @brief	Prints current engine configuration to human-readable console.
 */
void printConfiguration(engine_configuration_s *engineConfiguration, engine_configuration2_s *engineConfiguration2) {

	scheduleMsg(&logger, "Template %s/%d trigger %s", getConfigurationName(engineConfiguration->engineType),
			engineConfiguration->engineType, getTrigger_type_e(engineConfiguration->triggerConfig.triggerType));

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

	scheduleMsg(&logger, "ignitionMode: %s/enabled=%s", getIgnition_mode_e(engineConfiguration->ignitionMode),
			boolToString(engineConfiguration->isIgnitionEnabled));
	scheduleMsg(&logger, "timingMode: %d", /*getTiming_mode_etodo*/(engineConfiguration->timingMode));
	scheduleMsg(&logger, "fixedModeTiming: %d", (int) engineConfiguration->fixedModeTiming);
	scheduleMsg(&logger, "ignitionOffset=%f", engineConfiguration->ignitionOffset);
	scheduleMsg(&logger, "injectionOffset=%f/enabled=%s", (double) engineConfiguration->injectionOffset,
			boolToString(engineConfiguration->isInjectionEnabled));

	scheduleMsg(&logger, "map_avg=%s/mil=%s/fp=%s/ts=%s/wa=%s/it=%s/fastAdc=%s",
			boolToString(engineConfiguration->isMapAveragingEnabled), boolToString(engineConfiguration->isMilEnabled),
			boolToString(engineConfiguration->isFuelPumpEnabled),
			boolToString(engineConfiguration->isTunerStudioEnabled),
			boolToString(engineConfiguration->isWaveAnalyzerEnabled),
			boolToString(engineConfiguration->isIdleThreadEnabled), boolToString(boardConfiguration->isFastAdcEnabled));

	scheduleMsg(&logger, "isManualSpinningMode=%s/isCylinderCleanupEnabled=%s",
			boolToString(engineConfiguration->isManualSpinningMode),
			boolToString(engineConfiguration->isCylinderCleanupEnabled));

	scheduleMsg(&logger, "crankingChargeAngle=%f", engineConfiguration->crankingChargeAngle);
	scheduleMsg(&logger, "crankingTimingAngle=%f", engineConfiguration->crankingTimingAngle);
	scheduleMsg(&logger, "globalTriggerAngleOffset=%f", engineConfiguration->globalTriggerAngleOffset);

//	scheduleMsg(&logger, "analogChartMode: %d", engineConfiguration->analogChartMode);

	scheduleMsg(&logger, "crankingRpm: %d", engineConfiguration->crankingSettings.crankingRpm);

	scheduleMsg(&logger, "analogInputDividerCoefficient: %f", engineConfiguration->analogInputDividerCoefficient);

	scheduleMsg(&logger, "idlePin: mode %s @ %s freq=%d", getPin_output_mode_e(boardConfiguration->idleValvePinMode),
			hwPortname(boardConfiguration->idleValvePin), boardConfiguration->idleSolenoidFrequency);
	scheduleMsg(&logger, "malfunctionIndicatorn: %s mode=%s",
			hwPortname(boardConfiguration->malfunctionIndicatorPin),
			pinModeToString(boardConfiguration->malfunctionIndicatorPinMode));

	scheduleMsg(&logger, "fuelPumpPin: mode %s @ %s", getPin_output_mode_e(boardConfiguration->fuelPumpPinMode),
			hwPortname(boardConfiguration->fuelPumpPin));

	scheduleMsg(&logger, "injectionPins: mode %s", pinModeToString(boardConfiguration->injectionPinMode));
	for (int i = 0; i < engineConfiguration->cylindersCount; i++) {
		brain_pin_e brainPin = boardConfiguration->injectionPins[i];

		scheduleMsg(&logger, "injection %d @ %s", i, hwPortname(brainPin));
	}

	scheduleMsg(&logger, "ignitionPins: mode %s", pinModeToString(boardConfiguration->ignitionPinMode));
	for (int i = 0; i < engineConfiguration->cylindersCount; i++) {
		brain_pin_e brainPin = boardConfiguration->ignitionPins[i];
		scheduleMsg(&logger, "ignition %d @ %s", i, hwPortname(brainPin));
	}

	scheduleMsg(&logger, "boardTestModeJumperPin: %s", hwPortname(boardConfiguration->boardTestModeJumperPin));

	scheduleMsg(&logger, "digitalPotentiometerSpiDevice %d", boardConfiguration->digitalPotentiometerSpiDevice);

	for (int i = 0; i < DIGIPOT_COUNT; i++) {
		scheduleMsg(&logger, "digitalPotentiometer CS%d %s", i,
				hwPortname(boardConfiguration->digitalPotentiometerChipSelect[i]));
	}
#if EFI_PROD_CODE

	printSpiState(&logger, boardConfiguration);

#endif /* EFI_PROD_CODE */
	for (int i = 0; i < LE_COMMAND_COUNT; i++) {
		char * exp = boardConfiguration->le_formulas[i];
		if (exp[0] != 0) {
			scheduleMsg(&logger, "user out %d [%s] at %s", i, exp, hwPortname(boardConfiguration->gpioPins[i]));
		}
	}
}

extern engine_configuration_s *engineConfiguration;
extern engine_configuration2_s *engineConfiguration2;

static void doPrintConfiguration(Engine *engine) {
	printConfiguration(engineConfiguration, engine->engineConfiguration2);
}

extern Engine engine;

static void setFixedModeTiming(int value) {
	engineConfiguration->fixedModeTiming = value;
	doPrintConfiguration(&engine);
	incrementGlobalConfigurationVersion();
}

static void setTimingMode(int value) {
	engineConfiguration->timingMode = (timing_mode_e) value;
	doPrintConfiguration(&engine);
	incrementGlobalConfigurationVersion();
}

static void setIdleMode(int mode) {
	engineConfiguration->idleMode = (idle_mode_e) mode;
}

void setEngineType(int value) {
	engineConfiguration->engineType = (engine_type_e) value;
	resetConfigurationExt(&logger, (engine_type_e) value, &engine);
#if EFI_INTERNAL_FLASH
	writeToFlash();
//	scheduleReset();
#endif /* EFI_PROD_CODE */
	incrementGlobalConfigurationVersion();
	doPrintConfiguration(&engine);
}

static void setIdleSolenoidFrequency(int value) {
	boardConfiguration->idleSolenoidFrequency = value;
}

static void setInjectionPinMode(int value) {
	boardConfiguration->injectionPinMode = (pin_output_mode_e) value;
	doPrintConfiguration(&engine);
}

static void setIgnitionPinMode(int value) {
	boardConfiguration->ignitionPinMode = (pin_output_mode_e) value;
	doPrintConfiguration(&engine);
}

static void setIdlePinMode(int value) {
	boardConfiguration->idleValvePinMode = (pin_output_mode_e) value;
	doPrintConfiguration(&engine);
}

static void setInjectionOffset(int value) {
	engineConfiguration->injectionOffset = value;
	doPrintConfiguration(&engine);
	incrementGlobalConfigurationVersion();
}

static void setIgnitionOffset(int value) {
	engineConfiguration->ignitionOffset = value;
	doPrintConfiguration(&engine);
	incrementGlobalConfigurationVersion();
}

static void setFuelPumpPinMode(int value) {
	boardConfiguration->fuelPumpPinMode = (pin_output_mode_e) value;
	doPrintConfiguration(&engine);
}

static void setMalfunctionIndicatorPinMode(int value) {
	boardConfiguration->malfunctionIndicatorPinMode = (pin_output_mode_e) value;
	doPrintConfiguration(&engine);
}

static void setAnalogChartMode(int value) {
	engineConfiguration->analogChartMode = value;
	doPrintConfiguration(&engine);
}

static void setRpmMultiplier(int value) {
	engineConfiguration->rpmMultiplier = value;
	doPrintConfiguration(&engine);
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

	float value = getResistance(thermistor);

	scheduleMsg(&logger, "%s R=%f on channel %d@%s", msg, value, adcChannel,
			getPinNameByAdcChannel(adcChannel, pinNameBuffer));
#endif
}

#if EFI_PROD_CODE
static void printMAPInfo(void) {
	scheduleMsg(&logger, "map type=%d raw=%f MAP=%f", engineConfiguration->map.sensor.sensorType, getRawMap(),
			getMap());
	if (engineConfiguration->map.sensor.sensorType == MT_CUSTOM) {
		scheduleMsg(&logger, "at0=%f at5=%f", engineConfiguration->map.sensor.customValueAt0,
				engineConfiguration->map.sensor.customValueAt5);
	}

	scheduleMsg(&logger, "baro type=%d value=%f", engineConfiguration->baroSensor.sensorType, getBaroPressure());
	if (engineConfiguration->baroSensor.sensorType == MT_CUSTOM) {
		scheduleMsg(&logger, "min=%f max=%f", engineConfiguration->baroSensor.customValueAt0,
				engineConfiguration->baroSensor.customValueAt5);
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
	scheduleMsg(&logger, "current 10bit=%d value=%f rate=%f", getTPS10bitAdc(), getTPS(engineConfiguration), getTpsRateOfChange());
}

static void printTemperatureInfo(void) {
	printThermistor("CLT", &engine.clt);
	if (!isValidCoolantTemperature(getCoolantTemperature(&engine))) {
		scheduleMsg(&logger, "CLT sensing error");
	}
	printThermistor("IAT", &engine.iat);
	if (!isValidIntakeAirTemperature(getIntakeAirTemperature(&engine))) {
		scheduleMsg(&logger, "IAT sensing error");
	}

#if EFI_ANALOG_INPUTS
	scheduleMsg(&logger, "base cranking fuel %f", engineConfiguration->crankingSettings.baseCrankingFuel);
#endif
}

static void setCrankingRpm(int value) {
	engineConfiguration->crankingSettings.crankingRpm = value;
	doPrintConfiguration(&engine);
}

static void setAlgorithm(int value) {
	engineConfiguration->algorithm = (engine_load_mode_e) value;
	doPrintConfiguration(&engine);
}

static void setFiringOrder(int value) {
	engineConfiguration->firingOrder = (firing_order_e) value;
	doPrintConfiguration(&engine);
}

static void setRpmHardLimit(int value) {
	engineConfiguration->rpmHardLimit = value;
	doPrintConfiguration(&engine);
}

static void setCrankingFuel(float timeMs) {
	engineConfiguration->crankingSettings.baseCrankingFuel = timeMs;
	printTemperatureInfo();
}

static void setGlobalTriggerAngleOffset(int value) {
	engineConfiguration->globalTriggerAngleOffset = value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration(&engine);
}

static void setCrankingTimingAngle(float value) {
	engineConfiguration->crankingTimingAngle = value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration(&engine);
}

static void setCrankingInjectionMode(int value) {
	engineConfiguration->crankingInjectionMode = (injection_mode_e) value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration(&engine);
}

static void setInjectionMode(int value) {
	engineConfiguration->injectionMode = (injection_mode_e) value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration(&engine);
}

static void setIgnitionMode(int value) {
	engineConfiguration->ignitionMode = (ignition_mode_e) value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration(&engine);
}

static void setTriggerType(int value) {
	engineConfiguration->triggerConfig.triggerType = (trigger_type_e) value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration(&engine);
}

static void setToothedWheel(int total, int skipped) {
	setToothedWheelConfiguration(&engineConfiguration2->triggerShape, total, skipped, engineConfiguration);
//	initializeTriggerShape(&logger, engineConfiguration, engineConfiguration2);
	incrementGlobalConfigurationVersion();
	doPrintConfiguration(&engine);
}

static void setCrankingChargeAngle(float value) {
	engineConfiguration->crankingChargeAngle = value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration(&engine);
}

static void setGlobalFuelCorrection(float value) {
	if (value < 0.01 || value > 50)
		return;
	scheduleMsg(&logger, "setting fuel mult=%f", value);
	engineConfiguration->globalFuelCorrection = value;
}

static void setCltBias(float value) {
	engineConfiguration->cltThermistorConf.bias_resistor = value;
}

static void setIatBias(float value) {
	engineConfiguration->iatThermistorConf.bias_resistor = value;
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

static void setEgtSpi(int spi) {
	boardConfiguration->max31855spiDevice = (spi_device_e) spi;
}

static void setPotSpi(int spi) {
	boardConfiguration->digitalPotentiometerSpiDevice = (spi_device_e) spi;
}

static void setIgnitionPin(const char *indexStr, const char *pinName) {
	int index = atoi(indexStr);
	if (index < 0 || index > IGNITION_PIN_COUNT)
		return;
	brain_pin_e pin = parseBrainPin(pinName);
	// todo: extract method - code duplication with other 'set_xxx_pin' methods?
	if (pin == GPIO_INVALID) {
		scheduleMsg(&logger, "invalid pin name [%s]", pinName);
		return;
	}
	scheduleMsg(&logger, "setting ignition pin[%d] to %s please save&restart", index, hwPortname(pin));
	boardConfiguration->ignitionPins[index] = pin;
}

static void setIdlePin(const char *pinName) {
	brain_pin_e pin = parseBrainPin(pinName);
	// todo: extract method - code duplication with other 'set_xxx_pin' methods?
	if (pin == GPIO_INVALID) {
		scheduleMsg(&logger, "invalid pin name [%s]", pinName);
		return;
	}
	scheduleMsg(&logger, "setting idleValve pin to %s please save&restart", hwPortname(pin));
	boardConfiguration->idleValvePin = pin;
}

static void setFuelPumpPin(const char *pinName) {
	brain_pin_e pin = parseBrainPin(pinName);
	// todo: extract method - code duplication with other 'set_xxx_pin' methods?
	if (pin == GPIO_INVALID) {
		scheduleMsg(&logger, "invalid pin name [%s]", pinName);
		return;
	}
	scheduleMsg(&logger, "setting fuelPump pin to %s please save&restart", hwPortname(pin));
	boardConfiguration->fuelPumpPin = pin;
}

static void setInjectionPin(const char *indexStr, const char *pinName) {
	int index = atoi(indexStr);
	if (index < 0 || index > INJECTION_PIN_COUNT)
		return;
	brain_pin_e pin = parseBrainPin(pinName);
	// todo: extract method - code duplication with other 'set_xxx_pin' methods?
	if (pin == GPIO_INVALID) {
		scheduleMsg(&logger, "invalid pin name [%s]", pinName);
		return;
	}
	scheduleMsg(&logger, "setting injection pin[%d] to %s please save&restart", index, hwPortname(pin));
	boardConfiguration->injectionPins[index] = pin;
}

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
	if (index < 0 || index > TRIGGER_SIMULATOR_PIN_COUNT || absI(index) == ERROR_CODE) {
		return;
	}
	int mode = atoi(modeCode);
	if (absI(mode) == ERROR_CODE) {
		return;
	}
	boardConfiguration->triggerSimulatorPinModes[index] = (pin_output_mode_e) mode;
}

static void setEgtCSPin(const char *indexStr, const char *pinName, board_configuration_s * board_configuration_s) {
	int index = atoi(indexStr);
	if (index < 0 || index > MAX31855_CS_COUNT || absI(index) == ERROR_CODE)
		return;
	brain_pin_e pin = parseBrainPin(pinName);
	if (pin == GPIO_INVALID) {
		scheduleMsg(&logger, "invalid pin name [%s]", pinName);
		return;
	}
	scheduleMsg(&logger, "setting EGT CS pin[%d] to %s please save&restart", index, hwPortname(pin));
	boardConfiguration->max31855_cs[index] = pin;
}

static void setTriggerSimulatorPin(const char *indexStr, const char *pinName) {
	int index = atoi(indexStr);
	if (index < 0 || index > TRIGGER_SIMULATOR_PIN_COUNT || absI(index) == ERROR_CODE)
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

static void enableOrDisable(const char *param, bool isEnabled) {
	if (strEqualCaseInsensitive(param, "fastadc")) {
		boardConfiguration->isFastAdcEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "injection")) {
		engineConfiguration->isInjectionEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "ignition")) {
		engineConfiguration->isIgnitionEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "self_stimulation")) {
		engineConfiguration->directSelfStimulation = isEnabled;
	} else if (strEqualCaseInsensitive(param, "engine_control")) {
		boardConfiguration->isEngineControlEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "map_avg")) {
		engineConfiguration->isMapAveragingEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "mil")) {
		engineConfiguration->isMilEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "fuel_pump")) {
		engineConfiguration->isFuelPumpEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "tunerstudio")) {
		engineConfiguration->isTunerStudioEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "wave_analyzer")) {
		engineConfiguration->isWaveAnalyzerEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "idle_thread")) {
		engineConfiguration->isIdleThreadEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "manual_spinning")) {
		engineConfiguration->isManualSpinningMode = isEnabled;
	} else if (strEqualCaseInsensitive(param, "cylinder_cleanup")) {
		engineConfiguration->isCylinderCleanupEnabled = isEnabled;
	} else {
		scheduleMsg(&logger, "unexpected [%s]", param);
		return; // well, MISRA would not like this 'return' here :(
	}
	scheduleMsg(&logger, "[%s] %s", param, isEnabled ? "enabled" : "disabled");
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

void initSettings(engine_configuration_s *engineConfiguration) {
	initLoggingExt(&logger, "settings control", LOGGING_BUFFER, sizeof(LOGGING_BUFFER));

	addConsoleActionP("showconfig", (VoidPtr) doPrintConfiguration, &engine);
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
	addConsoleActionI("set_idle_pin_mode", setIdlePinMode);
	addConsoleActionI("set_fuel_pump_pin_mode", setFuelPumpPinMode);
	addConsoleActionI("set_malfunction_indicator_pin_mode", setMalfunctionIndicatorPinMode);
	addConsoleActionI("set_rpm_multiplier", setRpmMultiplier);
	// todo: start saving values into flash right away?

	addConsoleActionF("set_global_fuel_correction", setGlobalFuelCorrection);

	addConsoleActionF("set_cranking_fuel", setCrankingFuel);
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

	addConsoleActionS("enable", enable);
	addConsoleActionS("disable", disable);

	addConsoleActionII("set_toothed_wheel", setToothedWheel);
	addConsoleActionI("set_trigger_type", setTriggerType);

	addConsoleActionF("set_vbatt_divider", setVBattDivider);

	addConsoleActionF("set_clt_bias", setCltBias);
	addConsoleActionF("set_iat_bias", setIatBias);
	addConsoleActionI("set_idle_solenoid_freq", setIdleSolenoidFrequency);

#if EFI_PROD_CODE
	addConsoleActionSS("set_injection_pin", setInjectionPin);
	addConsoleActionSS("set_ignition_pin", setIgnitionPin);
	addConsoleActionSS("set_trigger_input_pin", setTriggerInputPin);
	addConsoleActionSS("set_trigger_simulator_pin", setTriggerSimulatorPin);

	addConsoleActionSSP("set_egt_cs_pin", (VoidCharPtrCharPtrVoidPtr) setEgtCSPin, boardConfiguration);
	addConsoleActionI("set_egt_spi", setEgtSpi);

	addConsoleActionSS("set_trigger_simulator_mode", setTriggerSimulatorMode);
	addConsoleActionS("set_fuel_pump_pin", setFuelPumpPin);
	addConsoleActionS("set_idle_pin", setIdlePin);

	addConsoleAction("mapinfo", printMAPInfo);
	addConsoleActionSS("set_analog_input_pin", setAnalogInputPin);
	addConsoleActionSS("set_logic_input_pin", setLogicInputPin);
	addConsoleActionI("set_pot_spi", setPotSpi);
#endif /* EFI_PROD_CODE */
}

