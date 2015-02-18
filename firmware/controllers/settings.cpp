/**
 * @file settings.cpp
 * @brief This file is about configuring engine via the human-readable protocol
 *
 * @date Dec 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2015
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
#include "map.h"
#include "trigger_decoder.h"
#include "console_io.h"
#include "engine.h"
#include "efiGpio.h"

#if EFI_PROD_CODE
#include "rusefi.h"
#include "pin_repository.h"
#include "hardware.h"
#endif /* EFI_PROD_CODE */

#if EFI_INTERNAL_FLASH
#include "flash_main.h"
#endif /* EFI_INTERNAL_FLASH */

static char LOGGING_BUFFER[1000];
static Logging logger("settings control", LOGGING_BUFFER, sizeof(LOGGING_BUFFER));

extern int maxNesting;
extern engine_pins_s enginePins;

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

static void printOutputs(engine_configuration_s *engineConfiguration) {
	// engine_configuration2_s *engineConfiguration2
	scheduleMsg(&logger, "injectionPins: mode %s", getPin_output_mode_e(boardConfiguration->injectionPinMode));
	for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
		brain_pin_e brainPin = boardConfiguration->injectionPins[i];
		scheduleMsg(&logger, "injection #%d @ %s", (1 + i), hwPortname(brainPin));
	}

	scheduleMsg(&logger, "ignitionPins: mode %s", getPin_output_mode_e(boardConfiguration->ignitionPinMode));
	for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
		brain_pin_e brainPin = boardConfiguration->ignitionPins[i];
		scheduleMsg(&logger, "ignition #%d @ %s", (1 + i), hwPortname(brainPin));
	}

	scheduleMsg(&logger, "idlePin: mode %s @ %s freq=%d", getPin_output_mode_e(boardConfiguration->idleValvePinMode),
			hwPortname(boardConfiguration->idleValvePin), boardConfiguration->idleSolenoidFrequency);
	scheduleMsg(&logger, "malfunctionIndicatorn: %s mode=%s", hwPortname(boardConfiguration->malfunctionIndicatorPin),
			getPin_output_mode_e(boardConfiguration->malfunctionIndicatorPinMode));

	scheduleMsg(&logger, "fuelPumpPin: mode %s @ %s", getPin_output_mode_e(boardConfiguration->fuelPumpPinMode),
			hwPortname(boardConfiguration->fuelPumpPin));

	scheduleMsg(&logger, "fanPin: mode %s @ %s", getPin_output_mode_e(boardConfiguration->fanPinMode),
			hwPortname(boardConfiguration->fanPin));

	scheduleMsg(&logger, "mainRelay: mode %s @ %s", getPin_output_mode_e(boardConfiguration->mainRelayPinMode),
			hwPortname(boardConfiguration->mainRelayPin));
}

EXTERN_ENGINE;

/**
 * These should be not very long because these are displayed on the LCD as is
 */
const char* getConfigurationName(engine_type_e engineType) {
	switch (engineType) {
        case CUSTOM_ENGINE:
          return "CUSTOM";
#if EFI_SUPPORT_DODGE_NEON
	case DODGE_NEON_1995:
		return "Neon95";
	case DODGE_NEON_2003:
		return "Neon03";
#endif /* EFI_SUPPORT_DODGE_NEON */
#if EFI_SUPPORT_FORD_ASPIRE
	case FORD_ASPIRE_1996:
		return "Aspire";
#endif /* EFI_SUPPORT_FORD_ASPIRE */
#if EFI_SUPPORT_FORD_FIESTA
	case FORD_FIESTA:
		return "Fiesta";
#endif /* EFI_SUPPORT_FORD_FIESTA */
#if EFI_SUPPORT_NISSAN_PRIMERA
	case NISSAN_PRIMERA:
		return "Primera";
#endif /* EFI_SUPPORT_NISSAN_PRIMERA */
	case HONDA_ACCORD_CD:
		return "Accord3";
	case HONDA_ACCORD_CD_TWO_WIRES:
		return "Accord2";
	case HONDA_ACCORD_CD_DIP:
		return "HondaD";
	case FORD_INLINE_6_1995:
		return "Fordi6";
	case GY6_139QMB:
		return "Gy6139";
	case MAZDA_MIATA_NB:
		return "MiataNB";
	case MAZDA_323:
		return "M323";
	case SATURN_ION_2004:
		return "Saturn Ion";
	case MINI_COOPER_R50:
		return "CoopR50";
	case FORD_ESCORT_GT:
		return "EscrtGT";
	case CITROEN_TU3JP:
		return "TU3JP";
	case ROVER_V8:
		return "Rvrv8";
	case MITSU_4G93:
		return "Mi4G93";
	case MIATA_1990:
		return "MX590";
	case MIATA_1994_DEVIATOR:
		return "MX594d";
	case MIATA_1994_SPAGS:
		return "MX594s";
	case MIATA_1996:
		return "MX596";
	case BMW_E34:
		return "BMWe34";
        case TEST_ENGINE:
                return "Test";
        case SACHS:
                return "SACHS";
    	case MAZDA_626:
    		return "Mazda626";
	default:
		firmwareError("Unexpected: engineType %d", engineType);
		return NULL;
	}
}

/**
 * @brief	Prints current engine configuration to human-readable console.
 */
void printConfiguration(engine_configuration_s *engineConfiguration) {

	scheduleMsg(&logger, "Template %s/%d trigger %s/%s", getConfigurationName(engineConfiguration->engineType),
			engineConfiguration->engineType, getTrigger_type_e(engineConfiguration->trigger.type),
			getEngine_load_mode_e(engineConfiguration->algorithm));

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

	printFloatArray("vBatt: ", engineConfiguration->injector.battLagCorr, VBAT_INJECTOR_CURVE_SIZE);
	printFloatArray("vBatt bins: ", engineConfiguration->injector.battLagCorrBins, VBAT_INJECTOR_CURVE_SIZE);

//	appendMsgPrefix(&logger);

	scheduleMsg(&logger, "rpmHardLimit: %d/rpmMultiplier=%f", engineConfiguration->rpmHardLimit,
			engineConfiguration->rpmMultiplier);

	scheduleMsg(&logger, "tpsMin: %d/tpsMax: %d", engineConfiguration->tpsMin, engineConfiguration->tpsMax);

	scheduleMsg(&logger, "ignitionMode: %s/enabled=%s", getIgnition_mode_e(engineConfiguration->ignitionMode),
			boolToString(engineConfiguration->isIgnitionEnabled));
	scheduleMsg(&logger, "globalTriggerAngleOffset=%f", engineConfiguration->globalTriggerAngleOffset);
	scheduleMsg(&logger, "timingMode: %d", /*getTiming_mode_etodo*/(engineConfiguration->timingMode));
	scheduleMsg(&logger, "fixedModeTiming: %d", (int) engineConfiguration->fixedModeTiming);
	scheduleMsg(&logger, "ignitionOffset=%f", engineConfiguration->ignitionBaseAngle);
	scheduleMsg(&logger, "injection %s offset=%f/enabled=%s", getInjection_mode_e(engineConfiguration->injectionMode),
			(double) engineConfiguration->injectionAngle,
			boolToString(engineConfiguration->isInjectionEnabled));

	if (engineConfiguration->useConstantDwellDuringCranking) {
		scheduleMsg(&logger, "ignitionDwellForCrankingMs=%f", engineConfiguration->ignitionDwellForCrankingMs);
	} else {
		scheduleMsg(&logger, "cranking charge charge angle=%f fire at %f", engineConfiguration->crankingChargeAngle,
				engineConfiguration->crankingTimingAngle);
	}

//	scheduleMsg(&logger, "analogChartMode: %d", engineConfiguration->analogChartMode);

	scheduleMsg(&logger, "crankingRpm: %d", engineConfiguration->cranking.rpm);

	scheduleMsg(&logger, "analogInputDividerCoefficient: %f", engineConfiguration->analogInputDividerCoefficient);

	printOutputs(engineConfiguration);

	scheduleMsg(&logger, "map_avg=%s/mil=%s/fp=%s/ts=%s/wa=%s/it=%s/fastAdc=%s",
			boolToString(engineConfiguration->isMapAveragingEnabled), boolToString(engineConfiguration->isMilEnabled),
			boolToString(engineConfiguration->isFuelPumpEnabled),
			boolToString(engineConfiguration->isTunerStudioEnabled),
			boolToString(engineConfiguration->isWaveAnalyzerEnabled),
			boolToString(engineConfiguration->isIdleThreadEnabled), boolToString(boardConfiguration->isFastAdcEnabled));

	scheduleMsg(&logger, "isManualSpinningMode=%s/isCylinderCleanupEnabled=%s",
			boolToString(engineConfiguration->isManualSpinningMode),
			boolToString(engineConfiguration->isCylinderCleanupEnabled));

	scheduleMsg(&logger, "clutchUp@%s: %s", hwPortname(engineConfiguration->clutchUpPin), boolToString(engine->clutchUpState));
	scheduleMsg(&logger, "clutchDown@%s: %s", hwPortname(boardConfiguration->clutchDownPin), boolToString(engine->clutchDownState));

	scheduleMsg(&logger, "boardTestModeJumperPin: %s/nesting=%d", hwPortname(boardConfiguration->boardTestModeJumperPin),
			maxNesting);

	scheduleMsg(&logger, "digitalPotentiometerSpiDevice %d", boardConfiguration->digitalPotentiometerSpiDevice);

	for (int i = 0; i < DIGIPOT_COUNT; i++) {
		scheduleMsg(&logger, "digitalPotentiometer CS%d %s", i,
				hwPortname(boardConfiguration->digitalPotentiometerChipSelect[i]));
	}
#if EFI_PROD_CODE

	printSpiState(&logger, boardConfiguration);

#endif /* EFI_PROD_CODE */
}

static void doPrintConfiguration(Engine *engine) {
	printConfiguration(engineConfiguration);
}


static void setFixedModeTiming(int value) {
	engineConfiguration->fixedModeTiming = value;
	doPrintConfiguration(engine);
	incrementGlobalConfigurationVersion();
}

static void setTimingMode(int value) {
	engineConfiguration->timingMode = (timing_mode_e) value;
	doPrintConfiguration(engine);
	incrementGlobalConfigurationVersion();
}

void setEngineType(int value) {
	engineConfiguration->engineType = (engine_type_e) value;
	resetConfigurationExt(&logger, (engine_type_e) value, engine);
#if EFI_INTERNAL_FLASH
	writeToFlash();
//	scheduleReset();
#endif /* EFI_PROD_CODE */
	incrementGlobalConfigurationVersion();
	doPrintConfiguration(engine);
}

static void setIdleSolenoidFrequency(int value) {
	boardConfiguration->idleSolenoidFrequency = value;
}

static void setInjectionPinMode(int value) {
	boardConfiguration->injectionPinMode = (pin_output_mode_e) value;
	doPrintConfiguration(engine);
}

static void setIgnitionPinMode(int value) {
	boardConfiguration->ignitionPinMode = (pin_output_mode_e) value;
	doPrintConfiguration(engine);
}

static void setIdlePinMode(int value) {
	boardConfiguration->idleValvePinMode = (pin_output_mode_e) value;
	doPrintConfiguration(engine);
}

static void setInjectionOffset(float value) {
	engineConfiguration->injectionAngle = value;
	doPrintConfiguration(engine);
	incrementGlobalConfigurationVersion();
}

static void setIgnitionOffset(float value) {
	engineConfiguration->ignitionBaseAngle = value;
	doPrintConfiguration(engine);
	incrementGlobalConfigurationVersion();
}

static void setFuelPumpPinMode(int value) {
	boardConfiguration->fuelPumpPinMode = (pin_output_mode_e) value;
	doPrintConfiguration(engine);
}

static void setMalfunctionIndicatorPinMode(int value) {
	boardConfiguration->malfunctionIndicatorPinMode = (pin_output_mode_e) value;
	doPrintConfiguration(engine);
}

static void setAnalogChartMode(int value) {
	boardConfiguration->analogChartMode = (analog_chart_e)value;
	doPrintConfiguration(engine);
}

static void setRpmMultiplier(int value) {
	engineConfiguration->rpmMultiplier = value;
	doPrintConfiguration(engine);
}

static char pinNameBuffer[16];

static void printThermistor(const char *msg, Thermistor *thermistor) {
	adc_channel_e adcChannel = thermistor->channel;
	float voltage = getVoltageDivided(adcChannel);
	float r = getResistance(thermistor);

	float t = getTemperatureC(thermistor);

	scheduleMsg(&logger, "%s v=%f C=%f R=%f on channel %d", msg, voltage, t, r, adcChannel);
	scheduleMsg(&logger, "@%s", getPinNameByAdcChannel(adcChannel, pinNameBuffer));
	scheduleMsg(&logger, "bias=%f A=%..100000f B=%..100000f C=%..100000f", thermistor->config->bias_resistor, thermistor->config->s_h_a,
			thermistor->config->s_h_b, thermistor->config->s_h_c);
//#if EFI_ANALOG_INPUTS
	scheduleMsg(&logger, "==============================");
//#endif
}

#if EFI_PROD_CODE
static void printMAPInfo(void) {
#if EFI_ANALOG_INPUTS
	scheduleMsg(&logger, "map type=%d raw=%f MAP=%f", engineConfiguration->map.sensor.type, getRawMap(),
			getMap());
	if (engineConfiguration->map.sensor.type == MT_CUSTOM) {
		scheduleMsg(&logger, "at0=%f at5=%f", engineConfiguration->map.sensor.valueAt0,
				engineConfiguration->map.sensor.valueAt5);
	}

	scheduleMsg(&logger, "baro type=%d value=%f", engineConfiguration->baroSensor.type, getBaroPressure());
	if (engineConfiguration->baroSensor.type == MT_CUSTOM) {
		scheduleMsg(&logger, "min=%f max=%f", engineConfiguration->baroSensor.valueAt0,
				engineConfiguration->baroSensor.valueAt5);
	}
#endif
}
#endif /* EFI_PROD_CODE */

static void printTPSInfo(void) {
#if (EFI_PROD_CODE && HAL_USE_ADC) || defined(__DOXYGEN__)
	if(!engineConfiguration->hasTpsSensor) {
		scheduleMsg(&logger, "NO TPS SENSOR");
		return;
	}

	GPIO_TypeDef* port = getAdcChannelPort(engineConfiguration->tpsAdcChannel);
	int pin = getAdcChannelPin(engineConfiguration->tpsAdcChannel);

	scheduleMsg(&logger, "tps min %d/max %d v=%f @%s%d", engineConfiguration->tpsMin, engineConfiguration->tpsMax,
			getTPSVoltage(PASS_ENGINE_PARAMETER_F), portname(port), pin);
#endif
	scheduleMsg(&logger, "current 10bit=%d value=%f rate=%f", getTPS10bitAdc(), getTPS(PASS_ENGINE_PARAMETER_F),
			getTpsRateOfChange());
}


static void printTemperatureInfo(void) {
#if EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
	printThermistor("CLT", &engine->clt);
	if (!isValidCoolantTemperature(getCoolantTemperature(engine))) {
		scheduleMsg(&logger, "CLT sensing error");
	}
	printThermistor("IAT", &engine->iat);
	if (!isValidIntakeAirTemperature(getIntakeAirTemperature(engine))) {
		scheduleMsg(&logger, "IAT sensing error");
	}

	scheduleMsg(&logger, "fan=%s @ %s", boolToString(enginePins.fanRelay.getLogicValue()),
			hwPortname(boardConfiguration->fanPin));

	scheduleMsg(&logger, "A/C relay=%s @ %s", boolToString(enginePins.acRelay.getLogicValue()),
			hwPortname(boardConfiguration->acRelayPin));

#endif
}

static void setCrankingRpm(int value) {
	engineConfiguration->cranking.rpm = value;
	doPrintConfiguration(engine);
}

static void setAlgorithm(int value) {
	engineConfiguration->algorithm = (engine_load_mode_e) value;
	doPrintConfiguration(engine);
}

static void setFiringOrder(int value) {
	engineConfiguration->specs.firingOrder = (firing_order_e) value;
	doPrintConfiguration(engine);
}

static void setRpmHardLimit(int value) {
	engineConfiguration->rpmHardLimit = value;
	doPrintConfiguration(engine);
}

static void setCrankingFuel(float timeMs) {
	engineConfiguration->cranking.baseFuel = timeMs;
	printTemperatureInfo();
}

static void setGlobalTriggerAngleOffset(float value) {
	engineConfiguration->globalTriggerAngleOffset = value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration(engine);
}

static void setCrankingTimingAngle(float value) {
	engineConfiguration->crankingTimingAngle = value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration(engine);
}

static void setCrankingInjectionMode(int value) {
	engineConfiguration->crankingInjectionMode = (injection_mode_e) value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration(engine);
}

static void setInjectionMode(int value) {
	engineConfiguration->injectionMode = (injection_mode_e) value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration(engine);
}

static void setIgnitionMode(int value) {
	engineConfiguration->ignitionMode = (ignition_mode_e) value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration(engine);
}

static void setTriggerType(int value) {
	engineConfiguration->trigger.type = (trigger_type_e) value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration(engine);
}

static void setToothedWheel(int total, int skipped) {
	setToothedWheelConfiguration(&engine->triggerShape, total, skipped, engineConfiguration);
//	initializeTriggerShape(&logger, engineConfiguration, engineConfiguration2);
	incrementGlobalConfigurationVersion();
	doPrintConfiguration(engine);
}

static void setCrankingChargeAngle(float value) {
	engineConfiguration->crankingChargeAngle = value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration(engine);
}

static void setGlobalFuelCorrection(float value) {
	if (value < 0.01 || value > 50)
		return;
	scheduleMsg(&logger, "setting fuel mult=%f", value);
	engineConfiguration->globalFuelCorrection = value;
}

static void setCltBias(float value) {
	engineConfiguration->clt.bias_resistor = value;
}

static void setFanSetting(float onTempC, float offTempC) {
	if(onTempC <= offTempC) {
		scheduleMsg(&logger, "ON temp [%f] should be above OFF temp [%f]", onTempC, offTempC);
		return;
	}
	engineConfiguration->fanOnTemperature = onTempC;
	engineConfiguration->fanOffTemperature = offTempC;
}

static void setIatBias(float value) {
	engineConfiguration->iat.bias_resistor = value;
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

static void setWholePhaseMapCmd(float value) {
	scheduleMsg(&logger, "Setting whole injection phase map to %f", value);
	setMap(engineConfiguration->injectionPhase, value);
}

static void setWholeTimingMapCmd(float value) {
	scheduleMsg(&logger, "Setting whole timing advance map to %f", value);
	setWholeTimingMap(value);
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
	int index = atoi(indexStr) - 1; // convert from human index into software index
	if (index < 0 || index >= IGNITION_PIN_COUNT)
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

// set_idle_pin none
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
	int index = atoi(indexStr) - 1; // convert from human index into software index
	if (index < 0 || index >= INJECTION_PIN_COUNT)
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
	if (index < 0 || index >= TRIGGER_SIMULATOR_PIN_COUNT || absI(index) == ERROR_CODE) {
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
	if (index < 0 || index >= EGT_CHANNEL_COUNT || absI(index) == ERROR_CODE)
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
	if (index < 0 || index >= TRIGGER_SIMULATOR_PIN_COUNT || absI(index) == ERROR_CODE)
		return;
	brain_pin_e pin = parseBrainPin(pinName);
	if (pin == GPIO_INVALID) {
		scheduleMsg(&logger, "invalid pin name [%s]", pinName);
		return;
	}
	scheduleMsg(&logger, "setting trigger simulator pin[%d] to %s please save&restart", index, hwPortname(pin));
	boardConfiguration->triggerSimulatorPins[index] = pin;
}

#if HAL_USE_ADC || defined(__DOXYGEN__)
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
#endif

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

static void showPinFunction(const char *pinName) {
	brain_pin_e pin = parseBrainPin(pinName);
	if (pin == GPIO_INVALID) {
		scheduleMsg(&logger, "invalid pin name [%s]", pinName);
		return;
	}
	scheduleMsg(&logger, "Pin %s: [%s]", pinName, getPinFunction(pin));
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
	printSpiState(&logger, boardConfiguration);
}

static void enableOrDisable(const char *param, bool isEnabled) {
	if (strEqualCaseInsensitive(param, "fastadc")) {
		boardConfiguration->isFastAdcEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "joystick")) {
		engineConfiguration->isJoystickEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "sd")) {
		boardConfiguration->isSdCardEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "can")) {
		engineConfiguration->isCanEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "can_read")) {
		engineConfiguration->canReadEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "can_write")) {
		engineConfiguration->canWriteEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "injection")) {
		engineConfiguration->isInjectionEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "trigger_details")) {
		engineConfiguration->isPrintTriggerSynchDetails = isEnabled;
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

void stopEngine(void) {
	engine->stopEngineRequestTimeNt = getTimeNowNt();
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

static void setInjectorLag(float value) {
	engineConfiguration->injector.lag = value;
}

void initSettings(engine_configuration_s *engineConfiguration) {
	addConsoleActionP("showconfig", (VoidPtr) doPrintConfiguration, &engine);
	addConsoleAction("tempinfo", printTemperatureInfo);
	addConsoleAction("tpsinfo", printTPSInfo);
	addConsoleAction("info", printAllInfo);

	addConsoleActionF("set_ignition_offset", setIgnitionOffset);
	addConsoleActionF("set_injection_offset", setInjectionOffset);
	addConsoleActionF("set_global_trigger_offset_angle", setGlobalTriggerAngleOffset);
	addConsoleActionI("set_analog_chart_mode", setAnalogChartMode);
	addConsoleActionI("set_fixed_mode_timing", setFixedModeTiming);
	addConsoleActionI("set_timing_mode", setTimingMode);
	addConsoleActionI("set_engine_type", setEngineType);

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

	addConsoleActionF("set_whole_phase_map", setWholePhaseMapCmd);
	addConsoleActionF("set_whole_timing_map", setWholeTimingMapCmd);
	addConsoleActionF("set_whole_fuel_map", setWholeFuelMapCmd);
	addConsoleActionSSS("set_fuel_map", setFuelMap);

	addConsoleActionF("set_whole_timing_map", setWholeTimingMap);
	addConsoleActionSSS("set_timing_map", setTimingMap);

	addConsoleActionI("set_rpm_hard_limit", setRpmHardLimit);
	addConsoleActionI("set_firing_order", setFiringOrder);
	addConsoleActionI("set_algorithm", setAlgorithm);
	addConsoleAction("stopengine", (Void)stopEngine);

	// todo: refactor this - looks like all boolean flags should be controlled with less code duplication
	addConsoleAction("enable_injection", enableInjection);
	addConsoleAction("disable_injection", disableInjection);
	addConsoleAction("enable_ignition", enableIgnition);
	addConsoleAction("disable_ignition", disableIgnition);

	addConsoleActionI("enable_spi", enableSpi);
	addConsoleActionI("disable_spi", disableSpi);

	addConsoleActionS("enable", enable);
	addConsoleActionS("disable", disable);

	addConsoleActionII("set_toothed_wheel", setToothedWheel);
	addConsoleActionI("set_trigger_type", setTriggerType);

	addConsoleActionF("set_vbatt_divider", setVBattDivider);

	addConsoleActionF("set_injector_lag", setInjectorLag);

	addConsoleActionF("set_clt_bias", setCltBias);
	addConsoleActionF("set_iat_bias", setIatBias);
	addConsoleActionI("set_idle_solenoid_freq", setIdleSolenoidFrequency);

	addConsoleActionFF("set_fan", setFanSetting);

#if EFI_PROD_CODE
	addConsoleActionS("showpin", showPinFunction);
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
#if HAL_USE_ADC || defined(__DOXYGEN__)
	addConsoleActionSS("set_analog_input_pin", setAnalogInputPin);
#endif
	addConsoleActionSS("set_logic_input_pin", setLogicInputPin);
	addConsoleActionI("set_pot_spi", setPotSpi);
#endif /* EFI_PROD_CODE */
}

