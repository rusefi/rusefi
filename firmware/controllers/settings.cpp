/**
 * @file settings.cpp
 * @brief This file is about configuring engine via the human-readable protocol
 *
 * @date Dec 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2018
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
#include "engine_math.h"
#include "alternatorController.h"
#include "idle_thread.h"
#include "allsensors.h"

#if EFI_PROD_CODE || defined(__DOXYGEN__)
#include "vehicle_speed.h"
#include "electronic_throttle.h"
#include "rtc_helper.h"
#include "can_hw.h"
#include "rusefi.h"
#include "pin_repository.h"
#include "hardware.h"
#endif /* EFI_PROD_CODE */

#if EFI_INTERNAL_FLASH || defined(__DOXYGEN__)
#include "flash_main.h"
#endif /* EFI_INTERNAL_FLASH */

#if EFI_ENGINE_SNIFFER || defined(__DOXYGEN__)
#include "engine_sniffer.h"
extern int waveChartUsedSize;
extern WaveChart waveChart;
#endif /* EFI_ENGINE_SNIFFER */

static char LOGGING_BUFFER[1000];
static Logging logger("settings control", LOGGING_BUFFER, sizeof(LOGGING_BUFFER));

extern int maxNesting;
EXTERN_ENGINE
;

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
		appendPrintf(&logger, "%.2f ", array[j]);
	}
	appendMsgPostfix(&logger);
	scheduleLogging(&logger);
}

void printSpiState(Logging *logger, board_configuration_s *boardConfiguration) {
	scheduleMsg(logger, "spi 1=%s/2=%s/3=%s", boolToString(boardConfiguration->is_enabled_spi_1),
			boolToString(boardConfiguration->is_enabled_spi_2), boolToString(boardConfiguration->is_enabled_spi_3));
}

extern board_configuration_s *boardConfiguration;

static void printOutputs(const engine_configuration_s *engineConfiguration) {
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

	scheduleMsg(&logger, "idlePin: mode %s @ %s freq=%d", getPin_output_mode_e(boardConfiguration->idle.solenoidPinMode),
			hwPortname(boardConfiguration->idle.solenoidPin), boardConfiguration->idle.solenoidFrequency);
	scheduleMsg(&logger, "malfunctionIndicatorn: %s mode=%s", hwPortname(boardConfiguration->malfunctionIndicatorPin),
			getPin_output_mode_e(boardConfiguration->malfunctionIndicatorPinMode));

	scheduleMsg(&logger, "fuelPumpPin: mode %s @ %s", getPin_output_mode_e(boardConfiguration->fuelPumpPinMode),
			hwPortname(boardConfiguration->fuelPumpPin));

	scheduleMsg(&logger, "fanPin: mode %s @ %s", getPin_output_mode_e(boardConfiguration->fanPinMode),
			hwPortname(boardConfiguration->fanPin));

	scheduleMsg(&logger, "mainRelay: mode %s @ %s", getPin_output_mode_e(boardConfiguration->mainRelayPinMode),
			hwPortname(boardConfiguration->mainRelayPin));

	scheduleMsg(&logger, "alternator field: mode %s @ %s",
			getPin_output_mode_e(boardConfiguration->alternatorControlPinMode),
			hwPortname(boardConfiguration->alternatorControlPin));
}


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
	case HONDA_ACCORD_1_24_SHIFTED:
		return "Accord24sh";
	case HONDA_ACCORD_CD_DIP:
		return "HondaD";
	case FORD_INLINE_6_1995:
		return "Fordi6";
	case GY6_139QMB:
		return "Gy6139";
	case MAZDA_MIATA_NB1:
		return "MiataNB1";
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
	case VW_ABA:
		return "VW_ABA";
	case DODGE_STRATUS:
		return "DODGE_STRATUS";
	case SACHS:
		return "SACHS";
	case DAIHATSU:
		return "DAIHATSU";
	case SUZUKI_VITARA:
		return "SUZUKI_VITARA";
	case CAMARO_4:
		return "CAMARO_4";
	case CHEVY_C20_1973:
		return "CHEVY C20";
	case GM_2_2:
	  return "GM_2_2";
	case DODGE_RAM:
		return "DODGE_RAM";
	default:
		return getEngine_type_e(engineType);
	}
}

/**
 * @brief	Prints current engine configuration to human-readable console.
 */
void printConfiguration(const engine_configuration_s *engineConfiguration) {

	scheduleMsg(&logger, "Template %s/%d trigger %s/%s/%d", getConfigurationName(engineConfiguration->engineType),
			engineConfiguration->engineType, getTrigger_type_e(engineConfiguration->trigger.type),
			getEngine_load_mode_e(engineConfiguration->fuelAlgorithm), engineConfiguration->fuelAlgorithm);


	scheduleMsg(&logger, "configurationVersion=%d", getGlobalConfigurationVersion());

	for (int k = 0; k < FUEL_LOAD_COUNT; k++) {
//		print("line %d (%.2f): ", k, engineConfiguration->fuelKeyBins[k]);
//		for (int r = 0; r < FUEL_RPM_COUNT; r++) {
//			print("%.2f ", engineConfiguration->fuelTable[k][r]);
//		}
//		print("\r\n");
	}

//	printFloatArray("RPM bin: ", config->fuelRpmBins, FUEL_RPM_COUNT);
//
//	printFloatArray("Y bin: ", config->fuelLoadBins, FUEL_LOAD_COUNT);
//
//	printFloatArray("CLT: ", config->cltFuelCorr, CLT_CURVE_SIZE);
//	printFloatArray("CLT bins: ", config->cltFuelCorrBins, CLT_CURVE_SIZE);
//
//	printFloatArray("IAT: ", config->iatFuelCorr, IAT_CURVE_SIZE);
//	printFloatArray("IAT bins: ", config->iatFuelCorrBins, IAT_CURVE_SIZE);
//
//	printFloatArray("vBatt: ", engineConfiguration->injector.battLagCorr, VBAT_INJECTOR_CURVE_SIZE);
//	printFloatArray("vBatt bins: ", engineConfiguration->injector.battLagCorrBins, VBAT_INJECTOR_CURVE_SIZE);

	scheduleMsg(&logger, "rpmHardLimit: %d/operationMode=%d", engineConfiguration->rpmHardLimit,
			engineConfiguration->operationMode);

	scheduleMsg(&logger, "globalTriggerAngleOffset=%.2f", engineConfiguration->globalTriggerAngleOffset);

	scheduleMsg(&logger, "=== cranking ===");
	scheduleMsg(&logger, "crankingRpm: %d", engineConfiguration->cranking.rpm);
	scheduleMsg(&logger, "cranking injection %s", getInjection_mode_e(engineConfiguration->crankingInjectionMode));

	if (engineConfiguration->useConstantDwellDuringCranking) {
		scheduleMsg(&logger, "ignitionDwellForCrankingMs=%.2f", engineConfiguration->ignitionDwellForCrankingMs);
	} else {
		scheduleMsg(&logger, "cranking charge charge angle=%.2f fire at %.2f", engineConfiguration->crankingChargeAngle,
				engineConfiguration->crankingTimingAngle);
	}

	scheduleMsg(&logger, "=== ignition ===");

	scheduleMsg(&logger, "ignitionMode: %s/enabled=%s", getIgnition_mode_e(engineConfiguration->ignitionMode),
			boolToString(engineConfiguration->isIgnitionEnabled));
	scheduleMsg(&logger, "timingMode: %s", getTiming_mode_e(engineConfiguration->timingMode));
	if (engineConfiguration->timingMode == TM_FIXED) {
		scheduleMsg(&logger, "fixedModeTiming: %d", (int) engineConfiguration->fixedModeTiming);
	}
	scheduleMsg(&logger, "ignitionOffset=%.2f", engineConfiguration->ignitionOffset);

	scheduleMsg(&logger, "=== injection ===");
	scheduleMsg(&logger, "injection %s offset=%.2f/enabled=%s", getInjection_mode_e(engineConfiguration->injectionMode),
			(double) engineConfiguration->extraInjectionOffset, boolToString(engineConfiguration->isInjectionEnabled));

	printOutputs(engineConfiguration);

	scheduleMsg(&logger, "map_avg=%s/ts=%s/wa=%s/fastAdc=%s",
			boolToString(engineConfiguration->isMapAveragingEnabled),
			boolToString(engineConfiguration->isTunerStudioEnabled),
			boolToString(engineConfiguration->isWaveAnalyzerEnabled),
			boolToString(boardConfiguration->isFastAdcEnabled));

	scheduleMsg(&logger, "isManualSpinningMode=%s/isCylinderCleanupEnabled=%s",
			boolToString(engineConfiguration->isManualSpinningMode),
			boolToString(engineConfiguration->isCylinderCleanupEnabled));

	scheduleMsg(&logger, "clutchUp@%s: %s", hwPortname(boardConfiguration->clutchUpPin),
			boolToString(engine->clutchUpState));
	scheduleMsg(&logger, "clutchDown@%s: %s", hwPortname(boardConfiguration->clutchDownPin),
			boolToString(engine->clutchDownState));

	scheduleMsg(&logger, "boardTestModeJumperPin: %s/nesting=%d",
			hwPortname(boardConfiguration->boardTestModeJumperPin), maxNesting);

	scheduleMsg(&logger, "digitalPotentiometerSpiDevice %d", boardConfiguration->digitalPotentiometerSpiDevice);

	for (int i = 0; i < DIGIPOT_COUNT; i++) {
		scheduleMsg(&logger, "digitalPotentiometer CS%d %s", i,
				hwPortname(boardConfiguration->digitalPotentiometerChipSelect[i]));
	}
#if EFI_PROD_CODE

	printSpiState(&logger, boardConfiguration);

#endif /* EFI_PROD_CODE */
}

static void doPrintConfiguration() {
	printConfiguration(engineConfiguration);
}

static void setFixedModeTiming(int value) {
	engineConfiguration->fixedModeTiming = value;
	doPrintConfiguration();
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
}

static void setTimingMode(int value) {
	engineConfiguration->timingMode = (timing_mode_e) value;
	doPrintConfiguration();
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
}

void setEngineType(int value) {
//	enginePins.stopPins(); // this is mostly needed by functional tests

	engineConfiguration->engineType = (engine_type_e) value;
	resetConfigurationExt(&logger, (engine_type_e) value PASS_ENGINE_PARAMETER_SUFFIX);
#if EFI_ENGINE_SNIFFER || defined(__DOXYGEN__)
	if (engine->isTestMode)
		waveChart.reset();
#endif

#if EFI_INTERNAL_FLASH
	writeToFlashNow();
//	scheduleReset();
#endif /* EFI_PROD_CODE */
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
	doPrintConfiguration();
}

static void setIdleSolenoidFrequency(int value) {
	boardConfiguration->idle.solenoidFrequency = value;
	incrementGlobalConfigurationVersion();
}

static void setInjectionPinMode(int value) {
	boardConfiguration->injectionPinMode = (pin_output_mode_e) value;
	doPrintConfiguration();
}

static void setIgnitionPinMode(int value) {
	boardConfiguration->ignitionPinMode = (pin_output_mode_e) value;
	doPrintConfiguration();
}

static void setIdlePinMode(int value) {
	boardConfiguration->idle.solenoidPinMode = (pin_output_mode_e) value;
	doPrintConfiguration();
}

static void setInjectionOffset(float value) {
	engineConfiguration->extraInjectionOffset = value;
	doPrintConfiguration();
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
}

static void setIgnitionOffset(float value) {
	engineConfiguration->ignitionOffset = value;
	doPrintConfiguration();
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
}

static void setFuelPumpPinMode(int value) {
	boardConfiguration->fuelPumpPinMode = (pin_output_mode_e) value;
	doPrintConfiguration();
}

static void setMalfunctionIndicatorPinMode(int value) {
	boardConfiguration->malfunctionIndicatorPinMode = (pin_output_mode_e) value;
	doPrintConfiguration();
}

static void setSensorChartMode(int value) {
	boardConfiguration->sensorChartMode = (sensor_chart_e) value;
	doPrintConfiguration();
}

static void setOM(int value) {
	engineConfiguration->operationMode = (operation_mode_e)value;
	doPrintConfiguration();
}

static char pinNameBuffer[16];

static void printThermistor(const char *msg, ThermistorConf *config, ThermistorMath *tm, bool useLinear) {
	adc_channel_e adcChannel = config->adcChannel;
	float voltage = getVoltageDivided("term", adcChannel);
	float r = getResistance(config, voltage);

	float t = getTemperatureC(config, tm, useLinear);

	thermistor_conf_s *tc = &config->config;

	scheduleMsg(&logger, "%s volts=%.2f Celsius=%.2f sensorR=%.2f on channel %d", msg, voltage, t, r, adcChannel);
	scheduleMsg(&logger, "@%s", getPinNameByAdcChannel(msg, adcChannel, pinNameBuffer));
	scheduleMsg(&logger, "C=%.2f/R=%.2f C=%.2f/R=%.2f C=%.2f/R=%.2f",
			tc->tempC_1, tc->resistance_1,
			tc->tempC_2, tc->resistance_2,
			tc->tempC_3, tc->resistance_3);

	// %.5f
	scheduleMsg(&logger, "bias resistor=%.2fK A=%.5f B=%.5f C=%.5f", tc->bias_resistor / 1000,
			tm->s_h_a, tm->s_h_b, tm->s_h_c);
	scheduleMsg(&logger, "==============================");
}

static void printTPSInfo(void) {
#if (EFI_PROD_CODE && HAL_USE_ADC) || defined(__DOXYGEN__)
	if (!hasTpsSensor()) {
		scheduleMsg(&logger, "NO TPS SENSOR");
		return;
	}
	static char pinNameBuffer[16];

	scheduleMsg(&logger, "tps min (closed) %d/max (full) %d v=%.2f @%s", engineConfiguration->tpsMin, engineConfiguration->tpsMax,
			getTPSVoltage(PASS_ENGINE_PARAMETER_SIGNATURE), getPinNameByAdcChannel("tps", engineConfiguration->tpsAdcChannel, pinNameBuffer));
#endif /* EFI_PROD_CODE */
	scheduleMsg(&logger, "current 10bit=%d value=%.2f rate=%.2f", getTPS12bitAdc() / TPS_TS_CONVERSION, getTPS(PASS_ENGINE_PARAMETER_SIGNATURE),
			getTpsRateOfChange());
}

static void printTemperatureInfo(void) {
#if EFI_ANALOG_SENSORS || defined(__DOXYGEN__)
	printThermistor("CLT", &engineConfiguration->clt, &engine->engineState.cltCurve,
			engineConfiguration->useLinearCltSensor);
	if (!isValidCoolantTemperature(getCoolantTemperature(PASS_ENGINE_PARAMETER_SIGNATURE))) {
		scheduleMsg(&logger, "CLT sensing error");
	}
	printThermistor("IAT", &engineConfiguration->iat, &engine->engineState.iatCurve,
			engineConfiguration->useLinearIatSensor);
	if (!isValidIntakeAirTemperature(getIntakeAirTemperature(PASS_ENGINE_PARAMETER_SIGNATURE))) {
		scheduleMsg(&logger, "IAT sensing error");
	}

	scheduleMsg(&logger, "fan=%s @ %s", boolToString(enginePins.fanRelay.getLogicValue()),
			hwPortname(boardConfiguration->fanPin));

	scheduleMsg(&logger, "A/C relay=%s @ %s", boolToString(enginePins.acRelay.getLogicValue()),
			hwPortname(boardConfiguration->acRelayPin));

	scheduleMsg(&logger, "warmupPID=%d corr=%.2f", boardConfiguration->useWarmupPidAfr,
			engine->engineState.cltFuelCorrection);

#endif /* EFI_ANALOG_SENSORS */
}

void setCallFromPitStop(int durationMs) {
	engine->callFromPitStopEndTime = currentTimeMillis() + durationMs;
}

static void setCrankingRpm(int value) {
	engineConfiguration->cranking.rpm = value;
	doPrintConfiguration();
}

/**
 * this method is used in console - it also prints current configuration
 */
static void setAlgorithmInt(int value) {
	setAlgorithm((engine_load_mode_e) value);
	doPrintConfiguration();
}

static void setFiringOrder(int value) {
	engineConfiguration->specs.firingOrder = (firing_order_e) value;
	doPrintConfiguration();
}

static void setRpmHardLimit(int value) {
	engineConfiguration->rpmHardLimit = value;
	doPrintConfiguration();
}

static void setCrankingIACExtra(float percent) {
	engineConfiguration->crankingIACposition = percent;
	scheduleMsg(&logger, "cranking_iac %.2f", percent);

}

static void setCrankingFuel(float timeMs) {
	engineConfiguration->cranking.baseFuel = timeMs;
	scheduleMsg(&logger, "cranking_fuel %.2f", timeMs);

	printTemperatureInfo();
}

static void setGlobalTriggerAngleOffset(float value) {
	if (cisnan(value)) {
		warning(CUSTOM_ERR_SGTP_ARGUMENT, "Invalid argument");
		return;
	}
	engineConfiguration->globalTriggerAngleOffset = value;
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
	doPrintConfiguration();
}

static void setCrankingPrimingPulse(float value) {
	engineConfiguration->startOfCrankingPrimingPulse = value;
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
}

static void setCrankingTimingAngle(float value) {
	engineConfiguration->crankingTimingAngle = value;
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
	doPrintConfiguration();
}

static void setCrankingInjectionMode(int value) {
	engineConfiguration->crankingInjectionMode = (injection_mode_e) value;
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
	doPrintConfiguration();
}

static void setInjectionMode(int value) {
	engineConfiguration->injectionMode = (injection_mode_e) value;
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
	doPrintConfiguration();
}

static void setIgnitionMode(int value) {
	engineConfiguration->ignitionMode = (ignition_mode_e) value;
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
	doPrintConfiguration();
}

static void setOneCoilIgnition(void) {
	setIgnitionMode((int)IM_ONE_COIL);
}

static void setWastedIgnition(void) {
	setIgnitionMode((int)IM_WASTED_SPARK);
}

static void setIndividualCoilsIgnition(void) {
	setIgnitionMode((int)IM_INDIVIDUAL_COILS);
}

static void setTriggerType(int value) {
	engineConfiguration->trigger.type = (trigger_type_e) value;
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
	doPrintConfiguration();
}

static void setDebugMode(int value) {
	engineConfiguration->debugMode = (debug_mode_e) value;
}

static void setInjectorLag(float voltage, float value) {
	setCurveValue(INJECTOR_LAG_CURVE, voltage, value);
}

static void setToothedWheel(int total, int skipped DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (total < 1 || skipped >= total) {
		scheduleMsg(&logger, "invalid parameters %d %d", total, skipped);
		return;
	}
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL;
	engineConfiguration->trigger.customTotalToothCount = total;
	engineConfiguration->trigger.customSkippedToothCount = skipped;

	scheduleMsg(&logger, "toothed: total=%d/skipped=%d", total, skipped);
	setToothedWheelConfiguration(&engine->triggerCentral.triggerShape, total, skipped, engineConfiguration->operationMode);
//	initializeTriggerShape(&logger, engineConfiguration, engineConfiguration2);
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
	doPrintConfiguration();
}

static void setCrankingChargeAngle(float value) {
	engineConfiguration->crankingChargeAngle = value;
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
	doPrintConfiguration();
}

static void setGlobalFuelCorrection(float value) {
	if (value < 0.01 || value > 50)
		return;
	scheduleMsg(&logger, "setting fuel mult=%.2f", value);
	engineConfiguration->globalFuelCorrection = value;
}

static void setCltBias(float value) {
	engineConfiguration->clt.config.bias_resistor = value;
}

static void setFanSetting(float onTempC, float offTempC) {
	if (onTempC <= offTempC) {
		scheduleMsg(&logger, "ON temp [%.2f] should be above OFF temp [%.2f]", onTempC, offTempC);
		return;
	}
	engineConfiguration->fanOnTemperature = onTempC;
	engineConfiguration->fanOffTemperature = offTempC;
}

static void setIatBias(float value) {
	engineConfiguration->iat.config.bias_resistor = value;
}

static void setVBattDivider(float value) {
	engineConfiguration->vbattDividerCoeff = value;
}

static void setWholeTimingMap(float value) {
	// todo: table helper?
	scheduleMsg(&logger, "Setting whole timing map to %.2f", value);
	for (int l = 0; l < IGN_LOAD_COUNT; l++) {
		for (int r = 0; r < IGN_RPM_COUNT; r++) {
			config->ignitionTable[l][r] = value;
		}
	}
}

static void setWholePhaseMapCmd(float value) {
	scheduleMsg(&logger, "Setting whole injection phase map to %.2f", value);
	setMap(config->injectionPhase, value);
}

static void setWholeTimingMapCmd(float value) {
	scheduleMsg(&logger, "Setting whole timing advance map to %.2f", value);
	setWholeTimingMap(value);
}

static void setWholeVeCmd(float value) {
	scheduleMsg(&logger, "Setting whole VE map to %.2f", value);
	if (engineConfiguration->fuelAlgorithm != LM_SPEED_DENSITY) {
		scheduleMsg(&logger, "WARNING: setting VE map not in SD mode is pointless");
	}
	setMap(config->veTable, value);
}

static void setWholeFuelMapCmd(float value) {
	scheduleMsg(&logger, "Setting whole fuel map to %.2f", value);
	if (engineConfiguration->fuelAlgorithm == LM_SPEED_DENSITY) {
		scheduleMsg(&logger, "WARNING: setting fuel map in SD mode is pointless");
	}
	setWholeFuelMap(value PASS_ENGINE_PARAMETER_SUFFIX);
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
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
}

static void setIndividualPin(const char *pinName, brain_pin_e *targetPin, const char *name) {
	brain_pin_e pin = parseBrainPin(pinName);
	if (pin == GPIO_INVALID) {
		scheduleMsg(&logger, "invalid pin name [%s]", pinName);
		return;
	}
	scheduleMsg(&logger, "setting %s pin to %s please save&restart", name, hwPortname(pin));
	*targetPin = pin;
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
}

// set_idle_pin none
static void setIdlePin(const char *pinName) {
	setIndividualPin(pinName, &boardConfiguration->idle.solenoidPin, "idle");
}

static void setMainRelayPin(const char *pinName) {
	setIndividualPin(pinName, &boardConfiguration->mainRelayPin, "main relay");
}

static void setAlternatorPin(const char *pinName) {
	setIndividualPin(pinName, &boardConfiguration->alternatorControlPin, "alternator");
}

static void setACPin(const char *pinName) {
	setIndividualPin(pinName, &boardConfiguration->acRelayPin, "A/C");
}

static void setFuelPumpPin(const char *pinName) {
	setIndividualPin(pinName, &boardConfiguration->fuelPumpPin, "fuelPump");
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
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
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
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
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

static void setEgtCSPin(const char *indexStr, const char *pinName) {
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
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
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
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
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
		engineConfiguration->clt.adcChannel = channel;
		scheduleMsg(&logger, "setting CLT to %s/%d", pinName, channel);
	} else if (strEqual("iat", sensorStr)) {
		engineConfiguration->iat.adcChannel = channel;
		scheduleMsg(&logger, "setting IAT to %s/%d", pinName, channel);
	} else if (strEqual("tps", sensorStr)) {
		engineConfiguration->tpsAdcChannel = channel;
		scheduleMsg(&logger, "setting TPS to %s/%d", pinName, channel);
	}
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
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

	int rpmIndex = findIndexMsg("setTM", config->ignitionRpmBins, IGN_RPM_COUNT, rpm);
	rpmIndex = rpmIndex < 0 ? 0 : rpmIndex;
	int loadIndex = findIndexMsg("setTM", config->ignitionLoadBins, IGN_LOAD_COUNT, engineLoad);
	loadIndex = loadIndex < 0 ? 0 : loadIndex;

	config->ignitionTable[loadIndex][rpmIndex] = value;
	scheduleMsg(&logger, "Setting timing map entry %d:%d to %.2f", rpmIndex, loadIndex, value);
}

static void setFuelMap(const char * rpmStr, const char *loadStr, const char *valueStr) {
	float rpm = atoff(rpmStr);
	float engineLoad = atoff(loadStr);
	float value = atoff(valueStr);

	int rpmIndex = findIndexMsg("setFM", config->fuelRpmBins, FUEL_RPM_COUNT, rpm);
	rpmIndex = rpmIndex < 0 ? 0 : rpmIndex;
	int loadIndex = findIndexMsg("setTM", config->fuelLoadBins, FUEL_LOAD_COUNT, engineLoad);
	loadIndex = loadIndex < 0 ? 0 : loadIndex;

	config->fuelTable[loadIndex][rpmIndex] = value;
	scheduleMsg(&logger, "Setting fuel map entry %d:%d to %.2f", rpmIndex, loadIndex, value);
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
	} else if (strEqualCaseInsensitive(param, "etb_auto")) {
		engine->etbAutoTune = isEnabled;
	} else if (strEqualCaseInsensitive(param, "constant_dwell")) {
		engineConfiguration->useConstantDwellDuringCranking = isEnabled;
	} else if (strEqualCaseInsensitive(param, "binary_mode_console")) {
		boardConfiguration->startConsoleInBinaryMode = isEnabled;
	} else if (strEqualCaseInsensitive(param, "cj125")) {
		boardConfiguration->isCJ125Enabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "engine_sniffer")) {
		engineConfiguration->isEngineChartEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "step1limimter")) {
		boardConfiguration->enabledStep1Limiter = isEnabled;
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	} else if (strEqualCaseInsensitive(param, "auto_idle")) {
		setIdleMode(isEnabled ? IM_MANUAL : IM_AUTO);
#endif /* EFI_PROD_CODE */
	} else if (strEqualCaseInsensitive(param, "serial")) {
		boardConfiguration->useSerialPort = isEnabled;
	} else if (strEqualCaseInsensitive(param, "stepperidle")) {
		boardConfiguration->useStepperIdle = isEnabled;
	} else if (strEqualCaseInsensitive(param, "trigger_only_front")) {
		engineConfiguration->useOnlyRisingEdgeForTrigger = isEnabled;
		incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
	} else if (strEqualCaseInsensitive(param, "use_only_first_channel")) {
		engineConfiguration->trigger.useOnlyFirstChannel = isEnabled;
		incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
	} else if (strEqualCaseInsensitive(param, "two_wire_batch_injection")) {
		engineConfiguration->twoWireBatchInjection = isEnabled;
		incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
	} else if (strEqualCaseInsensitive(param, "two_wire_wasted_spark")) {
		engineConfiguration->twoWireBatchIgnition = isEnabled;
		incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
	} else if (strEqualCaseInsensitive(param, "HIP9011")) {
		boardConfiguration->isHip9011Enabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "verbose_etb")) {
		engineConfiguration->isVerboseETB = isEnabled;
	} else if (strEqualCaseInsensitive(param, "verbose_idle")) {
		engineConfiguration->isVerboseIAC = isEnabled;
	} else if (strEqualCaseInsensitive(param, "auxdebug1")) {
		engineConfiguration->isVerboseAuxPid1 = isEnabled;
	} else if (strEqualCaseInsensitive(param, "auxdebug2")) {
		engineConfiguration->isVerboseAuxPid2 = isEnabled;
	} else if (strEqualCaseInsensitive(param, "auxdebug3")) {
		engineConfiguration->isVerboseAuxPid3 = isEnabled;
	} else if (strEqualCaseInsensitive(param, "auxdebug4")) {
		engineConfiguration->isVerboseAuxPid4 = isEnabled;
	} else if (strEqualCaseInsensitive(param, "altdebug")) {
		boardConfiguration->isVerboseAlternator = isEnabled;
	} else if (strEqualCaseInsensitive(param, "tpic_advanced_mode")) {
		boardConfiguration->useTpicAdvancedMode = isEnabled;
	} else if (strEqualCaseInsensitive(param, "knockdebug")) {
		engine->knockDebug = isEnabled;
	} else if (strEqualCaseInsensitive(param, "altcontrol")) {
		engineConfiguration->isAlternatorControlEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "sd")) {
		boardConfiguration->isSdCardEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "test_mode")) {
		engine->isTestMode = isEnabled;
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
	} else if (strEqualCaseInsensitive(param, "tunerstudio")) {
		engineConfiguration->isTunerStudioEnabled = isEnabled;
	} else if (strEqualCaseInsensitive(param, "wave_analyzer")) {
		engineConfiguration->isWaveAnalyzerEnabled = isEnabled;
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

void stopEngine(void) {
	engine->stopEngineRequestTimeNt = getTimeNowNt();
	// let's close injectors or else if these happen to be open right now
	enginePins.stopPins();
}

static void printAllInfo(void) {
	printTemperatureInfo();
	printTPSInfo();
#if EFI_ENGINE_SNIFFER
	scheduleMsg(&logger, "waveChartUsedSize=%d", waveChartUsedSize);
#endif
}

typedef struct {
	const char *token;
	int *value;
} plain_get_integer_s;

typedef struct {
	const char *token;
	uint16_t *value;
} plain_get_short_s;

typedef struct {
	const char *token;
	float *value;
} plain_get_float_s;


#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
plain_get_short_s getS_plain[] = {
		{"idle_pid_min", (uint16_t *)&engineConfiguration->idleRpmPid.minValue},
		{"idle_pid_max", (uint16_t *)&engineConfiguration->idleRpmPid.maxValue},
};

plain_get_integer_s getI_plain[] = {
//		{"cranking_rpm", &engineConfiguration->cranking.rpm},
//		{"cranking_injection_mode", setCrankingInjectionMode},
//		{"injection_mode", setInjectionMode},
//		{"sensor_chart_mode", setSensorChartMode},
//		{"tpsErrorDetectionTooLow", setTpsErrorDetectionTooLow},
//		{"tpsErrorDetectionTooHigh", setTpsErrorDetectionTooHigh},
//		{"fixed_mode_timing", setFixedModeTiming},
//		{"timing_mode", setTimingMode},
//		{"engine_type", setEngineType},
		{"warning_period", (int*)&engineConfiguration->warningPeriod},
		{"hard_limit", &engineConfiguration->rpmHardLimit},
//		{"firing_order", setFiringOrder},
//		{"algorithm", setAlgorithmInt},
//		{"injection_pin_mode", setInjectionPinMode},
//		{"ignition_pin_mode", setIgnitionPinMode},
//		{"idle_pin_mode", setIdlePinMode},
//		{"fuel_pump_pin_mode", setFuelPumpPinMode},
//		{"malfunction_indicator_pin_mode", setMalfunctionIndicatorPinMode},
//		{"operation_mode", setOM},
		{"debug_mode", (int*)&engineConfiguration->debugMode},
		{"cranking_iac", &engineConfiguration->crankingIACposition},
		{"trigger_type", (int*)&engineConfiguration->trigger.type},
//		{"idle_solenoid_freq", setIdleSolenoidFrequency},
//		{"tps_accel_len", setTpsAccelLen},
//		{"engine_load_accel_len", setEngineLoadAccelLen},
//		{"bor", setBor},
//		{"can_mode", setCanType},
//		{"idle_rpm", setTargetIdleRpm},
//		{"idle_dt", setIdleDT},
		//		{"", },
		//		{"", },
		//		{"", },
		//		{"", },
		//		{"", },
};

plain_get_float_s getF_plain[] = {
		{"cranking_dwell", &engineConfiguration->ignitionDwellForCrankingMs},
		{"idle_position", &boardConfiguration->manIdlePosition},
		{"ignition_offset", &engineConfiguration->ignitionOffset},
		{"injection_offset", &engineConfiguration->extraInjectionOffset},
		{"global_trigger_offset_angle", &engineConfiguration->globalTriggerAngleOffset},
		{"global_fuel_correction", &engineConfiguration->globalFuelCorrection},
		{"cranking_fuel", &engineConfiguration->cranking.baseFuel},
		{"cranking_timing_angle", &engineConfiguration->crankingTimingAngle},
		{"cranking_charge_angle", &engineConfiguration->crankingChargeAngle},
};
#endif /* EFI_UNIT_TEST */


static void getValue(const char *paramStr) {
#if ! EFI_UNIT_TEST || defined(__DOXYGEN__)
	{
		plain_get_integer_s *currentI = &getI_plain[0];
		while (currentI < getI_plain + sizeof(getI_plain)/sizeof(getI_plain[0])) {
			if (strEqualCaseInsensitive(paramStr, currentI->token)) {
				scheduleMsg(&logger, "%s value: %d", currentI->token, *currentI->value);
				return;
			}
			currentI++;
		}
	}

	plain_get_float_s *currentF = &getF_plain[0];
	while (currentF < getF_plain + sizeof(getF_plain)/sizeof(getF_plain[0])) {
		if (strEqualCaseInsensitive(paramStr, currentF->token)) {
			float value = *currentF->value;
			scheduleMsg(&logger, "%s value: %.2f", currentF->token, value);
			return;
		}
		currentF++;
	}


#endif /* EFI_UNIT_TEST */


	if (strEqualCaseInsensitive(paramStr, "isCJ125Enabled")) {
		scheduleMsg(&logger, "isCJ125Enabled=%d", boardConfiguration->isCJ125Enabled);
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	} else if (strEqualCaseInsensitive(paramStr, "bor")) {
		showBor();
#endif /* EFI_PROD_CODE */
	} else if (strEqualCaseInsensitive(paramStr, "nb_vvt_index")) {
		scheduleMsg(&logger, "nb_vvt_index=%d", engineConfiguration->nbVvtIndex);
	} else if (strEqualCaseInsensitive(paramStr, "global_trigger_offset_angle")) {
		scheduleMsg(&logger, "global_trigger_offset=%.2f", engineConfiguration->globalTriggerAngleOffset);
	} else if (strEqualCaseInsensitive(paramStr, "isHip9011Enabled")) {
		scheduleMsg(&logger, "isHip9011Enabled=%d", boardConfiguration->isHip9011Enabled);
	}

#if EFI_RTC || defined(__DOXYGEN__)
	else if (strEqualCaseInsensitive(paramStr, "date")) {
		printDateTime();
	}
#endif
	else {
		scheduleMsg(&logger, "unexpcted here: %s", paramStr);
	}
}

static void setFsioCurve1Value(float value) {
	setLinearCurve(engineConfiguration->fsioCurve1, FSIO_CURVE_16, value, value, 1);
}

static void setFsioCurve2Value(float value) {
	setLinearCurve(engineConfiguration->fsioCurve2, FSIO_CURVE_16, value, value, 1);
}

typedef struct {
	const char *token;
	VoidInt callback;
} command_i_s;

typedef struct {
	const char *token;
	VoidFloat callback;
} command_f_s;

command_f_s commandsF[] = {{"mock_iat_voltage", setMockIatVoltage},
		{"mock_pedal_position", setMockPedalPosition},
		{"mock_maf_voltage", setMockMafVoltage},
		{"mock_afr_voltage", setMockAfrVoltage},
		{"mock_tps_voltage", setMockTpsVoltage},
		{"mock_map_voltage", setMockMapVoltage},
		{"mock_vbatt_voltage", setMockVBattVoltage},
		{"mock_clt_voltage", setMockCltVoltage},
		{"fsio_curve_1_value", setFsioCurve1Value},
		{"fsio_curve_2_value", setFsioCurve2Value},
		{"ignition_offset", setIgnitionOffset},
		{"injection_offset", setInjectionOffset},
		{"global_trigger_offset_angle", setGlobalTriggerAngleOffset},
		{"global_fuel_correction", setGlobalFuelCorrection},
		{"cranking_fuel", setCrankingFuel},
		{"cranking_iac", setCrankingIACExtra},
		{"cranking_timing_angle", setCrankingTimingAngle},
		{"cranking_charge_angle", setCrankingChargeAngle},
		{"vbatt_divider", setVBattDivider},
		{"clt_bias", setCltBias},
		{"iat_bias", setIatBias},
		{"tps_accel_threshold", setTpsAccelThr},
		{"tps_decel_threshold", setTpsDecelThr},
		{"tps_decel_multiplier", setTpsDecelMult},
		{"cranking_priming_pulse", setCrankingPrimingPulse},
		{"engine_load_accel_threshold", setEngineLoadAccelThr},
		{"engine_load_accel_multiplier", setEngineLoadAccelMult},
		{"engine_decel_threshold", setDecelThr},
		{"engine_decel_multiplier", setDecelMult},
		{"flat_injector_lag", setFlatInjectorLag},
#if EFI_PROD_CODE || defined(__DOXYGEN__)
		{"mock_vehicle_speed", setMockVehicleSpeed},
		{"idle_offset", setIdleOffset},
		{"idle_p", setIdlePFactor},
		{"idle_i", setIdleIFactor},
		{"idle_d", setIdleDFactor},
		{"etb_p", setEtbPFactor},
		{"etb_i", setEtbIFactor},
#endif /* EFI_PROD_CODE */

		//		{"", },
//		{"", },
//		{"", },
		//		{"", },
		//		{"", },
		//		{"", },
};

static void setTpsErrorDetectionTooLow(int v) {
	engineConfiguration->tpsErrorDetectionTooLow = v;
}

static void setTpsErrorDetectionTooHigh(int v) {
	engineConfiguration->tpsErrorDetectionTooHigh = v;
}

command_i_s commandsI[] = {{"ignition_mode", setIgnitionMode},
		{"call_from_pitstop", setCallFromPitStop},
		{"cranking_rpm", setCrankingRpm},
		{"cranking_injection_mode", setCrankingInjectionMode},
		{"injection_mode", setInjectionMode},
		{"sensor_chart_mode", setSensorChartMode},
		{"tpsErrorDetectionTooLow", setTpsErrorDetectionTooLow},
		{"tpsErrorDetectionTooHigh", setTpsErrorDetectionTooHigh},
		{"fixed_mode_timing", setFixedModeTiming},
		{"timing_mode", setTimingMode},
		{"engine_type", setEngineType},
		{"rpm_hard_limit", setRpmHardLimit},
		{"firing_order", setFiringOrder},
		{"algorithm", setAlgorithmInt},
		{"injection_pin_mode", setInjectionPinMode},
		{"ignition_pin_mode", setIgnitionPinMode},
		{"idle_pin_mode", setIdlePinMode},
		{"fuel_pump_pin_mode", setFuelPumpPinMode},
		{"malfunction_indicator_pin_mode", setMalfunctionIndicatorPinMode},
		{"operation_mode", setOM},
		{"debug_mode", setDebugMode},
		{"trigger_type", setTriggerType},
		{"idle_solenoid_freq", setIdleSolenoidFrequency},
		{"tps_accel_len", setTpsAccelLen},
		{"engine_load_accel_len", setEngineLoadAccelLen},
#if EFI_PROD_CODE || defined(__DOXYGEN__)
		{"bor", setBor},
		{"can_mode", setCanType},
		{"idle_position", setIdleValvePosition},
		{"idle_rpm", setTargetIdleRpm},
		{"idle_dt", setIdleDT},
#endif /* EFI_PROD_CODE */
		//		{"", },
		//		{"", },
		//		{"", },
		//		{"", },
		//		{"", },
};

static void setValue(const char *paramStr, const char *valueStr) {
	float valueF = atoff(valueStr);
	int valueI = atoi(valueStr);

	command_f_s *currentF = &commandsF[0];
	while (currentF < commandsF + sizeof(commandsF)/sizeof(commandsF[0])) {
		if (strEqualCaseInsensitive(paramStr, currentF->token)) {
			currentF->callback(valueF);
			return;
		}
		currentF++;
	}

	command_i_s *currentI = &commandsI[0];
	while (currentI < commandsI + sizeof(commandsI)/sizeof(commandsI[0])) {
		if (strEqualCaseInsensitive(paramStr, currentI->token)) {
			currentI->callback(valueI);
			return;
		}
		currentI++;
	}


	if (strEqualCaseInsensitive(paramStr, "vsscoeff")) {
		engineConfiguration->vehicleSpeedCoef = valueF;
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	} else if (strEqualCaseInsensitive(paramStr, "alt_t")) {
		if (valueI > 10) {
			engineConfiguration->alternatorControl.period = valueI;
		}
		showAltInfo();
	} else if (strEqualCaseInsensitive(paramStr, "alt_offset")) {
		engineConfiguration->alternatorControl.offset = valueI;
	} else if (strEqualCaseInsensitive(paramStr, "alt_p")) {
		setAltPFactor(valueF);
#endif
//	} else if (strEqualCaseInsensitive(paramStr, "cranking_rpm")) {
//	} else if (strEqualCaseInsensitive(paramStr, "cranking_rpm")) {
//	} else if (strEqualCaseInsensitive(paramStr, "cranking_rpm")) {
//	} else if (strEqualCaseInsensitive(paramStr, "cranking_rpm")) {
//	} else if (strEqualCaseInsensitive(paramStr, "cranking_rpm")) {
//	} else if (strEqualCaseInsensitive(paramStr, "cranking_rpm")) {
	} else if (strEqualCaseInsensitive(paramStr, "warning_period")) {
		engineConfiguration->warningPeriod = valueI;
	} else if (strEqualCaseInsensitive(paramStr, "dwell")) {
		setConstantDwell(valueF PASS_ENGINE_PARAMETER_SUFFIX);
	} else if (strEqualCaseInsensitive(paramStr, "engineSnifferRpmThreshold")) {
		engineConfiguration->engineSnifferRpmThreshold = valueI;
	} else if (strEqualCaseInsensitive(paramStr, "step1rpm")) {
		engineConfiguration->step1rpm = valueI;
	} else if (strEqualCaseInsensitive(paramStr, "vvt_offset")) {
		engineConfiguration->vvtOffset = valueF;
	} else if (strEqualCaseInsensitive(paramStr, "vvt_mode")) {
		engineConfiguration->vvtMode = (vvt_mode_e)valueI;
	} else if (strEqualCaseInsensitive(paramStr, "nb_vvt_index")) {
		engineConfiguration->nbVvtIndex = valueI;
	} else if (strEqualCaseInsensitive(paramStr, "step1timing")) {
		engineConfiguration->step1timing = valueI;
	} else if (strEqualCaseInsensitive(paramStr, "operation_mode")) {
		engineConfiguration->operationMode = (operation_mode_e)valueI;
	} else if (strEqualCaseInsensitive(paramStr, "suckedOffCoef")) {
		engineConfiguration->suckedOffCoef = valueF;
	} else if (strEqualCaseInsensitive(paramStr, "addedToWallCoef")) {
		engineConfiguration->addedToWallCoef = valueF;
	} else if (strEqualCaseInsensitive(paramStr, "cranking_dwell")) {
		engineConfiguration->ignitionDwellForCrankingMs = valueF;
	} else if (strEqualCaseInsensitive(paramStr, "targetvbatt")) {
		engineConfiguration->targetVBatt = valueF;
#if EFI_RTC || defined(__DOXYGEN__)
	} else if (strEqualCaseInsensitive(paramStr, "date")) {
		// rusEfi console invokes this method with timestamp in local timezone
		setDateTime(valueStr);
#endif
	}
}

void initSettings(void) {
#if EFI_SIMULATOR
	printf("initSettings\n");
#endif

	// todo: start saving values into flash right away?

	addConsoleActionP("showconfig", (VoidPtr) doPrintConfiguration, &engine);
	addConsoleAction("tempinfo", printTemperatureInfo);
	addConsoleAction("tpsinfo", printTPSInfo);
	addConsoleAction("info", printAllInfo);

	addConsoleAction("set_one_coil_ignition", setOneCoilIgnition);
	addConsoleAction("set_wasted_spark_ignition", setWastedIgnition);
	addConsoleAction("set_individual_coils_ignition", setIndividualCoilsIgnition);

	addConsoleActionF("set_whole_phase_map", setWholePhaseMapCmd);
	addConsoleActionF("set_whole_timing_map", setWholeTimingMapCmd);
	addConsoleActionF("set_whole_fuel_map", setWholeFuelMapCmd);
	addConsoleActionF("set_whole_ve_map", setWholeVeCmd);
	addConsoleActionF("set_whole_ign_corr_map", setWholeIgnitionIatCorr);
	addConsoleActionSSS("set_fuel_map", setFuelMap);

	addConsoleActionSSS("set_timing_map", setTimingMap);

	addConsoleAction("stopengine", (Void) stopEngine);

	// todo: refactor this - looks like all boolean flags should be controlled with less code duplication
	addConsoleActionI("enable_spi", enableSpi);
	addConsoleActionI("disable_spi", disableSpi);

	addConsoleActionS("enable", enable);
	addConsoleActionS("disable", disable);

	addConsoleActionII("set_toothed_wheel", setToothedWheel);


	// flat curve - constant injector lag regardless of voltage
	addConsoleActionF("set_flat_injector_lag", setFlatInjectorLag);

	addConsoleActionFF("set_injector_lag", setInjectorLag);

	addConsoleActionFF("set_fan", setFanSetting);

	addConsoleActionSS("set", setValue);
	addConsoleActionS("get", getValue);

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	addConsoleActionS("showpin", showPinFunction);
	addConsoleActionSS("set_injection_pin", setInjectionPin);
	addConsoleActionSS("set_ignition_pin", setIgnitionPin);
	addConsoleActionSS("set_trigger_input_pin", setTriggerInputPin);
	addConsoleActionSS("set_trigger_simulator_pin", setTriggerSimulatorPin);

	addConsoleActionSS("set_egt_cs_pin", (VoidCharPtrCharPtr) setEgtCSPin);
	addConsoleActionI("set_egt_spi", setEgtSpi);

	addConsoleActionSS("set_trigger_simulator_mode", setTriggerSimulatorMode);
	addConsoleActionS("set_fuel_pump_pin", setFuelPumpPin);
	addConsoleActionS("set_ac_pin", setACPin);
	addConsoleActionS("set_alternator_pin", setAlternatorPin);
	addConsoleActionS("set_idle_pin", setIdlePin);
	addConsoleActionS("set_main_relay_pin", setMainRelayPin);

#if HAL_USE_ADC || defined(__DOXYGEN__)
	addConsoleActionSS("set_analog_input_pin", setAnalogInputPin);
#endif
	addConsoleActionSS("set_logic_input_pin", setLogicInputPin);
	addConsoleActionI("set_pot_spi", setPotSpi);
#endif /* EFI_PROD_CODE */
}

