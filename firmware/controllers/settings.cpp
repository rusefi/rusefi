/**
 * @file settings.c
 * @brief This file is about configuring engine via the human-readable protocol
 *
 * @date Dec 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "settings.h"

#include "eficonsole.h"
#include "engine_configuration.h"
#include "flash_main.h"
#include "adc_inputs.h"
#include "engine_controller.h"
#include "thermistors.h"
#include "adc_inputs.h"
#include "interpolation.h"
#include "tps.h"
#include "ec2.h"

#if EFI_PROD_CODE
#include "rusefi.h"
#include "pin_repository.h"
#endif /* EFI_PROD_CODE */

static Logging logger;

static char LOGGING_BUFFER[1000];

extern engine_configuration_s *engineConfiguration;
extern engine_configuration2_s *engineConfiguration2;
extern board_configuration_s *boardConfiguration;

static void doPrintConfiguration(void) {
	printConfiguration(engineConfiguration, engineConfiguration2);
}

static void printIntArray(int array[], int size) {
	for (int j = 0; j < size; j++)
		print("%d ", array[j]);
	print("\r\n");
}

void printFloatArray(const char *prefix, float array[], int size) {
	appendMsgPrefix(&logger);
	appendPrintf(&logger, prefix);
	for (int j = 0; j < size; j++)
		appendPrintf(&logger, "%f ", array[j]);
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
	case HONDA_ACCORD:
		return "Honda Accord";
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
	default:
		return NULL;
	}
}

/**
 * @brief	Prints current engine configuration to human-readable console.
 */
void printConfiguration(engine_configuration_s *engineConfiguration, engine_configuration2_s *engineConfiguration2) {

	scheduleMsg(&logger, getConfigurationName(engineConfiguration));

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

	scheduleMsg(&logger, "rpmHardLimit: %d", engineConfiguration->rpmHardLimit);
	scheduleMsg(&logger, "rpmMultiplier=%f", engineConfiguration->rpmMultiplier);

	scheduleMsg(&logger, "tpsMin: %d", engineConfiguration->tpsMin);
	scheduleMsg(&logger, "tpsMax: %d", engineConfiguration->tpsMax);

	scheduleMsg(&logger, "timingMode: %d", engineConfiguration->timingMode);
	scheduleMsg(&logger, "fixedModeTiming: %d", (int) engineConfiguration->fixedModeTiming);
	scheduleMsg(&logger, "ignitionOffset=%f", engineConfiguration->ignitionOffset);
	scheduleMsg(&logger, "injectionOffset=%f", engineConfiguration->injectionOffset);

	scheduleMsg(&logger, "crankingChargeAngle=%f", engineConfiguration->crankingChargeAngle);
	scheduleMsg(&logger, "crankingTimingAngle=%f", engineConfiguration->crankingTimingAngle);
	scheduleMsg(&logger, "globalTriggerAngleOffset=%f", engineConfiguration->globalTriggerAngleOffset);

//	scheduleMsg(&logger, "analogChartMode: %d", engineConfiguration->analogChartMode);

//	scheduleMsg(&logger, "crankingRpm: %d", engineConfiguration->crankingSettings.crankingRpm);

	scheduleMsg(&logger, "idlePinMode: %d", boardConfiguration->idleValvePinMode);
	scheduleMsg(&logger, "malfunctionIndicatorPinMode: %d", boardConfiguration->malfunctionIndicatorPinMode);
	scheduleMsg(&logger, "analogInputDividerCoefficient: %f", engineConfiguration->analogInputDividerCoefficient);

	scheduleMsg(&logger, "needSecondTriggerInput: %d", engineConfiguration->needSecondTriggerInput);



#if EFI_PROD_CODE
	scheduleMsg(&logger, "idleValvePin: %s", hwPortname(boardConfiguration->idleValvePin));
	scheduleMsg(&logger, "fuelPumpPin: mode %d @ %s", boardConfiguration->fuelPumpPinMode, hwPortname(boardConfiguration->fuelPumpPin));


	scheduleMsg(&logger, "injectionPins: mode %d", boardConfiguration->injectionPinMode);
	for (int i = 0; i < engineConfiguration->cylindersCount; i++) {
		brain_pin_e brainPin = boardConfiguration->injectionPins[i];

		scheduleMsg(&logger, "injection %d @ %s", i, hwPortname(brainPin));
	}

	scheduleMsg(&logger, "ignitionPins: mode %d", boardConfiguration->ignitionPinMode);
	// todo: calculate coils count based on ignition mode
	for (int i = 0; i < 4; i++) {
		brain_pin_e brainPin = boardConfiguration->ignitionPins[i];
		scheduleMsg(&logger, "ignition %d @ %s", i, hwPortname(brainPin));
	}

	scheduleMsg(&logger, "primary trigger simulator: %s %d", hwPortname(boardConfiguration->triggerSimulatorPins[0]), boardConfiguration->triggerSimulatorPinModes[0]);
	scheduleMsg(&logger, "secondary trigger simulator: %s %d", hwPortname(boardConfiguration->triggerSimulatorPins[1]), boardConfiguration->triggerSimulatorPinModes[1]);
#endif /* EFI_PROD_CODE */

	scheduleMsg(&logger, "isInjectionEnabledFlag %d", engineConfiguration2->isInjectionEnabledFlag);

	//	appendPrintf(&logger, DELIMETER);
//	scheduleLogging(&logger);
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

static void setEngineType(int value) {
	engineConfiguration->engineType = (engine_type_e) value;
	resetConfigurationExt((engine_type_e) value, engineConfiguration, engineConfiguration2, boardConfiguration);
#if EFI_PROD_CODE
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

static uint8_t pinNameBuffer[16];

static void printThermistor(char *msg, Thermistor *thermistor) {
	int adcChannel = thermistor->channel;
	float voltage = getVoltageDivided(adcChannel);
	float r = getResistance(thermistor);

	float t = getTemperatureC(thermistor);

	scheduleMsg(&logger, "%s v=%f C=%f R=%f on channel %d", msg, voltage, t, r, adcChannel);
	scheduleMsg(&logger, "bias=%f A=%f B=%f C=%f", thermistor->config->bias_resistor, thermistor->config->s_h_a,
			thermistor->config->s_h_b, thermistor->config->s_h_c);
#if EFI_PROD_CODE
	scheduleMsg(&logger, "@%s", getPinNameByAdcChannel(adcChannel, pinNameBuffer));
#endif
}

static void printTPSInfo(void) {
#if EFI_PROD_CODE
	GPIO_TypeDef* port = getAdcChannelPort(engineConfiguration->tpsAdcChannel);
	int pin = getAdcChannelPin(engineConfiguration->tpsAdcChannel);

	scheduleMsg(&logger, "tps min %d/max %d v=%f @%s%d", engineConfiguration->tpsMin, engineConfiguration->tpsMax, getTPSVoltage(), portname(port), pin);
#endif
	scheduleMsg(&logger, "current 10bit=%d value=%f rate=%f", getTPS10bitAdc(), getTPS(), getTpsRateOfChange());
}

static void printTemperatureInfo(void) {
	printThermistor("CLT", &engineConfiguration2->clt);
	printThermistor("IAT", &engineConfiguration2->iat);

	float rClt = getResistance(&engineConfiguration2->clt);
	float rIat = getResistance(&engineConfiguration2->iat);

#if EFI_PROD_CODE
	int cltChannel = engineConfiguration2->clt.channel;
	scheduleMsg(&logger, "CLT R=%f on channel %d@%s", rClt, cltChannel,
			getPinNameByAdcChannel(cltChannel, pinNameBuffer));
	int iatChannel = engineConfiguration2->iat.channel;
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

static void setFiringOrder(int value) {
	engineConfiguration->firingOrder = (firing_order_e)value;
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
	engineConfiguration->crankingInjectionMode = (injection_mode_e)value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration();
}

static void setInjectionMode(int value) {
	engineConfiguration->injectionMode = (injection_mode_e)value;
	incrementGlobalConfigurationVersion();
	doPrintConfiguration();
}

static void setIgnitionMode(int value) {
	engineConfiguration->ignitionMode = (ignition_mode_e)value;
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

static void setWholeTimingMap(float value) {
	scheduleMsg(&logger, "Setting whole timing map to %f", value);
	for (int l = 0; l < IGN_LOAD_COUNT; l++) {
		for (int r = 0; r < IGN_RPM_COUNT; r++) {
			engineConfiguration->ignitionTable[l][r] = value;
		}
	}
}

static void setWholeFuelMapCmd(float value) {
	scheduleMsg(&logger, "Setting whole fuel map to %f", value);
	setWholeFuelMap(engineConfiguration, value);
}

static void setTimingMap(char * rpmStr, char *loadStr, char *valueStr) {
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

static void setFuelMap(char * rpmStr, char *loadStr, char *valueStr) {
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

static void enableInjection(void) {
	engineConfiguration2->isInjectionEnabledFlag = TRUE;
	scheduleMsg(&logger, "injection enabled");
}

static void disableInjection(void) {
	engineConfiguration2->isInjectionEnabledFlag = FALSE;
	scheduleMsg(&logger, "injection disabled");
}

void initSettings(void) {
	initLoggingExt(&logger, "settings control", LOGGING_BUFFER, sizeof(LOGGING_BUFFER));

	addConsoleAction("showconfig", doPrintConfiguration);
	addConsoleAction("tempinfo", printTemperatureInfo);
	addConsoleAction("tpsinfo", printTPSInfo);

	addConsoleActionI("set_ignition_offset", setIgnitionOffset);
	addConsoleActionI("set_injection_offset", setInjectionOffset);
	addConsoleActionI("set_global_trigger_offset_angle", setGlobalTriggerAngleOffset);
	addConsoleActionI("set_analog_chart_mode", setAnalogChartMode);
	addConsoleActionI("set_fixed_mode_timing", setFixedModeTiming);
	addConsoleActionI("set_timing_mode", setTimingMode);
	addConsoleActionI("set_engine_type", setEngineType);

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

	addConsoleAction("enable_injection", enableInjection);
	addConsoleAction("disable_injection", disableInjection);
}

