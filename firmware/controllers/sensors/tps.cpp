/**
 * @author Andrey Belomutskiy, (c) 2012-2020
 */
#include "engine.h"
#include "tps.h"
#include "interpolation.h"
#include "adc_inputs.h"
#if EFI_PROD_CODE
#include "settings.h"
#endif /* EFI_PROD_CODE */

EXTERN_ENGINE;

/*
 * Return current TPS position based on configured ADC levels, and adc
 *
 * */
static percent_t getTpsValue(int index, float adc DECLARE_ENGINE_PARAMETER_SUFFIX) {

	DISPLAY_STATE(Engine)
	DISPLAY_TAG(TPS_SECTION);
	DISPLAY_SENSOR(TPS)
	DISPLAY_TEXT(EOL)


	DISPLAY_TEXT(Analog_MCU_reads);
	engine->engineState.currentTpsAdc = adc;
#if !EFI_UNIT_TEST
	engine->engineState.DISPLAY_FIELD(tpsVoltageMCU) = adcToVolts(adc);
#endif
	DISPLAY_TEXT(Volts);
	DISPLAY_TEXT(from_pin) DISPLAY(DISPLAY_CONFIG(tps1_1AdcChannel))
	DISPLAY_TEXT(EOL);

	DISPLAY_TEXT(Analog_ECU_reads);
	engine->engineState.DISPLAY_FIELD(tpsVoltageBoard) =
	DISPLAY_TEXT(Rdivider) engine->engineState.tpsVoltageMCU * CONFIG(DISPLAY_CONFIG(analogInputDividerCoefficient));
	DISPLAY_TEXT(EOL);


	if (engineConfiguration->tpsMin == engineConfiguration->tpsMax) {
		warning(CUSTOM_INVALID_TPS_SETTING, "Invalid TPS configuration: same value %d", engineConfiguration->tpsMin);
		return NAN;
	}

	DISPLAY_TEXT(Current_ADC)
	DISPLAY(DISPLAY_FIELD(currentTpsAdc))
	DISPLAY_TEXT(interpolate_between)

	DISPLAY(DISPLAY_CONFIG(tpsMax))
	DISPLAY_TEXT(and)
	DISPLAY(DISPLAY_CONFIG(tpsMin))

	int tpsMax = index == 0 ? CONFIG(tpsMax) : CONFIG(tps2Max);
	int tpsMin = index == 0 ? CONFIG(tpsMin) : CONFIG(tps2Min);

	const char *msg = index == 0 ? "TPS1" : "TPS2";
	float result = interpolateMsg(msg, tpsMax, 100, tpsMin, 0, adc);
	if (result < engineConfiguration->tpsErrorDetectionTooLow) {
#if EFI_PROD_CODE
		// too much noise with simulator
		warning(OBD_Throttle_Position_Sensor_Circuit_Malfunction, "TPS too low: %.2f", result);
#endif /* EFI_PROD_CODE */
	}
	if (result > engineConfiguration->tpsErrorDetectionTooHigh) {
#if EFI_PROD_CODE
		// too much noise with simulator
		warning(OBD_Throttle_Position_Sensor_Range_Performance_Problem, "TPS too high: %.2f", result);
#endif /* EFI_PROD_CODE */
	}

	// this would put the value into the 0-100 range
	return maxF(0, minF(100, result));
}

/*
 * Return TPS ADC readings.
 * We need ADC value because TunerStudio has a nice TPS configuration wizard, and this wizard
 * wants a TPS value.
 * @param index [0, ETB_COUNT)
 */
static float getTPS10bitAdc(int index DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (engineConfiguration->tps1_1AdcChannel == EFI_ADC_NONE)
		return -1;
#if EFI_PROD_CODE

	if (index == 0) {
		return convertVoltageTo10bitADC(getVoltageDivided("tps10", engineConfiguration->tps1_1AdcChannel));
	} else {
		return convertVoltageTo10bitADC(getVoltageDivided("tps20", engineConfiguration->tps2_1AdcChannel));
	}
	//	return tpsFastAdc / 4;
#else
	return 0;
#endif /* EFI_PROD_CODE */
}

void grabTPSIsClosed() {
#if EFI_PROD_CODE
	printTPSInfo();
	engineConfiguration->tpsMin = getTPS10bitAdc(0);
	printTPSInfo();
#endif /* EFI_PROD_CODE */
}

void grabTPSIsWideOpen() {
#if EFI_PROD_CODE
	printTPSInfo();
	engineConfiguration->tpsMax = getTPS10bitAdc(0);
	printTPSInfo();
#endif /* EFI_PROD_CODE */
}

void grabPedalIsUp() {
#if EFI_PROD_CODE
	float voltage = getVoltageDivided("pPS", engineConfiguration->throttlePedalPositionAdcChannel PASS_ENGINE_PARAMETER_SUFFIX);
	engineConfiguration->throttlePedalUpVoltage = voltage;
	printTPSInfo();
#endif /* EFI_PROD_CODE */
}

void grabPedalIsWideOpen() {
#if EFI_PROD_CODE
	float voltage = getVoltageDivided("pPS", engineConfiguration->throttlePedalPositionAdcChannel PASS_ENGINE_PARAMETER_SUFFIX);
	engineConfiguration->throttlePedalWOTVoltage = voltage;
	printTPSInfo();
#endif /* EFI_PROD_CODE */
}

/**
 * @brief Position on physical primary TPS
 */
static percent_t getPrimaryRawTPS(int index DECLARE_ENGINE_PARAMETER_SUFFIX) {
	float adcValue = getTPS10bitAdc(index PASS_ENGINE_PARAMETER_SUFFIX);
	percent_t tpsValue = getTpsValue(index, adcValue PASS_ENGINE_PARAMETER_SUFFIX);
	return tpsValue;
}

#define NO_TPS_MAGIC_VALUE 66.611

static bool hasTpsSensor(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	return engineConfiguration->tps1_1AdcChannel != EFI_ADC_NONE;
}

percent_t getTPS(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (!hasTpsSensor(PASS_ENGINE_PARAMETER_SIGNATURE))
		return NO_TPS_MAGIC_VALUE;
	// todo: if (config->isDualTps)
	// todo: blah blah
	// todo: if two TPS do not match - show OBD code via malfunction_central.c

	return getPrimaryRawTPS(0 PASS_ENGINE_PARAMETER_SUFFIX);
}
